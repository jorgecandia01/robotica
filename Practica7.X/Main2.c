#include <xc.h>
#include <stdint.h>
#include "Pic32Ini.h"
#include "DriverUART2.h"

#define baudios 9600


int main(void){
    
    InicializarUART12(baudios);
    char limpiezaInit = getcUART2();
    
    while(1){
        
    }
    
}













