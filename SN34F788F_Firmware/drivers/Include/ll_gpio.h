/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_GPIO_H_
#define _LL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "ll_gpio_afio.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief GPIO Init structure definition
 * \ingroup gpio_struct_define
 */
typedef struct
{
    uint32_t pin_mask; /**< Specifies the GPIO pins to be configured.
                            This parameter can be any value of @ref gpio_pin */
    uint32_t mode;     /**< Specifies the operating mode for the selected pins.
                            This parameter can be a value of @ref gpio_mode */
    uint32_t pull;     /**< Specifies the pull function enablement for the selected pins.
                            This parameter can be a value of @ref gpio_pull_function */
    uint32_t ie;       /**< Specifies the interrupt enablement for the selected pins.
                            This parameter can be a value of LL_DISABLE of LL_ENABLE */
    uint32_t itrig;    /**< Specifies the interrupt trigger (edge or level) for the selected pins.
                            This parameter can be a value of @ref gpio_trigger_edge_level */
    uint32_t ibe;      /**< Specifies the interrupt trigger (single edge or both edge) for the selected pins.
                            This parameter can be a value of @ref gpio_trigger_single_both */
    uint32_t irf;      /**< Specifies the interrupt trigger (rising or falling) for the selected pins.
                            This parameter can be a value of @ref gpio_trigger_rising_falling */
} LL_GPIO_Init_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup gpio_pin GPIO Pins
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_PIN_NONE 0x00000000U /**< No Pin selected */
#define LL_GPIO_PIN_0    0x00000001U /**< Pin 0 selected  */
#define LL_GPIO_PIN_1    0x00000002U /**< Pin 1 selected  */
#define LL_GPIO_PIN_2    0x00000004U /**< Pin 2 selected  */
#define LL_GPIO_PIN_3    0x00000008U /**< Pin 3 selected  */
#define LL_GPIO_PIN_4    0x00000010U /**< Pin 4 selected  */
#define LL_GPIO_PIN_5    0x00000020U /**< Pin 5 selected  */
#define LL_GPIO_PIN_6    0x00000040U /**< Pin 6 selected  */
#define LL_GPIO_PIN_7    0x00000080U /**< Pin 7 selected  */
#define LL_GPIO_PIN_8    0x00000100U /**< Pin 8 selected  */
#define LL_GPIO_PIN_9    0x00000200U /**< Pin 9 selected  */
#define LL_GPIO_PIN_10   0x00000400U /**< Pin 10 selected */
#define LL_GPIO_PIN_11   0x00000800U /**< Pin 11 selected */
#define LL_GPIO_PIN_12   0x00001000U /**< Pin 12 selected */
#define LL_GPIO_PIN_13   0x00002000U /**< Pin 13 selected */
#define LL_GPIO_PIN_14   0x00004000U /**< Pin 14 selected */
#define LL_GPIO_PIN_15   0x00008000U /**< Pin 15 selected */
#define LL_GPIO_PIN_20   0x00100000U /**< Pin 20 selected */
/**
 * @}
 */

/**
 * \defgroup gpio_mode GPIO Mode
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_MODE_INPUT  0x00000000U /**< Direction: input  */
#define LL_GPIO_MODE_OUTPUT 0x00000001U /**< Direction: output */
/**
 * @}
 */

/**
 * \defgroup gpio_pull_function GPIO Pull Function
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_PULL_DOWN    0x00000000U /**< Pull down             */
#define LL_GPIO_PULL_UP      0x00000001U /**< Pull up               */
#define LL_GPIO_PULL_DISABLE 0x00000002U /**< Disable pull function */
/**
 * @}
 */

/**
 * \defgroup gpio_trigger_edge_level GPIO Interrupt Trigger Edge or Level
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_TRIGGER_EDGE  0x00000000U /**< Interrupt will be triggered at the edge  */
#define LL_GPIO_TRIGGER_LEVEL 0x00000001U /**< Interrupt will be triggered at the level */
/**
 * @}
 */

/**
 * \defgroup gpio_trigger_single_both GPIO Interrupt Trigger Single Edge or Both Edge
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_TRIGGER_SINGLE_EDGE 0x00000000U /**< Interrupt is triggered by the single edge */
#define LL_GPIO_TRIGGER_BOTH_EDGE   0x00000001U /**< Interrupt is triggered by both edges      */
/**
 * @}
 */

/**
 * \defgroup gpio_trigger_rising_falling GPIO Interrupt Trigger Rising or Falling
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_TRIGGER_RISING_OR_HIGH 0x00000000U /**< Interrupt is triggered at the rising edge/high level */
#define LL_GPIO_TRIGGER_FALLING_OR_LOW 0x00000001U /**< Interrupt is triggered at the falling edge/low level */
/**
 * @}
 */

/**
 * \defgroup gpio_bnpre_range GPIO Bounce Prescaler Range
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_BNPRE_MIN 0x00000001U /**< GPIO bounce prescaler min constant */
#define LL_GPIO_BNPRE_MAX 0x00FFFFFFU /**< GPIO bounce prescaler max constant */
/**
 * @}
 */

/**
 * \defgroup gpio_clock_mode GPIO Clock mode
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_CLKMODE_NORMAL 0x00000000U /**< GPIO Normal mode    */
#define LL_GPIO_CLKMODE_NONCLK 0x00000001U /**< GPIO Non-clock mode */
/**
 * @}
 */

/**
 * \defgroup gpio_str GPIO Driving/Sinking Strength
 * \ingroup gpio_control
 * @{
 */
#define LL_GPIO_STR_17mA 0x00000000U /**< GPIO driving/sinking strength 17mA */
#define LL_GPIO_STR_19mA 0x00000001U /**< GPIO driving/sinking strength 19mA */
#define LL_GPIO_STR_21mA 0x00000002U /**< GPIO driving/sinking strength 21mA */
#define LL_GPIO_STR_23mA 0x00000003U /**< GPIO driving/sinking strength 23mA */
/**
 * @}
 */

/* Exported macros --------------------------------------------------------*/
#define LL_GPIO_INPUT_PULLUP(PORT, PIN_MASK)                \
    LL_GPIO_SetPinMode(PORT, PIN_MASK, LL_GPIO_MODE_INPUT); \
    LL_GPIO_SetPinPull(PORT, PIN_MASK, LL_GPIO_PULL_UP); /**< Input Pullup */

#define LL_GPIO_INPUT_PULLDOWN(PORT, PIN_MASK)              \
    LL_GPIO_SetPinMode(PORT, PIN_MASK, LL_GPIO_MODE_INPUT); \
    LL_GPIO_SetPinPull(PORT, PIN_MASK, LL_GPIO_PULL_DOWN); /**< Input Pulldown */

#define LL_GPIO_OUTPUT_LOW(PORT, PIN_MASK)                   \
    LL_GPIO_SetPinMode(PORT, PIN_MASK, LL_GPIO_MODE_OUTPUT); \
    LL_GPIO_ResetOutputPin(PORT, PIN_MASK); /**< Output Low */

#define LL_GPIO_OUTPUT_HIGH(PORT, PIN_MASK)                  \
    LL_GPIO_SetPinMode(PORT, PIN_MASK, LL_GPIO_MODE_OUTPUT); \
    LL_GPIO_SetOutputPin(PORT, PIN_MASK); /**< Output High */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Configure gpio mode for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @param  mode This parameter can be one of @ref gpio_mode
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetPinMode(SN_GPIO0_Type *gpio, uint32_t pin_mask, uint32_t mode)
{
    LL_REG_CBIT(gpio->DIR, pin_mask);
    LL_REG_SBIT(gpio->DIR, (pin_mask * mode));
}

/**
 * @brief  Return gpio mode for a dedicated pin of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of @ref gpio_mode
 */
__STATIC_INLINE uint32_t LL_GPIO_GetPinMode(SN_GPIO0_Type *gpio, uint32_t pin)
{
    return (LL_REG_RBIT(gpio->DIR, pin) >> POSITION_VAL(pin));
}

/**
 * @brief  Configure gpio pull type for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @param  pull This parameter can be one of @ref gpio_pull_function
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetPinPull(SN_GPIO0_Type *gpio, uint32_t pin_mask, uint32_t pull)
{
    uint8_t pull_type = pull & 0x01;
    uint8_t pull_en   = (~pull & 0x02) >> 1;
    LL_REG_CBIT(gpio->PULLEN, pin_mask);
    LL_REG_SBIT(gpio->PULLEN, (pin_mask * pull_en));
    LL_REG_CBIT(gpio->PUTYPE, pin_mask);
    LL_REG_SBIT(gpio->PUTYPE, (pin_mask * pull_type));
}

/**
 * @brief  Return gpio pull type for a dedicated pin of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of the @ref gpio_pull_function
 */
__STATIC_INLINE uint32_t LL_GPIO_GetPinPull(SN_GPIO0_Type *gpio, uint32_t pin)
{
    uint8_t pull_type = LL_REG_RBIT(gpio->PUTYPE, pin) >> POSITION_VAL(pin);
    uint8_t pull_en   = LL_REG_RBIT(gpio->PULLEN, pin) >> POSITION_VAL(pin);
    return (pull_type | ((~pull_en & 0x01) << 1));
}

/**
 * @brief  Return full input data register value on dedicated port.
 * @param  gpio GPIO Port
 * @retval Input data register value of port
 */
__STATIC_INLINE uint32_t LL_GPIO_ReadInputPort(SN_GPIO0_Type *gpio)
{
    return LL_REG_READ(gpio->DATAIN);
}

/**
 * @brief  Return if input data level for several pins of dedicated port is high or low.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsInputPinSet(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->DATAIN, pin_mask) == (pin_mask));
}

/**
 * @brief  Write output data register for the port.
 * @param  gpio GPIO Port
 * @param  data Level value for each pin of the port
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_WriteOutputPort(SN_GPIO0_Type *gpio, uint32_t data)
{
    LL_REG_WRITE(gpio->DATAOUT, data);
}

/**
 * @brief  Return full output data register value on dedicated port.
 * @param  gpio GPIO Port
 * @retval Output data register value of port
 */
__STATIC_INLINE uint32_t LL_GPIO_ReadOutputPort(SN_GPIO0_Type *gpio)
{
    return LL_REG_READ(gpio->DATAOUT);
}

/**
 * @brief  Set several pins to high level on dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_FORCEINLINE void LL_GPIO_SetOutputPin(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_SBIT(gpio->BSET, pin_mask);
}

/**
 * @brief  Set several pins to low level for dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_FORCEINLINE void LL_GPIO_ResetOutputPin(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_SBIT(gpio->BCLR, pin_mask);
}

/**
 * @brief  Return if input data level for several pins of dedicated port is high or low.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsOutputPinSet(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->DATAOUT, pin_mask) == (pin_mask));
}

/**
 * @brief  Toggle data value for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_FORCEINLINE void LL_GPIO_TogglePin(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    uint32_t data = LL_REG_READ(gpio->DATAOUT);
    LL_REG_WRITE(gpio->BCLR, (data & pin_mask));
    LL_REG_WRITE(gpio->BSET, (~data & pin_mask));
}

/**
 * @brief  Enable interrupt for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_EnablePinIT(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_SBIT(gpio->IE, pin_mask);
}

/**
 * @brief  Disable interrupt for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_DisablePinIT(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_CBIT(gpio->IE, pin_mask);
}

/**
 * @brief  Return if interrupt is enabled for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsEnabledPinIT(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->IE, pin_mask) == (pin_mask));
}

/**
 * @brief  Set interrupt trigger (edge or level) for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @param  itrig This parameter can be one of @ref gpio_trigger_edge_level
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetPinITRIG(SN_GPIO0_Type *gpio, uint32_t pin_mask, uint32_t itrig)
{
    LL_REG_CBIT(gpio->ITRIG, pin_mask);
    LL_REG_SBIT(gpio->ITRIG, (pin_mask * itrig));
}

/**
 * @brief  Return interrupt trigger (edge or level) for a dedicated pin of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of @ref gpio_trigger_edge_level
 */
__STATIC_INLINE uint32_t LL_GPIO_GetPinITRIG(SN_GPIO0_Type *gpio, uint32_t pin)
{
    return (LL_REG_RBIT(gpio->ITRIG, pin) >> POSITION_VAL(pin));
}

/**
 * @brief  Set interrupt trigger (single edge or both edge) for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @param  ibe This parameter can be one of @ref gpio_trigger_single_both
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetPinIBE(SN_GPIO0_Type *gpio, uint32_t pin_mask, uint32_t ibe)
{
    LL_REG_CBIT(gpio->IBETRIG, pin_mask);
    LL_REG_SBIT(gpio->IBETRIG, (pin_mask * ibe));
}

/**
 * @brief  Return interrupt trigger (single edge or both edge) for a dedicated pin of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of @ref gpio_trigger_single_both
 */
__STATIC_INLINE uint32_t LL_GPIO_GetPinIBE(SN_GPIO0_Type *gpio, uint32_t pin)
{
    return (LL_REG_RBIT(gpio->IBETRIG, pin) >> POSITION_VAL(pin));
}

/**
 * @brief  Set interrupt trigger (rising or falling) for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @param  irf This parameter can be one of @ref gpio_trigger_rising_falling
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetPinIRF(SN_GPIO0_Type *gpio, uint32_t pin_mask, uint32_t irf)
{
    LL_REG_CBIT(gpio->IRFTRIG, pin_mask);
    LL_REG_SBIT(gpio->IRFTRIG, (pin_mask * irf));
}

/**
 * @brief  Return interrupt trigger (rising or falling) for a dedicated pin of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of @ref gpio_trigger_rising_falling
 */
__STATIC_INLINE uint32_t LL_GPIO_GetPinIRF(SN_GPIO0_Type *gpio, uint32_t pin)
{
    return (LL_REG_RBIT(gpio->IRFTRIG, pin) >> POSITION_VAL(pin));
}

/**
 * @brief  Return if interrupt of rising edge is active for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsActiveFlag_PinRISR(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->RIS_R, pin_mask) == (pin_mask));
}

/**
 * @brief  Clear interrupt of rising edge for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_ClearFlag_PinITR(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_SBIT(gpio->IC_R, pin_mask);
}

/**
 * @brief  Return if interrupt of falling edge is active for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsActiveFlag_PinRISF(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->RIS_F, pin_mask) == (pin_mask));
}

/**
 * @brief  Clear interrupt of falling edge for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_ClearFlag_PinITF(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_SBIT(gpio->IC_F, pin_mask);
}

/**
 * @brief  Return if interrupt of both edge is active for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsActiveFlag_PinRISRF(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->RIS_RF, pin_mask) == (pin_mask));
}

/**
 * @brief  Clear interrupt of both edge for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_ClearFlag_PinITRF(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_SBIT(gpio->IC, pin_mask);
}

/**
 * @brief  Enable interrupt mask for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_EnablePinITMask(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_SBIT(gpio->IMASKEN, pin_mask);
}

/**
 * @brief  Disable interrupt mask for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_DisablePinITMask(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_CBIT(gpio->IMASKEN, pin_mask);
}

/**
 * @brief  Return if interrupt mask is enabled for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsEnabledPinITMask(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->IMASKEN, pin_mask) == (pin_mask));
}

/**
 * @brief  Return if interrupt is not detected or is masked for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsPinITMasked(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->IMASKST_RF, pin_mask) == (pin_mask));
}

/**
 * @brief  Enable debounce for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_EnablePinDebounce(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_SBIT(gpio->BNEN, pin_mask);
}

/**
 * @brief  Disable debounce for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_DisablePinDebounce(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    LL_REG_CBIT(gpio->BNEN, pin_mask);
}

/**
 * @brief  Return if debounce is enabled for several pins of dedicated port.
 * @param  gpio GPIO Port
 * @param  pin_mask This parameter can be several ones of @ref gpio_pin
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_GPIO_IsEnabledPinDebounce(SN_GPIO0_Type *gpio, uint32_t pin_mask)
{
    return (LL_REG_RBIT(gpio->BNEN, pin_mask) == (pin_mask));
}

/**
 * @brief  Configure bounce clock prescaler for dedicated port.
 * @param  gpio GPIO Port
 * @param  pre This parameter can be a value between @ref gpio_bnpre_range
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetBouncePre(SN_GPIO0_Type *gpio, uint32_t pre)
{
    LL_REG_WRITE(gpio->BNCLKPRE, pre);
}

/**
 * @brief  Return bounce clock prescaler on dedicated port.
 * @param  gpio GPIO Port
 * @retval Bounce clock prescaler
 */
__STATIC_INLINE uint32_t LL_GPIO_GetBouncePre(SN_GPIO0_Type *gpio)
{
    return LL_REG_READ(gpio->BNCLKPRE);
}

/**
 * @brief  Configure clock mode for dedicated port.
 * @param  gpio GPIO Port
 * @param  clk_mode This parameter can be several ones of @ref gpio_clock_mode
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetCLKMode(SN_GPIO0_Type *gpio, uint32_t clk_mode)
{
    LL_REG_WRITE(gpio->CLKMODE, clk_mode);
}

/**
 * @brief  Return clock mode on dedicated port.
 * @param  gpio GPIO Port
 * @retval Returned value can be one of the @ref gpio_clock_mode
 */
__STATIC_INLINE uint32_t LL_GPIO_GetCLKMode(SN_GPIO0_Type *gpio)
{
    return LL_REG_READ(gpio->CLKMODE);
}

/**
 * @brief  Configure driving/sinking strength for a dedicated pin of port 0.
 * @param  pin This parameter can be one of @ref gpio_pin
 * @param  str This parameter can be one of @ref gpio_str
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetSTR_P0(uint32_t pin, uint32_t str)
{
    uint8_t offset = POSITION_VAL(pin) << 1;
    LL_REG_CBIT(SN_SCU->P0STR, 0x03 << offset);
    LL_REG_SBIT(SN_SCU->P0STR, str << offset);
}

/**
 * @brief  Return driving/sinking strength for a dedicated pin of port 0.
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of @ref gpio_str
 */
__STATIC_INLINE uint32_t LL_GPIO_GetSTR_P0(uint32_t pin)
{
    uint8_t offset = POSITION_VAL(pin) << 1;
    return (LL_REG_RBIT(SN_SCU->P0STR, 0x03 << offset) >> offset);
}

/**
 * @brief  Configure driving/sinking strength for a dedicated pin of port 1.
 * @param  pin This parameter can be one of @ref gpio_pin
 * @param  str This parameter can be one of @ref gpio_str
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetSTR_P1(uint32_t pin, uint32_t str)
{
    uint8_t offset = POSITION_VAL(pin) << 1;
    LL_REG_CBIT(SN_SCU->P1STR, 0x03 << offset);
    LL_REG_SBIT(SN_SCU->P1STR, str << offset);
}

/**
 * @brief  Return driving/sinking strength for a dedicated pin of port 1.
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of @ref gpio_str
 */
__STATIC_INLINE uint32_t LL_GPIO_GetSTR_P1(uint32_t pin)
{
    uint8_t offset = POSITION_VAL(pin) << 1;
    return (LL_REG_RBIT(SN_SCU->P1STR, 0x03 << offset) >> offset);
}

/**
 * @brief  Configure driving/sinking strength for a dedicated pin of port 2.
 * @param  pin This parameter can be one of @ref gpio_pin
 * @param  str This parameter can be one of @ref gpio_str
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetSTR_P2(uint32_t pin, uint32_t str)
{
    uint8_t offset = POSITION_VAL(pin) << 1;
    LL_REG_CBIT(SN_SCU->P2STR, 0x03 << offset);
    LL_REG_SBIT(SN_SCU->P2STR, str << offset);
}

/**
 * @brief  Return driving/sinking strength for a dedicated pin of port 2.
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of @ref gpio_str
 */
__STATIC_INLINE uint32_t LL_GPIO_GetSTR_P2(uint32_t pin)
{
    uint8_t offset = POSITION_VAL(pin) << 1;
    return (LL_REG_RBIT(SN_SCU->P2STR, 0x03 << offset) >> offset);
}

/**
 * @brief  Configure driving/sinking strength for a dedicated pin of port 3.
 * @param  pin This parameter can be one of @ref gpio_pin
 * @param  str This parameter can be one of @ref gpio_str
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetSTR_P3(uint32_t pin, uint32_t str)
{
    uint8_t offset = POSITION_VAL(pin) << 1;
    LL_REG_CBIT(SN_SCU->P3STR, 0x03 << offset);
    LL_REG_SBIT(SN_SCU->P3STR, str << offset);
}

/**
 * @brief  Return driving/sinking strength for a dedicated pin of port 3.
 * @param  pin This parameter can be one of @ref gpio_pin
 * @retval Returned value can be one of @ref gpio_str
 */
__STATIC_INLINE uint32_t LL_GPIO_GetSTR_P3(uint32_t pin)
{
    uint8_t offset = POSITION_VAL(pin) << 1;
    return (LL_REG_RBIT(SN_SCU->P3STR, 0x03 << offset) >> offset);
}

/**
 * @brief  Configure P0.0 ~ P0.2 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P0_AFIO0(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P0_AFIO0, afio);
}

/**
 * @brief  Return P0.0 ~ P0.2 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P0_AFIO0(void)
{
    return LL_REG_READ(SN_ALWAYSON->P0_AFIO0);
}

/**
 * @brief  Configure P0.10 ~ P0.11 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P0_AFIO1(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P0_AFIO1, afio);
}

/**
 * @brief  Return P0.10 ~ P0.11 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P0_AFIO1(void)
{
    return LL_REG_READ(SN_ALWAYSON->P0_AFIO1);
}

/**
 * @brief  Configure P0.12 ~ P0.15 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P0_AFIO2(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P0_AFIO2, afio);
}

/**
 * @brief  Return P0.12 ~ P0.15 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P0_AFIO2(void)
{
    return LL_REG_READ(SN_ALWAYSON->P0_AFIO2);
}

/**
 * @brief  Configure P0.20 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P0_AFIO3(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P0_AFIO3, afio);
}

/**
 * @brief  Return P0.20 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P0_AFIO3(void)
{
    return LL_REG_READ(SN_ALWAYSON->P0_AFIO3);
}

/**
 * @brief  Configure P1.0 ~ P1.5 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P1_AFIO0(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P1_AFIO0, afio);
}

/**
 * @brief  Return P1.0 ~ P1.5 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P1_AFIO0(void)
{
    return LL_REG_READ(SN_ALWAYSON->P1_AFIO0);
}

/**
 * @brief  Configure P1.6 ~ P1.11 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P1_AFIO1(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P1_AFIO1, afio);
}

/**
 * @brief  Return P1.6 ~ P1.11 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P1_AFIO1(void)
{
    return LL_REG_READ(SN_ALWAYSON->P1_AFIO1);
}

/**
 * @brief  Configure P1.12 ~ P1.15 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P1_AFIO2(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P1_AFIO2, afio);
}

/**
 * @brief  Return P1.12 ~ P1.15 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P1_AFIO2(void)
{
    return LL_REG_READ(SN_ALWAYSON->P1_AFIO2);
}

/**
 * @brief  Configure P2.0 ~ P2.5 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P2_AFIO0(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P2_AFIO0, afio);
}

/**
 * @brief  Return P2.0 ~ P2.5 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P2_AFIO0(void)
{
    return LL_REG_READ(SN_ALWAYSON->P2_AFIO0);
}

/**
 * @brief  Configure P2.6 ~ P2.11 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P2_AFIO1(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P2_AFIO1, afio);
}

/**
 * @brief  Return P2.6 ~ P2.11 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P2_AFIO1(void)
{
    return LL_REG_READ(SN_ALWAYSON->P2_AFIO1);
}

/**
 * @brief  Configure P2.12 ~ P2.15 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P2_AFIO2(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P2_AFIO2, afio);
}

/**
 * @brief  Return P2.12 ~ P2.15 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P2_AFIO2(void)
{
    return LL_REG_READ(SN_ALWAYSON->P2_AFIO2);
}

/**
 * @brief  Configure P3.0 ~ P3.5 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P3_AFIO0(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P3_AFIO0, afio);
}

/**
 * @brief  Return P3.0 ~ P3.5 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P3_AFIO0(void)
{
    return LL_REG_READ(SN_ALWAYSON->P3_AFIO0);
}

/**
 * @brief  Configure P3.6 ~ P3.11 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P3_AFIO1(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P3_AFIO1, afio);
}

/**
 * @brief  Return P3.6 ~ P3.11 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P3_AFIO1(void)
{
    return LL_REG_READ(SN_ALWAYSON->P3_AFIO1);
}

/**
 * @brief  Configure P3.12 ~ P3.13 alternate function.
 * @param  afio alternate function value
 * @retval None
 */
__STATIC_INLINE void LL_GPIO_SetAFIO_P3_AFIO2(uint32_t afio)
{
    LL_REG_WRITE(SN_ALWAYSON->P3_AFIO2, afio);
}

/**
 * @brief  Return P3.12 ~ P3.13 alternate function.
 * @retval alternate function value
 */
__STATIC_INLINE uint32_t LL_GPIO_GetAFIO_P3_AFIO2(void)
{
    return LL_REG_READ(SN_ALWAYSON->P3_AFIO2);
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_GPIO_Init(SN_GPIO0_Type *gpio, LL_GPIO_Init_T *gpio_init);
LL_Status_T LL_GPIO_DeInit(SN_GPIO0_Type *gpio);
void        LL_GPIO_StructInit(LL_GPIO_Init_T *gpio_init);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_GPIO_H_ */
