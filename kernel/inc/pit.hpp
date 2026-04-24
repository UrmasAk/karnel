#pragma once

#include "io.hpp"

#define PIT_CONTROL_PORT  0x43
#define PIT_CHANNEL0_PORT 0x40
#define PIT_FREQUENCY     1193182


extern "C" {

static inline void io_wait() {
    outb(0x80, 0);
}

void pit_sleep_ms(unsigned int ms);

void setup_pit(unsigned int frequency);

}

