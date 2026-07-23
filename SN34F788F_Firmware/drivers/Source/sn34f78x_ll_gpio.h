/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_GPIO_H_
#define _SN34F78X_LL_GPIO_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
#define GPIO0_PIN_MASK 0x0010FC07
#define GPIO1_PIN_MASK 0x0000FFFF
#define GPIO2_PIN_MASK 0x0000FFFF
#define GPIO3_PIN_MASK 0x00003FFF

/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
#define IS_LL_GPIO_INSTANCE(__INSTANCE__) (((__INSTANCE__) == SN_GPIO0) || \
                                           ((__INSTANCE__) == SN_GPIO1) || \
                                           ((__INSTANCE__) == SN_GPIO2) || \
                                           ((__INSTANCE__) == SN_GPIO3))

#define IS_LL_GPIO_PIN(__INSTANCE__, __PINS__)                                                                                       \
    (((__INSTANCE__ == SN_GPIO0) && ((__PINS__ & GPIO0_PIN_MASK) != LL_GPIO_PIN_NONE) && ((__PINS__ & ~GPIO0_PIN_MASK) == 0x00U)) || \
     ((__INSTANCE__ == SN_GPIO1) && ((__PINS__ & GPIO1_PIN_MASK) != LL_GPIO_PIN_NONE) && ((__PINS__ & ~GPIO1_PIN_MASK) == 0x00U)) || \
     ((__INSTANCE__ == SN_GPIO2) && ((__PINS__ & GPIO2_PIN_MASK) != LL_GPIO_PIN_NONE) && ((__PINS__ & ~GPIO2_PIN_MASK) == 0x00U)) || \
     ((__INSTANCE__ == SN_GPIO3) && ((__PINS__ & GPIO3_PIN_MASK) != LL_GPIO_PIN_NONE) && ((__PINS__ & ~GPIO3_PIN_MASK) == 0x00U)))

#define IS_LL_GPIO_MODE(__VALUE__) (((__VALUE__) == LL_GPIO_MODE_INPUT) || \
                                    ((__VALUE__) == LL_GPIO_MODE_OUTPUT))

#define IS_LL_GPIO_PULL(__VALUE__) (((__VALUE__) == LL_GPIO_PULL_DOWN) || \
                                    ((__VALUE__) == LL_GPIO_PULL_UP) ||   \
                                    ((__VALUE__) == LL_GPIO_PULL_DISABLE))

#define IS_LL_GPIO_IE(__VALUE__) (((__VALUE__) == LL_DISABLE) || \
                                  ((__VALUE__) == LL_ENABLE))

#define IS_LL_GPIO_ITRIG(__VALUE__) (((__VALUE__) == LL_GPIO_TRIGGER_EDGE) || \
                                     ((__VALUE__) == LL_GPIO_TRIGGER_LEVEL))

#define IS_LL_GPIO_IBE(__VALUE__) (((__VALUE__) == LL_GPIO_TRIGGER_SINGLE_EDGE) || \
                                   ((__VALUE__) == LL_GPIO_TRIGGER_BOTH_EDGE))

#define IS_LL_GPIO_IRF(__VALUE__) (((__VALUE__) == LL_GPIO_TRIGGER_RISING_OR_HIGH) || \
                                   ((__VALUE__) == LL_GPIO_TRIGGER_FALLING_OR_LOW))

/* Private functions prototypes ----------------------------------------------*/

#endif /* _SN34F78X_LL_GPIO_H_ */
