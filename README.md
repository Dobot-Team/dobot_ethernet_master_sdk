# Ethernet Master SDK

[![License](https://img.shields.io/badge/license-Proprietary-blue.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/platform-Linux-lightgrey.svg)](https://www.linux.org/)

Ethernet Master SDK 是一个用于通过以太网（UDP）与下位机进行实时通讯的 C/C++ 动态链接库。该 SDK 提供了完整的伺服电机控制接口，支持最多 30 个电机的同步控制。

## ✨ 主要特性

- ✅ **多电机支持**：最多支持 30 个伺服电机同时控制
- ✅ **实时通讯**：1ms 通讯周期，低延迟、高可靠性
- ✅ **线程安全**：采用双缓冲机制，确保多线程环境下的数据一致性
- ✅ **C 接口**：提供纯 C 接口，兼容 C/C++ 项目
- ✅ **动态链接**：以 `.so` 动态库形式提供，便于集成
- ✅ **多架构支持**：提供 x86_64 和 aarch64 两种架构的库文件

## 📦 SDK 目录结构

```
ethernet-master-sdk/
├── include/                    # 头文件目录
│   └── EthernetMasterApi.h    # SDK 对外 API 头文件
├── lib/                        # 库文件目录
│   ├── x86_64/                 # x86_64 架构库文件
│   │   └── libethernet_master.so
│   └── aarch64/                # ARM 64位架构库文件
│       └── libethernet_master.so
├── docs/                       # 文档目录
│   └── SDK使用文档.md          # 详细使用文档
└── README.md                   # 本文件
```

## 🚀 快速开始

### 1. 检测系统架构

```bash
uname -m
# 输出 x86_64 或 aarch64
```

### 2. 在代码中包含头文件

```cpp
#include "EthernetMasterApi.h"
```

### 3. 链接库文件

根据系统架构选择对应的库文件：
- **x86_64**：`lib/x86_64/libethernet_master.so`
- **aarch64**：`lib/aarch64/libethernet_master.so`

### 4. 基本使用示例

```cpp
#include "EthernetMasterApi.h"
#include <iostream>

int main() {
    // 1. 初始化伺服配置
    ServoConfig config;
    for (size_t i = 0; i < ETHERNET_MASTER_MAX_MOTOR_NUM; i++) {
        config.isInverted[i] = 1;
        config.isVirtual[i] = false;
        config.reductionRatio_mol[i] = 1;
        config.reductionRatio_den[i] = 1;
    }
    
    // 2. 配置标幺数据转换参数（人形项目）
    AxisConversionConfig conversionConfig;
    conversionConfig.maxPosition = 650.0;
    conversionConfig.maxVelocity = 600.0;
    conversionConfig.maxTorque = 300.0;
    conversionConfig.maxPositionGain = 1000.0;
    conversionConfig.maxVelocityGain = 100.0;
    
    // 3. 初始化 Master（通讯周期为 1.0ms）
    const char* targetIp = "172.16.20.32";      // 目标IP地址
    uint16_t targetPort = 12345;                  // 目标端口
    const char* networkInterface = "eth1";      // 网络接口名称
    if (!MasterHandlerInit(config, conversionConfig, 1.0, 
                           targetIp, targetPort, networkInterface, RobotType::kHuman)) {
        std::cerr << "MasterHandler 初始化失败" << std::endl;
        return -1;
    }
    
    // 4. 启动通讯
    MasterStart();
    
    // 5. 发送控制命令
    LowerCmd cmd = {};
    cmd.motor_cmd[0].mode = 11;  // MIT 模式
    cmd.motor_cmd[0].q = 0.0;
    cmd.motor_cmd[0].dq = 0.0;
    cmd.motor_cmd[0].tau = 0.0;
    cmd.motor_cmd[0].kp = 50.0;
    cmd.motor_cmd[0].kd = 3.0;
    cmd.enableCmd[0] = true;
    MasterCmd(&cmd);
    
    // 6. 获取反馈状态
    LowerState state = {};
    MasterState(&state);
    std::cout << "Position: " << state.motor_state[0].q << std::endl;
    
    // 7. 停止通讯
    MasterStop();
    
    return 0;
}
```

## 📋 系统要求

### 硬件要求

- Linux 系统（推荐 Ubuntu 18.04+ 或 Debian 10+）
- 支持 UDP 通讯的网络接口
- x86_64 或 aarch64 架构处理器

### 软件依赖

- CMake 3.10 或更高版本（可选，用于构建示例项目）
- C++17 编译器（GCC 7+ 或 Clang 5+）
- pthread 库（系统自带）
- rt 库（可选，用于实时调度）

## 🔧 集成到项目

### 使用 CMake

```cmake
# 设置 SDK 路径
set(ETHERNET_MASTER_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ethernet-master-sdk)
set(ETHERNET_MASTER_SDK_INCLUDE_DIR ${ETHERNET_MASTER_SDK_DIR}/include)

# 自动检测架构
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(ETHERNET_MASTER_SDK_LIB_DIR ${ETHERNET_MASTER_SDK_DIR}/lib/x86_64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
    set(ETHERNET_MASTER_SDK_LIB_DIR ${ETHERNET_MASTER_SDK_DIR}/lib/aarch64)
endif()

# 添加头文件目录
target_include_directories(your_target PRIVATE
    ${ETHERNET_MASTER_SDK_INCLUDE_DIR}
)

# 链接库
target_link_libraries(your_target PRIVATE
    ${ETHERNET_MASTER_SDK_LIB_DIR}/libethernet_master.so
    pthread
    rt
)
```

### 使用 Makefile

```makefile
# SDK 路径
SDK_DIR = ./ethernet-master-sdk
SDK_INCLUDE_DIR = $(SDK_DIR)/include

# 自动检测架构
ARCH := $(shell uname -m)
ifeq ($(ARCH),x86_64)
    SDK_LIB_DIR = $(SDK_DIR)/lib/x86_64
else ifeq ($(ARCH),aarch64)
    SDK_LIB_DIR = $(SDK_DIR)/lib/aarch64
endif

# 编译选项
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
INCLUDES = -I$(SDK_INCLUDE_DIR)
LIBS = -L$(SDK_LIB_DIR) -lethernet_master -lpthread -lrt
```

## 📚 文档

详细的使用文档请参考：[docs/SDK使用文档.md](docs/SDK使用文档.md)

文档包含：
- 完整的 API 参考
- 数据结构说明
- 详细的使用示例
- 常见问题解答
- 故障排查指南

## 🏗️ 架构支持

| 架构 | 目录 | 适用系统 |
|------|------|----------|
| x86_64 | `lib/x86_64/` | Intel/AMD 64位处理器（PC、服务器） |
| aarch64 | `lib/aarch64/` | ARM 64位处理器（树莓派、Jetson、ARM 服务器） |

## 🔍 验证安装

```bash
# 检测系统架构
ARCH=$(uname -m)
echo "System architecture: $ARCH"

# 检查头文件
ls -l include/EthernetMasterApi.h

# 检查库文件
ls -l lib/${ARCH}/libethernet_master.so

# 检查库依赖
ldd lib/${ARCH}/libethernet_master.so

# 检查导出符号
nm -D lib/${ARCH}/libethernet_master.so | grep Master
```

应该能看到以下导出函数：
- `MasterHandlerInit`
- `MasterStart`
- `MasterCmd`
- `MasterState`
- `MasterStop`

## ❓ 常见问题

### Q: 如何选择正确的架构库文件？

A: 使用 `uname -m` 命令检测系统架构，然后选择对应的库文件目录。

### Q: 编译时找不到头文件？

A: 确保在 CMakeLists.txt 或 Makefile 中正确设置了 `INCLUDE_DIR` 路径。

### Q: 运行时找不到库文件？

A: 设置 `LD_LIBRARY_PATH` 环境变量，或使用 `rpath` 在编译时指定库路径。

更多问题请参考 [详细文档](docs/SDK使用文档.md) 中的常见问题章节。

## 📝 API 概览

### 初始化与启动

- `MasterHandlerInit()` - 初始化 Master 处理器（返回 bool 表示是否成功）
  - 参数：`ServoConfig`, `AxisConversionConfig`, `intervalMs`, `targetIp`, `targetPort`, `networkInterface`,`RobotType`
- `MasterStart()` - 启动通讯线程

### 数据交互

- `MasterCmd()` - 下发伺服控制命令（线程安全）
- `MasterState()` - 获取伺服反馈状态（线程安全）

### 停止与清理

- `MasterStop()` - 停止通讯线程并清理资源

详细 API 说明请参考 [详细文档](docs/SDK使用文档.md)。

## 📄 许可证

本 SDK 为专有软件，版权所有。使用前请确保已获得相应的使用许可。

## 👥 维护者

- chuhaoxian

## 📅 版本信息

- **SDK 版本**: 1.0.0
- **最后更新**: 2026

---

**需要帮助？** 请查看 [详细使用文档](docs/SDK使用文档.md) 或联系维护者。

