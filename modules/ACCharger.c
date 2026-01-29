#include "ACCharger.h"

uint16_t g_charge_state = stateCHARGE_STANDBY;
ACChargingPara g_ac_charging_para;
ACChargerInfo g_ac_charger_info;
NetworkInfo g_network_info;
LastChargeData g_last_charge_data;
ConnectPilotPara g_connect_pilot_para;


/**
 * 开始充电
 * 触发方式：屏幕点击开始充电，刷卡触发，app下发
 */
static void ACChargeStart(uint8_t startup_type)
{
    /*1.充电状态标记*/
    g_ac_charging_para.is_charge = ON;
    /*2.记录起始电量*/
    g_ac_charging_para.startup_watthour = g_ac_charging_para.now_watthour;
    /*3.充电时长清零*/
    g_ac_charging_para.charge_time = 0;
    /*4.记录充电类型*/
    g_ac_charging_para.startup_type = startup_type;
    /*5.连接继电器*/
    ACChargeRelayCtrl(ON);
    /*6.cp信号pwm使能*/
    g_connect_pilot_para.pwm_enable = ON;
    /*7.停止原因清零*/
    g_ac_charging_para.stop_result = 0;
}


/**
 * 结束充电
 * 触发方式：屏幕点击结束充电，刷卡触发，app触发，充电时异常状态触发
 */
static void ACChargeStop()
{
    if(g_ac_charger_network_info.network_isconnect)
    {
       
    }else
    {
        /*1.记录充电信息，卡号，充电时长，电量*/
        g_last_charge_data.startup_type = g_ac_charging_para.startup_type;
        memcpy(g_last_charge_data.card_num, g_ac_charging_para.card_num, sizeof(g_last_charge_data.card_num));
        g_last_charge_data.used_watthour = g_ac_charging_para.now_watthour - g_ac_charging_para.startup_watthour;
        g_last_charge_data.charge_time = g_ac_charging_para.charge_time;
        /*2.进入结算页面 */
        if(g_ac_charging_para.startup_type == CHARGE_STARTUP_BY_CARD)
        {
            g_charge_state = stateCHARGE_CARDSETTLE;
        }else if(g_ac_charging_para.startup_type == CHARGE_STARTUP_BY_SCREEN)
        {
            g_charge_state = stateCHARGE_SCREENSETTLE;
        }
        g_ac_charging_para.startup_type = 0;
        /*3.断开继电器*/
        ACChargeRelayCtrl(OFF);
        /*4.充电状态标记清零*/
        g_ac_charging_para.is_charge = OFF;
    }
}


/**
 * 恢复自动充电
 * 触发方式：过压，欠压，电表通讯异常可以自动恢复充电
 */
static void ACChargeAutoRecovery()
{
    if(g_ac_charging_para.stop_result == errorCHARGE_JERK || g_ac_charging_para.stop_result == errorCHARGE_UNDERVOLTAGE || g_ac_charging_para.stop_result == errorCHARGE_OVERVOLTAGE)
    {
        if(!g_ac_charger_info.overvoltage_flag && !g_ac_charger_info.undervoltage_flag && !g_ac_charger_info.jerk_flag)
        {
            /*1.记录上一次的充电时间，卡号，启动方式信息*/
            g_ac_charging_para.charge_time = g_last_charge_data.charge_time;
            g_ac_charging_para.startup_type = g_last_charge_data.startup_type;
            memcpy(g_ac_charging_para.card_num, g_last_charge_data.card_num, sizeof(g_ac_charging_para.card_num));
            /*2.充电状态标记*/
            g_ac_charging_para.is_charge = ON;
            /*3.连接继电器*/
            ACChargeRelayCtrl(ON);
            /*4.停止原因清零*/
            g_ac_charging_para.stop_result = 0;
        }
    }
}


void ACChargerStateCheck(void)
{
    switch(g_charge_state)
    {
        case stateCHARGE_STANDBY:
            if(g_ac_charger_info.jerk_flag)
            {
                g_charge_state = errorCHARGE_JERK;
            }
            if(g_ac_charger_info.overcurrent_flag)
            {
                g_charge_state = errorCHARGE_OVERCURRENT;
            }
            if(g_ac_charger_info.overvoltage_flag)
            {
                g_charge_state = errorCHARGE_OVERVOLTAGE;
            }
            if(g_ac_charger_info.undervoltage_flag)
            {
                g_charge_state = errorCHARGE_UNDERVOLTAGE;
            }
            if(g_ac_charger_info.leakage_flag)
            {
                g_charge_state = errorCHARGE_LEAKAGE;
            }
            if(g_ac_charger_info.meter_abnormal_flag)
            {
                g_charge_state = errorCHARGE_METERABNORMAL;
            }
        break;
        case stateCHARGE_PROCESS:
            if(g_ac_charger_info.jerk_flag && !g_ac_charging_para.stop_result)
            {
                ACChargeStop();
                g_ac_charging_para.stop_result = errorCHARGE_JERK;
            }
            if(g_ac_charger_info.overcurrent_flag && !g_ac_charging_para.stop_result)
            {
                ACChargeStop();
                g_ac_charging_para.stop_result = errorCHARGE_OVERCURRENT;
            }
            if(g_ac_charger_info.overvoltage_flag && !g_ac_charging_para.stop_result)
            {
                ACChargeStop();
                g_ac_charging_para.stop_result = errorCHARGE_OVERVOLTAGE;
            }
            if(g_ac_charger_info.undervoltage_flag && !g_ac_charging_para.stop_result)
            {
                ACChargeStop();
                g_ac_charging_para.stop_result = errorCHARGE_UNDERVOLTAGE;
            }
            if(g_ac_charger_info.leakage_flag && !g_ac_charging_para.stop_result)
            {
                ACChargeStop();
                g_ac_charging_para.stop_result = errorCHARGE_LEAKAGE;
            }
        break;

    }

}
