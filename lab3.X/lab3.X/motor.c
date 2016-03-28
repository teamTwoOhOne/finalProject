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
    //ODCDbits.ODCD6 = 1;
    //ODCCbits.ODCC13 = 1;
    LATFbits.LATF1 = 1;
    LATDbits.LATD12 = 1;
}