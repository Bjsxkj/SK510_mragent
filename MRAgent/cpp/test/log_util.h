// 打印时间耗时
#include <chrono>
#include <iostream>
#include <thread>

#define LOG_IX1(fmt,...) printf(fmt "\n", ## __VA_ARGS__)

#define CONCAT(a, b) a##b
#define UNIQUE_VAR(name) CONCAT(name, __LINE__)
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

static inline uint64_t getTimeInMsForNow()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

static struct TimeCost {
    uint64_t t0;
    std::string _msg;
    TimeCost(const std::string& msg){ t0 = getTimeInMsForNow(); _msg= msg; }
    ~TimeCost(){
        auto t1 = getTimeInMsForNow();
        LOG_IX1("%s: pid=%d, dt=%ld",  _msg.c_str(), std::this_thread::get_id(), t1 -t0); 
    }
};

template<typename... TT>
static std::string format1(const char* fmt1, TT... args){
    char buf[256];
    sprintf(buf, fmt1, args...);
    return std::string(buf);
}

#if 1
#define LOG_TS(name, fmt1,...)  TimeCost name = TimeCost(format1(fmt1, ## __VA_ARGS__));
#else
#define LOG_TS(name, fmt1,...)
#endif