
#include <cstdint>
#include <cstddef>
#include <limine.h>

#include "../inc/gdt.hpp"
#include "../inc/idt.hpp"
#include "../inc/pic.hpp"
#include "../inc/pit.hpp"
#include "../inc/renderer.hpp"

// Set the base revision to 6, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

namespace {

__attribute__((used, section(".limine_requests")))
volatile std::uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(6);

}

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

namespace {

__attribute__((used, section(".limine_requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0,
    .response = nullptr
};

}

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .cpp file, as seen fit.

namespace {

__attribute__((used, section(".limine_requests_start")))
volatile std::uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
volatile std::uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

}

// Halt and catch fire function.
namespace {

void hcf() {
    for (;;) {
#if defined (__x86_64__)
        asm ("hlt");
#elif defined (__aarch64__) || defined (__riscv)
        asm ("wfi");
#elif defined (__loongarch64)
        asm ("idle 0");
#endif
    }
}

}

// The following stubs are required by the Itanium C++ ABI (the one we use,
// regardless of the "Itanium" nomenclature).
// Like the memory functions above, these stubs can be moved to a different .cpp file,
// but should not be removed, unless you know what you are doing.
extern "C" {
    int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
    void __cxa_pure_virtual() { hcf(); }
    void *__dso_handle;
}

// Fondi kättesaamiseks mälust
extern "C" {
    extern const std::uint8_t font_u_vga16_start[];
    extern const std::uint8_t font_u_vga16_end[];
}

// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
extern "C" void kmain() {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false) {
        hcf();
    }

    // Call global constructors.
    for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    // Interruptide jaoks
    setup_gdt();
    setup_idt();
    // APIC on eelistatud 64bit systeemides, seega peame seda valja lylitama, muidu PIC ei tegele katkestustega tegelt
    // voib ka nii teha, et ta delegeeriks meie PIC-ile moned katkestused, aga noh hetkel lihtsam seda lihtsalt valja
    // lylitada, kuigi spurious interrupts on probleemiks sel juhul
    disable_apic();
    setup_pic(0x20, 0x28);
    setup_pit(1000);



    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }

    // Fetch the first framebuffer.
    limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    struct RGB
    {
        std::uint8_t blue;
        std::uint8_t green;
        std::uint8_t red;
        std::uint8_t alpha;
    };

    RGB* fb_ptr = static_cast<RGB*>(framebuffer->address);

    // Text setup

    /* set up context by global variables */
    ssfn_src = reinterpret_cast<ssfn_font_t*>(const_cast<std::uint8_t*>(font_u_vga16_start));      /* the bitmap font to use */

    ssfn_dst.ptr = static_cast<std::uint8_t*>(framebuffer->address);                  /* address of the linear frame buffer */
    ssfn_dst.w = framebuffer->width;                          /* width */
    ssfn_dst.h = framebuffer->height;                           /* height */
    ssfn_dst.p = framebuffer->pitch;                          /* bytes per line */
    ssfn_dst.x = ssfn_dst.y = 0;                /* pen position */
    ssfn_dst.fg = 0xFFFFFF;                     /* foreground color */




    while (true) {

        for (std::size_t y = 0; y < framebuffer->height; y++) {
            for (std::size_t x = 0; x < framebuffer->width; x++) {
                // std::uint32_t nX = x * 255 / framebuffer->width;
                std::uint32_t nY = y * 255 / framebuffer->height;
                fb_ptr[y * (framebuffer->pitch / 4) + x].red = nY;
            }
            pit_sleep_ms(1);
        }

        /* render UNICODE codepoints directly to the screen and then adjust pen position */
        ssfn_putc('H');
        ssfn_putc('e');
        ssfn_putc('l');
        ssfn_putc('l');
        ssfn_putc('o');


        // int lock_in_time_sec = 20 * 60;
        int lock_in_time_sec = 10;

        for (int i = 0; i < lock_in_time_sec; ++i) {
            pit_sleep_ms(1000);
        }

        for (std::size_t y = 0; y < framebuffer->height; y++) {
            for (std::size_t x = 0; x < framebuffer->width; x++) {
                fb_ptr[y * (framebuffer->pitch / 4) + x].blue = 100;
            }
            pit_sleep_ms(1);
        }

        // int lock_out_time_sec = 5 * 60;
        int lock_out_time_sec = 2;
        for (int i = 0; i < lock_out_time_sec; ++i) {
            pit_sleep_ms(1000);
        }

        for (std::size_t y = 0; y < framebuffer->height; y++) {
            for (std::size_t x = 0; x < framebuffer->width; x++) {
                fb_ptr[y * (framebuffer->pitch / 4) + x].blue = 0;
                fb_ptr[y * (framebuffer->pitch / 4) + x].green = 0;
                fb_ptr[y * (framebuffer->pitch / 4) + x].red = 0;
                fb_ptr[y * (framebuffer->pitch / 4) + x].alpha = 0;
            }
            pit_sleep_ms(1);
        }
    }

    // We're done, just hang...
    hcf();
}
