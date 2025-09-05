# Changelog
## [v1.6.1] 2025-07-30
### Changed
1.修改U3D swig，完成指针传递，不拷贝数据，实现彩色图2k和1080p帧率24帧
2.修改usertrack和彩色图的可用zmq方式传输，方便pc调试

# Changelog
## [v1.5.4] 2025-07-23
### Changed
1.修改共享内存写入和读取，降低延时
2.修改bug : 1861 , 1862  
3.提升计算骨架效率，降低骨架显示延时


# Changelog
## [v1.5.3] 2025-07-16
### Changed
1.1080p彩色图解码优化，降低数据拷贝
2.usertrack加入共享内存


# Changelog
## [v1.5.2] 2025-07-9
### Changed
1.支持1080p彩色图
2.agent支持1080p和480p切换
3.解决彩色图崩溃问题

# Changelog
## [v1.5.1] 2025-07-2
### Changed
1.agent 加入硬解码
2.manager去掉软解码


## [v1.4.8] 2025-06-13
### Changed
1.agent 3s没收到新的数据自动重连manager
2.manager状态的改变更严格定义
3.去掉action sensor相关

# Changelog
## [v1.4.7] 2025-06-04
### Changed
1.加入心跳机制
2.加入帧号打印
3.加入U3d游戏低版本aar支持

## [v1.4.4] 2025-04-21 
### Changed
1.增加彩色图传输功能
2.增加配准打开关闭功能
3.增加color sensor打开与关闭时与manager的协议通讯


## [v1.4.3] 2025-03-31 
### Changed
1.修改agent内存增长bug
2.去掉每帧都有的打印
3.分布版本1.4.3

## [v1.4.2] 2025-03-28 
### Changed
- 修改agent usertrackersensor退出的方法，调用usertrackersensor.stop可退出该sensor



## [v1.4.1] 2015-3-24
1. 统一S500输出的数据结构体
2. 配合MrManager采用回调方式获取数据，减少帧延迟
3. 需配合MrManager1.4.1版本使用

## [v1.3.4] 2015-01-17 
### Changed
- 减少native层和JVM层之间的数据拷贝，改为内存地址传递
- 无数据时，阻塞消费者线程，有数据时，唤醒消费者线程

