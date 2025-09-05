## 1. Project Overview
MrKit is a mixed reality core service component developed by Beijing Shixian Technology, aimed at providing developers with out-of-the-box capabilities such as gesture interaction and voice command recognition. By exposing user skeleton information, depth map data streams, and module API interfaces, it lowers the barrier to MR application development, enabling developers to quickly build immersive games, industrial simulations, medical training, and other innovative applications. Join our open-source ecosystem to get:

- Multi-platform Support: One codebase compatible with Android/Windows/Unity3D platforms
- High-performance Transmission: Shared memory/ZMQ dual-channel optional data transmission for different scenarios
- Plug-and-play SDK: Provides C++/Java/C# multi-language interfaces, integration in just 3 steps

### 1.1 Mrkit Structure Introduction
- MrKit, as a core service component, is mainly divided into MrManager, MrAgent, and MrCore. MrAgent and MrCore are SDKs that clients need to integrate for service provision, while MrManager is a management component deployed on the device side, responsible for interacting with MrAgent and MrCore.

- MrManager, as a device-side management component, uses its MessageCore communication component to interact with MrAgent for signaling and data transmission.

This project currently consists of MessageCore and MrManager components.

### 1.2 MessageCore Introduction
- As a core server component, responsible for managing and distributing various sensor data
- MessageCore is the server side of cmd communication, responsible for receiving and forwarding cmd instructions from MrAgent
- Cmd communication uses zmq socket's router-dealer communication method, with one server corresponding to multiple clients
- Supports both shared memory and ZMQ data transmission methods, where shared memory significantly improves transmission rates; ZMQ ensures network communication between manager and agent for data debugging
- Provides C++/Java/C# multi-language interfaces
- Main functions: data collection, real-time transmission, data communication

### 1.3 MRAgent Introduction
- As a client, responsible for receiving and processing data from MessageCore
- MrAgent is the client side of cmd communication, responsible for sending application layer instructions to MessageCore
- Supports multiple platforms (Android/Windows/Unity3D)
- Provides unified sensor data access interface
- Main data types: Color, Depth, UserTrack, Skeleton, etc.

## 2. Directory Structure

### 2.1 common
```
common/
├── cpptoml.h                # TOML configuration parsing header
├── DoubleBuffer.h           # Double buffer implementation
├── ImiMrDebugFile.cpp/h     # Debug file implementation
├── ShareMemory.cpp/h        # Shared memory implementation
├── utils.cpp/h              # Common utility functions
├── zmq.hpp                  # ZeroMQ C++ header
├── 3rdparty/                # Third-party libraries (e.g., libzmq)
├── scripts/                 # Related scripts
```

### 2.2 MessageCore
```
MessageCore/
├── cpp/                  # C++ core implementation
│   ├── include/          # Header files
│   ├── common/           # Common utilities
│   └── src/              # Source implementations
├── java_lang/            # Java interface implementation
│   ├── android/          # Android configuration
│   └── test/             # Java test code
├── test/                 # C++ test code
└── android/              # Android project configuration
```

### 2.3 MRAgent
```
MRAgent/
├── cpp/                  # C++ core implementation
│   ├── include/          # Header files
│   ├── common/          # Common utilities
│   ├── src/             # Source implementations
│   └── test/            # Test code
├── java_lang/           # Java/Android interface
├── csharp/              # C#/Unity interface
├── doc/                 # User documentation
└── doc-dev/             # Development documentation
```

### 2.4 doc
```
doc/
├── API Usage Guide                 # API usage documentation
├── MrAgent-API-reference-Android.pdf # Android platform API reference
```

## 3. Build System
### 3.1 Dependency Management
Uses xmake for C++ project building  
Gradle for Android project management
Main dependencies:

| Library  | Version  | License     | Use Case                |
|----------|----------|-------------|-------------------------|
| ZeroMQ   | 4.3.4    | LGPL-3.0    | Cross-device command transmission/data stream distribution |
| SWIG     | 4.1.1    | GPL-3.0     | Multi-language interface generation |

### 3.2 Build Methods
#### Windows Platform
```bash
# C++ Library
xmake config -p windows -a x64 --vs_runtime=MD
xmake build mragent_cpp

# C# Interface
xmake build mragent_cs
```

#### Android Platform
```bash
# Build arm64-v8a MRAgent
cd MRAgent\java_lang\android
Write local.properties, example:
    sdk.dir=e:\\dev\\SDK\\tools\\AndroidSdk
    ndk.dir=e:\\dev\\SDK\\tools\\AndroidSdk\\ndk\\25.2.9519653
gradle :app:assembleRelease --rerun-tasks
Generated aar file in MRAgent\java_lang\android\app\build\outputs\aar directory

# Build arm64-v8a MessageCore
cd MessageCore\android
Write local.properties, example:
    sdk.dir=e:\\dev\\SDK\\tools\\AndroidSdk
    ndk.dir=e:\\dev\\SDK\\tools\\AndroidSdk\\ndk\\25.2.9519653
gradle :app:assembleRelease --rerun-tasks
Generated aar file in MessageCore\android\app\build\outputs\aar directory
```

#### Unity3D Support
```bash
# Build Android Plugin
xmake config -p android -a arm64-v8a
xmake build mragent_cs

# Build Windows Plugin
xmake config -p windows -a x64
xmake build mragent_cs
```

### 3.3 Usage Instructions
#### Android Platform
Place the compiled aar libraries in the corresponding mrmanager and mrangent_test, then compile and run

#### Unity3D Platform
Place the compiled aar libraries in the corresponding mrmanager and mrangent_test, then compile and run

## 4. API Usage Instructions
See documentation

## 5. Important Notes
- Shared memory mode only supports inter-process communication on the same device
- Cross-device communication must use ZMQ mode
- Android platform recommends using arm64-v8a architecture
- Unity3D development requires ensuring plugin architecture matches

## 6. Cooperation and Technical Support
Company Website: https://www.sximi.com/
Developer Documentation Center: https://www.sximi.com/doc/
Official WeChat Account: 视线互娱
![QR Code](./doc/image.jpeg)

SDK Repository: https://github.com/Bjsxkj/SK510_mragent