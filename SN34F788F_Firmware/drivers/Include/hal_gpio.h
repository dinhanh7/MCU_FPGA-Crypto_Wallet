/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_GPIO_H_
#define _HAL_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal_def.h"

/* Include GPIO HAL Extension module */
#include "hal_gpio_ex.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief GPIO Instance definition
 * \ingroup gpio_struct_define
 */
typedef SN_GPIO0_Type *SN_GPIO_Type;

/**
 * @brief GPIO Init structure definition
 * \ingroup gpio_struct_define
 */
typedef struct
{
    uint32_t pin;   /**< Specifies the GPIO pins to be configured.
                         This parameter can be any value of @ref gpio_pin */
    uint32_t mode;  /**< Specifies the operating mode for the selected pins.
                         This parameter can be a value of @ref gpio_mode */
    uint32_t pull;  /**< Specifies the pull function for the selected pins.
                         This parameter can be a value of @ref gpio_pull_mode */
    uint32_t drive; /**< Specifies the driving/sinking strength for the selected pins.
                         This parameter can be a value of @ref gpio_drive */
} GPIO_Init_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup gpio_data GPIO Data
 * \ingroup gpio_control
 * @brief  GPIO Bit Data
 * @{
 */
#define GPIO_PIN_LOW  0 /**< setup current pin data as low default */
#define GPIO_PIN_HIGH 1 /**< setup current pin data as high        */
/**
 * @}
 */

/**
 * \defgroup gpio_mode GPIO mode define
 * \ingroup gpio_control
 * @{
 */
#define GPIO_MODE_INPUT      0  /**< INPUT pin                                                */
#define GPIO_MODE_OUTPUT     1  /**< OUTPUT pin                                               */
#define GPIO_MODE_EVT_HIGH   2  /**< INPUT PIN IRQ trigger mode, HIGH LEVEL                   */
#define GPIO_MODE_EVT_LOW    4  /**< INPUT PIN IRQ trigger mode, LOW LEVEL                    */
#define GPIO_MODE_IT_RISING  6  /**< INPUT PIN IRQ trigger mode, RISING Edge                  */
#define GPIO_MODE_IT_FALLING 8  /**< INPUT PIN IRQ trigger mode, FALLING Edge                 */
#define GPIO_MODE_IT_BOTH    10 /**< INPUT PIN IRQ trigger mode, BOTH RISING and FALLING Edge */
/**
 * @}
 */

/**
 * \defgroup gpio_pull_mode GPIO pull function define
 * \ingroup gpio_control
 * @{
 */
#define GPIO_PULL_DOWN     0 /**< config current pin with PULL DOWN                   */
#define GPIO_PULL_UP       1 /**< config current pin with PULL UP                     */
#define GPIO_PULL_INACTIVE 2 /**< do not config current pin with pull up or pull down */
/**
 * @}
 */

/**
 * \defgroup gpio_clock_mode GPIO Clock mode define
 * \ingroup gpio_control
 * @{
 */
#define GPIO_CLK_NORMAL 0 /**< current pin as normal mode, default. use HAL_GPIO_ChangeClockMode to switch clock mode */
#define GPIO_CLK_NON    1 /**< current pin as non clock mode. use HAL_GPIO_ChangeClockMode to switch clock mode  */
/**
 * @}
 */

/**
 * \defgroup gpio_pin GPIO pins define
 * \ingroup gpio_control
 * @{
 */
#define GPIO_PIN_NONE 0x00000000U /**< No Pin selected */
#define GPIO_PIN_0    0x00000001U /**< Pin 0 selected  */
#define GPIO_PIN_1    0x00000002U /**< Pin 1 selected  */
#define GPIO_PIN_2    0x00000004U /**< Pin 2 selected  */
#define GPIO_PIN_3    0x00000008U /**< Pin 3 selected  */
#define GPIO_PIN_4    0x00000010U /**< Pin 4 selected  */
#define GPIO_PIN_5    0x00000020U /**< Pin 5 selected  */
#define GPIO_PIN_6    0x00000040U /**< Pin 6 selected  */
#define GPIO_PIN_7    0x00000080U /**< Pin 7 selected  */
#define GPIO_PIN_8    0x00000100U /**< Pin 8 selected  */
#define GPIO_PIN_9    0x00000200U /**< Pin 9 selected  */
#define GPIO_PIN_10   0x00000400U /**< Pin 10 selected */
#define GPIO_PIN_11   0x00000800U /**< Pin 11 selected */
#define GPIO_PIN_12   0x00001000U /**< Pin 12 selected */
#define GPIO_PIN_13   0x00002000U /**< Pin 13 selected */
#define GPIO_PIN_14   0x00004000U /**< Pin 14 selected */
#define GPIO_PIN_15   0x00008000U /**< Pin 15 selected */
#define GPIO_PIN_20   0x00100000U /**< Pin 20 selected */
/**
 * @}
 */

/**
 * \defgroup gpio_bnpre_range GPIO bounce prescaler range define
 * \ingroup gpio_control
 * @{
 */
#define GPIO_BNPRE_MIN 1        /**< GPIO bounce prescaler min constant */
#define GPIO_BNPRE_MAX 0xFFFFFF /**< GPIO bounce prescaler max constant */

/**
 * @}
 */

/**
 * \defgroup gpio_drive GPIO Driving/Sinking Strength
 * \ingroup gpio_control
 * @{
 */
#define GPIO_DRV_17mA 0 /**< GPIO Driving/Sinking Strength 17mA */
#define GPIO_DRV_19mA 1 /**< GPIO Driving/Sinking Strength 19mA */
#define GPIO_DRV_21mA 2 /**< GPIO Driving/Sinking Strength 21mA */
#define GPIO_DRV_23mA 3 /**< GPIO Driving/Sinking Strength 23mA */
/**
 * @}
 */

/* Exported macros --------------------------------------------------------*/
/* Input / Output */
#define GPIO_SET_MODE_INPUT(__PORT__, __PIN_MASK__)  (HAL_REG_CBIT(__PORT__->DIR, __PIN_MASK__))
#define GPIO_SET_MODE_OUTPUT(__PORT__, __PIN_MASK__) (HAL_REG_SBIT(__PORT__->DIR, __PIN_MASK__))
/* Pull Function */
#define GPIO_SET_PULL_NONE(__PORT__, __PIN_MASK__) (HAL_REG_CBIT(__PORT__->PULLEN, __PIN_MASK__))
#define GPIO_SET_PULL_DOWN(__PORT__, __PIN_MASK__) \
    HAL_REG_CBIT(__PORT__->PUTYPE, __PIN_MASK__);  \
    HAL_REG_SBIT(__PORT__->PULLEN, __PIN_MASK__);
#define GPIO_SET_PULL_UP(__PORT__, __PIN_MASK__)  \
    HAL_REG_SBIT(__PORT__->PUTYPE, __PIN_MASK__); \
    HAL_REG_SBIT(__PORT__->PULLEN, __PIN_MASK__);
/* Set Data */
#define GPIO_SET_DATA_LOW(__PORT__, __PIN_MASK__)  (HAL_REG_SBIT(__PORT__->BCLR, __PIN_MASK__))
#define GPIO_SET_DATA_HIGH(__PORT__, __PIN_MASK__) (HAL_REG_SBIT(__PORT__->BSET, __PIN_MASK__))
#define GPIO_TOGGLE(__PORT__, __PIN_MASK__)                                          \
    HAL_REG_WRITE(__PORT__->BCLR, (HAL_REG_READ(__PORT__->DATAOUT) & __PIN_MASK__)); \
    HAL_REG_WRITE(__PORT__->BSET, (~HAL_REG_READ(__PORT__->DATAOUT) & __PIN_MASK__));

/* Exported functions --------------------------------------------------------*/
/* Initialization/de-initialization functions  ********************************/
HAL_Status_T HAL_GPIO_Init(SN_GPIO_Type gpio, GPIO_Init_T *gpio_init);
HAL_Status_T HAL_GPIO_DeInit(SN_GPIO_Type gpio, uint32_t gpio_pin);

/* IO operation functions *****************************************************/
uint32_t     HAL_GPIO_ReadPin(SN_GPIO_Type gpio, uint32_t gpio_pin);
HAL_Status_T HAL_GPIO_WritePin(SN_GPIO_Type gpio, uint32_t gpio_pin, uint32_t pin_state);
HAL_Status_T HAL_GPIO_TogglePin(SN_GPIO_Type gpio, uint32_t gpio_pin);
HAL_Status_T HAL_GPIO_ChangeClockMode(SN_GPIO_Type gpio, uint32_t clock_mode);
HAL_Status_T HAL_GPIO_ConfigDebounce(SN_GPIO_Type gpio, uint32_t gpio_pin, uint32_t deb_en, uint32_t deb_pre);
HAL_Status_T HAL_GPIO_ReleaseOutput(void);

/* AFIO operation functions  ********************************/
HAL_Status_T HAL_GPIO_SetAFIO(SN_GPIO_Type gpio, uint32_t gpio_pin, uint16_t option);
HAL_Status_T HAL_GPIO_GetAFIO(SN_GPIO_Type gpio, uint32_t gpio_pin, uint16_t *option);

/* Peripheral IRQ functions  **************************************************/
void HAL_GPIO_IRQHandler(SN_GPIO_Type gpio);
void HAL_GPIO_Callback(SN_GPIO_Type gpio, uint32_t gpio_pin);

#ifdef __cplusplus
}
#endif

#endif /* _HAL_GPIO_H_ */
