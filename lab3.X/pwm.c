/* 
 * File:   pwm.h
 * Author: gvanhoy
 *
 * Created on October 8, 2015, 9:14 AM
 */

#include <xc.h>


void initPWM(){
    RPD1Rbits.RPD1R = 0b1011; // map OC2 to RD1
    OC2CON = 0x0000; // Turn off OC2 while doing setup.
    OC2R = 0x0000; // Initialize primary Compare Register
    OC2RS = 0x0000; // Initialize secondary Compare Register
    OC2CON = 0x0006; // Configure for PWM mode
    OC2CONSET = 0x8000; // Enable OC2
    
    RPD3Rbits.RPD3R = 0b1011; // map OC4 to RD3
    OC4CON = 0x0000; // Turn off OC4 while doing setup.
    OC4R = 0x0000; // Initialize primary Compare Register
    OC4RS = 0x0000; // Initialize secondary Compare Register
    OC4CON = 0x0006; // Configure for PWM mode
    OC4CONSET = 0x8000; // Enable OC4
    
    RPD2Rbits.RPD2R = 0b1011; // map OC4 to RD3
    OC3CON = 0x0000; // Turn off OC4 while doing setup.
    OC3R = 0x0000; // Initialize primary Compare Register
    OC3RS = 0x0000; // Initialize secondary Compare Register
    OC3CON = 0x0006; // Configure for PWM mode
    OC3CONSET = 0x8000; // Enable OC4
    
    RPD0Rbits.RPD0R = 0b1011; // map OC4 to RD3
    OC1CON = 0x0000; // Turn off OC4 while doing setup.
    OC1R = 0x0000; // Initialize primary Compare Register
    OC1RS = 0x0000; // Initialize secondary Compare Register
    OC1CON = 0x0006; // Configure for PWM mode
    OC1CONSET = 0x8000; // Enable OC4
}