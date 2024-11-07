#include <xc.h>
#include <sys/attribs.h>
#pragma config JTAGEN = OFF
#pragma config ICESEL = ICS_PGx3
#pragma config WINDIS = OFF // Watchdog Timer Window Enable (Watchdog
#pragma config FWDTEN = OFF // Watchdog Timer Enable (WDT Enabled)
#pragma config FSOSCEN = OFF // Secondary Oscillator Enable (Enabled)
#pragma config PMDL1WAY = OFF // Peripheral Module Disable Configuration
#pragma config IOL1WAY = OFF // Peripheral Pin Select Configuration
#pragma config FNOSC = FRCDIV // Oscillator Selection Bits (Fast RC Osc
#pragma config FPBDIV = DIV_8 // Peripheral Clock Divisor (Pb_Clk is

int count = 0;

void __ISR(3) Interruptfunction(void)
{
    if (PORTBbits.RB7 == PORTBbits.RB9) 
        count++;
    else
          count--;  
    
    INTCONbits.INT0EP = ~INTCONbits.INT0EP;
    
    IFS0bits.INT0IF = 0; 
}

void __ISR(7) Interruptfunction1(void)
{
    
    if (PORTBbits.RB9 == PORTBbits.RB7) 
        count--;
    else
          count++;  
    INTCONbits.INT1EP = !INTCONbits.INT1EP;
    
    IFS0bits.INT1IF = 0; 
}

main(){
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
    CFGCONbits.JTAGEN=0;
    
    IEC0bits.INT0IE = 1;
    INTCONbits.INT0EP = 1;
    IFS0bits.INT0IF = 0;
    IPC0bits.INT0IP = 1;
    
    IEC0bits.INT1IE = 1;
    INTCONbits.INT1EP = 1;
    IFS0bits.INT1IF = 0;
    IPC1bits.INT1IP = 1;
    
    TRISA = 0xFFF0; //Set all pins to output.
    TRISBbits.TRISB7 = 1;
    TRISBbits.TRISB9 = 1;
    INT1R = 0b0100;
    

    while(1){
        LATA = count; //Output count to B
      
        if(count > 15)//Restrict count to 0-15, needing only 4 bits
            count = 0;
        if (count < 0)
            count = 15;
    }
}
