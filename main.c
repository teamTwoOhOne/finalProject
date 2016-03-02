// ******************************************************************************************* //
//
// File:         lab1p1.c
// Date:
// Authors:
//
// Description: Part 1 for lab 1
// ******************************************************************************************* //


#include <xc.h>
#include <sys/attribs.h>
#include "leds.h"
#include "interrupt.h"
#include "keypad.h"
#include "timer.h"
#include "config.h"


/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */
#define press 0
#define release 1

typedef int bool;
#define true    1
#define false   0


typedef enum stateTypeEnum {
     initial, stopped, running, deBounce1, deBounce2, increment
} stateType;

volatile stateType state, stateNext;
//volatile bool dbCount = false;

int min;
int sec;

int main(void)
{
    SYSTEMConfigPerformance(10000000);
    //initTimer1();
    initSW1();
    initSW2();
    initLEDs();
    initLCD();
    enableInterrupts();
    state = running;

    clearLCD();
    
    while(1)
    {
        //TODO: Using a finite-state machine, define the behavior of the LEDs
        //Debounce the switch
        switch(state){
            case stopped:
                turnOnLED(2);
                clearLCD();
                moveCursorLCD(1,1);
                printStringLCD("Stopped:")
                moveCursorLCD(1,2);
                printStringLCD(getTimeString(min,sec)); 
				stateNext = running;
                break;
            case running:
                initTimer1();
                turnOnLED(1);
                clearLCD();
                moveCursorLCD(1,1);
                printStringLCD("Running:")
                moveCursorLCD(1,2);
                printStringLCD(getTimeString(min,sec));
				stateNext = stopped;
                break;

            case deBounce1:
                delayUs(10000);
				break;
            case deBounce2:
                delayUs(10000);
                break;
            case increment:
                sec += 1;
                if (sec > 5999) {
                    min += 1;
                    sec = 0;
                }
                moveCursorLCD(1,1);
                printStringLCD("Running:")
                moveCursorLCD(1,2);
                printStringLCD(getTimeString(min,sec));  
            break;
        }
    }

    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){
    //TODO: Implement the interrupt to capture the press of the button
    IFS1bits.CNGIF = 0;
    PORTG;
    
    if(state == running && PORTGbits.RG13 == press) {
        T1CONbits.ON = 0;      //disable timer when not in 'running' state
        state = deBounce1;
    }
    else if (state == stopped && PORTGbits.RG13 == press) {
        state = deBounce1;
    }
    else if(state == deBounce1 && PORTGbits.RG13 == release) {
        state = stateNext;
    }
    else if (state == stopped && PORTDbits.RD6 == press) {
        state = deBounce2;
    }
    else if (state == deBounce2 && PORTDbits.RD6 == release) {
        sec = 0;
        min = 0;
        state = stopped;
    }  //PORTDbits.RD6
}
void __ISR(_TIMER_1_VECTOR, IPL3SRS) _T1Interrupt(){
    IFS0bits.T1IF = 0;
    if (state == running) {
        state = increment;

        

    }
}