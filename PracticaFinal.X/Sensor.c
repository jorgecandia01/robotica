#include <xc.h>
#include <stdint.h>
#include "Main.h"
#include "MaquinaEstados.h"
#include "Pic32Ini.h"
#include "Temporizador4.h"
#include "Servo.h"

#define PIN_SENSOR 9 //RA9

static int lectura;

void initSensor(void){
    
    //TRISA |= 1 << PIN_SENSOR; //Sensor como entrada (1)
    
    //ANSELBCLR = 1 << PIN_SENSOR; //Entrada digital (0)
    
}


int leerSensor(void){
    
    lectura = (PORTA >> PIN_SENSOR)&1;
    
    if(lectura == 1)
        LATCCLR = 1 << 2;
    else
        LATCSET = 1 << 2;
    
    return lectura;
    
}












