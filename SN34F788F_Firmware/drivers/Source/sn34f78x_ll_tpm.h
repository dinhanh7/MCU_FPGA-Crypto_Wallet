/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _SN34F78X_LL_TPM_H_
#define _SN34F78X_LL_TPM_H_

/* Includes ------------------------------------------------------------------*/
#include "sn34f78x_ll.h"

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
// check whether TPM instance is correct
#define IS_LL_TPM_INSTANCE(instance) ((instance == (void *)SN_CT16B0) || (instance == (void *)SN_CT16B1) || \
                                      (instance == (void *)SN_CT16B2) || (instance == (void *)SN_CT16B3) || \
                                      (instance == (void *)SN_CT16B4) || (instance == (void *)SN_CT16B5) || \
                                      (instance == (void *)SN_CT16B6) || (instance == (void *)SN_CT16B7) || \
                                      (instance == (void *)SN_CT16B8))

// check whether TPM PRE is valid
#define IS_LL_TPM_PRE(pre_scaler) (pre_scaler <= 0xFF)

// check whether TPM ctm is valid
#define IS_LL_TPM_MODE(ctm) ((ctm == TPM_01245_CNTCTRL_CTM_EveryRisingEdge) || (ctm == TPM_01245_CNTCTRL_CTM_RisingEdgeCAP0) || \
                             (ctm == TPM_01245_CNTCTRL_CTM_FallingEdgeCAP0) || (ctm == TPM_01245_CNTCTRL_CTM_BothEdgeCAP0))

// check whether TPM Counting mode is valid
#define IS_LL_TPM_COUNTING_MODE(cm) ((cm == TPM_0125_TMRCTRL_CM_EdgeUpCounting) || (cm == TPM_0125_TMRCTRL_CM_EdgeDownCounting) ||       \
                                     (cm == TPM_0125_TMRCTRL_CM_DownCountingSetFlag) || (cm == TPM_0125_TMRCTRL_CM_UpCountingSetFlag) || \
                                     (cm == TPM_0125_TMRCTRL_CM_UpDownCountingSetFlag))

// check whether TPM channel is valid
#define IS_LL_TPM_CHANNEL(CHANNEL) (CHANNEL <= LL_TPM_CHANNEL_PERIOD)

// check whether channel config is valid
#define IS_LL_OC_VALID_MODE(MODE)          ((MODE >= LL_TPM_OCMODE_TIMER) && (MODE <= LL_TPM_OCMODE_FORCED_HIGH))
#define IS_LL_OC_PERIOD(PREIOD)            (PREIOD <= 0xFFFF)
#define IS_LL_TPM_CHANNEL_COUNTING(CONFIG) ((IS_LL_OC_VALID_MODE(CONFIG->mode)) && (IS_LL_OC_PERIOD(CONFIG->period)) && \
                                            ((CONFIG->stop == 0) || (CONFIG->stop == 1)) &&                             \
                                            ((CONFIG->reset == 0) || (CONFIG->reset == 1)))

// check whether is PWM mode
#define IS_PWM_MODE(MODE) ((MODE == LL_TPM_OCMODE_PWM1) || (MODE == LL_TPM_OCMODE_PWM2))

// check whether is OC mode
#define IS_OC_MODE(MODE) ((MODE == LL_TPM_OCMODE_TIMER) || (MODE == LL_TPM_OCMODE_LOW) ||   \
                          (MODE == LL_TPM_OCMODE_HIGH) || (MODE == LL_TPM_OCMODE_TOGGLE) || \
                          (MODE == LL_TPM_OCMODE_FORCED_LOW) || (MODE == LL_TPM_OCMODE_FORCED_HIGH))

// check PWN Mode 2 in center mode
#define CHECK_PWM2_IN_CENTER_MODE(CM_MODE, PWN_MODE) ((CM_MODE != LL_TPM0_CM_COUNTING_UP) && (CM_MODE != LL_TPM0_CM_COUNTING_DOWN) &&     \
                                                      (CM_MODE != LL_TPM125_CM_COUNTING_UP) && (CM_MODE != LL_TPM125_CM_COUNTING_DOWN) && \
                                                      (PWN_MODE == LL_TPM_OCMODE_PWM2))

// check whether capture config is valid
#define IS_LL_TPM_CAP(CAP) (((CAP->rising == 0) || (CAP->rising == 1)) && \
                            ((CAP->falling == 0) || (CAP->falling == 1)))

// check whether is OC mode
#define IS_LL_OC_MODE(MODE) ((MODE == LL_TPM_OCMODE_TIMER) || (MODE == LL_TPM_OCMODE_LOW) ||   \
                             (MODE == LL_TPM_OCMODE_HIGH) || (MODE == LL_TPM_OCMODE_TOGGLE) || \
                             (MODE == LL_TPM_OCMODE_FORCED_LOW) || (MODE == LL_TPM_OCMODE_FORCED_HIGH))

// check whether PWMN config is valid
#define IS_LL_TPM_PWMN_DB(db) (db <= 0x3FF)

// check whether break function config is valid
#define IS_LL_TPM_BREAK_COUNTING(CONFIG) (((CONFIG->break_mode == LL_TPM_BREAK_DISABLE) || (CONFIG->break_mode == LL_TPM_BREAK_PIN_LOW) || (CONFIG->break_mode == LL_TPM_BREAK_PIN_HIGH)) && \
                                          (CONFIG->break_db <= LL_TPM_BREAK_128_PCLK) &&                                                                                                     \
                                          ((CONFIG->break_ie == LL_DISABLE) || (CONFIG->break_ie == LL_ENABLE)))

// channel function
#define LL_CH_TIMER      0x01 // support channel
#define LL_CH_OC         0x02 // support channel output(EMC/FORCED)
#define LL_CH_PWM        0x04 // support channel output(EMC/FORCED)
#define LL_CH_PWMN       0x08 // support PWMN
#define LL_CH_LOAD       0x10 // support load mode
#define LL_CH_DMA        0x20 // support DMA
#define LL_CH_REG_UPDATE 0x40 // support dynamic set value after CEN = 1

// TPM function
#define LL_TPM_CAPTURE0 0x01 // support capture
#define LL_TPM_BREAK    0x02 // support break function
#define LL_TPM_CM       0x04 // support set counting mode
#define LL_TPM_CTM      0x08 // support Counter/Timer mode

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

#define IS_CT16B8(instance) (_get_tpm_index(instance) == CT16B8)

// define REG OFFSET
#define REG_TMRCTRL(instance) ((uint32_t *)((uint8_t *)instance + 0x00))
#define REG_TC(instance)      ((uint32_t *)((uint8_t *)instance + 0x04))
#define REG_PRE(instance)     ((uint32_t *)((uint8_t *)instance + 0x08))
#define REG_PC(instance)      ((uint32_t *)((uint8_t *)instance + 0x0C))
#define REG_CNTCTRL(instance) ((uint32_t *)((uint8_t *)instance + 0x10))

#define REG_MCTRL(instance)  ((uint32_t *)((uint8_t *)instance + 0x14))
#define REG_MCTRL2(instance) ((uint32_t *)((uint8_t *)instance + 0x18))

#define REG_MR0(instance)      ((uint32_t *)(((uint8_t *)(instance)) + ((IS_CT16B8(instance)) ? 0x1C : 0x18)))
#define REG_MR1(instance)      ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x20 : 0x1C))))
#define REG_MR2(instance)      ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x24 : 0x20))))
#define REG_MR3(instance)      ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x28 : 0x24))))
#define REG_MR4(instance)      ((uint32_t *)((uint8_t *)instance + 0x2C))
#define REG_MR5(instance)      ((uint32_t *)((uint8_t *)instance + 0x30))
#define REG_MR6(instance)      ((uint32_t *)((uint8_t *)instance + 0x34))
#define REG_MR7(instance)      ((uint32_t *)((uint8_t *)instance + 0x38))
#define REG_MR8(instance)      ((uint32_t *)((uint8_t *)instance + 0x3C))
#define REG_MR9(instance)      ((uint32_t *)((uint8_t *)instance + 0x40))
#define REG_MR10(instance)     ((uint32_t *)((uint8_t *)instance + 0x44))
#define REG_MR11(instance)     ((uint32_t *)((uint8_t *)instance + 0x48))
#define REG_MRPERIOD(instance) ((uint32_t *)((uint8_t *)instance + 0x4C))

#define REG_MR0_ALIAS(instance) ((uint32_t *)((uint8_t *)instance + 0xD4))
#define REG_MR1_ALIAS(instance) ((uint32_t *)((uint8_t *)instance + 0xD6))
#define REG_MR2_ALIAS(instance) ((uint32_t *)((uint8_t *)instance + 0xD8))
#define REG_MR3_ALIAS(instance) ((uint32_t *)((uint8_t *)instance + 0xDA))

#define REG_CAP_CONTROL(instance) ((uint32_t *)((uint8_t *)instance + 0x28))
#define REG_CAP0(instance)        ((uint32_t *)((uint8_t *)instance + 0x2C))

#define REG_EM(instance)  ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x58 : 0x30))))
#define REG_EMC(instance) ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x5C : 0x30))))

#define REG_PWMCTRL(instance)       ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x60 : 0x34))))
#define REG_PWM_ENABLE(instance)    ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x64 : 0x34))))
#define REG_PWM_IO_ENABLE(instance) ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x68 : 0x34))))

#define REG_RIS(instance) ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x6C : 0x38))))
#define REG_IC(instance)  ((uint32_t *)((uint8_t *)instance + ((IS_CT16B8(instance) ? 0x70 : 0x3C))))

#define REG_PWM0NDB(instance)  ((uint32_t *)((uint8_t *)instance + 0x44))
#define REG_PWM1NDB(instance)  ((uint32_t *)((uint8_t *)instance + 0x48))
#define REG_PWM2NDB(instance)  ((uint32_t *)((uint8_t *)instance + 0x4C))
#define REG_PWM3NDB(instance)  ((uint32_t *)((uint8_t *)instance + 0x50))
#define REG_DMA(instance)      ((uint32_t *)((uint8_t *)instance + 0xD0))
#define REG_LOADCTRL(instance) ((uint32_t *)((uint8_t *)instance + 0x74))
#define REG_BRKCTRL(instance)  ((uint32_t *)((uint8_t *)instance + 0xE0))

#define REG_MR(instance, CHANNEL) ((CHANNEL == LL_TPM_CHANNEL_9) ? REG_MR9(instance) : ((REG_MR0(instance) + CHANNEL)))

#define PWM_EMC_MODE_BIT(instance, CHANNEL) (IS_CT16B8(instance) ? (2 * CHANNEL) : (4 + 2 * CHANNEL))
#define PWM_IO_EN_BIT(instance, CHANNEL)    (IS_CT16B8(instance) ? CHANNEL : (20 + CHANNEL))

#define GET_OFFSET_REG(instance, OFFSET) ((uint32_t *)((uint8_t *)instance + OFFSET))

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

#define CHECK_IRQ_IE(instance, IRQ_VALUE) (LL_REG_RBIT(*GET_OFFSET_REG(instance, GET_IRQ_IE_REG(IRQ_VALUE)), 1U << GET_IRQ_IE_BIT(IRQ_VALUE)))
#define CHECK_IRQ_IF(instance, IRQ_VALUE) (LL_REG_RBIT(*REG_RIS(instance), 1U << GET_IRQ_RIS_IC_BIT(IRQ_VALUE)))

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

// table lists all the supported functions of each TPM
static const uint8_t function_table[] = {
    LL_TPM_CAPTURE0 | LL_TPM_BREAK | LL_TPM_CM | LL_TPM_CTM, // CT16B0
    LL_TPM_CAPTURE0 | LL_TPM_CM | LL_TPM_CTM,                // CT16B1
    LL_TPM_CAPTURE0 | LL_TPM_CM | LL_TPM_CTM,                // CT16B2
    0,                                                       // CT16B3
    LL_TPM_CAPTURE0 | LL_TPM_CTM,                            // CT16B4
    LL_TPM_CAPTURE0 | LL_TPM_CM | LL_TPM_CTM,                // CT16B5
    0,                                                       // CT16B6
    0,                                                       // CT16B7
    0,                                                       // CT16B8
};

/* Private functions prototypes ----------------------------------------------*/
static void           _ll_config_channel_base(void *tpm, LL_TPM_OC_T *config, uint32_t channel);
static void           _ll_config_channel_pwm(void *tpm, LL_TPM_OC_T *config, uint32_t channel);
inline static uint8_t _get_tpm_index(void *tpm)
{
    if ((uint32_t)tpm == (uint32_t)SN_CT16B0)
        return 0;
    else if ((uint32_t)tpm == (uint32_t)SN_CT16B1)
        return 1;
    else if ((uint32_t)tpm == (uint32_t)SN_CT16B2)
        return 2;
    else if ((uint32_t)tpm == (uint32_t)SN_CT16B3)
        return 3;
    else if ((uint32_t)tpm == (uint32_t)SN_CT16B4)
        return 4;
    else if ((uint32_t)tpm == (uint32_t)SN_CT16B5)
        return 5;
    else if ((uint32_t)tpm == (uint32_t)SN_CT16B6)
        return 6;
    else if ((uint32_t)tpm == (uint32_t)SN_CT16B7)
        return 7;
    else // SN_CT16B8
        return 8;
}

#endif /* _SN34F78X_LL_TPM_H_ */
