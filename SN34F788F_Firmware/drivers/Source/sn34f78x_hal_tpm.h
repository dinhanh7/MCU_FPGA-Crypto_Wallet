/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_HAL_TPM_H_
#define _SN34F78X_HAL_TPM_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_hal.h"
/* private types -------------------------------------------------------------*/
/* private constants ---------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
// check whether TPM instance is correct
#define IS_TPM_INSTANCE(HANDLE) ((((HANDLE)->instance) == (void *)SN_CT16B0) || (((HANDLE)->instance) == (void *)SN_CT16B1) || \
                                 (((HANDLE)->instance) == (void *)SN_CT16B2) || (((HANDLE)->instance) == (void *)SN_CT16B3) || \
                                 (((HANDLE)->instance) == (void *)SN_CT16B4) || (((HANDLE)->instance) == (void *)SN_CT16B5) || \
                                 (((HANDLE)->instance) == (void *)SN_CT16B6) || (((HANDLE)->instance) == (void *)SN_CT16B7) || \
                                 (((HANDLE)->instance) == (void *)SN_CT16B8))
// check whether DMA instance is correct
#define IS_DMA_INSTANCE(HANDLE) (((HANDLE->instance) == SN_DMA0) || ((HANDLE->instance) == SN_DMA1))
// check whether TPM PRE is valid
#define IS_TPM_PRE(HANDLE) (HANDLE->init.pre_scaler <= 0xFF)
// check whether TPM mode is valid
#define IS_TPM_MODE(HANDLE) ((HANDLE->init.mode == TPM_CTM_TIMER) || (HANDLE->init.mode == TPM_CTM_COUNTER_RISING) || \
                             (HANDLE->init.mode == TPM_CTM_COUNTER_FALLING) || (HANDLE->init.mode == TPM_CTM_COUNTER_BOTH))
// check whether TPM Counting mode is valid
#define IS_TPM_COUNTING_MODE(HANDLE) ((HANDLE->init.cm == TPM_COUNTING_UP) || (HANDLE->init.cm == TPM_COUNTING_DOWN) ||               \
                                      (HANDLE->init.cm == TPM_COUNTING_CENTER_DOWN) || (HANDLE->init.cm == TPM_COUNTING_CENTER_UP) || \
                                      (HANDLE->init.cm == TPM_COUNTING_CENTER_BOTH))

// check whether channel config is valid
#define IS_OC_VALID_MODE(MODE)          ((MODE >= TPM_OCMODE_TIMER) && (MODE <= TPM_OCMODE_FORCED_HIGH))
#define IS_OC_PERIOD(PREIOD)            (PREIOD <= 0xFFFF)
#define IS_TPM_CHANNEL_COUNTING(CONFIG) ((IS_OC_VALID_MODE(CONFIG->mode)) && (IS_OC_PERIOD(CONFIG->period)) && \
                                         ((CONFIG->stop == HAL_DISABLE) || (CONFIG->stop == HAL_ENABLE)) &&    \
                                         ((CONFIG->reset == HAL_DISABLE) || (CONFIG->reset == HAL_ENABLE)))

// check whether is PWM mode
#define IS_PWM_MODE(MODE) ((MODE == TPM_OCMODE_PWM1) || (MODE == TPM_OCMODE_PWM2))

// check whether is OC mode
#define IS_OC_MODE(MODE) ((MODE == TPM_OCMODE_TIMER) || (MODE == TPM_OCMODE_LOW) ||   \
                          (MODE == TPM_OCMODE_HIGH) || (MODE == TPM_OCMODE_TOGGLE) || \
                          (MODE == TPM_OCMODE_FORCED_LOW) || (MODE == TPM_OCMODE_FORCED_HIGH))

// check PWN Mode 2 in center mode
#define CHECK_PWM2_IN_CENTER_MODE(CM_MODE, PWN_MODE) ((CM_MODE >= TPM_COUNTING_CENTER_DOWN) && (PWN_MODE == TPM_OCMODE_PWM2))

// check whether capture config is valid
#define IS_TPM_CAP(CAP) (((CAP->rising <= HAL_DISABLE) || (CAP->rising == HAL_ENABLE)) && \
                         ((CAP->falling == HAL_DISABLE) || (CAP->falling == HAL_ENABLE)))

// check whether instance support Counting mode
#define CHECK_SUPPORT_COUNTING_MODE(HANDLE) ((function_table[(HANDLE)->tpm_index] & TPM_CM) || (!(HANDLE)->init.cm))

// check whether instance support Counter/Timer mode
#define CHECK_SUPPORT_CTM(HANDLE) ((function_table[(HANDLE)->tpm_index] & TPM_CTM) || (!(HANDLE)->init.mode))

// check whether TPM channel is valid
#define IS_TPM_CHANNEL(CHANNEL) (CHANNEL <= TPM_CHANNEL_PERIOD)

// check whether PWMN config is valid
#define IS_TPM_PWMN_COUNTING(CONFIG) ((CONFIG->mode <= TPM_PWMN_MODE_2) && (CONFIG->db <= 0x3FF))

// check whether break function config is valid
#define IS_TPM_BREAK_COUNTING(CONFIG) (((CONFIG->break_mode == TPM_BREAK_DISABLE) || (CONFIG->break_mode == TPM_BREAK_PIN_LOW) || (CONFIG->break_mode == TPM_BREAK_PIN_HIGH)) && \
                                       (CONFIG->break_db <= TPM_BREAK_128_PCLK) &&                                                                                               \
                                       ((CONFIG->break_ie == HAL_DISABLE) || (CONFIG->break_ie == HAL_ENABLE)))

// check whether load mode config is valid
#define IS_TPM_LOAD_MODE_CONFIG(CONFIG) ((CONFIG->channel <= TPM_CHANNEL_3) && ((CONFIG->load_mode == TPM_NO_EFFECT) || (CONFIG->load_mode == TPM_LOAD_TC_EQ_0) || (CONFIG->load_mode == TPM_LOAD_TC_EQ_0_MR9)))

// channel info
#define CH_STATE_RESET 0x00
#define CH_STATE_READY 0x01
#define CH_STATE_BUSY  0x02

#define WRITE_CH_STATE(INFO, CHANNEL, VALUE)   \
    HAL_FLAG_CLR(INFO, 0x03 << (CHANNEL * 2)); \
    HAL_FLAG_SET(INFO, VALUE << (CHANNEL * 2))

#define GET_CH_ID(CHANNEL) (CHANNEL < TPM_CHANNEL_8 ? CHANNEL : CHANNEL - TPM_CHANNEL_8)
#define WRITE_CH_MODE(INFO, CHANNEL, VALUE)               \
    HAL_FLAG_CLR(INFO, 0x0F << (GET_CH_ID(CHANNEL) * 4)); \
    HAL_FLAG_SET(INFO, VALUE << (GET_CH_ID(CHANNEL) * 4))

#define WRITE_CH_PWMN_MODE(INFO, CHANNEL, VALUE) \
    HAL_FLAG_CLR(INFO, 0x03 << (CHANNEL * 2));   \
    HAL_FLAG_SET(INFO, VALUE << (CHANNEL * 2))

#define GET_CH_STATE(VALUE, CHANNEL)     (VALUE & (0x03 << (CHANNEL * 2))) >> (CHANNEL * 2)
#define GET_CH_MODE(VALUE, CHANNEL)      (VALUE & (0x0F << (GET_CH_ID(CHANNEL) * 4))) >> (GET_CH_ID(CHANNEL) * 4)
#define GET_CH_PWMN_MODE(VALUE, CHANNEL) (VALUE & (0x03 << (CHANNEL * 2))) >> (CHANNEL * 2)

// check channel in break : channel mode is not Timer , channel state is busy, channel output pin is disable
#define CH_IN_BREAK(HANDLE, CHANNEL) ((!HAL_FLAG_EQU((htpm->channel_info.mode[CHANNEL / TPM_CHANNEL_8] & (0x03 << (GET_CH_ID(CHANNEL) * 4))) >> (GET_CH_ID(CHANNEL) * 4), CH_TIMER)) && \
                                      HAL_FLAG_EQU((HANDLE->channel_info.state & (0x03 << (CHANNEL * 2))) >> (CHANNEL * 2), CH_STATE_BUSY) &&                                           \
                                      (HAL_REG_RBIT(*REG_PWMCTRL(htpm), 1 << (20 + CHANNEL)) == 0))

// define TPM index
#define CT16B0 0x00U
#define CT16B1 0x01U
#define CT16B2 0x02U
#define CT16B3 0x03U
#define CT16B4 0x04U
#define CT16B5 0x05U
#define CT16B6 0x06U
#define CT16B7 0x07U
#define CT16B8 0x08U

#define IS_CT16B8(HANDLE) (((HANDLE)->tpm_index) == CT16B8)

// define REG OFFSET
#define REG_TMRCTRL(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x00))
#define REG_TC(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x04))
#define REG_PRE(HANDLE)     ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x08))
#define REG_PC(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x0C))
#define REG_CNTCTRL(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x10))

#define REG_MCTRL(HANDLE)  ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x14))
#define REG_MCTRL2(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x18))

#define REG_MR0(HANDLE)      ((uint32_t *)(((uint8_t *)((HANDLE)->instance)) + ((IS_CT16B8(HANDLE)) ? 0x1C : 0x18)))
#define REG_MR1(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x20 : 0x1C))))
#define REG_MR2(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x24 : 0x20))))
#define REG_MR3(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x28 : 0x24))))
#define REG_MR4(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x2C))
#define REG_MR5(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x30))
#define REG_MR6(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x34))
#define REG_MR7(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x38))
#define REG_MR8(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x3C))
#define REG_MR9(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x40))
#define REG_MR10(HANDLE)     ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x44))
#define REG_MR11(HANDLE)     ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x48))
#define REG_MRPERIOD(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x4C))

#define REG_MR0_ALIAS(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0xD4))
#define REG_MR1_ALIAS(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0xD6))
#define REG_MR2_ALIAS(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0xD8))
#define REG_MR3_ALIAS(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0xDA))

#define REG_CAP_CONTROL(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x28))
#define REG_CAP0(HANDLE)        ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x2C))

#define REG_EM(HANDLE)  ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x58 : 0x30))))
#define REG_EMC(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x5C : 0x30))))

#define REG_PWMCTRL(HANDLE)       ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x60 : 0x34))))
#define REG_PWM_ENABLE(HANDLE)    ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x64 : 0x34))))
#define REG_PWM_IO_ENABLE(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x68 : 0x34))))

#define REG_RIS(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x6C : 0x38))))
#define REG_IC(HANDLE)  ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + ((IS_CT16B8(HANDLE) ? 0x70 : 0x3C))))

#define REG_PWM0NDB(HANDLE)  ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x44))
#define REG_PWM1NDB(HANDLE)  ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x48))
#define REG_PWM2NDB(HANDLE)  ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x4C))
#define REG_PWM3NDB(HANDLE)  ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x50))
#define REG_DMA(HANDLE)      ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0xD0))
#define REG_LOADCTRL(HANDLE) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0x74))
#define REG_BRKCTRL(HANDLE)  ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + 0xE0))

#define REG_MR(HANDLE, CHANNEL) ((CHANNEL == TPM_CHANNEL_9) ? REG_MR9(HANDLE) : ((REG_MR0(HANDLE) + CHANNEL)))

#define PWM_EMC_MODE_BIT(HANDLE, CHANNEL) (IS_CT16B8(HANDLE) ? (2 * CHANNEL) : (4 + 2 * CHANNEL))
#define PWM_IO_EN_BIT(HANDLE, CHANNEL)    (IS_CT16B8(HANDLE) ? CHANNEL : (20 + CHANNEL))

#define GET_DMA_ID(CHANNEL)            ((CHANNEL > TPM_CHANNEL_3) ? (CHANNEL / 3 + 1) : CHANNEL)
#define GET_OFFSET_REG(HANDLE, OFFSET) ((uint32_t *)(((uint8_t *)(HANDLE)->instance) + OFFSET))

#define HAL_REG_WBIT(reg, bit, value) \
    HAL_REG_CBIT(reg, bit);           \
    HAL_REG_SBIT(reg, value)

// channel function
#define CH_TIMER      0x01 // support channel
#define CH_OC         0x02 // support channel output(EMC/FORCED)
#define CH_PWM        0x04 // support channel output(EMC/FORCED)
#define CH_PWMN       0x08 // support PWMN
#define CH_LOAD       0x10 // support load mode
#define CH_DMA        0x20 // support DMA
#define CH_REG_UPDATE 0x40 // support dynamic set value after CEN = 1

// TPM function
#define TPM_CAPTURE0 0x01 // support capture
#define TPM_BREAK    0x02 // support break function
#define TPM_CM       0x04 // support set counting mode
#define TPM_CTM      0x08 // support Counter/Timer mode

// TPM channel table mask
#define CH_FUNC_MASK                    0x00FF
#define CH_MCTRL_MASK                   0xFF00
#define GET_CHANNEL_MCTRL(CHANNEL_INFO) ((CHANNEL_INFO & CH_MCTRL_MASK) >> 8)
#define MCTRL_OFFSET(x)                 (x << 8)

// TPM DMA table mask
#define DMA_REQ_MASK           0x00FF
#define DMA_ADDR_MASK          0xFF00
#define GET_DMA_REQ(DMA_INFO)  (DMA_INFO & DMA_REQ_MASK)
#define GET_DMA_ADDR(DMA_INFO) ((DMA_INFO & DMA_ADDR_MASK) >> 8)
#define DMA_ADDR_OFFSET(x)     (x << 8)

// TPM IQR table mask
#define IRQ_IE_REG_MASK              0x000000FF
#define IRQ_IE_BIT_MASK              0x0000FF00
#define IRQ_RIS_IC_BIT_MASK          0x00FF0000
#define IRQ_FUNC_ID_MASK             0x0F000000
#define IRQ_CH_MASK                  0xF0000000
#define GET_IRQ_IE_REG(IRQ_INFO)     (IRQ_INFO & IRQ_IE_REG_MASK)
#define GET_IRQ_IE_BIT(IRQ_INFO)     ((IRQ_INFO & IRQ_IE_BIT_MASK) >> 8)
#define GET_IRQ_RIS_IC_BIT(IRQ_INFO) ((IRQ_INFO & IRQ_RIS_IC_BIT_MASK) >> 16)
#define GET_IRQ_FUNC_ID(IRQ_INFO)    ((IRQ_INFO & IRQ_FUNC_ID_MASK) >> 24)
#define GET_IRQ_CH(IRQ_INFO)         ((IRQ_INFO & IRQ_CH_MASK) >> 28)
#define IRQ_IE_OFFSET(x)             (x << 8)
#define IRQ_RIS_IC_OFFSET(x)         (x << 16)
#define IRQ_FUNC_ID_OFFSET(x)        (x << 24)
#define IQR_CH_OFFSET(x)             (x << 28)

#define CHECK_IRQ_IE(HANDLE, IRQ_VALUE) (HAL_REG_RBIT(*GET_OFFSET_REG(HANDLE, GET_IRQ_IE_REG(IRQ_VALUE)), 1U << GET_IRQ_IE_BIT(IRQ_VALUE)))
#define CHECK_IRQ_IF(HANDLE, IRQ_VALUE) (HAL_REG_RBIT(*REG_RIS(HANDLE), 1U << GET_IRQ_RIS_IC_BIT(IRQ_VALUE)))
/* private variables ---------------------------------------------------------*/
// bit 0 ~ 7 : channel support function
// bit 8 ~ 15 : channel match control offset
static const uint16_t channel_table[][13] = {
    //  CT16Bn  CH0                                                                                       CH1                                                                                       CH2                                                                                       CH3                                                                                       CH4                                                           CH5                                                           CH6                                                           CH7                                                           CH8                                                             CH9                                                           CH10                                                        CH11                                                         CH_PRE
    [CT16B0] = {MCTRL_OFFSET(0) | CH_TIMER | CH_OC | CH_PWM | CH_PWMN | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(3) | CH_TIMER | CH_OC | CH_PWM | CH_PWMN | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(6) | CH_TIMER | CH_OC | CH_PWM | CH_PWMN | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(9) | CH_TIMER | CH_OC | CH_PWM | CH_PWMN | CH_LOAD | CH_DMA | CH_REG_UPDATE, 0, 0, 0, 0, 0, MCTRL_OFFSET(29) | CH_TIMER, 0, 0, 0},
    [CT16B1] = {MCTRL_OFFSET(0) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(3) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(6) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(9) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, 0, 0, 0, 0, 0, MCTRL_OFFSET(29) | CH_TIMER, 0, 0, 0},
    [CT16B2] = {MCTRL_OFFSET(0) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(3) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(6) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(9) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, 0, 0, 0, 0, 0, MCTRL_OFFSET(29) | CH_TIMER, 0, 0, 0},
    [CT16B3] = {MCTRL_OFFSET(0) | CH_TIMER | CH_OC | CH_PWM | CH_PWMN | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(3) | CH_TIMER | CH_OC | CH_PWM | CH_PWMN | CH_DMA | CH_REG_UPDATE, 0, 0, 0, 0, 0, 0, 0, MCTRL_OFFSET(29) | CH_TIMER, 0, 0, 0},
    [CT16B4] = {MCTRL_OFFSET(0) | CH_TIMER | CH_OC | CH_PWM | CH_PWMN | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(3) | CH_TIMER | CH_OC | CH_PWM | CH_PWMN | CH_DMA | CH_REG_UPDATE, 0, 0, 0, 0, 0, 0, 0, MCTRL_OFFSET(29) | CH_TIMER, 0, 0, 0},
    [CT16B5] = {MCTRL_OFFSET(0) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(3) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(6) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, MCTRL_OFFSET(9) | CH_TIMER | CH_OC | CH_PWM | CH_LOAD | CH_DMA | CH_REG_UPDATE, 0, 0, 0, 0, 0, MCTRL_OFFSET(29) | CH_TIMER, 0, 0, 0},
    [CT16B6] = {CH_TIMER | CH_DMA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    [CT16B7] = {CH_TIMER | CH_DMA, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    [CT16B8] = {MCTRL_OFFSET(0) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(3) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(6) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(9) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(12) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(15) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(18) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(21) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(24) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(27) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(0) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(3) | CH_TIMER | CH_OC | CH_PWM | CH_REG_UPDATE, MCTRL_OFFSET(29) | CH_TIMER},
};

static const uint8_t function_table[] = {
    TPM_CAPTURE0 | TPM_BREAK | TPM_CM | TPM_CTM, // CT16B0
    TPM_CAPTURE0 | TPM_CM | TPM_CTM,             // CT16B1
    TPM_CAPTURE0 | TPM_CM | TPM_CTM,             // CT16B2
    TPM_CAPTURE0,                                // CT16B3
    TPM_CAPTURE0 | TPM_CTM,                      // CT16B4
    TPM_CAPTURE0 | TPM_CM | TPM_CTM,             // CT16B5
    0,                                           // CT16B6
    0,                                           // CT16B7
    0,                                           // CT16B8
};

// bit 0 ~ 7 : dma request
// bit 8 ~ 15: dma address
static const uint16_t dma_table[][7] = {
    //  CT16Bn       CH0                         CH1                         CH2                         CH3                         CH9                         CH_PERIOD                   CAP0
    [CT16B0] = {DMA_ADDR_OFFSET(0xD4) | 0, DMA_ADDR_OFFSET(0xD6) | 1, DMA_ADDR_OFFSET(0xD8) | 2, DMA_ADDR_OFFSET(0xDA) | 3, DMA_ADDR_OFFSET(0x40) | 5, 0, DMA_ADDR_OFFSET(0x2C) | 4},
    [CT16B1] = {DMA_ADDR_OFFSET(0xD4) | 0, DMA_ADDR_OFFSET(0xD6) | 1, DMA_ADDR_OFFSET(0xD8) | 2, DMA_ADDR_OFFSET(0xDA) | 3, DMA_ADDR_OFFSET(0x40) | 5, 0, DMA_ADDR_OFFSET(0x2C) | 4},
    [CT16B2] = {DMA_ADDR_OFFSET(0xD4) | 0, DMA_ADDR_OFFSET(0xD6) | 1, DMA_ADDR_OFFSET(0xD8) | 2, DMA_ADDR_OFFSET(0xDA) | 3, DMA_ADDR_OFFSET(0x40) | 5, 0, DMA_ADDR_OFFSET(0x2C) | 4},
    [CT16B3] = {DMA_ADDR_OFFSET(0xD4) | 0, DMA_ADDR_OFFSET(0xD6) | 1, 0, 0, DMA_ADDR_OFFSET(0x40) | 3, 0, 0},
    [CT16B4] = {DMA_ADDR_OFFSET(0xD4) | 0, DMA_ADDR_OFFSET(0xD6) | 1, 0, 0, DMA_ADDR_OFFSET(0x40) | 3, 0, DMA_ADDR_OFFSET(0x2C) | 2},
    [CT16B5] = {DMA_ADDR_OFFSET(0xD4) | 0, DMA_ADDR_OFFSET(0xD6) | 1, DMA_ADDR_OFFSET(0xD8) | 2, DMA_ADDR_OFFSET(0xDA) | 3, DMA_ADDR_OFFSET(0x40) | 5, 0, DMA_ADDR_OFFSET(0x2C) | 4},
    [CT16B6] = {DMA_ADDR_OFFSET(0x18) | 0, 0, 0, 0, 0, 0, 0},
    [CT16B7] = {DMA_ADDR_OFFSET(0x18) | 0, 0, 0, 0, 0, 0, 0},
    [CT16B8] = {0, 0, 0, 0, 0, DMA_ADDR_OFFSET(0x4C) | 13, 0},
};

// bit 0 ~ 7: ie register address offset
// bit 8 ~ 15: irq ie bit
// bit 16 ~ 23 : irq ris&ic bit
// bit 24 ~ 27 : irq function index
// bit 28 ~ 31 : active channel
static const uint32_t tpm_0_1_2_5_irq_table[] = {
    IQR_CH_OFFSET(TPM_CHANNEL_0) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(0) | IRQ_IE_OFFSET(0) | 0x14,         // MR0
    IQR_CH_OFFSET(TPM_CHANNEL_1) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(1) | IRQ_IE_OFFSET(3) | 0x14,         // MR1
    IQR_CH_OFFSET(TPM_CHANNEL_2) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(2) | IRQ_IE_OFFSET(6) | 0x14,         // MR2
    IQR_CH_OFFSET(TPM_CHANNEL_3) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(3) | IRQ_IE_OFFSET(9) | 0x14,         // MR3
    IQR_CH_OFFSET(TPM_CHANNEL_CLEARED) | IRQ_FUNC_ID_OFFSET(1) | IRQ_RIS_IC_OFFSET(4) | IRQ_IE_OFFSET(2) | 0x28,   // CAP
    IQR_CH_OFFSET(TPM_CHANNEL_9) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(5) | IRQ_IE_OFFSET(29) | 0x14,        // MR9
    IQR_CH_OFFSET(TPM_CHANNEL_CLEARED) | IRQ_FUNC_ID_OFFSET(2) | IRQ_RIS_IC_OFFSET(31) | IRQ_IE_OFFSET(30) | 0xE0, // Break
};

// bit 0 ~ 7: ie register address offset
// bit 8 ~ 15: irq ie bit
// bit 16 ~ 23 : irq ris&ic bit
// bit 24 ~ 27 : irq function index
// bit 28 ~ 31 : active channel
static const uint32_t _tpm_3_irq_table[] = {
    IQR_CH_OFFSET(TPM_CHANNEL_0) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(0) | IRQ_IE_OFFSET(0) | 0x14,  // MR0
    IQR_CH_OFFSET(TPM_CHANNEL_1) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(1) | IRQ_IE_OFFSET(3) | 0x14,  // MR1
    IQR_CH_OFFSET(TPM_CHANNEL_9) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(3) | IRQ_IE_OFFSET(29) | 0x14, // MR9
};

// bit 0 ~ 7: ie register address offset
// bit 8 ~ 15: irq ie bit
// bit 16 ~ 23 : irq ris&ic bit
// bit 24 ~ 27 : irq function index
// bit 28 ~ 31 : active channel
static const uint32_t _tpm_4_irq_table[] = {
    IQR_CH_OFFSET(TPM_CHANNEL_0) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(0) | IRQ_IE_OFFSET(0) | 0x14,       // MR0
    IQR_CH_OFFSET(TPM_CHANNEL_1) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(1) | IRQ_IE_OFFSET(3) | 0x14,       // MR1
    IQR_CH_OFFSET(TPM_CHANNEL_CLEARED) | IRQ_FUNC_ID_OFFSET(1) | IRQ_RIS_IC_OFFSET(2) | IRQ_IE_OFFSET(2) | 0x28, // CAP
    IQR_CH_OFFSET(TPM_CHANNEL_9) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(3) | IRQ_IE_OFFSET(29) | 0x14,      // MR9
};

// bit 0 ~ 7: ie register address offset
// bit 8 ~ 15: irq ie bit
// bit 16 ~ 23 : irq ris&ic bit
// bit 24 ~ 27 : irq function index
// bit 28 ~ 31 : active channel
static const uint32_t tpm_8_irq_table[] = {
    IQR_CH_OFFSET(TPM_CHANNEL_0) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(0) | IRQ_IE_OFFSET(0) | 0x14,        // MR0
    IQR_CH_OFFSET(TPM_CHANNEL_1) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(1) | IRQ_IE_OFFSET(3) | 0x14,        // MR1
    IQR_CH_OFFSET(TPM_CHANNEL_2) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(2) | IRQ_IE_OFFSET(6) | 0x14,        // MR2
    IQR_CH_OFFSET(TPM_CHANNEL_3) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(3) | IRQ_IE_OFFSET(9) | 0x14,        // MR3
    IQR_CH_OFFSET(TPM_CHANNEL_4) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(4) | IRQ_IE_OFFSET(12) | 0x14,       // MR4
    IQR_CH_OFFSET(TPM_CHANNEL_5) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(5) | IRQ_IE_OFFSET(15) | 0x14,       // MR5
    IQR_CH_OFFSET(TPM_CHANNEL_6) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(6) | IRQ_IE_OFFSET(18) | 0x14,       // MR6
    IQR_CH_OFFSET(TPM_CHANNEL_7) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(7) | IRQ_IE_OFFSET(21) | 0x14,       // MR7
    IQR_CH_OFFSET(TPM_CHANNEL_8) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(8) | IRQ_IE_OFFSET(24) | 0x14,       // MR8
    IQR_CH_OFFSET(TPM_CHANNEL_9) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(9) | IRQ_IE_OFFSET(27) | 0x14,       // MR9
    IQR_CH_OFFSET(TPM_CHANNEL_10) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(10) | IRQ_IE_OFFSET(0) | 0x18,      // MR10
    IQR_CH_OFFSET(TPM_CHANNEL_11) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(11) | IRQ_IE_OFFSET(3) | 0x18,      // MR11
    IQR_CH_OFFSET(TPM_CHANNEL_PERIOD) | IRQ_FUNC_ID_OFFSET(0) | IRQ_RIS_IC_OFFSET(13) | IRQ_IE_OFFSET(29) | 0x18, // MR_PERIOD
};

/* Private function ----------------------------------------------------------*/
static void    _tpm_set_index(TPM_Handle_T *htpm);
static void    _reset_base(TPM_Handle_T *htpm);
static uint8_t _tpm_is_busy(TPM_Handle_T *htpm);
static void    _tpm_disable(TPM_Handle_T *htpm);

static void _config_channel_base(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel);
static void _config_channel_pwm(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel);
static void _config_channel_oc(TPM_Handle_T *htpm, TPM_OC_T *config, uint32_t channel);

static HAL_Status_T _start_channel_base(TPM_Handle_T *htpm, uint32_t channel, uint8_t ie);
static HAL_Status_T _start_channel_pwm(TPM_Handle_T *htpm, uint32_t channel, uint8_t ie);
static HAL_Status_T _start_channel_oc(TPM_Handle_T *htpm, uint32_t channel, uint8_t ie);

static HAL_Status_T _stop_channel_base(TPM_Handle_T *htpm, uint32_t channel);
static HAL_Status_T _stop_channel_pwm(TPM_Handle_T *htpm, uint32_t channel);
static HAL_Status_T _stop_channel_oc(TPM_Handle_T *htpm, uint32_t channel);

static HAL_Status_T _start_channel_dma(TPM_Handle_T *htpm, uint32_t channel, uint32_t *data, uint32_t length);
static HAL_Status_T _stop_channel_dma(TPM_Handle_T *htpm, uint32_t channel);

static HAL_Status_T _start_capture(TPM_Handle_T *htpm, uint8_t ie);
static HAL_Status_T _stop_capture(TPM_Handle_T *htpm);

static void _tpm_irq_kernel(TPM_Handle_T *htpm, const uint32_t *irq_table, uint8_t irq_size);
static void _tpm_0_1_2_5_irq(TPM_Handle_T *htpm);
static void _tpm_3_irq(TPM_Handle_T *htpm);
static void _tpm_4_irq(TPM_Handle_T *htpm);
static void _tpm_6_7_irq(TPM_Handle_T *htpm);
static void _tpm_8_irq(TPM_Handle_T *htpm);

static void _cap_irq(TPM_Handle_T *htpm, uint8_t ic_bit);
static void _mr_irq(TPM_Handle_T *htpm, uint8_t ic_bit);
static void _break_irq(TPM_Handle_T *htpm, uint8_t ic_bit);

#if (configUSE_TPM_REGISTER_CALLBACKS == 1)
static void _reset_callback(TPM_Handle_T *htpm);
#endif

#if (configUSE_TPM_DMA == 1)
static void     _register_dma_callback(TPM_Handle_T *htpm);
static void     _tpm_dma_xfer_delay_pulse_callback(DMA_Handle_T *hdma);
static void     _tpm_dma_xfer_capture_callback(DMA_Handle_T *hdma);
static void     _tpm_dma_xfer_error_callback(DMA_Handle_T *hdma);
static uint32_t _get_ch_ic_bit(TPM_Handle_T *htpm, uint8_t channel);
#endif

typedef void (*TPM_IRQ)(TPM_Handle_T *);

static TPM_IRQ tpm_irq[] = {
    _tpm_0_1_2_5_irq,
    _tpm_0_1_2_5_irq,
    _tpm_0_1_2_5_irq,
    _tpm_3_irq,
    _tpm_4_irq,
    _tpm_0_1_2_5_irq,
    _tpm_6_7_irq,
    _tpm_6_7_irq,
    _tpm_8_irq,
};

typedef void (*FUNC_IRQ)(TPM_Handle_T *, uint8_t);

static FUNC_IRQ func_irq[] = {
    _mr_irq,
    _cap_irq,
    _break_irq,
};

#endif /* _SN34F78X_HAL_TPM_H_ */
