/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_I2C_H_
#define _LL_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll_def.h"
#include "ll_reg_msk.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief I2C Init Structure Definition
 * \ingroup i2c_struct_define
 */
typedef struct
{
    uint32_t clock_speed;     /**< Specifies the clock frequency.
                                   This parameter can be a value of @ref i2c_clock_speed */
    uint32_t own_address;     /**< Specifies the device own address.
                                   This parameter can be a 7-bit or 10-bit address. */
    uint32_t addressing_mode; /**< Specifies if 7-bit or 10-bit addressing mode is selected.
                                   This parameter can be a value of @ref i2c_addressing_mode */
    uint32_t gc_en;           /**< Specifies if general call is enabled.
                                   This parameter can be a value of @ref LL_ENABLE or @ref LL_DISABLE. */
} LL_I2C_Init_T;

/* Exported constants --------------------------------------------------------*/
/**
 * \defgroup i2c_clock_speed I2C Clock Speed
 * \ingroup i2c_control
 * @{
 */
#define LL_I2C_CLOCKSPEED_STANDARD   0x00000000U /**< I2C works in Standard Mode (100KHz) */
#define LL_I2C_CLOCKSPEED_FAST       0x00000001U /**< I2C works in Fast Mode (400KHz) */
#define LL_I2C_CLOCKSPEED_FAST_PLUS  0x00000002U /**< I2C works in Fast+ Mode (1MHz) */
#define LL_I2C_CLOCKSPEED_HIGH_SPEED 0x00000003U /**< I2C works in High Speed Mode (3MHz) */
/**
 * @}
 */

/**
 * \defgroup i2c_addressing_mode I2C Addressing Mode
 * \ingroup i2c_control
 * @{
 */
#define LL_I2C_ADDRESSINGMODE_7BIT  0x00000000U /**< The device address is under 7 bits */
#define LL_I2C_ADDRESSINGMODE_10BIT 0x00000001U /**< The device address is under 10 bits */
/**
 * @}
 */

/**
 * \defgroup i2c_burst_mode I2C Burst Mode
 * \ingroup i2c_control
 * @{
 */
#define LL_I2C_BURST_DISABLE   I2C_CTRL_BURSTEN_Disable   /**< I2C Master/Slave protocol */
#define LL_I2C_BURST_MASTER_TX I2C_CTRL_BURSTEN_Master_TX /**< Master TX burst mode */
#define LL_I2C_BURST_MASTER_RX I2C_CTRL_BURSTEN_Master_RX /**< Master RX burst mode */
/**
 * @}
 */

/* Private constants ---------------------------------------------------------*/
#define LL_I2C_STATUS_MASK 0x00C00FF0

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Reset the I2C peripheral
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Reset(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_RESET);
}

/**
 * @brief  Enable the I2C controller
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_I2CEN);
}

/**
 * @brief  Disable the I2C controller
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_I2CEN);
}

/**
 * @brief  Return if I2C controller is enabled
 * @param  i2c i2c Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_I2CEN) == (I2C_CTRL_I2CEN));
}

/**
 * @brief  Enable the I2C master mode
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable_MasterMode(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_MSTEN);
}

/**
 * @brief  Disable the I2C master mode
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable_MasterMode(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_MSTEN);
}

/**
 * @brief  Return if I2C master mode is enabled
 * @param  i2c i2c Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled_MasterMode(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_MSTEN) == (I2C_CTRL_MSTEN));
}

/**
 * @brief  Enable the I2C general call response
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable_GC(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_GCEN);
}

/**
 * @brief  Disable the I2C general call response
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable_GC(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_GCEN);
}

/**
 * @brief  Return if I2C general call response is enabled
 * @param  i2c i2c Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled_GC(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_GCEN) == (I2C_CTRL_GCEN));
}

/**
 * @brief  Send start condition
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_SendStart(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_START);
}

/**
 * @brief  Send stop condition
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_SendStop(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_STOP);
}

/**
 * @brief  Send ACK signal
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_SendACK(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_ACKNACK);
}

/**
 * @brief  Send NACK signal
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_SendNACK(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_ACKNACK);
}

/**
 * @brief  Enable transfer byte
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_TransferByte(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_TBEN);
}

/**
 * @brief  Enable burst threshold interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_BSTTH(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_BSTTHODIEN);
}

/**
 * @brief  Disable burst threshold interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_BSTTH(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_BSTTHODIEN);
}

/**
 * @brief  Return if burst threshold interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_BSTTH(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_BSTTHODIEN) == (I2C_CTRL_BSTTHODIEN));
}

/**
 * @brief  Enable data interrupt interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_TD(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_TDIEN);
}

/**
 * @brief  Disable data threshold interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_TD(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_TDIEN);
}

/**
 * @brief  Return if data threshold interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_TD(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_TDIEN) == (I2C_CTRL_TDIEN));
}

/**
 * @brief  Enable NACK interrupt interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_NACK(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_NACKIEN);
}

/**
 * @brief  Disable NACK threshold interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_NACK(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_NACKIEN);
}

/**
 * @brief  Return if NACK threshold interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_NACK(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_NACKIEN) == (I2C_CTRL_NACKIEN));
}

/**
 * @brief  Enable stop interrupt interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_STOP(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_STOPIEN);
}

/**
 * @brief  Disable stop threshold interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_STOP(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_STOPIEN);
}

/**
 * @brief  Return if stop threshold interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_STOP(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_STOPIEN) == (I2C_CTRL_STOPIEN));
}

/**
 * @brief  Enable slave address hit interrupt interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_SAM(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_SAMIEN);
}

/**
 * @brief  Disable slave address hit threshold interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_SAM(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_SAMIEN);
}

/**
 * @brief  Return if slave address hit interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_SAM(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_SAMIEN) == (I2C_CTRL_SAMIEN));
}

/**
 * @brief  Enable lose arbitration interrupt interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_AL(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_ALIEN);
}

/**
 * @brief  Disable lose arbitration threshold interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_AL(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_ALIEN);
}

/**
 * @brief  Return if lose arbitration interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_AL(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_ALIEN) == (I2C_CTRL_ALIEN));
}

/**
 * @brief  Enable start condition interrupt interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_START(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_STARTIEN);
}

/**
 * @brief  Disable start condition threshold interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_START(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_STARTIEN);
}

/**
 * @brief  Return if start condition interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_START(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_STARTIEN) == (I2C_CTRL_STARTIEN));
}

/**
 * @brief  Enable SCL tied to low
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable_SCLLOW(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_SCLLOW);
}

/**
 * @brief  Disable SCL tied to low
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable_SCLLOW(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_SCLLOW);
}

/**
 * @brief  Return if SCL tied to low
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled_SCLLOW(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_SCLLOW) == (I2C_CTRL_SCLLOW));
}

/**
 * @brief  Enable SDA tied to low
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable_SDALOW(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_SDALOW);
}

/**
 * @brief  Disable SDA tied to low
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable_SDALOW(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_SDALOW);
}

/**
 * @brief  Return if SDA tied to low
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled_SDALOW(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_SDALOW) == (I2C_CTRL_SDALOW));
}

/**
 * @brief  Enable to ignore the arbitration lose detection in the single I2C master environment
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable_ARBOFF(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_ARBOFF);
}

/**
 * @brief  Disable to ignore the arbitration lose detection in the single I2C master environment
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable_ARBOFF(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_ARBOFF);
}

/**
 * @brief  Return if ignore the arbitration lose detection in the single I2C master environment
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled_ARBOFF(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_ARBOFF) == (I2C_CTRL_ARBOFF));
}

/**
 * @brief  Enable HS-mode (Generate mode 2 SCL refer to COUNTH)
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable_HSMODE(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_HSMODE);
}

/**
 * @brief  Disable HS-mode (Generate mode 1 SCL refer to COUNT)
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable_HSMODE(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_HSMODE);
}

/**
 * @brief  Return if HS-mode is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled_HSMODE(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_HSMODE) == (I2C_CTRL_HSMODE));
}

/**
 * @brief  Enable HS-mode (mode 2) code interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_HS(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_HSIEN);
}

/**
 * @brief  Disable HS-mode (mode 2) code interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_HS(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_HSIEN);
}

/**
 * @brief  Return if HS-mode (mode 2) code interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_HS(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_HSIEN) == (I2C_CTRL_HSIEN));
}

/**
 * @brief  Enable start byte interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_EnableIT_SB(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->CTRL, I2C_CTRL_SBIEN);
}

/**
 * @brief  Disable start byte interrupt
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_DisableIT_SB(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_SBIEN);
}

/**
 * @brief  Return if start byte interrupt is enabled
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabledIT_SB(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_SBIEN) == (I2C_CTRL_SBIEN));
}

/**
 * @brief  Set burst mode
 * @param  i2c I2C Instance
 * @param  burst This parameter can be several ones of @ref i2c_burst_mode
 * @retval none
 */
__STATIC_INLINE void LL_I2C_SetBurstMode(SN_I2C0_Type *i2c, uint32_t burst)
{
    LL_REG_CBIT(i2c->CTRL, I2C_CTRL_BURSTEN);
    LL_REG_SBIT(i2c->CTRL, burst & I2C_CTRL_BURSTEN);
}

/**
 * @brief  Get burst mode
 * @param  i2c I2C Instance
 * @retval Returned value can be one of @ref i2c_burst_mode
 */
__STATIC_INLINE uint32_t LL_I2C_GetBurstMode(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CTRL, I2C_CTRL_BURSTEN));
}

/**
 * @brief  Return whole status of I2c.
 * @param  i2c I2C Instance
 * @retval Value of status register.
 */
__STATIC_INLINE uint32_t LL_I2C_GetStatus(SN_I2C0_Type *i2c)
{
    return LL_REG_READ(i2c->STATUS);
}

/**
 * @brief  Return whole status of I2c.
 * @param  i2c I2C Instance
 * @param  flag The mask of status to be cleared
 * @retval none
 */
__STATIC_INLINE void LL_I2C_ClearStatus(SN_I2C0_Type *i2c, uint32_t flag)
{
    LL_REG_WRITE(i2c->STATUS, flag);
}

/**
 * @brief  Return if master is in RX mode / slave is in TX mode
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_MSTRX_SLVTX(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_RW) == (I2C_STATUS_RW));
}

/**
 * @brief  Return if i2c is busy
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_I2CBusy(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_I2CB) == (I2C_STATUS_I2CB));
}

/**
 * @brief  Return if bus is busy
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_BusBusy(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_BB) == (I2C_STATUS_BB));
}

/**
 * @brief  Return if data reaches the threshold in the master RX burst mode
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_BurstRXTH(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_BSTTHODSR) == (I2C_STATUS_BSTTHODSR));
}

/**
 * @brief  Clear status of data reaches the threshold in the master RX burst mode
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_BurstRXTH(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_BSTTHODSR);
}

/**
 * @brief  Return if transferring of a byte is finished, or the total data count (TDC bits in BSTM register) is finished in the master RX/TX burst mode
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_TD(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_TD) == (I2C_STATUS_TD));
}

/**
 * @brief  Clear status of transfer done
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_TD(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_TD);
}

/**
 * @brief  Return if detected an NACK response
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_NACK(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_NACK) == (I2C_STATUS_NACK));
}

/**
 * @brief  Clear status of NACK response
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_NACK(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_NACK);
}

/**
 * @brief  Return if detected an stop condition
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_STOP(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_STOP) == (I2C_STATUS_STOP));
}

/**
 * @brief  Clear status of detecting stop condition
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_STOP(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_STOP);
}

/**
 * @brief  Return if received slave address that hits the address in SLVADDR register
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_HIT(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_HIT) == (I2C_STATUS_HIT));
}

/**
 * @brief  Clear status of slave address hitting
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_HIT(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_HIT);
}

/**
 * @brief  Return if matched GC address in slave mode
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_GC(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_GC) == (I2C_STATUS_GC));
}

/**
 * @brief  Clear status of matching GC address in slave mode
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_GC(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_GC);
}

/**
 * @brief  Return if lost arbitration in master mode
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_AL(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_AL) == (I2C_STATUS_AL));
}

/**
 * @brief  Clear status of losing arbitration in master mode
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_AL(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_AL);
}

/**
 * @brief  Return if detected a start condition
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_START(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_START) == (I2C_STATUS_START));
}

/**
 * @brief  Clear status of detecting start condition
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_START(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_START);
}

/**
 * @brief  Return if detected a HS-mode (mode 2) code
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_HS(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_HSS) == (I2C_STATUS_HSS));
}

/**
 * @brief  Clear status of detecting HS-mode (mode 2) code
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_HS(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_HSS);
}

/**
 * @brief  Return if detected a start byte in slave mode
 * @param  i2c I2C Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsActiveFlag_SB(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->STATUS, I2C_STATUS_SBS) == (I2C_STATUS_SBS));
}

/**
 * @brief  Clear status of detecting a start byte in slave mode
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE void LL_I2C_ClearFlag_SB(SN_I2C0_Type *i2c)
{
    LL_REG_WRITE(i2c->STATUS, I2C_STATUS_SBS);
}

/**
 * @brief  Set counter value for F/S-mode to generate an I2C clock
 * @param  i2c I2C Instance
 * @param  counter Counter value
 * @retval None
 */
__STATIC_INLINE void LL_I2C_SetCounter(SN_I2C0_Type *i2c, uint32_t counter)
{
    LL_REG_CBIT(i2c->CLKDIV, I2C_CLKDIV_COUNT);
    LL_REG_SBIT(i2c->CLKDIV, (counter << I2C_CLKDIV_COUNT_Pos) & I2C_CLKDIV_COUNT);
}

/**
 * @brief  Set counter value for F/S-mode to generate an I2C clock
 * @param  i2c I2C Instance
 * @retval None
 */
__STATIC_INLINE uint32_t LL_I2C_GetCounter(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CLKDIV, I2C_CLKDIV_COUNT) >> I2C_CLKDIV_COUNT_Pos);
}

/**
 * @brief  Set counter value for F/S-mode to generate an I2C clock
 * @param  i2c I2C Instance
 * @param  counter Counter value
 * @retval None
 */
__STATIC_INLINE void LL_I2C_SetCounterH(SN_I2C0_Type *i2c, uint32_t counter)
{
    LL_REG_CBIT(i2c->CLKDIV, I2C_CLKDIV_COUNTH);
    LL_REG_SBIT(i2c->CLKDIV, (counter << I2C_CLKDIV_COUNTH_Pos) & I2C_CLKDIV_COUNTH);
}

/**
 * @brief  Get counter value for HS-mode to generate an I2C clock
 * @param  i2c I2C Instance
 * @retval Counter value for HS-mode to generate an I2C clock
 */
__STATIC_INLINE uint32_t LL_I2C_GetCounterH(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CLKDIV, I2C_CLKDIV_COUNTH) >> I2C_CLKDIV_COUNTH_Pos);
}

/**
 * @brief  Set I2C clock duty
 * @param  i2c I2C Instance
 * @param  duty_offset Clock Duty
 * @retval None
 */
__STATIC_INLINE void LL_I2C_SetDutyOffset(SN_I2C0_Type *i2c, uint32_t duty_offset)
{
    LL_REG_CBIT(i2c->CLKDIV, I2C_CLKDIV_DUTY_OFFSET);
    LL_REG_SBIT(i2c->CLKDIV, (duty_offset << I2C_CLKDIV_DUTY_OFFSET_Pos) & I2C_CLKDIV_DUTY_OFFSET);
}

/**
 * @brief  Get I2C clock duty
 * @param  i2c I2C Instance
 * @retval I2C Clock Duty
 */
__STATIC_INLINE uint32_t LL_I2C_GetDutyOffset(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->CLKDIV, I2C_CLKDIV_DUTY_OFFSET) >> I2C_CLKDIV_DUTY_OFFSET_Pos);
}

/**
 * @brief  Write in transmit data register
 * @param  i2c I2C Instance
 * @param  data Value to be transmitted
 * @retval None
 */
__STATIC_INLINE void LL_I2C_TransmitData(SN_I2C0_Type *i2c, uint8_t data)
{
    LL_REG_WRITE(i2c->DATA, data);
}

/**
 * @brief  Read receive data register.
 * @param  i2c I2C Instance
 * @retval Received data
 */
__STATIC_INLINE uint8_t LL_I2C_ReceiveData(SN_I2C0_Type *i2c)
{
    return LL_REG_READ(i2c->DATA);
}

/**
 * @brief  Set the 7-bit/10-bit I2C address
 * @param  i2c  I2C Instance
 * @param  addr Slave address
 * @retval None
 */
__STATIC_INLINE void LL_I2C_SetAddress(SN_I2C0_Type *i2c, uint32_t addr)
{
    LL_REG_CBIT(i2c->ADDR, I2C_ADDR_ADDR);
    LL_REG_SBIT(i2c->ADDR, (addr << I2C_ADDR_ADDR_Pos) & I2C_ADDR_ADDR);
}

/**
 * @brief  Get the 7-bit/10-bit I2C address
 * @param  i2c I2C Instance
 * @retval Slave address
 */
__STATIC_INLINE uint32_t LL_I2C_GetAddress(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->ADDR, I2C_ADDR_ADDR) >> I2C_ADDR_ADDR_Pos);
}

/**
 * @brief  Enable 10-bit addressing mode
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable_10BitAddressing(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->ADDR, I2C_ADDR_ADDR10EN);
}

/**
 * @brief  Disable 10-bit addressing mode
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable_10BitAddressing(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->ADDR, I2C_ADDR_ADDR10EN);
}

/**
 * @brief  Return if 10-bit addressing mode is enabled
 * @param  i2c i2c Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled_10BitAddressing(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->ADDR, I2C_ADDR_ADDR10EN) == (I2C_ADDR_ADDR10EN));
}

/**
 * @brief  Enable 2nd index bytes transfer
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Enable_2ndIndex(SN_I2C0_Type *i2c)
{
    LL_REG_SBIT(i2c->ADDR, I2C_ADDR_M2BIDX_EN);
}

/**
 * @brief  Disable 2nd index bytes transfer
 * @param  i2c I2C Instance
 * @retval none
 */
__STATIC_INLINE void LL_I2C_Disable_2ndIndex(SN_I2C0_Type *i2c)
{
    LL_REG_CBIT(i2c->ADDR, I2C_ADDR_M2BIDX_EN);
}

/**
 * @brief  Return if 2nd index bytes transfer is enabled
 * @param  i2c i2c Instance
 * @retval State of bit (1 or 0).
 */
__STATIC_INLINE uint32_t LL_I2C_IsEnabled_2ndIndex(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->ADDR, I2C_ADDR_M2BIDX_EN) == (I2C_ADDR_M2BIDX_EN));
}

/**
 * @brief  Set the 1st transmitted index for the master RX/TX burst mode
 * @param  i2c I2C Instance
 * @param  index 1st transmitted index for burst mode
 * @retval None
 */
__STATIC_INLINE void LL_I2C_Set1stIndex(SN_I2C0_Type *i2c, uint8_t index)
{
    LL_REG_CBIT(i2c->ADDR, I2C_ADDR_MEM_IDX);
    LL_REG_SBIT(i2c->ADDR, (index << I2C_ADDR_MEM_IDX_Pos) & I2C_ADDR_MEM_IDX);
}

/**
 * @brief  Get the 1st transmitted index for the master RX/TX burst mode
 * @param  i2c I2C Instance
 * @retval 1st Transmitted Index
 */
__STATIC_INLINE uint32_t LL_I2C_Get1stIndex(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->ADDR, I2C_ADDR_MEM_IDX) >> I2C_ADDR_MEM_IDX_Pos);
}

/**
 * @brief  Set the 2nd transmitted index for the master RX/TX burst mode
 * @param  i2c I2C Instance
 * @param  index 2nd transmitted index for burst mode
 * @retval None
 */
__STATIC_INLINE void LL_I2C_Set2ndIndex(SN_I2C0_Type *i2c, uint8_t index)
{
    LL_REG_CBIT(i2c->ADDR, I2C_ADDR_MEM_IDX2);
    LL_REG_SBIT(i2c->ADDR, (index << I2C_ADDR_MEM_IDX2_Pos) & I2C_ADDR_MEM_IDX2);
}

/**
 * @brief  Get the 2nd transmitted index for the master RX/TX burst mode
 * @param  i2c I2C Instance
 * @retval 2nd Transmitted Index
 */
__STATIC_INLINE uint32_t LL_I2C_Get2ndIndex(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->ADDR, I2C_ADDR_MEM_IDX2) >> I2C_ADDR_MEM_IDX2_Pos);
}

/**
 * @brief  Set the delay values of the PCLK clock cycles between SCL and SDA while data or ACK is driven
 * @param  i2c I2C Instance
 * @param  tsr Delay Values of the PCLK Clock Cycles
 * @retval None
 */
__STATIC_INLINE void LL_I2C_SetTSR(SN_I2C0_Type *i2c, uint8_t tsr)
{
    LL_REG_CBIT(i2c->TGS, I2C_TGS_TSR);
    LL_REG_SBIT(i2c->TGS, (tsr << I2C_TGS_TSR_Pos) & I2C_TGS_TSR);
}

/**
 * @brief  Get the delay values of the PCLK clock cycles between SCL and SDA while data or ACK is driven
 * @param  i2c I2C Instance
 * @retval Delay Values of the PCLK Clock Cycles
 */
__STATIC_INLINE uint32_t LL_I2C_GetTSR(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->TGS, I2C_TGS_TSR) >> I2C_TGS_TSR_Pos);
}

/**
 * @brief  Set the period to suppress glitch is GSR*PCLK clock cycles
 * @param  i2c I2C Instance
 * @param  gsr The Period to Suppress Glitch
 * @retval None
 */
__STATIC_INLINE void LL_I2C_SetGSR(SN_I2C0_Type *i2c, uint8_t gsr)
{
    LL_REG_CBIT(i2c->TGS, I2C_TGS_GSR);
    LL_REG_SBIT(i2c->TGS, (gsr << I2C_TGS_GSR_Pos) & I2C_TGS_GSR);
}

/**
 * @brief  Get the period to suppress glitch is GSR*PCLK clock cycles
 * @param  i2c I2C Instance
 * @retval The Period to Suppress Glitch
 */
__STATIC_INLINE uint32_t LL_I2C_GetGSR(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->TGS, I2C_TGS_GSR) >> I2C_TGS_GSR_Pos);
}

/**
 * @brief  Get the value of the SDA pin
 * @param  i2c I2C Instance
 * @retval The Value of The SDA Pin (1 or 0)
 */
__STATIC_INLINE uint32_t LL_I2C_GetSDAIN(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->BM, I2C_BM_SDAIN) >> I2C_BM_SDAIN_Pos);
}

/**
 * @brief  Get the value of the SCL pin
 * @param  i2c I2C Instance
 * @retval The Value of The SCL Pin (1 or 0)
 */
__STATIC_INLINE uint32_t LL_I2C_GetSCLIN(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->BM, I2C_BM_SCLIN) >> I2C_BM_SCLIN_Pos);
}

/**
 * @brief  Set burst threshold size
 * @param  i2c I2C Instance
 * @param  th Burst Threshold Size
 * @retval None
 */
__STATIC_INLINE void LL_I2C_SetBurstTH(SN_I2C0_Type *i2c, uint8_t th)
{
    uint32_t BSTM = LL_REG_READ(i2c->BSTM);
    LL_REG_CBIT(BSTM, I2C_BSTM_BSTTHOD);
    LL_REG_WRITE(i2c->BSTM, BSTM | ((th << I2C_BSTM_BSTTHOD_Pos) & I2C_BSTM_BSTTHOD));
}

/**
 * @brief  Get burst threshold size
 * @param  i2c I2C Instance
 * @retval Burst Threshold Size
 */
__STATIC_INLINE uint32_t LL_I2C_GetBurstTH(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->BSTM, I2C_BSTM_BSTTHOD) >> I2C_BSTM_BSTTHOD_Pos);
}

/**
 * @brief  Set total burst data count
 * @param  i2c I2C Instance
 * @param  count Burst Threshold Size
 * @retval None
 */
__STATIC_INLINE void LL_I2C_SetBurstCount(SN_I2C0_Type *i2c, uint8_t count)
{
    uint32_t BSTM = LL_REG_READ(i2c->BSTM);
    LL_REG_CBIT(BSTM, I2C_BSTM_TDC);
    LL_REG_WRITE(i2c->BSTM, BSTM | ((count << I2C_BSTM_TDC_Pos) & I2C_BSTM_TDC));
}

/**
 * @brief  Get total burst data count
 * @param  i2c I2C Instance
 * @retval Total Burst Data Count
 */
__STATIC_INLINE uint32_t LL_I2C_GetBurstCount(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->BSTM, I2C_BSTM_TDC) >> I2C_BSTM_TDC_Pos);
}

/**
 * @brief  Get burst buffer size
 * @param  i2c I2C Instance
 * @retval Burst Buffer Size
 */
__STATIC_INLINE uint32_t LL_I2C_GetBurstSize(SN_I2C0_Type *i2c)
{
    return (LL_REG_RBIT(i2c->BSTM, I2C_BSTM_BUFSZ) >> I2C_BSTM_BUFSZ_Pos);
}

#if defined(USE_FULL_LL_DRIVER)
LL_Status_T LL_I2C_Init(SN_I2C0_Type *i2c, LL_I2C_Init_T *i2c_init);
LL_Status_T LL_I2C_DeInit(SN_I2C0_Type *i2c);
void        LL_I2C_StructInit(LL_I2C_Init_T *i2c_init);
#endif /* USE_FULL_LL_DRIVER */

#ifdef __cplusplus
}
#endif

#endif /* _LL_I2C_H_ */
