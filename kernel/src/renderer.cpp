#define SSFN_CONSOLEBITMAP_TRUECOLOR    /* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_NO_CPP_STD_STRING
#include "../inc/ssfn.hpp"
#include "../inc/renderer.hpp"
#include "../inc/string.hpp"

Renderer::Renderer(limine_framebuffer *framebuffer_ptr, const uint8_t *font_ptr) {
    ssfn_src = reinterpret_cast<ssfn_font_t*>(const_cast<uint8_t*>(font_ptr));      /* the bitmap font to use */

    ptr_ = framebuffer_ptr;
    ssfn_dst.ptr = static_cast<uint8_t*>(framebuffer_ptr->address);                  /* address of the linear frame buffer */
    ssfn_dst.w  = framebuffer_ptr->width;                          /* width */
    ssfn_dst.h = framebuffer_ptr->height;                           /* height */
    ssfn_dst.p = framebuffer_ptr->pitch;                          /* bytes per line */
    ssfn_dst.x = ssfn_dst.y = 0;                /* pen position */
    ssfn_dst.fg = 0xFFFFFF;                     /* foreground color */
}

void Renderer::render(Pomodoro::State state, int remaining_time_sec) {
    String time = calculate_remaining_time_(remaining_time_sec);
    RGB pixel = [state]() -> RGB {
        switch (state) {
            case Pomodoro::State::W_RUN: return RGB{0, 0, 128, 255};
            case Pomodoro::State::W_END: return RGB{0, 0, 32, 255};
            case Pomodoro::State::W_PAUSE: return RGB{0, 0, 64, 255};
            case Pomodoro::State::B_RUN: return RGB{0, 128, 0, 255};
            case Pomodoro::State::B_END: return RGB{0, 32, 0, 255};
            case Pomodoro::State::B_PAUSE: return RGB{0, 64, 0, 255};
            default: return RGB {0, 0, 0, 0};
        }
    }();

    uint32_t pixel_value =
        (static_cast<uint32_t>(pixel.alpha) << 24) |
        (static_cast<uint32_t>(pixel.red) << 16) |
        (static_cast<uint32_t>(pixel.green) << 8) |
        (static_cast<uint32_t>(pixel.blue));

    fill_screen(pixel_value);

    ssfn_dst.x = ssfn_dst.w/2 - 20;
    ssfn_dst.y = ssfn_dst.h/2;
    putText_(time);
}

String Renderer::calculate_remaining_time_(int remaining_time_sec) {
    int remaining_minutes = remaining_time_sec / 60;
    int volatile remaining_seconds = remaining_time_sec - (remaining_minutes * 60);
    String minutes_padding{};
    String seconds_padding{};

    if (remaining_seconds < 10) {
        if (remaining_seconds == 0) {
            seconds_padding = String{"00"};
        } else {
            seconds_padding = String{"0"};
        }
    }

    if (remaining_minutes < 10) {
        if (remaining_minutes == 0) {
            minutes_padding = String{"00"};
        } else {
            minutes_padding = String{"0"};
        }
    }

    String str = minutes_padding + String::toString(remaining_minutes) + String(":") + seconds_padding + String::toString(remaining_seconds);

    return str;
}

void Renderer::fill_screen(uint32_t pixel_value) {
    volatile auto *fb_ptr = static_cast<volatile uint32_t *>(ptr_->address);
    for (uint64_t y = 0; y < ptr_->height; y++) {
        for (uint64_t x = 0; x < ptr_->width; x++) {
            fb_ptr[y * (ptr_->pitch / 4) + x] = pixel_value;
        }
    }
}

void Renderer::putText_(String &str) {
    for (int i = 0; i < str.length; ++i) {
        ssfn_putc(str[i]);
    }
}


