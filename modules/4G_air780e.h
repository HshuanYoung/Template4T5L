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


/**
 * @brief Air780E模块状态结构体
 * 
 * 用于存储Air780E 4G模块的各种状态信息和配置参数
 */
typedef struct
{
    uint16_t url_start_addr;      /**< URL起始地址，用于存储服务器地址 */
    uint16_t data_para_addr;      /**< 数据参数地址，用于存储参数信息 */
    uint16_t data_json_addr;      /**< 数据JSON地址，用于存储JSON格式数据 */
    uint16_t send_json_addr;      /**< 发送JSON地址，用于存储待发送的JSON数据 */

    uint16_t mqtt_topic_addr;     /**< MQTT主题地址，用于存储MQTT主题信息 */
    uint16_t download_addr;       /**< 下载地址，用于存储下载相关信息 */
    uint16_t mqtt_clean_start;    /**< MQTT清理会话标志，0表示不清理，1表示清理 */

    uint8_t url_string[128];      /**< URL字符串缓冲区，最大长度128字节 */
    uint8_t machine_no_rx[128];   /**< 设备号接收缓冲区，最大长度128字节 */
    uint8_t machine_no_tx[128];   /**< 设备号发送缓冲区，最大长度128字节 */

    uint8_t connect_type;         /**< 连接类型，取值：
                                   *  0: air780eCONNECT_DWINCLOUD (迪文云平台)
                                   *  1: air780eCONNECT_WEBSOCKET (WebSocket)
                                   *  2: air780eCONNECT_HTTP (HTTP)
                                   *  3: air780eCONNECT_MQTT (MQTT)
                                   */
    uint8_t connect_flag;         /**< 连接标志，0表示未连接，1表示已连接 */
    uint8_t connect_step;         /**< 连接步骤，用于跟踪连接过程的当前阶段 */
} Air780E_Status;
extern Air780E_Status air780e_status;
extern uint16_t air780e_num_rent[8];
extern uint8_t air780e_time_buffer[32];
extern uint8_t air780e_mac_buffer[32];
extern uint8_t air780e_ccid_buffer[32];

void Air780E_Task(void);

#endif /* _4G_AIR780E_H_ */