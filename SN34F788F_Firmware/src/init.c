#include "init.h"

/* USER CODE BEGIN #0 */

/* USER CODE END #0 */
SPI_Handle_T SPI0_Handle;

uint32_t sn34f7_SPI0_Init(void)
{
    SPI0_Handle.instance          = SN_SPI0;
    SPI0_Handle.init.frame_format = SPI_FFMT_SPI;
    SPI0_Handle.init.mode         = SPI_MODE_MASTER;
    SPI0_Handle.init.data_size    = 8;
    SPI0_Handle.init.fs_polarity  = SPI_FS_POLARITY_LOW;
    SPI0_Handle.init.clk_polarity = SPI_CLK_POLARITY_LOW;
    SPI0_Handle.init.clk_phase    = SPI_CLK_PHASE_1EDGE;
    SPI0_Handle.init.first_bit    = SPI_FIRST_BIT_MSB;
    SPI0_Handle.init.sclk_div     = 1;
    SPI0_Handle.init.tx_default   = 0;

    if (HAL_OK != HAL_SPI_Init(&SPI0_Handle))
        return HAL_ERROR;

    return HAL_OK;
}

GPIO_Init_T GPIO3_PIN3_INIT;
GPIO_Init_T GPIO3_PIN4_INIT;
GPIO_Init_T GPIO0_PIN15_INIT;
GPIO_Init_T GPIO1_PIN5_INIT;

uint32_t sn34f7_GPIO_Init(void)
{
    GPIO3_PIN3_INIT.pin   = GPIO_PIN_3;
    GPIO3_PIN3_INIT.mode  = GPIO_MODE_OUTPUT;
    GPIO3_PIN3_INIT.drive = GPIO_DRV_17mA;

    if (HAL_OK != HAL_GPIO_Init(SN_GPIO3, &GPIO3_PIN3_INIT))
        return HAL_ERROR;

    if (HAL_GPIO_WritePin(SN_GPIO3, GPIO_PIN_3, GPIO_PIN_LOW) != HAL_OK)
        return HAL_ERROR;

    GPIO3_PIN4_INIT.pin   = GPIO_PIN_4;
    GPIO3_PIN4_INIT.mode  = GPIO_MODE_OUTPUT;
    GPIO3_PIN4_INIT.drive = GPIO_DRV_17mA;

    if (HAL_OK != HAL_GPIO_Init(SN_GPIO3, &GPIO3_PIN4_INIT))
        return HAL_ERROR;

    if (HAL_GPIO_WritePin(SN_GPIO3, GPIO_PIN_4, GPIO_PIN_HIGH) != HAL_OK)
        return HAL_ERROR;

    GPIO0_PIN15_INIT.pin  = GPIO_PIN_15;
    GPIO0_PIN15_INIT.mode = GPIO_MODE_INPUT;
    GPIO0_PIN15_INIT.pull = GPIO_PULL_UP;

    if (HAL_OK != HAL_GPIO_Init(SN_GPIO0, &GPIO0_PIN15_INIT))
        return HAL_ERROR;

    GPIO1_PIN5_INIT.pin  = GPIO_PIN_5;
    GPIO1_PIN5_INIT.mode = GPIO_MODE_INPUT;
    GPIO1_PIN5_INIT.pull = GPIO_PULL_UP;

    if (HAL_OK != HAL_GPIO_Init(SN_GPIO1, &GPIO1_PIN5_INIT))
        return HAL_ERROR;

    return HAL_OK;
}

uint32_t init(void)
{
    /* USER CODE BEGIN Init #0 */

    /* USER CODE END Init #0 */

    sn34f7_GPIO_Init();

    RET_FLAG_FALSE(HAL_FLAG_EQU(sn34f7_SPI0_Init(), HAL_OK), HAL_ERROR);

    /* USER CODE BEGIN Init #1 */

    /* USER CODE END Init #1 */

    return HAL_OK;
}

uint32_t uninit(void)
{
    /* USER CODE BEGIN Uninit #0 */

    /* USER CODE END Uninit #0 */

    RET_FLAG_FALSE(HAL_FLAG_EQU(HAL_SPI_DeInit(&SPI0_Handle), HAL_OK), HAL_ERROR);

    HAL_DeInit();

    /* USER CODE BEGIN Uninit #1 */

    /* USER CODE END Uninit #1 */

    return HAL_OK;
}
