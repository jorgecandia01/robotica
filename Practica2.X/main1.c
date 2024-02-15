#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <stdint.h>

#define LED 0

int main(void) {
    
    TRISA = 0; //No hay inputs en todo el programa
    TRISB = 0;
    TRISC = 0;
    
    ANSELC = 0;
    
    LATA = 0;
    LATB = 0;
    LATC = 1; //Empiezo con el LED apagado
    
    //Configuro el timer 2
    T2CON = 0; //A modo de "reset"
    TMR2 = 0; //Cuenta a 0
    IFS0bits.T2IF = 0; //Flag a 0
    PR2 = 19530; // PR = 1 x 5000000 x 1/256 - 1
    T2CON = 0x8070; //ON y preescalado a 1:256
    
    
    while(1){
       while(IFS0bits.T2IF == 0);
       LATC ^= (1 << LED);
       IFS0bits.T2IF = 0;
    }
    
    
}

