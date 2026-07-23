/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#include "sn34f78x_hal_gpio.h"

/* Private constant --------------------------------------------------------*/
/**
 * @brief define the All support AFIO information
 *
 */
const static AFIO_Info_T AFIO_Info[] = {
    {P0_AFIO0, 0, AFIO_P0_0_DESC},
    {P0_AFIO0, 5, AFIO_P0_1_DESC},
    {P0_AFIO0, 10, AFIO_P0_2_DESC},
    [10] = {P0_AFIO1, 20, AFIO_P0_10_DESC},
    {P0_AFIO1, 25, AFIO_P0_11_DESC},
    {P0_AFIO2, 0, AFIO_P0_12_DESC},
    {P0_AFIO2, 5, AFIO_P0_13_DESC},
    {P0_AFIO2, 10, AFIO_P0_14_DESC},
    {P0_AFIO2, 15, AFIO_P0_15_DESC},
    [20] = {P0_AFIO3, 10, AFIO_P0_20_DESC},

    {P1_AFIO0, 0, AFIO_P1_0_DESC},
    {P1_AFIO0, 5, AFIO_P1_1_DESC},
    {P1_AFIO0, 10, AFIO_P1_2_DESC},
    {P1_AFIO0, 15, AFIO_P1_3_DESC},
    {P1_AFIO0, 20, AFIO_P1_4_DESC},
    {P1_AFIO0, 25, AFIO_P1_5_DESC},
    {P1_AFIO1, 0, AFIO_P1_6_DESC},
    {P1_AFIO1, 5, AFIO_P1_7_DESC},
    {P1_AFIO1, 10, AFIO_P1_8_DESC},
    {P1_AFIO1, 15, AFIO_P1_9_DESC},
    {P1_AFIO1, 20, AFIO_P1_10_DESC},
    {P1_AFIO1, 25, AFIO_P1_11_DESC},
    {P1_AFIO2, 0, AFIO_P1_12_DESC},
    {P1_AFIO2, 5, AFIO_P1_13_DESC},
    {P1_AFIO2, 10, AFIO_P1_14_DESC},
    {P1_AFIO2, 15, AFIO_P1_15_DESC},

    {P2_AFIO0, 0, AFIO_P2_0_DESC},
    {P2_AFIO0, 5, AFIO_P2_1_DESC},
    {P2_AFIO0, 10, AFIO_P2_2_DESC},
    {P2_AFIO0, 15, AFIO_P2_3_DESC},
    {P2_AFIO0, 20, AFIO_P2_4_DESC},
    {P2_AFIO0, 25, AFIO_P2_5_DESC},
    {P2_AFIO1, 0, AFIO_P2_6_DESC},
    {P2_AFIO1, 5, AFIO_P2_7_DESC},
    {P2_AFIO1, 10, AFIO_P2_8_DESC},
    {P2_AFIO1, 15, AFIO_P2_9_DESC},
    {P2_AFIO1, 20, AFIO_P2_10_DESC},
    {P2_AFIO1, 25, AFIO_P2_11_DESC},
    {P2_AFIO2, 0, AFIO_P2_12_DESC},
    {P2_AFIO2, 5, AFIO_P2_13_DESC},
    {P2_AFIO2, 10, AFIO_P2_14_DESC},
    {P2_AFIO2, 15, AFIO_P2_15_DESC},

    {P3_AFIO0, 0, AFIO_P3_0_DESC},
    {P3_AFIO0, 5, AFIO_P3_1_DESC},
    {P3_AFIO0, 10, AFIO_P3_2_DESC},
    {P3_AFIO0, 15, AFIO_P3_3_DESC},
    {P3_AFIO0, 20, AFIO_P3_4_DESC},
    {P3_AFIO0, 25, AFIO_P3_5_DESC},
    {P3_AFIO1, 0, AFIO_P3_6_DESC},
    {P3_AFIO1, 5, AFIO_P3_7_DESC},
    {P3_AFIO1, 10, AFIO_P3_8_DESC},
    {P3_AFIO1, 15, AFIO_P3_9_DESC},
    {P3_AFIO1, 20, AFIO_P3_10_DESC},
    {P3_AFIO1, 25, AFIO_P3_11_DESC},
    {P3_AFIO2, 0, AFIO_P3_12_DESC},
    {P3_AFIO2, 5, AFIO_P3_13_DESC},
};

/* Exported functions --------------------------------------------------------*/
/*
================================================================================
            ##### Initialization/de-initialization functions #####
================================================================================
*/
/**
 * @brief  Initializes the GPIOx peripheral according to the specified parameters in the GPIO_Init.
 * @param  gpio Pointer to a struct SN_GPIO_Type that contains the GPIOx registers information.
 * @param  gpio_init Pointer to a struct GPIO_Init_T that contains mode/cfg/irq... information.
 * @retval HAL_Status_T
 * @note
 * for input pin, the standard initialze flow could be:
 * 0: config gpio pin direction
 * 1: config gpio pin pull function
 * 2: config gpio pin clock mode
 * 3: config gpio pin irq trigger
 * 4: config gpio pin debounce
 * 5: config gpio pin ie
 * 6: config gpio pin imasken
 * for output pin, the standard initialze flow could be:
 * 0: config gpio pin direction
 */
HAL_Status_T HAL_GPIO_Init(SN_GPIO_Type gpio, GPIO_Init_T *gpio_init)
{
    uint32_t position = 0x00u;

    /* Check whether the parameter is valid */
    ASSERT_PARAM(IS_GPIO_ALL_INSTANCE(gpio));
    ASSERT_PARAM(IS_GPIO_PIN(gpio, gpio_init->pin));
    ASSERT_PARAM(IS_GPIO_MODE(gpio_init->mode));
    ASSERT_PARAM(IS_GPIO_PULL(gpio_init->pull));

    /* configure the port pins */
    uint32_t mode         = 0;
    uint32_t update_value = 0;
    while (((gpio_init->pin) >> position) != 0x00u)
    {
        /* get current io position */
        uint32_t pin = (1uL << position);
        if ((gpio_init->pin) & pin)
        {
            /* 0: config gpio pin direction */
            mode         = (gpio_init->mode & 1);
            update_value = mode << position;
            HAL_REG_CBIT(gpio->DIR, pin);
            HAL_REG_SBIT(gpio->DIR, update_value);

            if (IS_GPIO_MODE_INPUT(gpio, pin))
            {
                /* 1: config gpio pin pull function */
                if (IS_GPIO_PULL_INACTIVE(gpio_init->pull))
                {
                    // disable pull function
                    HAL_REG_CBIT(gpio->PULLEN, pin);
                }
                else
                {
                    // setup pull value
                    update_value = gpio_init->pull << position;
                    HAL_REG_CBIT(gpio->PUTYPE, pin);
                    HAL_REG_SBIT(gpio->PUTYPE, update_value);

                    // enable pull function
                    HAL_REG_SBIT(gpio->PULLEN, pin);
                }

                /* 2: config gpio pin driving */
                _gpio_config_pin_str(gpio, position, gpio_init->drive);

                /* 3: config gpio pin irq/ie/imasken for interrupt trigger */
                _gpio_config_irq(gpio, pin, gpio_init->mode);
            }
        }

        position++;
    }
    return HAL_OK;
}

/**
 * @brief  De-initializes the GPIOx peripheral registers to their default reset values.
 * @param  gpio Pointer to a struct SN_GPIO_Type that contains the GPIOx registers information.
 * @param  gpio_pin Specifies the port bit to be de-initializes.
 * @retval HAL_Status_T
 */
HAL_Status_T HAL_GPIO_DeInit(SN_GPIO_Type gpio, uint32_t gpio_pin)
{
    uint32_t position = 0x00u;

    /* Check whether the parameter is valid */
    ASSERT_PARAM(IS_GPIO_ALL_INSTANCE(gpio));
    ASSERT_PARAM(IS_GPIO_PIN(gpio, gpio_pin));

    while ((gpio_pin >> position) != 0x00u)
    {
        /* Get current io position */
        uint32_t pin = (1uL << position);

        if ((gpio_pin)&pin)
        {
            /* restore initial value of clock mode */
            HAL_REG_WRITE(gpio->CLKMODE, GPIO_CLK_NORMAL);

            /* disable bounce */
            HAL_REG_CBIT(gpio->BNEN, pin);

            /* restore initial value of pin mode */
            HAL_REG_CBIT(gpio->DIR, pin);

            if (IS_GPIO_PULL_ENABLE(gpio, gpio_pin))
            {
                /* restore initial value of pull mode */
                HAL_REG_CBIT(gpio->PUTYPE, pin);

                /* disable pull */
                HAL_REG_CBIT(gpio->PULLEN, pin);
            }

            /* disable mask */
            HAL_REG_CBIT(gpio->IMASKEN, pin);

            /* disable IRQ*/
            _gpio_config_irq(gpio, pin, GPIO_MODE_INPUT);
        }
        position++;
    }
    return HAL_OK;
}

/**
 * @brief  Reads the specified input port pin.
 * @param  gpio Pointer to a struct SN_GPIO_Type that contains the GPIOx registers information.
 * @param  gpio_pin Specifies the port bit to be read.
 * @retval The input port pin value.
 */
uint32_t HAL_GPIO_ReadPin(SN_GPIO_Type gpio, uint32_t gpio_pin)
{
    /* Check whether the parameter is valid */
    ASSERT_PARAM(IS_GPIO_ALL_INSTANCE(gpio));
    ASSERT_PARAM(IS_GPIO_PIN(gpio, gpio_pin));

    if (IS_GPIO_PIN_STATE_LOW(gpio->DATAIN, gpio_pin))
    {
        return GPIO_PIN_LOW;
    }
    else
    {
        return GPIO_PIN_HIGH;
    }
}

/**
 * @brief  Writes the specified output port pin.
 * @param  gpio Pointer to a struct SN_GPIO_Type that contains the GPIOx registers information.
 * @param  gpio_pin Specifies the port bit to be written.
 * @param  pin_state Specifies the data which is written to pin
 * @retval HAL_Status_T.
 */
HAL_Status_T HAL_GPIO_WritePin(SN_GPIO_Type gpio, uint32_t gpio_pin, uint32_t pin_state)
{
    /* Check whether the parameter is valid */
    ASSERT_PARAM(IS_GPIO_ALL_INSTANCE(gpio));
    ASSERT_PARAM(IS_GPIO_PIN(gpio, gpio_pin));
    /* Check whether the mode of gpio_pin is GPIO_MODE_OUTPUT */
    ASSERT_PARAM(IS_GPIO_MODE_OUTPUT(gpio, gpio_pin));

    if (IS_GPIO_DATA_LOW(pin_state))
    {
        HAL_REG_WRITE(gpio->BCLR, gpio_pin);
    }
    else
    {
        HAL_REG_WRITE(gpio->BSET, gpio_pin);
    }
    return HAL_OK;
}

/**
 * @brief  Toggles the specified output port pin.
 * @param  gpio Pointer to a struct SN_GPIO_Type that contains the GPIOx registers information.
 * @param  gpio_pin Specifies the port bit to be toggled.
 * @retval HAL_Status_T.
 */
HAL_Status_T HAL_GPIO_TogglePin(SN_GPIO_Type gpio, uint32_t gpio_pin)
{
    uint32_t odr = 0;

    /* Check whether the parameter is valid */
    ASSERT_PARAM(IS_GPIO_ALL_INSTANCE(gpio));
    ASSERT_PARAM(IS_GPIO_PIN(gpio, gpio_pin));
    /* Check whether the mode of gpio_pin is GPIO_MODE_OUTPUT */
    ASSERT_PARAM(IS_GPIO_MODE_OUTPUT(gpio, gpio_pin));

    /* get current Output Data Register value */
    odr = HAL_REG_READ(gpio->DATAOUT);

    /* Set selected pins that were at low level, and reset ones that were high */
    HAL_REG_WRITE(gpio->BCLR, (odr & gpio_pin));
    HAL_REG_WRITE(gpio->BSET, (~odr & gpio_pin));
    return HAL_OK;
}

/**
 * @brief  Change clock mode of the specified output port.
 * @param  gpio Pointer to a struct SN_GPIO_Type that contains the GPIOx registers information.
 * @retval HAL_Status_T.
 */
HAL_Status_T HAL_GPIO_ChangeClockMode(SN_GPIO_Type gpio, uint32_t clock_mode)
{
    /* Check whether the parameter is valid */
    ASSERT_PARAM(IS_GPIO_ALL_INSTANCE(gpio));
    ASSERT_PARAM(IS_GPIO_CLOCK_MODE(clock_mode));

    /* set clock mode if the existing clock mode is not equal to the set value */
    if (gpio->CLKMODE != clock_mode)
    {
        HAL_REG_WRITE(gpio->CLKMODE, clock_mode);
    }

    return HAL_OK;
}

/**
 * @brief  config the debounce for current gpio.
 * @param  gpio Pointer to a struct SN_GPIO_Type that contains the GPIOx registers information.
 * @param  pin Specifies the port bit to be config.
 * @param  deb_en enable/disable debounce function for current pin
 * @param  deb_pre detail debounce value, see \ref gpio_bnpre_range for config range
 * @retval HAL_Status_T.
 */
HAL_Status_T HAL_GPIO_ConfigDebounce(SN_GPIO_Type gpio, uint32_t gpio_pin, uint32_t deb_en, uint32_t deb_pre)
{
    /* Check whether the parameter is valid */
    ASSERT_PARAM(IS_GPIO_ALL_INSTANCE(gpio));
    ASSERT_PARAM(IS_GPIO_BNPRE_CONSTANT(deb_pre));

    if (!IS_GPIO_CLK_NORMAL(gpio->CLKMODE))
    {
        return HAL_ERROR;
    }

    /* set bounce prescaler*/
    HAL_REG_WRITE(gpio->BNCLKPRE, deb_pre);

    /* enable bounce*/
    if (deb_en == HAL_ENABLE)
    {
        HAL_REG_SBIT(gpio->BNEN, gpio_pin);
    }
    else
    {
        HAL_REG_CBIT(gpio->BNEN, gpio_pin);
    }

    return HAL_OK;
}
/**
 * @brief Release gpio Auto Hold the output function when exit from deep power down mode.
 * @param None
 * @return HAL_Status_T
 */
HAL_Status_T HAL_GPIO_ReleaseOutput(void)
{
    /* write 1 as clean*/
    HAL_REG_WRITE(SN_SCU->BTUP_STS_b.GPIO_HOLD, 1);
    return HAL_OK;
}

/**
 * @brief let the pin work as specify option
 *
 * @param gpio point to SN_GPIO_Type of general gpio
 * @param gpio_pin  the pin which will be config as one role; see \ref gpio_pins for pin definition.
 *             Only one pin could be setup at a time
 * @param option the specify option
 * @return HAL_Status_T
 */
HAL_Status_T HAL_GPIO_SetAFIO(SN_GPIO_Type gpio, uint32_t gpio_pin, uint16_t option)
{
    ASSERT_PARAM(IS_VALID_AFIO(gpio, gpio_pin));

    uint8_t            pin_pos  = POSITION_VAL(gpio_pin);
    uint8_t            port_pos = _pinmux_get_port_pos(gpio);
    const AFIO_Info_T *pin_afio = &AFIO_Info[port_pos + pin_pos];

    uint32_t desc = 1 << option;
    RET_FLAG_LACKS(pin_afio->desc, desc, HAL_ERROR);

    uint32_t field  = option << pin_afio->offset;
    uint32_t offset = AFIO_FIELD_MASK << pin_afio->offset;
    HAL_REG_CBIT(REG_CONST_EXPR(pin_afio->instance), offset);
    HAL_REG_SBIT(REG_CONST_EXPR(pin_afio->instance), field);
    return HAL_OK;
}

/**
 * @brief get current pins work option
 *
 * @param gpio
 * @param gpio_pin  see \ref gpio_pin for pin definition. Only one pin could be access at a time.
 * @param option the accessed pin option value will be stored in this field
 * @return HAL_Status_T
 */
HAL_Status_T HAL_GPIO_GetAFIO(SN_GPIO_Type gpio, uint32_t gpio_pin, uint16_t *option)
{
    ASSERT_PARAM(IS_VALID_AFIO(gpio, gpio_pin));
    ASSERT_PARAM(option);

    uint8_t            pin_pos  = POSITION_VAL(gpio_pin);
    uint8_t            port_pos = _pinmux_get_port_pos(gpio);
    const AFIO_Info_T *pin_afio = &AFIO_Info[port_pos + pin_pos];
    uint32_t           val      = HAL_REG_READ(REG_CONST_EXPR(pin_afio->instance));
    uint32_t           offset   = AFIO_FIELD_MASK << pin_afio->offset;

    val &= offset;
    *option = val >> pin_afio->offset;
    return HAL_OK;
}

/**
 * @brief GPIO IRQ server handler
 * @param gpio specifies IRQ Port
 * @retval None
 */
void HAL_GPIO_IRQHandler(SN_GPIO_Type gpio)
{
    uint32_t ris_rf = HAL_REG_READ(gpio->RIS_RF);

    if (IS_GPIO_CLK_NON(gpio->CLKMODE))
    {
        HAL_GPIO_ChangeClockMode(gpio, GPIO_CLK_NORMAL);
        HAL_REG_SBIT(gpio->IC, ris_rf);
        HAL_GPIO_ChangeClockMode(gpio, GPIO_CLK_NON);
    }
    else
    {
        HAL_REG_SBIT(gpio->IC, ris_rf);
    }

    HAL_GPIO_Callback(gpio, ris_rf);
}

/**
 * @brief GPIO IRQ callback function
 * @param gpio specifies IRQ Port
 * @param pin specifies IRQ Pin
 */
__weak void HAL_GPIO_Callback(SN_GPIO_Type gpio, uint32_t gpio_pin)
{
    /* Check whether the parameter is valid */
    ASSERT_PARAM(IS_GPIO_ALL_INSTANCE(gpio));
    ASSERT_PARAM(IS_GPIO_PIN(gpio, gpio_pin));
    /* Prevent unused argument(s) compilation warning */
    UNUSED(gpio);
    UNUSED(gpio_pin);
}

/* Private functions --------------------------------------------------------*/
/**
 * @brief  Sets the conditions for an interrupt to occur.
 * @param  gpio Pointer to a struct SN_GPIO_Type that contains the GPIOx registers information.
 * @param  gpio_pin Specifies the port bit to be de-initializes.
 * @param  mode setup irq trigger according to the pin mode
 * @retval HAL_Status_T
 */
static HAL_Status_T _gpio_config_irq(SN_GPIO_Type gpio, uint32_t pin, uint32_t mode)
{
    uint32_t imasken = HAL_DISABLE;                 /* imasken: HAL_DISABLE:trigger the irq handler; HAL_ENABLE: mask the irq handler */
    uint32_t ie      = HAL_REG_READ(gpio->IE);      /* IRQ enable flag */
    uint32_t ic      = HAL_REG_READ(gpio->IC);      /* clear interrupt flag and interrupt masked state  */
    uint32_t itrig   = HAL_REG_READ(gpio->ITRIG);   /* edge or event trigger */
    uint32_t ibetrig = HAL_REG_READ(gpio->IBETRIG); /* single edge or both edge trigger */
    uint32_t irftrig = HAL_REG_READ(gpio->IRFTRIG); /* rising edge/high level or falling edge/low level*/

    switch (mode)
    {
        case GPIO_MODE_EVT_HIGH:
            itrig |= pin;
            ibetrig &= ~pin;
            irftrig &= ~pin;
            break;
        case GPIO_MODE_EVT_LOW:
            itrig |= pin;
            ibetrig &= ~pin;
            irftrig |= pin;
            break;
        case GPIO_MODE_IT_RISING:
            itrig &= ~pin;
            ibetrig &= ~pin;
            irftrig &= ~pin;
            break;
        case GPIO_MODE_IT_FALLING:
            itrig &= ~pin;
            ibetrig &= ~pin;
            irftrig |= pin;
            break;
        case GPIO_MODE_IT_BOTH:
            itrig &= ~pin;
            ibetrig |= pin;
            break;
        default:
            /* default */
            ie &= ~pin;
            ic &= ~pin;
            imasken = HAL_ENABLE;
            break;
    }

    uint32_t imask = HAL_REG_READ(gpio->IMASKEN);
    if (IS_GPIO_IRQ_TRIGGER(imasken))
    {
        HAL_REG_WRITE(gpio->ITRIG, itrig);
        HAL_REG_WRITE(gpio->IBETRIG, ibetrig);
        if (GPIO_MODE_IT_BOTH != mode)
        {
            HAL_REG_WRITE(gpio->IRFTRIG, irftrig);
        }
        ie |= pin;
        ic |= pin;
        imask &= ~pin;
    }
    else
    {
        imask |= pin;
    }

    HAL_REG_WRITE(gpio->IC, ic);
    HAL_REG_WRITE(gpio->IE, ie);
    HAL_REG_WRITE(gpio->IMASKEN, imask);
    return HAL_OK;
}

static GPIO_PnSTR_T _gpio_get_pin_str(SN_GPIO_Type gpio, uint32_t gpio_pin)
{
    if (gpio == SN_GPIO0)
    {
        if (gpio_pin == GPIO_PIN_20)
        {
            return (GPIO_PnSTR_T){GPIO020_INDEX, GPIO0_STR_MASK};
        }
        else
        {
            return (GPIO_PnSTR_T){GPIO0_INDEX, GPIO0_STR_MASK};
        }
    }
    else if (gpio == SN_GPIO1)
    {
        return (GPIO_PnSTR_T){GPIO1_INDEX, GPIO1_STR_MASK};
    }
    else if (gpio == SN_GPIO2)
    {
        return (GPIO_PnSTR_T){GPIO2_INDEX, GPIO2_STR_MASK};
    }
    else
    {
        return (GPIO_PnSTR_T){GPIO3_INDEX, GPIO3_STR_MASK};
    }
}

static HAL_Status_T _gpio_config_pin_str(SN_GPIO_Type gpio, uint32_t pos, uint32_t str)
{
    uint32_t     pin   = 1 << pos;
    GPIO_PnSTR_T pnstr = _gpio_get_pin_str(gpio, pin);
    if (pnstr.mask & pin)
    {
        uint32_t str_mask = 0;
        uint32_t index    = pnstr.index;
        if (GPIO020_INDEX != pnstr.mask)
        {
            str_mask = 3 << (pos * 2);
            str      = str << (pos * 2);
        }
        else
        {
            // hard code for P0.20 PnSTR Setting
            str_mask = 3 << (8);
            str      = str << (8);
        }

        // SN_SCU->P0STR = 1;
        HAL_REG_CBIT(REG_PnSTR(index), str_mask);
        HAL_REG_SBIT(REG_PnSTR(index), str);
    }

    return HAL_OK;
}

/**
 * @brief get port pos value in AFIO_Info array
 *
 * @param gpio point to SN_GPIO_Type of general gpio
 * @return uint16_t return current port position value
 */
static uint16_t _pinmux_get_port_pos(SN_GPIO_Type gpio)
{
    if (SN_GPIO0 == gpio)
    {
        return P0_AFIO_INFO_INDEX;
    }
    else if (SN_GPIO1 == gpio)
    {
        return P1_AFIO_INFO_INDEX;
    }
    else if (SN_GPIO2 == gpio)
    {
        return P2_AFIO_INFO_INDEX;
    }
    return P3_AFIO_INFO_INDEX;
}
