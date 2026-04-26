#include "../inc/gdt.hpp"

extern "C" {

static GDT::Entry gdt[7];
static GDT::Register gdtr;
static GDT::TSS my_tss;

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

    gdt_install_tss(3, (uint64_t)&my_tss);

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

    asm volatile ("ltr %%ax" ::"a"(0x18));
}

void gdt_install_tss(int num, uint64_t base) {
    uint32_t limit = sizeof(GDT::TSS) - 1;

    GDT::TSSDescriptor* desc = (GDT::TSSDescriptor*)&gdt[num];

    desc->limit_low = limit & 0xFFFF;
    desc->base_low = base & 0xFFFF;
    desc->base_mid = (base >> 16) & 0xFF;
    desc->access = 0x89; // Present, Executable, Accessed (TSS type)
    desc->granularity = ((limit >> 16) & 0x0F);
    desc->base_high = (base >> 24) & 0xFF;
    desc->base_upper32 = (base >> 32) & 0xFFFFFFFF;
    desc->reserved = 0;
}

}
