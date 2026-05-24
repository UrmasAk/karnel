//
// Created by jesper on 22.05.26.
//

#ifndef KARNEL_RENDER_H
#define KARNEL_RENDER_H

#include "limine.h"


class Renderer {
public:
    Renderer(limine_framebuffer *framebuffer_ptr, const uint8_t* font_ptr);
    void setString(char *string);
    void render();



private:

    limine_framebuffer *ptr_;
    char *string_{};

    void clearScreen_();
    void putText_();
};



#endif //KARNEL_RENDER_H
