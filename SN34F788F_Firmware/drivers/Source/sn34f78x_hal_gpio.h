/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_GPIO_H_
#define _SN34F78X_HAL_GPIO_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* private types -------------------------------------------------------------*/
typedef struct
{
    uint32_t index;
    uint32_t mask;
} const GPIO_PnSTR_T;

/**
 * @brief define the structure for enum pin AFIO info
 *
 */
typedef struct
{
    uint32_t instance; /**< store the gpio AFIO register */
    uint16_t offset;   /**< pin start bits in AFIO register */
    uint32_t desc;     /**< pin description mask in AFIO register */
} AFIO_Info_T;

/* private variables ---------------------------------------------------------*/
/* private constants ---------------------------------------------------------*/
/* private macros ------------------------------------------------------------*/

// define the driver/sinking pin mask
#define GPIO0_STR_MASK 0x10FC07
#define GPIO1_STR_MASK 0xFFFF
#define GPIO2_STR_MASK 0xFFFF
#define GPIO3_STR_MASK 0x3FFF

// define the gpio PnSTR register index
#define GPIO0_INDEX   0
#define GPIO1_INDEX   1
#define GPIO2_INDEX   2
#define GPIO3_INDEX   3
#define GPIO020_INDEX 4

// check whether GPIO instance is correct
#define IS_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == SN_GPIO0) || ((INSTANCE) == SN_GPIO1) || \
                                        ((INSTANCE) == SN_GPIO2) || ((INSTANCE) == SN_GPIO3))

// check whether GPIO data is correct
#define IS_GPIO_DATA(DATA) (((DATA) == GPIO_PIN_LOW) || ((DATA) == GPIO_PIN_HIGH))
// check whether GPIO data is GPIO_PIN_LOW
#define IS_GPIO_DATA_LOW(DATA) ((DATA) == GPIO_PIN_LOW)
// check whether GPIO pin data is GPIO_PIN_LOW
#define IS_GPIO_PIN_STATE_LOW(DATA, GPIO_PIN) ((DATA & GPIO_PIN) == GPIO_PIN_LOW)

// check whether GPIO pin is correct
#define IS_GPIO_PIN(INSTANCE, PIN)                                                                          \
    ((((PIN & GPIO0_STR_MASK) != 0x00U) && ((PIN & ~GPIO0_STR_MASK) == 0x00U) && (INSTANCE == SN_GPIO0)) || \
     (((PIN & GPIO1_STR_MASK) != 0x00U) && ((PIN & ~GPIO1_STR_MASK) == 0x00U) && (INSTANCE == SN_GPIO1)) || \
     (((PIN & GPIO2_STR_MASK) != 0x00U) && ((PIN & ~GPIO2_STR_MASK) == 0x00U) && (INSTANCE == SN_GPIO2)) || \
     (((PIN & GPIO3_STR_MASK) != 0x00U) && ((PIN & ~GPIO3_STR_MASK) == 0x00U) && (INSTANCE == SN_GPIO3)))

// check whether GPIO mode is correct
#define IS_GPIO_MODE(MODE) (((MODE) == GPIO_MODE_OUTPUT) || ((MODE) == GPIO_MODE_INPUT) || ((MODE) == GPIO_MODE_EVT_HIGH) || ((MODE) == GPIO_MODE_EVT_LOW) || \
                            ((MODE) == GPIO_MODE_IT_RISING) || ((MODE) == GPIO_MODE_IT_FALLING) || ((MODE) == GPIO_MODE_IT_BOTH))
// check whether GPIO mode is GPIO_MODE_INPUT
#define IS_GPIO_MODE_INPUT(GPIO, GPIO_PIN) ((GPIO->DIR & GPIO_PIN) == GPIO_MODE_INPUT)
// check whether GPIO mode is GPIO_MODE_OUTPUT
#define IS_GPIO_MODE_OUTPUT(GPIO, GPIO_PIN) ((GPIO->DIR & GPIO_PIN) == GPIO_PIN)

// check whether GPIO pull mode is correct
#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_PULL_UP) || ((PULL) == GPIO_PULL_DOWN) || ((PULL) == GPIO_PULL_INACTIVE))
// check whether GPIO pull mode is GPIO_PULL_INACTIVE
#define IS_GPIO_PULL_INACTIVE(PULL) (PULL == GPIO_PULL_INACTIVE)
// check whether is enable GPIO pull function
#define IS_GPIO_PULL_ENABLE(GPIO, GPIO_PIN) ((GPIO->PULLEN & GPIO_PIN) != 0)

// check whether GPIO clock mode is correct
#define IS_GPIO_CLOCK_MODE(CLOCK) (((CLOCK) == GPIO_CLK_NORMAL) || ((CLOCK) == GPIO_CLK_NON))
// check whether GPIO clock mode is GPIO_CLK_NORMAL
#define IS_GPIO_CLK_NORMAL(CLOCK) ((CLOCK) == GPIO_CLK_NORMAL)
// check whether GPIO clock mode is GPIO_CLK_NON
#define IS_GPIO_CLK_NON(CLOCK) ((CLOCK) == GPIO_CLK_NON)

// check whether GPIO IRQ is GPIO_IRQ_DISABLE
#define IS_GPIO_IRQ_TRIGGER(IMASKEN) ((IMASKEN) == HAL_DISABLE)

// check whether GPIO bounce prescaler constant is correct
#define IS_GPIO_BNPRE_CONSTANT(CONSTANT) (CONSTANT >= GPIO_BNPRE_MIN && CONSTANT <= GPIO_BNPRE_MAX)

// define the pin str register
#define REG_PnSTR(INDEX) *((uint32_t *)(&(SN_SCU->P0STR) + INDEX))

// check the valid hold option value
#define IS_GPIO_HOLD_OPTION(VAR) ((VAR) == HAL_ENABLE || (VAR) == HAL_DISABLE)

// AFIO Constant ------------------------------------------------------------------------------
// define each port index which locate at AFIO_Info array
#define P0_AFIO_INFO_INDEX 0U
#define P1_AFIO_INFO_INDEX 21U
#define P2_AFIO_INFO_INDEX 37U
#define P3_AFIO_INFO_INDEX 53U

// define the description of P0 reference pins
#define AFIO_P0_0_DESC  0x117FCFU
#define AFIO_P0_1_DESC  0x15FF7FU
#define AFIO_P0_2_DESC  0x15FF1FU
#define AFIO_P0_10_DESC 0x3FFDFU
#define AFIO_P0_11_DESC 0x4BFCFU
#define AFIO_P0_12_DESC 0x0FD8BU
#define AFIO_P0_13_DESC 0x074C3U
#define AFIO_P0_14_DESC 0x474AFU
#define AFIO_P0_15_DESC 0x1748BU
#define AFIO_P0_20_DESC 0x05021U

// define the description of P1 reference pins
#define AFIO_P1_0_DESC  0x005B7FFU
#define AFIO_P1_1_DESC  0x00777BFU
#define AFIO_P1_2_DESC  0x007BFFFU
#define AFIO_P1_3_DESC  0x0043D07U
#define AFIO_P1_4_DESC  0x005FF8FU
#define AFIO_P1_5_DESC  0x0017FBFU
#define AFIO_P1_6_DESC  0x015731FU
#define AFIO_P1_7_DESC  0x003735FU
#define AFIO_P1_8_DESC  0x00577FFU
#define AFIO_P1_9_DESC  0x00512FFU
#define AFIO_P1_10_DESC 0x0772FFU
#define AFIO_P1_11_DESC 0x05781FU
#define AFIO_P1_12_DESC 0x10FFFFU
#define AFIO_P1_13_DESC 0x10EFFFU
#define AFIO_P1_14_DESC 0x017F5FU
#define AFIO_P1_15_DESC 0x017FDDU

// define the description of P2 reference pins
#define AFIO_P2_0_DESC  0x05FAEFU
#define AFIO_P2_1_DESC  0x07F9FFU
#define AFIO_P2_2_DESC  0x01F9C3U
#define AFIO_P2_3_DESC  0x04B983U
#define AFIO_P2_4_DESC  0x01F3FFU
#define AFIO_P2_5_DESC  0x01433CF
#define AFIO_P2_6_DESC  0x11FFE7U
#define AFIO_P2_7_DESC  0x11FBC7U
#define AFIO_P2_8_DESC  0x15F9AFU
#define AFIO_P2_9_DESC  0x10F9CFU
#define AFIO_P2_10_DESC 0x13007U
#define AFIO_P2_11_DESC 0x7B07FU
#define AFIO_P2_12_DESC 0x1B103U
#define AFIO_P2_13_DESC 0x1B007U
#define AFIO_P2_14_DESC 0x1F303U
#define AFIO_P2_15_DESC 0x4F303U

// define the description of P3 reference pins
#define AFIO_P3_0_DESC  0x01A18FU
#define AFIO_P3_1_DESC  0x0321FFU
#define AFIO_P3_2_DESC  0x0721FFU
#define AFIO_P3_3_DESC  0x01A1FFU
#define AFIO_P3_4_DESC  0x15A0FFU
#define AFIO_P3_5_DESC  0x010007U
#define AFIO_P3_6_DESC  0x03001FU
#define AFIO_P3_7_DESC  0x000003U
#define AFIO_P3_8_DESC  0x00F041U
#define AFIO_P3_9_DESC  0x00F041U
#define AFIO_P3_10_DESC 0x02003U
#define AFIO_P3_11_DESC 0x02003U
#define AFIO_P3_12_DESC 0x1001FU
#define AFIO_P3_13_DESC 0x4001FU

// define each IO has 5 bits of current AFIO Port register
#define AFIO_FIELD_MASK 0x1FU

// define each port AFIO
#define P0_AFIO0 (SN_ALWAYSON_BASE + 0x00000200U)
#define P0_AFIO1 (SN_ALWAYSON_BASE + 0x00000204U)
#define P0_AFIO2 (SN_ALWAYSON_BASE + 0x00000208U)
#define P0_AFIO3 (SN_ALWAYSON_BASE + 0x0000020CU)
#define P1_AFIO0 (SN_ALWAYSON_BASE + 0x00000210U)
#define P1_AFIO1 (SN_ALWAYSON_BASE + 0x00000214U)
#define P1_AFIO2 (SN_ALWAYSON_BASE + 0x00000218U)
#define P2_AFIO0 (SN_ALWAYSON_BASE + 0x00000220U)
#define P2_AFIO1 (SN_ALWAYSON_BASE + 0x00000224U)
#define P2_AFIO2 (SN_ALWAYSON_BASE + 0x00000228U)
#define P3_AFIO0 (SN_ALWAYSON_BASE + 0x00000230U)
#define P3_AFIO1 (SN_ALWAYSON_BASE + 0x00000234U)
#define P3_AFIO2 (SN_ALWAYSON_BASE + 0x00000238U)

/* private macro ---------------------------------------------------------*/
// define the pins of P0 AFIO value for checking the validation of pins
#define AFIO_P0_PIN_MASK (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_10 | GPIO_PIN_11 | \
                          GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_20)

// define the pins of P1 AFIO value for checking the validation of pins
#define AFIO_P1_PIN_MASK (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |   \
                          GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                          GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15)

// define the pins of P2 AFIO value for checking the validation of pins
#define AFIO_P2_PIN_MASK AFIO_P1_PIN_MASK

// define the pins of P3 AFIO value for checking the validation of pins
#define AFIO_P3_PIN_MASK (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 |   \
                          GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                          GPIO_PIN_12 | GPIO_PIN_13)

// define the port & pin macro to check the validation of ports and pins
#define IS_VALID_AFIO(port, pin) ((((SN_GPIO0 == (port)) && ((AFIO_P0_PIN_MASK & (pin)) == (pin))) ||  \
                                   ((SN_GPIO1 == (port)) && ((AFIO_P1_PIN_MASK & (pin)) == (pin))) ||  \
                                   ((SN_GPIO2 == (port)) && ((AFIO_P2_PIN_MASK & (pin)) == (pin))) ||  \
                                   ((SN_GPIO3 == (port)) && ((AFIO_P3_PIN_MASK & (pin)) == (pin)))) && \
                                  (((pin) & ((pin) - 1)) == 0))

/* Private functions ---------------------------------------------------------*/
static HAL_Status_T _gpio_config_irq(SN_GPIO_Type gpio, uint32_t pin, uint32_t mode);
static GPIO_PnSTR_T _gpio_get_pin_str(SN_GPIO_Type gpio, uint32_t gpio_pin);
static HAL_Status_T _gpio_config_pin_str(SN_GPIO_Type gpio, uint32_t pos, uint32_t str);
static uint16_t     _pinmux_get_port_pos(SN_GPIO_Type gpio);

#endif /* _SN34F78X_HAL_GPIO_H_ */
