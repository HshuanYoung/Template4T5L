#include "t5los8051.h"
#include "sys.h"

#define stateCHARGE_STANDBY              0
#define stateCHARGE_PROCESS              3
#define stateCHARGE_CARDSETTLE           5
#define stateCHARGE_SCREENSETTLE         7
#define stateCHARGE_UNSETTLED            8
#define stateCHARGE_BYCARD               9
#define stateCHARGE_BYAPP                10
#define stateCHARGE_ABREND_DELAY         11
#define stateVEHICLE_CHARGEEND           12
#define stateCARD_ONLINEWAIT            13
#define stateCARD_LOCK                   14
#define stateCARD_LOWMONEY               15

#define errorCHARGE_JERK                 16
#define errorCHARGE_OVERCURRENT          17
#define errorCHARGE_OVERVOLTAGE         18
#define errorCHARGE_UNDERVOLTAGE         19
#define errorCHARGE_LEAKAGE              20
#define errorCHARGE_METERABNORMAL        21
#define errorVEHICLE_GUNCONNECT          22
#define errorVEHICLE_ABNORMAL            23
#define errorCARD_NOBINDING              24
#define errorCARD_STARTUPDIFF            25


#define ON    1
#define OFF   0
#define CHARGE_STARTUP_BY_CARD            1
#define CHARGE_STARTUP_BY_APP             2
#define CHARGE_STARTUP_BY_SCREEN          3


/**
 * 充电参数结构体
 */
typedef struct
{
    uint16_t voltage;
    uint16_t current;
    uint16_t power;
    uint16_t stop_result;
    uint8_t startup_type;
    uint8_t card_num[32];
    float startup_watthour;
    float now_watthour;
    uint32_t charge_time;
    uint8_t is_charge;

}ACChargingPara;

typedef struct 
{
    uint8_t pwm_enable;
    uint16_t pwm_duty;
}ConnectPilotPara;


/**
 * 测试标志位结构体
 */
typedef struct
{
    uint8_t jerk_flag;
    uint8_t overcurrent_flag;
    uint8_t overvoltage_flag;
    uint8_t undervoltage_flag;
    uint8_t leakage_flag;
    uint8_t meter_abnormal_flag;
}ACChargerInfo;

typedef struct 
{
    uint8_t network_isconnect;
}NetworkInfo;

typedef struct 
{
    uint8_t startup_type;
    uint8_t card_num[32];
    float used_watthour;
    uint32_t charge_time;
}LastChargeData;


extern ACChargingPara g_ac_charging_para;
extern ACChargerInfo g_ac_charger_info;
extern NetworkInfo g_network_info;
extern LastChargeData g_last_charge_data;
extern ConnectPilotPara g_connect_pilot_para;
extern uint16_t g_charge_state;