//
// Created by jesper on 22.05.26.
//
#define SSFN_IMPLEMENTATION    /* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_NO_CPP_STD_STRING
#include "../inc/ssfn.hpp"
#include "../inc/renderer.hpp"
#include "../inc/string.hpp"




Renderer::Renderer(limine_framebuffer *framebuffer_ptr, const uint8_t *font_ptr) {
    auto *font = new SSFN::Font;
    font->Load(font_ptr);

    ssfn_src = reinterpret_cast<ssfn_font_t*>(const_cast<uint8_t*>(font_ptr));      /* the bitmap font to use */

    ptr_ = framebuffer_ptr;

    auto *buf_ = new ssfn_buf_t;
    *buf_ = {
        .ptr = static_cast<uint8_t*>(framebuffer_ptr->address),                  /* address of the linear frame buffer */
        .w = static_cast<int>(framebuffer_ptr->width),                          /* width */
        .h = static_cast<int>(framebuffer_ptr->height),                           /* height */
        .p = static_cast<uint16_t>(framebuffer_ptr->pitch),                          /* bytes per line */
        .x = 0,
        .y = 0,                /* pen position */
        .fg = 0xFFFFFF,                     /* foreground color */
        .bg = 0x000000
    };
}

#ifdef RENDER_NO_VARIABLE
void Renderer::render() {
    clearScreen_();
    ssfn_dst.x = ssfn_dst.w/2;
    ssfn_dst.y = ssfn_dst.h/2;
    putText_("tere");
}
#endif

void Renderer::render(Pomodoro::State state, int remaining_time_sec) {
    String time = calculate_remaining_time_(remaining_time_sec);
    RGB pixel = [state]() -> RGB {
        switch (state) {
            case Pomodoro::State::W_RUN: return RGB{0, 0, 255, 255};
            case Pomodoro::State::W_END: return RGB{0, 0, 64, 255};
            case Pomodoro::State::W_PAUSE: return RGB{0, 0, 128, 255};
            case Pomodoro::State::B_RUN: return RGB{0, 255, 0, 255};
            case Pomodoro::State::B_END: return RGB{0, 64, 0, 255};
            case Pomodoro::State::B_PAUSE: return RGB{0, 128, 0, 255};
        }
    }();

    uint32_t pixel_value =
        (static_cast<uint32_t>(pixel.alpha) << 24) |
        (static_cast<uint32_t>(pixel.red) << 16) |
        (static_cast<uint32_t>(pixel.green) << 8) |
        (static_cast<uint32_t>(pixel.blue));

    fill_screen(pixel_value);

    ssfn_dst.x = ssfn_dst.w/2;
    ssfn_dst.y = ssfn_dst.h/2;
    putText_(time);
}

String Renderer::calculate_remaining_time_(int remaining_time_sec) {
    int remaining_minutes = remaining_time_sec / 60;
    int volatile remaining_seconds = remaining_time_sec - (remaining_minutes * 60);

    String str;
    if (remaining_seconds < 10) {
        str = String::toString(remaining_minutes) + String(":0") + String::toString(remaining_seconds);
    } else {
        str = String::toString(remaining_minutes) + String(":") + String::toString(remaining_seconds);
    }

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

void Renderer::putText_(char *string) {
    for (int i = 0; i < 16; i++) {
        // ssfn_putc(static_cast<uint32_t>(character));
        // uint32_t test = static_cast<uint32_t>(character) & 0x000000FF ;
        font->Render(buf_, &string[i]);
    }
}

void Renderer::putText_(String &str) {
    for (int i = 0; i < str.length; ++i) {
        font->Render(buf_, &str.begin()[i]);
    }
}


