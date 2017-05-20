/* 
 * File:   at_cmd_list.h
 * Author: thiagordp
 *
 * Created on 18 de Abril de 2017, 15:46
 */

#ifndef AT_CMD_LIST_H
#define AT_CMD_LIST_H

#define AT_CMD_FINISH_STRING	"\r\n"
#define AT_RESET                "AT+RST"
#define AT_STATUS               "AT"
#define AT_LIST_NETWORKS        "AT+CWLAP"
#define AT_GET_IP               "AT+CIFSR"
#define AT_CONNECT_NETWORK      "AT+CWJAP"

#define AT_OPEN_CONNECTION      "AT+CIPSTART"
#define AT_CLOSE                "AT+CIPCLOSE"
#define AT_CONNECT_TCP          "TCP"
#define AT_CONNECT_UDP          "UDP"

#define AT_MODE                 "AT+CWMODE"
#define AT_MODE_STA             (0x01)
#define AT_MODE_AP              (0x02)
#define AT_MODE_BOTH            (0x03)

#define AT_SEND_DATA            "AT+CIPSEND"

#define AT_MULTI_CONN			"AT+CIPMUX"
#define AT_MULTI_CONN_ON		(0x01)
#define AT_MULTI_CONN_OFF		(0x00)

#define AT_RECEIVED_DATA		"+IPD"
#define AT_OK					"OK"
#define AT_ERROR				"ERROR"
#define AT_BUSY					"busy"

#endif /* AT_CMD_LIST_H */

