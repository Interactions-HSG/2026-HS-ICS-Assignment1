/*
    led_blink.c -- Task 4: blink an EXTERNAL LED wired to GPIO 16 (physical pin 36)

    This file starts out as an exact copy of power_blink.c, which blinks the Raspberry Pi's
    *power* LED on GPIO 42. Your job is to change it so it blinks the LED you wired up on
    GPIO 16 instead.

    Everything you need is in the BCM2711 ARM Peripherals manual (docs/bcm2711-peripherals.pdf),
    pages 65-70. Three things need to change -- look for the TODO markers below.

    Build it with:   make led_blink        (produces kernel7l.img)

    Copyright (c) 2013, Brian Sidebotham
    Original: https://github.com/BrianSidebotham/arm-tutorial-rpi/blob/master/part-1/armc-03/armc-03.c
    Copyright (c) 2022, Iori Mizutani

    This software is licensed under the MIT License.
    Please see the LICENSE file included with this software.
*/

// The base address for the GPIO registers (ARM physical address).
#define GPIO_BASE       0xFE200000UL

// The GPIO register address offsets, as 32-bit word indices.
// See p.66 of the manual: https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf
#define GPFSEL0    0
#define GPFSEL1    1
#define GPFSEL2    2
#define GPFSEL3    3
#define GPFSEL4    4
#define GPFSEL5    5
#define GPSET0     7
#define GPSET1     8
#define GPCLR0     10
#define GPCLR1     11

// Pointer that will point at the start of the GPIO Register set in memory
volatile unsigned int* gpio;

// Simple loop variable
volatile unsigned int timer;

int main(void) __attribute__((naked));
int main(void) {
    gpio = (unsigned int*) GPIO_BASE;

    // TODO (1/3): Select the right Function Select register and the right bit offset
    //             inside it, so that GPIO 16 becomes an OUTPUT.
    //             Which GPFSELn covers GPIO 16? Which three bits inside it belong to
    //             GPIO 16? What value do those three bits need?
    //             (Unlike GPIO 42, GPIO 16 is NOT an output by default.)
    gpio[GPFSEL4] |= (1 << 6);

    while(1) {
        // Busy-wait to emulate a timer
        for(timer = 0; timer < 500000; timer++) {
            ;
        }

        // TODO (2/3): Drive GPIO 16 LOW. Which GPCLRn register holds GPIO 16,
        //             and which bit inside it?
        gpio[GPCLR1] = (1 << 10);

        // Busy-wait to emulate a timer
        for(timer = 0; timer < 500000; timer++) {
            ;
        }

        // TODO (3/3): Drive GPIO 16 HIGH. Which GPSETn register holds GPIO 16,
        //             and which bit inside it?
        gpio[GPSET1] = (1 << 10);
    }
}
