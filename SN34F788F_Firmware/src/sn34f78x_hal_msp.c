/* USER INC & DEF BEGIN */

/* USER INC & DEF END */

#include "init.h"

/* USER CODE BEGIN #0 */

/* USER CODE END #0 */

/**
 * The HAL_MspInit will be called in HAL_Init.
 */
void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit #0 */

    /* USER CODE END MspInit #0 */
}

/**
 * The HAL_MspDeInit will be called in HAL_DeInit.
 */
void HAL_MspDeInit(void)
{
    /* USER CODE BEGIN MspDeInit #0 */

    /* USER CODE END MspDeInit #0 */
}

void HAL_SPI_MspInit(SPI_Handle_T *hspi)
{
    /* USER CODE BEGIN SPI_MspInit #0 */

    /* USER CODE END SPI_MspInit #0 */

    if (hspi->instance == SN_SPI0)
    {
        HAL_GPIO_SetAFIO(SN_GPIO0, GPIO_PIN_0, GPIO_P00_SPI_SCK0);
        HAL_GPIO_SetAFIO(SN_GPIO0, GPIO_PIN_1, GPIO_P01_SPI_SEL0);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_1, GPIO_P11_SPI_MI0);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_2, GPIO_P12_SPI_MO0);

        if (__HAL_RCC_SSP0_IS_CLK_DISABLE())
        {
            __HAL_RCC_SSP0_CLK_ENABLE();
            __HAL_RCC_SSP0_RESET();
        }
    }

    /* USER CODE BEGIN SPI_MspInit #1 */

    /* USER CODE END SPI_MspInit #1 */
}

void HAL_SPI_MspDeInit(SPI_Handle_T *hspi)
{
    /* USER CODE BEGIN SPI_MspDeInit #0 */

    /* USER CODE END SPI_MspDeInit #0 */

    if (hspi->instance == SN_SPI0)
    {
        if (__HAL_RCC_SSP0_IS_CLK_ENABLE())
        {
            __HAL_RCC_SSP0_CLK_DISABLE();
        }

        HAL_GPIO_SetAFIO(SN_GPIO0, GPIO_PIN_0, GPIO_P00_GPIO);
        HAL_GPIO_SetAFIO(SN_GPIO0, GPIO_PIN_1, GPIO_P01_GPIO);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_1, GPIO_P11_GPIO);
        HAL_GPIO_SetAFIO(SN_GPIO1, GPIO_PIN_2, GPIO_P12_GPIO);
    }

    /* USER CODE BEGIN SPI_MspDeInit #1 */

    /* USER CODE END SPI_MspDeInit #1 */
}
