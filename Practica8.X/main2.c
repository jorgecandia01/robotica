#include <xc.h>
#include <stdint.h>
#include "Pic32Ini.h"

#define PIN_SERVO 15 //RB15 -> OC1
#define PIN_PULSADOR 5 //RB5 -> +20º
#define PIN_PULSADOR_DECREMENTO 7 //RB7 -> -10º

#define INC_20_GRADOS 277;
#define DEC_10_GRADOS 138;


void initOC1(void){ //Conecto OC1 al servo -> genero mi señal con OC1 y se la entrego al servo
    
    //Configuro timer2
    T2CON = 0;
    TMR2 = 0;
    PR2 = 49999; //Inicio en -90º -> 1ms -> 2500
    //IFS0CLR = 1 << 9;
    T2CON = 0x8010;
    
    //Configuro OC1
    OC1CON = 0;
    OC1R = 2500; //1ms
    OC1RS = 2500; //Aumento este al pulsar
    OC1CON = 0x8006;

}


int main(void){
    
    //ANSELBCLR = (1<<PIN_SERVO || 1<<PIN_PULSADOR); 
    ANSELC = ~0xF;
    
    TRISA = 0;
    TRISB = ((1 << PIN_PULSADOR)|(1 << PIN_PULSADOR_DECREMENTO));
    TRISC = 0;
    
    LATA = 0;
    LATB = 0; //Servo empieza a 0
    LATC = 0xF; //LEDS apagados
    
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPB15R = 5;
    SYSKEY = 0x1CA11CA1;

    initOC1();
    
    int pulsador_ant = (PORTB >> PIN_PULSADOR) & 1;
    int pulsador_decremento_ant = (PORTB >> PIN_PULSADOR_DECREMENTO) & 1;
    int t_alto = 2500;
    int pulsador_act, pulsador_decremento_act;
    
    while(1){
        
        pulsador_act = (PORTB >> PIN_PULSADOR) & 1;
        pulsador_decremento_act = (PORTB >> PIN_PULSADOR_DECREMENTO) & 1;
        
        //Flanco +20º
        if((pulsador_act == 0) && (pulsador_ant == 1)){
            t_alto += INC_20_GRADOS;
            
            if(t_alto > 5000)
                t_alto = 2500; //1ms -> -90º -> estado inicial
            
            OC1RS = t_alto;
        }
        
        //Flanco -10º 
        
        if((pulsador_decremento_act == 0) && (pulsador_decremento_ant == 1)){
            t_alto -= DEC_10_GRADOS;
            
            if(t_alto < 2500)
                t_alto = 5000; //1ms -> -90º -> estado inicial, no baja más
            
            OC1RS = t_alto;
        }
        
        pulsador_decremento_ant = pulsador_decremento_act;
        pulsador_ant = pulsador_act;
        
    }
    
}








