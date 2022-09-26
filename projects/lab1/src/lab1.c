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
    if (contador % 500 == 0) {
        Led_Toggle(RED_LED);
    }
    if (contador % 1000 == 0) {
        contador = 0;
        actualizar = 1;
    }
}

/*  COMPLETAR LA IMPLEMENTACION DE ESTA FUNCION  */
void ConfigurarUART(LPC_USART_T *pUART) {
    Chip_UART_Init(pUART);

    pUART->LCR = pUART->LCR || 0X80;

    pUART->DLL = pUART->DLL || 0X30;
    pUART->DLM = pUART->DLM || 0X5;
    pUART->FDR = pUART->FDR || 0X8;

    pUART->LCR = pUART->LCR || 0x1B;

    pUART->TER2 = pUART->TER2 || 0x1;
}

/*  COMPLETAR LA IMPLEMENTACION DE ESTA FUNCION  */
uint8_t UARTDisponible(LPC_USART_T *pUART) {
    return ((pUART->LSR & 0x80) == 0x80) ? 1 : 0;
}

/*  COMPLETAR LA IMPLEMENTACION DE ESTA FUNCION  */
uint8_t UARTLeerByte(LPC_USART_T *pUART, uint8_t *data, uint8_t *error) {
    uint8_t resultado;
    uint8_t estado = pUART->LSR && 0x1;

    if (estado == 1) {
        data = pUART->RBR && 0xFF;
        resultado = 0x1;
    } else {
        resultado = 0;
    }
    return resultado;
}

int main(void) {
    uint8_t readData = 0;
    uint8_t readError = 0;
    uint8_t contador = 0;

    ConfigurarPuertosLaboratorio();
    ConfigurarInterrupcion();
    ConfigurarUART(USB_UART);

    while (1) {
        if (actualizar) {
            actualizar = 0;
            Led_On(GREEN_LED);

            /*  ESCRIBIR IMPLEMENTACION UARTDISPONIBLE Y ENVIAR LA CUENTA ACTUAL  */

            while (!UARTDisponible(USB_UART))
                ;
            USB_UART->THR = USB_UART->THR && 0xFFFFFF00;
            USB_UART->THR = USB_UART->THR || contador;

            Led_Off(GREEN_LED);

            /*  DETERMINAR SI SE RECIBIO UN CARACTER Y ACTUAR ACORDE  */

            if (UARTLeerByte(USB_UART, &readData, &readError)) {
                if (readError) {
                    /* code */
                } else {
                    if (readData == 'q') {
                        (contador = 100) ? contador = 0 : contador++;
                        // contador++;
                        // if (contador == 100) {
                        //     contador = 0;
                        // }
                    }
                    if (readData == 'w') {
                        (contador = -1) ? contador = 99 : contador--;
                        // contador--;
                        // if (contador == -1) {
                        //     contador = 99;
                        // }
                    }
                    if (readData == 'e') {
                        contador = 0;
                    }
                }
            }
        }
    }
}
