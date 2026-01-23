# Ethernet Master SDK 使用文档

> **本文档面向 SDK 使用者**  
> 本文档介绍如何使用 Ethernet Master SDK 进行伺服电机控制。如果您需要查看源码或进行二次开发，请参考其他相关文档。

## 目录

1. [概述](#概述)
2. [SDK 目录结构](#sdk-目录结构)
3. [SDK 库文件说明](#sdk-库文件说明)
4. [头文件引用](#头文件引用)
5. [链接 SDK 库](#链接-sdk-库)
6. [完整使用示例](#完整使用示例)
7. [API 详细说明](#api-详细说明)
8. [常见问题](#常见问题)
9. [附录](#附录)

---

## 概述

Ethernet Master SDK 是一个用于通过以太网（UDP）与下位机进行实时通讯的 C/C++ 动态链接库。该 SDK 提供了完整的伺服电机控制接口，支持最多 30 个电机的同步控制。

### 获取 SDK

SDK 以发布包形式提供，包含以下内容：
- **头文件**：`include/EthernetMasterApi.h` - 包含所有 API 声明
- **库文件**：`lib/libethernet_master.so` - 动态链接库
- **文档**：`docs/SDK使用文档.md` - 使用文档（本文档）

### 快速开始

1. 将 SDK 目录复制到您的项目中
2. 在代码中包含头文件：`#include "EthernetMasterApi.h"`
3. 根据系统架构链接对应的库文件：
   - x86_64 系统：链接 `lib/x86_64/libethernet_master.so`
   - aarch64 系统：链接 `lib/aarch64/libethernet_master.so`
   - 使用 `uname -m` 命令检测系统架构
4. 按照本文档的示例代码开始使用

**注意**：SDK 提供了两种架构的库文件，请确保使用与目标系统匹配的架构库文件。

### 主要特性

- ✅ **多电机支持**：最多支持 30 个伺服电机同时控制
- ✅ **实时通讯**：1ms 通讯周期，低延迟、高可靠性
- ✅ **线程安全**：采用双缓冲机制，确保多线程环境下的数据一致性
- ✅ **C 接口**：提供纯 C 接口，兼容 C/C++ 项目
- ✅ **动态链接**：以 `.so` 动态库形式提供，便于集成

---

## SDK 目录结构

SDK 发布包的标准目录结构如下：

```
ethernet-master-sdk/
├── include/                    # 头文件目录
│   └── EthernetMasterApi.h    # SDK 对外 API 头文件
├── lib/                        # 库文件目录
│   ├── x86_64/                 # x86_64 架构库文件
│   │   └── libethernet_master.so
│   └── aarch64/                # ARM 64位架构库文件
│       └── libethernet_master.so
└── docs/                       # 文档目录
    └── SDK使用文档.md          # 本文档
```

### 文件说明

- **`include/EthernetMasterApi.h`**：SDK 对外提供的唯一头文件，包含所有 API 声明和数据结构定义
- **`lib/x86_64/libethernet_master.so`**：x86_64 架构的动态链接库（适用于 Intel/AMD 64位处理器）
- **`lib/aarch64/libethernet_master.so`**：ARM 64位架构的动态链接库（适用于 ARM 64位处理器，如树莓派、Jetson 等）
- **`docs/SDK使用文档.md`**：SDK 使用文档（本文档）

### 架构选择

SDK 提供了两种架构的库文件，请根据您的目标系统架构选择合适的库文件：

- **x86_64**：适用于 Intel/AMD 64位处理器（常见于 PC 和服务器）
- **aarch64**：适用于 ARM 64位处理器（常见于嵌入式设备和 ARM 服务器）

**检测系统架构**：

```bash
# 查看系统架构
uname -m

# 输出示例：
# x86_64  -> 使用 lib/x86_64/libethernet_master.so
# aarch64 -> 使用 lib/aarch64/libethernet_master.so
```

---

## SDK 库文件说明

### 动态库文件

**文件名**：`libethernet_master.so`

**位置**：根据系统架构选择
- x86_64 架构：`lib/x86_64/libethernet_master.so`
- aarch64 架构：`lib/aarch64/libethernet_master.so`

**特性**：
- 动态链接库（Shared Object）
- 使用 C++17 标准编译
- 依赖系统库：`pthread`、`rt`（实时库）
- 符号可见性：仅导出 API 函数，内部符号隐藏
- **多架构支持**：提供 x86_64 和 aarch64 两种架构的库文件

### 库依赖

SDK 库依赖以下系统库：

- **pthread**：POSIX 线程库（必需）
- **rt**：实时扩展库（可选，用于实时调度）

### 库版本信息

查看库信息（根据您的系统架构选择对应的库文件）：

```bash
# 首先检测系统架构
ARCH=$(uname -m)

# 查看库的依赖关系
ldd lib/${ARCH}/libethernet_master.so

# 查看库的符号表（仅导出函数）
nm -D lib/${ARCH}/libethernet_master.so | grep ETHERNET_MASTER_API

# 查看库文件信息
file lib/${ARCH}/libethernet_master.so

# 或者直接指定架构
# x86_64 架构
ldd lib/x86_64/libethernet_master.so
# aarch64 架构
ldd lib/aarch64/libethernet_master.so
```

---

## 头文件引用

### 头文件位置

SDK 提供的唯一头文件：

```
include/EthernetMasterApi.h
```

（相对于 SDK 根目录）

### 在代码中引用

#### C++ 项目

```cpp
#include "EthernetMasterApi.h"
// 或者使用完整路径
#include "/path/to/ethernet-master-sdk/include/EthernetMasterApi.h"
```

#### C 项目

```c
#include "EthernetMasterApi.h"
```

**注意**：头文件使用 `extern "C"` 包装，在 C++ 项目中会自动处理，在 C 项目中直接使用即可。

### 头文件内容

头文件 `EthernetMasterApi.h` 包含：

1. **数据结构定义**：
   - `ServoConfig`：伺服配置结构
     - 提供减速比、电机方向以及虚实轴切换配置
     - 用于初始化时配置所有电机的基本参数
   - `AxisConversionConfig`：伺服标幺数据转换配置
     - 用于配置伺服数据的标幺换算参数
     - 支持人形项目和四足项目的不同配置
   - `MotorCmd`：单个电机控制命令
   - `MotorState`：单个电机反馈状态
   - `LowerCmd`：所有电机控制命令集合
   - `LowerState`：所有电机反馈状态集合
2. **API 函数声明**：
   - `MasterHandlerInit()`：初始化函数（需要六个参数，返回 bool 表示是否成功）
   - `MasterStart()`：启动通讯
   - `MasterCmd()`：发送控制命令
   - `MasterState()`：获取反馈状态
   - `MasterStop()`：停止通讯
3. **常量定义**：
   - `ETHERNET_MASTER_MAX_MOTOR_NUM`：最大电机数量（30）

---

## 链接 SDK 库

### 方法一：使用 CMake（推荐）

#### 步骤 1：设置 SDK 路径

在您的 `CMakeLists.txt` 中设置 SDK 路径：

```cmake
# 设置 SDK 路径（根据实际路径调整）
# 如果 SDK 在项目目录下
set(ETHERNET_MASTER_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ethernet-master-sdk)
# 或者如果 SDK 在其他位置
# set(ETHERNET_MASTER_SDK_DIR /path/to/ethernet-master-sdk)

set(ETHERNET_MASTER_SDK_INCLUDE_DIR ${ETHERNET_MASTER_SDK_DIR}/include)

# 自动检测系统架构并设置库文件路径
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(ETHERNET_MASTER_SDK_LIB_DIR ${ETHERNET_MASTER_SDK_DIR}/lib/x86_64)
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
    set(ETHERNET_MASTER_SDK_LIB_DIR ${ETHERNET_MASTER_SDK_DIR}/lib/aarch64)
else()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()
```

#### 步骤 2：添加头文件目录

```cmake
target_include_directories(your_target_name
    PRIVATE
        ${ETHERNET_MASTER_SDK_INCLUDE_DIR}
)
```

#### 步骤 3：链接库文件

```cmake
target_link_libraries(your_target_name
    PRIVATE
        ${ETHERNET_MASTER_SDK_LIB_DIR}/libethernet_master.so
        pthread
        rt
)
```

#### 完整 CMakeLists.txt 示例

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyEthernetApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 SDK 路径（根据实际路径调整）
set(ETHERNET_MASTER_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ethernet-master-sdk)
set(ETHERNET_MASTER_SDK_INCLUDE_DIR ${ETHERNET_MASTER_SDK_DIR}/include)

# 自动检测系统架构并设置库文件路径
if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
    set(ETHERNET_MASTER_SDK_LIB_DIR ${ETHERNET_MASTER_SDK_DIR}/lib/x86_64)
    message(STATUS "Using x86_64 library")
elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
    set(ETHERNET_MASTER_SDK_LIB_DIR ${ETHERNET_MASTER_SDK_DIR}/lib/aarch64)
    message(STATUS "Using aarch64 library")
else()
    message(FATAL_ERROR "Unsupported architecture: ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# 创建可执行文件
add_executable(my_app main.cpp)

# 添加头文件目录
target_include_directories(my_app
    PRIVATE
        ${ETHERNET_MASTER_SDK_INCLUDE_DIR}
)

# 链接库
target_link_libraries(my_app
    PRIVATE
        ${ETHERNET_MASTER_SDK_LIB_DIR}/libethernet_master.so
        pthread
        rt
)
```

### 方法二：使用 Makefile

#### 设置路径变量

```makefile
# SDK 路径
SDK_DIR = /path/to/ethernet-master-sdk
SDK_INCLUDE_DIR = $(SDK_DIR)/include

# 自动检测系统架构（根据 uname -m 输出）
ARCH := $(shell uname -m)
ifeq ($(ARCH),x86_64)
    SDK_LIB_DIR = $(SDK_DIR)/lib/x86_64
else ifeq ($(ARCH),aarch64)
    SDK_LIB_DIR = $(SDK_DIR)/lib/aarch64
else
    $(error Unsupported architecture: $(ARCH))
endif

# 编译选项
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
INCLUDES = -I$(SDK_INCLUDE_DIR)
LIBS = -L$(SDK_LIB_DIR) -lethernet_master -lpthread -lrt

# 目标文件
TARGET = my_app
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
```

#### 编译规则

```makefile
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
```

#### 完整 Makefile 示例

```makefile
# SDK 路径（根据实际路径调整）
SDK_DIR = ./ethernet-master-sdk
SDK_INCLUDE_DIR = $(SDK_DIR)/include

# 自动检测系统架构
ARCH := $(shell uname -m)
ifeq ($(ARCH),x86_64)
    SDK_LIB_DIR = $(SDK_DIR)/lib/x86_64
else ifeq ($(ARCH),aarch64)
    SDK_LIB_DIR = $(SDK_DIR)/lib/aarch64
else
    $(error Unsupported architecture: $(ARCH). Please use x86_64 or aarch64)
endif

# 编译器设置
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2
INCLUDES = -I$(SDK_INCLUDE_DIR)
LIBS = -L$(SDK_LIB_DIR) -lethernet_master -lpthread -lrt

# 目标
TARGET = my_app
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# 编译规则
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
```

### 方法三：使用 pkg-config（如果配置了）

如果 SDK 提供了 `pkg-config` 文件：

```bash
# 编译
g++ -std=c++17 main.cpp $(pkg-config --cflags --libs ethernet-master-sdk)
```

### 运行时库路径设置

编译完成后，运行时需要确保系统能找到 `.so` 库文件：

#### 方法 1：设置 LD_LIBRARY_PATH（临时）

```bash
# 自动检测架构
ARCH=$(uname -m)
export LD_LIBRARY_PATH=/path/to/ethernet-master-sdk/lib/${ARCH}:$LD_LIBRARY_PATH
./your_app

# 或者手动指定架构
# x86_64
export LD_LIBRARY_PATH=/path/to/ethernet-master-sdk/lib/x86_64:$LD_LIBRARY_PATH
# aarch64
export LD_LIBRARY_PATH=/path/to/ethernet-master-sdk/lib/aarch64:$LD_LIBRARY_PATH
```

#### 方法 2：复制到系统库目录（永久）

```bash
# 自动检测架构
ARCH=$(uname -m)
sudo cp /path/to/ethernet-master-sdk/lib/${ARCH}/libethernet_master.so /usr/local/lib/
sudo ldconfig

# 或者手动指定架构
# x86_64
sudo cp /path/to/ethernet-master-sdk/lib/x86_64/libethernet_master.so /usr/local/lib/
# aarch64
sudo cp /path/to/ethernet-master-sdk/lib/aarch64/libethernet_master.so /usr/local/lib/
sudo ldconfig
```

#### 方法 3：使用 rpath（推荐，编译时指定）

在 CMakeLists.txt 中：

```cmake
set_target_properties(your_target_name PROPERTIES
    INSTALL_RPATH "${ETHERNET_MASTER_SDK_LIB_DIR}"
    BUILD_WITH_INSTALL_RPATH TRUE
)
```

或在 Makefile 中：

```makefile
LIBS = -Wl,-rpath,$(SDK_LIB_DIR) -L$(SDK_LIB_DIR) -lethernet_master -lpthread -lrt
```

---

## 完整使用示例

### C++ 示例

```cpp
#include "EthernetMasterApi.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // 1. 初始化伺服配置
    ServoConfig config;
    for (size_t i = 0; i < ETHERNET_MASTER_MAX_MOTOR_NUM; i++) {
        config.isInverted[i] = 1;        // 电机方向：1=正转，-1=反转
        config.isVirtual[i] = false;      // 是否虚轴：false=实轴，true=虚轴
        config.reductionRatio_mol[i] = 1; // 减速比分子
        config.reductionRatio_den[i] = 1; // 减速比分母
    }
    
    // 2. 配置标幺数据转换参数
    // 人形项目配置
    AxisConversionConfig conversionConfig;
    conversionConfig.maxPosition = 650.0;      // 最大位置：±650.0 rad
    conversionConfig.maxVelocity = 600.0;      // 最大速度：±600.0 rad/s
    conversionConfig.maxTorque = 300.0;        // 最大力矩：±300.0 Nm
    conversionConfig.maxPositionGain = 1000.0; // 位置增益最大值：1000.0 (0.1Nm/rad)
    conversionConfig.maxVelocityGain = 100.0;  // 速度增益最大值：100.0 (0.1Nm/rad/s)
    
    /* 四足项目配置示例
    AxisConversionConfig conversionConfig;
    conversionConfig.maxPosition = 12.56637;   // 最大位置：±12.56637 rad (约±2π)
    conversionConfig.maxVelocity = 80.0;        // 最大速度：±80.0 rad/s
    conversionConfig.maxTorque = 150.0;         // 最大力矩：±150.0 Nm
    conversionConfig.maxPositionGain = 500.0;   // 位置增益最大值：500.0 (0.1Nm/rad)
    conversionConfig.maxVelocityGain = 50.0;    // 速度增益最大值：50.0 (0.1Nm/rad/s)
    */

    // 3. 初始化 Master（通讯周期为 1.0ms）
    const char* targetIp = "192.168.8.234";      // 目标IP地址
    uint16_t targetPort = 5000;                  // 目标端口
    const char* networkInterface = "eth0";      // 网络接口名称（如 "eth0", "eth1" 等）
    
    if (!MasterHandlerInit(config, conversionConfig, 1.0, 
                           targetIp, targetPort, networkInterface)) {
        std::cerr << "MasterHandler 初始化失败" << std::endl;
        return -1;
    }
    
    // 4. 启动通讯
    MasterStart();
    
    // 等待通讯建立
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // 5. 控制循环
    for (int cycle = 0; cycle < 1000; cycle++) {
        // 准备控制命令
        LowerCmd cmd = {};
        cmd.motor_cmd[0].mode = 11;  // MIT 模式
        cmd.motor_cmd[0].q = 0.0;
        cmd.motor_cmd[0].dq = 0.0;
        cmd.motor_cmd[0].tau = 0.0;
        cmd.motor_cmd[0].kp = 50.0;
        cmd.motor_cmd[0].kd = 3.0;
        cmd.enableCmd[0] = true;
        
        // 发送命令
        MasterCmd(&cmd);
        
        // 获取反馈
        LowerState state = {};
        MasterState(&state);
        
        // 打印状态
        if (cycle % 100 == 0) {
            std::cout << "Cycle " << cycle << ": "
                      << "Position = " << state.motor_state[0].q
                      << ", State = " << (int)state.motor_state[0].state
                      << std::endl;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    // 6. 停止
    MasterStop();
    return 0;
}
```

### C 示例

```c
#include "EthernetMasterApi.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    // 1. 初始化伺服配置
    ServoConfig config;
    for (int i = 0; i < ETHERNET_MASTER_MAX_MOTOR_NUM; i++) {
        config.isInverted[i] = 1;        // 电机方向：1=正转，-1=反转
        config.isVirtual[i] = false;      // 是否虚轴
        config.reductionRatio_mol[i] = 1;  // 减速比分子
        config.reductionRatio_den[i] = 1;  // 减速比分母
    }
    
    // 2. 配置标幺数据转换参数（人形项目）
    AxisConversionConfig conversionConfig;
    conversionConfig.maxPosition = 650.0;
    conversionConfig.maxVelocity = 600.0;
    conversionConfig.maxTorque = 300.0;
    conversionConfig.maxPositionGain = 1000.0;
    conversionConfig.maxVelocityGain = 100.0;

    // 3. 初始化 Master（通讯周期为 1.0ms）
    const char* targetIp = "192.168.8.234";      // 目标IP地址
    uint16_t targetPort = 5000;                  // 目标端口
    const char* networkInterface = "eth0";      // 网络接口名称
    
    if (!MasterHandlerInit(config, conversionConfig, 1.0, 
                           targetIp, targetPort, networkInterface)) {
        fprintf(stderr, "MasterHandler 初始化失败\n");
        return -1;
    }
    
    // 4. 启动通讯
    MasterStart();
    
    // 等待通讯建立
    sleep(1);
    
    // 5. 控制循环
    for (int cycle = 0; cycle < 1000; cycle++) {
        LowerCmd cmd = {0};
        cmd.motor_cmd[0].mode = 11;
        cmd.motor_cmd[0].q = 0.0f;
        cmd.motor_cmd[0].dq = 0.0f;
        cmd.motor_cmd[0].tau = 0.0f;
        cmd.motor_cmd[0].kp = 50.0f;
        cmd.motor_cmd[0].kd = 3.0f;
        cmd.enableCmd[0] = true;
        
        MasterCmd(&cmd);
        
        LowerState state = {0};
        MasterState(&state);
        
        if (cycle % 100 == 0) {
            printf("Cycle %d: Position = %f, State = %d\n",
                   cycle, state.motor_state[0].q, state.motor_state[0].state);
        }
        
        usleep(1000);  // 1ms
    }
    
    // 6. 停止
    MasterStop();
    return 0;
}
```

---

## 常见问题

### Q1: 编译时找不到头文件

**错误信息**：
```
fatal error: EthernetMasterApi.h: No such file or directory
```

**解决方法**：
1. 检查 SDK 路径是否正确
2. 确认 `CMakeLists.txt` 或 `Makefile` 中已正确设置 `INCLUDE_DIR`
3. 检查头文件是否存在于 SDK 的 `include/` 目录中

### Q2: 链接时找不到库文件

**错误信息**：
```
/usr/bin/ld: cannot find -lethernet_master
```

**解决方法**：
1. 检查库文件路径是否正确（注意架构目录：`lib/x86_64/` 或 `lib/aarch64/`）
2. 确认 `CMakeLists.txt` 或 `Makefile` 中已正确设置 `LIB_DIR`，并指向对应架构的目录
3. 检查库文件是否存在于 SDK 的 `lib/x86_64/` 或 `lib/aarch64/` 目录中
4. 确认系统架构与库文件架构匹配（使用 `uname -m` 检查）
5. 在 CMake 中使用完整路径：`${ETHERNET_MASTER_SDK_LIB_DIR}/libethernet_master.so`

### Q3: 运行时找不到库文件

**错误信息**：
```
error while loading shared libraries: libethernet_master.so: cannot open shared object file
```

**解决方法**：
1. 设置 `LD_LIBRARY_PATH` 环境变量：
   ```bash
   export LD_LIBRARY_PATH=/path/to/ethernet-master-sdk/lib:$LD_LIBRARY_PATH
   ```
2. 或复制库文件到系统目录：
   ```bash
   sudo cp /path/to/ethernet-master-sdk/lib/libethernet_master.so /usr/local/lib/
   sudo ldconfig
   ```
3. 或在编译时使用 `rpath`（推荐）

### Q4: 符号未定义错误

**错误信息**：
```
undefined reference to `MasterHandlerInit'
undefined reference to `MasterStart'
```

**解决方法**：
1. 确认已链接 `libethernet_master.so`
2. 确认链接顺序正确（库文件应在源文件之后）
3. 检查是否链接了必需的依赖库（`pthread`、`rt`）

### Q5: 如何验证 SDK 是否正确安装

**验证步骤**：

```bash
# 假设 SDK 在当前目录，根据实际路径调整
# 1. 检测系统架构
ARCH=$(uname -m)
echo "System architecture: $ARCH"

# 2. 检查头文件
ls -l include/EthernetMasterApi.h

# 3. 检查对应架构的库文件
ls -l lib/${ARCH}/libethernet_master.so

# 4. 检查库依赖
ldd lib/${ARCH}/libethernet_master.so

# 5. 检查导出符号
nm -D lib/${ARCH}/libethernet_master.so | grep Master

# 或者手动指定架构
# x86_64
ls -l lib/x86_64/libethernet_master.so
ldd lib/x86_64/libethernet_master.so
# aarch64
ls -l lib/aarch64/libethernet_master.so
ldd lib/aarch64/libethernet_master.so
```

应该能看到以下导出函数：
- `MasterHandlerInit`
- `MasterStart`
- `MasterCmd`
- `MasterState`
- `MasterStop`

### Q6: 如何选择正确的架构库文件

**问题**：SDK 提供了 x86_64 和 aarch64 两种架构的库文件，我应该使用哪个？

**解答**：

根据您的目标系统架构选择对应的库文件：

1. **检测系统架构**：
   ```bash
   uname -m
   ```
   - 输出 `x86_64` → 使用 `lib/x86_64/libethernet_master.so`
   - 输出 `aarch64` → 使用 `lib/aarch64/libethernet_master.so`

2. **常见系统对应关系**：
   - **x86_64**：Intel/AMD 64位处理器
     - 常见于：PC、服务器、大多数 Linux 桌面系统
   - **aarch64**：ARM 64位处理器
     - 常见于：树莓派 4/5、NVIDIA Jetson、ARM 服务器、嵌入式设备

3. **在 CMake 中自动选择**：
   ```cmake
   # 自动检测架构（已在文档示例中包含）
   if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|AMD64")
       set(ETHERNET_MASTER_SDK_LIB_DIR ${ETHERNET_MASTER_SDK_DIR}/lib/x86_64)
   elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64|ARM64")
       set(ETHERNET_MASTER_SDK_LIB_DIR ${ETHERNET_MASTER_SDK_DIR}/lib/aarch64)
   endif()
   ```

4. **在 Makefile 中自动选择**：
   ```makefile
   ARCH := $(shell uname -m)
   ifeq ($(ARCH),x86_64)
       SDK_LIB_DIR = $(SDK_DIR)/lib/x86_64
   else ifeq ($(ARCH),aarch64)
       SDK_LIB_DIR = $(SDK_DIR)/lib/aarch64
   endif
   ```

**注意**：如果使用了错误的架构库文件，会出现链接错误或运行时错误。

### Q7: 如何在不同项目中使用 SDK

**步骤**：

1. **将 SDK 复制到您的项目中**：
   ```bash
   cp -r ethernet-master-sdk /path/to/your/project/
   ```

2. **在项目中配置路径**：
   在您的 `CMakeLists.txt` 中：
   ```cmake
   set(ETHERNET_MASTER_SDK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ethernet-master-sdk)
   ```

3. **按照本文档的链接方法进行配置**

### Q8: 如何选择正确的 AxisConversionConfig 配置

**问题**：人形项目和四足项目的配置参数有什么区别？

**解答**：

不同项目类型需要不同的标幺换算参数，主要区别如下：

| 参数 | 人形项目 | 四足项目 | 说明 |
|------|---------|---------|------|
| `maxPosition` | 650.0 rad | 12.56637 rad | 人形项目需要更大的位置范围 |
| `maxVelocity` | 600.0 rad/s | 80.0 rad/s | 人形项目需要更高的速度 |
| `maxTorque` | 300.0 Nm | 150.0 Nm | 人形项目需要更大的力矩 |
| `maxPositionGain` | 1000.0 | 500.0 | 人形项目需要更高的位置增益 |
| `maxVelocityGain` | 100.0 | 50.0 | 人形项目需要更高的速度增益 |

**选择建议**：
- 如果您的项目是人形机器人，使用人形项目配置
- 如果您的项目是四足机器人，使用四足项目配置
- 如果您的项目是其他类型，请根据实际需求调整参数

**示例代码**：

```cpp
// 根据项目类型选择配置
AxisConversionConfig conversionConfig;

#ifdef HUMANOID_PROJECT
    // 人形项目配置
    conversionConfig.maxPosition = 650.0;
    conversionConfig.maxVelocity = 600.0;
    conversionConfig.maxTorque = 300.0;
    conversionConfig.maxPositionGain = 1000.0;
    conversionConfig.maxVelocityGain = 100.0;
#elif defined(QUADRUPED_PROJECT)
    // 四足项目配置
    conversionConfig.maxPosition = 12.56637;
    conversionConfig.maxVelocity = 80.0;
    conversionConfig.maxTorque = 150.0;
    conversionConfig.maxPositionGain = 500.0;
    conversionConfig.maxVelocityGain = 50.0;
#else
    // 自定义配置
    conversionConfig.maxPosition = 100.0;  // 根据实际需求调整
    conversionConfig.maxVelocity = 100.0;
    conversionConfig.maxTorque = 200.0;
    conversionConfig.maxPositionGain = 800.0;
    conversionConfig.maxVelocityGain = 80.0;
#endif
```

### Q9: intervalMs 参数应该设置为什么值？

**问题**：`MasterHandlerInit` 的第三个参数 `intervalMs` 应该设置为多少？

**解答**：

- **推荐值**：`1.0`（1 毫秒，对应 1000Hz 通讯频率）
- **说明**：`intervalMs` 表示通讯周期，单位为毫秒
  - `1.0` = 1ms 周期，1000Hz 频率（最常用）
  - `2.0` = 2ms 周期，500Hz 频率
  - `0.5` = 0.5ms 周期，2000Hz 频率（需要高性能系统）
- **注意事项**：
  - 值越小，通讯频率越高，对系统性能要求越高
  - 建议使用默认值 `1.0`，除非有特殊需求
  - 过小的值可能导致系统无法及时响应

### Q10: 如何配置网络参数（targetIp、targetPort、networkInterface）？

**问题**：`MasterHandlerInit` 需要提供网络参数，如何正确配置？

**解答**：

1. **targetIp（目标IP地址）**：
   - 下位机的 IP 地址，例如：`"192.168.8.234"`
   - 必须是有效的 IPv4 地址字符串
   - 确保与下位机在同一网络段

2. **targetPort（目标端口）**：
   - 下位机的 UDP 端口号，例如：`5000`
   - 范围：1-65535
   - 必须与下位机配置的端口一致

3. **networkInterface（网络接口）**：
   - 用于发送 UDP 数据包的网络接口名称
   - 常见值：`"eth0"`、`"eth1"`、`"wlan0"` 等
   - 查看可用接口：`ip addr show` 或 `ifconfig`
   - 选择连接到下位机的网络接口

**配置示例**：

```cpp
// 查看网络接口
// 在终端执行：ip addr show 或 ifconfig

// 示例配置
const char* targetIp = "192.168.8.234";      // 下位机IP
uint16_t targetPort = 5000;                  // 下位机端口
const char* networkInterface = "eth0";       // 网络接口

if (!MasterHandlerInit(config, conversionConfig, 1.0, 
                       targetIp, targetPort, networkInterface)) {
    std::cerr << "初始化失败" << std::endl;
    return -1;
}
```

**常见问题**：
- **初始化返回 false**：检查 IP 地址格式、端口范围、网络接口是否存在
- **无法通讯**：确认网络接口正确，IP 地址可达（使用 `ping` 测试）
- **找不到网络接口**：使用 `ip addr show` 查看系统可用的网络接口名称

---

## API 详细说明

### MasterHandlerInit

初始化 Master 处理器，配置伺服参数、标幺转换参数和网络通讯参数。

**函数签名**：
```c
bool MasterHandlerInit(
    ServoConfig config, 
    AxisConversionConfig conversionConfig, 
    double intervalMs, 
    const char* targetIp, 
    uint16_t targetPort, 
    const char* networkInterface
);
```

**参数说明**：
- `config`：伺服配置结构体，包含所有电机的基本配置信息
  - `isInverted[i]`：第 i 个电机是否反转（1=正转，-1=反转）
  - `isVirtual[i]`：第 i 个电机是否为虚轴（false=实轴，true=虚轴）
  - `reductionRatio_mol[i]`：第 i 个电机的减速比分子
  - `reductionRatio_den[i]`：第 i 个电机的减速比分母
- `conversionConfig`：标幺数据转换配置结构体，用于配置伺服数据的标幺换算参数
  - `maxPosition`：最大位置值（rad），用于位置数据的标幺换算
  - `maxVelocity`：最大速度值（rad/s），用于速度数据的标幺换算
  - `maxTorque`：最大力矩值（Nm），用于力矩数据的标幺换算
  - `maxPositionGain`：位置增益最大值，用于位置增益的标幺换算
  - `maxVelocityGain`：速度增益最大值，用于速度增益的标幺换算
- `intervalMs`：通讯周期（毫秒），通常设置为 1.0（1ms 周期，1000Hz 频率）
- `targetIp`：目标 IP 地址（字符串），下位机的 IP 地址，例如 `"192.168.8.234"`
- `targetPort`：目标端口号（uint16_t），下位机的 UDP 端口，例如 `5000`
- `networkInterface`：网络接口名称（字符串），用于发送 UDP 数据包的网络接口，例如 `"eth0"`、`"eth1"` 等

**返回值**：
- `true`：初始化成功
- `false`：初始化失败（参数无效或配置错误）

**说明**：
- 必须在调用其他 API 之前调用
- 只能调用一次，重复调用可能导致未定义行为
- 不同项目类型（人形/四足）需要配置不同的 `conversionConfig` 参数
- 如果返回 `false`，请检查错误信息并修正参数后重试
- `targetIp` 和 `networkInterface` 不能为 `NULL`，否则会返回 `false`

**使用示例**：

```cpp
// 配置网络参数
const char* targetIp = "192.168.8.234";      // 下位机IP地址
uint16_t targetPort = 5000;                  // 下位机端口
const char* networkInterface = "eth0";       // 网络接口

// 初始化并检查返回值
if (!MasterHandlerInit(config, conversionConfig, 1.0, 
                       targetIp, targetPort, networkInterface)) {
    std::cerr << "初始化失败，请检查参数配置" << std::endl;
    return -1;
}
```

**配置示例**：

```cpp
// 人形项目配置
AxisConversionConfig humanoidConfig;
humanoidConfig.maxPosition = 650.0;      // ±650.0 rad
humanoidConfig.maxVelocity = 600.0;      // ±600.0 rad/s
humanoidConfig.maxTorque = 300.0;       // ±300.0 Nm
humanoidConfig.maxPositionGain = 1000.0; // 1000.0 (0.1Nm/rad)
humanoidConfig.maxVelocityGain = 100.0;   // 100.0 (0.1Nm/rad/s)

// 四足项目配置
AxisConversionConfig quadrupedConfig;
quadrupedConfig.maxPosition = 12.56637;  // ±12.56637 rad (约±2π)
quadrupedConfig.maxVelocity = 80.0;       // ±80.0 rad/s
quadrupedConfig.maxTorque = 150.0;       // ±150.0 Nm
quadrupedConfig.maxPositionGain = 500.0;  // 500.0 (0.1Nm/rad)
quadrupedConfig.maxVelocityGain = 50.0;   // 50.0 (0.1Nm/rad/s)
```

---

## 附录

### API 快速参考

| 函数名 | 功能 | 参数 | 返回值 | 线程安全 |
|--------|------|------|--------|----------|
| `MasterHandlerInit()` | 初始化 | `ServoConfig`, `AxisConversionConfig`, `double intervalMs`, `const char* targetIp`, `uint16_t targetPort`, `const char* networkInterface` | `bool` | ❌ |
| `MasterStart()` | 启动通讯 | 无 | `void` | ❌ |
| `MasterCmd()` | 发送命令 | `const LowerCmd*` | `void` | ✅ |
| `MasterState()` | 获取状态 | `LowerState*` | `void` | ✅ |
| `MasterStop()` | 停止通讯 | 无 | `void` | ❌ |

### 数据结构详细说明

#### ServoConfig

伺服配置结构体，用于初始化时配置所有电机的基本参数。

```c
typedef struct {
    int isInverted[ETHERNET_MASTER_MAX_MOTOR_NUM];      // 电机是否反转（1/-1）
    bool isVirtual[ETHERNET_MASTER_MAX_MOTOR_NUM];      // 是否虚轴
    int reductionRatio_mol[ETHERNET_MASTER_MAX_MOTOR_NUM]; // 减速比分子
    int reductionRatio_den[ETHERNET_MASTER_MAX_MOTOR_NUM]; // 减速比分母
} ServoConfig;
```

**字段说明**：
- `isInverted[i]`：第 i 个电机的方向，`1` 表示正转，`-1` 表示反转
- `isVirtual[i]`：第 i 个电机是否为虚轴，`false` 表示实轴，`true` 表示虚轴
- `reductionRatio_mol[i]`：第 i 个电机的减速比分子
- `reductionRatio_den[i]`：第 i 个电机的减速比分母
- 减速比 = `reductionRatio_mol[i] / reductionRatio_den[i]`

#### AxisConversionConfig

伺服标幺数据转换配置结构体，用于配置伺服数据的标幺换算参数。

```c
typedef struct {
    double maxPosition;      // 最大位置（rad）
    double maxVelocity;      // 最大速度（rad/s）
    double maxTorque;        // 最大力矩（Nm）
    double maxPositionGain;  // 位置增益最大值
    double maxVelocityGain;  // 速度增益最大值
} AxisConversionConfig;
```

**字段说明**：
- `maxPosition`：位置数据的最大值（rad），用于位置命令和反馈的标幺换算
- `maxVelocity`：速度数据的最大值（rad/s），用于速度命令和反馈的标幺换算
- `maxTorque`：力矩数据的最大值（Nm），用于力矩命令和反馈的标幺换算
- `maxPositionGain`：位置增益的最大值，用于位置增益的标幺换算（单位：0.1Nm/rad）
- `maxVelocityGain`：速度增益的最大值，用于速度增益的标幺换算（单位：0.1Nm/rad/s）

**项目配置参考**：

| 项目类型 | maxPosition | maxVelocity | maxTorque | maxPositionGain | maxVelocityGain |
|---------|------------|-------------|-----------|----------------|----------------|
| 人形项目 | 650.0 rad | 600.0 rad/s | 300.0 Nm | 1000.0 | 100.0 |
| 四足项目 | 12.56637 rad | 80.0 rad/s | 150.0 Nm | 500.0 | 50.0 |

#### MotorCmd

单个电机的控制命令。

```c
typedef struct {
    uint8_t mode;  // 控制模式，MIT 模式为 11
    float q;       // 位置命令（rad）
    float dq;      // 速度命令（rad/s）
    float tau;     // 力矩命令（Nm）
    float kp;      // 比例增益
    float kd;      // 微分增益
} MotorCmd;
```

#### MotorState

单个电机的反馈状态。

```c
typedef struct {
    uint8_t state;      // 0-掉线 1-报错 2-失能 3-使能
    uint8_t mode;       // 当前控制模式
    float q;            // 当前位置（rad）
    float dq;           // 当前速度（rad/s）
    float ddq;          // 当前加速度（rad/s²）
    float tau_est;      // 估计力矩（Nm）
    float q_raw;        // 原始位置（rad）
    float dq_raw;       // 原始速度（rad/s）
    float ddq_raw;      // 原始加速度（rad/s²）
    uint8_t mcu_temp;   // MCU 温度（℃）
    uint8_t mos_temp;   // MOS 温度（℃）
    uint8_t motor_temp; // 电机温度（℃）
    uint8_t bus_voltage;// 总线电压（V）
    bool is_virtual;    // 是否为虚轴
    uint16_t error_code;// 错误代码
    uint16_t version;   // 版本号
} MotorState;
```

#### LowerCmd

所有电机的控制命令集合。

```c
typedef struct {
    MotorCmd motor_cmd[ETHERNET_MASTER_MAX_MOTOR_NUM];  // 电机控制命令数组
    bool enableCmd[ETHERNET_MASTER_MAX_MOTOR_NUM];     // 使能命令
    bool resetCmd[ETHERNET_MASTER_MAX_MOTOR_NUM];      // 复位命令
    bool caliCmd[ETHERNET_MASTER_MAX_MOTOR_NUM];       // 校准命令
    bool homingCmd[ETHERNET_MASTER_MAX_MOTOR_NUM];     // 回零命令
} LowerCmd;
```

#### LowerState

所有电机的反馈状态集合。

```c
typedef struct {
    MotorState motor_state[ETHERNET_MASTER_MAX_MOTOR_NUM]; // 电机状态数组
} LowerState;
```

### 数据结构快速参考

| 结构体 | 用途 | 使用场景 |
|--------|------|----------|
| `ServoConfig` | 伺服结构配置 | 初始化时配置电机基本参数 |
| `AxisConversionConfig` | 伺服数据换算配置 | 初始化时配置标幺换算参数 |
| `MotorCmd` | 单个电机控制命令 | 控制单个电机 |
| `MotorState` | 单个电机反馈状态 | 获取单个电机状态 |
| `LowerCmd` | 所有电机控制命令 | 发送所有电机控制命令 |
| `LowerState` | 所有电机反馈状态 | 获取所有电机反馈状态 |

### 常量定义

```c
#define ETHERNET_MASTER_MAX_MOTOR_NUM 30  // 最大电机数量
```

---

**文档版本**: 1.0  
**最后更新**: 2026  
**维护者**: chuhaoxian

