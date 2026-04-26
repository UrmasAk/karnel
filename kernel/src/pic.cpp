#include <cstdint>
#include "../inc/pic.hpp"
#include "../inc/pit.hpp"

void setup_pic(int offset1, int offset2) {
    __asm__ __volatile__("cli");

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, offset1);
    outb(PIC2_DATA, offset2);
    io_wait();

    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);
    io_wait();

    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
    io_wait();

    __asm__ __volatile__("sti");
}

void unmask_irq(unsigned char irq) {
    unsigned short port;
    unsigned char value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }

    value = inb(port);
    value &= ~(1 << irq);
    outb(port, value);
    io_wait();
}

void disable_apic() {
    uint32_t low = 0;
    uint32_t high = 0;
    // IA32_APIC_BASE MSR
    __asm__ volatile("rdmsr" : "=a"(low), "=d"(high) : "c"(0x1B));
    low &= ~(1 << 11); // Bit 11 is the 'Global Enable' bit
    __asm__ volatile("wrmsr" : : "a"(low), "d"(high), "c"(0x1B));
}


