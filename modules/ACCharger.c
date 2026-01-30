#include "ACCharger.h"
#include "string.h"

uint16_t g_charge_state = stateCHARGE_STANDBY;
ACChargingPara g_ac_charging_para;
ACChargerInfo g_ac_charger_info;
NetworkInfo g_network_info;
LastChargeData g_last_charge_data;
ConnectPilotPara g_connect_pilot_para;
ACChargerLimitPara g_ac_charger_limit_para;

static void ACChargeRelayCtrl(uint8_t enable_type)
{
	
}

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
    if(g_network_info.network_isconnect)
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


void ACChargerStateCheckTask(void)
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


/* 100ms定时器任务 */
void ACChargerKeyHandleTask()
{
    const uint16_t uint16_port_zero = 0;
    uint16_t dgus_value;
    static uint16_t self_check_delay_count = 0;
    ACChargeStepEnum charge_step = CHARGE_SELF_CHECK;
    #if sysDGUS_AUTO_UPLOAD_ENABLED || uartTA_PROTOCOL_ENABLED
    DgusAutoUpload();
    #endif /* sysDGUS_AUTO_UPLOAD_ENABLED ||uartTA_PROTOCOL_ENABLED */

    read_dgus_vp(ACCHARGE_SCAN_ADDRESS, (uint8_t *)&dgus_value, 1);
    if(dgus_value == 0x0001)
    {
        /* 屏幕点击进行充电 */
        /* 1.进行5s左右的自检*/
        /* 2.启动CP信号*/
        /* 3.进行充电*/
        if(charge_step == CHARGE_SELF_CHECK)
        {
            self_check_delay_count++;
            if(self_check_delay_count == 2)
            {
                /* 跳转到自检页面*/
            }
            if(self_check_delay_count >= 50)
            {
                self_check_delay_count = 0;
                charge_step = CHARGE_CONNECT_PILOT;
            }
        }else if(charge_step == CHARGE_CONNECT_PILOT)
        {
            /*启动CP信号,延时3s*/
            g_connect_pilot_para.pwm_enable = ON;
            self_check_delay_count++;
            if(self_check_delay_count >= 30)
            {
                self_check_delay_count = 0;
                charge_step = CHARGE_START;
            }
        }
        else if(charge_step == CHARGE_START)
        {
            ACChargeStart(CHARGE_STARTUP_BY_SCREEN);
            g_charge_state = stateCHARGE_PROCESS;
            /* 跳转到充电页面 */
            write_dgus_vp(ACCHARGE_SCAN_ADDRESS, (uint8_t *)&uint16_port_zero, 1);
        }
    }
}


static void OverCurrentDeal()
{
    static uint16_t over_current_delay_count = 0,over_current_recovery_delay_count = 0;
    /**
     * 如果电流超过设定值持续，将标志位置为1
     */
    if(g_ac_charging_para.current > (g_ac_charger_limit_para.rated_current * g_ac_charger_limit_para.over_current_ratio))
    {
        over_current_delay_count++;
        if(over_current_delay_count >= g_ac_charger_limit_para.alert_duration)
        {
            g_ac_charger_info.overcurrent_flag = ON;
        }
    }else if(g_ac_charging_para.current <= (g_ac_charger_limit_para.rated_current * g_ac_charger_limit_para.recovery_over_current_ratio))
    {
        over_current_delay_count = 0;
        over_current_recovery_delay_count++;
        if(over_current_recovery_delay_count >= g_ac_charger_limit_para.alert_duration)
        {
            g_ac_charger_info.overcurrent_flag = OFF;
        }
    }
}


static void UnderVoltageDeal()
{
    static uint16_t under_voltage_delay_count = 0,under_voltage_recovery_delay_count = 0;
    /**
     * 如果电压低于设定值持续，将标志位置为1
     */
    if(g_ac_charging_para.voltage < (g_ac_charger_limit_para.rated_voltage * g_ac_charger_limit_para.under_voltage_ratio))
    {
        under_voltage_delay_count++;
        if(under_voltage_delay_count >= g_ac_charger_limit_para.alert_duration)
        {
            g_ac_charger_info.undervoltage_flag = ON;
        }
    }else if(g_ac_charging_para.voltage >= (g_ac_charger_limit_para.rated_voltage * g_ac_charger_limit_para.recovery_under_voltage_ratio))
    {
        under_voltage_delay_count = 0;
        under_voltage_recovery_delay_count++;
        if(under_voltage_recovery_delay_count >= g_ac_charger_limit_para.alert_duration)
        {
            g_ac_charger_info.undervoltage_flag = OFF;
        }
    }
}

static void OverVoltageDeal()
{
    static uint16_t over_voltage_delay_count = 0,over_voltage_recovery_delay_count = 0;
    /**
     * 如果电压超过设定值持续，将标志位置为1
     */
    if(g_ac_charging_para.voltage > (g_ac_charger_limit_para.rated_voltage * g_ac_charger_limit_para.over_voltage_ratio))
    {
        over_voltage_delay_count++;
        if(over_voltage_delay_count >= g_ac_charger_limit_para.alert_duration)
        {
            g_ac_charger_info.overvoltage_flag = ON;
        }
    }else if(g_ac_charging_para.voltage <= (g_ac_charger_limit_para.rated_voltage * g_ac_charger_limit_para.recovery_over_voltage_ratio))
    {
        over_voltage_delay_count = 0;
        over_voltage_recovery_delay_count++;
        if(over_voltage_recovery_delay_count >= g_ac_charger_limit_para.alert_duration)
        {
            g_ac_charger_info.overvoltage_flag = OFF;
        }
    }
}


static void LeakageDeal()
{

}


static void LowCurrentAutoStopDeal()
{

}


static void GunConnectDeal()
{

}


static void LowBalanceDeal()
{

}

static void CPSignalMonitorDeal()
{

}


void ACChargerBackGroundTask()
{
    /**
     * 1.过流处理
     * 2.欠压处理
     * 3.过压处理
     * 4.漏电处理
     * 5.电流低于指定值时自动停止处理
     * 6.枪连接处理
     * 7.余额不足处理
     * 8.cp信号监测
     */

    OverCurrentDeal();
    UnderVoltageDeal();
    OverVoltageDeal();
    LeakageDeal();
    LowCurrentAutoStopDeal();
    GunConnectDeal();
    LowBalanceDeal();
    CPSignalMonitorDeal();
}


void ACChargerTask(void)
{
    ACChargerBackGroundTask();
    ACChargerKeyHandleTask();
    ACChargerStateCheckTask();
}