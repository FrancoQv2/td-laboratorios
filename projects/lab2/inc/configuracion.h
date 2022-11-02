#ifndef CPU
#error CPU shall be defined
#endif
#if (lpc4337 == CPU)
#include "chip.h"
#elif (mk60fx512vlq15 == CPU)
#else
#endif


void ConfigurarInterrupcion(void);

void ConfigurarPuertosLaboratorio(void);
