#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <stdint.h>

#include "Pic32Ini.h"
#include <math.h>

#define LED 0

int Retardo (uint16_t retardo_ms ) {
    uint32_t PR = 70000;
    uint16_t tckps = 0; //Podría ponerlo a -1 para q salga todo más bonito?
    
    while(PR > 65535){
        uint16_t preescalado = pow(2,tckps);
        PR = retardo_ms*5000000/1000/preescalado; //poner antes el *5000000 que el 1000, sino PR se queda a 0 (casts)
        if(tckps==7){PR = retardo_ms/1000*5000000/preescalado/2;}
        tckps++;  
    }
    tckps--;
    
    if(tckps<8){
        T2CON = 0; //A modo de "reset"
        TMR2 = 0; //Cuenta a 0
        IFS0bits.T2IF = 0; //Flag a 0
        PR2 = PR; //PR es como máximo 65534
        T2CON = 0x8000; //ON 
        T2CON |= (tckps << 4); //tckps es como máximo 7, por lo que el resto es or con ceros
        
        while(IFS0bits.T2IF == 0);
        T2CON = 0; //Lo dejo apagado (y reseteado), la flag sigue a 1
        
        return(0);
        
    } else {
        return(1);
    }
};


int main(void) {
    
    TRISA = 0; //No hay inputs en todo el programa
    TRISB = 0;
    TRISC = 0;
    
    ANSELC = 0;
    
    LATA = 0;
    LATB = 0;
    LATC = ~0; //Empiezo con el LED apagado
    
    //si PR hace overflow, no entra/sale del bucle
    while(~Retardo(1000)){ //Las funciones se pueden llamar así tb
        LATC ^= (1<<LED);
    }
    
}

