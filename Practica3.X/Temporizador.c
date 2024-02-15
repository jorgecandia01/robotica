#include "Temporizador.h"
#include <xc.h>

static uint32_t ticks, ticks2 = 0;

void InicializarTimer2(void){ //Inicialización del timer para que genere una interrupción cada 1ms
    T2CON = 0X0000;
    TMR2 = 0;
    PR2 = 4999; //1ms
    IPC2bits.T2IP = 2; // Prioridad 2
    IPC2bits.T2IS = 0; // Subprioridad 0
    IFS0bits.T2IF = 0; // Flag a 0
    IEC0bits.T2IE = 1; //Habilito que este módulo pueda generar interrupciones(????????)
    T2CON = 0x8000; //ON con preescalado 1:1
}

//Esto será lo que pase cuando salte la interrupción, se ejecutará esto
__attribute__((vector(_TIMER_2_VECTOR), interrupt(IPL2SOFT), nomips16)) void InterrupcionTimer2(void){
    IFS0bits.T2IF = 0; //Bajo la flag para que no entre en bucle por aquí (??????)
    ticks++;
}

//Pillo los ticks (ms) que han pasado con un getter
uint32_t getTicks(void){
    uint32_t c_ticks; //variable compartida!! Desactivo las interrupciones
    asm("di");
    c_ticks = ticks; //Hago la copia porque tengo que hacer "ei" antes del return!!
    asm("ei");
    return c_ticks;
}



//////////////////////////////////////////////////



void InicializarTimer3(void){ //Inicialización del timer para que genere una interrupción cada 1ms
    T3CON = 0X0000;
    TMR3 = 0;
    PR3 = 4999; //1ms
    IPC3bits.T3IP = 4; // Prioridad 4
    IPC3bits.T3IS = 0; // Subprioridad 0
    IFS0bits.T3IF = 0; // Flag a 0
    IEC0bits.T3IE = 1; //Habilito que este módulo pueda generar interrupciones(????????)
    T3CON = 0x8000; //ON con preescalado 1:1
}

//Esto será lo que pase cuando salte la interrupción, se ejecutará esto
__attribute__((vector(_TIMER_3_VECTOR), interrupt(IPL4SOFT), nomips16)) void InterrupcionTimer3(void){
    IFS0bits.T3IF = 0; //Bajo la flag para que no entre en bucle por aquí (??????)
    ticks2++;
}

//Pillo los ticks (ms) que han pasado con un getter
uint32_t getTicks2(void){
    uint32_t c_ticks2; //variable compartida!! Desactivo las interrupciones
    asm("di");
    c_ticks2 = ticks2; //Hago la copia porque tengo que hacer "ei" antes del return!!
    asm("ei");
    return c_ticks2;
}
















