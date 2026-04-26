#include "../inc/pit.hpp"

#include "../inc/idt.hpp"
#include "../inc/pic.hpp"

int pitFrequency;

void pit_sleep_ms(unsigned int ms) {
    int startTicks = pitInterruptsTriggered;
    int targetTicks = startTicks + (ms * pitFrequency) / 1000;

    while (pitInterruptsTriggered < targetTicks) {
        int currentTicks = pitInterruptsTriggered;

        if (currentTicks < startTicks) {
            startTicks = currentTicks;
            targetTicks = startTicks + (ms * pitFrequency) / 1000;
        }

        asm volatile("hlt");
    }
}


void setup_pit(unsigned int frequency) {
    __asm__ __volatile__("cli");
    pitFrequency = frequency;
    unsigned int divisor = PIT_FREQUENCY / frequency;
    outb(PIT_CONTROL_PORT, 0x36 | 0x02);
    io_wait();
    outb(PIT_CHANNEL0_PORT, divisor & 0xFF);
    io_wait();
    outb(PIT_CHANNEL0_PORT, divisor >> 8);
    io_wait();

    unmask_irq(0);
    __asm__ __volatile__("sti");
}
