//Lab0a for Embedded Systems
//Author: Alex Fay, Mark McAfoose
//id# 801336907
//date: 02/05/2023

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
    while(1);
}
#endif

#define CLOCK_PERIOD 0.0000000625 //clock period in microseconds, from datasheet
#define DELAY_LOOP_PERIOD 3 //num cycles in SysCtlDelay() loop

void delay(double time);
void blink(int color, int rate, int cyc);

//*****************************************************************************
//
// Blink the on-board LED according to the specification in the lab
//
//*****************************************************************************
int main(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    /*LED driven by GPIO F peripheral,
    have to enable this first to avoid hard fault*/

    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){
        ;
    }

    //LED pins are 1(red), 2(blue), and 3(green) on Port F
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

    while(1)
    {
        blink(0, 2, 7);//blink color red, .25 second on, .75 second off, 7 blinks
        delay(3);
    }
}

void delay(double time){
    //Multiplication of period of clock cycle and SysCtlDelay cycle time
    double per = CLOCK_PERIOD * DELAY_LOOP_PERIOD;
    long cyc = (long)(time / per);
    SysCtlDelay((uint32_t)cyc);
    return;
}

void blink(int color, int rate, int cyc){
    float ton = 0; //LED on time
    float toff = 0; //LED off time
    uint32_t mix = 0; //for call to PinWrite, sets the pin(s) to write

    switch(rate){
        //Setting up duty cycle
        case 0: ton = 1; toff = 1; break;
        case 1: ton = .5; toff = .5; break;
        case 2: ton = .25; toff = .75; break;
        default: return;
    }

    switch(color){
        case 0: mix = GPIO_PIN_1; break;//red
        case 1: mix = GPIO_PIN_2; break;//blue
        case 2: mix = GPIO_PIN_3; break;//green
        default: return;
    }

    int i;
    for(i = 0; i < cyc; i++){
        GPIOPinWrite(GPIO_PORTF_BASE, mix, mix);
        delay(ton);
        GPIOPinWrite(GPIO_PORTF_BASE, mix, 0x0);
        delay(toff);
    }
    return;
}
