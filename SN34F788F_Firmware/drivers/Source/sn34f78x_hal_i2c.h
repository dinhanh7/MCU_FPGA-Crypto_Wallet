/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_I2C_H_
#define _SN34F78X_HAL_I2C_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"

/* Private types -------------------------------------------------------------*/
typedef enum
{
    I2C_DIRECTION_TX       = 0x00,
    I2C_DIRECTION_RX       = 0x01,
    I2C_DIRECTION_TX_BURST = 0x02,
    I2C_DIRECTION_RX_BURST = 0x03,
    I2C_DIRECTION_NONE     = 0x0F,
} I2C_DIRECTION_T;

typedef enum
{
    I2C_XFER_STATE_START,
    I2C_XFER_STATE_HS_CTRL,
    I2C_XFER_STATE_7BITS_ADDR,
    I2C_XFER_STATE_10BITS_ADDR_H,
    I2C_XFER_STATE_10BITS_ADDR_L,
    I2C_XFER_STATE_10BITS_RESTART,
    I2C_XFER_STATE_10BITS_READ,
    I2C_XFER_STATE_DATA,
    I2C_XFER_STATE_STOP,
    I2C_XFER_STATE_COMPLETED,
} I2C_TRANSFER_STATE_T;

typedef enum
{
    I2C_STATUS_NONE             = 0U,
    I2C_STATUS_RW               = 1U << 0,
    I2C_STATUS_BUSY             = 1U << 2,
    I2C_STATUS_BUS_BUSY         = 1U << 3,
    I2C_STATUS_MASTER_RX_BST_TH = 1U << 4,
    I2C_STATUS_TRANSFER_DONE    = 1U << 5,
    I2C_STATUS_NACK             = 1U << 6,
    I2C_STATUS_STOP             = 1U << 7,
    I2C_STATUS_HIT              = 1U << 8,
    I2C_STATUS_GC               = 1U << 9,
    I2C_STATUS_ARBITRATION_LOST = 1U << 10,
    I2C_STATUS_START            = 1U << 11,
    I2C_STATUS_HS_MODE          = 1U << 22,
    I2C_STATUS_START_BYTE       = 1U << 23,
} I2C_STATUS_T;

typedef void (*I2C_IRQ_Handler)(I2C_Handle_T *hi2c, uint32_t raw_status);

/* Private constants ---------------------------------------------------------*/
#define I2C_READ_FLAG            0x01
#define I2C_NO_OPTION_FRAME      0x00
#define I2C_HS_CTRL_CODE         0x08
#define I2C_OPTION_SEQUENCE_FLAG (1U << 4)
#define I2C_OPTION_START_FLAG    (1U << 0)
#define I2C_OPTION_NACK_FLAG     (1U << 1)
#define I2C_OPTION_STOP_FLAG     (1U << 2)
// I2C Control Register
#define I2C_CTRL_MASK_RESET      (1U << 0)
#define I2C_CTRL_MASK_I2CEN      (1U << 1)
#define I2C_CTRL_MASK_MSTEN      (1U << 2)
#define I2C_CTRL_MASK_GCEN       (1U << 3)
#define I2C_CTRL_MASK_START      (1U << 4)
#define I2C_CTRL_MASK_STOP       (1U << 5)
#define I2C_CTRL_MASK_ACKNACK    (1U << 6)
#define I2C_CTRL_MASK_TBEN       (1U << 7)
#define I2C_CTRL_MASK_BSTTHODIEN (1U << 8)
#define I2C_CTRL_MASK_TDIEN      (1U << 9)
#define I2C_CTRL_MASK_NACKIEN    (1U << 10)
#define I2C_CTRL_MASK_STOPIEN    (1U << 11)
#define I2C_CTRL_MASK_SAMIEN     (1U << 12)
#define I2C_CTRL_MASK_ALIEN      (1U << 13)
#define I2C_CTRL_MASK_STARTIEN   (1U << 14)
#define I2C_CTRL_MASK_HSMODE     (1U << 19)
#define I2C_CTRL_MASK_HSIEN      (1U << 20)
#define I2C_CTRL_MASK_SBIEN      (1U << 21)
#define I2C_CTRL_MASK_BURSTEN    (1U << 24)

/* Private macros ------------------------------------------------------------*/
#define IT_OR_BURST (configUSE_I2C_NONBLOCKING_IT == 1 || configUSE_I2C_NONBLOCKING_BURST == 1)

#define IS_PERIPHERAL_I2C_INSTANCE(HANDLE) \
    (((HANDLE)->instance == SN_I2C0) ||    \
     ((HANDLE)->instance == SN_I2C1) ||    \
     ((HANDLE)->instance == SN_I2C2))

#define IS_PERIPHERAL_I2C_CLOCK_SPEED(CLOCK_SPEED)  \
    (((CLOCK_SPEED) == I2C_CLOCKSPEED_STANDARD) ||  \
     ((CLOCK_SPEED) == I2C_CLOCKSPEED_FAST) ||      \
     ((CLOCK_SPEED) == I2C_CLOCKSPEED_FAST_PLUS) || \
     ((CLOCK_SPEED) == I2C_CLOCKSPEED_HIGH_SPEED))

#define IS_PERIPHERAL_I2C_ADDRESS(ADDRESS) \
    ((ADDRESS & ~0x03FF) == 0)

#define IS_PERIPHERAL_I2C_ADDRESSING_MODE(ADDRESSDING_MODE) \
    (((ADDRESSDING_MODE) == I2C_ADDRESSINGMODE_7BIT) ||     \
     ((ADDRESSDING_MODE) == I2C_ADDRESSINGMODE_10BIT))

#define IS_PERIPHERAL_I2C_GENERAL_CALL_EN(GENERAL_CALL_EN) \
    (((GENERAL_CALL_EN) == HAL_DISABLE) ||                 \
     ((GENERAL_CALL_EN) == HAL_ENABLE))

#define IS_PERIPHERAL_I2C_BURST_INDEX2_EN(INDEX2_EN) \
    (((INDEX2_EN) == HAL_DISABLE) ||                 \
     ((INDEX2_EN) == HAL_ENABLE))

#define IS_I2C_TRANSFER_OPTION(OPTION)         \
    (((OPTION) == I2C_FIRST_AND_LAST_FRAME) || \
     ((OPTION) == I2C_FIRST_FRAME) ||          \
     ((OPTION) == I2C_NEXT_FRAME) ||           \
     ((OPTION) == I2C_LAST_FRAME_NO_STOP) ||   \
     ((OPTION) == I2C_LAST_FRAME) ||           \
     ((OPTION) == I2C_FIRST_AND_LAST_FRAME_NO_STOP))

#define PERIPHERAL_I2C_ENABLE(HANDLE)     HAL_REG_SBIT((HANDLE)->instance->CTRL, I2C_CTRL_MASK_I2CEN)
#define PERIPHERAL_I2C_DISABLE(HANDLE)    HAL_REG_CBIT((HANDLE)->instance->CTRL, I2C_CTRL_MASK_I2CEN)
#define I2C_7BITS_ADDR(ADDRESS)           ((ADDRESS & 0x007F) << 1)
#define I2C_10BITS_ADDR_HIGH(ADDRESS)     (0xF0 | ((ADDRESS & 0x0300) >> 7))
#define I2C_10BITS_ADDR_LOW(ADDRESS)      (ADDRESS & 0x00FF)
#define I2C_BURST_MODE(BURST_DIRECTION)   (BURST_DIRECTION - 1)
#define I2C_CLEAR_ALL_STATUS(INSTANCE)    HAL_REG_WRITE(INSTANCE->STATUS, 0x00C00FF0)
#define IS_I2C_MASTER_TX_SLAVE_RX(STATUS) ((STATUS & I2C_STATUS_RW) == 0x00)
#define IS_I2C_MASTER_RX_SLAVE_TX(STATUS) ((STATUS & I2C_STATUS_RW) == 0x01)

#define CHECK_STATUS_IN_TRANSFER(STATUS)                 \
    if (STATUS != HAL_OK)                                \
    {                                                    \
        if (hi2c->mode == HAL_I2C_MODE_MASTER)           \
        {                                                \
            /* Set stop condition */                     \
            HAL_REG_SBIT(reg->CTRL, I2C_CTRL_MASK_STOP); \
        }                                                \
        /* Clear I2C status */                           \
        I2C_CLEAR_ALL_STATUS(reg);                       \
        /* Disable the peripheral */                     \
        PERIPHERAL_I2C_DISABLE(hi2c);                    \
        /* Update the I2C state */                       \
        hi2c->state = HAL_STATE_READY;                   \
        hi2c->mode  = HAL_I2C_MODE_NONE;                 \
        /* Release the process protection */             \
        GIVE_MUTEX(hi2c);                                \
        /* Return the status */                          \
        return STATUS;                                   \
    }

#define CHECK_STATUS_IN_LOOP(STATUS) \
    if (STATUS != HAL_OK)            \
    {                                \
        break;                       \
    }

/* Private functions prototypes ----------------------------------------------*/
static HAL_Status_T _i2c_set_clock_speed(I2C_Handle_T *hi2c);
static void         _i2c_init_callbacks_to_default(I2C_Handle_T *hi2c);
static HAL_Status_T _i2c_wait_for_flag_until_timeout_target_first(I2C_Handle_T *hi2c, I2C_STATUS_T target_status, I2C_STATUS_T error_status, uint32_t tick_start, uint32_t timeout);
static HAL_Status_T _i2c_wait_for_flag_until_timeout_error_first(I2C_Handle_T *hi2c, I2C_STATUS_T target_status, I2C_STATUS_T error_status, uint32_t tick_start, uint32_t timeout, bool tb_en);
static HAL_Status_T _i2c_master_transfer_it(I2C_Handle_T *hi2c, I2C_DIRECTION_T direction, uint16_t dev_address, uint8_t *data, uint16_t size, uint32_t xfer_option);
static HAL_Status_T _i2c_slave_transfer_it(I2C_Handle_T *hi2c, I2C_DIRECTION_T direction, uint8_t *data, uint16_t size, uint32_t xfer_option);
static HAL_Status_T _i2c_check_transfer_option(I2C_Handle_T *hi2c, uint32_t direction, uint32_t xfer_option);
static void         _i2c_master_tx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status);
static void         _i2c_master_rx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status);
static void         _i2c_slave_tx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status);
static void         _i2c_slave_rx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status);
static void         _i2c_nack_condition(I2C_Handle_T *hi2c);
static void         _i2c_master_stop_condition(I2C_Handle_T *hi2c);
static void         _i2c_update_stop_state(I2C_Handle_T *hi2c);
static void         _i2c_master_completion_handler(I2C_Handle_T *hi2c);
static void         _i2c_slave_completion_handler(I2C_Handle_T *hi2c);
static void         _i2c_master_al_handler(I2C_Handle_T *hi2c);
static void         _i2c_it_transfer_end(I2C_Handle_T *hi2c);
static void         _i2c_slave_cplt_callback(I2C_Handle_T *hi2c);
static void         _i2c_ext_slave_tx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status);
static void         _i2c_ext_slave_rx_it_handler(I2C_Handle_T *hi2c, uint32_t raw_status);
static void         _i2c_ext_slave_stop_handler(I2C_Handle_T *hi2c);
static HAL_Status_T _i2c_master_transfer_burst(I2C_Handle_T *hi2c, I2C_DIRECTION_T direction, uint16_t dev_address, uint8_t *data, uint8_t size, I2C_Burst_T *burst_options);
static void         _i2c_master_tx_burst_handler(I2C_Handle_T *hi2c, uint32_t raw_status);
static void         _i2c_master_rx_burst_handler(I2C_Handle_T *hi2c, uint32_t raw_status);

#endif /* _SN34F78X_HAL_I2C_H_ */
