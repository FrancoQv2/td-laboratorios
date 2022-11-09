#include "stdlib.h"
#include "stdint.h"
#include "configuracion.h"
#include "led.h"
#include <string.h>

#define USB_UART LPC_USART2
#define MODEM_UART LPC_USART3

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

void ConfigurarUART(LPC_USART_T *pUART) {
    Chip_UART_Init(pUART);

    pUART->LCR |= 1 << 7;

    // Configuro el divisor del clock (204000000 / (16 * 38400))
    pUART->DLL = 0x4C;
    pUART->DLM = 0x01;

    // Deshabilito DLAB
    pUART->LCR &= 0 << 7;

    // Configuro el LCR
    pUART->LCR = 0b000010011;

    // Habilito Tx
    pUART->TER2 = (1 << 0);
}

uint8_t UARTDisponible(LPC_USART_T *pUART) {
    return ((pUART->LSR & UART_LSR_THRE) != 0);
}

uint8_t UARTLeerByte(LPC_USART_T *pUART, uint8_t *data, uint8_t *error) {
    uint8_t resultado = 0;
    uint8_t status = pUART->LSR;
    if (status & (1 << 0)) {
        resultado = 1;
        if (status & ((1 << 1) | (1 << 2) | (1 << 3) | (1 << 4))) {
            /*ERROR*/
            *error = status & (0x1E);
        } else {
            *data = (pUART->RBR & (0xFF));
        }
    }
    return resultado;
}

void UARTEscribirByte(LPC_USART_T *pUART, uint8_t data) {
    while (!UARTDisponible(pUART)) {
    };
    pUART->THR = data;
}

void UARTEscribirString(LPC_USART_T *pUART, char string[]) {
    for (int i = 0; i < strlen(string); i++) {
        UARTEscribirByte(pUART, (uint8_t)string[i]);
    }
}

uint8_t EnviarComandoAT(LPC_USART_T *pUART_MODEM, char AT[]) {
    uint8_t byte0 = 0;
    uint8_t byte1 = 0;
    uint8_t byteError = 0;
    uint8_t resultado = 0;
    uint8_t aux = 0;

    // LIMPIAR CUALQUIER BASURA QUE PUDIERA QUEDAR
    while (UARTLeerByte(pUART_MODEM, &byte0, &byteError)) {
    };

    // ESCRIBIR COMANDO
    UARTEscribirString(pUART_MODEM, AT);

    // INTERPRETAR RESPUESTA (PISTA: BYTE 0 Y 1)
    while (byte1 != 'O' || byte0 != 'K') {
        byte1 = byte0;
        UARTLeerByte(pUART_MODEM, &byte0, &byteError);
    }

    if (byte1 == 'K' && byte0 == 'O')
        resultado = 1;

    // VACIAR EL RESTO DEL BUFFER
    while (UARTLeerByte(pUART_MODEM, &byte0, &byteError))
        ;

    return resultado;
}

void ConfigurarMODEM(LPC_USART_T *pUART_USB, LPC_USART_T *pUART_MODEM) {
    uint8_t aux;
    uint8_t err;
    // Para configurar eléctricamente al puerto que querés
    Chip_SCU_PinMux(4, 7, SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS, SCU_MODE_FUNC0);
    // Para que pongás en bajo por las dudas aquel puerto
    Chip_GPIO_ClearValue(LPC_GPIO_PORT, 3, 1 << 12);
    // Para que configurés como salida el puerto
    Chip_GPIO_SetDir(LPC_GPIO_PORT, 3, 1 << 12, 1);
    // Ponés en alto ese puerto
    Chip_GPIO_SetValue(LPC_GPIO_PORT, 3, 1 << 12);
    while (UARTLeerByte(pUART_MODEM, &aux, &err)) {
    }
    if (1) {
        EnviarComandoAT(pUART_MODEM, "AT\r\n");
        EnviarComandoAT(pUART_MODEM, "AT+NAME=tdBT\r\n");
        EnviarComandoAT(pUART_MODEM, "AT+PSWD=1234\r\n");
        EnviarComandoAT(pUART_MODEM, "AT+UART=38400,1,0\r\n");
        EnviarComandoAT(pUART_MODEM, "AT+ROLE=0\r\n");
        EnviarComandoAT(pUART_MODEM, "AT+INIT\r\n");
        EnviarComandoAT(pUART_MODEM, "AT+INQ\r\n");
    }
}

int main(void) {
    uint8_t readData = 0;
    uint8_t readError = 0;
    uint8_t buffer[20] = "";
    uint8_t *ptrBuffer = buffer;
    uint8_t contador = 0;

    ConfigurarPuertosLaboratorio();
    ConfigurarInterrupcion();
    ConfigurarUART(USB_UART);
    ConfigurarUART(MODEM_UART);
    ConfigurarMODEM(USB_UART, MODEM_UART);

    while (1) {
        if (actualizar) {
            actualizar = 0;
            Led_On(GREEN_LED);
            while (*ptrBuffer != '\0') {
                UARTEscribirByte(USB_UART, *ptrBuffer);
                ptrBuffer++;
            }
            ptrBuffer = buffer;
            buffer[0] = '\0';
            Led_Off(GREEN_LED);
        }
        if (UARTLeerByte(MODEM_UART, &readData, &readError)) {
            if (readError) {
                Led_Toggle(YELLOW_LED);
                if (readError & (1 << 1)) {
                    strcpy(buffer, OVERRUN);
                } else if (readError & (1 << 2)) {
                    strcpy(buffer, PARITY);
                } else if (readError & (1 << 3)) {
                    strcpy(buffer, FRAMING);
                } else if (readError & (1 << 4)) {
                    strcpy(buffer, BREAK);
                } else {
                    strcpy(buffer, UNKNOWN);
                }
                readError = 0;
            } else {
                Led_Toggle(RGB_B_LED);
                if (readData == 'q') {
                    if (contador == 99) {
                        contador = 0;
                    } else {
                        contador++;
                    }
                } else if (readData == 'w') {
                    if (contador == 0) {
                        contador = 99;
                    } else {
                        contador--;
                    }
                } else if (readData == 'e') {
                    contador = 0;
                }
                buffer[0] = (char)((contador / 10) + 48);
                buffer[1] = (char)((contador % 10) + 48);
                buffer[2] = '\r';
                buffer[3] = '\n';
                buffer[4] = '\0';
            }
        }
    }
}