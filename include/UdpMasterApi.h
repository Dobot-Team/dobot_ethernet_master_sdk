#ifndef UDP_MASTER_API_H
#define UDP_MASTER_API_H

#include <stdbool.h>

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// ========== Linux 动态库导出宏 ==========
#define UDP_MASTER_API __attribute__((visibility("default")))

// ==================  对外导出 API ==================

/**
 * @brief 初始化 UDP 通讯模块（创建套接字、绑定本地端口等）。
 * 
 * @param targetIp 目标设备的 IP 地址（点分十进制字符串）。
 * @param targetPort 目标设备的 UDP 端口号。
 * @return true 初始化成功，false 初始化失败（如套接字已准备好、参数无效等）。
 */
UDP_MASTER_API bool UdpMasterInit(const char* targetIp, uint16_t targetPort);

/**
 * @brief 启动 UDP 通讯线程。
 */
UDP_MASTER_API void UdpMasterStart(void);

/**
 * @brief 停止 UDP 通讯并释放相关资源。
 */
UDP_MASTER_API void UdpMasterStop(void);

#ifdef __cplusplus
}
#endif

#endif  // UDP_MASTER_API_H
