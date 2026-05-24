#pragma once

#include "limine.h"
#include "pomodoro.hpp"
#include "string.hpp"


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

    String calculate_remaining_time(int remaining_time_sec);
    void fill_screen(uint32_t);
    void put_text(String &str);
};
