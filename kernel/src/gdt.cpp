#include "../inc/gdt.hpp"

GDT::Entry gdt[7];
GDT::Register gdtr;

void gdt_fill_entry(int num, std::uint8_t access, std::uint8_t granularity, std::uint32_t base, std::uint32_t limit) {
    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_mid = (base >> 16) & 0xFF;
    gdt[num].access = access;
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
    gdt[num].base_high = (base >> 24) & 0xFF;
}

void setup_gdt() {
    gdt_fill_entry(0, 0, 0, 0, 0);
    gdt_fill_entry(1, 0x9A, 0x20, 0, 0);
    gdt_fill_entry(2, 0x92, 0x00, 0, 0);

    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = reinterpret_cast<std::uint64_t>(&gdt);

    asm volatile ("lgdt %0" : : "m"(gdtr));
    asm volatile (
        "mov $0x10, %%ax \n"
        "mov %%ax, %%ds \n"
        "mov %%ax, %%ss \n"
        "mov %%ax, %%es \n"
        "mov %%ax, %%fs \n"
        "mov %%ax, %%gs \n"
        "pushq $0x08 \n"
        "lea 1f(%%rip), %%rax \n"
        "pushq %%rax \n"
        "lretq \n"
        "1:\n"
        : : : "rax"
    );

    // ei toota sest pole TSS indeksil 3
    // asm volatile ("ltr %%ax" ::"a"(0x18));
}
