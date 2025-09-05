#ifdef MESSAGECORE
#pragma once

#ifdef ANDROID
#include <android/sharedmem.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#endif

#include <atomic>
#include <chrono>
#include <cstdint>
#include <string>
#include <thread>

struct alignas(64) MemoryPoolIndex {
    uint64_t flag;
    std::atomic<uint32_t> head;
    std::atomic<uint32_t> size;
    std::atomic<uint32_t> sizes_[2];
    std::atomic<uint32_t> frontIndex_;
};

#define FRAME_COUNT 2
#define MAX_HEADER_SIZE 64
#define FRAME_MAX_DATA_SIZE (5 * 1024 * 1024) // 5MB
#define MEMORY_POOL_INDEX_SIZE sizeof(struct MemoryPoolIndex)

struct alignas(64) Frame {
    uint64_t flag;
    uint32_t header_size;
    uint32_t data_size;
    uint32_t width;
    uint32_t height;
    uint8_t header[MAX_HEADER_SIZE];
    uint8_t data[FRAME_MAX_DATA_SIZE];
};

class SharedMemoryPool {
  private:
    int shm_fd = -1;
    Frame *frames = nullptr;
    MemoryPoolIndex *m_ringbuffer = nullptr;
    std::string shm_name;

    // socket相关
    int listen_fd = -1;
    std::thread socket_thread;
    bool running = false;

    // 私有方法
    void socketListenThread();
    void sendShareMemoryFD(int socket_fd);
    bool startSocketServer(const char *sockname);

  public:
    SharedMemoryPool() = default;
    ~SharedMemoryPool();

    // create 内部自动完成共享内存和socket监听初始化
    bool create(const char *name);
    void writeImg(const void *header, uint32_t header_size, const void *data, size_t data_size, uint32_t width, uint32_t height);
    void release();
    int getFD() { return shm_fd; }
};

class SharedMemoryDoubleBuffer {
  private:
    int shm_fd = -1;
    Frame *frames = nullptr;
    MemoryPoolIndex *m_ringbuffer = nullptr;
    std::string shm_name;

    // socket相关
    int listen_fd = -1;
    std::thread socket_thread;
    bool running = false;

    // 私有方法
    void socketListenThread();
    void sendShareMemoryFD(int socket_fd);
    bool startSocketServer(const char *sockname);

  public:
    SharedMemoryDoubleBuffer() = default;
    ~SharedMemoryDoubleBuffer();

    // create 内部自动完成共享内存和socket监听初始化
    bool create(const char *name);
    void writeImg(const void *header, uint32_t header_size, const void *data, size_t data_size, uint32_t width, uint32_t height);
    void release();
    int getFD() { return shm_fd; }
};
#endif

#ifdef MRAGENT
#pragma once

#ifdef ANDROID
#include <android/sharedmem.h>
#include <sys/mman.h>
#include <unistd.h>
#endif

#include <atomic>
#include <chrono>
#include <string>

struct alignas(64) MemoryPoolIndex {
    uint64_t flag; // 用于调试，标记是否有效
    std::atomic<uint32_t> head;
    std::atomic<uint32_t> size;
    std::atomic<uint32_t> sizes_[2];
    std::atomic<uint32_t> frontIndex_;
};

#define FRAME_COUNT 2
#define MAX_HEADER_SIZE 64
#define FRAME_MAX_DATA_SIZE (5 * 1024 * 1024) // 5MB
#define MEMORY_POOL_INDEX_SIZE sizeof(struct MemoryPoolIndex)

struct alignas(64) Frame {
    uint64_t flag; // 用于调试，标记是否有效
    uint32_t header_size;
    uint32_t data_size;
    uint32_t width;
    uint32_t height;
    uint8_t header[MAX_HEADER_SIZE];
    uint8_t data[FRAME_MAX_DATA_SIZE];
};

class SharedMemoryPool {
  private:
    std::atomic_bool initialized{false};
    int shm_fd = -1;
    Frame *frames = nullptr;
    MemoryPoolIndex *m_ringbuffer = nullptr;
    std::string shm_name;
    int pool_empty_read_count = 0;
    std::chrono::time_point<std::chrono::system_clock> pool_empty_read_time;

  public:
    bool attach(const char *name);
    bool attached() const { return shm_fd > 0; }
    int readImg(void *out_header, void *out_data, uint32_t data_size, uint32_t &width, uint32_t &height);
    void release();
};

class SharedMemoryDoubleBuffer {
  private:
    std::atomic_bool initialized{false};
    int shm_fd = -1;
    Frame *frames = nullptr;
    MemoryPoolIndex *m_ringbuffer = nullptr;
    std::string shm_name;
    int pool_empty_read_count = 0;
    std::chrono::time_point<std::chrono::system_clock> pool_empty_read_time;

  public:
    bool attach(const char *name);
    bool attached() const { return shm_fd > 0; }
    int readImg(void *out_header, void *out_data, uint32_t data_size, uint32_t &width, uint32_t &height);
    void release();
};
#endif
