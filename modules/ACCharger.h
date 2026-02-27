#include "sys.h"
#include "uart.h"

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
#define warningCHARGE_FULLCOMPLETED          26


#define ON    1
#define OFF   0
#define CHARGE_STARTUP_BY_CARD            1
#define CHARGE_STARTUP_BY_APP             2
#define CHARGE_STARTUP_BY_SCREEN          3


#define DEBOUNCE_COUNT                     10

#define ACCHARGE_SCAN_ADDRESS            0x0600
#define ACCHARGE_TASK_INTERVAL            100


/*页面id宏定义*/
#define CHARGE_STANDBY_PAGE                1
#define CHARGE_SELF_CHECK_PAGE             8
#define CHARGE_CHARGING_PAGE               3
#define CHARGE_ABNORMAL_PAGE               6
#define CHARGE_COMPLETED_PAGE              4
#define CHARGE_NOGUN_PAGE                  0
#define CHARGE_GUN_UNCONNECT_PAGE          7


#define cmdCHARGE_PARA_GET                   0x01
#define cmdCHARGE_PARA_SEND                  0x02
#define cmdCHARGE_VOLTAGE_GET                0x03
#define cmdCHARGE_CONTROL_SEND               0x04

#define CHARGE_PILE_PARA_ADDR               0x2000
#define CHARGE_GUN_QUANTITY_ADDR            0x2004
#define CHARGE_RATED_CURRENT_ADDR           0x2005
#define CHARGE_PHASE_NUM_ADDR               0x2006
#define CHARGE_RATED_VOLTAGE_ADDR           0x2007


#define CHARGE_GUN_NUM_ADDR                 0x2010
#define CHARGE_VOLTAGE_ADDR                 0x2011
#define CHARGE_CURRENT_ADDR                 0x2012
#define CHARGE_POWER_ADDR                   0x2013
#define CHARGE_WATTHOUR_ADDR                0x2014
#define CHARGE_GUNCONNECT_STA_ADDR          0x2015
#define CHARGE_ERROR_CODE_ADDR              0x2016
#define CAHRGE_RUNTIME_STA_ADDR             0x2017


/**
 * 充电参数结构体
 */
typedef struct
{
    uint16_t voltage;              /* 当前电压值，通过HLW芯片串口获取*/
    uint16_t current;              /* 当前电流值，通过HLW芯片串口获取*/
    uint16_t power;                /* 当前功率值，通过HLW芯片串口获取*/
    uint32_t leakage_value;         /* 漏电值，ADC获取*/
    uint32_t connect_confirm_value;/* CC信号值，ADC获取，判断不同的电阻*/
    uint16_t stop_result;         /* 停止充电原因*/
    uint8_t startup_type;         /* 启动充电的方式，屏幕点击触发，刷卡触发，APP触发*/
    uint8_t card_num[32];         /* 卡号*/
    float startup_watthour;       /* 开始时的电量*/
    float now_watthour;           /* 当前电量*/
    uint32_t charge_time;         /* 已充电时间*/
    uint8_t is_charge;           /* 充电状态，0：未充电，1：充电中*/

}ACChargingPara;


typedef struct
{
    float rated_current;                   /*额定电流，初始化的时候进行设置，需要断电保存*/
    float over_current_ratio;              /*过流检测比值，过流=额定电流*比值，初始化的时候进行设置，需要断电保存*/
    float recovery_over_current_ratio;     /*过流检测自动恢复比值，过流自动恢复=额定电流*比值，初始化的时候进行设置，需要断电保存*/
    float rated_voltage;                    /*额定电压，初始化的时候进行设置，需要断电保存*/
    float over_voltage_ratio;               /*过压检测比值，过压=额定电压*比值，初始化的时候进行设置，需要断电保存*/
    float recovery_over_voltage_ratio;      /*过压检测自动恢复比值，过压自动恢复=额定电压*比值，初始化的时候进行设置，需要断电保存*/
    float under_voltage_ratio;              /*欠压检测比值，欠压=额定电压*比值，初始化的时候进行设置，需要断电保存*/
    float recovery_under_voltage_ratio;     /*欠压检测自动恢复比值，欠压自动恢复=额定电压*比值，初始化的时候进行设置，需要断电保存*/
    uint16_t leakage_current_threshold;     /*漏电检测阈值，用来判断是否漏电，初始化的时候进行设置，需要断电保存*/
    float min_current_threshold;             /*最低电流阈值，用来判断是否充满，初始化的时候进行设置，需要断电保存*/
    uint16_t gun_connect_threshold;         /*枪连接检测阈值，初始化的时候进行设置，需要断电保存*/
    uint16_t vehicle_connect_threshold;     /*车辆连接检测阈值，初始化的时候进行设置，需要断电保存*/
    uint16_t alert_duration;                 /*告警持续时间，初始化的时候进行设置，需要断电保存*/
}ACChargerLimitPara;

typedef struct 
{
    uint8_t pwm_enable;                      /*CP信号pwm输出标志，0不输出，保持12V高电平，1输出*/
    uint16_t pwm_duty;                       /*CP信号输出占空比，10%-85%*/
}ConnectPilotPara;


/**
 * 测试标志位结构体
 */
typedef struct
{
    uint8_t jerk_flag;                       /*急停标志*/
    uint8_t overcurrent_flag;                /*过流标志*/
    uint8_t overvoltage_flag;                /*过压标志*/
    uint8_t undervoltage_flag;               /*欠压标志*/
    uint8_t leakage_flag;                    /*漏电标志*/
    uint8_t meter_abnormal_flag;             /*电表异常标志*/
    uint8_t gun_connect_flag;                /*枪连接标志*/
    uint8_t vehicle_connect_flag;            /*车辆连接标志*/
}ACChargerInfo;

typedef struct 
{
    uint8_t network_isconnect;              /*网络是否连接*/
}NetworkInfo;

typedef struct 
{
    uint8_t startup_type;                  /*上一次记录启动方式，用于自动恢复充电*/
    uint8_t card_num[32];                  /*上一次记录卡号，用于自动恢复充电*/
    float used_watthour;                   /*上一次记录已使用电量*/
    uint32_t charge_time;                  /*上一次记录充电时间*/
}LastChargeData;

typedef enum 
{
    CHARGE_SELF_CHECK,
    CHARGE_CONNECT_PILOT,
    CHARGE_START
}ACChargeStepEnum;


extern ACChargingPara g_ac_charging_para;
extern ACChargerInfo g_ac_charger_info;
extern NetworkInfo g_network_info;
extern LastChargeData g_last_charge_data;
extern ConnectPilotPara g_connect_pilot_para;
extern uint16_t g_charge_state;
extern ACChargerLimitPara g_ac_charger_limit_para;

void ACChargerTask(void);
void UartACChargeUserProtocol(UART_TYPE *uart,uint8_t *frame, uint16_t len);