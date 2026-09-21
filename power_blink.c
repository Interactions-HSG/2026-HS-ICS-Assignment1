/*
    power_blink.c -- blink the power LED

    Copyright (c) 2013, Brian Sidebotham
    Original: https://github.com/BrianSidebotham/arm-tutorial-rpi/blob/master/part-1/armc-03/armc-03.c

    Copyright (c) 2022, Iori Mizutani
    This code is meant to run on Raspberry Pi 400 Rev 1.1 (BCM2711) and as a 32-bit kernel (kernel7l.img).

    This software is licensed under the MIT License.
    Please see the LICENSE file included with this software.
*/

// The base address for GPIO register comes from the linux source code (virtual address 0x7E20 0000 -> physical address 0xFE20 0000)
// https://github.com/raspberrypi/linux/blob/rpi-5.15.y/arch/arm/boot/dts/bcm2711.dtsi#L41
#define GPIO_BASE       0xFE200000UL

// Note for below: The RPi 400 model has the power LED attached to GPIO Pin 42
// cf. https://github.com/raspberrypi/linux/blob/rpi-5.15.y/arch/arm/boot/dts/bcm2711-rpi-400.dts#L18

// The GPIO register address offsets
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

/** Main function declared as "naked": when compiled, this function will not include any prologue (e.g., for managing parameters)
	or epilogue (e.g., for managing return values). This is fine because the function is called directly and will never return. */
int main(void) __attribute__((naked));
int main(void) {
    // Assign the address of the GPIO peripheral (Using ARM Physical Address)
    gpio = (unsigned int*) GPIO_BASE;

    // Write 1 to the GPIO init nibble in the Function Select GPIO peripheral register
    // A bitwise OR assignment (|=) is used to accomplish this
    // FSEL42 starts at the 6th bit (001 for FSEL42)
    // However, FSEL42 is "output" by default
    gpio[GPFSEL4] |= (1 << 6);

    // Loop forever ("while(true)"): Never exit as there is no operating system to exit to!
    while(1) {
        // To emulate a "timer", we keep the processor busy counting to 500000
        for(timer = 0; timer < 500000; timer++) {
            ;
        }

        // Set the LED GPIO pin low: Clear Register for GPIO Pin 42 is in GPCLR1
        gpio[GPCLR1] = (1 << 10); // n=42 for GPCLR1 is at the 10th bit

        // To emulate a "timer", we keep the processor busy counting to 500000
        for(timer = 0; timer < 500000; timer++) {
            ;
        }

        // Set the LED GPIO pin high: Set Register for GPIO Pin 42 is in GPSET1
        gpio[GPSET1] = (1 << 10); // n=42 for GPSET1 is at the 10th bit
    }
}
