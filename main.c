#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>

void GPIO_PORTF_INIT(void)
{
    SYSCTL_RCGCGPIO_R |= 0x20;  // enable clock to port F
    GPIO_PORTF_DIR_R = 0x0E;    // set PF1, PF2, PF3 as output (LEDs)
    GPIO_PORTF_DEN_R = 0x1F;    // enable digital on PF0, PF1, PF2, PF3, PF4
    GPIO_PORTF_PUR_R = 0x11;    // enable pull-ups on PF0, PF4 (SW1, SW2)

    GPIO_PORTF_IM_R  = 0x00;    // disable interrupts during setup
    GPIO_PORTF_IS_R  |= (1 << 4);  // level-sensitive interrupt on PF4 (SW2)
    GPIO_PORTF_IBE_R &= ~(1 << 4); // controlled by IEV
    GPIO_PORTF_IEV_R &= ~(1 << 4); // interrupt on low level
    GPIO_PORTF_IM_R  |= (1 << 4);  // enable interrupt on PF4
}

void delay(float time, int clock)
{
    float value;
    value = (time * clock) / 1000;
    NVIC_ST_RELOAD_R = value;      // set reload value
    NVIC_ST_CURRENT_R = 0x0;     // reset current value
    NVIC_ST_CTRL_R = 0x05;       // enable timer, use system clock
    while ((NVIC_ST_CTRL_R & (1 << 16)) == 0)
    {
        // wait for the count flag
    }
    NVIC_ST_CTRL_R = 0x0;        // disable timer
}

int clock = 16000000;  // system clock frequency

void GPIO_PORTF_Handler(void)
{
    int state0 = GPIO_PORTF_DATA_R;
    delay(25, clock);              // debounce delay
    int state1 = GPIO_PORTF_DATA_R;
    if (state0 != state1)
    {
        GPIO_PORTF_DATA_R ^= 0x02; // toggle red LED (PF1)
    }

    GPIO_PORTF_ICR_R |= (1 << 4);  // clear interrupt flag for PF4
}


int main(void)
{
    NVIC_EN0_R = (1 << 30);        // enable interrupt in NVIC for Port F
    GPIO_PORTF_INIT();             // initialize GPIO Port F


    while (1)
    {
        // main loop
    }
}
