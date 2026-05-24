//
// Created by jesper on 22.05.26.
//

#ifndef KARNEL_RENDER_H
#define KARNEL_RENDER_H
#define SSFN_CONSOLEBITMAP_TRUECOLOR    /* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_NO_CPP_STD_STRING

#include "limine.h"
#include "../inc/ssfn.hpp"


class Renderer {
public:
    Renderer(limine_framebuffer* framebuffer_ptr, const uint8_t* font_ptr);
    void setString(const char string[16]);
    void render();



private:

    uint8_t* ptr_;
    int w_;
    int h_;
    int p_;
    int x_;
    int y_;
    int fg_;
    char string_[16]{};

    void clearScreen_();
    void putText_();
};



#endif //KARNEL_RENDER_H
