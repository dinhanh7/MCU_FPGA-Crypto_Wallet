/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_ll_gpio.h"

#if defined(USE_FULL_LL_DRIVER)
/**
 * @brief  Initialize GPIO registers according to the specified parameters in gpio_init.
 * @param  gpio GPIO Port
 * @param  gpio_init pointer to a @ref LL_GPIO_Init_T structure
 *         that contains the configuration information for the specified GPIO peripheral.
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     GPIO registers are initialized according to gpio_init content
 *           - LL_ERROR:  Not applicable
 */
LL_Status_T LL_GPIO_Init(SN_GPIO0_Type *gpio, LL_GPIO_Init_T *gpio_init)
{
    /* Check the instance */
    RET_FLAG_FALSE(IS_LL_GPIO_INSTANCE(gpio), LL_ERROR);

    /* Check the parameters */
    RET_FLAG_FALSE(IS_LL_GPIO_PIN(gpio, gpio_init->pin_mask), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_GPIO_MODE(gpio_init->mode), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_GPIO_PULL(gpio_init->pull), LL_ERROR);
    RET_FLAG_FALSE(IS_LL_GPIO_IE(gpio_init->ie), LL_ERROR);
    if (gpio_init->ie == LL_ENABLE)
    {
        RET_FLAG_FALSE(IS_LL_GPIO_ITRIG(gpio_init->itrig), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_GPIO_IBE(gpio_init->ibe), LL_ERROR);
        RET_FLAG_FALSE(IS_LL_GPIO_IRF(gpio_init->irf), LL_ERROR);
    }

    LL_GPIO_SetPinMode(gpio, gpio_init->pin_mask, gpio_init->mode);
    LL_GPIO_SetPinPull(gpio, gpio_init->pin_mask, gpio_init->pull);
    if (gpio_init->ie == LL_ENABLE)
    {
        LL_GPIO_EnablePinIT(gpio, gpio_init->pin_mask);
    }
    else
    {
        LL_GPIO_DisablePinIT(gpio, gpio_init->pin_mask);
    }
    LL_GPIO_SetPinITRIG(gpio, gpio_init->pin_mask, gpio_init->itrig);
    LL_GPIO_SetPinIBE(gpio, gpio_init->pin_mask, gpio_init->ibe);
    LL_GPIO_SetPinIRF(gpio, gpio_init->pin_mask, gpio_init->irf);

    return LL_OK;
}

/**
 * @brief  De-initialize GPIO registers (Registers restored to their default values).
 * @param  gpio GPIO Port
 * @retval An ErrorStatus enumeration value:
 *           - LL_OK:     GPIO registers are de-initialized
 *           - LL_ERROR:  Wrong GPIO Port
 */
LL_Status_T LL_GPIO_DeInit(SN_GPIO0_Type *gpio)
{
    /* Check the instance */
    RET_FLAG_FALSE(IS_LL_GPIO_INSTANCE(gpio), LL_ERROR);

    uint32_t pin_mask = 0;
    if (gpio == SN_GPIO0)
    {
        pin_mask = GPIO0_PIN_MASK;
    }
    else if (gpio == SN_GPIO1)
    {
        pin_mask = GPIO1_PIN_MASK;
    }
    else if (gpio == SN_GPIO2)
    {
        pin_mask = GPIO2_PIN_MASK;
    }
    else if (gpio == SN_GPIO3)
    {
        pin_mask = GPIO3_PIN_MASK;
    }
    else
    {
        return LL_ERROR;
    }

    LL_GPIO_SetPinMode(gpio, pin_mask, LL_GPIO_MODE_INPUT);
    LL_GPIO_SetPinPull(gpio, pin_mask, LL_GPIO_PULL_DOWN);
    LL_GPIO_DisablePinIT(gpio, pin_mask);
    LL_GPIO_SetPinITRIG(gpio, pin_mask, LL_GPIO_TRIGGER_EDGE);
    LL_GPIO_SetPinIBE(gpio, pin_mask, LL_GPIO_TRIGGER_SINGLE_EDGE);
    LL_GPIO_SetPinIRF(gpio, pin_mask, LL_GPIO_TRIGGER_RISING_OR_HIGH);

    return LL_OK;
}

/**
 * @brief  Set each @ref LL_GPIO_Init_T field to default value.
 * @param  gpio_init pointer to a @ref LL_GPIO_Init_T structure  whose fields will be set to default values.
 * @retval None
 */
void LL_GPIO_StructInit(LL_GPIO_Init_T *gpio_init)
{
    if (gpio_init != NULL)
    {
        gpio_init->pin_mask = LL_GPIO_PIN_0;
        gpio_init->mode     = LL_GPIO_MODE_INPUT;
        gpio_init->pull     = LL_GPIO_PULL_DISABLE;
        gpio_init->ie       = LL_DISABLE;
        gpio_init->itrig    = LL_GPIO_TRIGGER_EDGE;
        gpio_init->ibe      = LL_GPIO_TRIGGER_SINGLE_EDGE;
        gpio_init->irf      = LL_GPIO_TRIGGER_RISING_OR_HIGH;
    }
}
#endif /* USE_FULL_LL_DRIVER */
