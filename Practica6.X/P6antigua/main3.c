#include <stdio.h>
#include <stdlib.h>

#include "Pic32Ini.h"
#include <xc.h>
#include <stdint.h>

#define PIN_RECEPCION 13

static uint32_t lectura = 0;

void initUART1(void){
    //Configuro el pin receptor (Rx)
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    U1RXR = 3;  //Receptor de la UART1 a RB13
    SYSKEY = 0x1CA11CA1;
    
    //Ahora configuro los par�metros de funcionamiento de la UART
    U1BRG = 32; //9600 baudios
    U1STAbits.URXEN = 1; //Siempre habilitado para reibir
    U1MODE = 0x8000; //El on a la UART1, PDSEL a 00 para modo 8N1
}



int main(void){
    
    ANSELB &= ~(1 << PIN_RECEPCION);
    
    TRISA = 0; 
    TRISB = (1 << PIN_RECEPCION);
    TRISC = 0;  //RC0-3 LEDS
    
    LATC = ~0;
    
    initUART1();
    
    
    while(1){ //Gestiono la recepci�n por polling
        if(U1STAbits.URXDA == 1){
            lectura = U1RXREG; //Push and dump el valor m�s alto de la FIFO
        }
        
        //Actualizo los LEDs en cada iteraci�n del bucle
        //lectura |= ~(0xF); //Pongo a 1 todo lo q no me interesa
        LATC = ~lectura;
        
    }
    
}
















