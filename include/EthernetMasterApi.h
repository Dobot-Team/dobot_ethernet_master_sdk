#ifndef ETHERNET_MASTER_API_H
#define ETHERNET_MASTER_API_H

#include <stdbool.h>

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// ========== Linux 动态库导出宏 ==========
#define ETHERNET_MASTER_API __attribute__((visibility("default")))

// ================== 数据结构定义 ==================

// ========== 电机数量定义（唯一真源） ==========
#define ETHERNET_MASTER_MAX_MOTOR_NUM 30

/**
 * @brief 伺服参数
 */
typedef struct {
  int isInverted[ETHERNET_MASTER_MAX_MOTOR_NUM];  // 电机是否反转（1/-1）
  bool isVirtual[ETHERNET_MASTER_MAX_MOTOR_NUM];  // 是否虚轴
  int reductionRatio_mol[ETHERNET_MASTER_MAX_MOTOR_NUM];  // 减速比分子
  int reductionRatio_den[ETHERNET_MASTER_MAX_MOTOR_NUM];  // 减速比分母
} ServoConfig;

/**
 * @brief 伺服运控指令
 */
typedef struct {
  uint8_t mode;  // 控制模式 mit -- 11
  float q;       // 位置命令（rad）
  float dq;      // 速度命令（rad/s）
  float tau;     // 力矩命令（Nm）
  float kp;      // 比例增益
  float kd;      // 微分增益
} MotorCmd;

/**
 * @brief 伺服反馈
 */
typedef struct {
  uint8_t state;  // 0-掉线 1-报错 2-失能 3-使能
  uint8_t mode;
  float q;
  float dq;
  float ddq;
  float tau_est;
  float q_raw;
  float dq_raw;
  float ddq_raw;
  uint8_t mcu_temp;
  uint8_t mos_temp;
  uint8_t motor_temp;
  uint8_t bus_voltage;
  bool is_virtual;
  uint16_t error_code;
  uint16_t version;
} MotorState;

/**
 * @brief 伺服控制指令
 */
typedef struct {
  MotorCmd motor_cmd[ETHERNET_MASTER_MAX_MOTOR_NUM];
  bool enableCmd[ETHERNET_MASTER_MAX_MOTOR_NUM];
  bool resetCmd[ETHERNET_MASTER_MAX_MOTOR_NUM];
  bool caliCmd[ETHERNET_MASTER_MAX_MOTOR_NUM];
  bool homingCmd[ETHERNET_MASTER_MAX_MOTOR_NUM];
} LowerCmd;

/**
 * @brief 伺服反馈指令
 */
typedef struct {
  MotorState motor_state[ETHERNET_MASTER_MAX_MOTOR_NUM];
} LowerState;

// ==================  对外导出 API ==================

/**
 * @brief 初始化接口
 */
ETHERNET_MASTER_API void MasterHandlerInit(ServoConfig config,
                                           double intervalMs);

/**
 * @brief 启动通讯线程
 */
ETHERNET_MASTER_API void MasterStart(void);

/**
 * @brief 下发伺服控制命令
 * @note 线程安全：是（内部已做双缓冲）
 */
ETHERNET_MASTER_API void MasterCmd(const LowerCmd* cmd);

/**
 * @brief 获取伺服反馈状态
 * @note 线程安全：是（内部已做双缓冲）
 */
ETHERNET_MASTER_API void MasterState(LowerState* out_state);

/**
 * @brief 停止通讯
 */
ETHERNET_MASTER_API void MasterStop(void);

#ifdef __cplusplus
}
#endif

#endif  // ETHERNET_MASTER_API_H
