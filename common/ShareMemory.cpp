#ifdef MESSAGECORE
#include "ShareMemory.h"

#include <cstdio>
#include <cstring>

#include "ImiMrLog.h"

#ifdef ANDROID
#include <errno.h>
#endif

static const size_t total_size = MEMORY_POOL_INDEX_SIZE + FRAME_COUNT * sizeof(Frame);

SharedMemoryPool::~SharedMemoryPool() { release(); }

bool SharedMemoryPool::create(const char *name) {
#ifdef ANDROID
    LOGI("SharedMemoryPool::create [[%s]] total_size %lu", name, total_size);
    shm_name = name;
    shm_fd = ASharedMemory_create(name, total_size);
    if (shm_fd < 0) {
        LOGE("SharedMemoryPool::create [[%s]] failed to create shared memory: %d", shm_name.c_str(), errno);
        return false;
    }
    void *addr = mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        LOGE("SharedMemoryPool::create [[%s]] failed to mmap shared memory: %d", shm_name.c_str(), errno);
        close(shm_fd);
        shm_fd = -1;
        return false;
    }
    memset(addr, 0, total_size);
    m_ringbuffer = (MemoryPoolIndex *)addr;
    frames = (Frame *)((uint8_t *)addr + MEMORY_POOL_INDEX_SIZE);
    m_ringbuffer->flag = 12344321;
    m_ringbuffer->head.store(0, std::memory_order_release);
    m_ringbuffer->size.store(0, std::memory_order_release);

    // 启动socket监听线程，sockname建议与shm_name一致
    startSocketServer(shm_name.c_str());
    LOGI("SharedMemoryPool::create [[%s]] shared memory created successfully: fd=%d", shm_name.c_str(), shm_fd);
    return true;
#else
    return false;
#endif
}

void SharedMemoryPool::release() {
#ifdef ANDROID
    running = false;
    if (listen_fd > 0) {
        close(listen_fd);
        listen_fd = -1;
    }
    if (socket_thread.joinable()) { socket_thread.join(); }
    if (m_ringbuffer != nullptr) {
        munmap(m_ringbuffer, total_size);
        m_ringbuffer = nullptr;
        frames = nullptr;
    }
    if (shm_fd > 0) {
        close(shm_fd);
        shm_fd = -1;
    }
    LOGI("SharedMemoryPool::release [[%s]] released successfully", shm_name.c_str());
#endif
}

void SharedMemoryPool::writeImg(const void *header, uint32_t header_size, const void *data, size_t data_size, uint32_t width, uint32_t height) {
#ifdef ANDROID
    while (shm_fd > 0 && m_ringbuffer != nullptr && frames != nullptr) {
        uint32_t size = m_ringbuffer->size.load(std::memory_order_acquire);
        if (size >= FRAME_COUNT - 1) {
            uint32_t head = m_ringbuffer->head.load(std::memory_order_acquire);
            Frame *frame = &frames[(head + FRAME_COUNT - 1) % FRAME_COUNT]; // 头部前的位置作为临时插入位置
            frame->flag = 12344321;
            memcpy(frame->header, header, header_size);
            frame->header_size = header_size;
            memcpy(frame->data, data, data_size);
            frame->data_size = data_size;
            frame->width = width;
            frame->height = height;
            uint32_t new_head = (head + 1) % FRAME_COUNT;
            if (m_ringbuffer->head.compare_exchange_strong(head, new_head, std::memory_order_release)) {
                // LOGI("MessageCore::SharedMemoryPool::writeImg [[%s]] buffer is full frameNum=%d headerSize=%d dataSize=%d", shm_name.c_str(), p_header->frameNum, frame->header_size, frame->data_size);
                return;
            }
        } else {
            uint32_t write_pos = (m_ringbuffer->head.load(std::memory_order_acquire) + size) % FRAME_COUNT;
            Frame *frame = &frames[write_pos];
            frame->flag = 12344321;
            memcpy(frame->header, header, header_size);
            frame->header_size = header_size;
            memcpy(frame->data, data, data_size);
            frame->data_size = data_size;
            frame->width = width;
            frame->height = height;
            if (m_ringbuffer->size.compare_exchange_strong(size, size + 1, std::memory_order_release)) {
                return;
            }
        }
        // 如果无法成功写入，再重试
    }
#endif
}

// 私有：socket监听线程
void SharedMemoryPool::socketListenThread() {
#ifdef ANDROID
    int epfd = epoll_create(1);
    struct epoll_event even;
    even.events = EPOLLIN;
    even.data.fd = listen_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &even);
    struct epoll_event evens[100];
    int len = sizeof(evens) / sizeof(struct epoll_event);
    running = true;
    LOGI("SharedMemoryPool:: %s, socketListenThread started, listening on fd: %d", shm_name.c_str(), listen_fd);
    while (running) {
        //LOGI("SharedMemoryPool::socketListenThread waiting for events...");
        // 等待事件，超时1秒
        int num = epoll_wait(epfd, evens, len, 1000);
        for (int i = 0; i < num; i++) {
            int curfd = evens[i].data.fd;
            if (curfd == listen_fd) {
                int cfd = accept(listen_fd, NULL, NULL);
                struct epoll_event even;
                even.events = EPOLLIN;
                even.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &even);
                LOGI("SharedMemoryPool::%s, socketListenThread accept: cfd=%d", shm_name.c_str(), cfd);
            } else {
                char buf[64] = {0};
                int rlen = read(curfd, buf, sizeof(buf));
                if (rlen > 0) {
                    LOGI("SharedMemoryPool::%s socketListenThread read: buf=[%s], rlen=%d", shm_name.c_str(), buf, rlen);
                    if (strncmp(buf, shm_name.c_str(), shm_name.size()) == 0) {
                        sendShareMemoryFD(curfd);
                        LOGI("SharedMemoryPool::%s socketListenThread sent fd to client: %d", shm_name.c_str(), curfd);
                    }
                } else if (rlen == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    LOGE("SharedMemoryPool:: %s socketListenThread client disconnected: %d", shm_name.c_str(), curfd);
                    // 关闭连接
                    close(curfd);
                } else {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    LOGE("SharedMemoryPool::%s socketListenThread read error on client %d: %s", shm_name.c_str(), curfd, strerror(errno));
                    close(curfd);
                }
            }
        }
    }
#endif
}

// 私有：发送fd
void SharedMemoryPool::sendShareMemoryFD(int socket_fd) {
#ifdef ANDROID
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(int))];
    memset(buf, 0, sizeof(buf));
    char dummy = 'A';
    struct iovec io = {.iov_base = &dummy, .iov_len = 1};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(cmsg) = shm_fd;
    sendmsg(socket_fd, &msg, 0);
#endif
}

// 私有：启动socket监听
bool SharedMemoryPool::startSocketServer(const char *sockname) {
#ifdef ANDROID
    if (listen_fd > 0) return true;
    listen_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd < 0) return false;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    strncpy(addr.sun_path + 1, sockname, sizeof(addr.sun_path) - 2);
    int addrlen = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(sockname);
    int ret = ::bind(listen_fd, (struct sockaddr *)&addr, addrlen);
    LOGI("SharedMemoryPool::%s startSocketServer bind ret=%d, listen_fd=%d, errno=%d", shm_name.c_str(), ret, listen_fd, errno);
    if (ret < 0) {
        close(listen_fd);
        listen_fd = -1;
        return false;
    }
    // listen 前后
    ret = ::listen(listen_fd, 5);
    LOGI("SharedMemoryPool::%s startSocketServer listen ret=%d, listen_fd=%d, errno=%d", shm_name.c_str(), ret, listen_fd, errno);
    if (ret < 0) {
        close(listen_fd);
        listen_fd = -1;
        return false;
    }
    socket_thread = std::thread(&SharedMemoryPool::socketListenThread, this);
    return true;
#else
    return false;
#endif
}

SharedMemoryDoubleBuffer::~SharedMemoryDoubleBuffer() { release(); }

bool SharedMemoryDoubleBuffer::create(const char *name) {
#ifdef ANDROID
    LOGI("SharedMemoryDoubleBuffer::create [[%s]] total_size %lu", name, total_size);
    shm_name = name;
    shm_fd = ASharedMemory_create(name, total_size);
    if (shm_fd < 0) {
        LOGE("SharedMemoryDoubleBuffer::create [[%s]] failed to create shared memory: %d", shm_name.c_str(), errno);
        return false;
    }
    void *addr = mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        LOGE("SharedMemoryDoubleBuffer::create [[%s]] failed to mmap shared memory: %d", shm_name.c_str(), errno);
        close(shm_fd);
        shm_fd = -1;
        return false;
    }
    memset(addr, 0, total_size);
    m_ringbuffer = (MemoryPoolIndex *)addr;
    frames = (Frame *)((uint8_t *)addr + MEMORY_POOL_INDEX_SIZE);
    m_ringbuffer->flag = 12344321;
    m_ringbuffer->head.store(0, std::memory_order_release);
    m_ringbuffer->size.store(0, std::memory_order_release);
    m_ringbuffer->sizes_[0].store(0, std::memory_order_release);
    m_ringbuffer->sizes_[1].store(0, std::memory_order_release);
    m_ringbuffer->frontIndex_.store(0, std::memory_order_release);

    // 启动socket监听线程，sockname建议与shm_name一致
    startSocketServer(shm_name.c_str());
    LOGI("SharedMemoryDoubleBuffer::create [[%s]] shared memory created successfully: fd=%d", shm_name.c_str(), shm_fd);
    return true;
#else
    return false;
#endif
}

void SharedMemoryDoubleBuffer::release() {
#ifdef ANDROID
    running = false;
    if (listen_fd > 0) {
        close(listen_fd);
        listen_fd = -1;
    }
    if (socket_thread.joinable()) { socket_thread.join(); }
    if (m_ringbuffer != nullptr) {
        munmap(m_ringbuffer, total_size);
        m_ringbuffer = nullptr;
        frames = nullptr;
    }
    if (shm_fd > 0) {
        close(shm_fd);
        shm_fd = -1;
    }
    LOGI("SharedMemoryDoubleBuffer::release [[%s]] released successfully", shm_name.c_str());
#endif
}

void SharedMemoryDoubleBuffer::writeImg(const void *header, uint32_t header_size, const void *data, size_t data_size, uint32_t width, uint32_t height) {
#ifdef ANDROID
    // Determine which buffer is currently front
    int oldFront = m_ringbuffer->frontIndex_.load(std::memory_order_acquire);
    int back = 1 - oldFront; // The other buffer is "back"
    // Write into back buffer
    Frame *frame = &frames[back];
    frame->flag = 12344321;
    frame->header_size = header_size;
    frame->data_size = data_size;
    frame->width = width;
    frame->height = height;
    memcpy(frame->header, header, header_size);
    memcpy(frame->data, data, data_size);
    // Update size of the back buffer
    m_ringbuffer->sizes_[back].store(header_size + data_size, std::memory_order_release);
    // Flip front index to point to the back
    m_ringbuffer->frontIndex_.store(back, std::memory_order_release);
#endif
}

// 私有：socket监听线程
void SharedMemoryDoubleBuffer::socketListenThread() {
#ifdef ANDROID
    int epfd = epoll_create(1);
    struct epoll_event even;
    even.events = EPOLLIN;
    even.data.fd = listen_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &even);
    struct epoll_event evens[100];
    int len = sizeof(evens) / sizeof(struct epoll_event);
    running = true;
    LOGI("SharedMemoryDoubleBuffer:: %s, socketListenThread started, listening on fd: %d", shm_name.c_str(), listen_fd);
    while (running) {
        //LOGI("SharedMemoryDoubleBuffer::socketListenThread waiting for events...");
        // 等待事件，超时1秒
        int num = epoll_wait(epfd, evens, len, 1000);
        for (int i = 0; i < num; i++) {
            int curfd = evens[i].data.fd;
            if (curfd == listen_fd) {
                int cfd = accept(listen_fd, NULL, NULL);
                struct epoll_event even;
                even.events = EPOLLIN;
                even.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &even);
                LOGI("SharedMemoryDoubleBuffer::%s, socketListenThread accept: cfd=%d", shm_name.c_str(), cfd);
            } else {
                char buf[64] = {0};
                int rlen = read(curfd, buf, sizeof(buf));
                if (rlen > 0) {
                    LOGI("SharedMemoryDoubleBuffer::%s socketListenThread read: buf=[%s], rlen=%d", shm_name.c_str(), buf, rlen);
                    if (strncmp(buf, shm_name.c_str(), shm_name.size()) == 0) {
                        sendShareMemoryFD(curfd);
                        LOGI("SharedMemoryDoubleBuffer::%s socketListenThread sent fd to client: %d", shm_name.c_str(), curfd);
                    }
                } else if (rlen == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    LOGE("SharedMemoryDoubleBuffer:: %s socketListenThread client disconnected: %d", shm_name.c_str(), curfd);
                    // 关闭连接
                    close(curfd);
                } else {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    LOGE("SharedMemoryDoubleBuffer::%s socketListenThread read error on client %d: %s", shm_name.c_str(), curfd, strerror(errno));
                    close(curfd);
                }
            }
        }
    }
#endif
}

// 私有：发送fd
void SharedMemoryDoubleBuffer::sendShareMemoryFD(int socket_fd) {
#ifdef ANDROID
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(int))];
    memset(buf, 0, sizeof(buf));
    char dummy = 'A';
    struct iovec io = {.iov_base = &dummy, .iov_len = 1};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    *(int *)CMSG_DATA(cmsg) = shm_fd;
    sendmsg(socket_fd, &msg, 0);
#endif
}

// 私有：启动socket监听
bool SharedMemoryDoubleBuffer::startSocketServer(const char *sockname) {
#ifdef ANDROID
    if (listen_fd > 0) return true;
    listen_fd = ::socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd < 0) return false;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    addr.sun_path[0] = '\0';
    strncpy(addr.sun_path + 1, sockname, sizeof(addr.sun_path) - 2);
    int addrlen = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(sockname);
    int ret = ::bind(listen_fd, (struct sockaddr *)&addr, addrlen);
    LOGI("SharedMemoryDoubleBuffer::%s startSocketServer bind ret=%d, listen_fd=%d, errno=%d", shm_name.c_str(), ret, listen_fd, errno);
    if (ret < 0) {
        close(listen_fd);
        listen_fd = -1;
        return false;
    }
    // listen 前后
    ret = ::listen(listen_fd, 5);
    LOGI("SharedMemoryDoubleBuffer::%s startSocketServer listen ret=%d, listen_fd=%d, errno=%d", shm_name.c_str(), ret, listen_fd, errno);
    if (ret < 0) {
        close(listen_fd);
        listen_fd = -1;
        return false;
    }
    socket_thread = std::thread(&SharedMemoryDoubleBuffer::socketListenThread, this);
    return true;
#else
    return false;
#endif
}
#endif

#ifdef MRAGENT
#include "ShareMemory.h"

#ifdef ANDROID
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#endif

#include <atomic>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "ImiMrLog.h"

static const size_t total_size = MEMORY_POOL_INDEX_SIZE + FRAME_COUNT * sizeof(Frame);

bool SharedMemoryPool::attach(const char *name) {
#ifdef ANDROID
    if (initialized.load(std::memory_order_acquire)) {
        LOGI("MRAgent::SharedMemoryPool::attach [[%s]] already attached", shm_name.c_str());
        return true;
    }
    shm_name = name; // 保存共享内存名称
    LOGI("MRAgent::SharedMemoryPool::attach [[%s]] total_size %lu", shm_name.c_str(), total_size);
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        LOGE("MRAgent::SharedMemoryPool::attach [[%s]] socket failed: %d", shm_name.c_str(), errno);
        return false;
    }
    {
        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        addr.sun_path[0] = '\0';
        strncpy(addr.sun_path + 1, name, sizeof(addr.sun_path) - 2);
        int addrlen = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(name);
        int ret = connect(sock_fd, (struct sockaddr *)&addr, addrlen);
        LOGI("MRAgent::SharedMemoryPool::attach connect ret=%d, sock_fd=%d, errno=%d", ret, sock_fd, errno);
        if (ret != 0) {
            LOGE("MRAgent::SharedMemoryPool::attach [[%s]] connect failed: %d", shm_name.c_str(), errno);
            close(sock_fd);
            return false;
        }
    }
    // 发送 shm_name 给服务端，触发 fd 传递
    write(sock_fd, name, strlen(name));

    // 接收 fd
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(int))];
    memset(buf, 0, sizeof(buf));
    char dummy;
    struct iovec io = {.iov_base = &dummy, .iov_len = 1};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    int n = recvmsg(sock_fd, &msg, 0);
    if (n <= 0) {
        LOGE("MRAgent::SharedMemoryPool::attach [[%s]] recvmsg failed: %d", shm_name.c_str(), errno);
        close(sock_fd);
        return false;
    }
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int)) && cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
        shm_fd = *(int *)CMSG_DATA(cmsg);
        LOGI("MRAgent::SharedMemoryPool::attach [[%s]] received shm_fd=%d", shm_name.c_str(), shm_fd);
    } else {
        LOGE("MRAgent::SharedMemoryPool::attach [[%s]] invalid fd received", shm_name.c_str());
        close(sock_fd);
        return false;
    }
    close(sock_fd); // 用完 socket 可以关闭

    // 然后再 mmap
    void *addr = mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        LOGE("MRAgent::SharedMemoryPool::attach [[%s]] failed to mmap shared memory: %d", shm_name.c_str(), errno);
        close(shm_fd);
        shm_fd = -1;
        return false;
    }
    m_ringbuffer = (MemoryPoolIndex *)addr;
    frames = (Frame *)((uint8_t *)addr + MEMORY_POOL_INDEX_SIZE);
    LOGI("MRAgent::SharedMemoryPool::attach [[%s]] shared memory attached successfully: fd=%d, frames=%p, m_ringbuffer=%p flag=%lu", shm_name.c_str(), shm_fd, frames, m_ringbuffer, m_ringbuffer->flag);
    initialized = true;
    return true;
#else
    return false;
#endif
}

int SharedMemoryPool::readImg(void *out_header, void *out_data, uint32_t data_size, uint32_t &width, uint32_t &height) {
#ifdef ANDROID
    if (initialized.load(std::memory_order_release) == false || shm_fd <= 0 || m_ringbuffer == nullptr || frames == nullptr) {
        LOGE("MRAgent::SharedMemoryPool::readImg [[%s]] shared memory not attached", shm_name.c_str());
        return -1;
    }
    uint32_t head = m_ringbuffer->head.load(std::memory_order_acquire);
    uint32_t size = m_ringbuffer->size.load(std::memory_order_acquire);
    if (size <= 0) {
        // LOGI("MRAgent::SharedMemoryPool::readImg [[%s]] buffer is empty", shm_name.c_str());
        if (pool_empty_read_count++ == 10) {
            pool_empty_read_time = std::chrono::system_clock::now();
        } else if (pool_empty_read_count++ >= 20) {
            auto now = std::chrono::system_clock::now();
            int diff = std::chrono::duration_cast<std::chrono::seconds>(now - pool_empty_read_time).count();
            if (diff > 3) {
                LOGE("MRAgent::SharedMemoryPool::readImg [[%s]] shared memory needs to be reset", shm_name.c_str());
                pool_empty_read_count = 0; // 重置空读计数
                return -1; // 返回错误，表示需要重置
            }
        }
        return 0;
    }
    pool_empty_read_count = 0; // 重置空读计数
    Frame *frame = &frames[head];
    uint32_t read_size = 0;
    if (out_header != nullptr) {
        memcpy(out_header, frame->header, frame->header_size);
        read_size += frame->header_size;
    }
    if (out_data != nullptr && data_size >= frame->data_size) {
        memcpy(out_data, frame->data, frame->data_size);
        read_size += frame->data_size;
    }
    width = frame->width;
    height = frame->height;
    uint32_t new_head = (head + 1) % FRAME_COUNT;
    if (read_size > 0 && m_ringbuffer->head.compare_exchange_strong(head, new_head, std::memory_order_acq_rel, std::memory_order_relaxed)) {
        m_ringbuffer->size.fetch_sub(1, std::memory_order_acq_rel);
        return read_size;
    }
    return 0;
#else
    return 0;
#endif
}

void SharedMemoryPool::release() {
#ifdef ANDROID
    if (initialized.load(std::memory_order_release) == false) { return; }
    if (m_ringbuffer != nullptr) {
        munmap(m_ringbuffer, total_size);
        m_ringbuffer = nullptr;
        frames = nullptr;
    }
    if (shm_fd > 0) {
        close(shm_fd);
        shm_fd = -1;
    }
    initialized.store(false, std::memory_order_release);
    LOGI("MRAgent::SharedMemoryPool::release [[%s]] shared memory released successfully", shm_name.c_str());
#endif
}

bool SharedMemoryDoubleBuffer::attach(const char *name) {
#ifdef ANDROID
    if (initialized.load(std::memory_order_acquire)) {
        LOGI("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] already attached", shm_name.c_str());
        return true;
    }
    shm_name = name; // 保存共享内存名称
    LOGI("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] total_size %lu", shm_name.c_str(), total_size);
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        LOGE("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] socket failed: %d", shm_name.c_str(), errno);
        return false;
    }
    {
        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        addr.sun_path[0] = '\0';
        strncpy(addr.sun_path + 1, name, sizeof(addr.sun_path) - 2);
        int addrlen = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(name);
        int ret = connect(sock_fd, (struct sockaddr *)&addr, addrlen);
        LOGI("MRAgent::SharedMemoryDoubleBuffer::attach connect ret=%d, sock_fd=%d, errno=%d", ret, sock_fd, errno);
        if (ret != 0) {
            LOGE("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] connect failed: %d", shm_name.c_str(), errno);
            close(sock_fd);
            return false;
        }
    }
    // 发送 shm_name 给服务端，触发 fd 传递
    write(sock_fd, name, strlen(name));

    // 接收 fd
    struct msghdr msg = {0};
    char buf[CMSG_SPACE(sizeof(int))];
    memset(buf, 0, sizeof(buf));
    char dummy;
    struct iovec io = {.iov_base = &dummy, .iov_len = 1};
    msg.msg_iov = &io;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof(buf);

    int n = recvmsg(sock_fd, &msg, 0);
    if (n <= 0) {
        LOGE("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] recvmsg failed: %d", shm_name.c_str(), errno);
        close(sock_fd);
        return false;
    }
    struct cmsghdr *cmsg = CMSG_FIRSTHDR(&msg);
    if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int)) && cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
        shm_fd = *(int *)CMSG_DATA(cmsg);
        LOGI("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] received shm_fd=%d", shm_name.c_str(), shm_fd);
    } else {
        LOGE("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] invalid fd received", shm_name.c_str());
        close(sock_fd);
        return false;
    }
    close(sock_fd); // 用完 socket 可以关闭

    // 然后再 mmap
    void *addr = mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (addr == MAP_FAILED) {
        LOGE("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] failed to mmap shared memory: %d", shm_name.c_str(), errno);
        close(shm_fd);
        shm_fd = -1;
        return false;
    }
    m_ringbuffer = (MemoryPoolIndex *)addr;
    frames = (Frame *)((uint8_t *)addr + MEMORY_POOL_INDEX_SIZE);
    LOGI("MRAgent::SharedMemoryDoubleBuffer::attach [[%s]] shared memory attached successfully: fd=%d, frames=%p, m_ringbuffer=%p flag=%lu", shm_name.c_str(), shm_fd, frames, m_ringbuffer, m_ringbuffer->flag);
    initialized = true;
    return true;
#else
    return false;
#endif
}

int SharedMemoryDoubleBuffer::readImg(void *out_header, void *out_data, uint32_t data_size, uint32_t &width, uint32_t &height) {
#ifdef ANDROID
    if (initialized.load(std::memory_order_release) == false || shm_fd <= 0 || m_ringbuffer == nullptr || frames == nullptr) {
        LOGE("MRAgent::SharedMemoryDoubleBuffer::readImg [[%s]] shared memory not attached", shm_name.c_str());
        return -1;
    }
    int currentFront = m_ringbuffer->frontIndex_.load(std::memory_order_acquire);
    uint32_t frontSize = m_ringbuffer->sizes_[currentFront].load(std::memory_order_acquire);
    if (frontSize <= 0) {
        // LOGI("MRAgent::SharedMemoryDoubleBuffer::readImg [[%s]] buffer is empty", shm_name.c_str());
        if (pool_empty_read_count++ == 10) {
            pool_empty_read_time = std::chrono::system_clock::now();
        } else if (pool_empty_read_count++ >= 20) {
            auto now = std::chrono::system_clock::now();
            int diff = std::chrono::duration_cast<std::chrono::seconds>(now - pool_empty_read_time).count();
            if (diff > 3) {
                LOGE("MRAgent::SharedMemoryDoubleBuffer::readImg [[%s]] shared memory needs to be reset", shm_name.c_str());
                pool_empty_read_count = 0; // 重置空读计数
                return -1; // 返回错误，表示需要重置
            }
        }
        return 0;
    }
    pool_empty_read_count = 0; // 重置空读计数
    Frame *frame = &frames[currentFront];
    uint32_t read_size = 0;
    if (out_header != nullptr) {
        memcpy(out_header, frame->header, frame->header_size);
        read_size += frame->header_size;
    }
    if (out_data != nullptr && data_size >= frame->data_size) {
        memcpy(out_data, frame->data, frame->data_size);
        read_size += frame->data_size;
    }
    width = frame->width;
    height = frame->height;
    m_ringbuffer->sizes_[currentFront].store(0, std::memory_order_release);
    return read_size;
#else
    return 0;
#endif
}

void SharedMemoryDoubleBuffer::release() {
#ifdef ANDROID
    if (initialized.load(std::memory_order_release) == false) { return; }
    if (m_ringbuffer != nullptr) {
        munmap(m_ringbuffer, total_size);
        m_ringbuffer = nullptr;
        frames = nullptr;
    }
    if (shm_fd > 0) {
        close(shm_fd);
        shm_fd = -1;
    }
    initialized.store(false, std::memory_order_release);
    LOGI("MRAgent::SharedMemoryDoubleBuffer::release [[%s]] shared memory released successfully", shm_name.c_str());
#endif
}
#endif
