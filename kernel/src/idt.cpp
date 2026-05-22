#include "../inc/idt.hpp"
#include "../inc/io.hpp"
#include "../inc/ps2.hpp"

static IDT::Entry idt[256];
static IDT::Register idtr;

void set_idt_entry(int index, int ist, int attr, void(* handler)()) {
    auto addr = reinterpret_cast<uintptr_t>(handler);

    // legacy vark teeb 3-ks offsetiks, Instruction Pointer pannakse neist kokku hiljem
    idt[index] = IDT::Entry(
        addr & 0xFFFF,
        0x08,
        ist,
        attr,
        (addr >> 16) & 0xFFFF,
        (addr >> 32),
        0
    );
}

volatile int pitInterruptsTriggered = 0;


[[gnu::interrupt]]
void pit_isr(void*) {
    pitInterruptsTriggered = pitInterruptsTriggered + 1;
    outb(0x20,0x20);
}

void setup_idt() {
    /*
    0x20                 : ISR number (In this case being IRQ0 (the PIT) when the master PIC is remapped to 0x20)
    0                    : The IST (The stack to be switched to based on the TSS, in most cases it should be 0)
    0x8E                 : The attribute value which for kernel only interrupts should be 0x8E
    (void (*)())pit_isr : The C function to be called whe the interrupt is triggered
    */
    set_idt_entry(0x20, 0, 0x8E, reinterpret_cast<void (*)()>(pit_isr)); // IRQ0
    set_idt_entry(0x21, 0, 0x8E, reinterpret_cast<void (*)()>(ps2_isr)); // IRQ1
    idtr.limit = sizeof(idt) - 1;
    idtr.base = reinterpret_cast<uint64_t>(&idt);

    asm volatile ("lidt %0" : : "m"(idtr));
    asm volatile ("sti");
}

volatile int shiftPressed = 0;
volatile uint8_t kbd_buffer_index;
volatile char kbd_buffer[256] = {'\0'};

[[gnu::interrupt]]
void ps2_isr(void*) {
    unsigned char scancode = inb(0x60);

    if (scancode == 0x2A) {
        shiftPressed = 1;
    }
    if (scancode == 0xAA) {
        shiftPressed = 0;
    }

    if (shiftPressed) {
        kbd_buffer[kbd_buffer_index] = asciiShift.data[scancode];
        kbd_buffer_index += 1;
    } else {
        kbd_buffer[kbd_buffer_index] = asciiNoShift.data[scancode];
        kbd_buffer_index += 1;
    }

    print_to_serial();
    outb(0x20,0x20);
}

