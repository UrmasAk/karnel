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

    struct [[gnu::packed]] TSSDescriptor {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t  base_mid;
        uint8_t  access;
        uint8_t  granularity;
        uint8_t  base_high;
        uint32_t base_upper32;
        uint32_t reserved;
    };

    struct [[gnu::packed]] TSS {
        uint32_t reserved0;
        uint64_t rsp0;      // Stack pointer for Ring 0
        uint64_t rsp1;
        uint64_t rsp2;
        uint64_t reserved1;
        uint64_t ist1;      // Interrupt Stack Table entries
        uint64_t ist2;
        uint64_t ist3;
        uint64_t ist4;
        uint64_t ist5;
        uint64_t ist6;
        uint64_t ist7;
        uint64_t reserved2;
        uint16_t reserved3;
        uint16_t iopb_offset;
    };
}

extern "C" {

void gdt_install_tss(int num, uint64_t base);

void gdt_fill_entry (int num, std::uint8_t access, std::uint8_t granularity, std::uint32_t base, std::uint32_t limit);

void setup_gdt();

}
