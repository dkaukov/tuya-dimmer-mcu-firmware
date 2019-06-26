#ifndef __WIFI_H_
#define __WIFI_H_


#ifndef TRUE
#define         TRUE                1
#endif
//
#ifndef FALSE
#define         FALSE               0
#endif
//
#ifndef NULL
#define         NULL                ((void *) 0)
#endif

#ifndef SUCCESS
#define         SUCCESS             1
#endif

#ifndef ERROR
#define         ERROR               0
#endif

#ifndef INVALID
#define         INVALID             0xFF
#endif

#ifndef ENABLE
#define         ENABLE                1
#endif
//
#ifndef DISABLE
#define         DISABLE               0
#endif
//=============================================================================
#define         DP_TYPE_RAW                     0x00
#define         DP_TYPE_BOOL                    0x01
#define         DP_TYPE_VALUE                   0x02
#define         DP_TYPE_STRING                  0x03
#define         DP_TYPE_ENUM                    0x04
#define         DP_TYPE_FAULT                   0x05

//=============================================================================
#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_SATE_UNKNOW                0xff
//=============================================================================
#define         SMART_CONFIG                    0x0  
#define         AP_CONFIG                       0x1   

//=============================================================================
#define         RESET_WIFI_ERROR                0
#define         RESET_WIFI_SUCCESS              1

//=============================================================================
#define         SET_WIFICONFIG_ERROR            0
#define         SET_WIFICONFIG_SUCCESS          1

//=============================================================================
#define         FIRM_STATE_UN_SUPPORT           0x00
#define         FIRM_STATE_WIFI_UN_READY        0x01
#define         FIRM_STATE_GET_ERROR            0x02
#define         FIRM_STATE_NO                   0x03
#define         FIRM_STATE_START                0x04

//=============================================================================
#define         UNION_WORK                      0x0
#define         WIFI_ALONE                      0x1

//=============================================================================
#define         NORMAL_MODE             0x00
#define         FACTORY_MODE            0x01
#define         UPDATE_MODE             0X02

#include "protocol.h"
#include "system.h"
#include "mcu_api.h"


//=============================================================================
typedef struct {
  unsigned char dp_id;
  unsigned char dp_type;
} DOWNLOAD_CMD_S;

#endif
