#include <xc.h>
#include <stdint.h>
#include "Main.h"
#include "MaquinaEstados.h"
#include "Pic32Ini.h"
#include "Temporizador4.h"


static int estado = 0;


void Reposo(void){ //En reposo el puente está arriba
    
    estado = 0;
    
    //Estado de reposo (default)
    T4CON = 0;
    TMR4 = 0;
    
    clrContador();
    clrCarrera();
    clrParpadeo();
    
    LATCSET = 1 << 3; //RC3 apagado, LED de NO PASAR
    
    LATCINV = 1 << 0; //Control de estado
}


//Entra cuando flanco de sensor -> bajo el puente
void Bajando(void){
    
    estado = 1;
    
    //Hago RC3 intermitente T=1s
    setParpadeo();
    LATCCLR = 1 << 3; //Empieza encendido
    
    //Activo T4 (LED)
    clrContador();
    T4CON = 0x8000;
    
    //Bajo el servo
    bajarServo();
    
    
    //Sale de bajando cuando acabe de bajar el servo
    LATCINV = 1 << 0; //Control de estado
}


//Entra cuando acaba de bajar el servo
void Abajo(void){
    
    estado = 2;

    //Espero a flanco de pulsador
    clrParpadeo();
    LATCCLR = 1 << 3; //Led activo cuando el puente está bajo
    
    LATCINV = 1 << 0; //Control de estado
}


//Entra cuando flanco de pulsador
void Subiendo(void){
    
    estado = 3;
    
    setParpadeo();
    subirServo();
    
    //Sale cuando acaba de subir el servo
    LATCINV = 1 << 0; //Control de estado
}



int getEstado(void){
    int estado_c;
    asm("di");
    estado_c = estado;
    asm("ei");
    return estado_c;
}










