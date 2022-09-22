#include "stdlib.h"
#include "stdint.h"
#include "configuracion.h"
#include "led.h"
#include <string.h>

#define USB_UART LPC_USART2
#define OVERRUN "ERROR OVERRUN\r\n"
#define PARITY "ERROR PARITY\r\n"
#define FRAMING "ERROR FRAMING\r\n"
#define BREAK "ERROR BREAK\r\n"
#define UNKNOWN "ERROR UNKNOWN\r\n"

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

void ConfigurarUART(LPC_USART_T *pUART){
    Chip_UART_Init(pUART);

    /*  COMPLETAR LA IMPLEMENTACION DE ESTA FUNCION  */
}

uint8_t UARTDisponible(LPC_USART_T *pUART){
     /*  COMPLETAR LA IMPLEMENTACION DE ESTA FUNCION  */
}

uint8_t UARTLeerByte(LPC_USART_T *pUART, uint8_t* data, uint8_t* error){
     /*  COMPLETAR LA IMPLEMENTACION DE ESTA FUNCION  */
}


int main(void)
{

   ConfigurarPuertosLaboratorio();
   ConfigurarInterrupcion();
   ConfigurarUART(USB_UART);
   
   while (1) {
       if(actualizar){
           actualizar = 0;
           Led_On(GREEN_LED);

           /*  ESCRIBIR IMPLEMENTACION UARTDISPONIBLE Y ENVIAR LA CUENTA ACTUAL  */
           
           Led_Off(GREEN_LED);
       }

       /*  DETERMINAR SI SE RECIBIO UN CARACTER Y ACTUAR ACORDE  */
       
   }
}


