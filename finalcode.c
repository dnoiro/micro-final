#include <xc.h>
#include <sys/attribs.h>

// Configuration Bits
#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF
#pragma config FNOSC = FRC
#pragma config FSOSCEN = OFF

// Function Prototypes
void setupInterrupts();
void setupPWM();
void setupTimer();

// Interrupt Service Routines
void __ISR(_EXTERNAL_0_VECTOR, IPL7SRS) ButtonInterrupt(void) {
    // Toggle an LED on button press
    LATAINV = 0x01; // Toggle RA0

    // Optionally, start/stop the buzzer
    OC1CONbits.ON ^= 1; // Toggle OC1 (buzzer) on/off

    // Clear the interrupt flag
    IFS0bits.INT0IF = 0;
}

void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3Handler(void) {
    // Toggle an LED connected to RA2
    LATAINV = 0x04; // Toggle RA2

    // Clear the interrupt flag
    IFS0bits.T3IF = 0;
}

int main() {
    // Initial setup
    ANSELA = 0; // Set PORTA as digital
    ANSELB = 0; // Set PORTB as digital
    TRISA = 0x00; // PORTA as output (RA0: LED, RA1: Buzzer, RA2: LED)
    TRISBbits.TRISB7 = 1; // RB7 as input (button)

    // Setup functions
    setupInterrupts();
    setupPWM();
    setupTimer();

    while (1) {
        // Main loop can perform other tasks or enter sleep mode
        // For demonstration, we'll adjust the buzzer frequency
        int frequency = 1000; // 1kHz
        int dutyCycle = 50; // 50%

        // Adjust PWM frequency by changing PR2
        PR2 = (8000000 / (frequency * 1)) - 1; // Adjust for prescaler 1:1

        // Adjust duty cycle
        OC1RS = (PR2 + 1) * dutyCycle / 100;

        // Small delay
        for (volatile int i = 0; i < 100000; i++);
    }
    return 0;
}

// Function Definitions
void setupInterrupts() {
    INTCONbits.MVEC = 1; // Enable multi-vector interrupts
    __builtin_enable_interrupts();

    // Configure INT0 for falling edge trigger (button press)
    INTCONbits.INT0EP = 1;

    // Set interrupt priority and subpriority
    IPC0bits.INT0IP = 7;
    IPC0bits.INT0IS = 3;

    // Clear interrupt flag and enable interrupt
    IFS0bits.INT0IF = 0;
    IEC0bits.INT0IE = 1;
}

void setupPWM() {
    // Map OC1 to a pin (e.g., RA1)
    RPA1R = 0b0101; // Assign OC1 to RA1

    // Configure Timer2
    T2CONbits.TCKPS = 0; // Prescaler 1:1
    PR2 = 39999; // Set period for PWM frequency (~1kHz)
    TMR2 = 0;

    // Configure OC1
    OC1CONbits.OCM = 0b110; // PWM mode without fault pin
    OC1CONbits.OCTSEL = 0; // Use Timer2
    OC1RS = 20000; // 50% duty cycle
    OC1R = 20000;

    // Enable Timer2 and OC1
    T2CONbits.ON = 1;
    OC1CONbits.ON = 1;
}

void setupTimer() {
    // Configure Timer3
    T3CONbits.TCKPS = 0b111; // Prescaler 1:256
    PR3 = 31250; // For 1Hz with 8MHz clock and prescaler
    TMR3 = 0;

    // Set up Timer3 interrupt
    IPC3bits.T3IP = 5;
    IPC3bits.T3IS = 0;
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1;

    // Enable Timer3
    T3CONbits.ON = 1;
}
