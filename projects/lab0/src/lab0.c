#include "stdint.h"
#include "configuracion.h"
#include "led.h"
#include "switch.h"

static uint8_t actualizar = 0;


void SysTick_Handler(void) {
   static int contador = 0;

   contador++;
   if (contador%500 == 0) {
      Led_Toggle(RED_LED);
   }
   if(contador %1000 == 0) {
       contador = 0;
       actualizar = 1;
   }
}


int main(void)
{
   uint8_t actual = 0;
   uint8_t anterior = 0;

   ConfigurarPuertosLaboratorio();
   ConfigurarInterrupcion();
   
   while (1) {
       actual = Read_Switches();
       if (actual != anterior) {
           switch(actual) {
               case TEC1:
                   Led_On(GREEN_LED);
                   break;
               case TEC2:
                   Led_Off(GREEN_LED);
                   break;
               case TEC3:
                   Led_Toggle(YELLOW_LED);
                   break;
               case TEC4:
                   break;
               default:
                   break;
           }
           anterior = actual;
       }
   }
}


