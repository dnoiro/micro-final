#include <xc.h>
#include <sys/attribs.h>

// Configuration Bits (Adjust according to your microcontroller's datasheet)
#pragma config JTAGEN = OFF     // Disable JTAG
#pragma config FWDTEN = OFF     // Disable Watchdog Timer
#pragma config FNOSC = FRC      // Oscillator Selection
#pragma config FSOSCEN = OFF    // Disable Secondary Oscillator

// Function Prototypes
void setupPins();
void setupInterrupts();
void setupPWM();
void setupTimer();

// Interrupt Service Routines

// External Interrupt 0 (Button Press)
void __ISR(_EXTERNAL_0_VECTOR, IPL7SRS) ButtonInterrupt(void) {
    LATAINV = 0x01;           // Toggle LED on RA0
    OC1CONbits.ON ^= 1;       // Toggle Buzzer ON/OFF (OC1 on RA1)
    IFS0bits.INT0IF = 0;      // Clear interrupt flag
}

// Timer 3 Interrupt (LED Blinker)
void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3Handler(void) {
    LATAINV = 0x04;           // Toggle LED on RA2
    IFS0bits.T3IF = 0;        // Clear interrupt flag
}

int main() {
    setupPins();
    setupInterrupts();
    setupPWM();
    setupTimer();

    while (1) {
        // Main loop can perform other tasks or adjust PWM if needed
    }
    return 0;
}

void setupPins() {
    // Disable analog inputs on PORTA and PORTB
    ANSELA = 0;
    ANSELB = 0;

    // Set RA0, RA1, RA2 as outputs (LEDs and Buzzer)
    TRISAbits.TRISA0 = 0;     // RA0 - LED Indicator
    TRISAbits.TRISA1 = 0;     // RA1 - Buzzer PWM Output
    TRISAbits.TRISA2 = 0;     // RA2 - LED Blinker

    // Set RB7 as input (Push Button)
    TRISBbits.TRISB7 = 1;     // RB7 - Button Input

    // Map OC1 to RA1 for PWM output to the buzzer
    RPA1R = 0b0101;           // Map OC1 output to RA1
}

void setupInterrupts() {
    INTCONbits.MVEC = 1;      // Enable multi-vector interrupts
    __builtin_enable_interrupts();

    // Configure INT0 (External Interrupt 0) for falling edge trigger
    INTCONbits.INT0EP = 1;    // Interrupt on falling edge (button press)

    // Set interrupt priority and subpriority for INT0
    IPC0bits.INT0IP = 7;      // Priority level 7
    IPC0bits.INT0IS = 3;      // Subpriority level 3

    // Clear interrupt flag and enable INT0
    IFS0bits.INT0IF = 0;      // Clear INT0 interrupt flag
    IEC0bits.INT0IE = 1;      // Enable INT0 interrupt
}

void setupPWM() {
    // Configure Timer2 for PWM
    T2CONbits.TCKPS = 0;      // Timer2 prescaler N=1
    PR2 = 39999;              // Period register for ~1kHz PWM frequency
    TMR2 = 0;                 // Clear Timer2 counter

    // Configure Output Compare Module 1 (OC1) for PWM mode
    OC1CONbits.OCM = 0b110;   // PWM mode without fault pin
    OC1CONbits.OCTSEL = 0;    // Use Timer2 as clock source
    OC1RS = 20000;            // Duty cycle (50%)
    OC1R = 20000;             // Initialize OC1R

    // Enable Timer2 and OC1
    T2CONbits.ON = 1;         // Turn on Timer2
    OC1CONbits.ON = 1;        // Turn on OC1
}

void setupTimer() {
    // Configure Timer3 for 1Hz interrupts (LED blinker)
    T3CONbits.TCKPS = 0b111;  // Timer3 prescaler N=256
    PR3 = 31250;              // Period register for 1Hz with 8MHz clock
    TMR3 = 0;                 // Clear Timer3 counter

    // Set interrupt priority and subpriority for Timer3
    IPC3bits.T3IP = 5;        // Priority level 5
    IPC3bits.T3IS = 0;        // Subpriority level 0

    // Clear interrupt flag and enable Timer3 interrupt
    IFS0bits.T3IF = 0;        // Clear Timer3 interrupt flag
    IEC0bits.T3IE = 1;        // Enable Timer3 interrupt

    // Enable Timer3
    T3CONbits.ON = 1;         // Turn on Timer3
}
