#include <xc.h>
#pragma config JTAGEN = OFF

#pragma config ICESEL = ICS_PGx3

#pragma config WINDIS = OFF

#pragma config FWDTEN = OFF

#pragma config FSOSCEN = OFF

#pragma config PMDL1WAY = OFF

#pragma config IOL1WAY = OFF

#pragma config FNOSC = FRCDIV

#pragma config FPBDIV = DIV_8

#include <sys/attribs.h>

//Simple delay w/o timers because of lab order
void delay(){
    int i, j;
    for(i = 0; i < 400; i++)
        for(j = 0; j < 400; j++);
}

void __ISR(3) example(void){
    LATB = 15;
    delay();
    IFS0bits.INT0IF =0;
}

main(){
    TRISB = 0;
    int count = 0;
    TRISBbits.TRISB7 = 1;
    CFGCONbits.JTAGEN = 0;
    __builtin_enable_interrupts();
    INTCONbits.MVEC = 1;
    CNPDBbits.CNPDB7 = 1;
    IEC0bits.INT0IE = 0;
    INTCONbits.INT0EP = 1;
    IFS0bits.INT0IF = 0;
    IPC0bits.INT0IP = 1;
    IEC0bits.INT0IE = 1;
    
    while(1){
        LATB = count;	// Write count to output pins 
        count++;
        if(count > 15)
            count = 0;
        delay();
    }
}
