/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include "tuya_ble_cfg.h"
#include "tuya_ble_hci.h"
#include "ble_hs_priv.h"

#if (TY_HS_BLE_HS_FLOW_CTRL)

#define BLE_HS_FLOW_ITVL_TICKS  \
    tuya_ble_time_ms_to_ticks32((TY_HS_BLE_HS_FLOW_CTRL_ITVL))

/**
 * The number of freed buffers since the most-recent
 * number-of-completed-packets event was sent.  This is used to determine if an
 * immediate event transmission is required.
 */
static uint16_t ble_hs_flow_num_completed_pkts;

/** Periodically sends number-of-completed-packets events.  */
static struct tuya_ble_callout ble_hs_flow_timer;

static tuya_ble_event_fn ble_hs_flow_event_cb;

static struct tuya_ble_event ble_hs_flow_ev;

/* Connection handle associated with each mbuf in ACL pool */
static uint16_t ble_hs_flow_mbuf_conn_handle[ (TUYA_BLE_ACL_BUF_COUNT) ];
//tuya add
static uintptr_t ble_hs_flow_mbuf_conn_index[ (TUYA_BLE_ACL_BUF_COUNT) ];

static inline int
ble_hs_flow_mbuf_index(const struct os_mbuf *om)
{
#if defined(TUYA_USE_DYNA_RAM) && (TUYA_USE_DYNA_RAM==0)
    const struct os_mempool *mp = om->om_omp->omp_pool;
    uintptr_t addr = (uintptr_t)om;
    int idx;

    idx = (addr - mp->mp_membuf_addr) / mp->mp_block_size;

    BLE_HS_DBG_ASSERT(mp->mp_membuf_addr + idx * mp->mp_block_size == addr);

    return idx;
#else
    for (size_t i = 0; i < sizeof(ble_hs_flow_mbuf_conn_index) / sizeof(uintptr_t); i++) {
        if ((uintptr_t *)ble_hs_flow_mbuf_conn_index[i] == (uintptr_t *)om) {
            // PR_ERR("GET I %d %d,  %p",sizeof(ble_hs_flow_mbuf_conn_index) / sizeof(uintptr_t), i, om);
            return i;
        }
    }
    PR_ERR("NOT FOUND MBUF INDEX ");
    BLE_HS_DBG_ASSERT(0);
#endif
}

static int
ble_hs_flow_tx_num_comp_pkts(void)
{
    uint8_t buf[
        sizeof(struct ble_hci_cb_host_num_comp_pkts_cp) +
        sizeof(struct ble_hci_cb_host_num_comp_pkts_entry)
    ];
    struct ble_hci_cb_host_num_comp_pkts_cp *cmd = (void *) buf;
    struct ble_hs_conn *conn;
    int rc;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    /* For each connection with completed packets, send a separate
     * host-number-of-completed-packets command.
     */
    for (conn = ble_hs_conn_first();
         conn != NULL;
         conn = SLIST_NEXT(conn, bhc_next)) {

        if (conn->bhc_completed_pkts > 0) {
            /* Only specify one connection per command. */
            /* TODO could combine this in single HCI command */
            cmd->handles = 1;

            /* Append entry for this connection. */
            cmd->h[0].handle = htole16(conn->bhc_handle);
            cmd->h[0].count = htole16(conn->bhc_completed_pkts);

            conn->bhc_completed_pkts = 0;

            /* The host-number-of-completed-packets command does not elicit a
             * response from the controller, so don't use the normal blocking
             * HCI API when sending it.
             */
            rc = ble_hs_hci_cmd_send_buf(
                BLE_HCI_OP(BLE_HCI_OGF_CTLR_BASEBAND,
                           BLE_HCI_OCF_CB_HOST_NUM_COMP_PKTS),
                buf, sizeof(buf));
            if (rc != 0) {
                return rc;
            }
        }
    }

    return 0;
}

static void
ble_hs_flow_event_cb(struct tuya_ble_event *ev)
{
    int rc;

    ble_hs_lock();

    if (ble_hs_flow_num_completed_pkts > 0) {
        rc = ble_hs_flow_tx_num_comp_pkts();
        if (rc != 0) {
            ble_hs_sched_reset(rc);
        }

        ble_hs_flow_num_completed_pkts = 0;
    }

    ble_hs_unlock();
}

static void
ble_hs_flow_inc_completed_pkts(struct ble_hs_conn *conn)
{
    uint16_t num_free;

    int rc;

    BLE_HS_DBG_ASSERT(ble_hs_locked_by_cur_task());

    conn->bhc_completed_pkts++;
    ble_hs_flow_num_completed_pkts++;

    if (ble_hs_flow_num_completed_pkts > (TUYA_BLE_ACL_BUF_COUNT)) {
        ble_hs_sched_reset(BLE_HS_ECONTROLLER);
        return;
    }

    /* If the number of free buffers is at or below the configured threshold,
     * send an immediate number-of-copmleted-packets event.
     */
    num_free = (TUYA_BLE_ACL_BUF_COUNT) - ble_hs_flow_num_completed_pkts;
    if (num_free <= (TY_HS_BLE_HS_FLOW_CTRL_THRESH)) {
        tuya_ble_eventq_put(ble_hs_evq_get(), &ble_hs_flow_ev);
        tuya_ble_callout_stop(&ble_hs_flow_timer);
    } else if (ble_hs_flow_num_completed_pkts == 1) {
        rc = tuya_ble_callout_reset(&ble_hs_flow_timer, BLE_HS_FLOW_ITVL_TICKS);
        BLE_HS_DBG_ASSERT_EVAL(rc == 0);
    }
}

static stats_error_t
ble_hs_flow_acl_free(struct os_mempool_ext *mpe, void *data, void *arg)
{
    struct ble_hs_conn *conn;
    const struct os_mbuf *om;
    uint16_t conn_handle;
    int idx;
    int rc;

    om = data;
    idx = ble_hs_flow_mbuf_index(om);
    conn_handle = ble_hs_flow_mbuf_conn_handle[idx];

    /* Free the mbuf back to its pool. */
#if defined(TUYA_USE_DYNA_RAM) && (TUYA_USE_DYNA_RAM==0)
    rc = os_memblock_put_from_cb(&mpe->mpe_mp, data);
#else
    rc = tuya_ble_hci_mp_num_buf_free(&mpe->mpe_mp, data);
#endif
    if (rc != 0) {
        return rc;
    }
    /* Allow nested locks - there are too many places where acl buffers can get
     * freed.
     */
    ble_hs_lock_nested();

    conn = ble_hs_conn_find(conn_handle);
    if (conn != NULL) {
        ble_hs_flow_inc_completed_pkts(conn);
    }

    ble_hs_unlock_nested();

    return 0;
}
#endif /* (TY_HS_BLE_HS_FLOW_CTRL) */

void
ble_hs_flow_connection_broken(uint16_t conn_handle)
{
#if (TY_HS_BLE_HS_FLOW_CTRL) &&                 \
    (TY_HS_BLE_HS_FLOW_CTRL_TX_ON_DISCONNECT)
    ble_hs_lock();
    ble_hs_flow_tx_num_comp_pkts();
    ble_hs_unlock();
#endif
}

/**
 * Associates incoming data packet with a connection handle of the sender.
 *
 * If flow control is disabled, this function is a no-op.
 */
void
ble_hs_flow_track_data_mbuf(struct os_mbuf *om)
{
#if (TY_HS_BLE_HS_FLOW_CTRL)
#if defined(TUYA_USE_DYNA_RAM) && (TUYA_USE_DYNA_RAM==0)
    const struct hci_data_hdr *hdr;
    int idx = ble_hs_flow_mbuf_index(om);

    hdr = (void *)om->om_data;
    ble_hs_flow_mbuf_conn_handle[idx] = BLE_HCI_DATA_HANDLE(hdr->hdh_handle_pb_bc);
#else
    const struct hci_data_hdr *hdr;
    int idx = (om->om_omp->omp_pool->mp_num_blocks - om->om_omp->omp_pool->mp_num_free - 1);
    BLE_HS_DBG_ASSERT((idx >= 0) && (idx <= om->om_omp->omp_pool->mp_num_blocks));

    // PR_ERR("IDX:%x %x,%p",idx, om->om_omp->omp_pool->mp_num_free, om);
    ble_hs_flow_mbuf_conn_index[idx] = om;
    hdr = (void *)om->om_data;
    ble_hs_flow_mbuf_conn_handle[idx] = BLE_HCI_DATA_HANDLE(hdr->hdh_handle_pb_bc);
#endif
#endif
}

/**
 * Sends the HCI commands to the controller required for enabling host flow
 * control.
 *
 * If flow control is disabled, this function is a no-op.
 */
int
ble_hs_flow_startup(void)
{
#if (TY_HS_BLE_HS_FLOW_CTRL)
    struct ble_hci_cb_ctlr_to_host_fc_cp enable_cmd;
    struct ble_hci_cb_host_buf_size_cp buf_size_cmd = {
            .acl_data_len = htole16((TUYA_BLE_ACL_BUF_SIZE)),
            .acl_num = htole16((TUYA_BLE_ACL_BUF_COUNT)),
    };
    int rc;

    tuya_ble_event_set_ev(&ble_hs_flow_ev, ble_hs_flow_event_cb, NULL);

    /* Assume failure. */
    tuya_ble_hci_set_acl_free_cb(NULL, NULL);


    enable_cmd.enable = BLE_HCI_CTLR_TO_HOST_FC_ACL;

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_CTLR_BASEBAND,
                                      BLE_HCI_OCF_CB_SET_CTLR_TO_HOST_FC),
                           &enable_cmd, sizeof(enable_cmd), NULL, 0);
    if (rc != 0) {
        return rc;
    }

    rc = ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_CTLR_BASEBAND,
                                      BLE_HCI_OCF_CB_HOST_BUF_SIZE),
                           &buf_size_cmd, sizeof(buf_size_cmd), NULL, 0);
    if (rc != 0) {
        enable_cmd.enable = BLE_HCI_CTLR_TO_HOST_FC_OFF;
        ble_hs_hci_cmd_tx(BLE_HCI_OP(BLE_HCI_OGF_CTLR_BASEBAND,
                                     BLE_HCI_OCF_CB_SET_CTLR_TO_HOST_FC),
                          &enable_cmd, sizeof(enable_cmd), NULL, 0);
        return rc;
    }

    /* Flow control successfully enabled. */
    ble_hs_flow_num_completed_pkts = 0;
    tuya_ble_hci_set_acl_free_cb(ble_hs_flow_acl_free, NULL);
    tuya_ble_callout_init(&ble_hs_flow_timer, ble_hs_evq_get(),
                         ble_hs_flow_event_cb, NULL);
#endif

    return 0;
}
