#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "motor.h"
#include "lcd.h"
#include "pwm.h"
#include "timer.h"
#include "adc.h"
#include "string.h"

volatile unsigned int val = 0;
volatile double volts = 0.0;
volatile int idle1;
//typedef enum stateTypeEnum {
//    disp, delay
//} stateType;
//
//volatile stateType state;
    typedef enum stateTypeEnum {
        forward, back, idle,debounce1, debounce2
    } stateType;

    volatile stateType state, stateNext, stateNext2;

int main(void){
    //SYSTEMConfigPerformance(10000000);
    int i;
    state = idle; 
    stateNext = back;
    float Volts = 0.0;
    init_timer_1();
    //enableInterrupts();
    init_lcd();
    clear_lcd();
    initPWM();
    char voltage[8];
    initSW1();
    initADC();
    initMotor();
               move_cursor_lcd(0,1);
        print_string_lcd("lLLLLLL");
    
    while(1){
        if (PORTDbits.RD6 == 0) state = debounce1;
        while(IFS0bits.AD1IF == 0) {
        }
        if (PORTDbits.RD6 == 0) state = debounce1;
        val = ADC1BUF0;
        IFS0bits.AD1IF = 0;
        
        Volts = (float)val*5.0/(1023.0);
            sprintf(voltage, "%.2f",Volts);
            move_cursor_lcd(0,1);
        print_string_lcd(voltage);
        switch(state) {
            case forward:
                stateNext = back;
                idle1 = 0;
                OC1RS = 0;
                OC2RS = 0;
                
                if (Volts > 2.5) {
                    OC3RS = 1023;
                    OC4RS = 2*(1023 - val);   
                }    
                else if (Volts < 2.5) {
                    OC3RS = val*2*1023;
                    OC4RS = 1023;   
                }    
                else {
                    OC3RS = 1023;
                    OC4RS = 1023;
                }

            break;
            
            case back:
                stateNext = forward;
                idle1 = 0;
                OC3RS = 0;
                OC4RS = 0;
                
                if (Volts > 2.5) {
                    OC1RS = 1023;
                    OC2RS = 2*(1023 - val);   
                }    
                else if (Volts < 2.5) {
                    OC1RS = val*2*1023;
                    OC2RS = 1023;   
                }    
                else {
                    OC1RS = 1023;
                    OC2RS = 1023;
                }
                    
            break;
            case idle:
                idle1 = 1;
                OC1RS = 1023;
                OC2RS = 0;
                OC3RS = 0;
                OC4RS = 0;
                break;
                
            case debounce1:
                delay_ms(5);
                if(PORTDbits.RD6 == 1) state = debounce2;
                break;
            case debounce2:
                delay_ms(1);
                if(idle1 == 1)
                    state = stateNext;
                else if (idle1 == 0)
                    state = idle;
                break;
        }
    }

    return 0;
}
//
//void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt( void ){
//    //TODO: Implement the interrupt to capture the press of the button
//    IFS1bits.CNDIF = 0;
//    //PORTD;
////    if (PORTDbits.RD6 == 0) state = debounce1;
////    else if (state == debounce1){
////        if(PORTDbits.RD6 == 1) state = debounce2;
////    }
//    if (state == debounce1) state = debounce2;
//    else state = debounce1;
//}