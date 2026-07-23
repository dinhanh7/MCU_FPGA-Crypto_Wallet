/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_i2c.h"

#if defined(USE_FULL_LL_DRIVER)

/* Exported functions --------------------------------------------------------*/
LL_Status_T LL_I2C_Init(SN_I2C0_Type *i2c, LL_I2C_Init_T *i2c_init)
{
    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_I2C_INSTANCE(i2c), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2C_CLOCK_SPEED(i2c_init->clock_speed), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2C_ADDRESS(i2c_init->own_address), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2C_ADDRESSING_MODE(i2c_init->addressing_mode), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_I2C_GC_EN(i2c_init->gc_en), LL_ERROR);

    LL_I2C_SetAddress(i2c, i2c_init->own_address);
    if (i2c_init->addressing_mode == LL_I2C_ADDRESSINGMODE_7BIT)
    {
        LL_I2C_Disable_10BitAddressing(i2c);
    }
    else
    {
        LL_I2C_Enable_10BitAddressing(i2c);
    }
    if (i2c_init->gc_en == LL_ENABLE)
    {
        LL_I2C_Enable_GC(i2c);
    }
    else
    {
        LL_I2C_Disable_GC(i2c);
    }

    uint32_t clock_list[] = {100000UL, 400000UL, 1000000UL, 3000000UL};
    uint32_t apb1_clk     = LL_RCC_GetAPB1CLKFreq();
    uint32_t divisor      = apb1_clk / clock_list[i2c_init->clock_speed];
    uint32_t count        = (divisor - 6) / 2; // SCL = PCLK / ( 2 * COUNT + GSR + 4)

    if (i2c_init->clock_speed < LL_I2C_CLOCKSPEED_HIGH_SPEED) // FS mode
    {
        /* COUNT >= 4 + GSR(2) + TSR(1) */
        if (count < 7 || count > 1048575)
        {
            return LL_ERROR;
        }
        LL_I2C_SetCounter(i2c, count);
    }
    else // HS mode
    {
        /* COUNT >= 4 + GSR(2) + TSR(1) */
        if (count < 7 || count > 255)
        {
            return LL_ERROR;
        }
        LL_I2C_SetCounterH(i2c, count);

        /* Set COUNT with 100K */
        divisor = apb1_clk / 100000UL;
        count   = (divisor - 6) / 2;
        LL_I2C_SetCounter(i2c, count);
    }

    LL_I2C_SetTSR(i2c, 1);
    LL_I2C_SetGSR(i2c, 2);
    return LL_OK;
}

LL_Status_T LL_I2C_DeInit(SN_I2C0_Type *i2c)
{
    RET_FLAG_FALSE(IS_LL_I2C_INSTANCE(i2c), LL_ERROR);
    LL_I2C_Reset(i2c);

    return LL_OK;
}

void LL_I2C_StructInit(LL_I2C_Init_T *i2c_init)
{
    if (i2c_init != NULL)
    {
        i2c_init->clock_speed     = LL_I2C_CLOCKSPEED_STANDARD;
        i2c_init->own_address     = 0U;
        i2c_init->addressing_mode = LL_I2C_ADDRESSINGMODE_7BIT;
        i2c_init->gc_en           = LL_DISABLE;
    }
}

#endif /* USE_FULL_LL_DRIVER */
