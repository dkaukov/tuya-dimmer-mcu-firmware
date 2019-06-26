#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_
#define PRODUCT_KEY "tasmota"
#define MCU_VER "1.0"

#ifndef SUPPORT_MCU_FIRM_UPDATE
#define WIFI_UART_QUEUE_LMT             24
#define WIFI_UART_RECV_BUF_LMT          24
#else
#define WIFI_UART_QUEUE_LMT             128
#define WIFI_UART_RECV_BUF_LMT          300
#endif

#define WIFIR_UART_SEND_BUF_LMT         24

//#define         WIFI_CONTROL_SELF_MODE
#ifdef          WIFI_CONTROL_SELF_MODE
  #define     WF_STATE_KEY
  #define     WF_RESERT_KEY
#endif

//#define         SUPPORT_MCU_RTC_CHECK
//#define         WIFI_TEST_ENABLE

#define DPID_SWITCH 1
#define DPID_BRIGHTNESS 2
#define DPID_FREQUENCY 3

void all_data_update(void);
void switch_update(void);
void brightness_update(void);
void frequency_update(void);

#ifdef SUPPORT_MCU_RTC_CHECK
void mcu_write_rtctime(unsigned char time[]);
#endif

#ifdef WIFI_TEST_ENABLE
void wifi_test_result(unsigned char result,unsigned char rssi);
#endif

#ifdef SUPPORT_MCU_FIRM_UPDATE
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length);
#endif

unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);

unsigned char get_download_cmd_total(void);



#endif
