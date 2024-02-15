#include <stdio.h>
#include <stdlib.h>

#include "Pic32Ini.h"
#include <xc.h>
#include <stdint.h>

#define PIN_RECEPCION 13
#define PIN_TRANSMISION 7
#define PIN_PULSADOR 5

static uint32_t lectura = 0;
static char mensaje[] = "Hola ICAI!"; //Lleva el '\0' al final automáticamente

void initUART1(void){
    //Configuro el pin receptor (Rx)
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    U1RXR = 3;  //Receptor de la UART1 a RB13
    RPB7R = 1;  //Emisor de la UART1 a RB7   
    SYSKEY = 0x1CA11CA1;
    
    //Ahora configuro los parámetros de funcionamiento de la UART
    U1BRG = 32; //9600 baudios
    U1STAbits.URXEN = 1; //Siempre habilitado para reibir
    LATB |= (1 << PIN_TRANSMISION); //A 1 al empezar, bit de stop
    U1MODE = 0x8000; //El on a la UART1, PDSEL a 00 para modo 8N1
}

void transmitir(void){
    U1STAbits.UTXEN = 1;
    for(int i=0; mensaje[i] != '\0'; i++){
        U1TXREG = mensaje[i];
        while(U1STAbits.TRMT == 0);
    }
    U1STAbits.UTXEN = 0;
}



int main(void){
    
    ANSELB &= ~((1 << PIN_RECEPCION) | (1 << PIN_TRANSMISION) | (1 << PIN_PULSADOR));
    
    TRISA = 0; 
    TRISB = ((1 << PIN_RECEPCION) | (1 << PIN_TRANSMISION) | (1 << PIN_PULSADOR));
    TRISC = 0;  //RC0-3 LEDS
    
    LATC = ~0;

    initUART1();
    
    int pulsador_ant , pulsador_act ; 
    pulsador_ant = (PORTB >> PIN_PULSADOR ) & 1;
    
    while(1){ //Gestiono la recepción por polling
        
        ////////// RECEPCIÓN //////////
        if(U1STAbits.URXDA == 1){
            lectura = U1RXREG; //Push and dump el valor más alto de la FIFO
        }
        
        //Actualizo los LEDs en cada iteración del bucle
        LATC = ~lectura;
        
        
        ////////// TRANSMISIÓN //////////
        pulsador_act = ( PORTB >> PIN_PULSADOR ) & 1;
        if( ( pulsador_act != pulsador_ant ) && ( pulsador_act== 0) ) { 
            transmitir();      
        }
        pulsador_ant = pulsador_act; 
        
    }
    
}

















