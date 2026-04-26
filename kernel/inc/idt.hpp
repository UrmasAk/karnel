#pragma once
#include "cstdint"

namespace IDT {
    struct [[gnu::packed]] Entry {
        uint16_t offset_low;
        uint16_t selector;
        uint8_t  ist;
        uint8_t  type_attr;
        uint16_t offset_mid;
        uint32_t offset_high;
        uint32_t zero;
    };

    struct [[gnu::packed]] Register {
        uint16_t limit;
        uint64_t base;
    };

    struct InterruptFrame {
        uintptr_t ip;
        uintptr_t cs;
        uintptr_t flags;
        uintptr_t sp;
        uintptr_t ss;
    };

}


extern "C" {

void set_idt_entry(int index, int ist, int attr, void (*handler)());

extern volatile int pitInterruptsTriggered;

[[gnu::interrupt]]
void pit_isr([[maybe_unused]] void* frame);

void setup_idt();

}
