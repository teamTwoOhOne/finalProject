                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  #include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "motor.h"
#include "lcd.h"
#include "pwm.h"
#include "timer.h"
#include "adc.h"
#include "string.h"
#define mThresh 65
#define rThresh 50
#define lThresh 40
#define Thresh 30
#define printInterval 500 //Number of iterations of PrintValue() that execute before LCD displays values
#define fSpeed 300        //Forward Speed
#define tSpeed 250        //Turning Speed
volatile unsigned int print = printInterval;
volatile unsigned int counter = 0;
char printVal[5];
char stateDisp[9];
volatile unsigned int valR = 0;
volatile unsigned int valM = 0;
volatile unsigned int valL = 0;
volatile int idle1;
volatile int lineL = 840;
volatile int lineM = 800;
volatile int lineR = 865;
volatile int Full = 0;
volatile int i = 0;
char mVal[5] = "1024";
char rVal[5] = "1024";
char lVal[5] = "1024";

    typedef enum stateTypeEnum {
    Delay, Idle, Forward, TurnLeft, TurnRight, Triple, TurnAround, SharpLeft, SharpRight
} stateType;
    
    volatile stateType state = Forward;
    volatile stateType stateNext = Forward;

int main(void){
    int j = 0;
    int onLine = 0;
    init_timer_1();
    enableInterrupts();
    init_lcd();
    clear_lcd();
    initPWM();
    initADC();
    initMotor();
    move_cursor_lcd(0,1);
    print_string_lcd("XXXX"); //Though the reason is unclear, the display does not function properly without some initial string being printed on it
    PrintValue();  
    lineL = valL + lThresh;
    lineR = valR + rThresh;
    lineM = valM - mThresh;
    while(1){
        switch(state) {
            //Default state. Proceed forward, while ready to change into any other state
            case Forward:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = fSpeed;
                OC4RS = fSpeed;
                onLine = 0;
                PrintValue();
                if(valR > lineR + Thresh && valL > lineL + Thresh && valM > lineM + Thresh) { 
                    stateNext = Triple;
                    state = Delay;
                }
               
                else {                   
                    if (valL > lineL + Thresh && valM > lineM + Thresh) {
                        Full = 1;
                        i = 0;
                        stateNext = SharpLeft;
                        state = Delay;
                    }
                    else if (valR > lineR + Thresh && valM > lineM + Thresh) {
                        Full = 1;
                        i = 0;
                        stateNext = SharpRight;
                        state = Delay;
                    }

                    else if (valR > lineR) {
                        state = TurnRight;
                    }
                    else if (valL > lineL) {
                        state = TurnLeft;
                    }
                }
                sprintf(stateDisp, "For");
                break;
                
            //All sensors are over a line    
            case Triple:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = fSpeed;
                OC4RS = fSpeed;
                if (onLine == 0) {
                        counter += 1;
                        onLine = 1;
                }
                sprintf(stateDisp,"Trip");
                PrintValue();
                if (!(valR > lineR + Thresh && valL > lineL + Thresh && valM > lineM + Thresh)) {
                    if (counter < 3 && counter > 1) {
                        i = 0;
                        Full = 1;
                        stateNext = SharpRight; 
                        state = Delay;
                    }
                    else if (counter == 3) state = TurnAround;
                    else if (counter > 3) state = Idle;                    
                }
                break;
            //Make 180 degree turn
            case TurnAround: 
                sprintf(stateDisp,"Tur");
                OC1RS = tSpeed;
                OC2RS = 0;
                OC3RS = tSpeed;
                OC4RS = 0;   
                onLine = 0;
                PrintValue();   
                delay_ms(1);
                if (valR > lineR /*&& valR < lineR*/ && i == 1) {
                    j = 1;                    
                }   
                if(valR < lineR) i = 1;
                if (j == 1 && valM > lineM + Thresh) {
                    i = 0;
                    j = 0;
                    state = Forward;
                }
                break;
            //Turn left until the left sensor no longer detects the line  
            case TurnLeft:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = 0;
                OC4RS = tSpeed;
                onLine = 0;
                sprintf(stateDisp, "Lef");
                PrintValue();
                if(valL < lineL) state = Forward;
                else if (valR > (lineR + Thresh) && valL > (lineL + Thresh) && valM > (lineM + Thresh)) {
                    state = Triple;
                }
                else if (valL > lineL + Thresh && valM > lineM + Thresh) {
                    i = 0;
                    Full = 1;
                    stateNext = SharpLeft;
                    state = Delay;
                    
                }
                break;
            //Turn right until the right sensor no longer detects the line
            case TurnRight:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = tSpeed;
                OC4RS = 0;
                onLine = 0;
                sprintf(stateDisp, "Rig");
                PrintValue();
                if(valR < lineR) state = Forward;
                else if (valR > (lineR + Thresh) && valL > (lineL + Thresh) && valM > (lineM + Thresh)) {
                    state = Triple;
                }
                else if (valR > lineR + Thresh && valM > lineM + Thresh) {
                    i = 0;
                    Full = 1;
                    stateNext = SharpRight;
                    state = Delay;
                    
                }
                break;
            //Turn right until both the right and middle sensors have passed over a new line    
            case SharpRight:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = tSpeed / 2;
                OC4RS = 0;   
                onLine = 0;
                sprintf(stateDisp, "sRi");
                PrintValue();   
                if (valM > lineM) state = Forward;
                if (valR > lineR && i == 1) {
                    j = 1;                    
                }   
                if(valR < lineR) i = 1;
                if (j == 1 && valM > lineM + Thresh) {
                    i = 0;
                    j = 0;
                    state = Forward;
                }
                break;
            //Turn left until both the left and middle sensors have passed over a new line
            case SharpLeft:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = 0;
                OC4RS = tSpeed;   
                onLine = 0;
                sprintf(stateDisp, "sLe");
                PrintValue();   
                if (valM > lineM) state = Forward;
                if (valL > lineL && i == 1) {
                    j = 1;                    
                }   
                if(valR < lineR) i = 1;
                if (j == 1 && valM > lineM + Thresh) { 
                    i = 0;
                    j = 0;
                    state = Forward;                
                }
                break;
            //Halt robot. Primarily for debugging
            case Idle:
                idle1 = 1;
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = fSpeed;
                OC4RS = fSpeed;
                sprintf(stateDisp, "Idl");
                PrintValue();
                break;
            //Continue forward until the robot has passed a 90 degree turn or perpendicular line, then take the appropriate action    
            case Delay:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = fSpeed;
                OC4RS = fSpeed;
                PrintValue();
                sprintf(stateDisp, "Del");
                if (!(valR > lineR + Thresh || valL > lineL + Thresh)) state = stateNext;
                break;
        }
    }

    return 0;
}

//Gather and display data from sensors
void PrintValue(){
        AD1CON1bits.ADON = 0;
        AD1CHSbits.CH0SA = 4; //pin 34
        AD1CON1bits.ADON = 1;
        while(IFS0bits.AD1IF == 0) {
        }
        valL = ADC1BUF0;
        IFS0bits.AD1IF = 0;  // Left sensor to valL

        sprintf(printVal,"%d",valL);
        if (print >= printInterval) {       
            
            i = 0;
            if (valL > lineL) {
                i = 1;
                if (valL > lineL + Thresh) i = 2;
            }
            move_cursor_lcd(0,1);
            sprintf(printVal,"%d", i);
            print_string_lcd(printVal);  
        }

        AD1CON1bits.ADON = 0;  
        AD1CHSbits.CH0SA = 2; //pin 32

        //Right sensor
        AD1CON1bits.ADON = 1;
        while(IFS0bits.AD1IF == 0) {
        }
        valR = ADC1BUF0;
        IFS0bits.AD1IF = 0; // Right sensor to valR
        sprintf(printVal,"%d",valR);
        if (print >= printInterval) {        
            move_cursor_lcd(0,2);
            
            i = 0;
            if (valR > lineR) {
                i = 1;
                if (valR > lineR + Thresh) i = 2;
            }
            sprintf(printVal, "%d", i);
            print_string_lcd(printVal);
        }
        //Middle sensor
        AD1CON1bits.ADON = 0;
        AD1CHSbits.CH0SA = 0; //pin 30
        AD1CON1bits.ADON = 1;
        while(IFS0bits.AD1IF == 0) {
        }
        valM = ADC1BUF0;
        IFS0bits.AD1IF = 0;  // Mid sensor to valM
        
        sprintf(printVal,"%d",valM);
        if (print >= printInterval) {            
            move_cursor_lcd(5,2);
            
            i = 0;
            if (valM > lineM) {
                i = 1;
                if (valM > lineM + Thresh) i = 2;
            }
            sprintf(printVal, "%d", i);
            print_string_lcd(printVal);
            print = 0;
        }
   
        else print += 1;
         
       move_cursor_lcd(5,1);        
            sprintf(printVal,"%d",counter);             
            print_string_lcd(printVal);
            i = 0;

}