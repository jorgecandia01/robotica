#include <xc.h>
#include <stdint.h>
#include "Pic32Ini.h"

#define INC_10_GRADOS 138*2; //Mejor 138 para que no haya overflow prematuro 
//(139*18 = 2502 -> llega al máximo antes de lo debido)

static int t_alto = 2500;
static int subiendo = 1;

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

void initT3(void){
    T3CON = 0;
    TMR3 = 0;
    PR3 = 39063; //Cuento 500ms
    
    IFS0CLR = 1 << 14;
    IEC0SET = 1 << 14;
    IPC3SET = 10; //Prioridad 2 subprioridad 2
    
    T3CON = 0x8060;
    
    INTCON |= 1 << 12;
    asm("ei");
    
}

__attribute__((vector(12), interrupt(IPL2SOFT), nomips16)) void InterrupcionTimer3(void){
    //IFS0bits.T3IF = 0; //Bajo la flag para que no entre en bucle 
    IFS0CLR = 1 << 14; //Bajo la flag
    
    if(subiendo == 1){
        t_alto += INC_10_GRADOS;
        LATCINV = 1 << 2;
        if(t_alto > 5000){
            t_alto -= 5000 - INC_10_GRADOS; 
            subiendo = 0;
            
        }
    }
    
    if(subiendo == 0){
        t_alto -= INC_10_GRADOS;
        LATCINV = 1 << 2;
        if(t_alto < 2500){
            t_alto = 2500 + INC_10_GRADOS;
            subiendo = 1;
            
        }
    }
    
    //LATC |= subiendo << 2;
    LATCCLR = 1 << subiendo;
    LATCINV = 1 << 3;
    OC1RS = t_alto;
}


int main(void){
 
    ANSELC = ~0xF;
    
    TRISA = 0;
    TRISB = 0;
    TRISC = 0;
    
    LATA = 0;
    LATB = 0; //Servo empieza a 0
    LATC = 0xF; //LEDS apagados
    
    SYSKEY = 0xAA996655;
    SYSKEY = 0x556699AA;
    RPB15R = 5;
    SYSKEY = 0x1CA11CA1;

    initOC1();
    initT3();
    
    while(1){
        
        
    }
    
}




