//
// Created by jesper on 22.05.26.
//

#ifndef KARNEL_RENDER_H
#define KARNEL_RENDER_H

#include "limine.h"
#include "pomodoro.hpp"
#include "string.hpp" 

namespace SSFN {
    class Font;
}

class Renderer {


public:
    Renderer(limine_framebuffer *framebuffer_ptr, const uint8_t* font_ptr);
    void render(Pomodoro::State state, int remaining_time_sec);

private:
    struct RGB {
        std::uint8_t blue;
        std::uint8_t green;
        std::uint8_t red;
        std::uint8_t alpha;
    };
    limine_framebuffer *ptr_;
    SSFN::Font *font;
    ssfn_buf_t *buf_;


    String calculate_remaining_time_(int remaining_time_sec);
    void fill_screen(uint32_t);
    void putText_(char *string);
    void putText_(String &str);
};



#endif //KARNEL_RENDER_H
