/* 
 * File:   at_cmd_list.h
 * Author: thiagordp
 *
 * Created on 18 de Abril de 2017, 15:46
 */

#ifndef AT_CMD_LIST_H
#define AT_CMD_LIST_H

#define AT_RESET                "AT+RST";
#define AT_STATUS               "AT";
#define AT_LIST_NETWORKS        "AT+CWLAP";
#define AT_GET_IP               "AT+CIFSR"
#define AT_CONNECT_NETWORK      "AT+CWJAP"

#define AT_OPEN_CONNECTION      "AT+CIPSTART"
#define AT_CLOSE                "AT+CIPCLOSE"
#define AT_CONNECT_TCP          "TCP"
#define AT_CONNECT_UDP          "UDP"

#define AT_MODE                 "AT+CWMODE"
#define AT_MODE_STA             1
#define AT_MODE_AP              2
#define AT_MODE_BOTH            3

#define AT_SEND_DATA            "AT+CIPSEND"

#endif /* AT_CMD_LIST_H */

