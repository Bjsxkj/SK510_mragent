

## 1. 项目概述

 MrKit项目是北京视线科技公司开发的混合现实平台的核⼼服务组件，提供体感动作交互，语音交互等核⼼功能，对外可以输出用户的骨架信息、动作信息、语音指令信息， 为第三方游戏、MR应用程序提供方便快捷的 MR交互体验。 

### 1.1 Mrkit结构介绍
- MrKit作为核⼼服务组件，主要分为MrManager，MrAgent，MrCore三部分，其中MrAgent和MrCore作为客户需要集成的sdk为客户提供服务，而MrManager作为设备端部署的管理组件，负责与MrAgent和MrCore进行交互。

- MrManager作为设备端部署的管理组件，其通信组件MessageCore与MrAgent进行信令的交互与数据的传输

此项目目前分为MessageCore和MrManager组件

### 1.2 MessageCore介绍
- 作为核心服务端组件，负责管理和分发各类传感器数据
- 支持共享内存和 ZMQ 两种数据传输方式
- MessageCore 是 cmd 通信的服务端，负责接收转发MrAgent 发送过来的 cmd 指令,cmd 通信采用zmq socket 的 router - dealer 通信方式，一个服务端对应多个客户端
- 提供 C++/Java/C# 多语言接口
- 主要功能：数据采集、实时传输、进程间通信

### 1.3 MRAgent介绍
- 作为客户端，负责接收和处理来自 MessageCore 的数据
- MrAgent 是 cmd 通信的客户端，负责将应用层的指令发送给MessageCore
- 支持多平台(Android/Windows/Unity3D)
- 提供统一的传感器数据访问接口
- 主要数据类型：Color、Depth、UserTrack、Skeleton 等

## 2. 目录结构

### 2.1 MessageCore
```
MessageCore/
├── cpp/                  # C++核心实现
│   ├── include/          # 头文件
│   ├── common/           # 通用工具类
│   └── src/              # 源文件实现
├── java_lang/            # Java接口实现
│   ├── android/          # Android相关配置
│   └── test/             # Java测试代码
├── test/                 # C++测试代码
└── android/              # Android工程配置
```

### 2.2 MRAgent
```
MRAgent/
├── cpp/                  # C++核心实现
│   ├── include/          # 头文件
│   ├── common/           # 通用工具类
│   ├── src/              # 源文件实现
│   └── test/             # 测试代码
├── java_lang/            # Java/Android接口
├── csharp/               # C#/Unity接口
├── doc/                  # 用户文档
└── doc-dev/              # 开发文档
```

## 3. 核心模块说明

### 3.1 主要类与文件

#### MessageCore 端
- **[`MessageCore`](MessageCore/cpp/include/MessageCoreApi.h)** 
  - 核心类，管理数据分发和共享内存
  - 提供 initialize/start/stop/release 等生命周期管理
  - 处理 Color/UserTrack/Skeleton 等数据发布
  - 管理 cmd 通信 socket
  - 接收 MrAgent 发送过来的 cmd
  - 将请求转发给MrManager处理
  - 将对应的属性值发送给MrAgent
  - 通过接收客户端color sensor 发送过来的 color ack , 判断是否需要通知MrManager关闭释放color sensor

- **[`SharedMemoryPool`](MessageCore/cpp/common/ShareMemory.cpp)**
  - 共享内存池管理
  - 支持多路数据高效传输
  - 封装 fd 传递和 mmap 操作

- **[`MessageCoreApi`](MessageCore/cpp/src/MessageCoreApi.cpp)**
  - MessageCore::initialize 资源初始化
    - 建立socket
    - 建立zmq
    - 建立共享内存

  - MessageCore::release 释放资源
    - 释放socket
    - 释放zmq
    - 释放共享内存

  - MessageCore::isStarted 判断是否启动，返回 true：启动成功，返回 false：启动失败

  - MessageCore::start 启动MessageCore，开启后台线程

  - MessageCore::join 等待后台线程结束

  - MessageCore::stop 停止MessageCore

  - int MessageCore::publishColorFrame(unsigned char *array_ref, int array_len, int w, int h, int frameNum)
    根据配置文件使用zmq或共享内存传输Color
    -- unsigned char *array_ref：Color数据
    -- int array_len：Color数据大小
    -- int w：Color宽度
    -- int h：Color高度
    -- int frameNum：Color帧号
    -- 返回值：0：发送成功，非0：发送失败

  - int MessageCore::publishForegroundData(unsigned char *array_ref, int array_len, int w, int h, int frameNum)
    根据配置文件使用zmq或共享内存传输Foreground
    -- unsigned char *array_ref：Foreground数据
    -- int array_len：Foreground数据大小
    -- int w：Foreground宽度
    -- int h：Foreground高度
    -- int frameNum：Foreground帧号
    -- 返回值：0：发送成功，非0：发送失败

  - int MessageCore::publishBodySkeletonData(const imimr::ImiBodySkeleton& skeletons, int frameNum)
    根据配置文件使用zmq或共享内存传输BodySkeleton
    -- const imimr::ImiBodySkeleton& skeletons：BodySkeleton数据
    -- int frameNum：BodySkeleton帧号
    -- 返回值：0：发送成功，非0：发送失败

   - void MessageCore::bindCmdSocket()
    绑定 cmd server socket

  - void MessageCore::recvLoop()
    循环接收 MrAgent 发来的消息

  - void MessageCore::parseCmd(identity,request)
   -- const string identity：客户端身份标识
    -- zmq::message_t request: 接收到的消息
    解析处理消息
  

#### MRAgent 端
- **[`IImiBaseDevice`](MRAgent/cpp/src/ImiMrAgent.cpp)**
  - 初始化 cmd socket
  - 发送 cmd 消息
  - 接收 MessageCore 传过来的MrManager 状态

- **[`ImiMrCmd`](MRAgent/cpp/include/ImiMrCmd.h)**
  - 定义了 cmd 的结构体
  - 定义了cmd type 的类型，支持以下类型：

|   cmd type     | 功能       | 
|-------------|-------------|
| CMD_DEVICE_OPEN     | 打开device     |
| CMD_DEVICE_CLOSE     | 关闭device     | 
| CMD_COLOR_ACK     | color frame 确认     | 
| CMD_HEARTBEAT     |  心跳     | 
  

- **[`IImiColorSensor`](MRAgent/cpp/include/ImiColorSensor.h)**
  - Color像数据接收处理
  - 支持共享内存/ZMQ 双模式

- **[`IImiUserTrackSensor`](MRAgent/cpp/include/IImiUserTrackSensor.h)**
  - 用户跟踪数据接收处理
  - 包含骨骼和前景数据

- **[`ImiMrAgent`](MRAgent/cpp/src/ImiMrAgent.cpp)**
  - int IImiColorSensor::start(int mode)
    开启ColorSensor，设置Color分辨率，根据配置文件连接zmq或共享内存
    - in mode: 0：使用480P分辨率，1：使用1080P分辨率，2：使用2K分辨率
    -- 返回值：0：成功，非0：失败

  - int IImiColorSensor::stop()
    停止ColorSensor，根据配置文件释放zmq或共享内存
    -- 返回值：0：成功，非0：失败

  - ImiBaseFrame IImiColorSensor::readFrame(int timeout)
    读取Color，断连时会自动重连
    - in timeout: 超时，当前不支持
    -- 返回值：ImiBaseFrame，若读取成功ImiBaseFrame包含Color RGB数据，若读取失败则ImiBaseFrame包含大小为0的数据，根据ImiBaseFrame的帧号判断是否有重复

  - int IImiUserTrackSensor::start(int startFlags)
    开启UserTrackSensor，根据配置文件连接zmq或共享内存
    - int startFlags: 当前不支持
    -- 返回值：0：成功，非0：失败

  - int IImiUserTrackSensor::stop()
    停止UserTrackSensor，根据配置文件释放zmq或共享内存
    -- 返回值：0：成功，非0：失败

  - ImiBaseFrame IImiUserTrackSensor::readFrame(int timeout)
    读取UserTrack，断连时会自动重连
    - int timeout: 超时，当前不支持
    -- 返回值：ImiBaseFrame，若读取成功ImiBaseFrame包含UserTrack数据，若读取失败则ImiBaseFrame包含大小为0的数据，根据ImiBaseFrame的帧号判断是否有重复

  - void IImiBaseDevice::initCmdSocket()
    初始化cmd client socket
  
  - void IImiBaseDevice::recvLoop()
    循环接收处理 MessageCore 发送过来的消息

  - void IImiBaseDevice::sendCmdMsg(cmd_data_native& cmd)
    发送cmd 消息
    - cmd_data_native cmd: cmd 消息结构体
  
  - int IImiBaseDevice::setProperty(int propId, IImiDeviceProperty* prop)
    设置属性
    - int propId: 业务id
    - IImiDeviceProperty prop: 属性值
    -- 返回值：0：成功，非0：失败

  - int IImiBaseDevice::getProperty(int propId, IImiDeviceProperty* prop)
    获取属性
    - int propId: 业务id
    - IImiDeviceProperty prop: 属性值
    -- 返回值：0：成功，非0：失败

### 3.2 典型数据流

#### 共享内存模式
1. MessageCore 初始化:
```cpp
ColorPool.create("COLOR");
SkeletonPool.create("SKELETON");
ForegroudPool.create("FOREGROUND");
```

2. MRAgent 连接:
```cpp
color_pool.attach("COLOR");
skeleton_pool.attach("SKELETON");
foreground_pool.attach("FOREGROUND");
```

3. 数据传输
```cpp
// MessageCore 写入
ColorPool.writeImg(&header, sizeof(header), data, size, w, h);

// MRAgent 读取
color_pool.readImg(&header, data, size, w, h);
```

#### ZMQ 模式
用于非本机进程间通信
通过 TCP 协议传输数据
支持发布/订阅模式


## 4. 构建系统
### 4.1 依赖管理
使用 xmake 管理 C++ 项目构建
Gradle 管理 Android 项目
主要依赖:
ZMQ: 网络通信
Protobuf: 数据序列化
SWIG: 多语言接口生成

### 4.2 编译方法
#### Windows 平台
```bash
# C++ 库
xmake config -p windows -a x64 --vs_runtime=MD
xmake build mragent_cpp

# C# 接口
xmake build mragent_cs
```

#### Android 平台
```bash
# 编译 arm64-v8a MRAgent
cd MRAgent\java_lang\android
编写local.properties，例子如下
    sdk.dir=e:\\dev\\SDK\\tools\\AndroidSdk
    ndk.dir=e:\\dev\\SDK\\tools\\AndroidSdk\\ndk\\25.2.9519653
gradle :app:assembleRelease --rerun-tasks
生成的aar文件在 MRAgent\java_lang\android\app\build\outputs\aar 目录下

# 编译 arm64-v8a MessageCore
cd MessageCore\java_lang\android
编写local.properties，例子如下
    sdk.dir=e:\\dev\\SDK\\tools\\AndroidSdk
    ndk.dir=e:\\dev\\SDK\\tools\\AndroidSdk\\ndk\\25.2.9519653
gradle :app:assembleRelease --rerun-tasks
生成的aar文件在 MessageCore\java_lang\android\app\build\outputs\aar 目录下
```

#### Unity3D 支持
```bash
# 编译 Android 插件
xmake config -p android -a arm64-v8a
xmake build mragent_cs

# 编译 Windows 插件
xmake config -p windows -a x64
xmake build mragent_cs
```


## 5.注意事项
共享内存模式仅支持同一设备的进程间通信
跨设备通信必须使用 ZMQ 模式
Android 平台建议使用 arm64-v8a 架构
Unity3D 开发需确保插件架构匹配

