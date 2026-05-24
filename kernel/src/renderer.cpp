//
// Created by jesper on 22.05.26.
//
#define SSFN_CONSOLEBITMAP_TRUECOLOR    /* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_NO_CPP_STD_STRING
#include "../inc/ssfn.hpp"
#include "../inc/renderer.hpp"




Renderer::Renderer(limine_framebuffer *framebuffer_ptr, const uint8_t *font_ptr) {
    ssfn_src = reinterpret_cast<ssfn_font_t*>(const_cast<uint8_t*>(font_ptr));      /* the bitmap font to use */

    ssfn_dst.ptr = ptr_ = static_cast<uint8_t*>(framebuffer_ptr->address);                  /* address of the linear frame buffer */
    ssfn_dst.w = w_ = framebuffer_ptr->width;                          /* width */
    ssfn_dst.h = h_ = framebuffer_ptr->height;                           /* height */
    ssfn_dst.p = p_ = framebuffer_ptr->pitch;                          /* bytes per line */
    ssfn_dst.x = x_ = y_ = ssfn_dst.y = 0;                /* pen position */
    ssfn_dst.fg = fg_ = 0xFFFFFF;                     /* foreground color */
}

void Renderer::setString(const char string[16]) {
    *string_ = *string;

}

void Renderer::render() {
    clearScreen_();
    x_ = h_/2;
    y_ = w_/2;

}

void Renderer::clearScreen_() {
    for (int y = 0; y < h_; y++) {
        for (int x = 0; x < w_; x++) {
            ptr_[y * (p_ / 4) + x] = 0;
        }
    }
}

void Renderer::putText_() {
    for (char character: string_) {
        ssfn_putc(character);
    }
}
