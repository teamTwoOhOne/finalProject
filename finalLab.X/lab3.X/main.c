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
    state = forward;    
    float Volts = 0.0;
    float VoltNew = 0.0;
    init_timer_1();
    enableInterrupts();
    init_lcd();
    clear_lcd();
    initPWM();
    char voltage[8];

    initADC();
    initMotor();
    
    

    
    while(1){
        while(IFS0bits.AD1IF == 0) {
        }
        val = ADC1BUF0;
        IFS0bits.AD1IF = 0;

        VoltNew = (float)val*5.0/(1023.0);
            Volts = VoltNew;
            sprintf(voltage, "%.2f",Volts);
            move_cursor_lcd(0,1);
        print_string_lcd(voltage);
        switch(state) {
            case forward:
                idle1 = 0;
                OC1RS = 0;
                OC2RS = 0;
                
                if (Volts > 2.5) {;
                    OC3RS = 1023;
                    OC4RS = (1- (Volts - 2.5)/2.5)*1023;   
                }    
                else if (Volts < 2.5) {
                    OC3RS = (1- (2.5 - Volts)/2.5)*1023;
                    OC4RS = 1023;   
                }    
                else {
                    OC3RS = 1023;
                    OC4RS = 1023;
                }
                stateNext = back;
            break;
            case back:
                idle1 = 0;
                OC3RS = 0;
                OC4RS = 0;
                
                if (Volts > 2.5) {;
                    OC1RS = 1023;
                    OC2RS = (1- (Volts - 2.5)/2.5)*1023;   
                }    
                else if (Volts < 2.5) {
                    OC1RS = (1- (2.5 - Volts)/2.5)*1023;
                    OC2RS = 1023;   
                }    
                else {
                    OC1RS = 1023;
                    OC2RS = 1023;
                }
                stateNext = forward;
                
            break;
            case idle:
                OC1RS = 0;
                OC2RS = 0;
                OC3RS = 0;
                OC4RS = 0;
                idle1 = 1;
                break;
            case debounce1:
                delay_ms(40);
                break;
            case debounce2:
                delay_ms(40);
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
void __ISR(_CHANGE_NOTICE_VECTOR, IPL2SRS) _CNInterrupt( void ){
    //TODO: Implement the interrupt to capture the press of the button
    IFS1bits.CNGIF = 0;
    if (PORTDbits.RD6) state = debounce1;
    
    else if (state == debounce1){
        if(!PORTDbits.RD6) state = debounce2;
    }
}