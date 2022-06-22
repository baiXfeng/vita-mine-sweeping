//
// Created by baifeng on 2021/10/10.
//

#ifndef SDL2_UI_LOG_H
#define SDL2_UI_LOG_H

namespace LOG {
    enum {
        none = 0,
        info,
        error,
        debug,
        all,
    };
}

#if defined(__vita__)
#include <debugnet.h>

// console: socat udp-recv:18194 stdout
#define LOG_INIT() {    \
    debugNetInit("192.168.1.22", 18194, LOG::debug); \
}
#define LOG(format, args...) {  \
    debugNetPrintf(LOG::debug, format, ##args);    \
}
#define LOG_FINI() {    \
    debugNetFinish();   \
}
#else
#define LOG_INIT() (0)
#define LOG(format, args...) printf(format, ##args)
#define LOG_FINI() (0)
#endif

#define LOG_INFO(format, args...) LOG(format, ##args)
#define LOG_ERROR(format, args...) LOG(format, ##args)
#define LOG_DEBUG(format, args...) LOG(format, ##args)

#define ENABLE_LOG false

#if defined(__APPLE__)
#undef ENABLE_LOG
#define ENABLE_LOG true
#elif defined(__WIN32__)
#undef ENABLE_LOG
#define ENABLE_LOG true
#endif

#if not ENABLE_LOG
#undef LOG_INIT
#undef LOG
#undef LOG_FINI
#define LOG_INIT()
#define LOG(format, args...)
#define LOG_FINI()
#endif

#endif //SDL2_UI_LOG_H
