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

#define Thresh 25
#define printInterval 500
volatile unsigned int print = printInterval;
volatile unsigned int counter = 0;
char printVal[5];
char stateDisp[9];
volatile unsigned int valR = 0;
volatile unsigned int valM = 0;
volatile unsigned int valL = 0;
volatile double volts = 0.0;
volatile int idle1;
volatile int lineL = 840;
volatile int lineM = 800;
volatile int lineR = 865;
volatile int lOn = 0;
volatile int mOn = 0;
volatile int rOn = 0;
char mVal[5] = "1024";
char rVal[5] = "1024";
char lVal[5] = "1024";

    typedef enum stateTypeEnum {
    Idle, Forward, TurnLeft, TurnRight, Double, TurnAround
} stateType;
    
    volatile stateType state = Forward;
    volatile stateType stateNext = TurnAround;

int main(void){
    //SYSTEMConfigPerformance(10000000);
    int i = 0;
    int j = 0;
    int onLine = 0;
    float Volts = 0.0;
    init_timer_1();
    enableInterrupts();
    init_lcd();
    clear_lcd();
    initPWM();
    char voltage[8];
    initSW1();
    initADC();
    initMotor();
    move_cursor_lcd(0,1);
    print_string_lcd("lLLLLL");
    PrintValue();  
    lineL = valL + 50;//780;
    lineR = valR + 50;//805;
    lineM = valM - 60;//840;
    while(1){
        switch(state) {

            case Forward:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = 200;
                OC4RS = 200;
                onLine = 0;
                PrintValue();
                if(valR > lineR && valL > lineL && valM > lineM) { 
                    state = Double;
                }
                else if (valL > lineL) {
                    onLine = 0;
                    state = TurnLeft;
                    if (valM > lineM + 50/*&& counter == 0*/) state = Forward;
                }

                else if (valR > lineR) {
                    onLine = 0;
                    state = TurnRight;
                    if (valM > lineM /*&& counter == 0*/) state = Forward;
                    //else state = TurnRight;
                } 
                
                else onLine = 0;
                sprintf(stateDisp, "For");
                break;
            //All sensors are over a line    
            case Double:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = 200;
                OC4RS = 200;
                if (onLine == 0) {
                        counter += 1;
                        onLine = 1;
                }
                sprintf(stateDisp,"Doub");
                PrintValue();
               // delay_ms(100);
                //if(!(valR > (lineR + Thresh) && valL > (lineL + Thresh) && valM > (lineM + Thresh))) state = Forward;
                if (counter >= 1) state = TurnAround;
                //if (counter > 5) state = TurnLeft;
                if (counter == 12) state = Idle;
                break;
            case TurnAround: 
                sprintf(stateDisp,"Tur");
                OC1RS = 100;
                OC2RS = 0;
                OC3RS = 100;
                OC4RS = 0;   
                onLine = 0;
                PrintValue();   
                stateNext = TurnAround; 
                delay_ms(1);
                if (valR > lineR /*&& valR < lineR*/ && i == 1) {
                    j = 1;                    
                }   
                if(valR < lineR) i = 1;
                if (j == 1 && valM > lineM + Thresh) {
                stateNext = Forward;
                    i = 0;
                    j = 0;
                }
                //else if(valR < lineR) state = Forward;
                state = stateNext;
                break;
            case TurnLeft:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = 0;
                OC4RS = 300;
                onLine = 0;
                sprintf(stateDisp, "Lef");
                PrintValue();
                if(valL < lineL) state = Forward;
                //else if (valR > lineR && valM > lineM) state = Double;
                break;
            case TurnRight:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = 300;
                OC4RS = 0;
                onLine = 0;
                sprintf(stateDisp, "Rig");
                PrintValue();
                if(valR < lineR) state = Forward;
                //if(valL > lineL) state = TurnLeft
                if(valL > lineL) state = TurnLeft;
                if (valR > (lineR + Thresh) && valL > (lineL + Thresh) && valM > (lineM + Thresh)) state = Double;
                    //else if(counter < 1) state = Forward;
                    //else state = TurnRight;
 
                break;

            case Idle:
                idle1 = 1;
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = 0;
                OC4RS = 0;
                sprintf(stateDisp, "Idl");
                PrintValue();
                break;
                
        }
    }

    return 0;
}
void PrintValue(){
    //if (print >= printInterval) {
        //Left sensor
        AD1CON1bits.ADON = 0;
        AD1CHSbits.CH0SA = 4; //pin 34
        AD1CON1bits.ADON = 1;
        while(IFS0bits.AD1IF == 0) {
        }
        valL = ADC1BUF0;
        IFS0bits.AD1IF = 0;  // Left sensor to valL

        sprintf(printVal,"%d",valL);
        if (print >= printInterval) {       
            move_cursor_lcd(0,1);
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
            print_string_lcd(printVal);
            print = 0;
        }
        else print += 1;
        
        if (print >= printInterval) { 
             move_cursor_lcd(5,1);
             sprintf(printVal,"%d",counter);
             print_string_lcd(printVal);
        }
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 