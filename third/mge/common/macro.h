//
// Created by baifeng on 2022/1/19.
//

#ifndef SDL2_UI_MACRO_H
#define SDL2_UI_MACRO_H

#include <stdio.h>

#define mge_begin namespace mge {
#define mge_end }

#if defined(WIN32) or defined(__WIN32__)
#define GameApp WinMain
#elif defined(__PSP__)
#define GameApp SDL_main
#else
#define GameApp main
#endif

#define Startup(Class) \
int GameApp(int argc, char * argv[]) {  \
    Class game; \
    return game.run();  \
}

#endif //SDL2_UI_MACRO_H
