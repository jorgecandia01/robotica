#include <xc.h>
#include <stdint.h>
#include "Pic32Ini.h"
#include "DriverUART1.h"

#define baudios 9600


int main(void){
    
    InicializarUART1(baudios);
    
    char eco[2];
    eco[0] = '\0';
    eco[1] = '\0';
    
    while(1){
        
        char c = getcUART();
        
        if(c != '\0'){
            eco[0] = c;
            putsUART(eco);
        }
    }
    
    
}

















