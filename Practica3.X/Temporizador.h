#ifndef TIMER_H
#define TIMER_H
#include <stdint.h> // Define uint32_t

void InicializarTimer2(void); //en __attribute__ no, pero aqu� puedo meter par�metros?
uint32_t getTicks(void);

#endif /* TIMER_H */ 