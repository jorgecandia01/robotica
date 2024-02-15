    #include <xc.h>
#include <stdint.h>
#include "Main.h"
#include "MaquinaEstados.h"
#include "Pic32Ini.h"
#include "Temporizador4.h"
#include "Servo.h"
#include "Sensor.h"

#define PIN_PULSADOR 5
#define PIN_SENSOR 9 //RA9

static int initCarrera, carrera, sensor = 0;

void initPines(void){
    
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    
    //Servo RB15 out, Pulsador RB5 in
    //Sensor RB7 in en su init
    TRISA = 1 << PIN_SENSOR;
    TRISB = 1 << PIN_PULSADOR; 
    //TRISB = ((1 << PIN_PULSADOR) | (1 << PIN_SENSOR)); 
    TRISC = 0;
    
    LATA = 0;
    LATB = 0;
    LATC = 0xF; //Leds apagados
    
}

void main(void){
    
    //Inicializaciones
    initPines();
    initT4();
    initServo(); //Dentro ya se inicializan T2 y T3
    //initSensor();
    //TRISB |= 1 << PIN_SENSOR;
    
    //Activo interrupciones
    INTCONbits.MVEC = 1;
    asm("ei");
    
    while(1){
        
        //sensor = leerSensor();
        
        if(leerSensor() == 0)
            initCarrera = 1;
        else
            initCarrera = 0;
        
        if(carrera == 0 && initCarrera == 1){
            carrera = 1; //Se queda a 1 hasta volver a Reposo()
            Bajando();
        }
        
        
        
        
        
    }
}



void clrCarrera(void){
    carrera = 0;
}














