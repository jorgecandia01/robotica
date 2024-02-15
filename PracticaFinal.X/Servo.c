#include <xc.h>
#include <stdint.h>
#include "Main.h"
#include "MaquinaEstados.h"
#include "Pic32Ini.h"
#include "Temporizador4.h"
#include "Servo.h"

//INCLINARÉ 8 VECES 10 GRADOS
#define INC_10_GRADOS 277; //Mirar si es la mitad
#define STOP 4700

static int subiendo = 0; 
static int t_alto = 5000;

void initServo(void){
    
    //Remapeo la salida de OC1 al pin donde voy a conectar el servo -> RB15
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPB15R = 5;
    SYSKEY = 0x1CA11CA1;

    //Inicializo OC1
    OC1CON = 0;
    OC1R = 5000; //Empieza alto
    OC1RS = 5000;
    OC1CON = 0x8006;
    
    //Inicializo T2 -> El que usa el servo
    T2CON = 0;
    TMR2 = 0;
    PR2 = 49999; //20ms de periodo
    T2CON = 0x8010;
    
    //Inicializo T3 para auxiliar las cuentas
    T3CON = 0;
    TMR3 = 0;
    PR3 = 39061; //Cuento 250ms, tckps 1:32 (5)
    
    IFS0bits.T3IF = 0; //Bajo flag
    IEC0bits.T3IE = 1; //Activo mask
    IPC3bits.T3IP = 2; //Prioridad 2
    IPC3bits.T3IS = 3; //Subprioridad 3
    
    T3CON = 0x0050; //OFF, le doy el ON cuando quiera contar
    
    
}


void bajarServo(void){
    
    //T3CON |= 0x8000;
    T3CON = 0x8050;
    subiendo = 0; //bajo el servo
    
}

void subirServo(void){
    
    //T3CON |= 0x8000;
    T3CON = 0x8050;
    subiendo = 1; //subo el servo
    
}




__attribute__((vector(12), interrupt(IPL2SOFT), nomips16)) void InterrupcionTimer3(void){
    //IFS0bits.T3IF = 0; //Bajo la flag para que no entre en bucle 
    IFS0CLR = 1 << 14; //Bajo la flag
    
    if(subiendo == 1){
        t_alto += INC_10_GRADOS;
        
        if(t_alto > STOP){ //Ya ha subido por completo
            t_alto = STOP;
            T3CON = 0x0050; //Desactivo el temporizador
            TMR3 = 0;
            Reposo(); //Cambio de estado en la máquina de estados
        }
    }
    
    if(subiendo == 0){
        t_alto -= INC_10_GRADOS;
        
        if(t_alto < 2550){
            t_alto = 2500;
            T3CON = 0x0050; //Desactivo el temporizador
            TMR3 = 0;
            Abajo(); //Cambio de estado en la máquina de estados
        }
    }
    
    OC1RS = t_alto;
}



