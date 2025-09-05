## 1. 项目概述
MrKit是北京视线科技打造的混合现实核心服务组件，旨在为开发者提供开箱即用的体感动作交互、语音指令识别等核心能力。通过开放用户的骨架信息、深度图数据流和模组API接口，降低MR应用开发门槛，让开发者能快速构建沉浸式游戏、工业模拟、医疗训练等创新应用。加入我们的开源生态，您将获得：

​多终端支持​：一套代码兼容Android/Windows/Unity3D平台
​高性能传输​：共享内存/ZMQ双通道可选数据传输，满足不同场景需求
​即插即用SDK​：提供C++/Java/C#多语言接口，集成仅需3步


### 1.1 Mrkit结构介绍
- MrKit作为核⼼服务组件，主要分为MrManager，MrAgent，MrCore三部分，其中MrAgent和MrCore作为客户需要集成的sdk为客户提供服务，而MrManager作为设备端部署的管理组件，负责与MrAgent和MrCore进行交互。

- MrManager作为设备端部署的管理组件，其通信组件MessageCore与MrAgent进行信令的交互与数据的传输

此项目目前分为MessageCore和MrManager组件

### 1.2 MessageCore介绍
- 作为核心服务端组件，负责管理和分发各类传感器数据
- MessageCore 是 cmd 通信的服务端，负责接收转发MrAgent 发送过来的 cmd 指令,cmd 通信采用zmq socket 的 router - dealer 通信方式，一个服务端对应多个客户端
- 支持共享内存和 ZMQ 两种数据传输方式，其中共享内存方式大幅提高传输速率；ZMQ方式保证manager和agent可通过网络进行通讯，保证数据调试
- 提供 C++/Java/C# 多语言接口
- 主要功能：数据采集、实时传输、数据通信

### 1.3 MRAgent介绍
- 作为客户端，负责接收和处理来自 MessageCore 的数据
- MrAgent 是 cmd 通信的客户端，负责将应用层的指令发送给MessageCore
- 支持多平台(Android/Windows/Unity3D)
- 提供统一的传感器数据访问接口
- 主要数据类型：Color、Depth、UserTrack、Skeleton 等

## 2. 目录结构


### 2.1 common
```
common/
├── cpptoml.h                # TOML 配置解析头文件
├── DoubleBuffer.h           # 双缓冲区实现
├── ImiMrDebugFile.cpp/h     # 调试文件相关实现
├── ShareMemory.cpp/h        # 共享内存实现
├── utils.cpp/h              # 通用工具函数
├── zmq.hpp                  # ZeroMQ C++头文件
├── 3rdparty/                # 第三方库（如libzmq）
├── scripts/                 # 相关脚本
```

### 2.2 MessageCore
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

### 2.3 MRAgent
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
### 2.4 doc

```
doc/
├── API接口使用说明                 # API使用说明文档
├── MrAgent-API-reference-Android.pdf # Android平台API参考文档
```

## 3. 构建系统
### 3.1 依赖管理
使用 xmake 管理 C++ 项目构建  
Gradle 管理 Android 项目
主要依赖:

| 依赖库   | 版本     | 开源协议      | 使用场景                |
|----------|---------|--------------|-------------------------|
| ZeroMQ   | 4.3.4   | LGPL-3.0     | 跨设备命令传输/数据流分发 |
| SWIG     | 4.1.1   | GPL-3.0      | 多语言接口生成           |



### 3.2 编译方法
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
cd MessageCore\android
编写local.properties，例子如下
    sdk.dir=e:\\dev\\SDK\\tools\\AndroidSdk
    ndk.dir=e:\\dev\\SDK\\tools\\AndroidSdk\\ndk\\25.2.9519653
gradle :app:assembleRelease --rerun-tasks
生成的aar文件在 MessageCore\android\app\build\outputs\aar 目录下
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

### 3.3 使用方法
#### Android 平台
将编译好的aar库放到相应mrmanager和mrangent_test中，编译运行即可

#### U3D 平台
将编译好的aar库放到相应mrmanager和mrangent_test中，编译运行即可



## 4. API接口使用说明 
见文档




## 5.注意事项
共享内存模式仅支持同一设备的进程间通信
跨设备通信必须使用 ZMQ 模式
Android 平台建议使用 arm64-v8a 架构
Unity3D 开发需确保插件架构匹配



## 6.合作与技术支持​
公司官网地址：https://www.sximi.com/
开发者文档中心地址：https://www.sximi.com/doc/
公众号：视线互娱
![二维码](./doc/image.jpeg)

SDK代码地址：https://github.com/Bjsxkj/SK510_mragent






