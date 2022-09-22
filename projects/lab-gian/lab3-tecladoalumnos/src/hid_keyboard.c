#include "board.h"
#include <stdint.h>
#include <string.h>
#include "usbd_rom_api.h"
#include "hid_keyboard.h"
#include "led.h"

//Estructura para guardar los datos del keyboard
typedef struct {
	USBD_HANDLE_T hUsb;	/*!< Manejador de stack USB*/
	uint8_t report[KEYBOARD_REPORT_SIZE];	/*!< Ultimo reporte*/
	uint8_t tx_busy;	/*!< Bandera indicadora de reporte pendiente en cola*/
} Keyboard_Ctrl_T;

//Intancia de la estructura
static Keyboard_Ctrl_T g_keyBoard;

extern const uint8_t Keyboard_ReportDescriptor[];
extern const uint16_t Keyboard_ReportDescSize;

static uint8_t presionadas = 0;
static uint8_t tecla = 0;
void Buttons_Init(void){
	//TEC1
	Chip_SCU_PinMuxSet(0x1, 0, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0));	//P1_0 to GPIO0[4]
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 0, 4);	//entrada
	Chip_SCU_GPIOIntPinSel(0, 0, 4);	//GPIO0[4] to INT0
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));	//INT0
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0));	//INT0
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(0));	//INT0
	NVIC_ClearPendingIRQ(PIN_INT0_IRQn);

	//TEC2
	Chip_SCU_PinMuxSet(0x1, 1, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0));	//P1_1 to GPIO0[8]
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 0, 8);	//entrada
	Chip_SCU_GPIOIntPinSel(1, 0, 8);	//GPIO0[8] to INT1
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));	//INT1
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(1));	//INT1
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(1));	//INT1
	NVIC_ClearPendingIRQ(PIN_INT1_IRQn);

	//TEC3
	Chip_SCU_PinMuxSet(0x1, 2, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0));	//P1_2 to GPIO0[9]
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 0, 9);	//entrada
	Chip_SCU_GPIOIntPinSel(2, 0, 9);	//GPIO0[9] to INT2
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));	//INT2
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(2));	//INT2
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(2));	//INT2
	NVIC_ClearPendingIRQ(PIN_INT2_IRQn);

	//TEC4
	Chip_SCU_PinMuxSet(0x1, 6, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0));	//P1_6 to GPIO1[9]
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, 1, 9);	//entrada
	Chip_SCU_GPIOIntPinSel(3, 1, 9);	//GPIO1[9] to INT3
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(3));	//INT3
	Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(3));	//INT3
	Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(3));	//INT3
	NVIC_ClearPendingIRQ(PIN_INT3_IRQn);

	NVIC_EnableIRQ(PIN_INT0_IRQn);
	NVIC_EnableIRQ(PIN_INT1_IRQn);
	NVIC_EnableIRQ(PIN_INT2_IRQn);
	NVIC_EnableIRQ(PIN_INT3_IRQn);
}

//TEC1
void GPIO0_IRQHandler (void)
{
	/*Clear interrupt*/
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0));
    tecla = presionadas && 0x1; 
	if (tecla == 0)
	{   //1111 1110
		presionadas &= 0xFE;
		presionadas |= 0x1;
	}
	else
	{
		presionadas &= 0xFE;
	}
	//presionadas bit 0
}

//TEC2
void GPIO1_IRQHandler (void)
{
	/*Clear interrupt*/
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(1));
	    tecla = presionadas && 0x2; 
	if (tecla == 0)
	{   //1111 1101
		presionadas &= 0xFD;
		presionadas |= 0x2;
	}
	else
	{
		presionadas &= 0xFD;
	}
	//presionadas bit 1
}

//TEC3
void GPIO2_IRQHandler(void){
	/*Clear interrupt*/
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(2));
	    tecla = presionadas && 0x4;//0100  
	if (tecla == 0)
	{   //1111 1011
		presionadas &= 0xFB;
		presionadas |= 0x4;
	}
	else
	{
		presionadas &= 0xFB;
	}	
	//presionadas bit 2
}

//TEC4
void GPIO3_IRQHandler(void){
	/*Clear interrupt*/
	Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(3));
	    tecla = presionadas && 0x8;//1000  
	if (tecla == 0)
	{   //1111 0111
		presionadas &= 0xF7;
		presionadas |= 0x8;
	}
	else
	{
		presionadas &= 0xF7;
	}	
	//presionadas bit 3
}

//Routine to update keyboard state
static void Keyboard_UpdateReport(void)
{
	HID_KEYBOARD_CLEAR_REPORT(&g_keyBoard.report[0]);
	if((presionadas && 0x1)==0x1){
		HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report,0x14)
		presionadas &= 0xFE; 
	}
	if((presionadas && 0x2)==0x2){
		HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report,0x1A)
		presionadas &= 0xFD; 
	}
	if((presionadas && 0x4)==0x4){
		HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report,0x08)
		presionadas &= 0xFB; 
	}
	if((presionadas && 0x8)==0x8){
		HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report,0x15)
		presionadas &= 0xF7; 
	}
	// HID_KEYBOARD_REPORT_SET_KEY_PRESS(g_keyBoard.report, ??? )

}

//HID Get Report Request Callback. Called automatically on HID Get Report Request
static ErrorCode_t Keyboard_GetReport(USBD_HANDLE_T hHid,
									  USB_SETUP_PACKET *pSetup,
									  uint8_t * *pBuffer,
									  uint16_t *plength)
{
	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_INPUT:
		Keyboard_UpdateReport();
		memcpy(*pBuffer, &g_keyBoard.report[0], KEYBOARD_REPORT_SIZE);
		*plength = KEYBOARD_REPORT_SIZE;
		break;

	case HID_REPORT_OUTPUT:				/* Not Supported */
	case HID_REPORT_FEATURE:			/* Not Supported */
		return ERR_USBD_STALL;
	}
	return LPC_OK;
}

//HID Set Report Request Callback. Called automatically on HID Set Report Request
static ErrorCode_t Keyboard_SetReport(USBD_HANDLE_T hHid, USB_SETUP_PACKET *pSetup, uint8_t * *pBuffer, uint16_t length)
{
	/* we will reuse standard EP0Buf */
	if (length == 0) {
		return LPC_OK;
	}
	/* ReportID = SetupPacket.wValue.WB.L; */
	switch (pSetup->wValue.WB.H) {
	case HID_REPORT_OUTPUT:
		/*  If the USB host tells us to turn on the NUM LOCK LED,
		 *  then turn on LED#2.
		 */
		if (**pBuffer & 0x01) {
			Board_LED_Set(GREEN_LED, 1);
		}
		else {
			Board_LED_Set(GREEN_LED, 0);
		}
		break;

	case HID_REPORT_INPUT:				/* Not Supported */
	case HID_REPORT_FEATURE:			/* Not Supported */
		return ERR_USBD_STALL;
	}
	return LPC_OK;
}

//HID interrupt IN endpoint handler
static ErrorCode_t Keyboard_EpIN_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	switch (event) {
	case USB_EVT_IN:
		g_keyBoard.tx_busy = 0;
		break;
	}
	return LPC_OK;
}

//HID keyboard init routine
ErrorCode_t Keyboard_init(USBD_HANDLE_T hUsb,
						  USB_INTERFACE_DESCRIPTOR *pIntfDesc,
						  uint32_t *mem_base,
						  uint32_t *mem_size)
{
	USBD_HID_INIT_PARAM_T hid_param;
	USB_HID_REPORT_T reports_data[1];
	ErrorCode_t ret = LPC_OK;

	/* Do a quick check of if the interface descriptor passed is the right one. */
	if ((pIntfDesc == 0) || (pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE)) {
		return ERR_FAILED;
	}

	/* init joystick control */
	Buttons_Init();
	Chip_GPIO_Init(LPC_GPIO_PORT);

	/* Init HID params */
	memset((void *) &hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
	hid_param.max_reports = 1;
	hid_param.mem_base = *mem_base;
	hid_param.mem_size = *mem_size;
	hid_param.intf_desc = (uint8_t *) pIntfDesc;
	/* user defined functions */
	hid_param.HID_GetReport = Keyboard_GetReport;
	hid_param.HID_SetReport = Keyboard_SetReport;
	hid_param.HID_EpIn_Hdlr  = Keyboard_EpIN_Hdlr;
	/* Init reports_data */
	reports_data[0].len = Keyboard_ReportDescSize;
	reports_data[0].idle_time = 0;
	reports_data[0].desc = (uint8_t *) &Keyboard_ReportDescriptor[0];
	hid_param.report_data  = reports_data;

	ret = USBD_API->hid->init(hUsb, &hid_param);
	/* update memory variables */
	*mem_base = hid_param.mem_base;
	*mem_size = hid_param.mem_size;
	/* store stack handle for later use. */
	g_keyBoard.hUsb = hUsb;
	g_keyBoard.tx_busy = 0;

	return ret;
}

//Keyboard tasks
void Keyboard_Tasks(void)
{
	/* REVISAR QUE EL DISPOSITIVO ESTÉ CONFIGURADO ANTES DE ENVIAR DARTOS. */
	if ( USB_IsConfigured(g_keyBoard.hUsb)) {
		/* enviar el reporte */
		if (g_keyBoard.tx_busy == 0) {
			g_keyBoard.tx_busy = 1;
			Keyboard_UpdateReport();
			USBD_API->hw->WriteEP(g_keyBoard.hUsb, HID_EP_IN, &g_keyBoard.report[0], KEYBOARD_REPORT_SIZE);
		}
	}
	else {
		/* resetear bandera si nos desconectamos. */
		g_keyBoard.tx_busy = 0;
	}

}
