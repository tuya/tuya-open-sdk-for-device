/**
 * @file eapol.h
 * @brief hostapd and wpa_supplicant EAPOL API
 * 
 * @copyright Copyright(C),2018-2020, 涂鸦科技 www.tuya.com
 * 
 */
#ifndef __EAPOL_H__
#define __EAPOL_H__

void etheapol_input(struct pbuf* pbuf, struct netif* netif);
int etheapol_output(int vif_index, unsigned char *buf, unsigned short len);

#endif /* __EAPOL_H__ */