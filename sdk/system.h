/****************************************Copyright (c)*************************
**                                 (C), 2015-2020
**
**                                 http://www.tuya.com
**
******************************************************************************/
#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
#define         HEAT_BEAT_CMD                   0
#define         PRODUCT_INFO_CMD                1
#define         WORK_MODE_CMD                   2
#define         WIFI_STATE_CMD                  3
#define         WIFI_RESET_CMD                  4
#define         WIFI_MODE_CMD                   5
#define         DATA_QUERT_CMD                  6
#define         STATE_UPLOAD_CMD                7
#define         STATE_QUERY_CMD                 8
#define         UPDATE_QUERY_CMD                9
#define         UPDATE_START_CMD                0x0a
#define         UPDATE_TRANS_CMD                0x0b
#define         GET_ONLINE_TIME_CMD             0x0c
#define         FACTORY_MODE_CMD                0x0d
#define         WIFI_TEST_CMD                   0x0e
#define         GET_LOCAL_TIME_CMD              0x1c


//=============================================================================
#define VERSION                 0x00
#define PROTOCOL_HEAD           0x07
#define FIRM_UPDATA_SIZE        256
#define FRAME_FIRST             0x55
#define FRAME_SECOND            0xaa
//============================================================================= 
SYSTEM_EXTERN unsigned char volatile wifi_queue_buf[PROTOCOL_HEAD + WIFI_UART_QUEUE_LMT];
SYSTEM_EXTERN unsigned char wifi_uart_rx_buf[PROTOCOL_HEAD + WIFI_UART_RECV_BUF_LMT];
SYSTEM_EXTERN unsigned char wifi_uart_tx_buf[PROTOCOL_HEAD + WIFIR_UART_SEND_BUF_LMT];
//
SYSTEM_EXTERN volatile unsigned char *queue_in;
SYSTEM_EXTERN volatile unsigned char *queue_out;
SYSTEM_EXTERN volatile unsigned short queue_total_data;

SYSTEM_EXTERN unsigned char stop_update_flag;

#ifndef WIFI_CONTROL_SELF_MODE
SYSTEM_EXTERN unsigned char reset_wifi_flag;
SYSTEM_EXTERN unsigned char set_wifimode_flag;
SYSTEM_EXTERN unsigned char wifi_work_state;
#endif


unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte);
unsigned short set_wifi_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len);
void wifi_uart_write_frame(unsigned char fr_type, unsigned short len);
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);
void data_handle(unsigned short offset);
unsigned short get_queue_total_data(void);
unsigned char Queue_Read_Byte(void);

#endif
  
  
