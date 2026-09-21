/*
    led_fade.c -- BONUS task: fade the external LED on GPIO 16 in and out

    The LED can only be driven fully on or fully off -- there is no analog output on these
    pins. So "fading" has to be produced some other way. One well-known option is software
    PWM (pulse-width modulation): switch the LED on and off far faster than the eye can
    follow, and slowly vary the fraction of time it spends on (the "duty cycle").
    The BCM2711 also has a hardware PWM peripheral (manual, chapter 8) if you prefer.

    Any functionally correct approach is accepted.

    Start from your working led_blink.c. Build it with:   make led_fade

    This software is licensed under the MIT License.
    Please see the LICENSE file included with this software.
*/

#define GPIO_BASE       0xFE200000UL

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

volatile unsigned int* gpio;
volatile unsigned int timer;

int main(void) __attribute__((naked));
int main(void) {
    gpio = (unsigned int*) GPIO_BASE;

    // TODO (bonus): configure GPIO 16 as an output, then fade it in and out.
    //
    // A sketch of the software-PWM idea, if you want it:
    //   - pick a short PERIOD, e.g. 1000 "ticks"
    //   - keep a duty value that walks from 0 up to PERIOD and back down again
    //   - in each period: hold the LED on for `duty` ticks, off for `PERIOD - duty` ticks
    //   - repeat each duty level a few hundred times so the fade is slow enough to see

    while(1) {
        ;
    }
}
