/**
 * @file tal_event_info.h
 * @brief simple event module
 *
 * @copyright Copyright 2023 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __BASE_EVENT_INFO_H__
#define __BASE_EVENT_INFO_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief the event define
 *
 */
#define EVENT_REBOOT_REQ        "dev.reboot.req"    // device health check reboot request, application should subscribe it if needed
#define EVENT_REBOOT_ACK        "dev.reboot.ack"    // device health check reboot ack, application should publish when it ready
#define EVENT_LAN_CLIENT_CLOSE  "lan.cli.close"     // lan client close
#define EVENT_RSC_UPDATE        "rsc.update"        // register center changed 
#define EVENT_HEALTH_ALERT      "health.alert"      // health alert


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /*__BASE_EVENT_H__ */


