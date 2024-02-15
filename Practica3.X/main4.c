#include <stdio.h>
#include <stdlib.h>

#include "Temporizador2.h"
#include "Pic32Ini.h"
#include <xc.h>

#define PIN_PULSADOR 5
#define PIN_LED 0
#define T_RETO 1000 //2000ms = 2s
#define P_RETO 5 //5 pulsaciones como reto

static uint32_t contador = 0;
static uint32_t pulsador, tiempo;



void refreshJuego(void){
    //No miro si está dentro de tiempo, eso lo hace la interrupción, si supera el tiempo reinicia contador y tiempo.
    if(contador >= P_RETO){ 
        LATC &= ~(1 >> PIN_LED);
    }   
}

void restartJuego(void){
    contador = 0;
    LATC |= (1 >> PIN_LED);
}

uint32_t getContador(void){
    uint32_t c_contador;
    asm("di"); //Variable compartida, desactivo interrupciones
    c_contador = contador; //Hago la copia porque tengo que hacer "ei" antes del return!!
    asm("ei");
    return c_contador;
}




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
    

    int pulsador_ant , pulsador_act ; 
    pulsador_ant = (PORTB >> PIN_PULSADOR ) & 1;
    
    while (1) {
           
        pulsador_act = ( PORTB >> PIN_PULSADOR ) & 1;
        if( ( pulsador_act != pulsador_ant ) && ( pulsador_act== 0) ) { 
            contador++;         
        }
        pulsador_ant = pulsador_act; 
        
        refreshJuego();
    }
}









