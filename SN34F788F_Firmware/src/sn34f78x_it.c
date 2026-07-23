/* USER INC & DEF BEGIN */

/* USER INC & DEF END */

#include "init.h"

/* USER CODE BEGIN #0 */

/* USER CODE END #0 */

__irq void SysTick_Handler(void)
{
    /* USER CODE BEGIN SYS_IRQ #0 */

    /* USER CODE END SYS_IRQ #0 */

    HAL_IncTick();

    /* USER CODE BEGIN SYS_IRQ #1 */

    /* USER CODE END SYS_IRQ #1 */
}

__irq void SCU_IRQHandler(void)
{
    /* USER CODE BEGIN SCU_IRQ #0 */

    /* USER CODE END SCU_IRQ #0 */

    HAL_SCU_IRQHandler();

    /* USER CODE BEGIN SCU_IRQ #1 */

    /* USER CODE END SCU_IRQ #1 */
}
