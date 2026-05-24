//
// Created by jesper on 22.05.26.
//
#define SSFN_CONSOLEBITMAP_TRUECOLOR    /* use the special renderer for 32 bit truecolor packed pixels */
#define SSFN_NO_CPP_STD_STRING
#include "../inc/ssfn.hpp"
#include "../inc/renderer.hpp"




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

void Renderer::setString(char *string) {
    string_ = string;

}

void Renderer::render() {
    clearScreen_();
    ssfn_dst.x = ssfn_dst.w/2;
    ssfn_dst.y = ssfn_dst.h/2;
    putText_();
}

void Renderer::clearScreen_() {
    volatile auto *fb_ptr = static_cast<volatile uint32_t *>(ptr_->address);
    for (uint64_t y = 0; y < ptr_->height; y++) {
        for (uint64_t x = 0; x < ptr_->width; x++) {
            fb_ptr[y * (ptr_->pitch / 4) + x] = 23556;
        }
    }
}

void Renderer::putText_() {
    for (int i = 0; i < 16; i++) {
        // ssfn_putc(static_cast<uint32_t>(character));
        // uint32_t test = static_cast<uint32_t>(character) & 0x000000FF ;
        ssfn_putc(string_[i]);
    }
}
