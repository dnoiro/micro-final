#include <xc.h>
#include <sys/attribs.h>
#define numChars 18 // use 24 for spiChars2
int i = 0;
int j = 0;
int m = 0;
char spiChars[numChars] = {0, 1, 2, 4, 8, 16, 32, 64, 128, 255,
254, 253, 251, 247, 239, 223, 191, 127};
char spiChars2[numChars] = {0, 1,2,4,8,16,32,64,128,255,
0,3,7,15,31,63,127,254,
252,248,240,224,192,128,};
void __ISR(3) button(void){
LATBbits.LATB0 = 1;
//utilize for loop to 'remove' button jitter
for(m = 0; m < 150; m++)
for(j = 0; j < 312; j++);
LATBbits.LATB0 = 0;
//Pass data to SPI buffer
SPI1BUF = spiChars[i];
//loop around back to the beginning if past number of array values
i++;
if(i > numChars-1){
i=0;
}
//Reset flag
IFS0bits.INT0IF = 0;
}
main(){
//Globally enable interrupts
INTCONbits.MVEC = 1;
__builtin_enable_interrupts();
TRISA = 0; //SS1, SD0
TRISB = 0;
CFGCONbits.JTAGEN = 0; // Disable JTAG to use B7
TRISBbits.TRISB7 = 1;
RPA0R = 3; //SS1 pin 2
RPA1R = 3;//SDO1 pin 3
//Set I/xO pins
//TRISBbits.TRISB0 = 0; //indicator LED
LATBbits.LATB0 = 1; delay(250); LATBbits.LATB0 = 0; //Pin B0 used for debugging
LATB = 0;
//Set up interrupt
INTCONbits.INT0EP = 0; //rising edge
IEC0bits.INT0IE = 1; //enable interrupt
IFS0bits.INT0IF = 1; //set flag
IPC0bits.INT0IP = 1; //Set highest priority
//Set up SPI
//Disable SPI interrupts
IEC1bits.SPI1EIE = 0;
IEC1bits.SPI1RXIE = 0;
IEC1bits.SPI1TXIE = 0;
//Turn SPI off
SPI1CONbits.ON = 0;
//Clear receive buffer
SPI1BUF = 0;
//Disable enhance buffer
SPI1CONbits.ENHBUF = 0;
//Clear SPI flags
IFS1bits.SPI1EIF = 0;
IFS1bits.SPI1RXIF = 0;
IFS1bits.SPI1TXIF = 0;
//Set Baud Rate
SPI1BRG = 1000; //between 2 and 1000 (use 1000)
//Clear the SPIROV bit
SPI1STATbits.SPIROV = 0;
//SPI settings
SPI1CONbits.MSSEN = 1;
SPI1CONbits.MCLKSEL = 0; //Use PB clock
SPI1CONbits.SMP = 1; //end of data output time
SPI1CONbits.CKE = 1; //falling edge trigger
SPI1CONbits.CKP = 0; //idle clock is low
SPI1CONbits.MSTEN = 1; //master mode
SPI1CONbits.DISSDO = 0; //enable DIO
SPI1CONbits.ON = 1;
while(1)
{
}
}
delay(int delayTime){
int k, j;
for(k = 0; k < delayTime; k++)
for(j = 0; j < 312; j++);
}
