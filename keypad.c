#include <xc.h>
#include "keypad.h"
#include "timer.h"

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void){
    //inputs
    TRISFbits.TRISF1 = INPUT;   // Enable input for switch
    CNCONFbits.ON = 1;          // Turn on CN device
    CNENFbits.CNIEF1 = 1;       // Enable CN interrupt for pin
    IEC1bits.CNDIE = 1;         // Enable overall CN Interrupt
    IFS1bits.CNDIF = 0;         // Put Interrupt flag down
    CNPUFbits.CNPUF1 = 1;        // Enable Internal Pull-up resistor **NOT NECESSARY?
    
    TRISGbits.TRISG13 = INPUT;   // Enable input for switch
    CNCONGbits.ON = 1;          // Turn on CN device
    CNENGbits.CNIEG13 = 1;       // Enable CN interrupt for pin
    CNPUGbits.CNPUG13 = 1;        // Enable Internal Pull-up resistor **NOT NECESSARY?
    
    TRISDbits.TRISD6 = INPUT;   // Enable input for switch
    CNCONDbits.ON = 1;          // Turn on CN device
    CNENDbits.CNIED16 = 1;       // Enable CN interrupt for pin
    CNPUDbits.CNPUD16 = 1;        // Enable Internal Pull-up resistor **NOT NECESSARY?
    
    
    //ODC's
    ODCGbits.ODCG0 = 1; //Not sure if this is adequate. Not comfortable with ODC initialization at this point
    LATGbits.LATG0 = 1;
    
    ODCDbits.ODCD13 = 1;
    LATDbits.LATD13 = 1;
    
    ODCCbits.ODCC13 = 1;
    LATCbits.LATC13 = 1;
    
    ODCDbits.ODCD12 = 1;
    LATDbits.LATD12 = 1;
    
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void){
    char key = -1;
    
    //1st row
    LATGbits.LATG0 = 1;
    LATDbits.LATD13 = 0;
    LATCbits.LATC13 = 0;
    LATDbits.LATD12 = 0;
        
    if(PORTFbits.RF1 == 0) key = 1;
    if(PORTGbits.RG13 == 0) key = 2;
    if(PORTDbits.RD6 == 0) key = 3;
    
    //2nd row
    LATGbits.LATG0 = 0;
    LATDbits.LATD13 = 1;
    LATCbits.LATC13 = 0;
    LATDbits.LATD12 = 0;
        
    if(PORTFbits.RF1 == 0) key = 4;
    if(PORTGbits.RG13 == 0) key = 5;
    if(PORTDbits.RD6 == 0) key = 6;
    
    //3rd row
    LATGbits.LATG0 = 0;
    LATDbits.LATD13 = 0;
    LATCbits.LATC13 = 1;
    LATDbits.LATD12 = 0;
        
    if(PORTFbits.RF1 == 0) key = 7;
    if(PORTGbits.RG13 == 0) key =8;
    if(PORTDbits.RD6 == 0) key = 9;
    
    //4th row
    LATGbits.LATG0 = 0;
    LATDbits.LATD13 = 0;
    LATCbits.LATC13 = 0;
    LATDbits.LATD12 = 1;
        
    if(PORTFbits.RF1 == 0) key = '*';
    if(PORTGbits.RG13 == 0) key =0;
    if(PORTDbits.RD6 == 0) key = '#';
    
    //set all to one so next input will be detected (necessary?))
    LATGbits.LATG0 = 1;
    LATDbits.LATD13 = 1;
    LATCbits.LATC13 = 1;
    LATDbits.LATD12 = 1;
    
    return key;
}
