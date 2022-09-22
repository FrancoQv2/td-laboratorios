#include "configuracion.h"


void ConfigurarInterrupcion(void) {
   __asm__ volatile ("cpsid i");

   /* Activate SysTick */
   SystemCoreClockUpdate();
   SysTick_Config(SystemCoreClock/1000);

   /* Update priority set by SysTick_Config */
   NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);

   __asm__ volatile ("cpsie i");
}

void ConfigurarPuertosLaboratorio(void) {
   #define SCU_MODE_SAL (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS)
   #define SCU_MODE_ENT (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN)

   /* Puerto Led RGB */
   /* Chip_SCU_PinMux(port, pin, options, function); */
   Chip_SCU_PinMux(2, 0, SCU_MODE_SAL, SCU_MODE_FUNC4);
   Chip_SCU_PinMux(2, 1, SCU_MODE_SAL, SCU_MODE_FUNC4);
   Chip_SCU_PinMux(2, 2, SCU_MODE_SAL, SCU_MODE_FUNC4);
   /* Chip_GPIO_ClearValue(LPC_GPIO_PORT, port, mask); */
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 5, (1 << 0) | (1 << 1) | (1 << 2));
   /* Chip_GPIO_SetDir(LPC_GPIO_PORT, port, mask, output); */
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 5, (1 << 0) | (1 << 1) | (1 << 2), 1);

   /* Puerto Leds 1 a 3 */
   Chip_SCU_PinMux(2, 10, SCU_MODE_SAL, SCU_MODE_FUNC0);
   Chip_SCU_PinMux(2, 11, SCU_MODE_SAL, SCU_MODE_FUNC0);
   Chip_SCU_PinMux(2, 12, SCU_MODE_SAL, SCU_MODE_FUNC0);
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 0, (1 << 14));
   Chip_GPIO_ClearValue(LPC_GPIO_PORT, 1, (1 << 11) | (1 << 12));
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0, (1 << 14), true);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1, (1 << 11) | (1 << 12), true);

   /* Puerto Teclas 1 a 4 */
   Chip_SCU_PinMux(1, 0, SCU_MODE_ENT, SCU_MODE_FUNC0);
   Chip_SCU_PinMux(1, 1, SCU_MODE_ENT, SCU_MODE_FUNC0);
   Chip_SCU_PinMux(1, 2, SCU_MODE_ENT, SCU_MODE_FUNC0);
   Chip_SCU_PinMux(1, 6, SCU_MODE_ENT, SCU_MODE_FUNC0);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 0, (1 << 4) | (1 << 8) | (1 << 9), false);
   Chip_GPIO_SetDir(LPC_GPIO_PORT, 1, (1 << 9), false);
}
