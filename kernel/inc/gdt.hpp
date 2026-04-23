#pragma once
#include "cstdint"

namespace GDT {
    struct [[gnu::packed]] Entry {
        std::uint16_t limit_low;
        std::uint16_t base_low;
        std::uint8_t  base_mid;
        std::uint8_t  access;
        std::uint8_t  granularity;
        std::uint8_t  base_high;
    };

    struct [[gnu::packed]] Register {
        std::uint16_t limit;
        std::uint64_t base;
    };
}

extern GDT::Entry gdt[7];
extern GDT::Register gdtr;

void gdt_fill_entry (int num, std::uint8_t access, std::uint8_t granularity, std::uint32_t base, std::uint32_t limit);

void setup_gdt();
