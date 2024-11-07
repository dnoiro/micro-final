#include <xc.h>
void main() {
    int i;
    unsigned char k = 32; // stores key press
    // sets ports a and b to digital mode
    ANSELB = 0;
    ANSELA = 0;
    // set all pins of ports a and b
    TRISB = 0;
    TRISA = 0;
    
    // sets specific pins on port B as inputs for keypad
    TRISBbits.TRISB12 = 1;
    TRISBbits.TRISB13 = 1;
    TRISBbits.TRISB14 = 1;
    TRISBbits.TRISB15 = 1;
    // clear port a
    LATA = 0;
// keypad mask values to detect key presses
    unsigned char mask[16] = {
        // Col 0        1        2        3
        0xEE, 0xDE, 0xBE, 0x7E, // Row 0
        0xED, 0xDD, 0xBD, 0x7D, // Row 1
        0xEB, 0xDB, 0xBB, 0x7B, // Row 2
        0xE7, 0xD7, 0xB7, 0x77  // Row 3
    };
// corresponding numbers to each keypad button
    unsigned char key[16] = {
        15, 2, 3, 10,
        1, 5, 6, 11,
        4, 8, 9, 12,
        7, 0, 14, 13
    }; // 15 = *, 14 = #

    while (1) {
        // scan keypad
        for (i = 0; i < 16; i++) {
            LATB = mask[i];
            // send mask to port b
            // check if keypad matches the mask
            if (((PORTB & 0xF000) >> 8) == (mask[i] & 0xF0)) {
                k = key[i]; // store the value
                break; // if match exit
            }
        } // if key was pressed
        if (k != 32) {
            LATA = k; // output key value
            k = 32;  // reset key press
        } else {
            LATA = 0; // clear port a. no key was pressed
        }
    }
}
