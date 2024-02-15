#include <xc.h>
#include <stdint.h>
#include "Main.h"
#include "MaquinaEstados.h"
#include "Pic32Ini.h"
#include "Temporizador4.h"

#define PIN_PULSADOR 5

//T4 se usará como temporizador general, para tareas no específicas a ningún otro módulo

static uint32_t contador;
static int parpadeo = 0;

void initT4(void){
    
    //Configuro la cuenta
    T4CON = 0;
    TMR4 = 0;
    PR4 = 4999; //Interrupción cada 1ms
    T4CON = 0; //tckps a 1:1 y ON APAGADO -> lo enciendo cuando lo necesite
    
    //Configuro interrupción -- CAMBIAR A MÁSCARAS CUANDO VEA QUE FUNCIONA
    IFS0bits.T4IF = 0; //Bajo flag
    IEC0bits.T4IE = 1; //Activo mask
    IPC4bits.T4IP = 2; //Prioridad 2
    IPC4bits.T4IS = 2; //Subprioridad 2
    
}


__attribute__((vector(_TIMER_4_VECTOR), interrupt(IPL2SOFT), nomips16)) 
void InterrupcionTimer4(void){
    IFS0bits.T4IF = 0; //Bajo la flag
    contador++;
    
    if(parpadeo == 1)
        if(contador % 500 == 0)
            LATCINV = 1 << 3;
    
    
    if(getEstado() == 2)
        if(PORTB >> PIN_PULSADOR == 0)
            Subiendo();
    
}


//contador es variable compartida
uint32_t getContador(void){
    uint32_t contador_c;
    asm("di");
    contador_c = contador;
    asm("ei");
    return contador_c;
}


void clrContador(void){
    contador = 0;
}

void setParpadeo(void){
    parpadeo = 1;
}

void clrParpadeo(void){
    parpadeo = 0;
}



