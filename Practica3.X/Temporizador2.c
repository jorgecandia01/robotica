#include "Temporizador.h"
#include <xc.h>
#include "main4.h"

#define T_RETO 1000 //2000ms = 2s
#define P_RETO 5 //5 pulsaciones como reto

static uint32_t ticks, ticks2, cont;

void InicializarTimer2(void){ //Inicialización del timer para que genere una interrupción cada 1ms
    T2CON = 0X0000;
    TMR2 = 0;
    PR2 = 4999; //1ms
    IPC2bits.T2IP = 2; // Prioridad 2
    IPC2bits.T2IS = 0; // Subprioridad 0
    IFS0bits.T2IF = 0; // Flag a 0
    IEC0bits.T2IE = 1; // Habilito que este módulo pueda generar interrupciones(????????)
    T2CON = 0x8000; //ON con preescalado 1:1
}

//Esto será lo que pase cuando salte la interrupción, se ejecutará esto
__attribute__((vector(_TIMER_2_VECTOR), interrupt(IPL2SOFT), nomips16)) void InterrupcionTimer2(void){
    IFS0bits.T2IF = 0; //Bajo la flag para que no entre en bucle por aquí (??????)
    ticks++;
    
    cont = getContador(); //Probar a meter esto en el if
    
    if((ticks > T_RETO) && (cont < P_RETO)){
        restartJuego();
        ticks = 0;
    }
    
    if((ticks > 4000) && (cont >= P_RETO)){
        restartJuego();
        ticks = 0;
    }
    
}


//Pillo los ticks (ms) que han pasado con un getter
uint32_t getTicks(void){
    uint32_t c_ticks; //variable compartida!! Desactivo las interrupciones
    asm("di");
    c_ticks = ticks; //Hago la copia porque tengo que hacer "ei" antes del return!!
    asm("ei");
    return c_ticks;
}

