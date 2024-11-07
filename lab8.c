#include <sys/attribs.h>
#include <xc.h>
int dty;
void delay(){
int i, j;
for(i = 0; i < 800; i++)
for(j = 0; j < 800; j++);
}
void main() {
 ANSELA = 0;
 TRISA = 0;
INTCONbits.MVEC = 1;
__builtin_enable_interrupts();
RPA0R = 5; // Double Check
OC1CONbits.ON = 0; // Ensure OC is off during setup
OC1CONbits.OC32 = 0; // Don't need 32-bit mode
OC1CONbits.OCTSEL = 0; // Select timer (Timer 2 in this case)
OC1CONbits.OCM = 6; // PWM mode
dty = 0;
OC1R = 0;
OC1RS = 0;
T2CONbits.ON = 0;
PR2 = 100;
T2CONbits.ON = 1;
OC1CONbits.ON = 1; // Enable OC when finished
while(1){
dty += 25;
if(dty > 100)
dty = 0;
OC1RS = dty; // Write to OC register (duty cycle will be OC2RS/PR2)
delay();
}
}
