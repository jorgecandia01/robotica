#ifndef TEMPORIZADOR4_H
#define TEMPORIZADOR4_H
#include <stdint.h> // Define uint32_t

void InicializarTimer4(void); //en __attribute__ no, pero aqu� puedo meter par�metros?
uint32_t getContador(void);
void clrContador(void);


#endif 