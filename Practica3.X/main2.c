#include <stdio.h>
#include <stdlib.h>

#include "Temporizador.h"
#include "Pic32Ini.h"
#include <xc.h>

#define PIN_PULSADOR 5
#define PIN_LED_PARPADEO 3
#define T_PARPADEO 1000 //1000ms = 1s

static uint32_t pulsador, ticks_act, ticks_ant; //Hace falta el static para comunicarme entre funciones no?

void PulsaLed(void){
    pulsador = ( PORTB >> PIN_PULSADOR ) & 1;
    
    if( pulsador == 0) { 
        LATC &= ~1; 
    } 
    else { 
        LATC |= 1; 
    }
}


void RefrescarParpadeo(void){
    ticks_act = getTicks();
    
    if((ticks_act - ticks_ant) > T_PARPADEO){
        ticks_ant = ticks_act;
        LATCINV = (1 << PIN_LED_PARPADEO);
    }
    
}


//////////////////////////////////////////////////


int main ( void )
{
    TRISC = 0; //RC0-3 outputs (todo outputs)
    LATC = 0xF; //Leds de RC0-3 empiezan apagados 
    TRISB = 1 << PIN_PULSADOR; 
    ANSELC = 0;
    
    TRISA = 0;
    LATA = 0;
    LATB = 0;
    
    InicializarTimer2();
    INTCONbits.MVEC = 1;
    asm("ei");
    
    ticks_ant = getTicks();

    while (1) {
        
        //Primero refresco el pulsador-LED
        PulsaLed();
        
        //Segundo refesco el LED parpadeante a T=2s -> cada 1s se invierte su estado
        RefrescarParpadeo(); //Desde que se escribe en LATC en PulsaLed, pasa por "mucho c�digo"
        //Antes de llegar a escribir en RefrescarParpadeo, no deber�an haber interferencias/reescrituras no?
        //Luego de vuelta en PulsaLed hace una lectura y un if, suficiente no?
    }
}






