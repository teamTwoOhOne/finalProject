/*
* File:   leds.c
* Author:
*
* Created on December 27, 2014, 1:31 PM
*/

#include <xc.h>
#include "leds.h"

#define input 1
#define output 0

#define ON 0
#define OFF 1

void initLEDs(){
    //TODO: Initialize the appropriate pins to work with the LEDs
    TRISGbits.TRISG12 = output;//initialize all led output
    TRISGbits.TRISG14 = output;
    ODCGbits.ODCG12 = 1;
    ODCGbits.ODCG14 = 1;
    LATGbits.LATG12 = OFF;//turn off LEDs
    LATGbits.LATG14 = OFF;
}

void turnOnLED(int led){
    if(led==1){
        LATGbits.LATG12 = ON;
        LATGbits.LATG14 = OFF;
    }
    else if(led==2){
        LATGbits.LATG12 = OFF;
        LATGbits.LATG14 = ON;
    }
}
