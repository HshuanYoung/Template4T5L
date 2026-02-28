#ifndef _4G_AIR780E_H_
#define _4G_AIR780E_H_



#include "sys.h"


#define AIR780E_UART    uart5
#define DATA_MAX_LEN    1024
#define air780eCONNECT_DWINCLOUD         0
#define air780eCONNECT_WEBSOCKET         1
#define air780eCONNECT_HTTP              2
#define air780eCONNECT_MQTT              3
#define air780eNORFLASH_RENT_ADDR        0x4000
#define AIR780E_TASK_INTERVAL            100
#define AIR780E_VALUE_SCAN_ADDR          0x7010

typedef struct
{
    uint16_t url_start_addr;
    uint16_t data_para_addr;
    uint16_t data_json_addr;
    uint16_t send_json_addr;

    uint16_t mqtt_topic_addr;
    uint16_t download_addr;
    uint16_t mqtt_clean_start;

    uint8_t url_string[128];
    uint8_t machine_no_rx[128];
    uint8_t machine_no_tx[128];

    uint8_t connect_type;
    uint8_t connect_flag;
    uint8_t connect_step;
} Air780E_Status;
extern Air780E_Status air780e_status;
extern uint16_t air780e_num_rent[8];
extern uint8_t air780e_time_buffer[32];
extern uint8_t air780e_mac_buffer[32];
extern uint8_t air780e_ccid_buffer[32];

void Air780E_Task(void);

#endif /* _4G_AIR780E_H_ */