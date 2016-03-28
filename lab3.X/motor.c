#include <proc/p32mx470f512l.h>

#include "motor.h"
#include "pwm.h"
//
//#define Input1 0
//#define Input2 0
//#define Input3 0
//#define Input4 1


//RF1 ENABLE 1
//RD12 ENABLE 2
//RD6 ODC1
//RC13 ODC2
void initMotor() {
    TRISFbits.TRISF1 = 0;
    TRISDbits.TRISD12 = 0;
    TRISDbits.TRISD6 = 1;

    LATFbits.LATF1 = 1;
    LATDbits.LATD12 = 1;
}

void initSW1(){
    TRISDbits.TRISD6 = 1;           // Configure switch as input
    CNCONDbits.ON = 1;                  // Enable overall interrupt
    CNENDbits.CNIED6 = 1;         // Enable pin CN
    CNPUDbits.CNPUD6 = 1;         // Enable pull-up resistor
    IFS1bits.CNDIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 2;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNDIE = 1;           // Enable interrupt for D pins
}