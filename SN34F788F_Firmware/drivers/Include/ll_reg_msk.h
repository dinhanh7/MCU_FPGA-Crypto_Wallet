/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _ll_Reg_Msk_H_
#define _ll_Reg_Msk_H_

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
/*                                                                            */
/* Peripheral Registers Bits Definition                                       */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/* ADC                                                                        */
/******************************************************************************/
/* Bits definition for ADC DATA register */
#define ADC_DATA_DATA_Pos (0U)
#define ADC_DATA_DATA_Msk (0xFFFU << ADC_DATA_DATA_Pos) /*!< 0x00000FFF */
#define ADC_DATA_DATA     ADC_DATA_DATA_Msk             /*!< ADC Data register */

/* Bits definition for ADC CTRL register */
#define ADC_CTRL_ADCEN_Pos            (0U)
#define ADC_CTRL_ADCEN_Msk            (0x1U << ADC_CTRL_ADCEN_Pos) /*!< 0x00000001 */
#define ADC_CTRL_ADCEN                ADC_CTRL_ADCEN_Msk           /*!< ADC Enable */
#define ADC_CTRL_SWSTART_Pos          (4U)
#define ADC_CTRL_SWSTART_Msk          (0x1U << ADC_CTRL_SWSTART_Pos) /*!< 0x00000010 */
#define ADC_CTRL_SWSTART              ADC_CTRL_SWSTART_Msk           /*!< ADC start conversion */
#define ADC_CTRL_CONVMODE_Pos         (8U)
#define ADC_CTRL_CONVMODE_Msk         (0x0FU << ADC_CTRL_CONVMODE_Pos) /*!< 0x00000F00 */
#define ADC_CTRL_CONVMODE             ADC_CTRL_CONVMODE_Msk            /*!< ADC conversion mode */
#define ADC_CTRL_CONVMODE_SINGLE_SCAN (0x1U << ADC_CTRL_CONVMODE_Pos)  /*!< Single scan conversion mode*/
#define ADC_CTRL_CONVMODE_LOOP_SCAN   (0x2U << ADC_CTRL_CONVMODE_Pos)  /*!< Continuous scan conversion mode */
#define ADC_CTRL_AUTOPWDN_Pos         (12U)
#define ADC_CTRL_AUTOPWDN_Msk         (0x1U << ADC_CTRL_AUTOPWDN_Pos) /*!< 0x00001000 */
#define ADC_CTRL_AUTOPWDN             ADC_CTRL_AUTOPWDN_Msk           /*!< ADC conversion mode */
#define ADC_CTRL_SCANNUM_Pos          (16U)
#define ADC_CTRL_SCANNUM_Msk          (0x1FU << ADC_CTRL_SCANNUM_Pos) /*!< 0x001F0000 */
#define ADC_CTRL_SCANNUM              ADC_CTRL_SCANNUM_Msk            /*!< ADC scan conversion channel numbers */

/* Bits definition for ADC MISC register */
#define ADC_MISC_XPD_LDO_Pos         (1U)
#define ADC_MISC_XPD_LDO_Msk         (0x1U << ADC_MISC_XPD_LDO_Pos) /*!< 0x00000002 */
#define ADC_MISC_XPD_LDO             ADC_MISC_XPD_LDO_Msk           /*!< Power-down mode for internal reference LDO */
#define ADC_MISC_XPD_ADC_Pos         (2U)
#define ADC_MISC_XPD_ADC_Msk         (0x1U << ADC_MISC_XPD_ADC_Pos) /*!< 0x00000004 */
#define ADC_MISC_XPD_ADC             ADC_MISC_XPD_ADC_Msk           /*!< Power-down mode for ADC core */
#define ADC_MISC_XPD_LDOSEL_Pos      (5U)
#define ADC_MISC_XPD_LDOSEL_Msk      (0x1U << ADC_MISC_XPD_LDOSEL_Pos) /*!< 0x00000020 */
#define ADC_MISC_XPD_LDOSEL          ADC_MISC_XPD_LDOSEL_Msk           /*!< Internal Reference LDO power-down from XPD_LDO bit control signal select */
#define ADC_MISC_XPD_ADCSEL_Pos      (6U)
#define ADC_MISC_XPD_ADCSEL_Msk      (0x1U << ADC_MISC_XPD_ADCSEL_Pos) /*!< 0x00000040 */
#define ADC_MISC_XPD_ADCSEL          ADC_MISC_XPD_ADCSEL_Msk           /*!< Whole ADC power-down from XPD_LDO bit control signal select */
#define ADC_MISC_XSEL_LDO_Pos        (8U)
#define ADC_MISC_XSEL_LDO_Msk        (0x3U << ADC_MISC_XSEL_LDO_Pos) /*!< 0x00000300 */
#define ADC_MISC_XSEL_LDO            ADC_MISC_XSEL_LDO_Msk           /*!< Internal reference LDO option select */
#define ADC_MISC_XSELLDO_REFVOL_2_0V (0x1U << ADC_MISC_XSEL_LDO_Pos) /*!< Internal reference voltage = 2.0V */
#define ADC_MISC_XSELLDO_REFVOL_1_5V (0x2U << ADC_MISC_XSEL_LDO_Pos) /*!< Internal reference voltage = 1.5V */
#define ADC_MISC_XREFSEL_Pos         (12U)
#define ADC_MISC_XREFSEL_Msk         (0x3U << ADC_MISC_XREFSEL_Pos) /*!< 0x00003000 */
#define ADC_MISC_XREFSEL             ADC_MISC_XREFSEL_Msk           /*!< ADC reference option select */
#define ADC_MISC_XREFSEL_VDD         (0x1U << ADC_MISC_XREFSEL_Pos) /*!< ADC in VDD/GND reference mode (VSS~VDD) */
#define ADC_MISC_XREFSEL_INTER       (0x2U << ADC_MISC_XREFSEL_Pos) /*!< ADC in internal reference mode (VSS~IREF) */
#define ADC_MISC_XTEST_Pos           (16U)
#define ADC_MISC_XTEST_Msk           (0x3FU << ADC_MISC_XTEST_Pos) /*!< 0x003F0000 */
#define ADC_MISC_XTEST               ADC_MISC_XTEST_Msk            /*!< Test pin */
#define ADC_MISC_XBVOS_Pos           (24U)
#define ADC_MISC_XBVOS_Msk           (0x7FU << ADC_MISC_XBVOS_Pos) /*!< 0x7F000000 */
#define ADC_MISC_XBVOS               ADC_MISC_XBVOS_Msk            /*!< ADC calibration code option */
#define ADC_MISC_XCAL_Pos            (31U)
#define ADC_MISC_XCAL_Msk            (0x1U << ADC_MISC_XCAL_Pos) /*!< 0x80000000 */
#define ADC_MISC_XCAL                ADC_MISC_XCAL_Msk           /*!< Set ADC calibration mode */

/* Bits definition for ADC IE register */
#define ADC_IE_ADC_DONEIE_Pos (0U)
#define ADC_IE_ADC_DONEIE_Msk (0x1U << ADC_IE_ADC_DONEIE_Pos) /*!< 0x00000001 */
#define ADC_IE_ADC_DONEIE     ADC_IE_ADC_DONEIE_Msk           /*!< Enable conversion finish interrupt */
#define ADC_IE_ADC_STOPIE_Pos (1U)
#define ADC_IE_ADC_STOPIE_Msk (0x1U << ADC_IE_ADC_STOPIE_Pos) /*!< 0x00000002 */
#define ADC_IE_ADC_STOPIE     ADC_IE_ADC_STOPIE_Msk           /*!< Enable stop interrupt */

/* Bits definition for ADC RIS register */
#define ADC_RIS_ADC_DONEIF_Pos (0U)
#define ADC_RIS_ADC_DONEIF_Msk (0x1U << ADC_RIS_ADC_DONEIF_Pos) /*!< 0x00000001 */
#define ADC_RIS_ADC_DONEIF     ADC_RIS_ADC_DONEIF_Msk           /*!< Clear ADC conversion done interrupt flag */
#define ADC_RIS_ADC_STOPIF_Pos (1U)
#define ADC_RIS_ADC_STOPIF_Msk (0x1U << ADC_RIS_ADC_STOPIF_Pos) /*!< 0x00000002 */
#define ADC_RIS_ADC_STOPIF     ADC_RIS_ADC_STOPIF_Msk           /*!< Clear ADC conversion stop interrupt flag */
#define ADC_RIS_DMACH0IF_Pos   (4U)
#define ADC_RIS_DMACH0IF_Msk   (0x1U << ADC_RIS_DMACH0IF_Pos) /*!< 0x00000010 */
#define ADC_RIS_DMACH0IF       ADC_RIS_DMACH0IF_Msk           /*!< Clear DMA Channel 0 interrupt flag */
#define ADC_RIS_DMACH1IF_Pos   (5U)
#define ADC_RIS_DMACH1IF_Msk   (0x1U << ADC_RIS_DMACH1IF_Pos) /*!< 0x00000020 */
#define ADC_RIS_DMACH1IF       ADC_RIS_DMACH1IF_Msk           /*!< Clear DMA Channel 1 interrupt flag */
#define ADC_RIS_DMACH2IF_Pos   (6U)
#define ADC_RIS_DMACH2IF_Msk   (0x1U << ADC_RIS_DMACH2IF_Pos) /*!< 0x00000040 */
#define ADC_RIS_DMACH2IF       ADC_RIS_DMACH2IF_Msk           /*!< Clear DMA Channel 2 interrupt flag */
#define ADC_RIS_DMACH3IF_Pos   (7U)
#define ADC_RIS_DMACH3IF_Msk   (0x1U << ADC_RIS_DMACH3IF_Pos) /*!< 0x00000080 */
#define ADC_RIS_DMACH3IF       ADC_RIS_DMACH3IF_Msk           /*!< Clear DMA Channel 3 interrupt flag */

/* Bits definition for ADC TPARM register */
#define ADC_TPARM_PWRENTIME_Pos (0U)
#define ADC_TPARM_PWRENTIME_Msk (0xFFFFU << ADC_TPARM_PWRENTIME_Pos) /*!< 0x0000FFFF */
#define ADC_TPARM_PWRENTIME     ADC_TPARM_PWRENTIME_Msk              /*!< ADC power enable cycle number of MCLK */
#define ADC_TPARM_WKUPTIME_Pos  (16U)
#define ADC_TPARM_WKUPTIME_Msk  (0x7U << ADC_TPARM_WKUPTIME_Pos) /*!< 0x00070000 */
#define ADC_TPARM_WKUPTIME      ADC_TPARM_WKUPTIME_Msk           /*!< ADC wakeup cycles of ADC conversion */
#define ADC_TPARM_WKUPTIME_3    (0x3U << ADC_TPARM_WKUPTIME_Pos) /*!< Waiting 3 ADC conversion cycle */
#define ADC_TPARM_WKUPTIME_4    (0x4U << ADC_TPARM_WKUPTIME_Pos) /*!< Waiting 4 ADC conversion cycle */
#define ADC_TPARM_WKUPTIME_5    (0x5U << ADC_TPARM_WKUPTIME_Pos) /*!< Waiting 5 ADC conversion cycle */
#define ADC_TPARM_WKUPTIME_6    (0x6U << ADC_TPARM_WKUPTIME_Pos) /*!< Waiting 6 ADC conversion cycle */
#define ADC_TPARM_WKUPTIME_7    (0x7U << ADC_TPARM_WKUPTIME_Pos) /*!< Waiting 7 ADC conversion cycle */

/* Bits definition for ADC SMPR register */
#define ADC_SMPR_SETTLTIME_Pos (0U)
#define ADC_SMPR_SETTLTIME_Msk (0xFFU << ADC_SMPR_SETTLTIME_Pos) /*!< 0x000000FF */
#define ADC_SMPR_SETTLTIME     ADC_SMPR_SETTLTIME_Msk            /*!< Analog input channel settling cycle number of MCLK */
#define ADC_SMPR_SAMPLTIME_Pos (8U)
#define ADC_SMPR_SAMPLTIME_Msk (0xFU << ADC_SMPR_SAMPLTIME_Pos) /*!< 0x00000F00 */
#define ADC_SMPR_SAMPLTIME     ADC_SMPR_SAMPLTIME_Msk           /*!< Analog input channel sample clock high level cycle number of MCLK */
#define ADC_SMPR_HOLDTIME_Pos  (16U)
#define ADC_SMPR_HOLDTIME_Msk  (0xFU << ADC_SMPR_HOLDTIME_Pos) /*!< 0x000F0000 */
#define ADC_SMPR_HOLDTIME      ADC_SMPR_HOLDTIME_Msk           /*!< Analog input channel hold cycle number of MCLK */
#define ADC_SMPR_CHSELTIME_Pos (20U)
#define ADC_SMPR_CHSELTIME_Msk (0x7U << ADC_SMPR_CHSELTIME_Pos) /*!< 0x00700000 */
#define ADC_SMPR_CHSELTIME     ADC_SMPR_CHSELTIME_Msk           /*!< Analog input channel change discharge cycle number of MCLK */
#define ADC_SMPR_CHDLYTIME_Pos (24U)
#define ADC_SMPR_CHDLYTIME_Msk (0xFFU << ADC_SMPR_CHDLYTIME_Pos) /*!< 0xFF000000 */
#define ADC_SMPR_CHDLYTIME     ADC_SMPR_CHDLYTIME_Msk            /*!< Delay time cycle number of MCLK for each channel sampling */

/* Bits definition for ADC DISCHTIME register */
#define ADC_DISCHTIME_DISCHTIME_Pos (0U)
#define ADC_DISCHTIME_DISCHTIME_Msk (0xFFFFU << ADC_DISCHTIME_DISCHTIME_Pos) /*!< 0x0000FFFF */
#define ADC_DISCHTIME_DISCHTIME     ADC_DISCHTIME_DISCHTIME_Msk              /*!< Sensing Discharge cycle number of MCLK */

/* Bits definition for ADC PRE register */
#define ADC_PRE_MCLKDIV_Pos (0U)
#define ADC_PRE_MCLKDIV_Msk (0xFFU << ADC_PRE_MCLKDIV_Pos) /*!< 0x000000FF */
#define ADC_PRE_MCLKDIV     ADC_PRE_MCLKDIV_Msk            /*!< MCLK divider factor */

/* Bits definition for ADC SQRx register */
#define ADC_SQRx_SQC_Msk (0x1FU)

/* Bits definition for ADC DMAxCTRL register */
#define ADC_DMAxCTRL_DMA_CH_Pos     (0U)
#define ADC_DMAxCTRL_DMA_CH_Msk     (0x1FU << ADC_DMAxCTRL_DMA_CH_Pos) /*!< 0x0000001F */
#define ADC_DMAxCTRL_DMA_CH         ADC_DMAxCTRL_DMA_CH_Msk            /*!< ADC channel select for DMA */
#define ADC_DMAxCTRL_DMA_THD_Pos    (8U)
#define ADC_DMAxCTRL_DMA_THD_Msk    (0x3U << ADC_DMAxCTRL_DMA_THD_Pos) /*!< 0x00000300 */
#define ADC_DMAxCTRL_DMA_THD        ADC_DMAxCTRL_DMA_THD_Msk           /*!< ADC DMA Threshold */
#define ADC_DMAxCTRL_DMA_THD_8      (0x1U << ADC_DMAxCTRL_DMA_THD_Pos) /*!< 0x00000100 */
#define ADC_DMAxCTRL_DMA_THD_16     (0x2U << ADC_DMAxCTRL_DMA_THD_Pos) /*!< 0x00000200 */
#define ADC_DMAxCTRL_DMA_THD_FULL   (0x3U << ADC_DMAxCTRL_DMA_THD_Pos) /*!< 0x00000300 */
#define ADC_DMAxCTRL_DMA_EN_Pos     (16U)
#define ADC_DMAxCTRL_DMA_EN_Msk     (0x1U << ADC_DMAxCTRL_DMA_EN_Pos) /*!< 0x00010000 */
#define ADC_DMAxCTRL_DMA_EN         ADC_DMAxCTRL_DMA_EN_Msk           /*!< Enable ADC DMA */
#define ADC_DMAxCTRL_DMA_INJECT_Pos (24U)
#define ADC_DMAxCTRL_DMA_INJECT_Msk (0x3U << ADC_DMAxCTRL_DMA_INJECT_Pos) /*!< 0x00100000 */
#define ADC_DMAxCTRL_DMA_INJECT     ADC_DMAxCTRL_DMA_INJECT_Msk           /*!< ADC DMA injected data mode */
#define ADC_DMAxCTRL_DMA_INJECT_ALL (0x1U << ADC_DMAxCTRL_DMA_INJECT_Pos) /*!< Injected data for all conversion */

/* Bits definition for ADC DMAxDAT register */
#define ADC_DMADAT_DMAFIFO0_Pos (0U)
#define ADC_DMADAT_DMAFIFO0_Msk (0xFFFU << ADC_DMADAT_DMAFIFO0_Pos) /*!< 0x00000FFF */
#define ADC_DMADAT_DMAFIFO0     ADC_DMADAT_DMAFIFO0_Msk             /*!< Odd conversion data */
#define ADC_DMADAT_DMAFIFO1_Pos (16U)
#define ADC_DMADAT_DMAFIFO1_Msk (0xFFFU << ADC_DMADAT_DMAFIFO1_Pos) /*!< 0x0FFF0000 */
#define ADC_DMADAT_DMAFIFO1     ADC_DMADAT_DMAFIFO1_Msk             /*!< Even conversion data */

/* Bits definition for ADC DMAxIE register */
#define ADC_DMAIE_DMA_UDRIE_Pos (0U)
#define ADC_DMAIE_DMA_UDRIE_Msk (0x1U << ADC_DMAIE_DMA_UDRIE_Pos) /*!< 0x00000001 */
#define ADC_DMAIE_DMA_UDRIE     ADC_DMAIE_DMA_UDRIE_Msk           /*!< DMA underrun interrupt enable */
#define ADC_DMAIE_DMA_OVRIE_Pos (1U)
#define ADC_DMAIE_DMA_OVRIE_Msk (0x1U << ADC_DMAIE_DMA_OVRIE_Pos) /*!< 0x00000002 */
#define ADC_DMAIE_DMA_OVRIE     ADC_DMAIE_DMA_OVRIE_Msk           /*!< DMA overrun interrupt enable */

/* Bits definition for ADC DMAxIS register */
#define ADC_DMAIS_DMA_UDRIF_Pos    (0U)
#define ADC_DMAIS_DMA_UDRIF_Msk    (0x1U << ADC_DMAIS_DMA_UDRIF_Pos) /*!< 0x00000001 */
#define ADC_DMAIS_DMA_UDRIF        ADC_DMAIS_DMA_UDRIF_Msk           /*!< DMA FIFO underrun interrupt flag */
#define ADC_DMAIS_DMA_OVRIF_Pos    (1U)
#define ADC_DMAIS_DMA_OVRIF_Msk    (0x1U << ADC_DMAIS_DMA_OVRIF_Pos) /*!< 0x00000002 */
#define ADC_DMAIS_DMA_OVRIF        ADC_DMAIS_DMA_OVRIF_Msk           /*!< DMA FIFO overrun interrupt flag */
#define ADC_DMAIS_RPTR_EMPTY_Pos   (16U)
#define ADC_DMAIS_RPTR_EMPTY_Msk   (0x1U << ADC_DMAIS_RPTR_EMPTY_Pos) /*!< 0x00010000 */
#define ADC_DMAIS_RPTR_EMPTY       ADC_DMAIS_RPTR_EMPTY_Msk           /*!< DMA FIFO empty flag */
#define ADC_DMAIS_FIFO_HFULL_Pos   (17U)
#define ADC_DMAIS_FIFO_HFULL_Msk   (0x1U << ADC_DMAIS_FIFO_HFULL_Pos) /*!< 0x00020000 */
#define ADC_DMAIS_FIFO_HFULL       ADC_DMAIS_FIFO_HFULL_Msk           /*!< DMA FIFO full flag according to DMA_THD */
#define ADC_DMAIS_WPTR_FULL_Pos    (18U)
#define ADC_DMAIS_WPTR_FULL_Msk    (0x1U << ADC_DMAIS_WPTR_FULL_Pos) /*!< 0x00040000 */
#define ADC_DMAIS_WPTR_FULL        ADC_DMAIS_WPTR_FULL_Msk           /*!< DMA FIFO full flag */
#define ADC_DMAIS_FIFO_DAT_CNT_Pos (24U)
#define ADC_DMAIS_FIFO_DAT_CNT_Msk (0x1FU << ADC_DMAIS_FIFO_DAT_CNT_Pos) /*!< 0x1F000000 */
#define ADC_DMAIS_FIFO_DAT_CNT     ADC_DMAIS_FIFO_DAT_CNT_Msk            /*!< DMA FIFO count */

/* Bits definition for ADC THRHOLD register */
#define ADC_THRHOLD_THRHD_VAL_Pos  (0U)
#define ADC_THRHOLD_THRHD_VAL_Msk  (0xFFFU << ADC_THRHOLD_THRHD_VAL_Pos) /*!< 0x00000FFF */
#define ADC_THRHOLD_THRHD_VAL      ADC_THRHOLD_THRHD_VAL_Msk             /*!< Threshold detect value programming register(HTR/LTR) */
#define ADC_THRHOLD_THRHD_CH_Pos   (16U)
#define ADC_THRHOLD_THRHD_CH_Msk   (0x1FU << ADC_THRHOLD_THRHD_CH_Pos) /*!< 0x001F0000 */
#define ADC_THRHOLD_THRHD_CH       ADC_THRHOLD_THRHD_CH_Msk            /*!< Threshold detect for ADC channels */
#define ADC_THRHOLD_THRHD_MODE_Pos (31U)
#define ADC_THRHOLD_THRHD_MODE_Msk (0x1U << ADC_THRHOLD_THRHD_MODE_Pos) /*!< 0x80000000 */
#define ADC_THRHOLD_THRHD_MODE     ADC_THRHOLD_THRHD_MODE_Msk           /*!< Threshold detecting mode for HTR*/

/******************************************************************************/
/* ALWAYSON                                                                   */
/******************************************************************************/
/* Bits definition for System Reset Status register */
#define ALWAYSON_RSTST_LVDF_Pos (2U)
#define ALWAYSON_RSTST_LVDF_Msk (0x1U << ALWAYSON_RSTST_LVDF_Pos) /*!< 0x00000004 */
#define ALWAYSON_RSTST_LVDF     ALWAYSON_RSTST_LVDF_Msk           /*!< LVD reset flag or interrupt flag */

/* Bits definition for LVD control register */
#define ALWAYSON_LVDCTRL_LVDLVL_Pos   (0U)
#define ALWAYSON_LVDCTRL_LVDLVL_Msk   (0x3U << ALWAYSON_LVDCTRL_LVDLVL_Pos) /*!< 0x00000003 */
#define ALWAYSON_LVDCTRL_LVDLVL       ALWAYSON_LVDCTRL_LVDLVL_Msk           /*!< LVD reset/interrupt level */
#define ALWAYSON_LVDCTRL_LVDLVL_2_4_V (0x0U << ALWAYSON_LVDCTRL_LVDLVL_Pos) /*!< LVD reset/interrupt level = 2.40V*/
#define ALWAYSON_LVDCTRL_LVDLVL_2_6_V (0x1U << ALWAYSON_LVDCTRL_LVDLVL_Pos) /*!< LVD reset/interrupt level = 2.60V*/
#define ALWAYSON_LVDCTRL_LVDLVL_2_8_V (0x2U << ALWAYSON_LVDCTRL_LVDLVL_Pos) /*!< LVD reset/interrupt level = 2.80V*/
#define ALWAYSON_LVDCTRL_LVDLVL_3_0_V (0x3U << ALWAYSON_LVDCTRL_LVDLVL_Pos) /*!< LVD reset/interrupt level = 3.00V*/
#define ALWAYSON_LVDCTRL_LVDRSTEN_Pos (14U)
#define ALWAYSON_LVDCTRL_LVDRSTEN_Msk (0x1U << ALWAYSON_LVDCTRL_LVDRSTEN_Pos) /*!< 0x00004000 */
#define ALWAYSON_LVDCTRL_LVDRSTEN     ALWAYSON_LVDCTRL_LVDRSTEN_Msk           /*!< LVD Reset enable */
#define ALWAYSON_LVDCTRL_LVDEN_Pos    (15U)
#define ALWAYSON_LVDCTRL_LVDEN_Msk    (0x1U << ALWAYSON_LVDCTRL_LVDEN_Pos) /*!< 0x00008000 */
#define ALWAYSON_LVDCTRL_LVDEN        ALWAYSON_LVDCTRL_LVDEN_Msk           /*!< LVD function enable */

/* Bits definition for Oscillator Control register */
#define ALWAYSON_OSCCTL_EHSEN_Pos     (4U)
#define ALWAYSON_OSCCTL_EHSEN_Msk     (0x1U << ALWAYSON_OSCCTL_EHSEN_Pos) /*!< 0x00000010 */
#define ALWAYSON_OSCCTL_EHSEN         ALWAYSON_OSCCTL_EHSEN_Msk           /*!< External high-speed clock enable */
#define ALWAYSON_OSCCTL_EHSINROFF_Pos (6U)
#define ALWAYSON_OSCCTL_EHSINROFF_Msk (0x1U << ALWAYSON_OSCCTL_EHSINROFF_Pos) /*!< 0x00000040 */
#define ALWAYSON_OSCCTL_EHSINROFF     ALWAYSON_OSCCTL_EHSINROFF_Msk           /*!< Internal resistor of EHS XTAL control */

/* Bits definition for Oscillator Ready Flag register */
#define ALWAYSON_OSCRDY_EHSRDY_Pos (4U)
#define ALWAYSON_OSCRDY_EHSRDY_Msk (0x1U << ALWAYSON_OSCRDY_EHSRDY_Pos) /*!< 0x00000010 */
#define ALWAYSON_OSCRDY_EHSRDY     ALWAYSON_OSCRDY_EHSRDY_Msk           /*!< EHS XTAL ready flag */

/* Bits definition for POR Miscellaneous Control register */
#define ALWAYSON_POR_MISC_RTCCLKEN_Pos  (0U)
#define ALWAYSON_POR_MISC_RTCCLKEN_Msk  (0x1U << ALWAYSON_POR_MISC_RTCCLKEN_Pos) /*!< 0x00000001 */
#define ALWAYSON_POR_MISC_RTCCLKEN      ALWAYSON_POR_MISC_RTCCLKEN_Msk           /*!< Enable clock for RTC */
#define ALWAYSON_POR_MISC_RTCCLKSEL_Pos (1U)
#define ALWAYSON_POR_MISC_RTCCLKSEL_Msk (0x1U << ALWAYSON_POR_MISC_RTCCLKSEL_Pos) /*!< 0x00000002 */
#define ALWAYSON_POR_MISC_RTCCLKSEL     ALWAYSON_POR_MISC_RTCCLKSEL_Msk           /*!< RTC counter clock source */
#define ALWAYSON_POR_MISC_ELSEN_Pos     (2U)
#define ALWAYSON_POR_MISC_ELSEN_Msk     (0x1U << ALWAYSON_POR_MISC_ELSEN_Pos) /*!< 0x00000004 */
#define ALWAYSON_POR_MISC_ELSEN         ALWAYSON_POR_MISC_ELSEN_Msk           /*!< External low-speed oscillator enable */
#define ALWAYSON_POR_MISC_ELSFLOFF_Pos  (3U)
#define ALWAYSON_POR_MISC_ELSFLOFF_Msk  (0x1U << ALWAYSON_POR_MISC_ELSFLOFF_Pos) /*!< 0x00000008 */
#define ALWAYSON_POR_MISC_ELSFLOFF      ALWAYSON_POR_MISC_ELSFLOFF_Msk           /*!< External low-speed oscillator clock filter control */
#define ALWAYSON_POR_MISC_ELSRDY_Pos    (4U)
#define ALWAYSON_POR_MISC_ELSRDY_Msk    (0x1U << ALWAYSON_POR_MISC_ELSRDY_Pos) /*!< 0x00000010 */
#define ALWAYSON_POR_MISC_ELSRDY        ALWAYSON_POR_MISC_ELSRDY_Msk           /*!< ELS XTAL ready flag */
#define ALWAYSON_POR_MISC_PORRSTF_Pos   (16U)
#define ALWAYSON_POR_MISC_PORRSTF_Msk   (0x1U << ALWAYSON_POR_MISC_PORRSTF_Pos) /*!< 0x00010000 */
#define ALWAYSON_POR_MISC_PORRSTF       ALWAYSON_POR_MISC_PORRSTF_Msk           /*!< POR reset flag */

/* Bits definition for GPIO0.0~GPIO0.2 Alternate Function Control register */
#define ALWAYSON_P0_AFIO0_IO0_Pos (0U)
#define ALWAYSON_P0_AFIO0_IO0_Msk (0x1FU << ALWAYSON_P0_AFIO0_IO0_Pos) /*!< 0x0000001F */
#define ALWAYSON_P0_AFIO0_IO0     ALWAYSON_P0_AFIO0_IO0_Msk            /*!< Alternate function of P0.0  */
#define ALWAYSON_P0_AFIO0_IO1_Pos (5U)
#define ALWAYSON_P0_AFIO0_IO1_Msk (0x1FU << ALWAYSON_P0_AFIO0_IO1_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P0_AFIO0_IO1     ALWAYSON_P0_AFIO0_IO1_Msk            /*!< Alternate function of P0.1  */
#define ALWAYSON_P0_AFIO0_IO2_Pos (10U)
#define ALWAYSON_P0_AFIO0_IO2_Msk (0x1FU << ALWAYSON_P0_AFIO0_IO2_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P0_AFIO0_IO2     ALWAYSON_P0_AFIO0_IO2_Msk            /*!< Alternate function of P0.2  */

/* Bits definition for GPIO0.10~GPIO0.11 Alternate Function Control register */
#define ALWAYSON_P0_AFIO1_IO10_Pos (20U)
#define ALWAYSON_P0_AFIO1_IO10_Msk (0x1FU << ALWAYSON_P0_AFIO1_IO10_Pos) /*!< 0x01F00000 */
#define ALWAYSON_P0_AFIO1_IO10     ALWAYSON_P0_AFIO1_IO10_Msk            /*!< Alternate function of P0.10  */
#define ALWAYSON_P0_AFIO1_IO11_Pos (25U)
#define ALWAYSON_P0_AFIO1_IO11_Msk (0x1FU << ALWAYSON_P0_AFIO1_IO11_Pos) /*!< 0x3E000000 */
#define ALWAYSON_P0_AFIO1_IO11     ALWAYSON_P0_AFIO1_IO11_Msk            /*!< Alternate function of P0.11  */

/* Bits definition for GPIO0.12~GPIO0.15 Alternate Function Control register  */
#define ALWAYSON_P0_AFIO2_IO12_Pos (0U)
#define ALWAYSON_P0_AFIO2_IO12_Msk (0x1FU << ALWAYSON_P0_AFIO2_IO12_Pos) /*!< 0x0000001F */
#define ALWAYSON_P0_AFIO2_IO12     ALWAYSON_P0_AFIO2_IO12_Msk            /*!< Alternate function of P0.12  */
#define ALWAYSON_P0_AFIO2_IO13_Pos (5U)
#define ALWAYSON_P0_AFIO2_IO13_Msk (0x1FU << ALWAYSON_P0_AFIO2_IO13_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P0_AFIO2_IO13     ALWAYSON_P0_AFIO2_IO13_Msk            /*!< Alternate function of P0.13  */
#define ALWAYSON_P0_AFIO2_IO14_Pos (10U)
#define ALWAYSON_P0_AFIO2_IO14_Msk (0x1FU << ALWAYSON_P0_AFIO2_IO14_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P0_AFIO2_IO14     ALWAYSON_P0_AFIO2_IO14_Msk            /*!< Alternate function of P0.14  */
#define ALWAYSON_P0_AFIO2_IO15_Pos (15U)
#define ALWAYSON_P0_AFIO2_IO15_Msk (0x1FU << ALWAYSON_P0_AFIO2_IO15_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P0_AFIO2_IO15     ALWAYSON_P0_AFIO2_IO15_Msk            /*!< Alternate function of P0.15  */

/* Bits definition for GPIO1.0~GPIO1.5 Alternate Function Control register  */
#define ALWAYSON_P1_AFIO0_IO0_Pos (0U)
#define ALWAYSON_P1_AFIO0_IO0_Msk (0x1FU << ALWAYSON_P1_AFIO0_IO0_Pos) /*!< 0x0000001F */
#define ALWAYSON_P1_AFIO0_IO0     ALWAYSON_P1_AFIO0_IO0_Msk            /*!< Alternate function of P1.0  */
#define ALWAYSON_P1_AFIO0_IO1_Pos (5U)
#define ALWAYSON_P1_AFIO0_IO1_Msk (0x1FU << ALWAYSON_P1_AFIO0_IO1_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P1_AFIO0_IO1     ALWAYSON_P1_AFIO0_IO1_Msk            /*!< Alternate function of P1.1  */
#define ALWAYSON_P1_AFIO0_IO2_Pos (10U)
#define ALWAYSON_P1_AFIO0_IO2_Msk (0x1FU << ALWAYSON_P1_AFIO0_IO2_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P1_AFIO0_IO2     ALWAYSON_P1_AFIO0_IO2_Msk            /*!< Alternate function of P1.2  */
#define ALWAYSON_P1_AFIO0_IO3_Pos (15U)
#define ALWAYSON_P1_AFIO0_IO3_Msk (0x1FU << ALWAYSON_P1_AFIO0_IO3_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P1_AFIO0_IO3     ALWAYSON_P1_AFIO0_IO3_Msk            /*!< Alternate function of P1.3  */
#define ALWAYSON_P1_AFIO0_IO4_Pos (20U)
#define ALWAYSON_P1_AFIO0_IO4_Msk (0x1FU << ALWAYSON_P1_AFIO0_IO4_Pos) /*!< 0x01F00000 */
#define ALWAYSON_P1_AFIO0_IO4     ALWAYSON_P1_AFIO0_IO4_Msk            /*!< Alternate function of P1.4  */
#define ALWAYSON_P1_AFIO0_IO5_Pos (25U)
#define ALWAYSON_P1_AFIO0_IO5_Msk (0x1FU << ALWAYSON_P1_AFIO0_IO5_Pos) /*!< 0x3E000000 */
#define ALWAYSON_P1_AFIO0_IO5     ALWAYSON_P1_AFIO0_IO5_Msk            /*!< Alternate function of P1.5  */

/* Bits definition for GPIO1.6~GPIO1.11 Alternate Function Control register  */
#define ALWAYSON_P1_AFIO1_IO6_Pos  (0U)
#define ALWAYSON_P1_AFIO1_IO6_Msk  (0x1FU << ALWAYSON_P1_AFIO1_IO6_Pos) /*!< 0x0000001F */
#define ALWAYSON_P1_AFIO1_IO6      ALWAYSON_P1_AFIO1_IO6_Msk            /*!< Alternate function of P1.6  */
#define ALWAYSON_P1_AFIO1_IO7_Pos  (5U)
#define ALWAYSON_P1_AFIO1_IO7_Msk  (0x1FU << ALWAYSON_P1_AFIO1_IO7_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P1_AFIO1_IO7      ALWAYSON_P1_AFIO1_IO7_Msk            /*!< Alternate function of P1.7  */
#define ALWAYSON_P1_AFIO1_IO8_Pos  (10U)
#define ALWAYSON_P1_AFIO1_IO8_Msk  (0x1FU << ALWAYSON_P1_AFIO1_IO8_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P1_AFIO1_IO8      ALWAYSON_P1_AFIO1_IO8_Msk            /*!< Alternate function of P1.8  */
#define ALWAYSON_P1_AFIO1_IO9_Pos  (15U)
#define ALWAYSON_P1_AFIO1_IO9_Msk  (0x1FU << ALWAYSON_P1_AFIO1_IO9_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P1_AFIO1_IO9      ALWAYSON_P1_AFIO1_IO9_Msk            /*!< Alternate function of P1.9  */
#define ALWAYSON_P1_AFIO1_IO10_Pos (20U)
#define ALWAYSON_P1_AFIO1_IO10_Msk (0x1FU << ALWAYSON_P1_AFIO1_IO10_Pos) /*!< 0x01F00000 */
#define ALWAYSON_P1_AFIO1_IO10     ALWAYSON_P1_AFIO1_IO10_Msk            /*!< Alternate function of P1.10  */
#define ALWAYSON_P1_AFIO1_IO11_Pos (25U)
#define ALWAYSON_P1_AFIO1_IO11_Msk (0x1FU << ALWAYSON_P1_AFIO1_IO11_Pos) /*!< 0x3E000000 */
#define ALWAYSON_P1_AFIO1_IO11     ALWAYSON_P1_AFIO1_IO11_Msk            /*!< Alternate function of P1.11 */

/* Bits definition for GPIO1.12~GPIO1.15 Alternate Function Control register  */
#define ALWAYSON_P1_AFIO2_IO12_Pos (0U)
#define ALWAYSON_P1_AFIO2_IO12_Msk (0x1FU << ALWAYSON_P1_AFIO2_IO12_Pos) /*!< 0x0000001F */
#define ALWAYSON_P1_AFIO2_IO12     ALWAYSON_P1_AFIO2_IO12_Msk            /*!< Alternate function of P1.12  */
#define ALWAYSON_P1_AFIO2_IO13_Pos (5U)
#define ALWAYSON_P1_AFIO2_IO13_Msk (0x1FU << ALWAYSON_P1_AFIO2_IO13_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P1_AFIO2_IO13     ALWAYSON_P1_AFIO2_IO13_Msk            /*!< Alternate function of P1.13  */
#define ALWAYSON_P1_AFIO2_IO14_Pos (10U)
#define ALWAYSON_P1_AFIO2_IO14_Msk (0x1FU << ALWAYSON_P1_AFIO2_IO14_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P1_AFIO2_IO14     ALWAYSON_P1_AFIO2_IO14_Msk            /*!< Alternate function of P1.14  */
#define ALWAYSON_P1_AFIO2_IO15_Pos (15U)
#define ALWAYSON_P1_AFIO2_IO15_Msk (0x1FU << ALWAYSON_P1_AFIO2_IO15_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P1_AFIO2_IO15     ALWAYSON_P1_AFIO2_IO15_Msk            /*!< Alternate function of P1.15  */

/* Bits definition for GPIO2.0~GPIO2.5 Alternate Function Control register  */
#define ALWAYSON_P2_AFIO0_IO0_Pos (0U)
#define ALWAYSON_P2_AFIO0_IO0_Msk (0x1FU << ALWAYSON_P2_AFIO0_IO0_Pos) /*!< 0x0000001F */
#define ALWAYSON_P2_AFIO0_IO0     ALWAYSON_P2_AFIO0_IO0_Msk            /*!< Alternate function of P2.0  */
#define ALWAYSON_P2_AFIO0_IO1_Pos (5U)
#define ALWAYSON_P2_AFIO0_IO1_Msk (0x1FU << ALWAYSON_P2_AFIO0_IO1_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P2_AFIO0_IO1     ALWAYSON_P2_AFIO0_IO1_Msk            /*!< Alternate function of P2.1  */
#define ALWAYSON_P2_AFIO0_IO2_Pos (10U)
#define ALWAYSON_P2_AFIO0_IO2_Msk (0x1FU << ALWAYSON_P2_AFIO0_IO2_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P2_AFIO0_IO2     ALWAYSON_P2_AFIO0_IO2_Msk            /*!< Alternate function of P2.2  */
#define ALWAYSON_P2_AFIO0_IO3_Pos (15U)
#define ALWAYSON_P2_AFIO0_IO3_Msk (0x1FU << ALWAYSON_P2_AFIO0_IO3_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P2_AFIO0_IO3     ALWAYSON_P2_AFIO0_IO3_Msk            /*!< Alternate function of P2.3  */
#define ALWAYSON_P2_AFIO0_IO4_Pos (20U)
#define ALWAYSON_P2_AFIO0_IO4_Msk (0x1FU << ALWAYSON_P2_AFIO0_IO4_Pos) /*!< 0x01F00000 */
#define ALWAYSON_P2_AFIO0_IO4     ALWAYSON_P2_AFIO0_IO4_Msk            /*!< Alternate function of P2.4  */
#define ALWAYSON_P2_AFIO0_IO5_Pos (25U)
#define ALWAYSON_P2_AFIO0_IO5_Msk (0x1FU << ALWAYSON_P2_AFIO0_IO5_Pos) /*!< 0x3E000000 */
#define ALWAYSON_P2_AFIO0_IO5     ALWAYSON_P2_AFIO0_IO5_Msk            /*!< Alternate function of P2.5  */

/* Bits definition for GPIO2.6~GPIO2.11 Alternate Function Control register  */
#define ALWAYSON_P2_AFIO1_IO6_Pos  (0U)
#define ALWAYSON_P2_AFIO1_IO6_Msk  (0x1FU << ALWAYSON_P2_AFIO1_IO6_Pos) /*!< 0x0000001F */
#define ALWAYSON_P2_AFIO1_IO6      ALWAYSON_P2_AFIO1_IO6_Msk            /*!< Alternate function of P2.6  */
#define ALWAYSON_P2_AFIO1_IO7_Pos  (5U)
#define ALWAYSON_P2_AFIO1_IO7_Msk  (0x1FU << ALWAYSON_P2_AFIO1_IO7_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P2_AFIO1_IO7      ALWAYSON_P2_AFIO1_IO7_Msk            /*!< Alternate function of P2.7  */
#define ALWAYSON_P2_AFIO1_IO8_Pos  (10U)
#define ALWAYSON_P2_AFIO1_IO8_Msk  (0x1FU << ALWAYSON_P2_AFIO1_IO8_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P2_AFIO1_IO8      ALWAYSON_P2_AFIO1_IO8_Msk            /*!< Alternate function of P2.8  */
#define ALWAYSON_P2_AFIO1_IO9_Pos  (15U)
#define ALWAYSON_P2_AFIO1_IO9_Msk  (0x1FU << ALWAYSON_P2_AFIO1_IO9_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P2_AFIO1_IO9      ALWAYSON_P2_AFIO1_IO9_Msk            /*!< Alternate function of P2.9  */
#define ALWAYSON_P2_AFIO1_IO10_Pos (20U)
#define ALWAYSON_P2_AFIO1_IO10_Msk (0x1FU << ALWAYSON_P2_AFIO1_IO10_Pos) /*!< 0x01F00000 */
#define ALWAYSON_P2_AFIO1_IO10     ALWAYSON_P2_AFIO1_IO10_Msk            /*!< Alternate function of P2.10  */
#define ALWAYSON_P2_AFIO1_IO11_Pos (25U)
#define ALWAYSON_P2_AFIO1_IO11_Msk (0x1FU << ALWAYSON_P2_AFIO1_IO11_Pos) /*!< 0x3E000000 */
#define ALWAYSON_P2_AFIO1_IO11     ALWAYSON_P2_AFIO1_IO11_Msk            /*!< Alternate function of P2.11 */

/* Bits definition for GPIO2.12~GPIO2.15 Alternate Function Control register  */
#define ALWAYSON_P2_AFIO2_IO12_Pos (0U)
#define ALWAYSON_P2_AFIO2_IO12_Msk (0x1FU << ALWAYSON_P2_AFIO2_IO12_Pos) /*!< 0x0000001F */
#define ALWAYSON_P2_AFIO2_IO12     ALWAYSON_P2_AFIO2_IO12_Msk            /*!< Alternate function of P2.12  */
#define ALWAYSON_P2_AFIO2_IO13_Pos (5U)
#define ALWAYSON_P2_AFIO2_IO13_Msk (0x1FU << ALWAYSON_P2_AFIO2_IO13_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P2_AFIO2_IO13     ALWAYSON_P2_AFIO2_IO13_Msk            /*!< Alternate function of P2.13  */
#define ALWAYSON_P2_AFIO2_IO14_Pos (10U)
#define ALWAYSON_P2_AFIO2_IO14_Msk (0x1FU << ALWAYSON_P2_AFIO2_IO14_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P2_AFIO2_IO14     ALWAYSON_P2_AFIO2_IO14_Msk            /*!< Alternate function of P2.14  */
#define ALWAYSON_P2_AFIO2_IO15_Pos (15U)
#define ALWAYSON_P2_AFIO2_IO15_Msk (0x1FU << ALWAYSON_P2_AFIO2_IO15_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P2_AFIO2_IO15     ALWAYSON_P2_AFIO2_IO15_Msk            /*!< Alternate function of P2.15  */

/* Bits definition for GPIO3.0~GPIO3.5 Alternate Function Control register  */
#define ALWAYSON_P3_AFIO0_IO0_Pos (0U)
#define ALWAYSON_P3_AFIO0_IO0_Msk (0x1FU << ALWAYSON_P3_AFIO0_IO0_Pos) /*!< 0x0000001F */
#define ALWAYSON_P3_AFIO0_IO0     ALWAYSON_P3_AFIO0_IO0_Msk            /*!< Alternate function of P3.0  */
#define ALWAYSON_P3_AFIO0_IO1_Pos (5U)
#define ALWAYSON_P3_AFIO0_IO1_Msk (0x1FU << ALWAYSON_P3_AFIO0_IO1_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P3_AFIO0_IO1     ALWAYSON_P3_AFIO0_IO1_Msk            /*!< Alternate function of P3.1  */
#define ALWAYSON_P3_AFIO0_IO2_Pos (10U)
#define ALWAYSON_P3_AFIO0_IO2_Msk (0x1FU << ALWAYSON_P3_AFIO0_IO2_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P3_AFIO0_IO2     ALWAYSON_P3_AFIO0_IO2_Msk            /*!< Alternate function of P3.2  */
#define ALWAYSON_P3_AFIO0_IO3_Pos (15U)
#define ALWAYSON_P3_AFIO0_IO3_Msk (0x1FU << ALWAYSON_P3_AFIO0_IO3_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P3_AFIO0_IO3     ALWAYSON_P3_AFIO0_IO3_Msk            /*!< Alternate function of P3.3  */
#define ALWAYSON_P3_AFIO0_IO4_Pos (20U)
#define ALWAYSON_P3_AFIO0_IO4_Msk (0x1FU << ALWAYSON_P3_AFIO0_IO4_Pos) /*!< 0x01F00000 */
#define ALWAYSON_P3_AFIO0_IO4     ALWAYSON_P3_AFIO0_IO4_Msk            /*!< Alternate function of P3.4  */
#define ALWAYSON_P3_AFIO0_IO5_Pos (25U)
#define ALWAYSON_P3_AFIO0_IO5_Msk (0x1FU << ALWAYSON_P3_AFIO0_IO5_Pos) /*!< 0x3E000000 */
#define ALWAYSON_P3_AFIO0_IO5     ALWAYSON_P3_AFIO0_IO5_Msk            /*!< Alternate function of P3.5  */

/* Bits definition for GPIO3.6~GPIO3.11 Alternate Function Control register  */
#define ALWAYSON_P3_AFIO1_IO6_Pos  (0U)
#define ALWAYSON_P3_AFIO1_IO6_Msk  (0x1FU << ALWAYSON_P3_AFIO1_IO6_Pos) /*!< 0x0000001F */
#define ALWAYSON_P3_AFIO1_IO6      ALWAYSON_P3_AFIO1_IO6_Msk            /*!< Alternate function of P3.6  */
#define ALWAYSON_P3_AFIO1_IO7_Pos  (5U)
#define ALWAYSON_P3_AFIO1_IO7_Msk  (0x1FU << ALWAYSON_P3_AFIO1_IO7_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P3_AFIO1_IO7      ALWAYSON_P3_AFIO1_IO7_Msk            /*!< Alternate function of P3.7  */
#define ALWAYSON_P3_AFIO1_IO8_Pos  (10U)
#define ALWAYSON_P3_AFIO1_IO8_Msk  (0x1FU << ALWAYSON_P3_AFIO1_IO8_Pos) /*!< 0x00007C00 */
#define ALWAYSON_P3_AFIO1_IO8      ALWAYSON_P3_AFIO1_IO8_Msk            /*!< Alternate function of P3.8  */
#define ALWAYSON_P3_AFIO1_IO9_Pos  (15U)
#define ALWAYSON_P3_AFIO1_IO9_Msk  (0x1FU << ALWAYSON_P3_AFIO1_IO9_Pos) /*!< 0x000F8000 */
#define ALWAYSON_P3_AFIO1_IO9      ALWAYSON_P3_AFIO1_IO9_Msk            /*!< Alternate function of P3.9  */
#define ALWAYSON_P3_AFIO1_IO10_Pos (20U)
#define ALWAYSON_P3_AFIO1_IO10_Msk (0x1FU << ALWAYSON_P3_AFIO1_IO10_Pos) /*!< 0x01F00000 */
#define ALWAYSON_P3_AFIO1_IO10     ALWAYSON_P3_AFIO1_IO10_Msk            /*!< Alternate function of P3.10  */
#define ALWAYSON_P3_AFIO1_IO11_Pos (25U)
#define ALWAYSON_P3_AFIO1_IO11_Msk (0x1FU << ALWAYSON_P3_AFIO1_IO11_Pos) /*!< 0x3E000000 */
#define ALWAYSON_P3_AFIO1_IO11     ALWAYSON_P3_AFIO1_IO11_Msk            /*!< Alternate function of P3.11 */

/* Bits definition for GPIO3.12~GPIO3.13 Alternate Function Control register  */
#define ALWAYSON_P3_AFIO2_IO12_Pos (0U)
#define ALWAYSON_P3_AFIO2_IO12_Msk (0x1FU << ALWAYSON_P3_AFIO2_IO12_Pos) /*!< 0x0000001F */
#define ALWAYSON_P3_AFIO2_IO12     ALWAYSON_P3_AFIO2_IO12_Msk            /*!< Alternate function of P3.12  */
#define ALWAYSON_P3_AFIO2_IO13_Pos (5U)
#define ALWAYSON_P3_AFIO2_IO13_Msk (0x1FU << ALWAYSON_P3_AFIO2_IO13_Pos) /*!< 0x000003E0 */
#define ALWAYSON_P3_AFIO2_IO13     ALWAYSON_P3_AFIO2_IO13_Msk            /*!< Alternate function of P3.13  */

/* Bits definition for Oscillator Miscellaneous Control register  */
#define ALWAYSON_OSCMISC_EHSFREQ_Pos (4U)
#define ALWAYSON_OSCMISC_EHSFREQ_Msk (0x3U << ALWAYSON_OSCMISC_EHSFREQ_Pos) /*!< 0x00110000 */
#define ALWAYSON_OSCMISC_EHSFREQ     ALWAYSON_OSCMISC_EHSFREQ_Msk           /*!< Frequency range of EHS XTAL  */

/******************************************************************************/
/* CRC                                                                        */
/******************************************************************************/
/* Bits definition for CRC Control register */
#define CRC_CTRL_POLY_Pos          (0U)
#define CRC_CTRL_POLY_Msk          (0x3U << CRC_CTRL_POLY_Pos) /*!< 0x00000003 */
#define CRC_CTRL_POLY              CRC_CTRL_POLY_Msk           /*!< CRC polynomial */
#define CRC_CTRL_POLY_CRC_16_CCITT (0x0U << CRC_CTRL_POLY_Pos) /*!< CRC polynomial CRC-16-CCITT */
#define CRC_CTRL_POLY_CRC_16       (0x1U << CRC_CTRL_POLY_Pos) /*!< CRC polynomial CRC-16 */
#define CRC_CTRL_POLY_CRC_32       (0x2U << CRC_CTRL_POLY_Pos) /*!< CRC polynomial CRC-32 */
#define CRC_CTRL_RESET_Pos         (2U)
#define CRC_CTRL_RESET_Msk         (0x1U << CRC_CTRL_RESET_Pos) /*!< 0x00000004 */
#define CRC_CTRL_RESET             CRC_CTRL_RESET_Msk           /*!< CRC reset bit */
#define CRC_CTRL_BUSY_Pos          (4U)
#define CRC_CTRL_BUSY_Msk          (0x1U << CRC_CTRL_BUSY_Pos) /*!< 0x00000010 */
#define CRC_CTRL_BUSY              CRC_CTRL_BUSY_Msk           /*!< CRC calculation busy flag */

/******************************************************************************/
/* DMA                                                                        */
/******************************************************************************/
/* Bits definition DMA Main Configuration Status register */
#define DMA_MCSR_DMACEN_Pos   (0U)
#define DMA_MCSR_DMACEN_Msk   (0x1U << DMA_MCSR_DMACEN_Pos) /*!< 0x00000001 */
#define DMA_MCSR_DMACEN       DMA_MCSR_DMACEN_Msk           /* DMA controller enable */
#define DMA_MCSR_M0ENDIAN_Pos (1U)
#define DMA_MCSR_M0ENDIAN_Msk (0x1U << DMA_MCSR_M0ENDIAN_Pos) /*!< 0x00000002 */
#define DMA_MCSR_M0ENDIAN     DMA_MCSR_M0ENDIAN_Msk           /* AHB Master 0 endian configuration */
#define DMA_MCSR_M1ENDIAN_Pos (2U)
#define DMA_MCSR_M1ENDIAN_Msk (0x1U << DMA_MCSR_M1ENDIAN_Pos) /*!< 0x00000004 */
#define DMA_MCSR_M1ENDIAN     DMA_MCSR_M1ENDIAN_Msk           /* AHB Master 1 endian configuration */

/* Bits definition DMA Control register */
#define DMA_CSR_CH_EN_Pos      (0U)
#define DMA_CSR_CH_EN_Msk      (0x1U << DMA_CSR_CH_EN_Pos) /*!< 0x00000001 */
#define DMA_CSR_CH_EN          DMA_CSR_CH_EN_Msk           /* Channel enable bit */
#define DMA_CSR_DST_SEL_Pos    (1U)
#define DMA_CSR_DST_SEL_Msk    (0x1U << DMA_CSR_DST_SEL_Pos) /*!< 0x00000002 */
#define DMA_CSR_DST_SEL        DMA_CSR_DST_SEL_Msk           /* Choose AHB Master for Destination */
#define DMA_CSR_SRC_SEL_Pos    (2U)
#define DMA_CSR_SRC_SEL_Msk    (0x1U << DMA_CSR_SRC_SEL_Pos) /*!< 0x00000004 */
#define DMA_CSR_SRC_SEL        DMA_CSR_SRC_SEL_Msk           /* Choose AHB Master for Source */
#define DMA_CSR_DSTAD_CTL_Pos  (3U)
#define DMA_CSR_DSTAD_CTL_Msk  (0x3U << DMA_CSR_DSTAD_CTL_Pos) /*!< 0x00000018 */
#define DMA_CSR_DSTAD_CTL      DMA_CSR_DSTAD_CTL_Msk           /* Destination Address Control */
#define DMA_CSR_SRCAD_CTL_Pos  (5U)
#define DMA_CSR_SRCAD_CTL_Msk  (0x3U << DMA_CSR_SRCAD_CTL_Pos) /*!< 0x00000060 */
#define DMA_CSR_SRCAD_CTL      DMA_CSR_SRCAD_CTL_Msk           /* Source Address Control */
#define DMA_CSR_MODE_Pos       (7U)
#define DMA_CSR_MODE_Msk       (0x1U << DMA_CSR_MODE_Pos) /*!< 0x00000080 */
#define DMA_CSR_MODE           DMA_CSR_MODE_Msk           /* Support Hardware Handshake Mode */
#define DMA_CSR_DST_WIDTH_Pos  (8U)
#define DMA_CSR_DST_WIDTH_Msk  (0x3U << DMA_CSR_DST_WIDTH_Pos) /*!< 0x00000300 */
#define DMA_CSR_DST_WIDTH      DMA_CSR_DST_WIDTH_Msk           /* Destination transfer width */
#define DMA_CSR_SRC_WIDTH_Pos  (11U)
#define DMA_CSR_SRC_WIDTH_Msk  (0x3U << DMA_CSR_SRC_WIDTH_Pos) /*!< 0x00001800 */
#define DMA_CSR_SRC_WIDTH      DMA_CSR_SRC_WIDTH_Msk           /* Source transfer width */
#define DMA_CSR_ABT_Pos        (15U)
#define DMA_CSR_ABT_Msk        (0x1U << DMA_CSR_ABT_Pos) /*!< 0x00008000 */
#define DMA_CSR_ABT            DMA_CSR_ABT_Msk           /* Transaction abort */
#define DMA_CSR_SRC_SIZE_Pos   (16U)
#define DMA_CSR_SRC_SIZE_Msk   (0x7U << DMA_CSR_SRC_SIZE_Pos) /*!< 0x00070000 */
#define DMA_CSR_SRC_SIZE       DMA_CSR_SRC_SIZE_Msk           /* Source burst size selection */
#define DMA_CSR_CHPRI_Pos      (22U)
#define DMA_CSR_CHPRI_Msk      (0x3U << DMA_CSR_CHPRI_Pos) /*!< 0x00C00000 */
#define DMA_CSR_CHPRI          DMA_CSR_CHPRI_Msk           /* Channel priority level */
#define DMA_CSR_DMA_FF_TH_Pos  (24U)
#define DMA_CSR_DMA_FF_TH_Msk  (0x7U << DMA_CSR_DMA_FF_TH_Pos) /*!< 0x07000000 */
#define DMA_CSR_DMA_FF_TH      DMA_CSR_DMA_FF_TH_Msk           /* DMA FIFO threshold value */
#define DMA_CSR_DMA_TC_MSK_Pos (31U)
#define DMA_CSR_DMA_TC_MSK_Msk (0x1U << DMA_CSR_DMA_TC_MSK_Pos) /*!< 0x80000000 */
#define DMA_CSR_DMA_TC_MSK     DMA_CSR_DMA_TC_MSK_Msk           /* Terminal count status mask for current transaction */

/* Bits definition DMA Configuration register */
#define DMA_CFG_INT_TC_MSK_Pos  (0U)
#define DMA_CFG_INT_TC_MSK_Msk  (0x1U << DMA_CFG_INT_TC_MSK_Pos) /*!< 0x00000001 */
#define DMA_CFG_INT_TC_MSK      DMA_CFG_INT_TC_MSK_Msk           /* Channel terminal count interrupt mask */
#define DMA_CFG_INT_ERR_MSK_Pos (1U)
#define DMA_CFG_INT_ERR_MSK_Msk (0x1U << DMA_CFG_INT_ERR_MSK_Pos) /*!< 0x00000002 */
#define DMA_CFG_INT_ERR_MSK     DMA_CFG_INT_ERR_MSK_Msk           /* Channel error interrupt mask */
#define DMA_CFG_INT_ABT_MSK_Pos (2U)
#define DMA_CFG_INT_ABT_MSK_Msk (0x1U << DMA_CFG_INT_ABT_MSK_Pos) /*!< 0x00000004 */
#define DMA_CFG_INT_ABT_MSK     DMA_CFG_INT_ABT_MSK_Msk           /* Channel abort interrupt mask */
#define DMA_CFG_SRC_HE_Pos      (7U)
#define DMA_CFG_SRC_HE_Msk      (0x1U << DMA_CFG_SRC_HE_Pos) /*!< 0x00000080 */
#define DMA_CFG_SRC_HE          DMA_CFG_SRC_HE_Msk           /* Source Peripheral Mode enable */
#define DMA_CFG_BUSY_Pos        (8U)
#define DMA_CFG_BUSY_Msk        (0x1U << DMA_CFG_BUSY_Pos) /*!< 0x00000100 */
#define DMA_CFG_BUSY            DMA_CFG_BUSY_Msk           /* The DMA channel is busy */
#define DMA_CFG_DST_HE_Pos      (13U)
#define DMA_CFG_DST_HE_Msk      (0x1U << DMA_CFG_DST_HE_Pos) /*!< 0x00002000 */
#define DMA_CFG_DST_HE          DMA_CFG_DST_HE_Msk           /* Destination Peripheral Mode enable */
#define DMA_CFG_SRC_RS_Pos      (16U)
#define DMA_CFG_SRC_RS_Msk      (0x3FU << DMA_CFG_SRC_RS_Pos) /*!< 0x003F0000 */
#define DMA_CFG_SRC_RS          DMA_CFG_SRC_RS_Msk            /* Source DMA request select */
#define DMA_CFG_DST_RS_Pos      (24U)
#define DMA_CFG_DST_RS_Msk      (0x3FU << DMA_CFG_DST_RS_Pos) /*!< 0x3F000000 */
#define DMA_CFG_DST_RS          DMA_CFG_DST_RS_Msk            /* Destination DMA request select */

/* Bits definition DMA Transfer Size register */
#define DMA_SIZE_TOT_SIZE_Pos (0U)
#define DMA_SIZE_TOT_SIZE_Msk (0x3FFFFFU << DMA_SIZE_TOT_SIZE_Pos) /*!< 0x003FFFFF */
#define DMA_SIZE_TOT_SIZE_MSK DMA_SIZE_TOT_SIZE_Msk                /* Total transfer size for source */

/******************************************************************************/
/* Flash                                                                      */
/******************************************************************************/
/* Bits definition for Flash MSG0 register */
#define FLASH_MSG0_OPERATION_Pos            (0U)
#define FLASH_MSG0_OPERATION_Msk            (0xFFU << FLASH_MSG0_OPERATION_Pos) /*!< 0x000000FF */
#define FLASH_MSG0_OPERATION                FLASH_MSG0_OPERATION_Msk            /*!< FMC operation */
#define FLASH_MSG0_OPERATION_Erase          (0x1U << FLASH_MSG0_OPERATION_Pos)  /*!< Erase operation */
#define FLASH_MSG0_OPERATION_Program        (0x2U << FLASH_MSG0_OPERATION_Pos)  /*!< Program operation */
#define FLASH_MSG0_OPERATION_W_Arbitrary    (0x3U << FLASH_MSG0_OPERATION_Pos)  /*!< Arbitrary write operation */
#define FLASH_MSG0_OPERATION_R_Protect      (0x4U << FLASH_MSG0_OPERATION_Pos)  /*!< Read protect operation */
#define FLASH_MSG0_OPERATION_W_Protect      (0x5U << FLASH_MSG0_OPERATION_Pos)  /*!< Write protect operation */
#define FLASH_MSG0_OPERATION_R_Protect_Stat (0x6U << FLASH_MSG0_OPERATION_Pos)  /*!< Read protect status operation */
#define FLASH_MSG0_OPERATION_RW_Boot_Flag   (0x7U << FLASH_MSG0_OPERATION_Pos)  /*!< Read/Write boot flag operation */
#define FLASH_MSG0_OPERATION_W_Custom       (0x8U << FLASH_MSG0_OPERATION_Pos)  /*!< customer use write operation */
#define FLASH_MSG0_OPERATION_RW_Freq        (0x9U << FLASH_MSG0_OPERATION_Pos)  /*!< Read/Write FLASH ROM frequency operation */
#define FLASH_MSG0_OPERATION_Checksum       (0xAU << FLASH_MSG0_OPERATION_Pos)  /*!< Calculate MM checksum operation */
#define FLASH_MSG0_OPERATION_MUX_SEL        (0xBU << FLASH_MSG0_OPERATION_Pos)  /*!< Mux Select operation */
#define FLASH_MSG0_TYPE_Pos                 (8U)
#define FLASH_MSG0_TYPE_Msk                 (0x3U << FLASH_MSG0_TYPE_Pos) /*!< 0x00000300 */
#define FLASH_MSG0_TYPE                     FLASH_MSG0_TYPE_Msk           /*!< FMC TYPE */
#define FLASH_MSG0_TYPE_MM                  (0x0U << FLASH_MSG0_TYPE_Pos) /*!< Main memory*/
#define FLASH_MSG0_TYPE_SM                  (0x1U << FLASH_MSG0_TYPE_Pos) /*!< Supervisory memory*/
#define FLASH_MSG0_RANGE_Pos                (10U)
#define FLASH_MSG0_RANGE_Msk                (0x3U << FLASH_MSG0_RANGE_Pos) /*!< 0x00000C00 */
#define FLASH_MSG0_RANGE                    FLASH_MSG0_RANGE_Msk           /*!< Memory range used ONLY for Erase/Program operation */
#define FLASH_MSG0_RANGE_Page               (0x0U << FLASH_MSG0_RANGE_Pos) /*!< Page (MM and SM) */
#define FLASH_MSG0_RANGE_Subsector          (0x1U << FLASH_MSG0_RANGE_Pos) /*!< Subsector (MM only) */
#define FLASH_MSG0_RANGE_Sector             (0x2U << FLASH_MSG0_RANGE_Pos) /*!< Sector (MM only) */
#define FLASH_MSG0_RANGE_Bulk_All           (0x3U << FLASH_MSG0_RANGE_Pos) /*!< Bulk all (MM only) */

/* Bits definition for Flash MSG1 register */
#define FLASH_MSG1_MSG1_Pos (0U)
#define FLASH_MSG1_MSG1_Msk (0xFFFFFFFFU << FLASH_MSG1_MSG1_Pos) /*!< 0xFFFFFFFF */
#define FLASH_MSG1_MSG1     FLASH_MSG1_MSG1_Msk                  /*!< FMC Message 1, Page address[11:0] ONLY for Erase/Program/Arbitrary write operation. */

/* Bits definition for Flash MSG2 register */
#define FLASH_MSG2_MSG2_Pos (0U)
#define FLASH_MSG2_MSG2_Msk (0xFFFFFFFFU << FLASH_MSG2_MSG2_Pos) /*!< 0xFFFFFFFF */
#define FLASH_MSG2_MSG2     FLASH_MSG2_MSG2_Msk                  /*!< FMC Message 2. */

/* Bits definition for Flash MSG3 register */
#define FLASH_MSG3_STATUS_CODE_Pos                (0U)
#define FLASH_MSG3_STATUS_CODE_Msk                (0xFFFFU << FLASH_MSG3_STATUS_CODE_Pos) /*!< 0x0000FFFF */
#define FLASH_MSG3_STATUS_CODE                    FLASH_MSG3_STATUS_CODE_Msk              /*!< Status code. */
#define FLASH_MSG3_STATUS_CODE_Invalid_Addr       (0x1U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Invalid page address */
#define FLASH_MSG3_STATUS_CODE_Invalid_Type       (0x2U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Invalid memory type */
#define FLASH_MSG3_STATUS_CODE_Erase_Op           (0x3U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Erase operation */
#define FLASH_MSG3_STATUS_CODE_Erase_Failed       (0x4U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Erase failed */
#define FLASH_MSG3_STATUS_CODE_Program_Failed     (0x5U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Program failed */
#define FLASH_MSG3_STATUS_CODE_RDP_Change_Failed  (0x6U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Read protect change failed */
#define FLASH_MSG3_STATUS_CODE_WRP_En_Dis_Failed  (0x7U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Write protect enable/disable failed */
#define FLASH_MSG3_STATUS_CODE_W_Arbitrary_Failed (0x8U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Arbitrary write failed */
#define FLASH_MSG3_STATUS_CODE_Invalid_Magic_Num  (0x9U << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Invalid magic number */
#define FLASH_MSG3_STATUS_CODE_W_Boot_Flag_Failed (0xAU << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Write boot flag failed */
#define FLASH_MSG3_STATUS_CODE_W_Custom_Failed    (0xBU << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Write customer-used data failed */
#define FLASH_MSG3_STATUS_CODE_Invalid_Offset     (0xCU << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Invalid offset */
#define FLASH_MSG3_STATUS_CODE_Invalid_Length     (0xDU << FLASH_MSG3_STATUS_CODE_Pos)    /*!< Invalid length */
#define FLASH_MSG3_STATUS_CODE_Failure            (0xFFFFU << FLASH_MSG3_STATUS_CODE_Pos) /*!< Failure */
#define FLASH_MSG3_STATUS_CODE2_Pos               (16U)
#define FLASH_MSG3_STATUS_CODE2_Msk               (0xFFFFU << FLASH_MSG3_STATUS_CODE2_Pos) /*!< 0xFFFF0000 */
#define FLASH_MSG3_STATUS_CODE2                   FLASH_MSG3_STATUS_CODE2_Msk              /*!< MM read protection status or SM write protection status or boot flag or MM checksum. */

/* Bits definition for Flash PROC_START register */
#define FLASH_PROC_START_PROC_START_Pos (0U)
#define FLASH_PROC_START_PROC_START_Msk (0x1U << FLASH_PROC_START_PROC_START_Pos) /*!< 0x00000001 */
#define FLASH_PROC_START_PROC_START     FLASH_PROC_START_PROC_START_Msk           /*!< Process start control bit. */

/* Bits definition for Flash PROC_BUSY register */
#define FLASH_PROC_BUSY_PROC_BUSY_Pos (0U)
#define FLASH_PROC_BUSY_PROC_BUSY_Msk (0x1U << FLASH_PROC_BUSY_PROC_BUSY_Pos) /*!< 0x00000001 */
#define FLASH_PROC_BUSY_PROC_BUSY     FLASH_PROC_BUSY_PROC_BUSY_Msk           /*!< Process busy status. */

/* Bits definition for Flash IE register */
#define FLASH_IE_IE_Pos (0U)
#define FLASH_IE_IE_Msk (0x1U << FLASH_IE_IE_Pos) /*!< 0x00000001 */
#define FLASH_IE_IE     FLASH_IE_IE_Msk           /*!< Interrupt enable bit. */

/* Bits definition for Flash IST register */
#define FLASH_IST_DONE_Pos (0U)
#define FLASH_IST_DONE_Msk (0x1U << FLASH_IST_DONE_Pos) /*!< 0x00000001 */
#define FLASH_IST_DONE     FLASH_IST_DONE_Msk           /*!< Process done status. */
#define FLASH_IST_FAIL_Pos (1U)
#define FLASH_IST_FAIL_Msk (0x1U << FLASH_IST_FAIL_Pos) /*!< 0x00000002 */
#define FLASH_IST_FAIL     FLASH_IST_FAIL_Msk           /*!< Process fail status. */

/* Bits definition for Flash AUTO_HOLD register */
#define FLASH_AUTO_HOLD_AUTO_HOLD_Pos (0U)
#define FLASH_AUTO_HOLD_AUTO_HOLD_Msk (0x1U << FLASH_AUTO_HOLD_AUTO_HOLD_Pos) /*!< 0x00000001 */
#define FLASH_AUTO_HOLD_AUTO_HOLD     FLASH_AUTO_HOLD_AUTO_HOLD_Msk           /*!< AHB interface auto-hold enable bit. */

/******************************************************************************/
/* I2C                                                                        */
/******************************************************************************/
/* Bits definition for I2C Control register */
#define I2C_CTRL_RESET_Pos         (0U)
#define I2C_CTRL_RESET_Msk         (0x1U << I2C_CTRL_RESET_Pos) /*!< 0x00000001 */
#define I2C_CTRL_RESET             I2C_CTRL_RESET_Msk           /*!< I2C reset bit */
#define I2C_CTRL_I2CEN_Pos         (1U)
#define I2C_CTRL_I2CEN_Msk         (0x1U << I2C_CTRL_I2CEN_Pos) /*!< 0x00000002 */
#define I2C_CTRL_I2CEN             I2C_CTRL_I2CEN_Msk           /*!< I2C enable controller */
#define I2C_CTRL_MSTEN_Pos         (2U)
#define I2C_CTRL_MSTEN_Msk         (0x1U << I2C_CTRL_MSTEN_Pos) /*!< 0x00000004 */
#define I2C_CTRL_MSTEN             I2C_CTRL_MSTEN_Msk           /*!< Master mode enable bit */
#define I2C_CTRL_GCEN_Pos          (3U)
#define I2C_CTRL_GCEN_Msk          (0x1U << I2C_CTRL_GCEN_Pos) /*!< 0x00000008 */
#define I2C_CTRL_GCEN              I2C_CTRL_GCEN_Msk           /*!< General call response enable bit */
#define I2C_CTRL_START_Pos         (4U)
#define I2C_CTRL_START_Msk         (0x1U << I2C_CTRL_START_Pos) /*!< 0x00000010 */
#define I2C_CTRL_START             I2C_CTRL_START_Msk           /*!< Start condition initial bit */
#define I2C_CTRL_STOP_Pos          (5U)
#define I2C_CTRL_STOP_Msk          (0x1U << I2C_CTRL_STOP_Pos) /*!< 0x00000020 */
#define I2C_CTRL_STOP              I2C_CTRL_STOP_Msk           /*!< Stop condition initial bit */
#define I2C_CTRL_ACKNACK_Pos       (6U)
#define I2C_CTRL_ACKNACK_Msk       (0x1U << I2C_CTRL_ACKNACK_Pos) /*!< 0x00000040 */
#define I2C_CTRL_ACKNACK           I2C_CTRL_ACKNACK_Msk           /*!< ACK/NACK signal control bit */
#define I2C_CTRL_TBEN_Pos          (7U)
#define I2C_CTRL_TBEN_Msk          (0x1U << I2C_CTRL_TBEN_Pos) /*!< 0x00000080 */
#define I2C_CTRL_TBEN              I2C_CTRL_TBEN_Msk           /*!< Transfer Byte enable bit */
#define I2C_CTRL_BSTTHODIEN_Pos    (8U)
#define I2C_CTRL_BSTTHODIEN_Msk    (0x1U << I2C_CTRL_TBEN_Pos) /*!< 0x00000100 */
#define I2C_CTRL_BSTTHODIEN        I2C_CTRL_TBEN_Msk           /*!< Burst threshold interrupt enable bit */
#define I2C_CTRL_TDIEN_Pos         (9U)
#define I2C_CTRL_TDIEN_Msk         (0x1U << I2C_CTRL_TDIEN_Pos) /*!< 0x00000200 */
#define I2C_CTRL_TDIEN             I2C_CTRL_TDIEN_Msk           /*!< Data interrupt enable bit */
#define I2C_CTRL_NACKIEN_Pos       (10U)
#define I2C_CTRL_NACKIEN_Msk       (0x1U << I2C_CTRL_NACKIEN_Pos) /*!< 0x00000400 */
#define I2C_CTRL_NACKIEN           I2C_CTRL_NACKIEN_Msk           /*!< NACK interrupt enable bit */
#define I2C_CTRL_STOPIEN_Pos       (11U)
#define I2C_CTRL_STOPIEN_Msk       (0x1U << I2C_CTRL_STOPIEN_Pos) /*!< 0x00000800 */
#define I2C_CTRL_STOPIEN           I2C_CTRL_STOPIEN_Msk           /*!< Stop condition interrupt enable bit */
#define I2C_CTRL_SAMIEN_Pos        (12U)
#define I2C_CTRL_SAMIEN_Msk        (0x1U << I2C_CTRL_SAMIEN_Pos) /*!< 0x00001000 */
#define I2C_CTRL_SAMIEN            I2C_CTRL_SAMIEN_Msk           /*!< Slave address hit interrupt enable bit */
#define I2C_CTRL_ALIEN_Pos         (13U)
#define I2C_CTRL_ALIEN_Msk         (0x1U << I2C_CTRL_ALIEN_Pos) /*!< 0x00002000 */
#define I2C_CTRL_ALIEN             I2C_CTRL_ALIEN_Msk           /*!< Lose arbitration interrupt enable bit */
#define I2C_CTRL_STARTIEN_Pos      (14U)
#define I2C_CTRL_STARTIEN_Msk      (0x1U << I2C_CTRL_STARTIEN_Pos) /*!< 0x00004000 */
#define I2C_CTRL_STARTIEN          I2C_CTRL_STARTIEN_Msk           /*!< Start condition interrupt enable bit */
#define I2C_CTRL_SCLLOW_Pos        (15U)
#define I2C_CTRL_SCLLOW_Msk        (0x1U << I2C_CTRL_SCLLOW_Pos) /*!< 0x00008000 */
#define I2C_CTRL_SCLLOW            I2C_CTRL_SCLLOW_Msk           /*!< SCL tied to LOW */
#define I2C_CTRL_SDALOW_Pos        (16U)
#define I2C_CTRL_SDALOW_Msk        (0x1U << I2C_CTRL_SDALOW_Pos) /*!< 0x00010000 */
#define I2C_CTRL_SDALOW            I2C_CTRL_SDALOW_Msk           /*!< SDA tied to LOW */
#define I2C_CTRL_ARBOFF_Pos        (18U)
#define I2C_CTRL_ARBOFF_Msk        (0x1U << I2C_CTRL_ARBOFF_Pos) /*!< 0x00040000 */
#define I2C_CTRL_ARBOFF            I2C_CTRL_ARBOFF_Msk           /*!< Ignore the arbitration lose detection in the single I2C master environment */
#define I2C_CTRL_HSMODE_Pos        (19U)
#define I2C_CTRL_HSMODE_Msk        (0x1U << I2C_CTRL_HSMODE_Pos) /*!< 0x00080000 */
#define I2C_CTRL_HSMODE            I2C_CTRL_HSMODE_Msk           /*!< Switch between mode 2 and mode 1 */
#define I2C_CTRL_HSIEN_Pos         (20U)
#define I2C_CTRL_HSIEN_Msk         (0x1U << I2C_CTRL_HSIEN_Pos) /*!< 0x00100000 */
#define I2C_CTRL_HSIEN             I2C_CTRL_HSIEN_Msk           /*!< Mode 2 code interrupt enable bit in the slave mode */
#define I2C_CTRL_SBIEN_Pos         (21U)
#define I2C_CTRL_SBIEN_Msk         (0x1U << I2C_CTRL_SBIEN_Pos) /*!< 0x00200000 */
#define I2C_CTRL_SBIEN             I2C_CTRL_SBIEN_Msk           /*!< START byte interrupt enable bit */
#define I2C_CTRL_BURSTEN_Pos       (24U)
#define I2C_CTRL_BURSTEN_Msk       (0x3U << I2C_CTRL_BURSTEN_Pos) /*!< 0x03000000 */
#define I2C_CTRL_BURSTEN           I2C_CTRL_BURSTEN_Msk           /*!< I2C burst transaction control */
#define I2C_CTRL_BURSTEN_Disable   (0x0U << I2C_CTRL_BURSTEN_Pos) /*!< I2C Master/Slave protocol */
#define I2C_CTRL_BURSTEN_Master_TX (0x1U << I2C_CTRL_BURSTEN_Pos) /*!< Master TX burst mode */
#define I2C_CTRL_BURSTEN_Master_RX (0x2U << I2C_CTRL_BURSTEN_Pos) /*!< Master RX burst mode */

/* Bits definition for I2C Status register */
#define I2C_STATUS_RW_Pos        (0U)
#define I2C_STATUS_RW_Msk        (0x1U << I2C_STATUS_RW_Pos) /*!< 0x00000001 */
#define I2C_STATUS_RW            I2C_STATUS_RW_Msk           /*!< Act status */
#define I2C_STATUS_I2CB_Pos      (2U)
#define I2C_STATUS_I2CB_Msk      (0x1U << I2C_STATUS_I2CB_Pos) /*!< 0x00000004 */
#define I2C_STATUS_I2CB          I2C_STATUS_I2CB_Msk           /*!< Busy status */
#define I2C_STATUS_BB_Pos        (3U)
#define I2C_STATUS_BB_Msk        (0x1U << I2C_STATUS_BB_Pos) /*!< 0x00000008 */
#define I2C_STATUS_BB            I2C_STATUS_BB_Msk           /*!< Bus busy status */
#define I2C_STATUS_BSTTHODSR_Pos (4U)
#define I2C_STATUS_BSTTHODSR_Msk (0x1U << I2C_STATUS_BSTTHODSR_Pos) /*!< 0x00000010 */
#define I2C_STATUS_BSTTHODSR     I2C_STATUS_BSTTHODSR_Msk           /*!< Data reaches the threshold in the master RX burst mode */
#define I2C_STATUS_TD_Pos        (5U)
#define I2C_STATUS_TD_Msk        (0x1U << I2C_STATUS_TD_Pos) /*!< 0x00000020 */
#define I2C_STATUS_TD            I2C_STATUS_TD_Msk           /*!< Data transferring status */
#define I2C_STATUS_NACK_Pos      (6U)
#define I2C_STATUS_NACK_Msk      (0x1U << I2C_STATUS_NACK_Pos) /*!< 0x00000040 */
#define I2C_STATUS_NACK          I2C_STATUS_NACK_Msk           /*!< NACK status */
#define I2C_STATUS_STOP_Pos      (7U)
#define I2C_STATUS_STOP_Msk      (0x1U << I2C_STATUS_STOP_Pos) /*!< 0x00000080 */
#define I2C_STATUS_STOP          I2C_STATUS_STOP_Msk           /*!< Stop condition status */
#define I2C_STATUS_HIT_Pos       (8U)
#define I2C_STATUS_HIT_Msk       (0x1U << I2C_STATUS_HIT_Pos) /*!< 0x00000100 */
#define I2C_STATUS_HIT           I2C_STATUS_HIT_Msk           /*!< Address hit status */
#define I2C_STATUS_GC_Pos        (9U)
#define I2C_STATUS_GC_Msk        (0x1U << I2C_STATUS_GC_Pos) /*!< 0x00000200 */
#define I2C_STATUS_GC            I2C_STATUS_GC_Msk           /*!< General-call status */
#define I2C_STATUS_AL_Pos        (10U)
#define I2C_STATUS_AL_Msk        (0x1U << I2C_STATUS_AL_Pos) /*!< 0x00000400 */
#define I2C_STATUS_AL            I2C_STATUS_AL_Msk           /*!< Arbitration lost status */
#define I2C_STATUS_START_Pos     (11U)
#define I2C_STATUS_START_Msk     (0x1U << I2C_STATUS_START_Pos) /*!< 0x00000800 */
#define I2C_STATUS_START         I2C_STATUS_START_Msk           /*!< Start condition status */
#define I2C_STATUS_HSS_Pos       (22U)
#define I2C_STATUS_HSS_Msk       (0x1U << I2C_STATUS_HSS_Pos) /*!< 0x00400000 */
#define I2C_STATUS_HSS           I2C_STATUS_HSS_Msk           /*!< Mode 2 code status */
#define I2C_STATUS_SBS_Pos       (23U)
#define I2C_STATUS_SBS_Msk       (0x1U << I2C_STATUS_SBS_Pos) /*!< 0x00800000 */
#define I2C_STATUS_SBS           I2C_STATUS_SBS_Msk           /*!< START byte status */

/* Bits definition for I2C Clock Division register */
#define I2C_CLKDIV_COUNT_Pos       (0U)
#define I2C_CLKDIV_COUNT_Msk       (0xFFFFFU << I2C_CLKDIV_COUNT_Pos) /*!< 0x000FFFFF */
#define I2C_CLKDIV_COUNT           I2C_CLKDIV_COUNT_Msk               /*!< Counter value for mode 1 to generate an I2C clock from the PCLK */
#define I2C_CLKDIV_COUNTH_Pos      (20U)
#define I2C_CLKDIV_COUNTH_Msk      (0xFFU << I2C_CLKDIV_COUNTH_Pos) /*!< 0x0FF00000 */
#define I2C_CLKDIV_COUNTH          I2C_CLKDIV_COUNTH_Msk            /*!< Counter value for mode 2 to generate an I2C clock from the PCLK */
#define I2C_CLKDIV_DUTY_OFFSET_Pos (28U)
#define I2C_CLKDIV_DUTY_OFFSET_Msk (0xFU << I2C_CLKDIV_DUTY_OFFSET_Pos) /*!< 0xF0000000 */
#define I2C_CLKDIV_DUTY_OFFSET     I2C_CLKDIV_DUTY_OFFSET_Msk           /*!< I2C clock duty */

/* Bits definition for I2C Address register */
#define I2C_ADDR_ADDR_Pos      (0U)
#define I2C_ADDR_ADDR_Msk      (0x3FFU << I2C_ADDR_ADDR_Pos) /*!< 0x000003FF */
#define I2C_ADDR_ADDR          I2C_ADDR_ADDR_Msk             /*!< The 7-bit/10-bit address in the master burst mode or in the slave mode */
#define I2C_ADDR_ADDR10EN_Pos  (12U)
#define I2C_ADDR_ADDR10EN_Msk  (0x1U << I2C_ADDR_ADDR10EN_Pos) /*!< 0x00001000 */
#define I2C_ADDR_ADDR10EN      I2C_ADDR_ADDR10EN_Msk           /*!< 10-bit addressing mode enable bit */
#define I2C_ADDR_M2BIDX_EN_Pos (13U)
#define I2C_ADDR_M2BIDX_EN_Msk (0x1U << I2C_ADDR_M2BIDX_EN_Pos) /*!< 0x00002000 */
#define I2C_ADDR_M2BIDX_EN     I2C_ADDR_M2BIDX_EN_Msk           /*!< 2nd index bytes transfer enable bit */
#define I2C_ADDR_MEM_IDX_Pos   (16U)
#define I2C_ADDR_MEM_IDX_Msk   (0xFFU << I2C_ADDR_MEM_IDX_Pos) /*!< 0x00FF0000 */
#define I2C_ADDR_MEM_IDX       I2C_ADDR_MEM_IDX_Msk            /*!< 1st transmitted index for the master RX/TX burst mode */
#define I2C_ADDR_MEM_IDX2_Pos  (24U)
#define I2C_ADDR_MEM_IDX2_Msk  (0xFFU << I2C_ADDR_MEM_IDX2_Pos) /*!< 0x00FF0000 */
#define I2C_ADDR_MEM_IDX2      I2C_ADDR_MEM_IDX2_Msk            /*!< 2nd transmitted index for the master RX/TX burst mode */

/* Bits definition for I2C Set/Hold Time and Glitch Suppression register */
#define I2C_TGS_TSR_Pos (0U)
#define I2C_TGS_TSR_Msk (0x3FFU << I2C_TGS_TSR_Pos) /*!< 0x000003FF */
#define I2C_TGS_TSR     I2C_TGS_TSR_Msk             /*!< The delay values of the PCLK clock cycles between SCL and SDA while data or ACK is driven */
#define I2C_TGS_GSR_Pos (10U)
#define I2C_TGS_GSR_Msk (0xFU << I2C_TGS_GSR_Pos) /*!< 0x00003C00 */
#define I2C_TGS_GSR     I2C_TGS_GSR_Msk           /*!< The period to suppress glitch is GSR * PCLK clock cycles */

/* Bits definition for I2C Bus Monitor register */
#define I2C_BM_SDAIN_Pos (0U)
#define I2C_BM_SDAIN_Msk (0x1U << I2C_BM_SDAIN_Pos) /*!< 0x00000001 */
#define I2C_BM_SDAIN     I2C_BM_SDAIN_Msk           /*!< The value of the SDA pin */
#define I2C_BM_SCLIN_Pos (1U)
#define I2C_BM_SCLIN_Msk (0x1U << I2C_BM_SCLIN_Pos) /*!< 0x00000002 */
#define I2C_BM_SCLIN     I2C_BM_SCLIN_Msk           /*!< The value of the SCL pin */

/* Bits definition for I2C Burst Mode register */
#define I2C_BSTM_BSTTHOD_Pos (0U)
#define I2C_BSTM_BSTTHOD_Msk (0x3FU << I2C_BSTM_BSTTHOD_Pos) /*!< 0x0000003F */
#define I2C_BSTM_BSTTHOD     I2C_BSTM_BSTTHOD_Msk            /*!< Burst threshold size. These bits must be less than or equal to TDC */
#define I2C_BSTM_TDC_Pos     (8U)
#define I2C_BSTM_TDC_Msk     (0x3FU << I2C_BSTM_TDC_Pos) /*!< 0x00003F00 */
#define I2C_BSTM_TDC         I2C_BSTM_TDC_Msk            /*!< Total burst data count, which must be less than or equal to BUFSZ */
#define I2C_BSTM_BUFSZ_Pos   (16U)
#define I2C_BSTM_BUFSZ_Msk   (0x7U << I2C_BSTM_BUFSZ_Pos) /*!< 0x00070000 */
#define I2C_BSTM_BUFSZ       I2C_BSTM_BUFSZ_Msk           /*!< Buffer size */

/******************************************************************************/
/* WDT                                                                       */
/******************************************************************************/
/* Bits definition for Watchdog Control register */
#define WDT_CTRL_WDTEN_Pos (0U)
#define WDT_CTRL_WDTEN_Msk (0x1U << WDT_CTRL_WDTEN_Pos) /*!< 0x00000001 */
#define WDT_CTRL_WDTEN     WDT_CTRL_WDTEN_Msk           /*!< WDT enable bit */
#define WDT_CTRL_RSTEN_Pos (1U)
#define WDT_CTRL_RSTEN_Msk (0x1U << WDT_CTRL_RSTEN_Pos) /*!< 0x00000002 */
#define WDT_CTRL_RSTEN     WDT_CTRL_RSTEN_Msk           /*!< WDT system reset enable bit */
#define WDT_CTRL_IE_Pos    (2U)
#define WDT_CTRL_IE_Msk    (0x1U << WDT_CTRL_IE_Pos) /*!< 0x00000004 */
#define WDT_CTRL_IE        WDT_CTRL_IE_Msk           /*!< WDT system interrupt enable bit */

/* Bits definition for Watchdog Status register */
#define WDT_STATUS_COUNT_Pos (0U)
#define WDT_STATUS_COUNT_Msk (0x1U << WDT_STATUS_COUNT_Pos) /*!< 0x00000001 */
#define WDT_STATUS_COUNT     WDT_STATUS_COUNT_Msk           /*!< WDT counting status */

/* Bits definition for  Watchdog Clear register */
#define WDT_CLEAR_STATUS_Pos (0U)
#define WDT_CLEAR_STATUS_Msk (0x1U << WDT_CLEAR_STATUS_Pos) /*!< 0x00000001 */
#define WDT_CLEAR_STATUS     WDT_CLEAR_STATUS_Msk           /*!< WDT status clear bit.Writing '1' or '0' to clear WDT_STATUS register */

/******************************************************************************/
/* SCU                                                                        */
/******************************************************************************/
/* Bits definition for SCU Boot-up Status register */
#define SCU_BTUP_STS_GPIO_HOLD_Pos (2U)
#define SCU_BTUP_STS_GPIO_HOLD_Msk (0x1U << SCU_BTUP_STS_GPIO_HOLD_Pos) /*!< 0x00000002 */
#define SCU_BTUP_STS_GPIO_HOLD     SCU_BTUP_STS_GPIO_HOLD_Msk           /*!< Auto Hold the GPIO output state bit */
#define SCU_BTUP_STS_EXTRSTF_Pos   (8U)
#define SCU_BTUP_STS_EXTRSTF_Msk   (0x1U << SCU_BTUP_STS_EXTRSTF_Pos) /*!< 0x00000100 */
#define SCU_BTUP_STS_EXTRSTF       SCU_BTUP_STS_EXTRSTF_Msk           /*!< External reset flag */
#define SCU_BTUP_STS_WDTRSTF_Pos   (9U)
#define SCU_BTUP_STS_WDTRSTF_Msk   (0x1U << SCU_BTUP_STS_WDTRSTF_Pos) /*!< 0x00000200 */
#define SCU_BTUP_STS_WDTRSTF       SCU_BTUP_STS_WDTRSTF_Msk           /*!< Watchdog reset flag */
#define SCU_BTUP_STS_DPDWKF_Pos    (10U)
#define SCU_BTUP_STS_DPDWKF_Msk    (0x1U << SCU_BTUP_STS_DPDWKF_Pos) /*!< 0x00000400 */
#define SCU_BTUP_STS_DPDWKF        SCU_BTUP_STS_DPDWKF_Msk           /*!< Deep power-down wakeup flag */
#define SCU_BTUP_STS_LVDRSTF_Pos   (12U)
#define SCU_BTUP_STS_LVDRSTF_Msk   (0x1U << SCU_BTUP_STS_LVDRSTF_Pos) /*!< 0x00001000 */
#define SCU_BTUP_STS_LVDRSTF       SCU_BTUP_STS_LVDRSTF_Msk           /*!< LVD reset flag */
#define SCU_BTUP_STS_SWRSTF_Pos    (15U)
#define SCU_BTUP_STS_SWRSTF_Msk    (0x1U << SCU_BTUP_STS_SWRSTF_Pos) /*!< 0x00008000 */
#define SCU_BTUP_STS_SWRSTF        SCU_BTUP_STS_SWRSTF_Msk           /*!< Software reset flag */
#define SCU_BTUP_STS_RTCWKF_Pos    (17U)
#define SCU_BTUP_STS_RTCWKF_Msk    (0x1U << SCU_BTUP_STS_RTCWKF_Pos) /*!< 0x00020000 */
#define SCU_BTUP_STS_RTCWKF        SCU_BTUP_STS_RTCWKF_Msk           /*!< RTC alarm wakeup flag */
#define SCU_BTUP_STS_P0WKF_Pos     (18U)
#define SCU_BTUP_STS_P0WKF_Msk     (0x1U << SCU_BTUP_STS_P0WKF_Pos) /*!< 0x00040000 */
#define SCU_BTUP_STS_P0WKF         SCU_BTUP_STS_P0WKF_Msk           /*!< GPIO0 interrupt wakeup flag */
#define SCU_BTUP_STS_P1WKF_Pos     (19U)
#define SCU_BTUP_STS_P1WKF_Msk     (0x1U << SCU_BTUP_STS_P1WKF_Pos) /*!< 0x00080000 */
#define SCU_BTUP_STS_P1WKF         SCU_BTUP_STS_P1WKF_Msk           /*!< GPIO1 interrupt wakeup flag */
#define SCU_BTUP_STS_P2WKF_Pos     (20U)
#define SCU_BTUP_STS_P2WKF_Msk     (0x1U << SCU_BTUP_STS_P2WKF_Pos) /*!< 0x00100000 */
#define SCU_BTUP_STS_P2WKF         SCU_BTUP_STS_P2WKF_Msk           /*!< GPIO2 interrupt wakeup flag */
#define SCU_BTUP_STS_P3WKF_Pos     (21U)
#define SCU_BTUP_STS_P3WKF_Msk     (0x1U << SCU_BTUP_STS_P3WKF_Pos) /*!< 0x00200000 */
#define SCU_BTUP_STS_P3WKF         SCU_BTUP_STS_P3WKF_Msk           /*!< GPIO3 interrupt wakeup flag */
#define SCU_BTUP_STS_ETHWKF_Pos    (23U)
#define SCU_BTUP_STS_ETHWKF_Msk    (0x1U << SCU_BTUP_STS_ETHWKF_Pos) /*!< 0x00800000 */
#define SCU_BTUP_STS_ETHWKF        SCU_BTUP_STS_ETHWKF_Msk           /*!< Ethernet wake-on-LAN wakeup flag */
#define SCU_BTUP_STS_ALLWKF        (0xFFU << SCU_BTUP_STS_RTCWKF_Pos) /*!< All wakeup flag */

/* Bits definition for SCU Boot-up Control register */
#define SCU_BTUP_CTRL_RTCWKEN_Pos  (17U)
#define SCU_BTUP_CTRL_RTCWKEN_Msk  (0x1U << SCU_BTUP_CTRL_RTCWKEN_Pos) /*!< 0x00020000 */
#define SCU_BTUP_CTRL_RTCWKEN      SCU_BTUP_CTRL_RTCWKEN_Msk           /*!< Enable the wakeup by the RTC alarm */
#define SCU_BTUP_CTRL_P0WKEN_Pos   (18U)
#define SCU_BTUP_CTRL_P0WKEN_Msk   (0x1U << SCU_BTUP_CTRL_P0WKEN_Pos) /*!< 0x00040000 */
#define SCU_BTUP_CTRL_P0WKEN       SCU_BTUP_CTRL_P0WKEN_Msk           /*!< Enable the wakeup by the GPIO0 interrupt */
#define SCU_BTUP_CTRL_P1WKEN_Pos   (19U)
#define SCU_BTUP_CTRL_P1WKEN_Msk   (0x1U << SCU_BTUP_CTRL_P1WKEN_Pos) /*!< 0x00080000 */
#define SCU_BTUP_CTRL_P1WKEN       SCU_BTUP_CTRL_P1WKEN_Msk           /*!< Enable the wakeup by the GPIO1 interrupt */
#define SCU_BTUP_CTRL_P2WKEN_Pos   (20U)
#define SCU_BTUP_CTRL_P2WKEN_Msk   (0x1U << SCU_BTUP_CTRL_P2WKEN_Pos) /*!< 0x00100000 */
#define SCU_BTUP_CTRL_P2WKEN       SCU_BTUP_CTRL_P2WKEN_Msk           /*!< Enable the wakeup by the GPIO2 interrupt */
#define SCU_BTUP_CTRL_P3WKEN_Pos   (21U)
#define SCU_BTUP_CTRL_P3WKEN_Msk   (0x1U << SCU_BTUP_CTRL_P3WKEN_Pos) /*!< 0x00200000 */
#define SCU_BTUP_CTRL_P3WKEN       SCU_BTUP_CTRL_P3WKEN_Msk           /*!< Enable the wakeup by the GPIO3 interrupt */
#define SCU_BTUP_CTRL_ETHWKEN_Pos  (23U)
#define SCU_BTUP_CTRL_ETHWKEN_Msk  (0x1U << SCU_BTUP_CTRL_ETHWKEN_Pos) /*!< 0x00800000 */
#define SCU_BTUP_CTRL_ETHWKEN      SCU_BTUP_CTRL_ETHWKEN_Msk           /*!< Enable the wakeup by the Ethernet Wake-on-LAN Frame */

/* Bits definition for SCU CHIPID register */
#define SCU_CHIPID_CHIP_ID_Pos (0U)
#define SCU_CHIPID_CHIP_ID_Msk (0xFFFFFFFFU << SCU_CHIPID_CHIP_ID_Pos) /*!< 0xFFFFFFFF */
#define SCU_CHIPID_CHIP_ID     SCU_CHIPID_CHIP_ID_Msk                  /*!< Chip ID */

/* Bits definition for SCU CHIPID register */
#define SCU_VERSION_VERSION_Pos (0U)
#define SCU_VERSION_VERSION_Msk (0xFFFFFFFFU << SCU_VERSION_VERSION_Pos) /*!< 0xFFFFFFFF */
#define SCU_VERSION_VERSION     SCU_VERSION_VERSION_Msk                  /*!< IP version */

/* Bits definition for SCU Power Mode register */
#define SCU_PWRMODE_DPD_Pos           (1U)
#define SCU_PWRMODE_DPD_Msk           (0x1U << SCU_PWRMODE_DPD_Pos) /*!< 0x00000002 */
#define SCU_PWRMODE_DPD               SCU_PWRMODE_DPD_Msk           /*!< Deep power-down mode sequence */
#define SCU_PWRMODE_DSLEEP_Pos        (2U)
#define SCU_PWRMODE_DSLEEP_Msk        (0x1U << SCU_PWRMODE_DSLEEP_Pos) /*!< 0x00000004 */
#define SCU_PWRMODE_DSLEEP            SCU_PWRMODE_DSLEEP_Msk           /*!< Deep sleep mode sequence */
#define SCU_PWRMODE_SLEEP_Pos         (3U)
#define SCU_PWRMODE_SLEEP_Msk         (0x1U << SCU_PWRMODE_SLEEP_Pos) /*!< 0x00000008 */
#define SCU_PWRMODE_SLEEP             SCU_PWRMODE_SLEEP_Msk           /*!< Sleep mode sequence */
#define SCU_PWRMODE_FCS_Pos           (6U)
#define SCU_PWRMODE_FCS_Msk           (0x1U << SCU_PWRMODE_FCS_Pos) /*!< 0x00000040 */
#define SCU_PWRMODE_FCS               SCU_PWRMODE_FCS_Msk           /*!< Frequency Change Sequence */
#define SCU_PWRMODE_REBOOT_Pos        (7U)
#define SCU_PWRMODE_REBOOT_Msk        (0x1U << SCU_PWRMODE_REBOOT_Pos) /*!< 0x00000080 */
#define SCU_PWRMODE_REBOOT            SCU_PWRMODE_REBOOT_Msk           /*!< Reboot command */
#define SCU_PWRMODE_PERRST_Pos        (10U)
#define SCU_PWRMODE_PERRST_Msk        (0x1U << SCU_PWRMODE_PERRST_Pos) /*!< 0x00000400 */
#define SCU_PWRMODE_PERRST            SCU_PWRMODE_PERRST_Msk           /*!< Peripheral reset command */
#define SCU_PWRMODE_REMAP_Pos         (24U)
#define SCU_PWRMODE_REMAP_Msk         (0x1U << SCU_PWRMODE_REMAP_Pos) /*!< 0x01000000 */
#define SCU_PWRMODE_REMAP             SCU_PWRMODE_REMAP_Msk           /*!< Target remap programmable register */
#define SCU_PWRMODE_FCS_PLLRSTOFF_Pos (28U)
#define SCU_PWRMODE_FCS_PLLRSTOFF_Msk (0x1U << SCU_PWRMODE_FCS_PLLRSTOFF_Pos) /*!< 0x10000000 */
#define SCU_PWRMODE_FCS_PLLRSTOFF     SCU_PWRMODE_FCS_PLLRSTOFF_Msk           /*!< PLL resets in the frequency change sequence */
#define SCU_PWRMODE_EFC_STB_OFF_Pos   (31U)
#define SCU_PWRMODE_EFC_STB_OFF_Msk   (0x1U << SCU_PWRMODE_EFC_STB_OFF_Pos) /*!< 0x80000000 */
#define SCU_PWRMODE_EFC_STB_OFF       SCU_PWRMODE_EFC_STB_OFF_Msk           /*!< eFlash stand-by command off */

/* Bits definition for SCU Raw Interrupt Status register */
#define SCU_RIS_INT_DS_WAKEUP_Pos (2U)
#define SCU_RIS_INT_DS_WAKEUP_Msk (0x1U << SCU_RIS_INT_DS_WAKEUP_Pos) /*!< 0x00000004 */
#define SCU_RIS_INT_DS_WAKEUP     SCU_RIS_INT_DS_WAKEUP_Msk           /*!< Deep sleep mode wakeup interrupt status */
#define SCU_RIS_INT_WAKEUP_Pos    (3U)
#define SCU_RIS_INT_WAKEUP_Msk    (0x1U << SCU_RIS_INT_WAKEUP_Pos) /*!< 0x00000008 */
#define SCU_RIS_INT_WAKEUP        SCU_RIS_INT_WAKEUP_Msk           /*!< Sleep mode wakeup interrupt status */
#define SCU_RIS_INT_FCS_Pos       (6U)
#define SCU_RIS_INT_FCS_Msk       (0x1U << SCU_RIS_INT_FCS_Pos) /*!< 0x00000040 */
#define SCU_RIS_INT_FCS           SCU_RIS_INT_FCS_Msk           /*!< FCS command finish interrupt status */
#define SCU_RIS_INT_REMAPCHG_Pos  (11U)
#define SCU_RIS_INT_REMAPCHG_Msk  (0x1U << SCU_RIS_INT_REMAPCHG_Pos) /*!< 0x00000800 */
#define SCU_RIS_INT_REMAPCHG      SCU_RIS_INT_REMAPCHG_Msk           /*!< Remap is changed status after reboot command */
#define SCU_RIS_INT_RTC_ALARM_Pos (16U)
#define SCU_RIS_INT_RTC_ALARM_Msk (0x1U << SCU_RIS_INT_RTC_ALARM_Pos) /*!< 0x00010000 */
#define SCU_RIS_INT_RTC_ALARM     SCU_RIS_INT_RTC_ALARM_Msk           /*!< RTC alarm interrupt status */
#define SCU_RIS_INT_RTC_PER_Pos   (17U)
#define SCU_RIS_INT_RTC_PER_Msk   (0x1U << SCU_RIS_INT_RTC_PER_Pos) /*!< 0x00020000 */
#define SCU_RIS_INT_RTC_PER       SCU_RIS_INT_RTC_PER_Msk           /*!< RTC periodic interrupt status */
#define SCU_RIS_INT_RTC_SEC_Pos   (18U)
#define SCU_RIS_INT_RTC_SEC_Msk   (0x1U << SCU_RIS_INT_RTC_SEC_Pos) /*!< 0x00040000 */
#define SCU_RIS_INT_RTC_SEC       SCU_RIS_INT_RTC_SEC_Msk           /*!< RTC second out interrupt status */

/* Bits definition for SCU Interrupt Enable register */
#define SCU_IE_DS_WAKEUP_EINT_Pos (2U)
#define SCU_IE_DS_WAKEUP_EINT_Msk (0x1U << SCU_IE_DS_WAKEUP_EINT_Pos) /*!< 0x00000004 */
#define SCU_IE_DS_WAKEUP_EINT     SCU_IE_DS_WAKEUP_EINT_Msk           /*!< Deep sleep mode Wake-up interrupt enable bit */
#define SCU_IE_WAKEUP_EINT_Pos    (3U)
#define SCU_IE_WAKEUP_EINT_Msk    (0x1U << SCU_IE_WAKEUP_EINT_Pos) /*!< 0x00000008 */
#define SCU_IE_WAKEUP_EINT        SCU_IE_WAKEUP_EINT_Msk           /*!< Sleep mode Wake-up interrupt enable bit */
#define SCU_IE_FCS_EINT_Pos       (6U)
#define SCU_IE_FCS_EINT_Msk       (0x1U << SCU_IE_FCS_EINT_Pos) /*!< 0x00000040 */
#define SCU_IE_FCS_EINT           SCU_IE_FCS_EINT_Msk           /*!< FCS command finish interrupt enable bit */
#define SCU_IE_REMAPCHG_EINT_Pos  (11U)
#define SCU_IE_REMAPCHG_EINT_Msk  (0x1U << SCU_IE_REMAPCHG_EINT_Pos) /*!< 0x00000800 */
#define SCU_IE_REMAPCHG_EINT      SCU_IE_REMAPCHG_EINT_Msk           /*!< REMAP change interrupt enable */
#define SCU_IE_RTC_ALARM_EINT_Pos (16U)
#define SCU_IE_RTC_ALARM_EINT_Msk (0x1U << SCU_IE_RTC_ALARM_EINT_Pos) /*!< 0x00010000 */
#define SCU_IE_RTC_ALARM_EINT     SCU_IE_RTC_ALARM_EINT_Msk           /*!< RTC alarm interrupt enable bit */
#define SCU_IE_RTC_PER_EINT_Pos   (17U)
#define SCU_IE_RTC_PER_EINT_Msk   (0x1U << SCU_IE_RTC_PER_EINT_Pos) /*!< 0x00020000 */
#define SCU_IE_RTC_PER_EINT       SCU_IE_RTC_PER_EINT_Msk           /*!< RTC periodic interrupt enable bit */
#define SCU_IE_RTC_SEC_EINT_Pos   (18U)
#define SCU_IE_RTC_SEC_EINT_Msk   (0x1U << SCU_IE_RTC_SEC_EINT_Pos) /*!< 0x00040000 */
#define SCU_IE_RTC_SEC_EINT       SCU_IE_RTC_SEC_EINT_Msk           /*!< RTC second interrupt enable bit */

/* Bits definition for SCU PERRST Reset Control register */
#define SCU_PERRSTCTL_PERRSTAC_Pos (0U)
#define SCU_PERRSTCTL_PERRSTAC_Msk (0xFFFFU << SCU_PERRSTCTL_PERRSTAC_Pos) /*!< 0x0000FFFF */
#define SCU_PERRSTCTL_PERRSTAC     SCU_PERRSTCTL_PERRSTAC_Msk              /*!< PERRST reset active cycles of APB0 bus clock */
#define SCU_PERRSTCTL_PERRSTDC_Pos (16U)
#define SCU_PERRSTCTL_PERRSTDC_Msk (0xFFFFU << SCU_PERRSTCTL_PERRSTDC_Pos) /*!< 0xFFFF0000 */
#define SCU_PERRSTCTL_PERRSTDC     SCU_PERRSTCTL_PERRSTDC_Msk              /*!< PERRST reset delay cycles of APB0 bus clock */

/* Bits definition for SCU PLL Control register */
#define SCU_PLLCTRL_PLLEN_Pos      (0U)
#define SCU_PLLCTRL_PLLEN_Msk      (0x1U << SCU_PLLCTRL_PLLEN_Pos) /*!< 0x00000001 */
#define SCU_PLLCTRL_PLLEN          SCU_PLLCTRL_PLLEN_Msk           /*!< PLL enable control bit */
#define SCU_PLLCTRL_PLLSTABLE_Pos  (1U)
#define SCU_PLLCTRL_PLLSTABLE_Msk  (0x1U << SCU_PLLCTRL_PLLSTABLE_Pos) /*!< 0x00000002 */
#define SCU_PLLCTRL_PLLSTABLE      SCU_PLLCTRL_PLLSTABLE_Msk           /*!< PLL locking and stability status */
#define SCU_PLLCTRL_PLLCLKSEL_Pos  (4U)
#define SCU_PLLCTRL_PLLCLKSEL_Msk  (0x1U << SCU_PLLCTRL_PLLCLKSEL_Pos) /*!< 0x00000010 */
#define SCU_PLLCTRL_PLLCLKSEL      SCU_PLLCTRL_PLLCLKSEL_Msk           /*!< PLL clock source */
#define SCU_PLLCTRL_PLLCLKSEL_IHRC (0x0U << SCU_PLLCTRL_PLLCLKSEL_Pos) /*!< IHRC */
#define SCU_PLLCTRL_PLLCLKSEL_EHS  (0x1U << SCU_PLLCTRL_PLLCLKSEL_Pos) /*!< EHS */
#define SCU_PLLCTRL_SYSCLKSEL_Pos  (5U)
#define SCU_PLLCTRL_SYSCLKSEL_Msk  (0x7U << SCU_PLLCTRL_SYSCLKSEL_Pos) /*!< 0x000000E0 */
#define SCU_PLLCTRL_SYSCLKSEL      SCU_PLLCTRL_SYSCLKSEL_Msk           /*!< System clock source */
#define SCU_PLLCTRL_SYSCLKSEL_IHRC (0x0U << SCU_PLLCTRL_SYSCLKSEL_Pos) /*!< IHRC */
#define SCU_PLLCTRL_SYSCLKSEL_EHS  (0x2U << SCU_PLLCTRL_SYSCLKSEL_Pos) /*!< EHS */
#define SCU_PLLCTRL_SYSCLKSEL_PLL  (0x4U << SCU_PLLCTRL_SYSCLKSEL_Pos) /*!< PLL */
#define SCU_PLLCTRL_FS_Pos         (8U)
#define SCU_PLLCTRL_FS_Msk         (0x3U << SCU_PLLCTRL_FS_Pos) /*!< 0x00000300 */
#define SCU_PLLCTRL_FS             SCU_PLLCTRL_FS_Msk           /*!< PLL frequency range output divider */
#define SCU_PLLCTRL_FS_Div_32      (0x0U << SCU_PLLCTRL_FS_Pos) /*!< PLL CLKOUT= PLLCLK / 32 */
#define SCU_PLLCTRL_FS_Div_16      (0x1U << SCU_PLLCTRL_FS_Pos) /*!< PLL CLKOUT= PLLCLK / 16 */
#define SCU_PLLCTRL_FS_Div_8       (0x2U << SCU_PLLCTRL_FS_Pos) /*!< PLL CLKOUT= PLLCLK / 8 */
#define SCU_PLLCTRL_FS_Div_4       (0x3U << SCU_PLLCTRL_FS_Pos) /*!< PLL CLKOUT= PLLCLK / 4 */
#define SCU_PLLCTRL_NS_Pos         (24U)
#define SCU_PLLCTRL_NS_Msk         (0x7FU << SCU_PLLCTRL_NS_Pos) /*!< 0x7F000000 */
#define SCU_PLLCTRL_NS             SCU_PLLCTRL_NS_Msk            /*!< 7-bit programmable loop divider. PLLCLK = FREF * NS/FS */

/* Bits definition for SCU AHB Clock Control register */
#define SCU_AHBCLKG_DMA0CLKEN_Pos  (0U)
#define SCU_AHBCLKG_DMA0CLKEN_Msk  (0x1U << SCU_AHBCLKG_DMA0CLKEN_Pos) /*!< 0x00000001 */
#define SCU_AHBCLKG_DMA0CLKEN      SCU_AHBCLKG_DMA0CLKEN_Msk           /*!< Enable clock for DMA0 */
#define SCU_AHBCLKG_DMA1CLKEN_Pos  (1U)
#define SCU_AHBCLKG_DMA1CLKEN_Msk  (0x1U << SCU_AHBCLKG_DMA1CLKEN_Pos) /*!< 0x00000002 */
#define SCU_AHBCLKG_DMA1CLKEN      SCU_AHBCLKG_DMA1CLKEN_Msk           /*!< Enable clock for DMA1 */
#define SCU_AHBCLKG_SDIOCLKEN_Pos  (6U)
#define SCU_AHBCLKG_SDIOCLKEN_Msk  (0x1U << SCU_AHBCLKG_SDIOCLKEN_Pos) /*!< 0x00000040 */
#define SCU_AHBCLKG_SDIOCLKEN      SCU_AHBCLKG_SDIOCLKEN_Msk           /*!< Enable clock for SDIO */
#define SCU_AHBCLKG_ETHCLKEN_Pos   (8U)
#define SCU_AHBCLKG_ETHCLKEN_Msk   (0x1U << SCU_AHBCLKG_ETHCLKEN_Pos) /*!< 0x00000100 */
#define SCU_AHBCLKG_ETHCLKEN       SCU_AHBCLKG_ETHCLKEN_Msk           /*!< Enable clock for Ethernet MAC */
#define SCU_AHBCLKG_BKPSRAMEN_Pos  (10U)
#define SCU_AHBCLKG_BKPSRAMEN_Msk  (0x1U << SCU_AHBCLKG_BKPSRAMEN_Pos) /*!< 0x00000400 */
#define SCU_AHBCLKG_BKPSRAMEN      SCU_AHBCLKG_BKPSRAMEN_Msk           /*!< Enable clock for Backup SRAM */
#define SCU_AHBCLKG_ETHSRAMEN_Pos  (14U)
#define SCU_AHBCLKG_ETHSRAMEN_Msk  (0x1U << SCU_AHBCLKG_ETHSRAMEN_Pos) /*!< 0x00040000 */
#define SCU_AHBCLKG_ETHSRAMEN      SCU_AHBCLKG_ETHSRAMEN_Msk           /*!< Enable clock for Ethernet MAC SRAM */

/* Bits definition for SCU SLP_AHBCLKG register */
#define SCU_SLP_AHBCLKG_DMA0CLKEN_Pos  (0U)
#define SCU_SLP_AHBCLKG_DMA0CLKEN_Msk  (0x1U << SCU_SLP_AHBCLKG_DMA0CLKEN_Pos) /*!< 0x00000001 */
#define SCU_SLP_AHBCLKG_DMA0CLKEN      SCU_SLP_AHBCLKG_DMA0CLKEN_Msk           /*!< Enable clock for DMA0 in sleep mode */
#define SCU_SLP_AHBCLKG_DMA1CLKEN_Pos  (1U)
#define SCU_SLP_AHBCLKG_DMA1CLKEN_Msk  (0x1U << SCU_SLP_AHBCLKG_DMA1CLKEN_Pos) /*!< 0x00000002 */
#define SCU_SLP_AHBCLKG_DMA1CLKEN      SCU_SLP_AHBCLKG_DMA1CLKEN_Msk           /*!< Enable clock for DMA1 in sleep mode */
#define SCU_SLP_AHBCLKG_SRAM1EN_Pos    (2U)
#define SCU_SLP_AHBCLKG_SRAM1EN_Msk    (0x1U << SCU_SLP_AHBCLKG_SRAM1EN_Pos) /*!< 0x00000004 */
#define SCU_SLP_AHBCLKG_SRAM1EN        SCU_SLP_AHBCLKG_SRAM1EN_Msk           /*!< Enable clock for SRAM1 in sleep mode */
#define SCU_SLP_AHBCLKG_SRAM2EN_Pos    (3U)
#define SCU_SLP_AHBCLKG_SRAM2EN_Msk    (0x1U << SCU_SLP_AHBCLKG_SRAM2EN_Pos) /*!< 0x00000008 */
#define SCU_SLP_AHBCLKG_SRAM2EN        SCU_SLP_AHBCLKG_SRAM2EN_Msk           /*!< Enable clock for SRAM2 in sleep mode */
#define SCU_SLP_AHBCLKG_SRAM3EN_Pos    (4U)
#define SCU_SLP_AHBCLKG_SRAM3EN_Msk    (0x1U << SCU_SLP_AHBCLKG_SRAM3EN_Pos) /*!< 0x00000010 */
#define SCU_SLP_AHBCLKG_SRAM3EN        SCU_SLP_AHBCLKG_SRAM3EN_Msk           /*!< Enable clock for SRAM3 in sleep mode */
#define SCU_SLP_AHBCLKG_FLASHEN_Pos    (5U)
#define SCU_SLP_AHBCLKG_FLASHEN_Msk    (0x1U << SCU_SLP_AHBCLKG_FLASHEN_Pos) /*!< 0x00000020 */
#define SCU_SLP_AHBCLKG_FLASHEN        SCU_SLP_AHBCLKG_FLASHEN_Msk           /*!< Enable clock for FLASH and FMC in sleep mode */
#define SCU_SLP_AHBCLKG_SDIOCLKEN_Pos  (6U)
#define SCU_SLP_AHBCLKG_SDIOCLKEN_Msk  (0x1U << SCU_SLP_AHBCLKG_SDIOCLKEN_Pos) /*!< 0x00000040 */
#define SCU_SLP_AHBCLKG_SDIOCLKEN      SCU_SLP_AHBCLKG_SDIOCLKEN_Msk           /*!< Enable clock for SDIO in sleep mode */
#define SCU_SLP_AHBCLKG_ETHCLKEN_Pos   (8U)
#define SCU_SLP_AHBCLKG_ETHCLKEN_Msk   (0x1U << SCU_SLP_AHBCLKG_ETHCLKEN_Pos) /*!< 0x00000100 */
#define SCU_SLP_AHBCLKG_ETHCLKEN       SCU_SLP_AHBCLKG_ETHCLKEN_Msk           /*!< Enable clock for Ethernet MAC in sleep mode*/
#define SCU_SLP_AHBCLKG_BKPSRAMEN_Pos  (10U)
#define SCU_SLP_AHBCLKG_BKPSRAMEN_Msk  (0x1U << SCU_SLP_AHBCLKG_BKPSRAMEN_Pos) /*!< 0x00000400 */
#define SCU_SLP_AHBCLKG_BKPSRAMEN      SCU_SLP_AHBCLKG_BKPSRAMEN_Msk           /*!< Enable clock for Backup SRAM in sleep mode*/
#define SCU_SLP_AHBCLKG_AHBMCLKEN_Pos  (11U)
#define SCU_SLP_AHBCLKG_AHBMCLKEN_Msk  (0x1U << SCU_SLP_AHBCLKG_AHBMCLKEN_Pos) /*!< 0x00000800 */
#define SCU_SLP_AHBCLKG_AHBMCLKEN      SCU_SLP_AHBCLKG_AHBMCLKEN_Msk           /*!< Enable clock for AHB bus Matrix in sleep mode*/
#define SCU_SLP_AHBCLKG_H2P0HCLKEN_Pos (12U)
#define SCU_SLP_AHBCLKG_H2P0HCLKEN_Msk (0x1U << SCU_SLP_AHBCLKG_H2P0HCLKEN_Pos) /*!< 0x00001000 */
#define SCU_SLP_AHBCLKG_H2P0HCLKEN     SCU_SLP_AHBCLKG_H2P0HCLKEN_Msk           /*!< Enable clock for AHB to APB0 bridge in sleep mode */
#define SCU_SLP_AHBCLKG_H2P1HCLKEN_Pos (13U)
#define SCU_SLP_AHBCLKG_H2P1HCLKEN_Msk (0x1U << SCU_SLP_AHBCLKG_H2P1HCLKEN_Pos) /*!< 0x00002000 */
#define SCU_SLP_AHBCLKG_H2P1HCLKEN     SCU_SLP_AHBCLKG_H2P1HCLKEN_Msk           /*!< Enable clock for AHB to APB1 bridge in sleep mode */
#define SCU_SLP_AHBCLKG_ETHSRAMEN_Pos  (14U)
#define SCU_SLP_AHBCLKG_ETHSRAMEN_Msk  (0x1U << SCU_SLP_AHBCLKG_ETHSRAMEN_Pos) /*!< 0x00040000 */
#define SCU_SLP_AHBCLKG_ETHSRAMEN      SCU_SLP_AHBCLKG_ETHSRAMEN_Msk           /*!< Enable clock for Ethernet MAC SRAM in sleep mode*/

/* Bits definition for SCU APB0CLKG register */
#define SCU_APB0CLKG_WDTCLKEN_Pos       (0U)
#define SCU_APB0CLKG_WDTCLKEN_Msk       (0x1U << SCU_APB0CLKG_WDTCLKEN_Pos) /*!< 0x00000001 */
#define SCU_APB0CLKG_WDTCLKEN           SCU_APB0CLKG_WDTCLKEN_Msk           /*!< Enable clock for WDT */
#define SCU_APB0CLKG_WWDTCLKEN_Pos      (1U)
#define SCU_APB0CLKG_WWDTCLKEN_Msk      (0x1U << SCU_APB0CLKG_WWDTCLKEN_Pos) /*!< 0x00000002 */
#define SCU_APB0CLKG_WWDTCLKEN          SCU_APB0CLKG_WWDTCLKEN_Msk           /*!< Enable clock for WWDT */
#define SCU_APB0CLKG_CAN0CLKEN_Pos      (2U)
#define SCU_APB0CLKG_CAN0CLKEN_Msk      (0x1U << SCU_APB0CLKG_CAN0CLKEN_Pos) /*!< 0x00000004 */
#define SCU_APB0CLKG_CAN0CLKEN          SCU_APB0CLKG_CAN0CLKEN_Msk           /*!< Enable clock for CAN0 */
#define SCU_APB0CLKG_CAN1CLKEN_Pos      (3U)
#define SCU_APB0CLKG_CAN1CLKEN_Msk      (0x1U << SCU_APB0CLKG_CAN1CLKEN_Pos) /*!< 0x00000008 */
#define SCU_APB0CLKG_CAN1CLKEN          SCU_APB0CLKG_CAN1CLKEN_Msk           /*!< Enable clock for CAN1 */
#define SCU_APB0CLKG_UART1CLKEN_Pos     (4U)
#define SCU_APB0CLKG_UART1CLKEN_Msk     (0x1U << SCU_APB0CLKG_UART1CLKEN_Pos) /*!< 0x00000010 */
#define SCU_APB0CLKG_UART1CLKEN         SCU_APB0CLKG_UART1CLKEN_Msk           /*!< Enable clock for UART1 */
#define SCU_APB0CLKG_UART2CLKEN_Pos     (5U)
#define SCU_APB0CLKG_UART2CLKEN_Msk     (0x1U << SCU_APB0CLKG_UART2CLKEN_Pos) /*!< 0x00000020 */
#define SCU_APB0CLKG_UART2CLKEN         SCU_APB0CLKG_UART2CLKEN_Msk           /*!< Enable clock for UART2 */
#define SCU_APB0CLKG_UART3CLKEN_Pos     (6U)
#define SCU_APB0CLKG_UART3CLKEN_Msk     (0x1U << SCU_APB0CLKG_UART3CLKEN_Pos) /*!< 0x00000040 */
#define SCU_APB0CLKG_UART3CLKEN         SCU_APB0CLKG_UART3CLKEN_Msk           /*!< Enable clock for UART3 */
#define SCU_APB0CLKG_UART4CLKEN_Pos     (7U)
#define SCU_APB0CLKG_UART4CLKEN_Msk     (0x1U << SCU_APB0CLKG_UART4CLKEN_Pos) /*!< 0x00000080 */
#define SCU_APB0CLKG_UART4CLKEN         SCU_APB0CLKG_UART4CLKEN_Msk           /*!< Enable clock for UART4 */
#define SCU_APB0CLKG_CT16B6CLKEN_Pos    (8U)
#define SCU_APB0CLKG_CT16B6CLKEN_Msk    (0x1U << SCU_APB0CLKG_CT16B6CLKEN_Pos) /*!< 0x00000100 */
#define SCU_APB0CLKG_CT16B6CLKEN        SCU_APB0CLKG_CT16B6CLKEN_Msk           /*!< Enable clock for CT16B6 */
#define SCU_APB0CLKG_CT16B7CLKEN_Pos    (9U)
#define SCU_APB0CLKG_CT16B7CLKEN_Msk    (0x1U << SCU_APB0CLKG_CT16B7CLKEN_Pos) /*!< 0x00000200 */
#define SCU_APB0CLKG_CT16B7CLKEN        SCU_APB0CLKG_CT16B7CLKEN_Msk           /*!< Enable clock for CT16B7 */
#define SCU_APB0CLKG_ALWAYSONREGEN_Pos  (10U)
#define SCU_APB0CLKG_ALWAYSONREGEN_Msk  (0x1U << SCU_APB0CLKG_ALWAYSONREGEN_Pos) /*!< 0x00000400 */
#define SCU_APB0CLKG_ALWAYSONREGEN      SCU_APB0CLKG_ALWAYSONREGEN_Msk           /*!< Enable clock for Always-on domain registers */
#define SCU_APB0CLKG_CAN0SRAMCLKEN_Pos  (13U)
#define SCU_APB0CLKG_CAN0SRAMCLKEN_Msk  (0x1U << SCU_APB0CLKG_CAN0SRAMCLKEN_Pos) /*!< 0x00002000 */
#define SCU_APB0CLKG_CAN0SRAMCLKEN      SCU_APB0CLKG_CAN0SRAMCLKEN_Msk           /*!< Enable clock for CAN0 SRAM */
#define SCU_APB0CLKG_CAN1SRAMCLKEN_Pos  (14U)
#define SCU_APB0CLKG_CAN1SRAMCLKEN_Msk  (0x1U << SCU_APB0CLKG_CAN1SRAMCLKEN_Pos) /*!< 0x00004000 */
#define SCU_APB0CLKG_CAN1SRAMCLKEN      SCU_APB0CLKG_CAN1SRAMCLKEN_Msk           /*!< Enable clock for CAN1 SRAM */
#define SCU_APB0CLKG_CLKOUTEN_Pos       (15U)
#define SCU_APB0CLKG_CLKOUTEN_Msk       (0x1U << SCU_APB0CLKG_CLKOUTEN_Pos) /*!< 0x00008000 */
#define SCU_APB0CLKG_CLKOUTEN           SCU_APB0CLKG_CLKOUTEN_Msk           /*!< Enable clock for CLKOUT */

/* Bits definition for SCU APB1CLKG register */
#define SCU_APB1CLKG_ADC0CLKEN_Pos   (0U)
#define SCU_APB1CLKG_ADC0CLKEN_Msk   (0x1U << SCU_APB1CLKG_ADC0CLKEN_Pos) /*!< 0x00000001 */
#define SCU_APB1CLKG_ADC0CLKEN       SCU_APB1CLKG_ADC0CLKEN_Msk           /*!< Enable clock for ADC0 */
#define SCU_APB1CLKG_SSP0CLKEN_Pos   (1U)
#define SCU_APB1CLKG_SSP0CLKEN_Msk   (0x1U << SCU_APB1CLKG_SSP0CLKEN_Pos) /*!< 0x00000002 */
#define SCU_APB1CLKG_SSP0CLKEN       SCU_APB1CLKG_SSP0CLKEN_Msk           /*!< Enable clock for SSP0 */
#define SCU_APB1CLKG_SSP1CLKEN_Pos   (2U)
#define SCU_APB1CLKG_SSP1CLKEN_Msk   (0x1U << SCU_APB1CLKG_SSP1CLKEN_Pos) /*!< 0x00000004 */
#define SCU_APB1CLKG_SSP1CLKEN       SCU_APB1CLKG_SSP1CLKEN_Msk           /*!< Enable clock for SSP1 */
#define SCU_APB1CLKG_SSP2CLKEN_Pos   (3U)
#define SCU_APB1CLKG_SSP2CLKEN_Msk   (0x1U << SCU_APB1CLKG_SSP2CLKEN_Pos) /*!< 0x00000008 */
#define SCU_APB1CLKG_SSP2CLKEN       SCU_APB1CLKG_SSP2CLKEN_Msk           /*!< Enable clock for SSP2 */
#define SCU_APB1CLKG_UART0CLKEN_Pos  (4U)
#define SCU_APB1CLKG_UART0CLKEN_Msk  (0x1U << SCU_APB1CLKG_UART0CLKEN_Pos) /*!< 0x00000010 */
#define SCU_APB1CLKG_UART0CLKEN      SCU_APB1CLKG_UART0CLKEN_Msk           /*!< Enable clock for UART0 */
#define SCU_APB1CLKG_UART5CLKEN_Pos  (5U)
#define SCU_APB1CLKG_UART5CLKEN_Msk  (0x1U << SCU_APB1CLKG_UART5CLKEN_Pos) /*!< 0x00000020 */
#define SCU_APB1CLKG_UART5CLKEN      SCU_APB1CLKG_UART5CLKEN_Msk           /*!< Enable clock for UART5 */
#define SCU_APB1CLKG_I2C0CLKEN_Pos   (6U)
#define SCU_APB1CLKG_I2C0CLKEN_Msk   (0x1U << SCU_APB1CLKG_I2C0CLKEN_Pos) /*!< 0x00000040 */
#define SCU_APB1CLKG_I2C0CLKEN       SCU_APB1CLKG_I2C0CLKEN_Msk           /*!< Enable clock for I2C0 */
#define SCU_APB1CLKG_I2C1CLKEN_Pos   (7U)
#define SCU_APB1CLKG_I2C1CLKEN_Msk   (0x1U << SCU_APB1CLKG_I2C1CLKEN_Pos) /*!< 0x00000080 */
#define SCU_APB1CLKG_I2C1CLKEN       SCU_APB1CLKG_I2C1CLKEN_Msk           /*!< Enable clock for I2C1 */
#define SCU_APB1CLKG_I2C2CLKEN_Pos   (8U)
#define SCU_APB1CLKG_I2C2CLKEN_Msk   (0x1U << SCU_APB1CLKG_I2C2CLKEN_Pos) /*!< 0x00000100 */
#define SCU_APB1CLKG_I2C2CLKEN       SCU_APB1CLKG_I2C2CLKEN_Msk           /*!< Enable clock for I2C2 */
#define SCU_APB1CLKG_CT16B0CLKEN_Pos (9U)
#define SCU_APB1CLKG_CT16B0CLKEN_Msk (0x1U << SCU_APB1CLKG_CT16B0CLKEN_Pos) /*!< 0x00000200 */
#define SCU_APB1CLKG_CT16B0CLKEN     SCU_APB1CLKG_CT16B0CLKEN_Msk           /*!< Enable clock for CT16B0 */
#define SCU_APB1CLKG_CT16B1CLKEN_Pos (10U)
#define SCU_APB1CLKG_CT16B1CLKEN_Msk (0x1U << SCU_APB1CLKG_CT16B1CLKEN_Pos) /*!< 0x00000400 */
#define SCU_APB1CLKG_CT16B1CLKEN     SCU_APB1CLKG_CT16B1CLKEN_Msk           /*!< Enable clock for CT16B1 */
#define SCU_APB1CLKG_CT16B2CLKEN_Pos (11U)
#define SCU_APB1CLKG_CT16B2CLKEN_Msk (0x1U << SCU_APB1CLKG_CT16B2CLKEN_Pos) /*!< 0x00000800 */
#define SCU_APB1CLKG_CT16B2CLKEN     SCU_APB1CLKG_CT16B2CLKEN_Msk           /*!< Enable clock for CT16B2 */
#define SCU_APB1CLKG_CT16B3CLKEN_Pos (12U)
#define SCU_APB1CLKG_CT16B3CLKEN_Msk (0x1U << SCU_APB1CLKG_CT16B3CLKEN_Pos) /*!< 0x00001000 */
#define SCU_APB1CLKG_CT16B3CLKEN     SCU_APB1CLKG_CT16B3CLKEN_Msk           /*!< Enable clock for CT16B3 */
#define SCU_APB1CLKG_CT16B4CLKEN_Pos (13U)
#define SCU_APB1CLKG_CT16B4CLKEN_Msk (0x1U << SCU_APB1CLKG_CT16B4CLKEN_Pos) /*!< 0x00002000 */
#define SCU_APB1CLKG_CT16B4CLKEN     SCU_APB1CLKG_CT16B4CLKEN_Msk           /*!< Enable clock for CT16B4 */
#define SCU_APB1CLKG_CT16B5CLKEN_Pos (14U)
#define SCU_APB1CLKG_CT16B5CLKEN_Msk (0x1U << SCU_APB1CLKG_CT16B5CLKEN_Pos) /*!< 0x00004000 */
#define SCU_APB1CLKG_CT16B5CLKEN     SCU_APB1CLKG_CT16B5CLKEN_Msk           /*!< Enable clock for CT16B5 */
#define SCU_APB1CLKG_CT16B8CLKEN_Pos (15U)
#define SCU_APB1CLKG_CT16B8CLKEN_Msk (0x1U << SCU_APB1CLKG_CT16B8CLKEN_Pos) /*!< 0x00008000 */
#define SCU_APB1CLKG_CT16B8CLKEN     SCU_APB1CLKG_CT16B8CLKEN_Msk           /*!< Enable clock for CT16B8 */
#define SCU_APB1CLKG_CRCCLKEN_Pos    (16U)
#define SCU_APB1CLKG_CRCCLKEN_Msk    (0x1U << SCU_APB1CLKG_CRCCLKEN_Pos) /*!< 0x00010000 */
#define SCU_APB1CLKG_CRCCLKEN        SCU_APB1CLKG_CRCCLKEN_Msk           /*!< Enable clock for CRC */
#define SCU_APB1CLKG_P0CLKEN_Pos     (17U)
#define SCU_APB1CLKG_P0CLKEN_Msk     (0x1U << SCU_APB1CLKG_P0CLKEN_Pos) /*!< 0x00020000 */
#define SCU_APB1CLKG_P0CLKEN         SCU_APB1CLKG_P0CLKEN_Msk           /*!< Enable clock for GPIO0 */
#define SCU_APB1CLKG_P1CLKEN_Pos     (18U)
#define SCU_APB1CLKG_P1CLKEN_Msk     (0x1U << SCU_APB1CLKG_P1CLKEN_Pos) /*!< 0x00040000 */
#define SCU_APB1CLKG_P1CLKEN         SCU_APB1CLKG_P1CLKEN_Msk           /*!< Enable clock for GPIO1 */
#define SCU_APB1CLKG_P2CLKEN_Pos     (19U)
#define SCU_APB1CLKG_P2CLKEN_Msk     (0x1U << SCU_APB1CLKG_P2CLKEN_Pos) /*!< 0x00080000 */
#define SCU_APB1CLKG_P2CLKEN         SCU_APB1CLKG_P2CLKEN_Msk           /*!< Enable clock for GPIO2 */
#define SCU_APB1CLKG_P3CLKEN_Pos     (20U)
#define SCU_APB1CLKG_P3CLKEN_Msk     (0x1U << SCU_APB1CLKG_P3CLKEN_Pos) /*!< 0x00100000 */
#define SCU_APB1CLKG_P3CLKEN         SCU_APB1CLKG_P3CLKEN_Msk           /*!< Enable clock for GPIO3 */
#define SCU_APB1CLKG_LCMCLKEN_Pos    (21U)
#define SCU_APB1CLKG_LCMCLKEN_Msk    (0x1U << SCU_APB1CLKG_LCMCLKEN_Pos) /*!< 0x00200000 */
#define SCU_APB1CLKG_LCMCLKEN        SCU_APB1CLKG_LCMCLKEN_Msk           /*!< Enable clock for LCM */

/* Bits definition for SCU SLP_APB0CLKG register */
#define SCU_SLP_APB0CLKG_WDTCLKEN_Pos       (0U)
#define SCU_SLP_APB0CLKG_WDTCLKEN_Msk       (0x1U << SCU_SLP_APB0CLKG_WDTCLKEN_Pos) /*!< 0x00000001 */
#define SCU_SLP_APB0CLKG_WDTCLKEN           SCU_SLP_APB0CLKG_WDTCLKEN_Msk           /*!< Enable clock for WDT in sleep mode */
#define SCU_SLP_APB0CLKG_WWDTCLKEN_Pos      (1U)
#define SCU_SLP_APB0CLKG_WWDTCLKEN_Msk      (0x1U << SCU_SLP_APB0CLKG_WWDTCLKEN_Pos) /*!< 0x00000002 */
#define SCU_SLP_APB0CLKG_WWDTCLKEN          SCU_SLP_APB0CLKG_WWDTCLKEN_Msk           /*!< Enable clock for WWDT in sleep mode */
#define SCU_SLP_APB0CLKG_CAN0CLKEN_Pos      (2U)
#define SCU_SLP_APB0CLKG_CAN0CLKEN_Msk      (0x1U << SCU_SLP_APB0CLKG_CAN0CLKEN_Pos) /*!< 0x00000004 */
#define SCU_SLP_APB0CLKG_CAN0CLKEN          SCU_SLP_APB0CLKG_CAN0CLKEN_Msk           /*!< Enable clock for CAN0 in sleep mode */
#define SCU_SLP_APB0CLKG_CAN1CLKEN_Pos      (3U)
#define SCU_SLP_APB0CLKG_CAN1CLKEN_Msk      (0x1U << SCU_SLP_APB0CLKG_CAN1CLKEN_Pos) /*!< 0x00000008 */
#define SCU_SLP_APB0CLKG_CAN1CLKEN          SCU_SLP_APB0CLKG_CAN1CLKEN_Msk           /*!< Enable clock for CAN1 in sleep mode */
#define SCU_SLP_APB0CLKG_UART1CLKEN_Pos     (4U)
#define SCU_SLP_APB0CLKG_UART1CLKEN_Msk     (0x1U << SCU_SLP_APB0CLKG_UART1CLKEN_Pos) /*!< 0x00000010 */
#define SCU_SLP_APB0CLKG_UART1CLKEN         SCU_SLP_APB0CLKG_UART1CLKEN_Msk           /*!< Enable clock for UART1 in sleep mode */
#define SCU_SLP_APB0CLKG_UART2CLKEN_Pos     (5U)
#define SCU_SLP_APB0CLKG_UART2CLKEN_Msk     (0x1U << SCU_SLP_APB0CLKG_UART2CLKEN_Pos) /*!< 0x00000020 */
#define SCU_SLP_APB0CLKG_UART2CLKEN         SCU_SLP_APB0CLKG_UART2CLKEN_Msk           /*!< Enable clock for UART2 in sleep mode */
#define SCU_SLP_APB0CLKG_UART3CLKEN_Pos     (6U)
#define SCU_SLP_APB0CLKG_UART3CLKEN_Msk     (0x1U << SCU_SLP_APB0CLKG_UART3CLKEN_Pos) /*!< 0x00000040 */
#define SCU_SLP_APB0CLKG_UART3CLKEN         SCU_SLP_APB0CLKG_UART3CLKEN_Msk           /*!< Enable clock for UART3 in sleep mode */
#define SCU_SLP_APB0CLKG_UART4CLKEN_Pos     (7U)
#define SCU_SLP_APB0CLKG_UART4CLKEN_Msk     (0x1U << SCU_SLP_APB0CLKG_UART4CLKEN_Pos) /*!< 0x00000080 */
#define SCU_SLP_APB0CLKG_UART4CLKEN         SCU_SLP_APB0CLKG_UART4CLKEN_Msk           /*!< Enable clock for UART4 in sleep mode */
#define SCU_SLP_APB0CLKG_CT16B6CLKEN_Pos    (8U)
#define SCU_SLP_APB0CLKG_CT16B6CLKEN_Msk    (0x1U << SCU_SLP_APB0CLKG_CT16B6CLKEN_Pos) /*!< 0x00000100 */
#define SCU_SLP_APB0CLKG_CT16B6CLKEN        SCU_SLP_APB0CLKG_CT16B6CLKEN_Msk           /*!< Enable clock for CT16B6 in sleep mode */
#define SCU_SLP_APB0CLKG_CT16B7CLKEN_Pos    (9U)
#define SCU_SLP_APB0CLKG_CT16B7CLKEN_Msk    (0x1U << SCU_SLP_APB0CLKG_CT16B7CLKEN_Pos) /*!< 0x00000200 */
#define SCU_SLP_APB0CLKG_CT16B7CLKEN        SCU_SLP_APB0CLKG_CT16B7CLKEN_Msk           /*!< Enable clock for CT16B7 in sleep mode */
#define SCU_SLP_APB0CLKG_ALWAYSONREGEN_Pos  (10U)
#define SCU_SLP_APB0CLKG_ALWAYSONREGEN_Msk  (0x1U << SCU_SLP_APB0CLKG_ALWAYSONREGEN_Pos) /*!< 0x00000400 */
#define SCU_SLP_APB0CLKG_ALWAYSONREGEN      SCU_SLP_APB0CLKG_ALWAYSONREGEN_Msk           /*!< Enable clock for Always-on domain registers in sleep mode */
#define SCU_SLP_APB0CLKG_CAN0SRAMCLKEN_Pos  (13U)
#define SCU_SLP_APB0CLKG_CAN0SRAMCLKEN_Msk  (0x1U << SCU_SLP_APB0CLKG_CAN0SRAMCLKEN_Pos) /*!< 0x00002000 */
#define SCU_SLP_APB0CLKG_CAN0SRAMCLKEN      SCU_SLP_APB0CLKG_CAN0SRAMCLKEN_Msk           /*!< Enable clock for CAN0 SRAM in sleep mode */
#define SCU_SLP_APB0CLKG_CAN1SRAMCLKEN_Pos  (14U)
#define SCU_SLP_APB0CLKG_CAN1SRAMCLKEN_Msk  (0x1U << SCU_SLP_APB0CLKG_CAN1SRAMCLKEN_Pos) /*!< 0x00004000 */
#define SCU_SLP_APB0CLKG_CAN1SRAMCLKEN      SCU_SLP_APB0CLKG_CAN1SRAMCLKEN_Msk           /*!< Enable clock for CAN1 SRAM in sleep mode */
#define SCU_SLP_APB0CLKG_CLKOUTEN_Pos       (15U)
#define SCU_SLP_APB0CLKG_CLKOUTEN_Msk       (0x1U << SCU_SLP_APB0CLKG_CLKOUTEN_Pos) /*!< 0x00008000 */
#define SCU_SLP_APB0CLKG_CLKOUTEN           SCU_SLP_APB0CLKG_CLKOUTEN_Msk           /*!< Enable clock for CLKOUT in sleep mode */

/* Bits definition for SCU SLP_APB1CLKG register */
#define SCU_SLP_APB1CLKG_ADC0CLKEN_Pos   (0U)
#define SCU_SLP_APB1CLKG_ADC0CLKEN_Msk   (0x1U << SCU_SLP_APB1CLKG_ADC0CLKEN_Pos) /*!< 0x00000001 */
#define SCU_SLP_APB1CLKG_ADC0CLKEN       SCU_SLP_APB1CLKG_ADC0CLKEN_Msk           /*!< Enable clock for ADC0 in sleep mode */
#define SCU_SLP_APB1CLKG_SSP0CLKEN_Pos   (1U)
#define SCU_SLP_APB1CLKG_SSP0CLKEN_Msk   (0x1U << SCU_SLP_APB1CLKG_SSP0CLKEN_Pos) /*!< 0x00000002 */
#define SCU_SLP_APB1CLKG_SSP0CLKEN       SCU_SLP_APB1CLKG_SSP0CLKEN_Msk           /*!< Enable clock for SSP0 in sleep mode */
#define SCU_SLP_APB1CLKG_SSP1CLKEN_Pos   (2U)
#define SCU_SLP_APB1CLKG_SSP1CLKEN_Msk   (0x1U << SCU_SLP_APB1CLKG_SSP1CLKEN_Pos) /*!< 0x00000004 */
#define SCU_SLP_APB1CLKG_SSP1CLKEN       SCU_SLP_APB1CLKG_SSP1CLKEN_Msk           /*!< Enable clock for SSP1 in sleep mode */
#define SCU_SLP_APB1CLKG_SSP2CLKEN_Pos   (3U)
#define SCU_SLP_APB1CLKG_SSP2CLKEN_Msk   (0x1U << SCU_SLP_APB1CLKG_SSP2CLKEN_Pos) /*!< 0x00000008 */
#define SCU_SLP_APB1CLKG_SSP2CLKEN       SCU_SLP_APB1CLKG_SSP2CLKEN_Msk           /*!< Enable clock for SSP2 in sleep mode */
#define SCU_SLP_APB1CLKG_UART0CLKEN_Pos  (4U)
#define SCU_SLP_APB1CLKG_UART0CLKEN_Msk  (0x1U << SCU_SLP_APB1CLKG_UART0CLKEN_Pos) /*!< 0x00000010 */
#define SCU_SLP_APB1CLKG_UART0CLKEN      SCU_SLP_APB1CLKG_UART0CLKEN_Msk           /*!< Enable clock for UART0 in sleep mode */
#define SCU_SLP_APB1CLKG_UART5CLKEN_Pos  (5U)
#define SCU_SLP_APB1CLKG_UART5CLKEN_Msk  (0x1U << SCU_SLP_APB1CLKG_UART5CLKEN_Pos) /*!< 0x00000020 */
#define SCU_SLP_APB1CLKG_UART5CLKEN      SCU_SLP_APB1CLKG_UART5CLKEN_Msk           /*!< Enable clock for UART5 in sleep mode */
#define SCU_SLP_APB1CLKG_I2C0CLKEN_Pos   (6U)
#define SCU_SLP_APB1CLKG_I2C0CLKEN_Msk   (0x1U << SCU_SLP_APB1CLKG_I2C0CLKEN_Pos) /*!< 0x00000040 */
#define SCU_SLP_APB1CLKG_I2C0CLKEN       SCU_SLP_APB1CLKG_I2C0CLKEN_Msk           /*!< Enable clock for I2C0 in sleep mode */
#define SCU_SLP_APB1CLKG_I2C1CLKEN_Pos   (7U)
#define SCU_SLP_APB1CLKG_I2C1CLKEN_Msk   (0x1U << SCU_SLP_APB1CLKG_I2C1CLKEN_Pos) /*!< 0x00000080 */
#define SCU_SLP_APB1CLKG_I2C1CLKEN       SCU_SLP_APB1CLKG_I2C1CLKEN_Msk           /*!< Enable clock for I2C1 in sleep mode */
#define SCU_SLP_APB1CLKG_I2C2CLKEN_Pos   (8U)
#define SCU_SLP_APB1CLKG_I2C2CLKEN_Msk   (0x1U << SCU_SLP_APB1CLKG_I2C2CLKEN_Pos) /*!< 0x00000100 */
#define SCU_SLP_APB1CLKG_I2C2CLKEN       SCU_SLP_APB1CLKG_I2C2CLKEN_Msk           /*!< Enable clock for I2C2 in sleep mode */
#define SCU_SLP_APB1CLKG_CT16B0CLKEN_Pos (9U)
#define SCU_SLP_APB1CLKG_CT16B0CLKEN_Msk (0x1U << SCU_SLP_APB1CLKG_CT16B0CLKEN_Pos) /*!< 0x00000200 */
#define SCU_SLP_APB1CLKG_CT16B0CLKEN     SCU_SLP_APB1CLKG_CT16B0CLKEN_Msk           /*!< Enable clock for CT16B0 in sleep mode */
#define SCU_SLP_APB1CLKG_CT16B1CLKEN_Pos (10U)
#define SCU_SLP_APB1CLKG_CT16B1CLKEN_Msk (0x1U << SCU_SLP_APB1CLKG_CT16B1CLKEN_Pos) /*!< 0x00000400 */
#define SCU_SLP_APB1CLKG_CT16B1CLKEN     SCU_SLP_APB1CLKG_CT16B1CLKEN_Msk           /*!< Enable clock for CT16B1 in sleep mode */
#define SCU_SLP_APB1CLKG_CT16B2CLKEN_Pos (11U)
#define SCU_SLP_APB1CLKG_CT16B2CLKEN_Msk (0x1U << SCU_SLP_APB1CLKG_CT16B2CLKEN_Pos) /*!< 0x00000800 */
#define SCU_SLP_APB1CLKG_CT16B2CLKEN     SCU_SLP_APB1CLKG_CT16B2CLKEN_Msk           /*!< Enable clock for CT16B2 in sleep mode */
#define SCU_SLP_APB1CLKG_CT16B3CLKEN_Pos (12U)
#define SCU_SLP_APB1CLKG_CT16B3CLKEN_Msk (0x1U << SCU_SLP_APB1CLKG_CT16B3CLKEN_Pos) /*!< 0x00001000 */
#define SCU_SLP_APB1CLKG_CT16B3CLKEN     SCU_SLP_APB1CLKG_CT16B3CLKEN_Msk           /*!< Enable clock for CT16B3 in sleep mode */
#define SCU_SLP_APB1CLKG_CT16B4CLKEN_Pos (13U)
#define SCU_SLP_APB1CLKG_CT16B4CLKEN_Msk (0x1U << SCU_SLP_APB1CLKG_CT16B4CLKEN_Pos) /*!< 0x00002000 */
#define SCU_SLP_APB1CLKG_CT16B4CLKEN     SCU_SLP_APB1CLKG_CT16B4CLKEN_Msk           /*!< Enable clock for CT16B4 in sleep mode */
#define SCU_SLP_APB1CLKG_CT16B5CLKEN_Pos (14U)
#define SCU_SLP_APB1CLKG_CT16B5CLKEN_Msk (0x1U << SCU_SLP_APB1CLKG_CT16B5CLKEN_Pos) /*!< 0x00004000 */
#define SCU_SLP_APB1CLKG_CT16B5CLKEN     SCU_SLP_APB1CLKG_CT16B5CLKEN_Msk           /*!< Enable clock for CT16B5 in sleep mode */
#define SCU_SLP_APB1CLKG_CT16B8CLKEN_Pos (15U)
#define SCU_SLP_APB1CLKG_CT16B8CLKEN_Msk (0x1U << SCU_SLP_APB1CLKG_CT16B8CLKEN_Pos) /*!< 0x00008000 */
#define SCU_SLP_APB1CLKG_CT16B8CLKEN     SCU_SLP_APB1CLKG_CT16B8CLKEN_Msk           /*!< Enable clock for CT16B8 in sleep mode */
#define SCU_SLP_APB1CLKG_CRCCLKEN_Pos    (16U)
#define SCU_SLP_APB1CLKG_CRCCLKEN_Msk    (0x1U << SCU_SLP_APB1CLKG_CRCCLKEN_Pos) /*!< 0x00010000 */
#define SCU_SLP_APB1CLKG_CRCCLKEN        SCU_SLP_APB1CLKG_CRCCLKEN_Msk           /*!< Enable clock for CRC in sleep mode */
#define SCU_SLP_APB1CLKG_P0CLKEN_Pos     (17U)
#define SCU_SLP_APB1CLKG_P0CLKEN_Msk     (0x1U << SCU_SLP_APB1CLKG_P0CLKEN_Pos) /*!< 0x00020000 */
#define SCU_SLP_APB1CLKG_P0CLKEN         SCU_SLP_APB1CLKG_P0CLKEN_Msk           /*!< Enable clock for GPIO0 in sleep mode */
#define SCU_SLP_APB1CLKG_P1CLKEN_Pos     (18U)
#define SCU_SLP_APB1CLKG_P1CLKEN_Msk     (0x1U << SCU_SLP_APB1CLKG_P1CLKEN_Pos) /*!< 0x00040000 */
#define SCU_SLP_APB1CLKG_P1CLKEN         SCU_SLP_APB1CLKG_P1CLKEN_Msk           /*!< Enable clock for GPIO1 in sleep mode */
#define SCU_SLP_APB1CLKG_P2CLKEN_Pos     (19U)
#define SCU_SLP_APB1CLKG_P2CLKEN_Msk     (0x1U << SCU_SLP_APB1CLKG_P2CLKEN_Pos) /*!< 0x00080000 */
#define SCU_SLP_APB1CLKG_P2CLKEN         SCU_SLP_APB1CLKG_P2CLKEN_Msk           /*!< Enable clock for GPIO2 in sleep mode */
#define SCU_SLP_APB1CLKG_P3CLKEN_Pos     (20U)
#define SCU_SLP_APB1CLKG_P3CLKEN_Msk     (0x1U << SCU_SLP_APB1CLKG_P3CLKEN_Pos) /*!< 0x00100000 */
#define SCU_SLP_APB1CLKG_P3CLKEN         SCU_SLP_APB1CLKG_P3CLKEN_Msk           /*!< Enable clock for GPIO3 in sleep mode */
#define SCU_SLP_APB1CLKG_LCMCLKEN_Pos    (21U)
#define SCU_SLP_APB1CLKG_LCMCLKEN_Msk    (0x1U << SCU_SLP_APB1CLKG_LCMCLKEN_Pos) /*!< 0x00200000 */
#define SCU_SLP_APB1CLKG_LCMCLKEN        SCU_SLP_APB1CLKG_LCMCLKEN_Msk           /*!< Enable clock for LCM in sleep mode */

/* Bits definition for SCU Sleep Wakeup Events Status register */
#define SCU_SLP_WAKUPST_SLP_WAKUP_INT_Pos (1U)
#define SCU_SLP_WAKUPST_SLP_WAKUP_INT_Msk (0x1U << SCU_SLP_WAKUPST_SLP_WAKUP_INT_Pos) /*!< 0x00000002 */
#define SCU_SLP_WAKUPST_SLP_WAKUP_INT     SCU_SLP_WAKUPST_SLP_WAKUP_INT_Msk           /*!< Wakeup from sleep mode by NVIC interrupts */

/* Bits definition for SCU Sleep Wakeup Events Enable register */
#define SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN_Pos (1U)
#define SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN_Msk (0x1U << SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN_Pos) /*!< 0x00000002 */
#define SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN     SCU_SLP_WAKUPEN_SLP_WAKUP_INTEN_Msk           /*!< Sleep wakeup enable for NVIC interrupts */

/* Bits definition for SCU Power Mode Clear register */
#define SCU_PWRMODEMISC_PWRMODECLR_Pos (12U)
#define SCU_PWRMODEMISC_PWRMODECLR_Msk (0x1U << SCU_PWRMODEMISC_PWRMODECLR_Pos) /*!< 0x00001000 */
#define SCU_PWRMODEMISC_PWRMODECLR     SCU_PWRMODEMISC_PWRMODECLR_Msk           /*!< Writing 0 will clear FCS/SLEEP/DSLEEP/DPD bits of PWRMODE */

/* Bits definition for SCU Peripheral Clock Source Select register */
#define SCU_CLKSEL_CT16B0CLKSEL_Pos  (0U)
#define SCU_CLKSEL_CT16B0CLKSEL_Msk  (0x1U << SCU_CLKSEL_CT16B0CLKSEL_Pos) /*!< 0x00000001 */
#define SCU_CLKSEL_CT16B0CLKSEL      SCU_CLKSEL_CT16B0CLKSEL_Msk           /*!< CT16B0 counter clock source */
#define SCU_CLKSEL_CT16B1CLKSEL_Pos  (1U)
#define SCU_CLKSEL_CT16B1CLKSEL_Msk  (0x1U << SCU_CLKSEL_CT16B1CLKSEL_Pos) /*!< 0x00000002 */
#define SCU_CLKSEL_CT16B1CLKSEL      SCU_CLKSEL_CT16B1CLKSEL_Msk           /*!< CT16B1 counter clock source */
#define SCU_CLKSEL_CT16B2CLKSEL_Pos  (2U)
#define SCU_CLKSEL_CT16B2CLKSEL_Msk  (0x1U << SCU_CLKSEL_CT16B2CLKSEL_Pos) /*!< 0x00000004 */
#define SCU_CLKSEL_CT16B2CLKSEL      SCU_CLKSEL_CT16B2CLKSEL_Msk           /*!< CT16B2 counter clock source */
#define SCU_CLKSEL_CT16B3CLKSEL_Pos  (3U)
#define SCU_CLKSEL_CT16B3CLKSEL_Msk  (0x1U << SCU_CLKSEL_CT16B3CLKSEL_Pos) /*!< 0x00000008 */
#define SCU_CLKSEL_CT16B3CLKSEL      SCU_CLKSEL_CT16B3CLKSEL_Msk           /*!< CT16B3 counter clock source */
#define SCU_CLKSEL_CT16B4CLKSEL_Pos  (4U)
#define SCU_CLKSEL_CT16B4CLKSEL_Msk  (0x1U << SCU_CLKSEL_CT16B4CLKSEL_Pos) /*!< 0x00000010 */
#define SCU_CLKSEL_CT16B4CLKSEL      SCU_CLKSEL_CT16B4CLKSEL_Msk           /*!< CT16B4 counter clock source */
#define SCU_CLKSEL_CT16B5CLKSEL_Pos  (5U)
#define SCU_CLKSEL_CT16B5CLKSEL_Msk  (0x3U << SCU_CLKSEL_CT16B5CLKSEL_Pos) /*!< 0x00000060 */
#define SCU_CLKSEL_CT16B5CLKSEL      SCU_CLKSEL_CT16B5CLKSEL_Msk           /*!< CT16B5 counter clock source */
#define SCU_CLKSEL_CT16B6CLKSEL_Pos  (7U)
#define SCU_CLKSEL_CT16B6CLKSEL_Msk  (0x1U << SCU_CLKSEL_CT16B6CLKSEL_Pos) /*!< 0x00000080 */
#define SCU_CLKSEL_CT16B6CLKSEL      SCU_CLKSEL_CT16B6CLKSEL_Msk           /*!< CT16B6 counter clock source */
#define SCU_CLKSEL_CT16B7CLKSEL_Pos  (8U)
#define SCU_CLKSEL_CT16B7CLKSEL_Msk  (0x1U << SCU_CLKSEL_CT16B7CLKSEL_Pos) /*!< 0x00000100 */
#define SCU_CLKSEL_CT16B7CLKSEL      SCU_CLKSEL_CT16B7CLKSEL_Msk           /*!< CT16B7 counter clock source */
#define SCU_CLKSEL_CT16B8CLKSEL_Pos  (9U)
#define SCU_CLKSEL_CT16B8CLKSEL_Msk  (0x1U << SCU_CLKSEL_CT16B8CLKSEL_Pos) /*!< 0x00000200 */
#define SCU_CLKSEL_CT16B8CLKSEL      SCU_CLKSEL_CT16B8CLKSEL_Msk           /*!< CT16B8 counter clock source */
#define SCU_CLKSEL_UART0CLKSEL_Pos   (10U)
#define SCU_CLKSEL_UART0CLKSEL_Msk   (0x1U << SCU_CLKSEL_UART0CLKSEL_Pos) /*!< 0x00000400 */
#define SCU_CLKSEL_UART0CLKSEL       SCU_CLKSEL_UART0CLKSEL_Msk           /*!< UART0 UCLK clock source */
#define SCU_CLKSEL_UART1CLKSEL_Pos   (11U)
#define SCU_CLKSEL_UART1CLKSEL_Msk   (0x1U << SCU_CLKSEL_UART1CLKSEL_Pos) /*!< 0x00000800 */
#define SCU_CLKSEL_UART1CLKSEL       SCU_CLKSEL_UART1CLKSEL_Msk           /*!< UART1 UCLK clock source */
#define SCU_CLKSEL_UART2CLKSEL_Pos   (12U)
#define SCU_CLKSEL_UART2CLKSEL_Msk   (0x1U << SCU_CLKSEL_UART2CLKSEL_Pos) /*!< 0x00001000 */
#define SCU_CLKSEL_UART2CLKSEL       SCU_CLKSEL_UART2CLKSEL_Msk           /*!< UART2 UCLK clock source */
#define SCU_CLKSEL_UART3CLKSEL_Pos   (13U)
#define SCU_CLKSEL_UART3CLKSEL_Msk   (0x1U << SCU_CLKSEL_UART3CLKSEL_Pos) /*!< 0x00002000 */
#define SCU_CLKSEL_UART3CLKSEL       SCU_CLKSEL_UART3CLKSEL_Msk           /*!< UART3 UCLK clock source */
#define SCU_CLKSEL_UART4CLKSEL_Pos   (14U)
#define SCU_CLKSEL_UART4CLKSEL_Msk   (0x1U << SCU_CLKSEL_UART4CLKSEL_Pos) /*!< 0x00004000 */
#define SCU_CLKSEL_UART4CLKSEL       SCU_CLKSEL_UART4CLKSEL_Msk           /*!< UART4 UCLK clock source */
#define SCU_CLKSEL_UART5CLKSEL_Pos   (15U)
#define SCU_CLKSEL_UART5CLKSEL_Msk   (0x1U << SCU_CLKSEL_UART5CLKSEL_Pos) /*!< 0x00008000 */
#define SCU_CLKSEL_UART5CLKSEL       SCU_CLKSEL_UART5CLKSEL_Msk           /*!< UART5 UCLK clock source */
#define SCU_CLKSEL_SSP0CLKSEL_Pos    (16U)
#define SCU_CLKSEL_SSP0CLKSEL_Msk    (0x1U << SCU_CLKSEL_SSP0CLKSEL_Pos) /*!< 0x00010000 */
#define SCU_CLKSEL_SSP0CLKSEL        SCU_CLKSEL_SSP0CLKSEL_Msk           /*!< SSP0 SSPCLK clock source */
#define SCU_CLKSEL_SSP1CLKSEL_Pos    (17U)
#define SCU_CLKSEL_SSP1CLKSEL_Msk    (0x1U << SCU_CLKSEL_SSP1CLKSEL_Pos) /*!< 0x00020000 */
#define SCU_CLKSEL_SSP1CLKSEL        SCU_CLKSEL_SSP1CLKSEL_Msk           /*!< SSP1 SSPCLK clock source */
#define SCU_CLKSEL_SSP2CLKSEL_Pos    (18U)
#define SCU_CLKSEL_SSP2CLKSEL_Msk    (0x1U << SCU_CLKSEL_SSP2CLKSEL_Pos) /*!< 0x00040000 */
#define SCU_CLKSEL_SSP2CLKSEL        SCU_CLKSEL_SSP2CLKSEL_Msk           /*!< SSP2 SSPCLK clock source */
#define SCU_CLKSEL_SDIOCLKSEL_Pos    (19U)
#define SCU_CLKSEL_SDIOCLKSEL_Msk    (0x1U << SCU_CLKSEL_SDIOCLKSEL_Pos) /*!< 0x00080000 */
#define SCU_CLKSEL_SDIOCLKSEL        SCU_CLKSEL_SDIOCLKSEL_Msk           /*!< SDIO clock source */
#define SCU_CLKSEL_WWDTCLKSEL_Pos    (21U)
#define SCU_CLKSEL_WWDTCLKSEL_Msk    (0x1U << SCU_CLKSEL_WWDTCLKSEL_Pos) /*!< 0x00200000 */
#define SCU_CLKSEL_WWDTCLKSEL        SCU_CLKSEL_WWDTCLKSEL_Msk           /*!< WWDT clock source */
#define SCU_CLKSEL_LCMCLKSEL_Pos     (22U)
#define SCU_CLKSEL_LCMCLKSEL_Msk     (0x1U << SCU_CLKSEL_LCMCLKSEL_Pos) /*!< 0x00400000 */
#define SCU_CLKSEL_LCMCLKSEL         SCU_CLKSEL_LCMCLKSEL_Msk           /*!< LCM clock source */
#define SCU_CLKSEL_CLKOUTSEL_Pos     (29U)
#define SCU_CLKSEL_CLKOUTSEL_Msk     (0x7U << SCU_CLKSEL_CLKOUTSEL_Pos) /*!< 0xE0000000 */
#define SCU_CLKSEL_CLKOUTSEL         SCU_CLKSEL_CLKOUTSEL_Msk           /*!< Clock output source */
#define SCU_CLKSEL_CLKOUTSEL_DISABLE (0x0U << SCU_CLKSEL_CLKOUTSEL_Pos) /*!< Clock Output source: Disable */
#define SCU_CLKSEL_CLKOUTSEL_ILRC    (0x1U << SCU_CLKSEL_CLKOUTSEL_Pos) /*!< Clock Output source: ILRC */
#define SCU_CLKSEL_CLKOUTSEL_ELS     (0x2U << SCU_CLKSEL_CLKOUTSEL_Pos) /*!< Clock Output source: ELS */
#define SCU_CLKSEL_CLKOUTSEL_HCLK    (0x4U << SCU_CLKSEL_CLKOUTSEL_Pos) /*!< Clock Output source: HCLK */
#define SCU_CLKSEL_CLKOUTSEL_IHRC    (0x5U << SCU_CLKSEL_CLKOUTSEL_Pos) /*!< Clock Output source: IHRC */
#define SCU_CLKSEL_CLKOUTSEL_EHS     (0x6U << SCU_CLKSEL_CLKOUTSEL_Pos) /*!< Clock Output source: EHS */
#define SCU_CLKSEL_CLKOUTSEL_PLL     (0x7U << SCU_CLKSEL_CLKOUTSEL_Pos) /*!< Clock Output source: PLL */

/* Bits definition for SCU CLKPRE register */
#define SCU_CLKPRE_AHBPRE_Pos        (0U)
#define SCU_CLKPRE_AHBPRE_Msk        (0x7U << SCU_CLKPRE_AHBPRE_Pos) /*!< 0x00000007 */
#define SCU_CLKPRE_AHBPRE            SCU_CLKPRE_AHBPRE_Msk           /*!< HCLK pre-scaler */
#define SCU_CLKPRE_AHBPRE_Div_1      (0x0U << SCU_CLKPRE_AHBPRE_Pos) /*!< HCLK = SYSCLK/1 */
#define SCU_CLKPRE_AHBPRE_Div_2      (0x1U << SCU_CLKPRE_AHBPRE_Pos) /*!< HCLK = SYSCLK/2 */
#define SCU_CLKPRE_AHBPRE_Div_4      (0x2U << SCU_CLKPRE_AHBPRE_Pos) /*!< HCLK = SYSCLK/4 */
#define SCU_CLKPRE_AHBPRE_Div_8      (0x3U << SCU_CLKPRE_AHBPRE_Pos) /*!< HCLK = SYSCLK/8 */
#define SCU_CLKPRE_AHBPRE_Div_16     (0x4U << SCU_CLKPRE_AHBPRE_Pos) /*!< HCLK = SYSCLK/16 */
#define SCU_CLKPRE_AHBPRE_Div_32     (0x5U << SCU_CLKPRE_AHBPRE_Pos) /*!< HCLK = SYSCLK/32 */
#define SCU_CLKPRE_AHBPRE_Div_64     (0x6U << SCU_CLKPRE_AHBPRE_Pos) /*!< HCLK = SYSCLK/64 */
#define SCU_CLKPRE_AHBPRE_Div_128    (0x7U << SCU_CLKPRE_AHBPRE_Pos) /*!< HCLK = SYSCLK/128 */
#define SCU_CLKPRE_APB0PRE_Pos       (3U)
#define SCU_CLKPRE_APB0PRE_Msk       (0x7U << SCU_CLKPRE_APB0PRE_Pos) /*!< 0x0000003C */
#define SCU_CLKPRE_APB0PRE           SCU_CLKPRE_APB0PRE_Msk           /*!< APB0CLK pre-scaler */
#define SCU_CLKPRE_APB0PRE_Div_1     (0x0U << SCU_CLKPRE_APB0PRE_Pos) /*!< APB0CLK = HCLK/1 */
#define SCU_CLKPRE_APB0PRE_Div_2     (0x1U << SCU_CLKPRE_APB0PRE_Pos) /*!< APB0CLK = HCLK/2 */
#define SCU_CLKPRE_APB0PRE_Div_4     (0x2U << SCU_CLKPRE_APB0PRE_Pos) /*!< APB0CLK = HCLK/4 */
#define SCU_CLKPRE_APB0PRE_Div_8     (0x3U << SCU_CLKPRE_APB0PRE_Pos) /*!< APB0CLK = HCLK/8 */
#define SCU_CLKPRE_APB0PRE_Div_16    (0x4U << SCU_CLKPRE_APB0PRE_Pos) /*!< APB0CLK = HCLK/16 */
#define SCU_CLKPRE_APB0PRE_Div_32    (0x5U << SCU_CLKPRE_APB0PRE_Pos) /*!< APB0CLK = HCLK/32 */
#define SCU_CLKPRE_APB0PRE_Div_64    (0x6U << SCU_CLKPRE_APB0PRE_Pos) /*!< APB0CLK = HCLK/64 */
#define SCU_CLKPRE_APB0PRE_Div_128   (0x7U << SCU_CLKPRE_APB0PRE_Pos) /*!< APB0CLK = HCLK/128 */
#define SCU_CLKPRE_APB1PRE_Pos       (6U)
#define SCU_CLKPRE_APB1PRE_Msk       (0x7U << SCU_CLKPRE_APB1PRE_Pos) /*!< 0x000001C0 */
#define SCU_CLKPRE_APB1PRE           SCU_CLKPRE_APB1PRE_Msk           /*!< APB1CLK pre-scaler */
#define SCU_CLKPRE_APB1PRE_Div_1     (0x0U << SCU_CLKPRE_APB1PRE_Pos) /*!< APB1CLK = HCLK/1 */
#define SCU_CLKPRE_APB1PRE_Div_2     (0x1U << SCU_CLKPRE_APB1PRE_Pos) /*!< APB1CLK = HCLK/2 */
#define SCU_CLKPRE_APB1PRE_Div_4     (0x2U << SCU_CLKPRE_APB1PRE_Pos) /*!< APB1CLK = HCLK/4 */
#define SCU_CLKPRE_APB1PRE_Div_8     (0x3U << SCU_CLKPRE_APB1PRE_Pos) /*!< APB1CLK = HCLK/8 */
#define SCU_CLKPRE_APB1PRE_Div_16    (0x4U << SCU_CLKPRE_APB1PRE_Pos) /*!< APB1CLK = HCLK/16 */
#define SCU_CLKPRE_APB1PRE_Div_32    (0x5U << SCU_CLKPRE_APB1PRE_Pos) /*!< APB1CLK = HCLK/32 */
#define SCU_CLKPRE_APB1PRE_Div_64    (0x6U << SCU_CLKPRE_APB1PRE_Pos) /*!< APB1CLK = HCLK/64 */
#define SCU_CLKPRE_APB1PRE_Div_128   (0x7U << SCU_CLKPRE_APB1PRE_Pos) /*!< APB1CLK = HCLK/128 */
#define SCU_CLKPRE_CLKOUTPRE_Pos     (9U)
#define SCU_CLKPRE_CLKOUTPRE_Msk     (0xFU << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< 0x00001E00 */
#define SCU_CLKPRE_CLKOUTPRE         SCU_CLKPRE_CLKOUTPRE_Msk           /*!< CLKOUT pre-scaler */
#define SCU_CLKPRE_CLKOUTPRE_Div_1   (0x0U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/1 */
#define SCU_CLKPRE_CLKOUTPRE_Div_2   (0x1U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/2 */
#define SCU_CLKPRE_CLKOUTPRE_Div_3   (0x2U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/3 */
#define SCU_CLKPRE_CLKOUTPRE_Div_4   (0x3U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/4 */
#define SCU_CLKPRE_CLKOUTPRE_Div_6   (0x4U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/6 */
#define SCU_CLKPRE_CLKOUTPRE_Div_8   (0x5U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/8 */
#define SCU_CLKPRE_CLKOUTPRE_Div_16  (0x6U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/16 */
#define SCU_CLKPRE_CLKOUTPRE_Div_32  (0x7U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/32 */
#define SCU_CLKPRE_CLKOUTPRE_Div_64  (0x8U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/64 */
#define SCU_CLKPRE_CLKOUTPRE_Div_128 (0x9U << SCU_CLKPRE_CLKOUTPRE_Pos) /*!< CLKOUT = clock source/128 */
#define SCU_CLKPRE_WRPKEY_Pos        (16U)
#define SCU_CLKPRE_WRPKEY_Msk        (0xFFFFU << SCU_CLKPRE_WRPKEY_Pos) /*!< 0xFFFF0000 */
#define SCU_CLKPRE_WRPKEY            SCU_CLKPRE_WRPKEY_Msk              /*!< Write Protect key */

/* Bits definition for SCU IHRCCTRL register */
#define SCU_IHRCCTRL_IHRCEN_Pos  (0U)
#define SCU_IHRCCTRL_IHRCEN_Msk  (0x1U << SCU_IHRCCTRL_IHRCEN_Pos) /*!< 0x00000001 */
#define SCU_IHRCCTRL_IHRCEN      SCU_IHRCCTRL_IHRCEN_Msk           /*!< Internal high-speed clock enable */
#define SCU_IHRCCTRL_IHRCRDY_Pos (1U)
#define SCU_IHRCCTRL_IHRCRDY_Msk (0x1U << SCU_IHRCCTRL_IHRCRDY_Pos) /*!< 0x00000002 */
#define SCU_IHRCCTRL_IHRCRDY     SCU_IHRCCTRL_IHRCRDY_Msk           /*!< IHRC ready flag */

/* Bits definition for SCU PLLSTS register */
#define SCU_PLLSTS_PLLENSTS_Pos   (0U)
#define SCU_PLLSTS_PLLENSTS_Msk   (0x1U << SCU_PLLSTS_PLLENSTS_Pos) /*!< 0x00000001 */
#define SCU_PLLSTS_PLLENSTS       SCU_PLLSTS_PLLENSTS_Msk           /*!< PLL enable status */
#define SCU_PLLSTS_PLLCLKSTS_Pos  (4U)
#define SCU_PLLSTS_PLLCLKSTS_Msk  (0x1U << SCU_PLLSTS_PLLCLKSTS_Pos) /*!< 0x00000010 */
#define SCU_PLLSTS_PLLCLKSTS      SCU_PLLSTS_PLLCLKSTS_Msk           /*!< PLL clock status */
#define SCU_PLLSTS_SYSCLKSTS_Pos  (5U)
#define SCU_PLLSTS_SYSCLKSTS_Msk  (0x7U << SCU_PLLSTS_SYSCLKSTS_Pos) /*!< 0x000000E0 */
#define SCU_PLLSTS_SYSCLKSTS      SCU_PLLSTS_SYSCLKSTS_Msk           /*!< System clock status */
#define SCU_PLLSTS_SYSCLKSTS_IHRC (0x0U << SCU_PLLSTS_SYSCLKSTS_Pos) /*!< IHRC */
#define SCU_PLLSTS_SYSCLKSTS_EHS  (0x2U << SCU_PLLSTS_SYSCLKSTS_Pos) /*!< EHS */
#define SCU_PLLSTS_SYSCLKSTS_PLL  (0x4U << SCU_PLLSTS_SYSCLKSTS_Pos) /*!< PLL */
#define SCU_PLLSTS_FSSTS_Pos      (8U)
#define SCU_PLLSTS_FSSTS_Msk      (0x3U << SCU_PLLSTS_FSSTS_Pos) /*!< 0x00000300 */
#define SCU_PLLSTS_FSSTS          SCU_PLLSTS_FSSTS_Msk           /*!< PLL frequency range output divider status */
#define SCU_PLLSTS_FSSTS_Div_32   (0x0U << SCU_PLLSTS_FSSTS_Pos) /*!< PLL CKOUT= PLLCLK / 32 */
#define SCU_PLLSTS_FSSTS_Div_16   (0x1U << SCU_PLLSTS_FSSTS_Pos) /*!< PLL CKOUT= PLLCLK / 16 */
#define SCU_PLLSTS_FSSTS_Div_8    (0x2U << SCU_PLLSTS_FSSTS_Pos) /*!< PLL CKOUT= PLLCLK / 8 */
#define SCU_PLLSTS_FSSTS_Div_4    (0x3U << SCU_PLLSTS_FSSTS_Pos) /*!< PLL CKOUT= PLLCLK / 4 */
#define SCU_PLLSTS_NSSTS_Pos      (24U)
#define SCU_PLLSTS_NSSTS_Msk      (0x7FU << SCU_PLLSTS_NSSTS_Pos) /*!< 0x7F000000 */
#define SCU_PLLSTS_NSSTS          SCU_PLLSTS_NSSTS_Msk            /*!< 7-bit programmable loop divider status */

/* Bits definition for SCU AHBRST register */
#define SCU_AHBRST_DMA0RST_Pos  (0U)
#define SCU_AHBRST_DMA0RST_Msk  (0x1U << SCU_AHBRST_DMA0RST_Pos) /*!< 0x00000001 */
#define SCU_AHBRST_DMA0RST      SCU_AHBRST_DMA0RST_Msk           /*!< DMA0 reset control */
#define SCU_AHBRST_DMA1RST_Pos  (1U)
#define SCU_AHBRST_DMA1RST_Msk  (0x1U << SCU_AHBRST_DMA1RST_Pos) /*!< 0x00000002 */
#define SCU_AHBRST_DMA1RST      SCU_AHBRST_DMA1RST_Msk           /*!< DMA1 reset control */
#define SCU_AHBRST_SDIORST_Pos  (6U)
#define SCU_AHBRST_SDIORST_Msk  (0x1U << SCU_AHBRST_SDIORST_Pos) /*!< 0x00000080 */
#define SCU_AHBRST_SDIORST      SCU_AHBRST_SDIORST_Msk           /*!< SDIO reset control */
#define SCU_AHBRST_ETHRST_Pos   (8U)
#define SCU_AHBRST_ETHRST_Msk   (0x1U << SCU_AHBRST_ETHRST_Pos) /*!< 0x00000200 */
#define SCU_AHBRST_ETHRST       SCU_AHBRST_ETHRST_Msk           /*!< Ethernet MAC reset control */

/* Bits definition for SCU APB0RST register */
#define SCU_APB0RST_WDTRST_Pos    (0U)
#define SCU_APB0RST_WDTRST_Msk    (0x1U << SCU_APB0RST_WDTRST_Pos) /*!< 0x00000001 */
#define SCU_APB0RST_WDTRST        SCU_APB0RST_WDTRST_Msk           /*!< WDT reset control */
#define SCU_APB0RST_WWDTRST_Pos   (1U)
#define SCU_APB0RST_WWDTRST_Msk   (0x1U << SCU_APB0RST_WWDTRST_Pos) /*!< 0x00000002 */
#define SCU_APB0RST_WWDTRST       SCU_APB0RST_WWDTRST_Msk           /*!< WWDT reset control */
#define SCU_APB0RST_CAN0RST_Pos   (2U)
#define SCU_APB0RST_CAN0RST_Msk   (0x1U << SCU_APB0RST_CAN0RST_Pos) /*!< 0x00000004 */
#define SCU_APB0RST_CAN0RST       SCU_APB0RST_CAN0RST_Msk           /*!< CAN0 reset control */
#define SCU_APB0RST_CAN1RST_Pos   (3U)
#define SCU_APB0RST_CAN1RST_Msk   (0x1U << SCU_APB0RST_CAN1RST_Pos) /*!< 0x00000008 */
#define SCU_APB0RST_CAN1RST       SCU_APB0RST_CAN1RST_Msk           /*!< CAN1 reset control */
#define SCU_APB0RST_UART1RST_Pos  (4U)
#define SCU_APB0RST_UART1RST_Msk  (0x1U << SCU_APB0RST_UART1RST_Pos) /*!< 0x00000010 */
#define SCU_APB0RST_UART1RST      SCU_APB0RST_UART1RST_Msk           /*!< UART1 reset control */
#define SCU_APB0RST_UART2RST_Pos  (5U)
#define SCU_APB0RST_UART2RST_Msk  (0x1U << SCU_APB0RST_UART2RST_Pos) /*!< 0x00000020 */
#define SCU_APB0RST_UART2RST      SCU_APB0RST_UART2RST_Msk           /*!< UART2 reset control */
#define SCU_APB0RST_UART3RST_Pos  (6U)
#define SCU_APB0RST_UART3RST_Msk  (0x1U << SCU_APB0RST_UART3RST_Pos) /*!< 0x00000040 */
#define SCU_APB0RST_UART3RST      SCU_APB0RST_UART3RST_Msk           /*!< UART3 reset control */
#define SCU_APB0RST_UART4RST_Pos  (7U)
#define SCU_APB0RST_UART4RST_Msk  (0x1U << SCU_APB0RST_UART4RST_Pos) /*!< 0x00000080 */
#define SCU_APB0RST_UART4RST      SCU_APB0RST_UART4RST_Msk           /*!< UART4 reset control */
#define SCU_APB0RST_CT16B6RST_Pos (8U)
#define SCU_APB0RST_CT16B6RST_Msk (0x1U << SCU_APB0RST_CT16B6RST_Pos) /*!< 0x00000100 */
#define SCU_APB0RST_CT16B6RST     SCU_APB0RST_CT16B6RST_Msk           /*!< CT16B6 reset control */
#define SCU_APB0RST_CT16B7RST_Pos (9U)
#define SCU_APB0RST_CT16B7RST_Msk (0x1U << SCU_APB0RST_CT16B7RST_Pos) /*!< 0x00000200 */
#define SCU_APB0RST_CT16B7RST     SCU_APB0RST_CT16B7RST_Msk           /*!< CT16B7 reset control */

/* Bits definition for SCU APB1RST register */
#define SCU_APB1RST_ADC0RST_Pos   (0U)
#define SCU_APB1RST_ADC0RST_Msk   (0x1U << SCU_APB1RST_ADC0RST_Pos) /*!< 0x00000001 */
#define SCU_APB1RST_ADC0RST       SCU_APB1RST_ADC0RST_Msk           /*!< ADC0 reset control */
#define SCU_APB1RST_SSP0RST_Pos   (1U)
#define SCU_APB1RST_SSP0RST_Msk   (0x1U << SCU_APB1RST_SSP0RST_Pos) /*!< 0x00000002 */
#define SCU_APB1RST_SSP0RST       SCU_APB1RST_SSP0RST_Msk           /*!< SSP0 reset control */
#define SCU_APB1RST_SSP1RST_Pos   (2U)
#define SCU_APB1RST_SSP1RST_Msk   (0x1U << SCU_APB1RST_SSP1RST_Pos) /*!< 0x00000004 */
#define SCU_APB1RST_SSP1RST       SCU_APB1RST_SSP1RST_Msk           /*!< SSP1 reset control */
#define SCU_APB1RST_SSP2RST_Pos   (3U)
#define SCU_APB1RST_SSP2RST_Msk   (0x1U << SCU_APB1RST_SSP2RST_Pos) /*!< 0x00000008 */
#define SCU_APB1RST_SSP2RST       SCU_APB1RST_SSP2RST_Msk           /*!< SSP2 reset control */
#define SCU_APB1RST_UART0RST_Pos  (4U)
#define SCU_APB1RST_UART0RST_Msk  (0x1U << SCU_APB1RST_UART0RST_Pos) /*!< 0x00000010 */
#define SCU_APB1RST_UART0RST      SCU_APB1RST_UART0RST_Msk           /*!< UART0 reset control */
#define SCU_APB1RST_UART5RST_Pos  (5U)
#define SCU_APB1RST_UART5RST_Msk  (0x1U << SCU_APB1RST_UART5RST_Pos) /*!< 0x00000020 */
#define SCU_APB1RST_UART5RST      SCU_APB1RST_UART5RST_Msk           /*!< UART5 reset control */
#define SCU_APB1RST_I2C0RST_Pos   (6U)
#define SCU_APB1RST_I2C0RST_Msk   (0x1U << SCU_APB1RST_I2C0RST_Pos) /*!< 0x00000040 */
#define SCU_APB1RST_I2C0RST       SCU_APB1RST_I2C0RST_Msk           /*!< I2C0 reset control */
#define SCU_APB1RST_I2C1RST_Pos   (7U)
#define SCU_APB1RST_I2C1RST_Msk   (0x1U << SCU_APB1RST_I2C1RST_Pos) /*!< 0x00000080 */
#define SCU_APB1RST_I2C1RST       SCU_APB1RST_I2C1RST_Msk           /*!< I2C1 reset control */
#define SCU_APB1RST_I2C2RST_Pos   (8U)
#define SCU_APB1RST_I2C2RST_Msk   (0x1U << SCU_APB1RST_I2C2RST_Pos) /*!< 0x00000100 */
#define SCU_APB1RST_I2C2RST       SCU_APB1RST_I2C2RST_Msk           /*!< I2C2 reset control */
#define SCU_APB1RST_CT16B0RST_Pos (9U)
#define SCU_APB1RST_CT16B0RST_Msk (0x1U << SCU_APB1RST_CT16B0RST_Pos) /*!< 0x00000200 */
#define SCU_APB1RST_CT16B0RST     SCU_APB1RST_CT16B0RST_Msk           /*!< CT16B0 reset control */
#define SCU_APB1RST_CT16B1RST_Pos (10U)
#define SCU_APB1RST_CT16B1RST_Msk (0x1U << SCU_APB1RST_CT16B1RST_Pos) /*!< 0x00000400 */
#define SCU_APB1RST_CT16B1RST     SCU_APB1RST_CT16B1RST_Msk           /*!< CT16B1 reset control */
#define SCU_APB1RST_CT16B2RST_Pos (11U)
#define SCU_APB1RST_CT16B2RST_Msk (0x1U << SCU_APB1RST_CT16B2RST_Pos) /*!< 0x00000800 */
#define SCU_APB1RST_CT16B2RST     SCU_APB1RST_CT16B2RST_Msk           /*!< CT16B2 reset control */
#define SCU_APB1RST_CT16B3RST_Pos (12U)
#define SCU_APB1RST_CT16B3RST_Msk (0x1U << SCU_APB1RST_CT16B3RST_Pos) /*!< 0x00001000 */
#define SCU_APB1RST_CT16B3RST     SCU_APB1RST_CT16B3RST_Msk           /*!< CT16B3 reset control */
#define SCU_APB1RST_CT16B4RST_Pos (13U)
#define SCU_APB1RST_CT16B4RST_Msk (0x1U << SCU_APB1RST_CT16B4RST_Pos) /*!< 0x00002000 */
#define SCU_APB1RST_CT16B4RST     SCU_APB1RST_CT16B4RST_Msk           /*!< CT16B4 reset control */
#define SCU_APB1RST_CT16B5RST_Pos (14U)
#define SCU_APB1RST_CT16B5RST_Msk (0x1U << SCU_APB1RST_CT16B5RST_Pos) /*!< 0x00004000 */
#define SCU_APB1RST_CT16B5RST     SCU_APB1RST_CT16B5RST_Msk           /*!< CT16B5 reset control */
#define SCU_APB1RST_CT16B8RST_Pos (15U)
#define SCU_APB1RST_CT16B8RST_Msk (0x1U << SCU_APB1RST_CT16B8RST_Pos) /*!< 0x00008000 */
#define SCU_APB1RST_CT16B8RST     SCU_APB1RST_CT16B8RST_Msk           /*!< CT16B8 reset control */
#define SCU_APB1RST_CRCRST_Pos    (16U)
#define SCU_APB1RST_CRCRST_Msk    (0x1U << SCU_APB1RST_CRCRST_Pos) /*!< 0x00010000 */
#define SCU_APB1RST_CRCRST        SCU_APB1RST_CRCRST_Msk           /*!< CRC reset control */
#define SCU_APB1RST_GPIO0RST_Pos  (17U)
#define SCU_APB1RST_GPIO0RST_Msk  (0x1U << SCU_APB1RST_GPIO0RST_Pos) /*!< 0x00020000 */
#define SCU_APB1RST_GPIO0RST      SCU_APB1RST_GPIO0RST_Msk           /*!< GPIO0 reset control */
#define SCU_APB1RST_GPIO1RST_Pos  (18U)
#define SCU_APB1RST_GPIO1RST_Msk  (0x1U << SCU_APB1RST_GPIO1RST_Pos) /*!< 0x00040000 */
#define SCU_APB1RST_GPIO1RST      SCU_APB1RST_GPIO1RST_Msk           /*!< GPIO1 reset control */
#define SCU_APB1RST_GPIO2RST_Pos  (19U)
#define SCU_APB1RST_GPIO2RST_Msk  (0x1U << SCU_APB1RST_GPIO2RST_Pos) /*!< 0x00080000 */
#define SCU_APB1RST_GPIO2RST      SCU_APB1RST_GPIO2RST_Msk           /*!< GPIO2 reset control */
#define SCU_APB1RST_GPIO3RST_Pos  (20U)
#define SCU_APB1RST_GPIO3RST_Msk  (0x1U << SCU_APB1RST_GPIO3RST_Pos) /*!< 0x00100000 */
#define SCU_APB1RST_GPIO3RST      SCU_APB1RST_GPIO3RST_Msk           /*!< GPIO3 reset control */
#define SCU_APB1RST_LCMRST_Pos    (21U)
#define SCU_APB1RST_LCMRST_Msk    (0x1U << SCU_APB1RST_LCMRST_Pos) /*!< 0x00200000 */
#define SCU_APB1RST_LCMRST        SCU_APB1RST_LCMRST_Msk           /*!< LCM reset control */

/* Bits definition for SCU PRECTRL register */
#define SCU_PRECTRL_LCMPRE_Pos    (0U)
#define SCU_PRECTRL_LCMPRE_Msk    (0x7U << SCU_PRECTRL_LCMPRE_Pos) /*!< 0x00000007 */
#define SCU_PRECTRL_LCMPRE        SCU_PRECTRL_LCMPRE_Msk           /*!< LCM clock pre-scaler */
#define SCU_PRECTRL_LCMPRE_Div_1  (0x0U << SCU_PRECTRL_LCMPRE_Pos) /*!< LCM clock = LCMCLK / 1 */
#define SCU_PRECTRL_LCMPRE_Div_2  (0x1U << SCU_PRECTRL_LCMPRE_Pos) /*!< LCM clock = LCMCLK / 2 */
#define SCU_PRECTRL_LCMPRE_Div_4  (0x2U << SCU_PRECTRL_LCMPRE_Pos) /*!< LCM clock = LCMCLK / 4 */
#define SCU_PRECTRL_LCMPRE_Div_8  (0x3U << SCU_PRECTRL_LCMPRE_Pos) /*!< LCM clock = LCMCLK / 8 */
#define SCU_PRECTRL_LCMPRE_Div_16 (0x4U << SCU_PRECTRL_LCMPRE_Pos) /*!< LCM clock = LCMCLK / 16 */

/* Bits definition for SCU PnSTR register */
#define SCU_PnSTR_STR0_Pos   (0U)
#define SCU_PnSTR_STR0_Msk   (0x3U << SCU_PnSTR_STR0_Pos) /*!< 0x00000003 */
#define SCU_PnSTR_STR0       SCU_PnSTR_STR0_Msk           /*!< Pn.0 driving/sinking strength */
#define SCU_PnSTR_STR0_17mA  (0x0U << SCU_PnSTR_STR0_Pos) /*!< 17mA */
#define SCU_PnSTR_STR0_19mA  (0x1U << SCU_PnSTR_STR0_Pos) /*!< 19mA */
#define SCU_PnSTR_STR0_21mA  (0x2U << SCU_PnSTR_STR0_Pos) /*!< 21mA */
#define SCU_PnSTR_STR0_23mA  (0x3U << SCU_PnSTR_STR0_Pos) /*!< 23mA */
#define SCU_PnSTR_STR1_Pos   (2U)
#define SCU_PnSTR_STR1_Msk   (0x3U << SCU_PnSTR_STR1_Pos) /*!< 0x0000000C */
#define SCU_PnSTR_STR1       SCU_PnSTR_STR1_Msk           /*!< Pn.1 driving/sinking strength */
#define SCU_PnSTR_STR1_17mA  (0x0U << SCU_PnSTR_STR1_Pos) /*!< 17mA */
#define SCU_PnSTR_STR1_19mA  (0x1U << SCU_PnSTR_STR1_Pos) /*!< 19mA */
#define SCU_PnSTR_STR1_21mA  (0x2U << SCU_PnSTR_STR1_Pos) /*!< 21mA */
#define SCU_PnSTR_STR1_23mA  (0x3U << SCU_PnSTR_STR1_Pos) /*!< 23mA */
#define SCU_PnSTR_STR2_Pos   (4U)
#define SCU_PnSTR_STR2_Msk   (0x3U << SCU_PnSTR_STR2_Pos) /*!< 0x00000030 */
#define SCU_PnSTR_STR2       SCU_PnSTR_STR2_Msk           /*!< Pn.2 driving/sinking strength */
#define SCU_PnSTR_STR2_17mA  (0x0U << SCU_PnSTR_STR2_Pos) /*!< 17mA */
#define SCU_PnSTR_STR2_19mA  (0x1U << SCU_PnSTR_STR2_Pos) /*!< 19mA */
#define SCU_PnSTR_STR2_21mA  (0x2U << SCU_PnSTR_STR2_Pos) /*!< 21mA */
#define SCU_PnSTR_STR2_23mA  (0x3U << SCU_PnSTR_STR2_Pos) /*!< 23mA */
#define SCU_PnSTR_STR3_Pos   (6U)
#define SCU_PnSTR_STR3_Msk   (0x3U << SCU_PnSTR_STR3_Pos) /*!< 0x000000C0 */
#define SCU_PnSTR_STR3       SCU_PnSTR_STR3_Msk           /*!< Pn.3 driving/sinking strength */
#define SCU_PnSTR_STR3_17mA  (0x0U << SCU_PnSTR_STR3_Pos) /*!< 17mA */
#define SCU_PnSTR_STR3_19mA  (0x1U << SCU_PnSTR_STR3_Pos) /*!< 19mA */
#define SCU_PnSTR_STR3_21mA  (0x2U << SCU_PnSTR_STR3_Pos) /*!< 21mA */
#define SCU_PnSTR_STR3_23mA  (0x3U << SCU_PnSTR_STR3_Pos) /*!< 23mA */
#define SCU_PnSTR_STR4_Pos   (8U)
#define SCU_PnSTR_STR4_Msk   (0x3U << SCU_PnSTR_STR4_Pos) /*!< 0x00000300 */
#define SCU_PnSTR_STR4       SCU_PnSTR_STR4_Msk           /*!< Pn.4 driving/sinking strength */
#define SCU_PnSTR_STR4_17mA  (0x0U << SCU_PnSTR_STR4_Pos) /*!< 17mA */
#define SCU_PnSTR_STR4_19mA  (0x1U << SCU_PnSTR_STR4_Pos) /*!< 19mA */
#define SCU_PnSTR_STR4_21mA  (0x2U << SCU_PnSTR_STR4_Pos) /*!< 21mA */
#define SCU_PnSTR_STR4_23mA  (0x3U << SCU_PnSTR_STR4_Pos) /*!< 23mA */
#define SCU_PnSTR_STR5_Pos   (10U)
#define SCU_PnSTR_STR5_Msk   (0x3U << SCU_PnSTR_STR5_Pos) /*!< 0x00000C00 */
#define SCU_PnSTR_STR5       SCU_PnSTR_STR5_Msk           /*!< Pn.5 driving/sinking strength */
#define SCU_PnSTR_STR5_17mA  (0x0U << SCU_PnSTR_STR5_Pos) /*!< 17mA */
#define SCU_PnSTR_STR5_19mA  (0x1U << SCU_PnSTR_STR5_Pos) /*!< 19mA */
#define SCU_PnSTR_STR5_21mA  (0x2U << SCU_PnSTR_STR5_Pos) /*!< 21mA */
#define SCU_PnSTR_STR5_23mA  (0x3U << SCU_PnSTR_STR5_Pos) /*!< 23mA */
#define SCU_PnSTR_STR6_Pos   (12U)
#define SCU_PnSTR_STR6_Msk   (0x3U << SCU_PnSTR_STR6_Pos) /*!< 0x00003000 */
#define SCU_PnSTR_STR6       SCU_PnSTR_STR6_Msk           /*!< Pn.6 driving/sinking strength */
#define SCU_PnSTR_STR6_17mA  (0x0U << SCU_PnSTR_STR6_Pos) /*!< 17mA */
#define SCU_PnSTR_STR6_19mA  (0x1U << SCU_PnSTR_STR6_Pos) /*!< 19mA */
#define SCU_PnSTR_STR6_21mA  (0x2U << SCU_PnSTR_STR6_Pos) /*!< 21mA */
#define SCU_PnSTR_STR6_23mA  (0x3U << SCU_PnSTR_STR6_Pos) /*!< 23mA */
#define SCU_PnSTR_STR7_Pos   (14U)
#define SCU_PnSTR_STR7_Msk   (0x3U << SCU_PnSTR_STR7_Pos) /*!< 0x0000C000 */
#define SCU_PnSTR_STR7       SCU_PnSTR_STR7_Msk           /*!< Pn.7 driving/sinking strength */
#define SCU_PnSTR_STR7_17mA  (0x0U << SCU_PnSTR_STR7_Pos) /*!< 17mA */
#define SCU_PnSTR_STR7_19mA  (0x1U << SCU_PnSTR_STR7_Pos) /*!< 19mA */
#define SCU_PnSTR_STR7_21mA  (0x2U << SCU_PnSTR_STR7_Pos) /*!< 21mA */
#define SCU_PnSTR_STR7_23mA  (0x3U << SCU_PnSTR_STR7_Pos) /*!< 23mA */
#define SCU_PnSTR_STR8_Pos   (16U)
#define SCU_PnSTR_STR8_Msk   (0x3U << SCU_PnSTR_STR8_Pos) /*!< 0x00030000 */
#define SCU_PnSTR_STR8       SCU_PnSTR_STR8_Msk           /*!< Pn.8 driving/sinking strength */
#define SCU_PnSTR_STR8_17mA  (0x0U << SCU_PnSTR_STR8_Pos) /*!< 17mA */
#define SCU_PnSTR_STR8_19mA  (0x1U << SCU_PnSTR_STR8_Pos) /*!< 19mA */
#define SCU_PnSTR_STR8_21mA  (0x2U << SCU_PnSTR_STR8_Pos) /*!< 21mA */
#define SCU_PnSTR_STR8_23mA  (0x3U << SCU_PnSTR_STR8_Pos) /*!< 23mA */
#define SCU_PnSTR_STR9_Pos   (18U)
#define SCU_PnSTR_STR9_Msk   (0x3U << SCU_PnSTR_STR9_Pos) /*!< 0x000C0000 */
#define SCU_PnSTR_STR9       SCU_PnSTR_STR9_Msk           /*!< Pn.9 driving/sinking strength */
#define SCU_PnSTR_STR9_17mA  (0x0U << SCU_PnSTR_STR9_Pos) /*!< 17mA */
#define SCU_PnSTR_STR9_19mA  (0x1U << SCU_PnSTR_STR9_Pos) /*!< 19mA */
#define SCU_PnSTR_STR9_21mA  (0x2U << SCU_PnSTR_STR9_Pos) /*!< 21mA */
#define SCU_PnSTR_STR9_23mA  (0x3U << SCU_PnSTR_STR9_Pos) /*!< 23mA */
#define SCU_PnSTR_STR10_Pos  (20U)
#define SCU_PnSTR_STR10_Msk  (0x3U << SCU_PnSTR_STR10_Pos) /*!< 0x00300000 */
#define SCU_PnSTR_STR10      SCU_PnSTR_STR10_Msk           /*!< Pn.10 driving/sinking strength */
#define SCU_PnSTR_STR10_17mA (0x0U << SCU_PnSTR_STR10_Pos) /*!< 17mA */
#define SCU_PnSTR_STR10_19mA (0x1U << SCU_PnSTR_STR10_Pos) /*!< 19mA */
#define SCU_PnSTR_STR10_21mA (0x2U << SCU_PnSTR_STR10_Pos) /*!< 21mA */
#define SCU_PnSTR_STR10_23mA (0x3U << SCU_PnSTR_STR10_Pos) /*!< 23mA */
#define SCU_PnSTR_STR11_Pos  (22U)
#define SCU_PnSTR_STR11_Msk  (0x3U << SCU_PnSTR_STR11_Pos) /*!< 0x00C00000 */
#define SCU_PnSTR_STR11      SCU_PnSTR_STR11_Msk           /*!< Pn.11 driving/sinking strength */
#define SCU_PnSTR_STR11_17mA (0x0U << SCU_PnSTR_STR11_Pos) /*!< 17mA */
#define SCU_PnSTR_STR11_19mA (0x1U << SCU_PnSTR_STR11_Pos) /*!< 19mA */
#define SCU_PnSTR_STR11_21mA (0x2U << SCU_PnSTR_STR11_Pos) /*!< 21mA */
#define SCU_PnSTR_STR11_23mA (0x3U << SCU_PnSTR_STR11_Pos) /*!< 23mA */
#define SCU_PnSTR_STR12_Pos  (24U)
#define SCU_PnSTR_STR12_Msk  (0x3U << SCU_PnSTR_STR12_Pos) /*!< 0x03000000 */
#define SCU_PnSTR_STR12      SCU_PnSTR_STR12_Msk           /*!< Pn.12 driving/sinking strength */
#define SCU_PnSTR_STR12_17mA (0x0U << SCU_PnSTR_STR12_Pos) /*!< 17mA */
#define SCU_PnSTR_STR12_19mA (0x1U << SCU_PnSTR_STR12_Pos) /*!< 19mA */
#define SCU_PnSTR_STR12_21mA (0x2U << SCU_PnSTR_STR12_Pos) /*!< 21mA */
#define SCU_PnSTR_STR12_23mA (0x3U << SCU_PnSTR_STR12_Pos) /*!< 23mA */
#define SCU_PnSTR_STR13_Pos  (26U)
#define SCU_PnSTR_STR13_Msk  (0x3U << SCU_PnSTR_STR13_Pos) /*!< 0x03000000 */
#define SCU_PnSTR_STR13      SCU_PnSTR_STR13_Msk           /*!< Pn.13 driving/sinking strength */
#define SCU_PnSTR_STR13_17mA (0x0U << SCU_PnSTR_STR13_Pos) /*!< 17mA */
#define SCU_PnSTR_STR13_19mA (0x1U << SCU_PnSTR_STR13_Pos) /*!< 19mA */
#define SCU_PnSTR_STR13_21mA (0x2U << SCU_PnSTR_STR13_Pos) /*!< 21mA */
#define SCU_PnSTR_STR13_23mA (0x3U << SCU_PnSTR_STR13_Pos) /*!< 23mA */
#define SCU_PnSTR_STR14_Pos  (28U)
#define SCU_PnSTR_STR14_Msk  (0x3U << SCU_PnSTR_STR14_Pos) /*!< 0x0C000000 */
#define SCU_PnSTR_STR14      SCU_PnSTR_STR14_Msk           /*!< Pn.14 driving/sinking strength */
#define SCU_PnSTR_STR14_17mA (0x0U << SCU_PnSTR_STR14_Pos) /*!< 17mA */
#define SCU_PnSTR_STR14_19mA (0x1U << SCU_PnSTR_STR14_Pos) /*!< 19mA */
#define SCU_PnSTR_STR14_21mA (0x2U << SCU_PnSTR_STR14_Pos) /*!< 21mA */
#define SCU_PnSTR_STR14_23mA (0x3U << SCU_PnSTR_STR14_Pos) /*!< 23mA */
#define SCU_PnSTR_STR15_Pos  (30U)
#define SCU_PnSTR_STR15_Msk  (0x3U << SCU_PnSTR_STR15_Pos) /*!< 0x30000000 */
#define SCU_PnSTR_STR15      SCU_PnSTR_STR15_Msk           /*!< Pn.15 driving/sinking strength */
#define SCU_PnSTR_STR15_17mA (0x0U << SCU_PnSTR_STR15_Pos) /*!< 17mA */
#define SCU_PnSTR_STR15_19mA (0x1U << SCU_PnSTR_STR15_Pos) /*!< 19mA */
#define SCU_PnSTR_STR15_21mA (0x2U << SCU_PnSTR_STR15_Pos) /*!< 21mA */
#define SCU_PnSTR_STR15_23mA (0x3U << SCU_PnSTR_STR15_Pos) /*!< 23mA */

/* Bits definition for SCU P0STR1 register */
#define SCU_P0STR1_STR20_Pos  (8U)
#define SCU_P0STR1_STR20_Msk  (0x3U << SCU_P0STR1_STR20_Pos) /*!< 0x00000300 */
#define SCU_P0STR1_STR20      SCU_P0STR1_STR20_Msk           /*!< Pn.20 driving/sinking strength */
#define SCU_P0STR1_STR20_17mA (0x0U << SCU_P0STR1_STR20_Pos) /*!< 17mA */
#define SCU_P0STR1_STR20_19mA (0x1U << SCU_P0STR1_STR20_Pos) /*!< 19mA */
#define SCU_P0STR1_STR20_21mA (0x2U << SCU_P0STR1_STR20_Pos) /*!< 21mA */
#define SCU_P0STR1_STR20_23mA (0x3U << SCU_P0STR1_STR20_Pos) /*!< 23mA */

/* Bits definition for SCU AHBRSTMSK register */
#define SCU_AHBRSTMSK_DMA0RSTMSK_Pos  (0U)
#define SCU_AHBRSTMSK_DMA0RSTMSK_Msk  (0x1U << SCU_AHBRSTMSK_DMA0RSTMSK_Pos) /*!< 0x00000001 */
#define SCU_AHBRSTMSK_DMA0RSTMSK      SCU_AHBRSTMSK_DMA0RSTMSK_Msk           /*!< DMA0 reset mask */
#define SCU_AHBRSTMSK_DMA1RSTMSK_Pos  (1U)
#define SCU_AHBRSTMSK_DMA1RSTMSK_Msk  (0x1U << SCU_AHBRSTMSK_DMA1RSTMSK_Pos) /*!< 0x00000002 */
#define SCU_AHBRSTMSK_DMA1RSTMSK      SCU_AHBRSTMSK_DMA1RSTMSK_Msk           /*!< DMA1 reset mask */
#define SCU_AHBRSTMSK_SDIORSTMSK_Pos  (6U)
#define SCU_AHBRSTMSK_SDIORSTMSK_Msk  (0x1U << SCU_AHBRSTMSK_SDIORSTMSK_Pos) /*!< 0x00000040 */
#define SCU_AHBRSTMSK_SDIORSTMSK      SCU_AHBRSTMSK_SDIORSTMSK_Msk           /*!< SDIO reset mask */
#define SCU_AHBRSTMSK_ETHRSTMSK_Pos   (8U)
#define SCU_AHBRSTMSK_ETHRSTMSK_Msk   (0x1U << SCU_AHBRSTMSK_ETHRSTMSK_Pos) /*!< 0x00000100 */
#define SCU_AHBRSTMSK_ETHRSTMSK       SCU_AHBRSTMSK_ETHRSTMSK_Msk           /*!< Ethernet MAC reset mask */

/* Bits definition for SCU APB0RSTMSK register */
#define SCU_APB0RSTMSK_WDTRSTMSK_Pos    (0U)
#define SCU_APB0RSTMSK_WDTRSTMSK_Msk    (0x1U << SCU_APB0RSTMSK_WDTRSTMSK_Pos) /*!< 0x00000001 */
#define SCU_APB0RSTMSK_WDTRSTMSK        SCU_APB0RSTMSK_WDTRSTMSK_Msk           /*!< WDT reset mask */
#define SCU_APB0RSTMSK_WWDTRSTMSK_Pos   (1U)
#define SCU_APB0RSTMSK_WWDTRSTMSK_Msk   (0x1U << SCU_APB0RSTMSK_WWDTRSTMSK_Pos) /*!< 0x00000002 */
#define SCU_APB0RSTMSK_WWDTRSTMSK       SCU_APB0RSTMSK_WWDTRSTMSK_Msk           /*!< WWDT reset mask */
#define SCU_APB0RSTMSK_CAN0RSTMSK_Pos   (2U)
#define SCU_APB0RSTMSK_CAN0RSTMSK_Msk   (0x1U << SCU_APB0RSTMSK_CAN0RSTMSK_Pos) /*!< 0x00000004 */
#define SCU_APB0RSTMSK_CAN0RSTMSK       SCU_APB0RSTMSK_CAN0RSTMSK_Msk           /*!< CAN0 reset mask */
#define SCU_APB0RSTMSK_CAN1RSTMSK_Pos   (3U)
#define SCU_APB0RSTMSK_CAN1RSTMSK_Msk   (0x1U << SCU_APB0RSTMSK_CAN1RSTMSK_Pos) /*!< 0x00000008 */
#define SCU_APB0RSTMSK_CAN1RSTMSK       SCU_APB0RSTMSK_CAN1RSTMSK_Msk           /*!< CAN1 reset mask */
#define SCU_APB0RSTMSK_UART1RSTMSK_Pos  (4U)
#define SCU_APB0RSTMSK_UART1RSTMSK_Msk  (0x1U << SCU_APB0RSTMSK_UART1RSTMSK_Pos) /*!< 0x00000010 */
#define SCU_APB0RSTMSK_UART1RSTMSK      SCU_APB0RSTMSK_UART1RSTMSK_Msk           /*!< UART1 reset mask */
#define SCU_APB0RSTMSK_UART2RSTMSK_Pos  (5U)
#define SCU_APB0RSTMSK_UART2RSTMSK_Msk  (0x1U << SCU_APB0RSTMSK_UART2RSTMSK_Pos) /*!< 0x00000020 */
#define SCU_APB0RSTMSK_UART2RSTMSK      SCU_APB0RSTMSK_UART2RSTMSK_Msk           /*!< UART2 reset mask */
#define SCU_APB0RSTMSK_UART3RSTMSK_Pos  (6U)
#define SCU_APB0RSTMSK_UART3RSTMSK_Msk  (0x1U << SCU_APB0RSTMSK_UART3RSTMSK_Pos) /*!< 0x00000040 */
#define SCU_APB0RSTMSK_UART3RSTMSK      SCU_APB0RSTMSK_UART3RSTMSK_Msk           /*!< UART3 reset mask */
#define SCU_APB0RSTMSK_UART4RSTMSK_Pos  (7U)
#define SCU_APB0RSTMSK_UART4RSTMSK_Msk  (0x1U << SCU_APB0RSTMSK_UART4RSTMSK_Pos) /*!< 0x00000080 */
#define SCU_APB0RSTMSK_UART4RSTMSK      SCU_APB0RSTMSK_UART4RSTMSK_Msk           /*!< UART4 reset mask */
#define SCU_APB0RSTMSK_CT16B6RSTMSK_Pos (8U)
#define SCU_APB0RSTMSK_CT16B6RSTMSK_Msk (0x1U << SCU_APB0RSTMSK_CT16B6RSTMSK_Pos) /*!< 0x00000100 */
#define SCU_APB0RSTMSK_CT16B6RSTMSK     SCU_APB0RSTMSK_CT16B6RSTMSK_Msk           /*!< CT16B6 reset mask */
#define SCU_APB0RSTMSK_CT16B7RSTMSK_Pos (9U)
#define SCU_APB0RSTMSK_CT16B7RSTMSK_Msk (0x1U << SCU_APB0RSTMSK_CT16B7RSTMSK_Pos) /*!< 0x00000200 */
#define SCU_APB0RSTMSK_CT16B7RSTMSK     SCU_APB0RSTMSK_CT16B7RSTMSK_Msk           /*!< CT16B7 reset mask */

/* Bits definition for SCU APB1RSTMSK register */
#define SCU_APB1RSTMSK_ADC0RSTMSK_Pos   (0U)
#define SCU_APB1RSTMSK_ADC0RSTMSK_Msk   (0x1U << SCU_APB1RSTMSK_ADC0RSTMSK_Pos) /*!< 0x00000001 */
#define SCU_APB1RSTMSK_ADC0RSTMSK       SCU_APB1RSTMSK_ADC0RSTMSK_Msk           /*!< ADC0 reset mask */
#define SCU_APB1RSTMSK_SSP0RSTMSK_Pos   (1U)
#define SCU_APB1RSTMSK_SSP0RSTMSK_Msk   (0x1U << SCU_APB1RSTMSK_SSP0RSTMSK_Pos) /*!< 0x00000002 */
#define SCU_APB1RSTMSK_SSP0RSTMSK       SCU_APB1RSTMSK_SSP0RSTMSK_Msk           /*!< SSP0 reset mask */
#define SCU_APB1RSTMSK_SSP1RSTMSK_Pos   (2U)
#define SCU_APB1RSTMSK_SSP1RSTMSK_Msk   (0x1U << SCU_APB1RSTMSK_SSP1RSTMSK_Pos) /*!< 0x00000004 */
#define SCU_APB1RSTMSK_SSP1RSTMSK       SCU_APB1RSTMSK_SSP1RSTMSK_Msk           /*!< SSP1 reset mask */
#define SCU_APB1RSTMSK_SSP2RSTMSK_Pos   (3U)
#define SCU_APB1RSTMSK_SSP2RSTMSK_Msk   (0x1U << SCU_APB1RSTMSK_SSP2RSTMSK_Pos) /*!< 0x00000008 */
#define SCU_APB1RSTMSK_SSP2RSTMSK       SCU_APB1RSTMSK_SSP2RSTMSK_Msk           /*!< SSP2 reset mask */
#define SCU_APB1RSTMSK_UART0RSTMSK_Pos  (4U)
#define SCU_APB1RSTMSK_UART0RSTMSK_Msk  (0x1U << SCU_APB1RSTMSK_UART0RSTMSK_Pos) /*!< 0x00000010 */
#define SCU_APB1RSTMSK_UART0RSTMSK      SCU_APB1RSTMSK_UART0RSTMSK_Msk           /*!< UART0 reset mask */
#define SCU_APB1RSTMSK_UART5RSTMSK_Pos  (5U)
#define SCU_APB1RSTMSK_UART5RSTMSK_Msk  (0x1U << SCU_APB1RSTMSK_UART5RSTMSK_Pos) /*!< 0x00000020 */
#define SCU_APB1RSTMSK_UART5RSTMSK      SCU_APB1RSTMSK_UART5RSTMSK_Msk           /*!< UART5 reset mask */
#define SCU_APB1RSTMSK_I2C0RSTMSK_Pos   (6U)
#define SCU_APB1RSTMSK_I2C0RSTMSK_Msk   (0x1U << SCU_APB1RSTMSK_I2C0RSTMSK_Pos) /*!< 0x00000040 */
#define SCU_APB1RSTMSK_I2C0RSTMSK       SCU_APB1RSTMSK_I2C0RSTMSK_Msk           /*!< I2C0 reset mask */
#define SCU_APB1RSTMSK_I2C1RSTMSK_Pos   (7U)
#define SCU_APB1RSTMSK_I2C1RSTMSK_Msk   (0x1U << SCU_APB1RSTMSK_I2C1RSTMSK_Pos) /*!< 0x00000080 */
#define SCU_APB1RSTMSK_I2C1RSTMSK       SCU_APB1RSTMSK_I2C1RSTMSK_Msk           /*!< I2C1 reset mask */
#define SCU_APB1RSTMSK_I2C2RSTMSK_Pos   (8U)
#define SCU_APB1RSTMSK_I2C2RSTMSK_Msk   (0x1U << SCU_APB1RSTMSK_I2C2RSTMSK_Pos) /*!< 0x00000100 */
#define SCU_APB1RSTMSK_I2C2RSTMSK       SCU_APB1RSTMSK_I2C2RSTMSK_Msk           /*!< I2C2 reset mask */
#define SCU_APB1RSTMSK_CT16B0RSTMSK_Pos (9U)
#define SCU_APB1RSTMSK_CT16B0RSTMSK_Msk (0x1U << SCU_APB1RSTMSK_CT16B0RSTMSK_Pos) /*!< 0x00000200 */
#define SCU_APB1RSTMSK_CT16B0RSTMSK     SCU_APB1RSTMSK_CT16B0RSTMSK_Msk           /*!< CT16B0 reset mask */
#define SCU_APB1RSTMSK_CT16B1RSTMSK_Pos (10U)
#define SCU_APB1RSTMSK_CT16B1RSTMSK_Msk (0x1U << SCU_APB1RSTMSK_CT16B1RSTMSK_Pos) /*!< 0x00000400 */
#define SCU_APB1RSTMSK_CT16B1RSTMSK     SCU_APB1RSTMSK_CT16B1RSTMSK_Msk           /*!< CT16B1 reset mask */
#define SCU_APB1RSTMSK_CT16B2RSTMSK_Pos (11U)
#define SCU_APB1RSTMSK_CT16B2RSTMSK_Msk (0x1U << SCU_APB1RSTMSK_CT16B2RSTMSK_Pos) /*!< 0x00000800 */
#define SCU_APB1RSTMSK_CT16B2RSTMSK     SCU_APB1RSTMSK_CT16B2RSTMSK_Msk           /*!< CT16B2 reset mask */
#define SCU_APB1RSTMSK_CT16B3RSTMSK_Pos (12U)
#define SCU_APB1RSTMSK_CT16B3RSTMSK_Msk (0x1U << SCU_APB1RSTMSK_CT16B3RSTMSK_Pos) /*!< 0x00001000 */
#define SCU_APB1RSTMSK_CT16B3RSTMSK     SCU_APB1RSTMSK_CT16B3RSTMSK_Msk           /*!< CT16B3 reset mask */
#define SCU_APB1RSTMSK_CT16B4RSTMSK_Pos (13U)
#define SCU_APB1RSTMSK_CT16B4RSTMSK_Msk (0x1U << SCU_APB1RSTMSK_CT16B4RSTMSK_Pos) /*!< 0x00002000 */
#define SCU_APB1RSTMSK_CT16B4RSTMSK     SCU_APB1RSTMSK_CT16B4RSTMSK_Msk           /*!< CT16B4 reset mask */
#define SCU_APB1RSTMSK_CT16B5RSTMSK_Pos (14U)
#define SCU_APB1RSTMSK_CT16B5RSTMSK_Msk (0x1U << SCU_APB1RSTMSK_CT16B5RSTMSK_Pos) /*!< 0x00004000 */
#define SCU_APB1RSTMSK_CT16B5RSTMSK     SCU_APB1RSTMSK_CT16B5RSTMSK_Msk           /*!< CT16B5 reset mask */
#define SCU_APB1RSTMSK_CT16B8RSTMSK_Pos (15U)
#define SCU_APB1RSTMSK_CT16B8RSTMSK_Msk (0x1U << SCU_APB1RSTMSK_CT16B8RSTMSK_Pos) /*!< 0x00008000 */
#define SCU_APB1RSTMSK_CT16B8RSTMSK     SCU_APB1RSTMSK_CT16B8RSTMSK_Msk           /*!< CT16B8 reset mask */
#define SCU_APB1RSTMSK_CRCRSTMSK_Pos    (16U)
#define SCU_APB1RSTMSK_CRCRSTMSK_Msk    (0x1U << SCU_APB1RSTMSK_CRCRSTMSK_Pos) /*!< 0x00010000 */
#define SCU_APB1RSTMSK_CRCRSTMSK        SCU_APB1RSTMSK_CRCRSTMSK_Msk           /*!< CRC reset mask */
#define SCU_APB1RSTMSK_GPIO0RSTMSK_Pos  (17U)
#define SCU_APB1RSTMSK_GPIO0RSTMSK_Msk  (0x1U << SCU_APB1RSTMSK_GPIO0RSTMSK_Pos) /*!< 0x00020000 */
#define SCU_APB1RSTMSK_GPIO0RSTMSK      SCU_APB1RSTMSK_GPIO0RSTMSK_Msk           /*!< GPIO0 reset mask */
#define SCU_APB1RSTMSK_GPIO1RSTMSK_Pos  (18U)
#define SCU_APB1RSTMSK_GPIO1RSTMSK_Msk  (0x1U << SCU_APB1RSTMSK_GPIO1RSTMSK_Pos) /*!< 0x00040000 */
#define SCU_APB1RSTMSK_GPIO1RSTMSK      SCU_APB1RSTMSK_GPIO1RSTMSK_Msk           /*!< GPIO1 reset mask */
#define SCU_APB1RSTMSK_GPIO2RSTMSK_Pos  (19U)
#define SCU_APB1RSTMSK_GPIO2RSTMSK_Msk  (0x1U << SCU_APB1RSTMSK_GPIO2RSTMSK_Pos) /*!< 0x00080000 */
#define SCU_APB1RSTMSK_GPIO2RSTMSK      SCU_APB1RSTMSK_GPIO2RSTMSK_Msk           /*!< GPIO2 reset mask */
#define SCU_APB1RSTMSK_GPIO3RSTMSK_Pos  (20U)
#define SCU_APB1RSTMSK_GPIO3RSTMSK_Msk  (0x1U << SCU_APB1RSTMSK_GPIO3RSTMSK_Pos) /*!< 0x00100000 */
#define SCU_APB1RSTMSK_GPIO3RSTMSK      SCU_APB1RSTMSK_GPIO3RSTMSK_Msk           /*!< GPIO3 reset mask */
#define SCU_APB1RSTMSK_LCMRSTMSK_Pos    (21U)
#define SCU_APB1RSTMSK_LCMRSTMSK_Msk    (0x1U << SCU_APB1RSTMSK_LCMRSTMSK_Pos) /*!< 0x00200000 */
#define SCU_APB1RSTMSK_LCMRSTMSK        SCU_APB1RSTMSK_LCMRSTMSK_Msk           /*!< LCM reset mask */

/* Bits definition for SCU GMAC Special Controll register */
#define SCU_GMAC_SC_ETHMAC_NP_TX_POLL_Pos (16U)
#define SCU_GMAC_SC_ETHMAC_NP_TX_POLL_Msk (0x1U << SCU_GMAC_SC_ETHMAC_NP_TX_POLL_Pos) /*!< 0x00010000 */
#define SCU_GMAC_SC_ETHMAC_NP_TX_POLL     SCU_GMAC_SC_ETHMAC_NP_TX_POLL_Msk           /*!< Normal priority TX poll demand, on cycle pulse activation with the system clock domain */
#define SCU_GMAC_SC_ETHMAC_HP_TX_POLL_Pos (17U)
#define SCU_GMAC_SC_ETHMAC_HP_TX_POLL_Msk (0x1U << SCU_GMAC_SC_ETHMAC_HP_TX_POLL_Pos) /*!< 0x00020000 */
#define SCU_GMAC_SC_ETHMAC_HP_TX_POLL     SCU_GMAC_SC_ETHMAC_HP_TX_POLL_Msk           /*!< High priority TX poll demand, on cycle pulse activation with the system clock domain */
#define SCU_GMAC_SC_ETHSRAMRET_Pos        (28U)
#define SCU_GMAC_SC_ETHSRAMRET_Msk        (0x1U << SCU_GMAC_SC_ETHSRAMRET_Pos) /*!< 0x10000000 */
#define SCU_GMAC_SC_ETHSRAMRET            SCU_GMAC_SC_ETHSRAMRET_Msk           /*!< Ethernet MAC SRAM retention mode control */
#define SCU_GMAC_SC_CAN1SRAMRET_Pos       (29U)
#define SCU_GMAC_SC_CAN1SRAMRET_Msk       (0x1U << SCU_GMAC_SC_CAN1SRAMRET_Pos) /*!< 0x20000000 */
#define SCU_GMAC_SC_CAN1SRAMRET           SCU_GMAC_SC_CAN1SRAMRET_Msk           /*!< CAN1 SRAM retention mode control  */
#define SCU_GMAC_SC_CAN0SRAMRET_Pos       (30U)
#define SCU_GMAC_SC_CAN0SRAMRET_Msk       (0x1U << SCU_GMAC_SC_CAN0SRAMRET_Pos) /*!< 0x20000000 */
#define SCU_GMAC_SC_CAN0SRAMRET           SCU_GMAC_SC_CAN0SRAMRET_Msk           /*!< CAN0 SRAM retention mode control  */

/* Bits definition for SCU SRAM1 controller sideband signal register */
#define SCU_SRAM1CTRL_AUTONAPEN_Pos (0U)
#define SCU_SRAM1CTRL_AUTONAPEN_Msk (0x1U << SCU_SRAM1CTRL_AUTONAPEN_Pos) /*!< 0x00000001 */
#define SCU_SRAM1CTRL_AUTONAPEN     SCU_SRAM1CTRL_AUTONAPEN_Msk           /*!< SRAM1 automatic nap function. When the SRAM1 is idle, it automatically enters nap mode */
#define SCU_SRAM1CTRL_AUTORETEN_Pos (1U)
#define SCU_SRAM1CTRL_AUTORETEN_Msk (0x1U << SCU_SRAM1CTRL_AUTORETEN_Pos) /*!< 0x00000002 */
#define SCU_SRAM1CTRL_AUTORETEN     SCU_SRAM1CTRL_AUTORETEN_Msk           /*!< SRAM1 automatic retention function. When the SRAM1 is idle, it automatically enters retention mode */
#define SCU_SRAM1CTRL_AUTOSLPEN_Pos (2U)
#define SCU_SRAM1CTRL_AUTOSLPEN_Msk (0x1U << SCU_SRAM1CTRL_AUTOSLPEN_Pos) /*!< 0x00000004 */
#define SCU_SRAM1CTRL_AUTOSLPEN     SCU_SRAM1CTRL_AUTOSLPEN_Msk           /*!< SRAM1 automatic sleep function. When the SRAM1 is idle, it automatically enters sleep mode */
#define SCU_SRAM1CTRL_NAPRESDLY_Pos (4U)
#define SCU_SRAM1CTRL_NAPRESDLY_Msk (0xFU << SCU_SRAM1CTRL_NAPRESDLY_Pos) /*!< 0x000000F0 */
#define SCU_SRAM1CTRL_NAPRESDLY     SCU_SRAM1CTRL_NAPRESDLY_Msk           /*!< Resuming delay time for SRAM1 nap mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM1CTRL_RETRESDLY_Pos (8U)
#define SCU_SRAM1CTRL_RETRESDLY_Msk (0xFU << SCU_SRAM1CTRL_RETRESDLY_Pos) /*!< 0x00000F00 */
#define SCU_SRAM1CTRL_RETRESDLY     SCU_SRAM1CTRL_RETRESDLY_Msk           /*!< Resuming delay time for SRAM1 retention mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM1CTRL_SLPRESDLY_Pos (12U)
#define SCU_SRAM1CTRL_SLPRESDLY_Msk (0xFU << SCU_SRAM1CTRL_SLPRESDLY_Pos) /*!< 0x0000F000 */
#define SCU_SRAM1CTRL_SLPRESDLY     SCU_SRAM1CTRL_SLPRESDLY_Msk           /*!< Resuming delay time for SRAM1 sleep mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM1CTRL_TIMEOUT_Pos   (16U)
#define SCU_SRAM1CTRL_TIMEOUT_Msk   (0xFFFU << SCU_SRAM1CTRL_TIMEOUT_Pos) /*!< 0x0FFF0000 */
#define SCU_SRAM1CTRL_TIMEOUT       SCU_SRAM1CTRL_TIMEOUT_Msk             /*!< Time-out counter to trigger SRAM1 enter the power-saving mode */

/* Bits definition for SCU SRAM2 controller sideband signal register */
#define SCU_SRAM2CTRL_AUTONAPEN_Pos (0U)
#define SCU_SRAM2CTRL_AUTONAPEN_Msk (0x1U << SCU_SRAM2CTRL_AUTONAPEN_Pos) /*!< 0x00000001 */
#define SCU_SRAM2CTRL_AUTONAPEN     SCU_SRAM2CTRL_AUTONAPEN_Msk           /*!< SRAM2 automatic nap function. When the SRAM2 is idle, it automatically enters nap mode */
#define SCU_SRAM2CTRL_AUTORETEN_Pos (1U)
#define SCU_SRAM2CTRL_AUTORETEN_Msk (0x1U << SCU_SRAM2CTRL_AUTORETEN_Pos) /*!< 0x00000002 */
#define SCU_SRAM2CTRL_AUTORETEN     SCU_SRAM2CTRL_AUTORETEN_Msk           /*!< SRAM2 automatic retention function. When the SRAM2 is idle, it automatically enters retention mode */
#define SCU_SRAM2CTRL_AUTOSLPEN_Pos (2U)
#define SCU_SRAM2CTRL_AUTOSLPEN_Msk (0x1U << SCU_SRAM2CTRL_AUTOSLPEN_Pos) /*!< 0x00000004 */
#define SCU_SRAM2CTRL_AUTOSLPEN     SCU_SRAM2CTRL_AUTOSLPEN_Msk           /*!< SRAM2 automatic sleep function. When the SRAM2 is idle, it automatically enters sleep mode */
#define SCU_SRAM2CTRL_NAPRESDLY_Pos (4U)
#define SCU_SRAM2CTRL_NAPRESDLY_Msk (0xFU << SCU_SRAM2CTRL_NAPRESDLY_Pos) /*!< 0x000000F0 */
#define SCU_SRAM2CTRL_NAPRESDLY     SCU_SRAM2CTRL_NAPRESDLY_Msk           /*!< Resuming delay time for SRAM2 nap mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM2CTRL_RETRESDLY_Pos (8U)
#define SCU_SRAM2CTRL_RETRESDLY_Msk (0xFU << SCU_SRAM2CTRL_RETRESDLY_Pos) /*!< 0x00000F00 */
#define SCU_SRAM2CTRL_RETRESDLY     SCU_SRAM2CTRL_RETRESDLY_Msk           /*!< Resuming delay time for SRAM2 retention mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM2CTRL_SLPRESDLY_Pos (12U)
#define SCU_SRAM2CTRL_SLPRESDLY_Msk (0xFU << SCU_SRAM2CTRL_SLPRESDLY_Pos) /*!< 0x0000F000 */
#define SCU_SRAM2CTRL_SLPRESDLY     SCU_SRAM2CTRL_SLPRESDLY_Msk           /*!< Resuming delay time for SRAM2 sleep mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM2CTRL_TIMEOUT_Pos   (16U)
#define SCU_SRAM2CTRL_TIMEOUT_Msk   (0xFFFU << SCU_SRAM2CTRL_TIMEOUT_Pos) /*!< 0x0FFF0000 */
#define SCU_SRAM2CTRL_TIMEOUT       SCU_SRAM2CTRL_TIMEOUT_Msk             /*!< Time-out counter to trigger SRAM2 enter the power-saving mode */

/* Bits definition for SCU SRAM3 controller sideband signal register */
#define SCU_SRAM3CTRL_AUTONAPEN_Pos (0U)
#define SCU_SRAM3CTRL_AUTONAPEN_Msk (0x1U << SCU_SRAM3CTRL_AUTONAPEN_Pos) /*!< 0x00000001 */
#define SCU_SRAM3CTRL_AUTONAPEN     SCU_SRAM3CTRL_AUTONAPEN_Msk           /*!< SRAM3 automatic nap function. When the SRAM3 is idle, it automatically enters nap mode */
#define SCU_SRAM3CTRL_AUTORETEN_Pos (1U)
#define SCU_SRAM3CTRL_AUTORETEN_Msk (0x1U << SCU_SRAM3CTRL_AUTORETEN_Pos) /*!< 0x00000002 */
#define SCU_SRAM3CTRL_AUTORETEN     SCU_SRAM3CTRL_AUTORETEN_Msk           /*!< SRAM3 automatic retention function. When the SRAM3 is idle, it automatically enters retention mode */
#define SCU_SRAM3CTRL_AUTOSLPEN_Pos (2U)
#define SCU_SRAM3CTRL_AUTOSLPEN_Msk (0x1U << SCU_SRAM3CTRL_AUTOSLPEN_Pos) /*!< 0x00000004 */
#define SCU_SRAM3CTRL_AUTOSLPEN     SCU_SRAM3CTRL_AUTOSLPEN_Msk           /*!< SRAM3 automatic sleep function. When the SRAM3 is idle, it automatically enters sleep mode */
#define SCU_SRAM3CTRL_NAPRESDLY_Pos (4U)
#define SCU_SRAM3CTRL_NAPRESDLY_Msk (0xFU << SCU_SRAM3CTRL_NAPRESDLY_Pos) /*!< 0x000000F0 */
#define SCU_SRAM3CTRL_NAPRESDLY     SCU_SRAM3CTRL_NAPRESDLY_Msk           /*!< Resuming delay time for SRAM3 nap mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM3CTRL_RETRESDLY_Pos (8U)
#define SCU_SRAM3CTRL_RETRESDLY_Msk (0xFU << SCU_SRAM3CTRL_RETRESDLY_Pos) /*!< 0x00000F00 */
#define SCU_SRAM3CTRL_RETRESDLY     SCU_SRAM3CTRL_RETRESDLY_Msk           /*!< Resuming delay time for SRAM3 retention mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM3CTRL_SLPRESDLY_Pos (12U)
#define SCU_SRAM3CTRL_SLPRESDLY_Msk (0xFU << SCU_SRAM3CTRL_SLPRESDLY_Pos) /*!< 0x0000F000 */
#define SCU_SRAM3CTRL_SLPRESDLY     SCU_SRAM3CTRL_SLPRESDLY_Msk           /*!< Resuming delay time for SRAM3 sleep mode returns to normal mode (Time for the power recovery) */
#define SCU_SRAM3CTRL_TIMEOUT_Pos   (16U)
#define SCU_SRAM3CTRL_TIMEOUT_Msk   (0xFFFU << SCU_SRAM3CTRL_TIMEOUT_Pos) /*!< 0x0FFF0000 */
#define SCU_SRAM3CTRL_TIMEOUT       SCU_SRAM3CTRL_TIMEOUT_Msk             /*!< Time-out counter to trigger SRAM3 enter the power-saving mode */

/* Bits definition for SCU BKPSRAM controller sideband signal register */
#define SCU_BKPSRAMCTRL_AUTONAPEN_Pos (0U)
#define SCU_BKPSRAMCTRL_AUTONAPEN_Msk (0x1U << SCU_BKPSRAMCTRL_AUTONAPEN_Pos) /*!< 0x00000001 */
#define SCU_BKPSRAMCTRL_AUTONAPEN     SCU_BKPSRAMCTRL_AUTONAPEN_Msk           /*!< BKPSRAM automatic nap function. When the BKPSRAM is idle, it automatically enters nap mode */
#define SCU_BKPSRAMCTRL_AUTORETEN_Pos (1U)
#define SCU_BKPSRAMCTRL_AUTORETEN_Msk (0x1U << SCU_BKPSRAMCTRL_AUTORETEN_Pos) /*!< 0x00000002 */
#define SCU_BKPSRAMCTRL_AUTORETEN     SCU_BKPSRAMCTRL_AUTORETEN_Msk           /*!< BKPSRAM automatic retention function. When the BKPSRAM is idle, it automatically enters retention mode */
#define SCU_BKPSRAMCTRL_AUTOSLPEN_Pos (2U)
#define SCU_BKPSRAMCTRL_AUTOSLPEN_Msk (0x1U << SCU_BKPSRAMCTRL_AUTOSLPEN_Pos) /*!< 0x00000004 */
#define SCU_BKPSRAMCTRL_AUTOSLPEN     SCU_BKPSRAMCTRL_AUTOSLPEN_Msk           /*!< BKPSRAM automatic sleep function. When the BKPSRAM is idle, it automatically enters sleep mode */
#define SCU_BKPSRAMCTRL_NAPRESDLY_Pos (4U)
#define SCU_BKPSRAMCTRL_NAPRESDLY_Msk (0xFU << SCU_BKPSRAMCTRL_NAPRESDLY_Pos) /*!< 0x000000F0 */
#define SCU_BKPSRAMCTRL_NAPRESDLY     SCU_BKPSRAMCTRL_NAPRESDLY_Msk           /*!< Resuming delay time for BKPSRAM nap mode returns to normal mode (Time for the power recovery) */
#define SCU_BKPSRAMCTRL_RETRESDLY_Pos (8U)
#define SCU_BKPSRAMCTRL_RETRESDLY_Msk (0xFU << SCU_BKPSRAMCTRL_RETRESDLY_Pos) /*!< 0x00000F00 */
#define SCU_BKPSRAMCTRL_RETRESDLY     SCU_BKPSRAMCTRL_RETRESDLY_Msk           /*!< Resuming delay time for BKPSRAM retention mode returns to normal mode (Time for the power recovery) */
#define SCU_BKPSRAMCTRL_SLPRESDLY_Pos (12U)
#define SCU_BKPSRAMCTRL_SLPRESDLY_Msk (0xFU << SCU_BKPSRAMCTRL_SLPRESDLY_Pos) /*!< 0x0000F000 */
#define SCU_BKPSRAMCTRL_SLPRESDLY     SCU_BKPSRAMCTRL_SLPRESDLY_Msk           /*!< Resuming delay time for BKPSRAM sleep mode returns to normal mode (Time for the power recovery) */
#define SCU_BKPSRAMCTRL_TIMEOUT_Pos   (16U)
#define SCU_BKPSRAMCTRL_TIMEOUT_Msk   (0xFFFU << SCU_BKPSRAMCTRL_TIMEOUT_Pos) /*!< 0x0FFF0000 */
#define SCU_BKPSRAMCTRL_TIMEOUT       SCU_BKPSRAMCTRL_TIMEOUT_Msk             /*!< Time-out counter to trigger BKPSRAM enter the power-saving mode */

/* Bits definition for SCU RTC Timer register 1 */
#define SCU_RTC_TIME1_SEC_Pos     (0U)
#define SCU_RTC_TIME1_SEC_Msk     (0x3FU << SCU_RTC_TIME1_SEC_Pos) /*!< 0x0000003F */
#define SCU_RTC_TIME1_SEC         SCU_RTC_TIME1_SEC_Msk            /*!< Current second */
#define SCU_RTC_TIME1_MIN_Pos     (8U)
#define SCU_RTC_TIME1_MIN_Msk     (0x3FU << SCU_RTC_TIME1_MIN_Pos) /*!< 0x00003F00 */
#define SCU_RTC_TIME1_MIN         SCU_RTC_TIME1_MIN_Msk            /*!< Current minute */
#define SCU_RTC_TIME1_HOUR_Pos    (16U)
#define SCU_RTC_TIME1_HOUR_Msk    (0x1FU << SCU_RTC_TIME1_HOUR_Pos) /*!< 0x001F0000 */
#define SCU_RTC_TIME1_HOUR        SCU_RTC_TIME1_HOUR_Msk            /*!< Current hour */
#define SCU_RTC_TIME1_WEEKDAY_Pos (24U)
#define SCU_RTC_TIME1_WEEKDAY_Msk (0x7U << SCU_RTC_TIME1_WEEKDAY_Pos) /*!< 0x07000000 */
#define SCU_RTC_TIME1_WEEKDAY     SCU_RTC_TIME1_WEEKDAY_Msk           /*!< Current weekday */

/* Bits definition for SCU RTC Timer register 2 */
#define SCU_RTC_TIME2_DATE_Pos    (0U)
#define SCU_RTC_TIME2_DATE_Msk    (0x1FU << SCU_RTC_TIME2_DATE_Pos) /*!< 0x0000001F */
#define SCU_RTC_TIME2_DATE        SCU_RTC_TIME2_DATE_Msk            /*!< Current date */
#define SCU_RTC_TIME2_MONTH_Pos   (8U)
#define SCU_RTC_TIME2_MONTH_Msk   (0xFU << SCU_RTC_TIME2_MONTH_Pos) /*!< 0x00000F00 */
#define SCU_RTC_TIME2_MONTH       SCU_RTC_TIME2_MONTH_Msk           /*!< Current month */
#define SCU_RTC_TIME2_YEAR_Pos    (16U)
#define SCU_RTC_TIME2_YEAR_Msk    (0x7FU << SCU_RTC_TIME2_YEAR_Pos) /*!< 0x007F0000 */
#define SCU_RTC_TIME2_YEAR        SCU_RTC_TIME2_YEAR_Msk            /*!< Current year */
#define SCU_RTC_TIME2_CENTURY_Pos (24U)
#define SCU_RTC_TIME2_CENTURY_Msk (0x7FU << SCU_RTC_TIME2_CENTURY_Pos) /*!< 0x7F000000 */
#define SCU_RTC_TIME2_CENTURY     SCU_RTC_TIME2_CENTURY_Msk            /*!< Current century */

/* Bits definition for SCU RTC Alarm Timer register 1 */
#define SCU_RTC_ALM1_SEC_Pos     (0U)
#define SCU_RTC_ALM1_SEC_Msk     (0x3FU << SCU_RTC_ALM1_SEC_Pos) /*!< 0x0000003F */
#define SCU_RTC_ALM1_SEC         SCU_RTC_ALM1_SEC_Msk            /*!< Alarm second */
#define SCU_RTC_ALM1_MIN_Pos     (8U)
#define SCU_RTC_ALM1_MIN_Msk     (0x3FU << SCU_RTC_ALM1_MIN_Pos) /*!< 0x00003F00 */
#define SCU_RTC_ALM1_MIN         SCU_RTC_ALM1_MIN_Msk            /*!< Alarm minute */
#define SCU_RTC_ALM1_HOUR_Pos    (16U)
#define SCU_RTC_ALM1_HOUR_Msk    (0x1FU << SCU_RTC_ALM1_HOUR_Pos) /*!< 0x001F0000 */
#define SCU_RTC_ALM1_HOUR        SCU_RTC_ALM1_HOUR_Msk            /*!< Alarm hour */
#define SCU_RTC_ALM1_WEEKDAY_Pos (24U)
#define SCU_RTC_ALM1_WEEKDAY_Msk (0x7U << SCU_RTC_ALM1_WEEKDAY_Pos) /*!< 0x07000000 */
#define SCU_RTC_ALM1_WEEKDAY     SCU_RTC_ALM1_WEEKDAY_Msk           /*!< Alarm weekday */

/* Bits definition for SCU RTC Alarm Timer register 2 */
#define SCU_RTC_ALM2_DATE_Pos  (0U)
#define SCU_RTC_ALM2_DATE_Msk  (0x1FU << SCU_RTC_ALM2_DATE_Pos) /*!< 0x0000001F */
#define SCU_RTC_ALM2_DATE      SCU_RTC_ALM2_DATE_Msk            /*!< Alarm date */
#define SCU_RTC_ALM2_MONTH_Pos (8U)
#define SCU_RTC_ALM2_MONTH_Msk (0xFU << SCU_RTC_ALM2_MONTH_Pos) /*!< 0x00000F00 */
#define SCU_RTC_ALM2_MONTH     SCU_RTC_ALM2_MONTH_Msk           /*!< Alarm month */

/* Bits definition for SCU RTC Control register */
#define SCU_RTC_CTRL_RTC_EN_Pos        (0U)
#define SCU_RTC_CTRL_RTC_EN_Msk        (0x1U << SCU_RTC_CTRL_RTC_EN_Pos) /*!< 0x00000001 */
#define SCU_RTC_CTRL_RTC_EN            SCU_RTC_CTRL_RTC_EN_Msk           /*!< RTC enable bit */
#define SCU_RTC_CTRL_RTC_ALARM_EN_Pos  (1U)
#define SCU_RTC_CTRL_RTC_ALARM_EN_Msk  (0x1U << SCU_RTC_CTRL_RTC_ALARM_EN_Pos) /*!< 0x00000002 */
#define SCU_RTC_CTRL_RTC_ALARM_EN      SCU_RTC_CTRL_RTC_ALARM_EN_Msk           /*!< RTC alarm enable bit */
#define SCU_RTC_CTRL_LOCK_EN_Pos       (2U)
#define SCU_RTC_CTRL_LOCK_EN_Msk       (0x1U << SCU_RTC_CTRL_LOCK_EN_Pos) /*!< 0x00000004 */
#define SCU_RTC_CTRL_LOCK_EN           SCU_RTC_CTRL_LOCK_EN_Msk           /*!< Lock the Time2 register after reading the Time1 register */
#define SCU_RTC_CTRL_PERINT_SEL_Pos    (4U)
#define SCU_RTC_CTRL_PERINT_SEL_Msk    (0x7U << SCU_RTC_CTRL_PERINT_SEL_Pos) /*!< 0x00000070 */
#define SCU_RTC_CTRL_PERINT_SEL        SCU_RTC_CTRL_PERINT_SEL_Msk           /*!< Periodic interrupt output signal select */
#define SCU_RTC_CTRL_SECOUT_EN_Pos     (7U)
#define SCU_RTC_CTRL_SECOUT_EN_Msk     (0x1U << SCU_RTC_CTRL_SECOUT_EN_Pos) /*!< 0x00000080 */
#define SCU_RTC_CTRL_SECOUT_EN         SCU_RTC_CTRL_SECOUT_EN_Msk           /*!< Enable an event in each second */
#define SCU_RTC_CTRL_RTCEN_STS_Pos     (8U)
#define SCU_RTC_CTRL_RTCEN_STS_Msk     (0x1U << SCU_RTC_CTRL_RTCEN_STS_Pos) /*!< 0x00000100 */
#define SCU_RTC_CTRL_RTCEN_STS         SCU_RTC_CTRL_RTCEN_STS_Msk           /*!< RTC enable status */
#define SCU_RTC_CTRL_RTC_ALMEN_STS_Pos (9U)
#define SCU_RTC_CTRL_RTC_ALMEN_STS_Msk (0x1U << SCU_RTC_CTRL_RTC_ALMEN_STS_Pos) /*!< 0x00000200 */
#define SCU_RTC_CTRL_RTC_ALMEN_STS     SCU_RTC_CTRL_RTC_ALMEN_STS_Msk           /*!< RTC alarm enable status */
#define SCU_RTC_CTRL_PWUTEN_STS_Pos    (11U)
#define SCU_RTC_CTRL_PWUTEN_STS_Msk    (0x1U << SCU_RTC_CTRL_PWUTEN_STS_Pos) /*!< 0x00000800 */
#define SCU_RTC_CTRL_PWUTEN_STS        SCU_RTC_CTRL_PWUTEN_STS_Msk           /*!< Periodic wakeup timer enable status */
#define SCU_RTC_CTRL_RTC_REGRDY_Pos    (15U)
#define SCU_RTC_CTRL_RTC_REGRDY_Msk    (0x1U << SCU_RTC_CTRL_RTC_REGRDY_Pos) /*!< 0x00008000 */
#define SCU_RTC_CTRL_RTC_REGRDY        SCU_RTC_CTRL_RTC_REGRDY_Msk           /*!< RTC registers ready flag. After RTC clock is enabled, user has to wait this flag */

/* Bits definition for SCU RTC Tick Trim Control register */
#define SCU_RTC_TRIM_TRIM_F_Pos (0U)
#define SCU_RTC_TRIM_TRIM_F_Msk (0xFFFFU << SCU_RTC_TRIM_TRIM_F_Pos) /*!< 0x0000FFFF */
#define SCU_RTC_TRIM_TRIM_F     SCU_RTC_TRIM_TRIM_F_Msk              /*!< RTC tick fractional factor */
#define SCU_RTC_TRIM_TRIM_P_Pos (16U)
#define SCU_RTC_TRIM_TRIM_P_Msk (0xFFFFU << SCU_RTC_TRIM_TRIM_P_Pos) /*!< 0xFFFF0000 */
#define SCU_RTC_TRIM_TRIM_P     SCU_RTC_TRIM_TRIM_P_Msk              /*!< RTC tick dividing factor */

/* Bits definition for SCU RTC Timer for BCD register 1  */
#define SCU_BCD_RTC_TIME1_SEC_Pos      (0U)
#define SCU_BCD_RTC_TIME1_SEC_Msk      (0xFU << SCU_BCD_RTC_TIME1_SEC_Pos) /*!< 0x0000000F */
#define SCU_BCD_RTC_TIME1_SEC          SCU_BCD_RTC_TIME1_SEC_Msk           /*!< Current second in unit values */
#define SCU_BCD_RTC_TIME1_SEC_DEC_Pos  (4U)
#define SCU_BCD_RTC_TIME1_SEC_DEC_Msk  (0xFU << SCU_BCD_RTC_TIME1_SEC_DEC_Pos) /*!< 0x000000F0 */
#define SCU_BCD_RTC_TIME1_SEC_DEC      SCU_BCD_RTC_TIME1_SEC_DEC_Msk           /*!< Current second in decimal values */
#define SCU_BCD_RTC_TIME1_MIN_Pos      (8U)
#define SCU_BCD_RTC_TIME1_MIN_Msk      (0xFU << SCU_BCD_RTC_TIME1_MIN_Pos) /*!< 0x00000F00 */
#define SCU_BCD_RTC_TIME1_MIN          SCU_BCD_RTC_TIME1_MIN_Msk           /*!< Current minute in unit values */
#define SCU_BCD_RTC_TIME1_MIN_DEC_Pos  (12U)
#define SCU_BCD_RTC_TIME1_MIN_DEC_Msk  (0xFU << SCU_BCD_RTC_TIME1_MIN_DEC_Pos) /*!< 0x0000F000 */
#define SCU_BCD_RTC_TIME1_MIN_DEC      SCU_BCD_RTC_TIME1_MIN_DEC_Msk           /*!< Current minute in decimal values */
#define SCU_BCD_RTC_TIME1_HOUR_Pos     (16U)
#define SCU_BCD_RTC_TIME1_HOUR_Msk     (0xFU << SCU_BCD_RTC_TIME1_HOUR_Pos) /*!< 0x000F0000 */
#define SCU_BCD_RTC_TIME1_HOUR         SCU_BCD_RTC_TIME1_HOUR_Msk           /*!< Current hour in unit values */
#define SCU_BCD_RTC_TIME1_HOUR_DEC_Pos (20U)
#define SCU_BCD_RTC_TIME1_HOUR_DEC_Msk (0xFU << SCU_BCD_RTC_TIME1_HOUR_DEC_Pos) /*!< 0x00F00000 */
#define SCU_BCD_RTC_TIME1_HOUR_DEC     SCU_BCD_RTC_TIME1_HOUR_DEC_Msk           /*!< Current hour in decimal values */
#define SCU_BCD_RTC_TIME1_WEEKDAY_Pos  (24U)
#define SCU_BCD_RTC_TIME1_WEEKDAY_Msk  (0x7U << SCU_BCD_RTC_TIME1_WEEKDAY_Pos) /*!< 0x07000000 */
#define SCU_BCD_RTC_TIME1_WEEKDAY      SCU_BCD_RTC_TIME1_WEEKDAY_Msk           /*!< Current weekday */

/* Bits definition for SCU RTC Timer for BCD register 2  */
#define SCU_BCD_RTC_TIME2_DATE_Pos        (0U)
#define SCU_BCD_RTC_TIME2_DATE_Msk        (0xFU << SCU_BCD_RTC_TIME2_DATE_Pos) /*!< 0x0000000F */
#define SCU_BCD_RTC_TIME2_DATE            SCU_BCD_RTC_TIME2_DATE_Msk           /*!< Current day in unit values */
#define SCU_BCD_RTC_TIME2_DATE_DEC_Pos    (4U)
#define SCU_BCD_RTC_TIME2_DATE_DEC_Msk    (0xFU << SCU_BCD_RTC_TIME2_DATE_DEC_Pos) /*!< 0x000000F0 */
#define SCU_BCD_RTC_TIME2_DATE_DEC        SCU_BCD_RTC_TIME2_DATE_DEC_Msk           /*!< Current day in decimal values */
#define SCU_BCD_RTC_TIME2_MONTH_Pos       (8U)
#define SCU_BCD_RTC_TIME2_MONTH_Msk       (0xFU << SCU_BCD_RTC_TIME2_MONTH_Pos) /*!< 0x00000F00 */
#define SCU_BCD_RTC_TIME2_MONTH           SCU_BCD_RTC_TIME2_MONTH_Msk           /*!< Current month in unit values */
#define SCU_BCD_RTC_TIME2_MONTH_DEC_Pos   (12U)
#define SCU_BCD_RTC_TIME2_MONTH_DEC_Msk   (0xFU << SCU_BCD_RTC_TIME2_MONTH_DEC_Pos) /*!< 0x0000F000 */
#define SCU_BCD_RTC_TIME2_MONTH_DEC       SCU_BCD_RTC_TIME2_MONTH_DEC_Msk           /*!< Current month in decimal values */
#define SCU_BCD_RTC_TIME2_YEAR_Pos        (16U)
#define SCU_BCD_RTC_TIME2_YEAR_Msk        (0xFU << SCU_BCD_RTC_TIME2_YEAR_Pos) /*!< 0x000F0000 */
#define SCU_BCD_RTC_TIME2_YEAR            SCU_BCD_RTC_TIME2_YEAR_Msk           /*!< Current year in unit values */
#define SCU_BCD_RTC_TIME2_YEAR_DEC_Pos    (20U)
#define SCU_BCD_RTC_TIME2_YEAR_DEC_Msk    (0xFU << SCU_BCD_RTC_TIME2_YEAR_DEC_Pos) /*!< 0x00F00000 */
#define SCU_BCD_RTC_TIME2_YEAR_DEC        SCU_BCD_RTC_TIME2_YEAR_DEC_Msk           /*!< Current year in decimal values */
#define SCU_BCD_RTC_TIME2_CENTURY_Pos     (24U)
#define SCU_BCD_RTC_TIME2_CENTURY_Msk     (0xFU << SCU_BCD_RTC_TIME2_CENTURY_Pos) /*!< 0x0F000000 */
#define SCU_BCD_RTC_TIME2_CENTURY         SCU_BCD_RTC_TIME2_CENTURY_Msk           /*!< Current century in unit values */
#define SCU_BCD_RTC_TIME2_CENTURY_DEC_Pos (28U)
#define SCU_BCD_RTC_TIME2_CENTURY_DEC_Msk (0xFU << SCU_BCD_RTC_TIME2_CENTURY_DEC_Pos) /*!< 0xF0000000 */
#define SCU_BCD_RTC_TIME2_CENTURY_DEC     SCU_BCD_RTC_TIME2_CENTURY_DEC_Msk           /*!< Current century in decimal values */

/* Bits definition for SCU RTC Alarm Time for BCD register 1  */
#define SCU_BCD_RTC_ALM1_SEC_Pos      (0U)
#define SCU_BCD_RTC_ALM1_SEC_Msk      (0xFU << SCU_BCD_RTC_ALM1_SEC_Pos) /*!< 0x0000000F */
#define SCU_BCD_RTC_ALM1_SEC          SCU_BCD_RTC_ALM1_SEC_Msk           /*!< Alarm second setting by BCD in unit values */
#define SCU_BCD_RTC_ALM1_SEC_DEC_Pos  (4U)
#define SCU_BCD_RTC_ALM1_SEC_DEC_Msk  (0xFU << SCU_BCD_RTC_ALM1_SEC_DEC_Pos) /*!< 0x000000F0 */
#define SCU_BCD_RTC_ALM1_SEC_DEC      SCU_BCD_RTC_ALM1_SEC_DEC_Msk           /*!< Alarm second setting by BCD in decimal values */
#define SCU_BCD_RTC_ALM1_MIN_Pos      (8U)
#define SCU_BCD_RTC_ALM1_MIN_Msk      (0xFU << SCU_BCD_RTC_ALM1_MIN_Pos) /*!< 0x00000F00 */
#define SCU_BCD_RTC_ALM1_MIN          SCU_BCD_RTC_ALM1_MIN_Msk           /*!< Alarm minute setting by BCD in unit values */
#define SCU_BCD_RTC_ALM1_MIN_DEC_Pos  (12U)
#define SCU_BCD_RTC_ALM1_MIN_DEC_Msk  (0xFU << SCU_BCD_RTC_ALM1_MIN_DEC_Pos) /*!< 0x0000F000 */
#define SCU_BCD_RTC_ALM1_MIN_DEC      SCU_BCD_RTC_ALM1_MIN_DEC_Msk           /*!< Alarm minute setting by BCD in decimal values */
#define SCU_BCD_RTC_ALM1_HOUR_Pos     (16U)
#define SCU_BCD_RTC_ALM1_HOUR_Msk     (0xFU << SCU_BCD_RTC_ALM1_HOUR_Pos) /*!< 0x000F0000 */
#define SCU_BCD_RTC_ALM1_HOUR         SCU_BCD_RTC_ALM1_HOUR_Msk           /*!< Alarm hour setting by BCD in unit values */
#define SCU_BCD_RTC_ALM1_HOUR_DEC_Pos (20U)
#define SCU_BCD_RTC_ALM1_HOUR_DEC_Msk (0xFU << SCU_BCD_RTC_ALM1_HOUR_DEC_Pos) /*!< 0x00F00000 */
#define SCU_BCD_RTC_ALM1_HOUR_DEC     SCU_BCD_RTC_ALM1_HOUR_DEC_Msk           /*!< Alarm hour setting by BCD in decimal values */
#define SCU_BCD_RTC_ALM1_WEEKDAY_Pos  (24U)
#define SCU_BCD_RTC_ALM1_WEEKDAY_Msk  (0x7U << SCU_BCD_RTC_ALM1_WEEKDAY_Pos) /*!< 0x07000000 */
#define SCU_BCD_RTC_ALM1_WEEKDAY      SCU_BCD_RTC_ALM1_WEEKDAY_Msk           /*!< Alarm weekday setting by BCD*/

/* Bits definition for SCU RTC Alarm Time for BCD register 2  */
#define SCU_BCD_RTC_ALM2_DATE_Pos      (0U)
#define SCU_BCD_RTC_ALM2_DATE_Msk      (0xFU << SCU_BCD_RTC_ALM2_DATE_Pos) /*!< 0x0000000F */
#define SCU_BCD_RTC_ALM2_DATE          SCU_BCD_RTC_ALM2_DATE_Msk           /*!< Alarm day setting by BCD in unit values */
#define SCU_BCD_RTC_ALM2_DATE_DEC_Pos  (4U)
#define SCU_BCD_RTC_ALM2_DATE_DEC_Msk  (0xFU << SCU_BCD_RTC_ALM2_DATE_DEC_Pos) /*!< 0x000000F0 */
#define SCU_BCD_RTC_ALM2_DATE_DEC      SCU_BCD_RTC_ALM2_DATE_DEC_Msk           /*!< Alarm day setting by BCD in decimal values */
#define SCU_BCD_RTC_ALM2_MONTH_Pos     (8U)
#define SCU_BCD_RTC_ALM2_MONTH_Msk     (0xFU << SCU_BCD_RTC_ALM2_MONTH_Pos) /*!< 0x00000F00 */
#define SCU_BCD_RTC_ALM2_MONTH         SCU_BCD_RTC_ALM2_MONTH_Msk           /*!< Alarm month setting by BCD in unit values */
#define SCU_BCD_RTC_ALM2_MONTH_DEC_Pos (12U)
#define SCU_BCD_RTC_ALM2_MONTH_DEC_Msk (0xFU << SCU_BCD_RTC_ALM2_MONTH_DEC_Pos) /*!< 0x0000F000 */
#define SCU_BCD_RTC_ALM2_MONTH_DEC     SCU_BCD_RTC_ALM2_MONTH_DEC_Msk           /*!< Alarm month setting by BCD in decimal values */

/******************************************************************************/
/* SD                                                                         */
/******************************************************************************/
/* Bits definition for SDIO SDMA System Address register */
#define SDIO_SDMAADD_ADDR_Pos (0U)
#define SDIO_SDMAADD_ADDR_Msk (0xFFFFFFFFU << SDIO_SDMAADD_ADDR_Pos) /*!< 0xFFFFFFFF */
#define SDIO_SDMAADD_ADDR     SDIO_SDMAADD_ADDR_Msk                  /*!< SDMA system address register */

/* Bits definition for SDIO Block Control register */
#define SDIO_BLKCTRL_SIZE_Pos     (0U)
#define SDIO_BLKCTRL_SIZE_Msk     (0xFFFU << SDIO_BLKCTRL_SIZE_Pos) /*!< 0x00000FFF */
#define SDIO_BLKCTRL_SIZE         SDIO_BLKCTRL_SIZE_Msk             /*!< The block size of data transfer */
#define SDIO_BLKCTRL_BUFBOUND_Pos (12U)
#define SDIO_BLKCTRL_BUFBOUND_Msk (0x7U << SDIO_BLKCTRL_BUFBOUND_Pos) /*!< 0x00007000 */
#define SDIO_BLKCTRL_BUFBOUND     SDIO_BLKCTRL_BUFBOUND_Msk           /*!< Buffer boundary of the SDMA host */
#define SDIO_BLKCTRL_BLKCNT_Pos   (16U)
#define SDIO_BLKCTRL_BLKCNT_Msk   (0xFFFF << SDIO_BLKCTRL_BLKCNT_Pos) /*!< 0xFFFF0000 */
#define SDIO_BLKCTRL_BLKCNT       SDIO_BLKCTRL_BLKCNT_Msk             /*!< Block count of the current transfer */

/* Bits definition for SDIO Argument 1 register */
#define SDIO_ARG1_CMDARG_Pos (0U)
#define SDIO_ARG1_CMDARG_Msk (0xFFFFFFFFU << SDIO_ARG1_CMDARG_Pos) /*!< 0xFFFFFFFF */
#define SDIO_ARG1_CMDARG     SDIO_ARG1_CMDARG_Msk                  /*!< Command argument as bit[39:8] of command field */

/* Bits definition for SDIO Transfer Control register */
#define SDIO_TRANSCTRL_DMAEN_Pos    (0U)
#define SDIO_TRANSCTRL_DMAEN_Msk    (0x1U << SDIO_TRANSCTRL_DMAEN_Pos) /*!< 0x00000001 */
#define SDIO_TRANSCTRL_DMAEN        SDIO_TRANSCTRL_DMAEN_Msk           /*!< DMA enable */
#define SDIO_TRANSCTRL_BLKCNTEN_Pos (1U)
#define SDIO_TRANSCTRL_BLKCNTEN_Msk (0x1U << SDIO_TRANSCTRL_BLKCNTEN_Pos) /*!< 0x00000002 */
#define SDIO_TRANSCTRL_BLKCNTEN     SDIO_TRANSCTRL_BLKCNTEN_Msk           /*!< Block count enable */
#define SDIO_TRANSCTRL_ATCMDEN_Pos  (2U)
#define SDIO_TRANSCTRL_ATCMDEN_Msk  (0x3U << SDIO_TRANSCTRL_ATCMDEN_Pos) /*!< 0x0000000C */
#define SDIO_TRANSCTRL_ATCMDEN      SDIO_TRANSCTRL_ATCMDEN_Msk           /*!< Auto command enable */
#define SDIO_TRANSCTRL_TRANSDIR_Pos (4U)
#define SDIO_TRANSCTRL_TRANSDIR_Msk (0x1U << SDIO_TRANSCTRL_TRANSDIR_Pos) /*!< 0x00000010 */
#define SDIO_TRANSCTRL_TRANSDIR     SDIO_TRANSCTRL_TRANSDIR_Msk           /*!< Data transfer direction selection */
#define SDIO_TRANSCTRL_MULBLKEN_Pos (5U)
#define SDIO_TRANSCTRL_MULBLKEN_Msk (0x1U << SDIO_TRANSCTRL_MULBLKEN_Pos) /*!< 0x00000020 */
#define SDIO_TRANSCTRL_MULBLKEN     SDIO_TRANSCTRL_MULBLKEN_Msk           /*!< Single/Multi-block selection */

/* Bits definition for SDIO Command Issuing register */
#define SDIO_CMDISU_RSPTYPE_Pos     (0U)
#define SDIO_CMDISU_RSPTYPE_Msk     (0x3U << SDIO_CMDISU_RSPTYPE_Pos) /*!< 0x00000003 */
#define SDIO_CMDISU_RSPTYPE         SDIO_CMDISU_RSPTYPE_Msk           /*!< Response type selection */
#define SDIO_CMDISU_CMDCRCEN_Pos    (3U)
#define SDIO_CMDISU_CMDCRCEN_Msk    (0x1U << SDIO_CMDISU_CMDCRCEN_Pos) /*!< 0x00000008 */
#define SDIO_CMDISU_CMDCRCEN        SDIO_CMDISU_CMDCRCEN_Msk           /*!< Command CRC check enable */
#define SDIO_CMDISU_CMDINDCHKEN_Pos (4U)
#define SDIO_CMDISU_CMDINDCHKEN_Msk (0x1U << SDIO_CMDISU_CMDINDCHKEN_Pos) /*!< 0x00000010 */
#define SDIO_CMDISU_CMDINDCHKEN     SDIO_CMDISU_CMDINDCHKEN_Msk           /*!< Command Index check enable */
#define SDIO_CMDISU_DATPRES_Pos     (5U)
#define SDIO_CMDISU_DATPRES_Msk     (0x1U << SDIO_CMDISU_DATPRES_Pos) /*!< 0x00000020 */
#define SDIO_CMDISU_DATPRES         SDIO_CMDISU_DATPRES_Msk           /*!< Data present select */
#define SDIO_CMDISU_CMDTYPE_Pos     (6U)
#define SDIO_CMDISU_CMDTYPE_Msk     (0x3U << SDIO_CMDISU_CMDTYPE_Pos) /*!< 0x000000C0 */
#define SDIO_CMDISU_CMDTYPE         SDIO_CMDISU_CMDTYPE_Msk           /*!< Command type */
#define SDIO_CMDISU_CMDIDX_Pos      (8U)
#define SDIO_CMDISU_CMDIDX_Msk      (0x3FU << SDIO_CMDISU_CMDIDX_Pos) /*!< 0x00003F00 */
#define SDIO_CMDISU_CMDIDX          SDIO_CMDISU_CMDIDX_Msk            /*!< Command index as bit[45:40] of command field */

/* Bits definition for SDIO Response register 0~3 */
#define SDIO_RESP03_RSP_Pos (0U)
#define SDIO_RESP03_RSP_Msk (0xFFFFFFFFU << SDIO_RESP03_RSP_Pos) /*!< 0xFFFFFFFF */
#define SDIO_RESP03_RSP     SDIO_RESP03_RSP_Msk                  /*!< Command response as bit[31:0] of response field */

/* Bits definition for SDIO Buffer Data Port register */
#define SDIO_BUFDP_DATA_Pos (0U)
#define SDIO_BUFDP_DATA_Msk (0xFFFFFFFFU << SDIO_BUFDP_DATA_Pos) /*!< 0xFFFFFFFF */
#define SDIO_BUFDP_DATA     SDIO_BUFDP_DATA_Msk                  /*!< Buffer data port */

/* Bits definition for SDIO Present State register */
#define SDIO_PRSSTA_CMDINBCMD_Pos (0U)
#define SDIO_PRSSTA_CMDINBCMD_Msk (0x1U << SDIO_PRSSTA_CMDINBCMD_Pos) /*!< 0x00000001 */
#define SDIO_PRSSTA_CMDINBCMD     SDIO_PRSSTA_CMDINBCMD_Msk           /*!< Command Inhibit (CMD) */
#define SDIO_PRSSTA_CMDINBDAT_Pos (1U)
#define SDIO_PRSSTA_CMDINBDAT_Msk (0x1U << SDIO_PRSSTA_CMDINBDAT_Pos) /*!< 0x00000002 */
#define SDIO_PRSSTA_CMDINBDAT     SDIO_PRSSTA_CMDINBDAT_Msk           /*!< Command Inhibit (DAT) */
#define SDIO_PRSSTA_DATACT_Pos    (2U)
#define SDIO_PRSSTA_DATACT_Msk    (0x1U << SDIO_PRSSTA_DATACT_Pos) /*!< 0x00000004 */
#define SDIO_PRSSTA_DATACT        SDIO_PRSSTA_DATACT_Msk           /*!< Data line active,This bit is used to determine whether the Data line is in use or not */
#define SDIO_PRSSTA_WACT_Pos      (8U)
#define SDIO_PRSSTA_WACT_Msk      (0x1U << SDIO_PRSSTA_WACT_Pos) /*!< 0x00000100 */
#define SDIO_PRSSTA_WACT          SDIO_PRSSTA_WACT_Msk           /*!< Write transfer active */
#define SDIO_PRSSTA_RACT_Pos      (9U)
#define SDIO_PRSSTA_RACT_Msk      (0x1U << SDIO_PRSSTA_RACT_Pos) /*!< 0x00000200 */
#define SDIO_PRSSTA_RACT          SDIO_PRSSTA_RACT_Msk           /*!< Read transfer active */
#define SDIO_PRSSTA_BUFWEN_Pos    (10U)
#define SDIO_PRSSTA_BUFWEN_Msk    (0x1U << SDIO_PRSSTA_BUFWEN_Pos) /*!< 0x00000400 */
#define SDIO_PRSSTA_BUFWEN        SDIO_PRSSTA_BUFWEN_Msk           /*!< Buffer write enable status */
#define SDIO_PRSSTA_BUFREN_Pos    (11U)
#define SDIO_PRSSTA_BUFREN_Msk    (0x1U << SDIO_PRSSTA_BUFREN_Pos) /*!< 0x00000800 */
#define SDIO_PRSSTA_BUFREN        SDIO_PRSSTA_BUFREN_Msk           /*!< Buffer read enable status */
#define SDIO_PRSSTA_DATLV_Pos     (20U)
#define SDIO_PRSSTA_DATLV_Msk     (0xFU << SDIO_PRSSTA_DATLV_Pos) /*!< 0x00F00000 */
#define SDIO_PRSSTA_DATLV         SDIO_PRSSTA_DATLV_Msk           /*!< DATA[3:0] line signal level */
#define SDIO_PRSSTA_CMDLV_Pos     (24U)
#define SDIO_PRSSTA_CMDLV_Msk     (0x1U << SDIO_PRSSTA_CMDLV_Pos) /*!< 0x01000000 */
#define SDIO_PRSSTA_CMDLV         SDIO_PRSSTA_CMDLV_Msk           /*!< Command line signal level */

/* Bits definition for SDIO Host Control register */
#define SDIO_HSTCTRL_DATWID_Pos    (1U)
#define SDIO_HSTCTRL_DATWID_Msk    (0x1U << SDIO_HSTCTRL_DATWID_Pos) /*!< 0x00000002 */
#define SDIO_HSTCTRL_DATWID        SDIO_HSTCTRL_DATWID_Msk           /*!< Data transfer width */
#define SDIO_HSTCTRL_HSPEN_Pos     (2U)
#define SDIO_HSTCTRL_HSPEN_Msk     (0x1U << SDIO_HSTCTRL_HSPEN_Pos) /*!< 0x00000004 */
#define SDIO_HSTCTRL_HSPEN         SDIO_HSTCTRL_HSPEN_Msk           /*!< High speed enable */
#define SDIO_HSTCTRL_DMATYPE_Pos   (3U)
#define SDIO_HSTCTRL_DMATYPE_Msk   (0x3U << SDIO_HSTCTRL_DMATYPE_Pos) /*!< 0x00000018 */
#define SDIO_HSTCTRL_DMATYPE       SDIO_HSTCTRL_DMATYPE_Msk           /*!< DMA type select*/
#define SDIO_HSTCTRL_EXDATWID_Pos  (5U)
#define SDIO_HSTCTRL_EXDATWID_Msk  (0x1U << SDIO_HSTCTRL_EXDATWID_Pos) /*!< 0x00000020 */
#define SDIO_HSTCTRL_EXDATWID      SDIO_HSTCTRL_EXDATWID_Msk           /*!< Extended data transfer width*/
#define SDIO_HSTCTRL_SDBUSPW_Pos   (8U)
#define SDIO_HSTCTRL_SDBUSPW_Msk   (0x1U << SDIO_HSTCTRL_SDBUSPW_Pos) /*!<  0x00000100*/
#define SDIO_HSTCTRL_SDBUSPW       SDIO_HSTCTRL_SDBUSPW_Msk           /*!< SD bus power*/
#define SDIO_HSTCTRL_BLKGAPREQ_Pos (16U)
#define SDIO_HSTCTRL_BLKGAPREQ_Msk (0x1U << SDIO_HSTCTRL_BLKGAPREQ_Pos) /*!< 0x00020000 */
#define SDIO_HSTCTRL_BLKGAPREQ     SDIO_HSTCTRL_BLKGAPREQ_Msk           /*!< Stop at block gap request*/
#define SDIO_HSTCTRL_CONREQ_Pos    (17U)
#define SDIO_HSTCTRL_CONREQ_Msk    (0x1U << SDIO_HSTCTRL_CONREQ_Pos) /*!< 0x00040000 */
#define SDIO_HSTCTRL_CONREQ        SDIO_HSTCTRL_CONREQ_Msk           /*!< Continue request*/
#define SDIO_HSTCTRL_RWAITCTRL_Pos (18U)
#define SDIO_HSTCTRL_RWAITCTRL_Msk (0x1U << SDIO_HSTCTRL_RWAITCTRL_Pos) /*!< 0x00080000 */
#define SDIO_HSTCTRL_RWAITCTRL     SDIO_HSTCTRL_RWAITCTRL_Msk           /*!< Read wait control*/
#define SDIO_HSTCTRL_INTBLKGAP_Pos (19U)
#define SDIO_HSTCTRL_INTBLKGAP_Msk (0x1U << SDIO_HSTCTRL_INTBLKGAP_Pos) /*!< 0x00100000 */
#define SDIO_HSTCTRL_INTBLKGAP     SDIO_HSTCTRL_INTBLKGAP_Msk           /*!< Interrupt at block gap*/

/* Bits definition for SDIO Clock Control register */
#define SDIO_CLKCTRL_INTCLKEN_Pos  (0U)
#define SDIO_CLKCTRL_INTCLKEN_Msk  (0x1U << SDIO_CLKCTRL_INTCLKEN_Pos) /*!< 0x00000001 */
#define SDIO_CLKCTRL_INTCLKEN      SDIO_CLKCTRL_INTCLKEN_Msk           /*!< Internal clock enable */
#define SDIO_CLKCTRL_INTCLKSTB_Pos (1U)
#define SDIO_CLKCTRL_INTCLKSTB_Msk (0x1U << SDIO_CLKCTRL_INTCLKSTB_Pos) /*!< 0x00000002 */
#define SDIO_CLKCTRL_INTCLKSTB     SDIO_CLKCTRL_INTCLKSTB_Msk           /*!< Internal clock stable */
#define SDIO_CLKCTRL_SDCLKEN_Pos   (2U)
#define SDIO_CLKCTRL_SDCLKEN_Msk   (0x1U << SDIO_CLKCTRL_SDCLKEN_Pos) /*!< 0x00000004 */
#define SDIO_CLKCTRL_SDCLKEN       SDIO_CLKCTRL_SDCLKEN_Msk           /*!< SD clock enable */
#define SDIO_CLKCTRL_CLKGENSEL_Pos (5U)
#define SDIO_CLKCTRL_CLKGENSEL_Msk (0x1U << SDIO_CLKCTRL_CLKGENSEL_Pos) /*!< 0x00000020 */
#define SDIO_CLKCTRL_CLKGENSEL     SDIO_CLKCTRL_CLKGENSEL_Msk           /*!< Clock generator select (fixed to 0) */
#define SDIO_CLKCTRL_UCLKSET_Pos   (6U)
#define SDIO_CLKCTRL_UCLKSET_Msk   (0x3U << SDIO_CLKCTRL_UCLKSET_Pos) /*!< 0x000000C0 */
#define SDIO_CLKCTRL_UCLKSET       SDIO_CLKCTRL_UCLKSET_Msk           /*!< SD clock frequency value[9:8] for the 10-bit divided clock mode */
#define SDIO_CLKCTRL_LCLKSET_Pos   (8U)
#define SDIO_CLKCTRL_LCLKSET_Msk   (0xFFU << SDIO_CLKCTRL_LCLKSET_Pos) /*!< 0x0000FF00 */
#define SDIO_CLKCTRL_LCLKSET       SDIO_CLKCTRL_LCLKSET_Msk            /*!< SD clock frequency value[7:0] for the 10-bit divided clock mode */

/* Bits definition for SDIO Data Timeout Control register */
#define SDIO_DATTOCTRL_TOCNT_Pos (0U)
#define SDIO_DATTOCTRL_TOCNT_Msk (0xFU << SDIO_DATTOCTRL_TOCNT_Pos) /*!< 0x0000000F */
#define SDIO_DATTOCTRL_TOCNT     SDIO_DATTOCTRL_TOCNT_Msk           /*!< Data timeout counter value */

/* Bits definition for SDIO Software Reset register */
#define SDIO_SWRST_SWRSTALL_Pos (0U)
#define SDIO_SWRST_SWRSTALL_Msk (0x1U << SDIO_SWRST_SWRSTALL_Pos) /*!< 0x00000001 */
#define SDIO_SWRST_SWRSTALL     SDIO_SWRST_SWRSTALL_Msk           /*!< Software reset */
#define SDIO_SWRST_SWRSTCMD_Pos (1U)
#define SDIO_SWRST_SWRSTCMD_Msk (0x1U << SDIO_SWRST_SWRSTCMD_Pos) /*!< 0x00000002 */
#define SDIO_SWRST_SWRSTCMD     SDIO_SWRST_SWRSTCMD_Msk           /*!< Software reset for command line */
#define SDIO_SWRST_SWRSTDAT_Pos (2U)
#define SDIO_SWRST_SWRSTDAT_Msk (0x1U << SDIO_SWRST_SWRSTDAT_Pos) /*!< 0x00000004 */
#define SDIO_SWRST_SWRSTDAT     SDIO_SWRST_SWRSTDAT_Msk           /*!< Software reset for data line */

/* Bits definition for SDIO Interrupt Status register */
#define SDIO_RIS_CMDCPLIF_Pos   (0U)
#define SDIO_RIS_CMDCPLIF_Msk   (0x1U << SDIO_RIS_CMDCPLIF_Pos) /*!< 0x00000001 */
#define SDIO_RIS_CMDCPLIF       SDIO_RIS_CMDCPLIF_Msk           /*!< Command complete interrupt flag */
#define SDIO_RIS_TRANSCPLIF_Pos (1U)
#define SDIO_RIS_TRANSCPLIF_Msk (0x1U << SDIO_RIS_TRANSCPLIF_Pos) /*!< 0x00000002 */
#define SDIO_RIS_TRANSCPLIF     SDIO_RIS_TRANSCPLIF_Msk           /*!< Transfer complete interrupt flag */
#define SDIO_RIS_BLKGAPIF_Pos   (2U)
#define SDIO_RIS_BLKGAPIF_Msk   (0x1U << SDIO_RIS_BLKGAPIF_Pos) /*!< 0x00000004 */
#define SDIO_RIS_BLKGAPIF       SDIO_RIS_BLKGAPIF_Msk           /*!< Block gap event interrupt flag*/
#define SDIO_RIS_DMAIF_Pos      (3U)
#define SDIO_RIS_DMAIF_Msk      (0x1U << SDIO_RIS_DMAIF_Pos) /*!< 0x00000008 */
#define SDIO_RIS_DMAIF          SDIO_RIS_DMAIF_Msk           /*!< DMA interrupt flag*/
#define SDIO_RIS_BUFWIF_Pos     (4U)
#define SDIO_RIS_BUFWIF_Msk     (0x1U << SDIO_RIS_BUFWIF_Pos) /*!< 0x00000010 */
#define SDIO_RIS_BUFWIF         SDIO_RIS_BUFWIF_Msk           /*!< Buffer write ready interrupt flag*/
#define SDIO_RIS_BUFRIF_Pos     (5U)
#define SDIO_RIS_BUFRIF_Msk     (0x1U << SDIO_RIS_BUFRIF_Pos) /*!< 0x00000020 */
#define SDIO_RIS_BUFRIF         SDIO_RIS_BUFRIF_Msk           /*!< Buffer read ready interrupt flag*/
#define SDIO_RIS_CARDIF_Pos     (8U)
#define SDIO_RIS_CARDIF_Msk     (0x1U << SDIO_RIS_CARDIF_Pos) /*!< 0x00000100 */
#define SDIO_RIS_CARDIF         SDIO_RIS_CARDIF_Msk           /*!< Card interrupt flag*/
#define SDIO_RIS_ERRIF_Pos      (15U)
#define SDIO_RIS_ERRIF_Msk      (0x1U << SDIO_RIS_ERRIF_Pos) /*!< 0x00008000 */
#define SDIO_RIS_ERRIF          SDIO_RIS_ERRIF_Msk           /*!< Error Interrupt*/
#define SDIO_RIS_CMDTOEIF_Pos   (16U)
#define SDIO_RIS_CMDTOEIF_Msk   (0x1U << SDIO_RIS_CMDTOEIF_Pos) /*!< 0x00010000 */
#define SDIO_RIS_CMDTOEIF       SDIO_RIS_CMDTOEIF_Msk           /*!< Command timeout error interrupt flag*/
#define SDIO_RIS_CMDCRCEIF_Pos  (17U)
#define SDIO_RIS_CMDCRCEIF_Msk  (0x1U << SDIO_RIS_CMDCRCEIF_Pos) /*!< 0x00020000 */
#define SDIO_RIS_CMDCRCEIF      SDIO_RIS_CMDCRCEIF_Msk           /*!< Command CRC error interrupt flag*/
#define SDIO_RIS_CMDEBEIF_Pos   (18U)
#define SDIO_RIS_CMDEBEIF_Msk   (0x1U << SDIO_RIS_CMDEBEIF_Pos) /*!< 0x00040000 */
#define SDIO_RIS_CMDEBEIF       SDIO_RIS_CMDEBEIF_Msk           /*!< Command end bit error interrupt flag*/
#define SDIO_RIS_CMDINDEIF_Pos  (19U)
#define SDIO_RIS_CMDINDEIF_Msk  (0x1U << SDIO_RIS_CMDINDEIF_Pos) /*!< 0x00080000 */
#define SDIO_RIS_CMDINDEIF      SDIO_RIS_CMDINDEIF_Msk           /*!< Command index error interrupt flag*/
#define SDIO_RIS_DATTOEIF_Pos   (20U)
#define SDIO_RIS_DATTOEIF_Msk   (0x1U << SDIO_RIS_DATTOEIF_Pos) /*!< 0x00100000 */
#define SDIO_RIS_DATTOEIF       SDIO_RIS_DATTOEIF_Msk           /*!< Data timeout error interrupt flag*/
#define SDIO_RIS_DATCRCEIF_Pos  (21U)
#define SDIO_RIS_DATCRCEIF_Msk  (0x1U << SDIO_RIS_DATCRCEIF_Pos) /*!< 0x00200000 */
#define SDIO_RIS_DATCRCEIF      SDIO_RIS_DATCRCEIF_Msk           /*!< Data CRC error interrupt flag*/
#define SDIO_RIS_DATEBEIF_Pos   (22U)
#define SDIO_RIS_DATEBEIF_Msk   (0x1U << SDIO_RIS_DATEBEIF_Pos) /*!< 0x00400000 */
#define SDIO_RIS_DATEBEIF       SDIO_RIS_DATEBEIF_Msk           /*!< Data end bit error interrupt flag*/
#define SDIO_RIS_ATCMDEIF_Pos   (24U)
#define SDIO_RIS_ATCMDEIF_Msk   (0x1U << SDIO_RIS_ATCMDEIF_Pos) /*!< 0x01000000 */
#define SDIO_RIS_ATCMDEIF       SDIO_RIS_ATCMDEIF_Msk           /*!< Auto CMD12 error interrupt flag*/
#define SDIO_RIS_ADMAEIF_Pos    (25U)
#define SDIO_RIS_ADMAEIF_Msk    (0x1U << SDIO_RIS_ADMAEIF_Pos) /*!< 0x02000000 */
#define SDIO_RIS_ADMAEIF        SDIO_RIS_ADMAEIF_Msk           /*!< ADMA error interrupt flag*/

/* Bits definition for SDIO Interrupt Status Enable register */
#define SDIO_ISTEN_CMDCPLIE_Pos   (0U)
#define SDIO_ISTEN_CMDCPLIE_Msk   (0x1U << SDIO_ISTEN_CMDCPLIE_Pos) /*!< 0x00000001 */
#define SDIO_ISTEN_CMDCPLIE       SDIO_ISTEN_CMDCPLIE_Msk           /*!< Command complete interrupt enable */
#define SDIO_ISTEN_TRANSCPLIE_Pos (1U)
#define SDIO_ISTEN_TRANSCPLIE_Msk (0x1U << SDIO_ISTEN_TRANSCPLIE_Pos) /*!< 0x00000002 */
#define SDIO_ISTEN_TRANSCPLIE     SDIO_ISTEN_TRANSCPLIE_Msk           /*!< Transfer complete interrupt enable */
#define SDIO_ISTEN_BLKGAPIE_Pos   (2U)
#define SDIO_ISTEN_BLKGAPIE_Msk   (0x1U << SDIO_ISTEN_BLKGAPIE_Pos) /*!< 0x00000004 */
#define SDIO_ISTEN_BLKGAPIE       SDIO_ISTEN_BLKGAPIE_Msk           /*!< Block gap event interrupt enable */
#define SDIO_ISTEN_DMAIE_Pos      (3U)
#define SDIO_ISTEN_DMAIE_Msk      (0x1U << SDIO_ISTEN_DMAIE_Pos) /*!< 0x00000008 */
#define SDIO_ISTEN_DMAIE          SDIO_ISTEN_DMAIE_Msk           /*!< DMA interrupt enable */
#define SDIO_ISTEN_BUFWIE_Pos     (4U)
#define SDIO_ISTEN_BUFWIE_Msk     (0x1U << SDIO_ISTEN_BUFWIE_Pos) /*!< 0x00000010 */
#define SDIO_ISTEN_BUFWIE         SDIO_ISTEN_BUFWIE_Msk           /*!< Buffer write ready interrupt enable */
#define SDIO_ISTEN_BUFRIE_Pos     (5U)
#define SDIO_ISTEN_BUFRIE_Msk     (0x1U << SDIO_ISTEN_BUFRIE_Pos) /*!< 0x00000020 */
#define SDIO_ISTEN_BUFRIE         SDIO_ISTEN_BUFRIE_Msk           /*!< Buffer read ready interrupt enable */
#define SDIO_ISTEN_CARDIE_Pos     (8U)
#define SDIO_ISTEN_CARDIE_Msk     (0x1U << SDIO_ISTEN_CARDIE_Pos) /*!< 0x00000100 */
#define SDIO_ISTEN_CARDIE         SDIO_ISTEN_CARDIE_Msk           /*!< Card interrupt enable */
#define SDIO_ISTEN_CMDTOEIE_Pos   (16U)
#define SDIO_ISTEN_CMDTOEIE_Msk   (0x1U << SDIO_ISTEN_CMDTOEIE_Pos) /*!< 0x00010000 */
#define SDIO_ISTEN_CMDTOEIE       SDIO_ISTEN_CMDTOEIE_Msk           /*!< Command timeout error interrupt enable */
#define SDIO_ISTEN_CMDCRCEIE_Pos  (17U)
#define SDIO_ISTEN_CMDCRCEIE_Msk  (0x1U << SDIO_ISTEN_CMDCRCEIE_Pos) /*!< 0x00020000 */
#define SDIO_ISTEN_CMDCRCEIE      SDIO_ISTEN_CMDCRCEIE_Msk           /*!< Command CRC error interrupt enable */
#define SDIO_ISTEN_CMDEBEIE_Pos   (18U)
#define SDIO_ISTEN_CMDEBEIE_Msk   (0x1U << SDIO_ISTEN_CMDEBEIE_Pos) /*!< 0x00040000 */
#define SDIO_ISTEN_CMDEBEIE       SDIO_ISTEN_CMDEBEIE_Msk           /*!< Command end bit error interrupt enable */
#define SDIO_ISTEN_CMDINDEIE_Pos  (19U)
#define SDIO_ISTEN_CMDINDEIE_Msk  (0x1U << SDIO_ISTEN_CMDINDEIE_Pos) /*!< 0x00080000 */
#define SDIO_ISTEN_CMDINDEIE      SDIO_ISTEN_CMDINDEIE_Msk           /*!< Command index error interrupt enable */
#define SDIO_ISTEN_DATTOEIE_Pos   (20U)
#define SDIO_ISTEN_DATTOEIE_Msk   (0x1U << SDIO_ISTEN_DATTOEIE_Pos) /*!< 0x00100000 */
#define SDIO_ISTEN_DATTOEIE       SDIO_ISTEN_DATTOEIE_Msk           /*!< Data timeout error interrupt enable */
#define SDIO_ISTEN_DATCRCEIE_Pos  (21U)
#define SDIO_ISTEN_DATCRCEIE_Msk  (0x1U << SDIO_ISTEN_DATCRCEIE_Pos) /*!< 0x00200000 */
#define SDIO_ISTEN_DATCRCEIE      SDIO_ISTEN_DATCRCEIE_Msk           /*!< Data CRC error interrupt enable */
#define SDIO_ISTEN_DATEBEIE_Pos   (22U)
#define SDIO_ISTEN_DATEBEIE_Msk   (0x1U << SDIO_ISTEN_DATEBEIE_Pos) /*!< 0x00400000 */
#define SDIO_ISTEN_DATEBEIE       SDIO_ISTEN_DATEBEIE_Msk           /*!< Data end bit error interrupt enable */
#define SDIO_ISTEN_ATCMDEIE_Pos   (24U)
#define SDIO_ISTEN_ATCMDEIE_Msk   (0x1U << SDIO_ISTEN_ATCMDEIE_Pos) /*!< 0x01000000 */
#define SDIO_ISTEN_ATCMDEIE       SDIO_ISTEN_ATCMDEIE_Msk           /*!< Auto CMD12 error interrupt enable */
#define SDIO_ISTEN_ADMAEIE_Pos    (25U)
#define SDIO_ISTEN_ADMAEIE_Msk    (0x1U << SDIO_ISTEN_ADMAEIE_Pos) /*!< 0x02000000 */
#define SDIO_ISTEN_ADMAEIE        SDIO_ISTEN_ADMAEIE_Msk           /*!< ADMA error interrupt enable */

/* Bits definition for SDIO Normal Interrupt Signal Enable register */
#define SDIO_ISGEN_CMDCPLISE_Pos   (0U)
#define SDIO_ISGEN_CMDCPLISE_Msk   (0x1U << SDIO_ISGEN_CMDCPLISE_Pos) /*!< 0x00000001 */
#define SDIO_ISGEN_CMDCPLISE       SDIO_ISGEN_CMDCPLISE_Msk           /*!< Command complete interrupt signal enable */
#define SDIO_ISGEN_TRANSCPLISE_Pos (1U)
#define SDIO_ISGEN_TRANSCPLISE_Msk (0x1U << SDIO_ISGEN_TRANSCPLISE_Pos) /*!< 0x00000002 */
#define SDIO_ISGEN_TRANSCPLISE     SDIO_ISGEN_TRANSCPLISE_Msk           /*!< Transfer complete interrupt signal enable */
#define SDIO_ISGEN_BLKGAPISE_Pos   (2U)
#define SDIO_ISGEN_BLKGAPISE_Msk   (0x1U << SDIO_ISGEN_BLKGAPISE_Pos) /*!< 0x00000004 */
#define SDIO_ISGEN_BLKGAPISE       SDIO_ISGEN_BLKGAPISE_Msk           /*!< Block gap event interrupt signal enable */
#define SDIO_ISGEN_DMAISE_Pos      (3U)
#define SDIO_ISGEN_DMAISE_Msk      (0x1U << SDIO_ISGEN_DMAISE_Pos) /*!< 0x00000008 */
#define SDIO_ISGEN_DMAISE          SDIO_ISGEN_DMAISE_Msk           /*!< DMA interrupt signal enable */
#define SDIO_ISGEN_BUFWISE_Pos     (4U)
#define SDIO_ISGEN_BUFWISE_Msk     (0x1U << SDIO_ISGEN_BUFWISE_Pos) /*!< 0x00000010 */
#define SDIO_ISGEN_BUFWISE         SDIO_ISGEN_BUFWISE_Msk           /*!< Buffer write ready interrupt signal enable */
#define SDIO_ISGEN_BUFRISE_Pos     (5U)
#define SDIO_ISGEN_BUFRISE_Msk     (0x1U << SDIO_ISGEN_BUFRISE_Pos) /*!< 0x00000020 */
#define SDIO_ISGEN_BUFRISE         SDIO_ISGEN_BUFRISE_Msk           /*!< Buffer read ready interrupt signal enable */
#define SDIO_ISGEN_CARDISE_Pos     (8U)
#define SDIO_ISGEN_CARDISE_Msk     (0x1U << SDIO_ISGEN_CARDISE_Pos) /*!< 0x00000100 */
#define SDIO_ISGEN_CARDISE         SDIO_ISGEN_CARDISE_Msk           /*!< Card interrupt signal enable */
#define SDIO_ISGEN_CMDTOEISE_Pos   (16U)
#define SDIO_ISGEN_CMDTOEISE_Msk   (0x1U << SDIO_ISGEN_CMDTOEISE_Pos) /*!< 0x00010000 */
#define SDIO_ISGEN_CMDTOEISE       SDIO_ISGEN_CMDTOEISE_Msk           /*!< Command timeout error interrupt signal enable */
#define SDIO_ISGEN_CMDCRCEISE_Pos  (17U)
#define SDIO_ISGEN_CMDCRCEISE_Msk  (0x1U << SDIO_ISGEN_CMDCRCEISE_Pos) /*!< 0x00020000 */
#define SDIO_ISGEN_CMDCRCEISE      SDIO_ISGEN_CMDCRCEISE_Msk           /*!< Command CRC error interrupt signal enable */
#define SDIO_ISGEN_CMDEBEISE_Pos   (18U)
#define SDIO_ISGEN_CMDEBEISE_Msk   (0x1U << SDIO_ISGEN_CMDEBEISE_Pos) /*!< 0x00040000 */
#define SDIO_ISGEN_CMDEBEISE       SDIO_ISGEN_CMDEBEISE_Msk           /*!< Command end bit error interrupt signal enable */
#define SDIO_ISGEN_CMDINDEISE_Pos  (19U)
#define SDIO_ISGEN_CMDINDEISE_Msk  (0x1U << SDIO_ISGEN_CMDINDEISE_Pos) /*!< 0x00080000 */
#define SDIO_ISGEN_CMDINDEISE      SDIO_ISGEN_CMDINDEISE_Msk           /*!< Command index error interrupt signal enable */
#define SDIO_ISGEN_DATTOEISE_Pos   (20U)
#define SDIO_ISGEN_DATTOEISE_Msk   (0x1U << SDIO_ISGEN_DATTOEISE_Pos) /*!< 0x00100000 */
#define SDIO_ISGEN_DATTOEISE       SDIO_ISGEN_DATTOEISE_Msk           /*!< Data timeout error interrupt signal enable */
#define SDIO_ISGEN_DATCRCEISE_Pos  (21U)
#define SDIO_ISGEN_DATCRCEISE_Msk  (0x1U << SDIO_ISGEN_DATCRCEISE_Pos) /*!< 0x00200000 */
#define SDIO_ISGEN_DATCRCEISE      SDIO_ISGEN_DATCRCEISE_Msk           /*!< Data CRC error interrupt signal enable */
#define SDIO_ISGEN_DATEBEISE_Pos   (22U)
#define SDIO_ISGEN_DATEBEISE_Msk   (0x1U << SDIO_ISGEN_DATEBEISE_Pos) /*!< 0x00400000 */
#define SDIO_ISGEN_DATEBEISE       SDIO_ISGEN_DATEBEISE_Msk           /*!< Data end bit error interrupt signal enable */
#define SDIO_ISGEN_ATCMDEISE_Pos   (24U)
#define SDIO_ISGEN_ATCMDEISE_Msk   (0x1U << SDIO_ISGEN_ATCMDEISE_Pos) /*!< 0x01000000 */
#define SDIO_ISGEN_ATCMDEISE       SDIO_ISGEN_ATCMDEISE_Msk           /*!< Auto CMD12 error interrupt signal enable */
#define SDIO_ISGEN_ADMAEISE_Pos    (25U)
#define SDIO_ISGEN_ADMAEISE_Msk    (0x1U << SDIO_ISGEN_ADMAEISE_Pos) /*!< 0x02000000 */
#define SDIO_ISGEN_ADMAEISE        SDIO_ISGEN_ADMAEISE_Msk           /*!< ADMA error interrupt signal enable */

/* Bits definition for SDIO Auto CMD12 Error Status register */
#define SDIO_ATCMDERR_ATCMDEXCT_Pos  (0U)
#define SDIO_ATCMDERR_ATCMDEXCT_Msk  (0x1U << SDIO_ATCMDERR_ATCMDEXCT_Pos) /*!< 0x00000001 */
#define SDIO_ATCMDERR_ATCMDEXCT      SDIO_ATCMDERR_ATCMDEXCT_Msk           /*!< Auto CMD12 not executed */
#define SDIO_ATCMDERR_ATCMDTOE_Pos   (1U)
#define SDIO_ATCMDERR_ATCMDTOE_Msk   (0x1U << SDIO_ATCMDERR_ATCMDTOE_Pos) /*!< 0x00000002 */
#define SDIO_ATCMDERR_ATCMDTOE       SDIO_ATCMDERR_ATCMDTOE_Msk           /*!< Auto CMD time out error */
#define SDIO_ATCMDERR_ATCMDCRCE_Pos  (2U)
#define SDIO_ATCMDERR_ATCMDCRCE_Msk  (0x1U << SDIO_ATCMDERR_ATCMDCRCE_Pos) /*!< 0x00000004 */
#define SDIO_ATCMDERR_ATCMDCRCE      SDIO_ATCMDERR_ATCMDCRCE_Msk           /*!< Auto CMD CRC error */
#define SDIO_ATCMDERR_ATCMDEBE_Pos   (3U)
#define SDIO_ATCMDERR_ATCMDEBE_Msk   (0x1U << SDIO_ATCMDERR_ATCMDEBE_Pos) /*!< 0x00000008 */
#define SDIO_ATCMDERR_ATCMDEBE       SDIO_ATCMDERR_ATCMDEBE_Msk           /*!< Auto CMD end bit error */
#define SDIO_ATCMDERR_ATCMDINDE_Pos  (4U)
#define SDIO_ATCMDERR_ATCMDINDE_Msk  (0x1U << SDIO_ATCMDERR_ATCMDINDE_Pos) /*!< 0x00000010 */
#define SDIO_ATCMDERR_ATCMDINDE      SDIO_ATCMDERR_ATCMDINDE_Msk           /*!< Auto CMD index error */
#define SDIO_ATCMDERR_AUTCMDEXCT_Pos (7U)
#define SDIO_ATCMDERR_AUTCMDEXCT_Msk (0x1U << SDIO_ATCMDERR_AUTCMDEXCT_Pos) /*!< 0x00000080 */
#define SDIO_ATCMDERR_AUTCMDEXCT     SDIO_ATCMDERR_AUTCMDEXCT_Msk           /*!< Command not executed by Auto CMD12 error */

/* Bits definition for SDIO Force Event register */
#define SDIO_FORCEEVT_FATCMDEXCT_Pos  (0U)
#define SDIO_FORCEEVT_FATCMDEXCT_Msk  (0x1U << SDIO_FORCEEVT_FATCMDEXCT_Pos) /*!< 0x00000001 */
#define SDIO_FORCEEVT_FATCMDEXCT      SDIO_FORCEEVT_FATCMDEXCT_Msk           /*!< Force event for Auto CMD12 not executed */
#define SDIO_FORCEEVT_FATCMDTOE_Pos   (1U)
#define SDIO_FORCEEVT_FATCMDTOE_Msk   (0x1U << SDIO_FORCEEVT_FATCMDTOE_Pos) /*!< 0x00000002 */
#define SDIO_FORCEEVT_FATCMDTOE       SDIO_FORCEEVT_FATCMDTOE_Msk           /*!< Force event for Auto CMD time out error */
#define SDIO_FORCEEVT_FATCMDCRCE_Pos  (2U)
#define SDIO_FORCEEVT_FATCMDCRCE_Msk  (0x1U << SDIO_FORCEEVT_FATCMDCRCE_Pos) /*!< 0x00000004 */
#define SDIO_FORCEEVT_FATCMDCRCE      SDIO_FORCEEVT_FATCMDCRCE_Msk           /*!< Force event for Auto CMD CRC error */
#define SDIO_FORCEEVT_FATCMDEBE_Pos   (3U)
#define SDIO_FORCEEVT_FATCMDEBE_Msk   (0x1U << SDIO_FORCEEVT_FATCMDEBE_Pos) /*!< 0x00000008 */
#define SDIO_FORCEEVT_FATCMDEBE       SDIO_FORCEEVT_FATCMDEBE_Msk           /*!< Force event for Auto CMD end bit error */
#define SDIO_FORCEEVT_FATCMDINDE_Pos  (4U)
#define SDIO_FORCEEVT_FATCMDINDE_Msk  (0x1U << SDIO_FORCEEVT_FATCMDINDE_Pos) /*!< 0x00000010 */
#define SDIO_FORCEEVT_FATCMDINDE      SDIO_FORCEEVT_FATCMDINDE_Msk           /*!< Force event for Auto CMD index error */
#define SDIO_FORCEEVT_FAUTCMDEXCT_Pos (7U)
#define SDIO_FORCEEVT_FAUTCMDEXCT_Msk (0x1U << SDIO_FORCEEVT_FAUTCMDEXCT_Pos) /*!< 0x00000080 */
#define SDIO_FORCEEVT_FAUTCMDEXCT     SDIO_FORCEEVT_FAUTCMDEXCT_Msk           /*!< Force event for Command not executed by Auto CMD12 error */
#define SDIO_FORCEEVT_FCMDTOE_Pos     (16U)
#define SDIO_FORCEEVT_FCMDTOE_Msk     (0x1U << SDIO_FORCEEVT_FCMDTOE_Pos) /*!< 0x00010000 */
#define SDIO_FORCEEVT_FCMDTOE         SDIO_FORCEEVT_FCMDTOE_Msk           /*!< Force event for Command timeout error */
#define SDIO_FORCEEVT_FCMDCRCE_Pos    (17U)
#define SDIO_FORCEEVT_FCMDCRCE_Msk    (0x1U << SDIO_FORCEEVT_FCMDCRCE_Pos) /*!< 0x00020000 */
#define SDIO_FORCEEVT_FCMDCRCE        SDIO_FORCEEVT_FCMDCRCE_Msk           /*!< Force event for Command CRC error */
#define SDIO_FORCEEVT_FCMDEBE_Pos     (18U)
#define SDIO_FORCEEVT_FCMDEBE_Msk     (0x1U << SDIO_FORCEEVT_FCMDEBE_Pos) /*!< 0x00040000 */
#define SDIO_FORCEEVT_FCMDEBE         SDIO_FORCEEVT_FCMDEBE_Msk           /*!< Force event for Command end bit error */
#define SDIO_FORCEEVT_FCMDINDE_Pos    (19U)
#define SDIO_FORCEEVT_FCMDINDE_Msk    (0x1U << SDIO_FORCEEVT_FCMDINDE_Pos) /*!< 0x00080000 */
#define SDIO_FORCEEVT_FCMDINDE        SDIO_FORCEEVT_FCMDINDE_Msk           /*!< Force event for Command index error */
#define SDIO_FORCEEVT_FDATTOE_Pos     (20U)
#define SDIO_FORCEEVT_FDATTOE_Msk     (0x1U << SDIO_FORCEEVT_FDATTOE_Pos) /*!< 0x00100000 */
#define SDIO_FORCEEVT_FDATTOE         SDIO_FORCEEVT_FDATTOE_Msk           /*!< Force event for Data timeout error */
#define SDIO_FORCEEVT_FDATCRCE_Pos    (21U)
#define SDIO_FORCEEVT_FDATCRCE_Msk    (0x1U << SDIO_FORCEEVT_FDATCRCE_Pos) /*!< 0x00200000 */
#define SDIO_FORCEEVT_FDATCRCE        SDIO_FORCEEVT_FDATCRCE_Msk           /*!< Force event for Data CRC error */
#define SDIO_FORCEEVT_FDATEBE_Pos     (22U)
#define SDIO_FORCEEVT_FDATEBE_Msk     (0x1U << SDIO_FORCEEVT_FDATEBE_Pos) /*!< 0x00400000 */
#define SDIO_FORCEEVT_FDATEBE         SDIO_FORCEEVT_FDATEBE_Msk           /*!< Force event for Data end bit error */
#define SDIO_FORCEEVT_FATCMDE_Pos     (24U)
#define SDIO_FORCEEVT_FATCMDE_Msk     (0x1U << SDIO_FORCEEVT_FATCMDE_Pos) /*!< 0x01000000 */
#define SDIO_FORCEEVT_FATCMDE         SDIO_FORCEEVT_FATCMDE_Msk           /*!< Force event for Auto CMD error */
#define SDIO_FORCEEVT_FADMAE_Pos      (25U)
#define SDIO_FORCEEVT_FADMAE_Msk      (0x1U << SDIO_FORCEEVT_FADMAE_Pos) /*!< 0x02000000 */
#define SDIO_FORCEEVT_FADMAE          SDIO_FORCEEVT_FADMAE_Msk           /*!< Force event for ADMA error */
#define SDIO_FORCEEVT_FAHBRSPE_Pos    (28U)
#define SDIO_FORCEEVT_FAHBRSPE_Msk    (0x1U << SDIO_FORCEEVT_FAHBRSPE_Pos) /*!< 0x10000000 */
#define SDIO_FORCEEVT_FAHBRSPE        SDIO_FORCEEVT_FAHBRSPE_Msk           /*!< Force event for the AHB response error */

/* Bits definition for SDIO ADMA Error Status register */
#define SDIO_ADMAERRSTA_ADMMERR_Pos    (0U)
#define SDIO_ADMAERRSTA_ADMMERR_Msk    (0x3U << SDIO_ADMAERRSTA_ADMMERR_Pos) /*!< 0x00000003 */
#define SDIO_ADMAERRSTA_ADMMERR        SDIO_ADMAERRSTA_ADMMERR_Msk           /*!< ADMA error state */
#define SDIO_ADMAERRSTA_ADMALENERR_Pos (2U)
#define SDIO_ADMAERRSTA_ADMALENERR_Msk (0x1U << SDIO_ADMAERRSTA_ADMALENERR_Pos) /*!< 0x00000004 */
#define SDIO_ADMAERRSTA_ADMALENERR     SDIO_ADMAERRSTA_ADMALENERR_Msk           /*!< ADMA length mismatch error */

/* Bits definition for SDIO ADMA System Address register */
#define SDIO_ADMAADD_ADMALADD_Pos (0U)
#define SDIO_ADMAADD_ADMALADD_Msk (0xFFFFFFFFU << SDIO_ADMAADD_ADMALADD_Pos) /*!< 0xFFFFFFFF */
#define SDIO_ADMAADD_ADMALADD     SDIO_ADMAADD_ADMALADD_Msk                  /*!< Lower 32-bit ADMA system address */

/* Bits definition for SDIO DMA Handshake Enable register */
#define SDIO_DMAHSKEN_Pos (0U)
#define SDIO_DMAHSKEN_Msk (0x1U << SDIO_DMAHSKEN_Pos) /*!< 0x00000001 */
#define SDIO_DMAHSKEN     SDIO_DMAHSKEN_Msk           /*!< DMA handshake enable */

/******************************************************************************/
/* SSP                                                                        */
/******************************************************************************/
/* Bits definition for SSP CTRL0 register */
#define SSP_CTRL0_SCLKPH_Pos        (0U)
#define SSP_CTRL0_SCLKPH_Msk        (0x1U << SSP_CTRL0_SCLKPH_Pos) /*!< 0x00000001 */
#define SSP_CTRL0_SCLKPH            SSP_CTRL0_SCLKPH_Msk           /*!< SCLK polarity for SPI mode */
#define SSP_CTRL0_SCLKPH_2EDGE      (0x1U << SSP_CTRL0_SCLKPH_Pos) /*!< SCLK will start running after half an SCLK cycle */
#define SSP_CTRL0_SCLKPO_Pos        (1U)
#define SSP_CTRL0_SCLKPO_Msk        (0x1U << SSP_CTRL0_SCLKPO_Pos) /*!< 0x00000002 */
#define SSP_CTRL0_SCLKPO            SSP_CTRL0_SCLKPO_Msk           /*!< SCLK status when SSP is idle */
#define SSP_CTRL0_SCLKPO_HIGH       (0x1U << SSP_CTRL0_SCLKPO_Pos) /*!< SCLK will remain High when SSP is idle */
#define SSP_CTRL0_OPM_Pos           (2U)
#define SSP_CTRL0_OPM_Msk           (0x3U << SSP_CTRL0_OPM_Pos) /*!< 0x0000000C */
#define SSP_CTRL0_OPM               SSP_CTRL0_OPM_Msk           /*!< Operation mode : Master or Slave */
#define SSP_CTRL0_OPM_SLAVE_MONO    (0x0U << SSP_CTRL0_OPM_Pos) /*!< Slave Mode,I2S is MONO in addition */
#define SSP_CTRL0_OPM_SLAVE_STEREO  (0x1U << SSP_CTRL0_OPM_Pos) /*!< Slave Mode,I2S is STEREO in addition */
#define SSP_CTRL0_OPM_MASTER_MONO   (0x2U << SSP_CTRL0_OPM_Pos) /*!< Master Mode,I2S is MONO in addition */
#define SSP_CTRL0_OPM_MASTER_STEREO (0x3U << SSP_CTRL0_OPM_Pos) /*!< Master Mode,I2S is STEREO in addition */
#define SSP_CTRL0_FSJSTFY_Pos       (4U)
#define SSP_CTRL0_FSJSTFY_Msk       (0x1U << SSP_CTRL0_FSJSTFY_Pos) /*!< 0x00000010 */
#define SSP_CTRL0_FSJSTFY           SSP_CTRL0_FSJSTFY_Msk           /*!< The padding data will be in front of the serial data */
#define SSP_CTRL0_FSPO_Pos          (5U)
#define SSP_CTRL0_FSPO_Msk          (0x1U << SSP_CTRL0_FSPO_Pos) /*!< 0x00000020 */
#define SSP_CTRL0_FSPO              SSP_CTRL0_FSPO_Msk           /*!< SEL Active status */
#define SSP_CTRL0_FSPO_LOW          (0x1U << SSP_CTRL0_FSPO_Pos) /*!< SEL Active low */
#define SSP_CTRL0_LSB_Pos           (6U)
#define SSP_CTRL0_LSB_Msk           (0x1U << SSP_CTRL0_LSB_Pos) /*!< 0x00000040 */
#define SSP_CTRL0_LSB               SSP_CTRL0_LSB_Msk           /*!< LSB first */
#define SSP_CTRL0_LBM_Pos           (7U)
#define SSP_CTRL0_LBM_Msk           (0x1U << SSP_CTRL0_LBM_Pos) /*!< 0x00000080 */
#define SSP_CTRL0_LBM               SSP_CTRL0_LBM_Msk           /*!< The transmitted data will be connected to the received data internally */
#define SSP_CTRL0_FSDIST_Pos        (8U)
#define SSP_CTRL0_FSDIST_Msk        (0x3U << SSP_CTRL0_FSDIST_Pos) /*!< 0x00000300 */
#define SSP_CTRL0_FSDIST            SSP_CTRL0_FSDIST_Msk           /*!< WS and data distance for I2S only */
#define SSP_CTRL0_FSDIST_0SCLK      (0x0U << SSP_CTRL0_FSDIST_Pos) /*!< 0 SCLK cycle between the first bit */
#define SSP_CTRL0_FSDIST_1SCLK      (0x1U << SSP_CTRL0_FSDIST_Pos) /*!< 1 SCLK cycle between the first bit */
#define SSP_CTRL0_FSDIST_2SCLK      (0x2U << SSP_CTRL0_FSDIST_Pos) /*!< 2 SCLK cycle between the first bit */
#define SSP_CTRL0_FSDIST_3SCLK      SSP_CTRL0_FSDIST               /*!< 3 SCLK cycle between the first bit */
#define SSP_CTRL0_FLASH_Pos         (11U)
#define SSP_CTRL0_FLASH_Msk         (0x1U << SSP_CTRL0_FLASH_Pos) /*!< 0x00000800 */
#define SSP_CTRL0_FLASH             SSP_CTRL0_FLASH_Msk           /*!< Enable SSP Flash mode */
#define SSP_CTRL0_FFMT_Pos          (12U)
#define SSP_CTRL0_FFMT_Msk          (0x7U << SSP_CTRL0_FFMT_Pos) /*!< 0x00007000 */
#define SSP_CTRL0_FFMT              SSP_CTRL0_FFMT_Msk           /*!< Frame format */
#define SSP_CTRL0_FFMT_MotorolaSPI  (0x1U << SSP_CTRL0_FFMT_Pos) /*!< Motorola SPI */
#define SSP_CTRL0_FFMT_MicrowireSPI (0x2U << SSP_CTRL0_FFMT_Pos) /*!< MICROWIRE SPI */
#define SSP_CTRL0_FFMT_PhilipsI2S   (0x3U << SSP_CTRL0_FFMT_Pos) /*!< Philips I2S */
#define SSP_CTRL0_SPIFSPO_Pos       (15U)
#define SSP_CTRL0_SPIFSPO_Msk       (0x1U << SSP_CTRL0_SPIFSPO_Pos) /*!< 0x00008000 */
#define SSP_CTRL0_SPIFSPO           SSP_CTRL0_SPIFSPO_Msk           /*!< SEL Active for SSP mode */
#define SSP_CTRL0_SPIFSPO_HIGH      (0x1U << SSP_CTRL0_SPIFSPO_Pos) /*!< SEL Active high for SSP mode */
#define SSP_CTRL0_SCLKFDBK_Pos      (16U)
#define SSP_CTRL0_SCLKFDBK_Msk      (0x1U << SSP_CTRL0_SCLKFDBK_Pos) /*!< 0x00010000 */
#define SSP_CTRL0_SCLKFDBK          SSP_CTRL0_SCLKFDBK_Msk           /*!< Operation mode : Master */
#define SSP_CTRL0_FSFDBK_Pos        (17U)
#define SSP_CTRL0_FSFDBK_Msk        (0x1U << SSP_CTRL0_FSFDBK_Pos) /*!< 0x00020000 */
#define SSP_CTRL0_FSFDBK            SSP_CTRL0_FSFDBK_Msk           /*!< Operation mode : Master */
#define SSP_CTRL0_FLASHTX_Pos       (18U)
#define SSP_CTRL0_FLASHTX_Msk       (0x1U << SSP_CTRL0_FLASHTX_Pos) /*!< 0x00040000 */
#define SSP_CTRL0_FLASHTX           SSP_CTRL0_FLASHTX_Msk           /*!< FLASH mode transmit control */
#define SSP_CTRL0_SPICONTX_Pos      (19U)
#define SSP_CTRL0_SPICONTX_Msk      (0x1U << SSP_CTRL0_SPICONTX_Pos) /*!< 0x00080000 */
#define SSP_CTRL0_SPICONTX          SSP_CTRL0_SPICONTX_Msk           /*!<  Continuously transfer without asserting SEL when the Transmit FIFO is not empty */

/* Bits definition for SSP CTRL1 register */
#define SSP_CTRL1_SCLKDIV_Pos (0U)
#define SSP_CTRL1_SCLKDIV_Msk (0xFFFFU << SSP_CTRL1_SCLKDIV_Pos) /*!< 0x0000FFFF */
#define SSP_CTRL1_SCLKDIV     SSP_CTRL1_SCLKDIV_Msk              /*!< SCLK divider */
#define SSP_CTRL1_SDL_Pos     (16U)
#define SSP_CTRL1_SDL_Msk     (0x7FU << SSP_CTRL1_SDL_Pos) /*!< 0x007F0000 */
#define SSP_CTRL1_SDL         SSP_CTRL1_SDL_Msk            /*!< Serial data length */
#define SSP_CTRL1_PDL_Pos     (24U)
#define SSP_CTRL1_PDL_Msk     (0xFFU << SSP_CTRL1_PDL_Pos) /*!< 0xFF000000 */
#define SSP_CTRL1_PDL         SSP_CTRL1_PDL_Msk            /*!< Serial data length */

/* Bits definition for SSP CTRL2 register */
#define SSP_CTRL2_SSPEN_Pos  (0U)
#define SSP_CTRL2_SSPEN_Msk  (0x1U << SSP_CTRL2_SSPEN_Pos) /*!< 0x00000001 */
#define SSP_CTRL2_SSPEN      SSP_CTRL2_SSPEN_Msk           /*!< Enable SSP */
#define SSP_CTRL2_TXDOE_Pos  (1U)
#define SSP_CTRL2_TXDOE_Msk  (0x1U << SSP_CTRL2_TXDOE_Pos) /*!< 0x00000002 */
#define SSP_CTRL2_TXDOE      SSP_CTRL2_TXDOE_Msk           /*!< Enable Transmit data output */
#define SSP_CTRL2_RXFCLR_Pos (2U)
#define SSP_CTRL2_RXFCLR_Msk (0x1U << SSP_CTRL2_RXFCLR_Pos) /*!< 0x00000004 */
#define SSP_CTRL2_RXFCLR     SSP_CTRL2_RXFCLR_Msk           /*!< Clear RX FIFO */
#define SSP_CTRL2_TXFCLR_Pos (3U)
#define SSP_CTRL2_TXFCLR_Msk (0x1U << SSP_CTRL2_TXFCLR_Pos) /*!< 0x00000008 */
#define SSP_CTRL2_TXFCLR     SSP_CTRL2_TXFCLR_Msk           /*!< Clear TX FIFO */
#define SSP_CTRL2_RESET_Pos  (6U)
#define SSP_CTRL2_RESET_Msk  (0x1U << SSP_CTRL2_RESET_Pos) /*!< 0x00000040 */
#define SSP_CTRL2_RESET      SSP_CTRL2_RESET_Msk           /*!< Clear TX FIFO */
#define SSP_CTRL2_RXEN_Pos   (7U)
#define SSP_CTRL2_RXEN_Msk   (0x1U << SSP_CTRL2_RXEN_Pos) /*!< 0x00000080 */
#define SSP_CTRL2_RXEN       SSP_CTRL2_RXEN_Msk           /*!< Enable Rx */
#define SSP_CTRL2_TXEN_Pos   (8U)
#define SSP_CTRL2_TXEN_Msk   (0x1U << SSP_CTRL2_TXEN_Pos) /*!< 0x00000100 */
#define SSP_CTRL2_TXEN       SSP_CTRL2_TXEN_Msk           /*!< Enable Tx */
#define SSP_CTRL2_FS_Pos     (9U)
#define SSP_CTRL2_FS_Msk     (0x1U << SSP_CTRL2_FS_Pos) /*!< 0x00000200 */
#define SSP_CTRL2_FS         SSP_CTRL2_FS_Msk           /*!< SEL active high */

/* Bits definition for SSP STATUS register */
#define SSP_STATUS_RFF_Pos  (0U)
#define SSP_STATUS_RFF_Msk  (0x1U << SSP_STATUS_RFF_Pos) /*!< 0x00000001 */
#define SSP_STATUS_RFF      SSP_STATUS_RFF_Msk           /*!< Rx FIFO Full */
#define SSP_STATUS_TFNF_Pos (1U)
#define SSP_STATUS_TFNF_Msk (0x1U << SSP_STATUS_TFNF_Pos) /*!< 0x00000002 */
#define SSP_STATUS_TFNF     SSP_STATUS_TFNF_Msk           /*!< Tx FIFO Not Full */
#define SSP_STATUS_BUSY_Pos (2U)
#define SSP_STATUS_BUSY_Msk (0x1U << SSP_STATUS_BUSY_Pos) /*!< 0x00000004 */
#define SSP_STATUS_BUSY     SSP_STATUS_BUSY_Msk           /*!< SSP is Busy */
#define SSP_STATUS_RFVE_Pos (4U)
#define SSP_STATUS_RFVE_Msk (0x3FU << SSP_STATUS_RFVE_Pos) /*!< 0x000003F0 */
#define SSP_STATUS_RFVE     SSP_STATUS_RFVE_Msk            /*!< RFVE Mask */
#define SSP_STATUS_TFVE_Pos (12U)
#define SSP_STATUS_TFVE_Msk (0x3FU << SSP_STATUS_TFVE_Pos) /*!< 0x0003F000 */
#define SSP_STATUS_TFVE     SSP_STATUS_TFVE_Msk            /*!< TFVE Mask */

/* Bits definition for SSP ICTRL register */
#define SSP_ICTRL_RFORIEN_Pos     (0U)
#define SSP_ICTRL_RFORIEN_Msk     (0x1U << SSP_ICTRL_RFORIEN_Pos) /*!< 0x00000001 */
#define SSP_ICTRL_RFORIEN         SSP_ICTRL_RFORIEN_Msk           /*!< Enable Receive FIFO Overrun Interrupt */
#define SSP_ICTRL_TFURIEN_Pos     (1U)
#define SSP_ICTRL_TFURIEN_Msk     (0x1U << SSP_ICTRL_TFURIEN_Pos) /*!< 0x00000002 */
#define SSP_ICTRL_TFURIEN         SSP_ICTRL_TFURIEN_Msk           /*!< Enable Transmit FIFO underrun Interrupt */
#define SSP_ICTRL_RFTHIEN_Pos     (2U)
#define SSP_ICTRL_RFTHIEN_Msk     (0x1U << SSP_ICTRL_RFTHIEN_Pos) /*!< 0x00000004 */
#define SSP_ICTRL_RFTHIEN         SSP_ICTRL_RFTHIEN_Msk           /*!< Enable Receive FIFO Threshold Interrupt */
#define SSP_ICTRL_TFTHIEN_Pos     (3U)
#define SSP_ICTRL_TFTHIEN_Msk     (0x1U << SSP_ICTRL_TFTHIEN_Pos) /*!< 0x00000008 */
#define SSP_ICTRL_TFTHIEN         SSP_ICTRL_TFTHIEN_Msk           /*!< Enable Transmit FIFO Threshold Interrupt */
#define SSP_ICTRL_RFDMAEN_Pos     (4U)
#define SSP_ICTRL_RFDMAEN_Msk     (0x1U << SSP_ICTRL_RFDMAEN_Pos) /*!< 0x00000010 */
#define SSP_ICTRL_RFDMAEN         SSP_ICTRL_RFDMAEN_Msk           /*!< Enable Receive DMA Request */
#define SSP_ICTRL_TFDMAEN_Pos     (5U)
#define SSP_ICTRL_TFDMAEN_Msk     (0x1U << SSP_ICTRL_TFDMAEN_Pos) /*!< 0x00000020 */
#define SSP_ICTRL_TFDMAEN         SSP_ICTRL_TFDMAEN_Msk           /*!< Enable Receive DMA Request */
#define SSP_ICTRL_RFTHOD_Pos      (7U)
#define SSP_ICTRL_RFTHOD_Msk      (0x1FU << SSP_ICTRL_RFTHOD_Pos) /*!< 0x00000F80 */
#define SSP_ICTRL_RFTHOD          SSP_ICTRL_RFTHOD_Msk            /*!< Receive FIFO Threshold Mask */
#define SSP_ICTRL_TFTHOD_Pos      (12U)
#define SSP_ICTRL_TFTHOD_Msk      (0x1FU << SSP_ICTRL_TFTHOD_Pos) /*!< 0x0001F000 */
#define SSP_ICTRL_TFTHOD          SSP_ICTRL_TFTHOD_Msk            /*!< Transmit FIFO Threshold Mask */
#define SSP_ICTRL_RFTHOD_UNIT_Pos (17U)
#define SSP_ICTRL_RFTHOD_UNIT_Msk (0x1U << SSP_ICTRL_RFTHOD_UNIT_Pos) /*!< 0x00020000 */
#define SSP_ICTRL_RFTHOD_UNIT     SSP_ICTRL_RFTHOD_UNIT_Msk           /*!< RX FIFO threshold = RFTHOD + 1*/
#define SSP_ICTRL_TXCIEN_Pos      (18U)
#define SSP_ICTRL_TXCIEN_Msk      (0x1U << SSP_ICTRL_TXCIEN_Pos) /*!< 0x00040000 */
#define SSP_ICTRL_TXCIEN          SSP_ICTRL_TXCIEN_Msk           /*!< Transmit Data Complete Interrupt Enable*/

/* Bits definition for SSP RIS register */
#define SSP_RIS_RFORI_Pos (0U)
#define SSP_RIS_RFORI_Msk (0x1U << SSP_RIS_RFORI_Pos) /*!< 0x00000001 */
#define SSP_RIS_RFORI     SSP_RIS_RFORI_Msk           /*!< RX FIFO Overrun Interrupt*/
#define SSP_RIS_TFURI_Pos (1U)
#define SSP_RIS_TFURI_Msk (0x1U << SSP_RIS_TFURI_Pos) /*!< 0x00000002 */
#define SSP_RIS_TFURI     SSP_RIS_TFURI_Msk           /*!< TX FIFO Underrun Interrupt*/
#define SSP_RIS_RFTHI_Pos (2U)
#define SSP_RIS_RFTHI_Msk (0x1U << SSP_RIS_RFTHI_Pos) /*!< 0x00000004 */
#define SSP_RIS_RFTHI     SSP_RIS_RFTHI_Msk           /*!< RX FIFO Threshold interrupt*/
#define SSP_RIS_TFTHI_Pos (3U)
#define SSP_RIS_TFTHI_Msk (0x1U << SSP_RIS_TFTHI_Pos) /*!< 0x00000008 */
#define SSP_RIS_TFTHI     SSP_RIS_TFTHI_Msk           /*!< TX FIFO Threshold interrupt*/
#define SSP_RIS_TXCI_Pos  (5U)
#define SSP_RIS_TXCI_Msk  (0x1U << SSP_RIS_TXCI_Pos) /*!< 0x00000020 */
#define SSP_RIS_TXCI      SSP_RIS_TXCI_Msk           /*!< Transmit Data Complete Interrupt*/

/* Bits definition for SSP CTRL3 register */
#define SSP_CTRL3_PCL_Pos    (0U)
#define SSP_CTRL3_PCL_Msk    (0x3FFU << SSP_CTRL3_PCL_Pos) /*!< 0x000002FF */
#define SSP_CTRL3_PCL        SSP_CTRL3_PCL_Msk             /*!< Padding Cycle Length */
#define SSP_CTRL3_DPDLEN_Pos (12U)
#define SSP_CTRL3_DPDLEN_Msk (0x1U << SSP_CTRL3_DPDLEN_Pos) /*!< 0x00001000 */
#define SSP_CTRL3_DPDLEN     SSP_CTRL3_DPDLEN_Msk           /*!< Different Padding Data Length enable for I2S mode only.*/
#define SSP_CTRL3_DPDL_Pos   (16U)
#define SSP_CTRL3_DPDL_Msk   (0xFFU << SSP_CTRL3_DPDL_Pos) /*!< 0x00FF0000 */
#define SSP_CTRL3_DPDL       SSP_CTRL3_DPDL_Msk            /*!< Different Padding Data Length for I2S mode only.*/

/******************************************************************************/
/* TPM                                                                        */
/******************************************************************************/
/* Bits definition for CT16Bn TMRCTRL (n=0,1,2,5) register */
#define TPM_0125_TMRCTRL_CEN_Pos                  (0U)
#define TPM_0125_TMRCTRL_CEN_Msk                  (0x1U << TPM_0125_TMRCTRL_CEN_Pos) /*!< 0x00000001 */
#define TPM_0125_TMRCTRL_CEN                      TPM_0125_TMRCTRL_CEN_Msk           /*!< Counter enable */
#define TPM_0125_TMRCTRL_CRST_Pos                 (1U)
#define TPM_0125_TMRCTRL_CRST_Msk                 (0x1U << TPM_0125_TMRCTRL_CRST_Pos) /*!< 0x00000002 */
#define TPM_0125_TMRCTRL_CRST                     TPM_0125_TMRCTRL_CRST_Msk           /*!< Counter Reset */
#define TPM_0125_TMRCTRL_CM_Pos                   (4U)
#define TPM_0125_TMRCTRL_CM_Msk                   (0x7U << TPM_0125_TMRCTRL_CM_Pos) /*!< 0x00000070 */
#define TPM_0125_TMRCTRL_CM                       TPM_0125_TMRCTRL_CM_Msk           /*!< Counting mode selection */
#define TPM_0125_TMRCTRL_CM_EdgeUpCounting        (0x0U << TPM_0125_TMRCTRL_CM_Pos) /*!< Edge-aligned counting mode */
#define TPM_0125_TMRCTRL_CM_EdgeDownCounting      (0x1U << TPM_0125_TMRCTRL_CM_Pos) /*!< Edge-aligned counting mode */
#define TPM_0125_TMRCTRL_CM_DownCountingSetFlag   (0x2U << TPM_0125_TMRCTRL_CM_Pos) /*!< The match interrupt flag is set during the down-counting period */
#define TPM_0125_TMRCTRL_CM_UpCountingSetFlag     (0x4U << TPM_0125_TMRCTRL_CM_Pos) /*!< The match interrupt flag is set during the up-counting period */
#define TPM_0125_TMRCTRL_CM_UpDownCountingSetFlag (0x6U << TPM_0125_TMRCTRL_CM_Pos) /*!< The match interrupt flag is set during the up and down counting period */

/* Bits definition for CT16Bn TMRCTRL (n=3,4,6,7,8) register */
#define TPM_34678_TMRCTRL_CEN_Pos  (0U)
#define TPM_34678_TMRCTRL_CEN_Msk  (0x1U << TPM_34678_TMRCTRL_CEN_Pos) /*!< 0x00000001 */
#define TPM_34678_TMRCTRL_CEN      TPM_34678_TMRCTRL_CEN_Msk           /*!< Counter enable */
#define TPM_34678_TMRCTRL_CRST_Pos (1U)
#define TPM_34678_TMRCTRL_CRST_Msk (0x1U << TPM_34678_TMRCTRL_CRST_Pos) /*!< 0x00000002 */
#define TPM_34678_TMRCTRL_CRST     TPM_34678_TMRCTRL_CRST_Msk           /*!< Counter Reset */

/* Bits definition for CT16Bn TC (n=0,1,2,3,4,5,6,7,8) register */
#define TPM_TC_TC_Pos (0U)
#define TPM_TC_TC_Msk (0xFFFFU << TPM_TC_TC_Pos) /*!< 0x0000FFFF */
#define TPM_TC_TC     TPM_TC_TC_Msk              /*!< Timer Counter */

/* Bits definition for CT16Bn PRE (n=0,1,2,3,4,5,6,7,8) register */
#define TPM_PRE_PRE_Pos (0U)
#define TPM_PRE_PRE_Msk (0xFFU << TPM_PRE_PRE_Pos) /*!< 0x000000FF */
#define TPM_PRE_PRE     TPM_PRE_PRE_Msk            /*!< Prescaler value */

/* Bits definition for CT16Bn PC (n=0,1,2,3,4,5,6,7,8) register */
#define TPM_PC_PC_Pos (0U)
#define TPM_PC_PC_Msk (0xFFU << TPM_PC_PC_Pos) /*!< 0x000000FF */
#define TPM_PC_PC     TPM_PC_PC_Msk            /*!< Prescaler Counter */

/* Bits definition for CT16Bn CNTCTRL (n=0,1,2,4,5) register */
#define TPM_01245_CNTCTRL_CTM_Pos             (0U)
#define TPM_01245_CNTCTRL_CTM_Msk             (0x3U << TPM_01245_CNTCTRL_CTM_Pos) /*!< 0x00000003 */
#define TPM_01245_CNTCTRL_CTM                 TPM_01245_CNTCTRL_CTM_Msk           /*!< Counter/Timer Mode */
#define TPM_01245_CNTCTRL_CTM_EveryRisingEdge (0x0U << TPM_01245_CNTCTRL_CTM_Pos) /*!< Every rising edge of the input source */
#define TPM_01245_CNTCTRL_CTM_RisingEdgeCAP0  (0x1U << TPM_01245_CNTCTRL_CTM_Pos) /*!< TC is incremented on rising edges on the CAP0 input selected */
#define TPM_01245_CNTCTRL_CTM_FallingEdgeCAP0 (0x2U << TPM_01245_CNTCTRL_CTM_Pos) /*!< TC is incremented on falling edges on the CAP0 input selected */
#define TPM_01245_CNTCTRL_CTM_BothEdgeCAP0    (0x3U << TPM_01245_CNTCTRL_CTM_Pos) /*!< TC is incremented on both edges on the CAP0 input selected */

/* Bits definition for CT16Bn MCTRL (n=0,1,2,5) register */
#define TPM_0125_MCTRL_MR0IE_Pos   (0U)
#define TPM_0125_MCTRL_MR0IE_Msk   (0x1U << TPM_0125_MCTRL_MR0IE_Pos) /*!< 0x00000001 */
#define TPM_0125_MCTRL_MR0IE       TPM_0125_MCTRL_MR0IE_Msk           /*!< Enable generating an interrupt when MR0 matches TC */
#define TPM_0125_MCTRL_MR0RST_Pos  (1U)
#define TPM_0125_MCTRL_MR0RST_Msk  (0x1U << TPM_0125_MCTRL_MR0RST_Pos) /*!< 0x00000002 */
#define TPM_0125_MCTRL_MR0RST      TPM_0125_MCTRL_MR0RST_Msk           /*!< Enable reset TC when MR0 matches TC */
#define TPM_0125_MCTRL_MR0STOP_Pos (2U)
#define TPM_0125_MCTRL_MR0STOP_Msk (0x1U << TPM_0125_MCTRL_MR0STOP_Pos) /*!< 0x00000004 */
#define TPM_0125_MCTRL_MR0STOP     TPM_0125_MCTRL_MR0STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR0 matches TC */
#define TPM_0125_MCTRL_MR1IE_Pos   (3U)
#define TPM_0125_MCTRL_MR1IE_Msk   (0x1U << TPM_0125_MCTRL_MR1IE_Pos) /*!< 0x00000008 */
#define TPM_0125_MCTRL_MR1IE       TPM_0125_MCTRL_MR1IE_Msk           /*!< Enable generating an interrupt when MR1 matches TC */
#define TPM_0125_MCTRL_MR1RST_Pos  (4U)
#define TPM_0125_MCTRL_MR1RST_Msk  (0x1U << TPM_0125_MCTRL_MR1RST_Pos) /*!< 0x00000010 */
#define TPM_0125_MCTRL_MR1RST      TPM_0125_MCTRL_MR1RST_Msk           /*!< Enable reset TC when MR1 matches TC */
#define TPM_0125_MCTRL_MR1STOP_Pos (5U)
#define TPM_0125_MCTRL_MR1STOP_Msk (0x1U << TPM_0125_MCTRL_MR1STOP_Pos) /*!< 0x00000020 */
#define TPM_0125_MCTRL_MR1STOP     TPM_0125_MCTRL_MR1STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR1 matches TC */
#define TPM_0125_MCTRL_MR2IE_Pos   (6U)
#define TPM_0125_MCTRL_MR2IE_Msk   (0x1U << TPM_0125_MCTRL_MR2IE_Pos) /*!< 0x00000040 */
#define TPM_0125_MCTRL_MR2IE       TPM_0125_MCTRL_MR2IE_Msk           /*!< Enable generating an interrupt when MR2 matches TC */
#define TPM_0125_MCTRL_MR2RST_Pos  (7U)
#define TPM_0125_MCTRL_MR2RST_Msk  (0x1U << TPM_0125_MCTRL_MR2RST_Pos) /*!< 0x00000080 */
#define TPM_0125_MCTRL_MR2RST      TPM_0125_MCTRL_MR2RST_Msk           /*!< Enable reset TC when MR2 matches TC*/
#define TPM_0125_MCTRL_MR2STOP_Pos (8U)
#define TPM_0125_MCTRL_MR2STOP_Msk (0x1U << TPM_0125_MCTRL_MR2STOP_Pos) /*!< 0x00000100 */
#define TPM_0125_MCTRL_MR2STOP     TPM_0125_MCTRL_MR2STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR2 matches TC*/
#define TPM_0125_MCTRL_MR3IE_Pos   (9U)
#define TPM_0125_MCTRL_MR3IE_Msk   (0x1U << TPM_0125_MCTRL_MR3IE_Pos) /*!< 0x00000200 */
#define TPM_0125_MCTRL_MR3IE       TPM_0125_MCTRL_MR3IE_Msk           /*!< Enable generating an interrupt when MR3 matches TC */
#define TPM_0125_MCTRL_MR3RST_Pos  (10U)
#define TPM_0125_MCTRL_MR3RST_Msk  (0x1U << TPM_0125_MCTRL_MR3RST_Pos) /*!< 0x00000400 */
#define TPM_0125_MCTRL_MR3RST      TPM_0125_MCTRL_MR3RST_Msk           /*!< Enable reset TC when MR3 matches TC */
#define TPM_0125_MCTRL_MR3STOP_Pos (11U)
#define TPM_0125_MCTRL_MR3STOP_Msk (0x1U << TPM_0125_MCTRL_MR3STOP_Pos) /*!< 0x00000800 */
#define TPM_0125_MCTRL_MR3STOP     TPM_0125_MCTRL_MR3STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR3 matches TC */
#define TPM_0125_MCTRL_MR9IE_Pos   (29U)
#define TPM_0125_MCTRL_MR9IE_Msk   (0x1U << TPM_0125_MCTRL_MR9IE_Pos) /*!< 0x20000000 */
#define TPM_0125_MCTRL_MR9IE       TPM_0125_MCTRL_MR9IE_Msk           /*!< Generating an interrupt when MR9 matches TC */
#define TPM_0125_MCTRL_MR9RST_Pos  (30U)
#define TPM_0125_MCTRL_MR9RST_Msk  (0x1U << TPM_0125_MCTRL_MR9RST_Pos) /*!< 0x40000000 */
#define TPM_0125_MCTRL_MR9RST      TPM_0125_MCTRL_MR9RST_Msk           /*!< Enable reset TC when MR9 matches TC */
#define TPM_0125_MCTRL_MR9STOP_Pos (31U)
#define TPM_0125_MCTRL_MR9STOP_Msk (0x1U << TPM_0125_MCTRL_MR9STOP_Pos) /*!< 0x80000000 */
#define TPM_0125_MCTRL_MR9STOP     TPM_0125_MCTRL_MR9STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR9 matches TC */

/* Bits definition for CT16Bn MCTRL (n=3,4) register */
#define TPM_34_MCTRL_MR0IE_Pos   (0U)
#define TPM_34_MCTRL_MR0IE_Msk   (0x1U << TPM_34_MCTRL_MR0IE_Pos) /*!< 0x00000001 */
#define TPM_34_MCTRL_MR0IE       TPM_34_MCTRL_MR0IE_Msk           /*!< Enable generating an interrupt when MR0 matches TC */
#define TPM_34_MCTRL_MR0RST_Pos  (1U)
#define TPM_34_MCTRL_MR0RST_Msk  (0x1U << TPM_34_MCTRL_MR0RST_Pos) /*!< 0x00000002 */
#define TPM_34_MCTRL_MR0RST      TPM_34_MCTRL_MR0RST_Msk           /*!< Enable reset TC when MR0 matches TC */
#define TPM_34_MCTRL_MR0STOP_Pos (2U)
#define TPM_34_MCTRL_MR0STOP_Msk (0x1U << TPM_34_MCTRL_MR0STOP_Pos) /*!< 0x00000004 */
#define TPM_34_MCTRL_MR0STOP     TPM_34_MCTRL_MR0STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR0 matches TC */
#define TPM_34_MCTRL_MR1IE_Pos   (3U)
#define TPM_34_MCTRL_MR1IE_Msk   (0x1U << TPM_34_MCTRL_MR1IE_Pos) /*!< 0x00000008 */
#define TPM_34_MCTRL_MR1IE       TPM_34_MCTRL_MR1IE_Msk           /*!< Enable generating an interrupt when MR1 matches TC */
#define TPM_34_MCTRL_MR1RST_Pos  (4U)
#define TPM_34_MCTRL_MR1RST_Msk  (0x1U << TPM_34_MCTRL_MR1RST_Pos) /*!< 0x00000010 */
#define TPM_34_MCTRL_MR1RST      TPM_34_MCTRL_MR1RST_Msk           /*!< Enable reset TC when MR1 matches TC */
#define TPM_34_MCTRL_MR1STOP_Pos (5U)
#define TPM_34_MCTRL_MR1STOP_Msk (0x1U << TPM_34_MCTRL_MR1STOP_Pos) /*!< 0x00000020 */
#define TPM_34_MCTRL_MR1STOP     TPM_34_MCTRL_MR1STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR1 matches TC */
#define TPM_34_MCTRL_MR9IE_Pos   (29U)
#define TPM_34_MCTRL_MR9IE_Msk   (0x1U << TPM_34_MCTRL_MR9IE_Pos) /*!< 0x20000000 */
#define TPM_34_MCTRL_MR9IE       TPM_34_MCTRL_MR9IE_Msk           /*!< Generating an interrupt when MR9 matches TC */
#define TPM_34_MCTRL_MR9RST_Pos  (30U)
#define TPM_34_MCTRL_MR9RST_Msk  (0x1U << TPM_34_MCTRL_MR9RST_Pos) /*!< 0x40000000 */
#define TPM_34_MCTRL_MR9RST      TPM_34_MCTRL_MR9RST_Msk           /*!< Reset TC when MR9 matches TC */
#define TPM_34_MCTRL_MR9STOP_Pos (31U)
#define TPM_34_MCTRL_MR9STOP_Msk (0x1U << TPM_34_MCTRL_MR9STOP_Pos) /*!< 0x80000000 */
#define TPM_34_MCTRL_MR9STOP     TPM_34_MCTRL_MR9STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR9 matches TC */

/* Bits definition for CT16Bn MCTRL (n=6,7) register */
#define TPM_67_MCTRL_MR0IE_Pos   (0U)
#define TPM_67_MCTRL_MR0IE_Msk   (0x1U << TPM_67_MCTRL_MR0IE_Pos) /*!< 0x00000001 */
#define TPM_67_MCTRL_MR0IE       TPM_67_MCTRL_MR0IE_Msk           /*!< Generating an interrupt when MR0 matches TC */
#define TPM_67_MCTRL_MR0RST_Pos  (1U)
#define TPM_67_MCTRL_MR0RST_Msk  (0x1U << TPM_67_MCTRL_MR0RST_Pos) /*!< 0x00000002 */
#define TPM_67_MCTRL_MR0RST      TPM_67_MCTRL_MR0RST_Msk           /*!< Reset TC when MR0 matches TC */
#define TPM_67_MCTRL_MR0STOP_Pos (2U)
#define TPM_67_MCTRL_MR0STOP_Msk (0x1U << TPM_67_MCTRL_MR0STOP_Pos) /*!< 0x00000004 */
#define TPM_67_MCTRL_MR0STOP     TPM_67_MCTRL_MR0STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR0 matches TC */

/* Bits definition for CT16Bn MCTRL (n=8) register */
#define TPM_8_MCTRL_MR0IE_Pos   (0U)
#define TPM_8_MCTRL_MR0IE_Msk   (0x1U << TPM_8_MCTRL_MR0IE_Pos) /*!< 0x00000001 */
#define TPM_8_MCTRL_MR0IE       TPM_8_MCTRL_MR0IE_Msk           /*!< Enable generating an interrupt when MR0 matches TC */
#define TPM_8_MCTRL_MR0RST_Pos  (1U)
#define TPM_8_MCTRL_MR0RST_Msk  (0x1U << TPM_8_MCTRL_MR0RST_Pos) /*!< 0x00000002 */
#define TPM_8_MCTRL_MR0RST      TPM_8_MCTRL_MR0RST_Msk           /*!< Enable reset TC when MR0 matches TC */
#define TPM_8_MCTRL_MR0STOP_Pos (2U)
#define TPM_8_MCTRL_MR0STOP_Msk (0x1U << TPM_8_MCTRL_MR0STOP_Pos) /*!< 0x00000004 */
#define TPM_8_MCTRL_MR0STOP     TPM_8_MCTRL_MR0STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR0 matches TC */
#define TPM_8_MCTRL_MR1IE_Pos   (3U)
#define TPM_8_MCTRL_MR1IE_Msk   (0x1U << TPM_8_MCTRL_MR1IE_Pos) /*!< 0x00000008 */
#define TPM_8_MCTRL_MR1IE       TPM_8_MCTRL_MR1IE_Msk           /*!< Enable generating an interrupt when MR1 matches TC */
#define TPM_8_MCTRL_MR1RST_Pos  (4U)
#define TPM_8_MCTRL_MR1RST_Msk  (0x1U << TPM_8_MCTRL_MR1RST_Pos) /*!< 0x00000010 */
#define TPM_8_MCTRL_MR1RST      TPM_8_MCTRL_MR1RST_Msk           /*!< Enable reset TC when MR1 matches TC */
#define TPM_8_MCTRL_MR1STOP_Pos (5U)
#define TPM_8_MCTRL_MR1STOP_Msk (0x1U << TPM_8_MCTRL_MR1STOP_Pos) /*!< 0x00000020 */
#define TPM_8_MCTRL_MR1STOP     TPM_8_MCTRL_MR1STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR1 matches TC */
#define TPM_8_MCTRL_MR2IE_Pos   (6U)
#define TPM_8_MCTRL_MR2IE_Msk   (0x1U << TPM_8_MCTRL_MR2IE_Pos) /*!< 0x00000040 */
#define TPM_8_MCTRL_MR2IE       TPM_8_MCTRL_MR2IE_Msk           /*!< Enable generating an interrupt when MR2 matches TC */
#define TPM_8_MCTRL_MR2RST_Pos  (7U)
#define TPM_8_MCTRL_MR2RST_Msk  (0x1U << TPM_8_MCTRL_MR2RST_Pos) /*!< 0x00000080 */
#define TPM_8_MCTRL_MR2RST      TPM_8_MCTRL_MR2RST_Msk           /*!< Enable reset TC when MR2 matches TC*/
#define TPM_8_MCTRL_MR2STOP_Pos (8U)
#define TPM_8_MCTRL_MR2STOP_Msk (0x1U << TPM_8_MCTRL_MR2STOP_Pos) /*!< 0x00000100 */
#define TPM_8_MCTRL_MR2STOP     TPM_8_MCTRL_MR2STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR2 matches TC*/
#define TPM_8_MCTRL_MR3IE_Pos   (9U)
#define TPM_8_MCTRL_MR3IE_Msk   (0x1U << TPM_8_MCTRL_MR3IE_Pos) /*!< 0x00000200 */
#define TPM_8_MCTRL_MR3IE       TPM_8_MCTRL_MR3IE_Msk           /*!< Enable generating an interrupt when MR3 matches TC */
#define TPM_8_MCTRL_MR3RST_Pos  (10U)
#define TPM_8_MCTRL_MR3RST_Msk  (0x1U << TPM_8_MCTRL_MR3RST_Pos) /*!< 0x00000400 */
#define TPM_8_MCTRL_MR3RST      TPM_8_MCTRL_MR3RST_Msk           /*!< Enable reset TC when MR3 matches TC */
#define TPM_8_MCTRL_MR3STOP_Pos (11U)
#define TPM_8_MCTRL_MR3STOP_Msk (0x1U << TPM_8_MCTRL_MR3STOP_Pos) /*!< 0x00000800 */
#define TPM_8_MCTRL_MR3STOP     TPM_8_MCTRL_MR3STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR3 matches TC */
#define TPM_8_MCTRL_MR4IE_Pos   (12U)
#define TPM_8_MCTRL_MR4IE_Msk   (0x1U << TPM_8_MCTRL_MR4IE_Pos) /*!< 0x00001000 */
#define TPM_8_MCTRL_MR4IE       TPM_8_MCTRL_MR4IE_Msk           /*!< Enable generating an interrupt when MR3 matches TC */
#define TPM_8_MCTRL_MR4RST_Pos  (13U)
#define TPM_8_MCTRL_MR4RST_Msk  (0x1U << TPM_8_MCTRL_MR4RST_Pos) /*!< 0x00002000 */
#define TPM_8_MCTRL_MR4RST      TPM_8_MCTRL_MR4RST_Msk           /*!< Enable reset TC when MR3 matches TC */
#define TPM_8_MCTRL_MR4STOP_Pos (14U)
#define TPM_8_MCTRL_MR4STOP_Msk (0x1U << TPM_8_MCTRL_MR4STOP_Pos) /*!< 0x00004000 */
#define TPM_8_MCTRL_MR4STOP     TPM_8_MCTRL_MR4STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR3 matches TC */
#define TPM_8_MCTRL_MR5IE_Pos   (15U)
#define TPM_8_MCTRL_MR5IE_Msk   (0x1U << TPM_8_MCTRL_MR5IE_Pos) /*!< 0x00008000 */
#define TPM_8_MCTRL_MR5IE       TPM_8_MCTRL_MR5IE_Msk           /*!< Enable generating an interrupt when MR3 matches TC */
#define TPM_8_MCTRL_MR5RST_Pos  (16U)
#define TPM_8_MCTRL_MR5RST_Msk  (0x1U << TPM_8_MCTRL_MR5RST_Pos) /*!< 0x00010000 */
#define TPM_8_MCTRL_MR5RST      TPM_8_MCTRL_MR5RST_Msk           /*!< Enable reset TC when MR3 matches TC */
#define TPM_8_MCTRL_MR5STOP_Pos (17U)
#define TPM_8_MCTRL_MR5STOP_Msk (0x1U << TPM_8_MCTRL_MR5STOP_Pos) /*!< 0x00020000 */
#define TPM_8_MCTRL_MR5STOP     TPM_8_MCTRL_MR5STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR3 matches TC */
#define TPM_8_MCTRL_MR6IE_Pos   (18U)
#define TPM_8_MCTRL_MR6IE_Msk   (0x1U << TPM_8_MCTRL_MR6IE_Pos) /*!< 0x00040000 */
#define TPM_8_MCTRL_MR6IE       TPM_8_MCTRL_MR6IE_Msk           /*!< Enable generating an interrupt when MR3 matches TC */
#define TPM_8_MCTRL_MR6RST_Pos  (19U)
#define TPM_8_MCTRL_MR6RST_Msk  (0x1U << TPM_8_MCTRL_MR6RST_Pos) /*!< 0x00080000 */
#define TPM_8_MCTRL_MR6RST      TPM_8_MCTRL_MR6RST_Msk           /*!< Enable reset TC when MR3 matches TC */
#define TPM_8_MCTRL_MR6STOP_Pos (20U)
#define TPM_8_MCTRL_MR6STOP_Msk (0x1U << TPM_8_MCTRL_MR6STOP_Pos) /*!< 0x00100000 */
#define TPM_8_MCTRL_MR6STOP     TPM_8_MCTRL_MR6STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR3 matches TC */
#define TPM_8_MCTRL_MR7IE_Pos   (21U)
#define TPM_8_MCTRL_MR7IE_Msk   (0x1U << TPM_8_MCTRL_MR7IE_Pos) /*!< 0x00200000 */
#define TPM_8_MCTRL_MR7IE       TPM_8_MCTRL_MR7IE_Msk           /*!< Enable generating an interrupt when MR3 matches TC */
#define TPM_8_MCTRL_MR7RST_Pos  (22U)
#define TPM_8_MCTRL_MR7RST_Msk  (0x1U << TPM_8_MCTRL_MR7RST_Pos) /*!< 0x00400000 */
#define TPM_8_MCTRL_MR7RST      TPM_8_MCTRL_MR7RST_Msk           /*!< Enable reset TC when MR3 matches TC */
#define TPM_8_MCTRL_MR7STOP_Pos (23U)
#define TPM_8_MCTRL_MR7STOP_Msk (0x1U << TPM_8_MCTRL_MR7STOP_Pos) /*!< 0x00800000 */
#define TPM_8_MCTRL_MR7STOP     TPM_8_MCTRL_MR7STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR3 matches TC */
#define TPM_8_MCTRL_MR8IE_Pos   (24U)
#define TPM_8_MCTRL_MR8IE_Msk   (0x1U << TPM_8_MCTRL_MR8IE_Pos) /*!< 0x01000000 */
#define TPM_8_MCTRL_MR8IE       TPM_8_MCTRL_MR8IE_Msk           /*!< Enable generating an interrupt when MR3 matches TC */
#define TPM_8_MCTRL_MR8RST_Pos  (25U)
#define TPM_8_MCTRL_MR8RST_Msk  (0x1U << TPM_8_MCTRL_MR8RST_Pos) /*!< 0x02000000 */
#define TPM_8_MCTRL_MR8RST      TPM_8_MCTRL_MR8RST_Msk           /*!< Enable reset TC when MR3 matches TC */
#define TPM_8_MCTRL_MR8STOP_Pos (26U)
#define TPM_8_MCTRL_MR8STOP_Msk (0x1U << TPM_8_MCTRL_MR8STOP_Pos) /*!< 0x04000000 */
#define TPM_8_MCTRL_MR8STOP     TPM_8_MCTRL_MR8STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR3 matches TC */
#define TPM_8_MCTRL_MR9IE_Pos   (27U)
#define TPM_8_MCTRL_MR9IE_Msk   (0x1U << TPM_8_MCTRL_MR9IE_Pos) /*!< 0x08000000 */
#define TPM_8_MCTRL_MR9IE       TPM_8_MCTRL_MR9IE_Msk           /*!< Enable generating an interrupt when MR3 matches TC */
#define TPM_8_MCTRL_MR9RST_Pos  (28U)
#define TPM_8_MCTRL_MR9RST_Msk  (0x1U << TPM_8_MCTRL_MR9RST_Pos) /*!< 0x10000000 */
#define TPM_8_MCTRL_MR9RST      TPM_8_MCTRL_MR9RST_Msk           /*!< Enable reset TC when MR3 matches TC */
#define TPM_8_MCTRL_MR9STOP_Pos (29U)
#define TPM_8_MCTRL_MR9STOP_Msk (0x1U << TPM_8_MCTRL_MR9STOP_Pos) /*!< 0x20000000 */
#define TPM_8_MCTRL_MR9STOP     TPM_8_MCTRL_MR9STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR3 matches TC */

/* Bits definition for CT16Bn MCTRL2 (n=8) register */
#define TPM_8_MCTRL2_MR10IE_Pos     (0U)
#define TPM_8_MCTRL2_MR10IE_Msk     (0x1U << TPM_8_MCTRL2_MR10IE_Pos) /*!< 0x00000001 */
#define TPM_8_MCTRL2_MR10IE         TPM_8_MCTRL2_MR10IE_Msk           /*!< Enable generating an interrupt when MR0 matches TC */
#define TPM_8_MCTRL2_MR10RST_Pos    (1U)
#define TPM_8_MCTRL2_MR10RST_Msk    (0x1U << TPM_8_MCTRL2_MR10RST_Pos) /*!< 0x00000002 */
#define TPM_8_MCTRL2_MR10RST        TPM_8_MCTRL2_MR10RST_Msk           /*!< Enable reset TC when MR0 matches TC */
#define TPM_8_MCTRL2_MR10STOP_Pos   (2U)
#define TPM_8_MCTRL2_MR10STOP_Msk   (0x1U << TPM_8_MCTRL2_MR10STOP_Pos) /*!< 0x00000004 */
#define TPM_8_MCTRL2_MR10STOP       TPM_8_MCTRL2_MR10STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR0 matches TC */
#define TPM_8_MCTRL2_MR11IE_Pos     (3U)
#define TPM_8_MCTRL2_MR11IE_Msk     (0x1U << TPM_8_MCTRL2_MR11IE_Pos) /*!< 0x00000008 */
#define TPM_8_MCTRL2_MR11IE         TPM_8_MCTRL2_MR11IE_Msk           /*!< Enable generating an interrupt when MR1 matches TC */
#define TPM_8_MCTRL2_MR11RST_Pos    (4U)
#define TPM_8_MCTRL2_MR11RST_Msk    (0x1U << TPM_8_MCTRL2_MR11RST_Pos) /*!< 0x00000010 */
#define TPM_8_MCTRL2_MR11RST        TPM_8_MCTRL2_MR11RST_Msk           /*!< Enable reset TC when MR1 matches TC */
#define TPM_8_MCTRL2_MR11STOP_Pos   (5U)
#define TPM_8_MCTRL2_MR11STOP_Msk   (0x1U << TPM_8_MCTRL2_MR11STOP_Pos) /*!< 0x00000020 */
#define TPM_8_MCTRL2_MR11STOP       TPM_8_MCTRL2_MR11STOP_Msk           /*!< Stop TC and PC and clear CEN bit when MR1 matches TC */
#define TPM_8_MCTRL2_MRPERIODIE_Pos (29U)
#define TPM_8_MCTRL2_MRPERIODIE_Msk (0x1U << TPM_8_MCTRL2_MRPERIODIE_Pos) /*!< 0x20000000 */
#define TPM_8_MCTRL2_MRPERIODIE     TPM_8_MCTRL2_MRPERIODIE_Msk           /*!< Enable generating an interrupt based on CM[2:0] \
                                                                        when MR_PERIOD matches the value in the TC */
#define TPM_8_MCTRL2_MRPERIODRST_Pos  (30U)
#define TPM_8_MCTRL2_MRPERIODRST_Msk  (0x1U << TPM_8_MCTRL2_MRPERIODRST_Pos) /*!< 0x40000000 */
#define TPM_8_MCTRL2_MRPERIODRST      TPM_8_MCTRL2_MRPERIODRST_Msk           /*!< Enable reset TC when MR_PERIOD matches TC */
#define TPM_8_MCTRL2_MRPERIODSTOP_Pos (31U)
#define TPM_8_MCTRL2_MRPERIODSTOP_Msk (0x1U << TPM_8_MCTRL2_MRPERIODSTOP_Pos) /*!< 0x80000000 */
#define TPM_8_MCTRL2_MRPERIODSTOP     TPM_8_MCTRL2_MRPERIODSTOP_Msk           /*!< TC and PC will stop and CEN bit will be cleared if MR_PERIOD matches TC */

/* Bits definition for CT16Bn MR0 (n=0,1,2,3,4,5,6,7) register */
#define TPM_01234567_MR0_MR_Pos (0U)
#define TPM_01234567_MR0_MR_Msk (0xFFFFU << TPM_01234567_MR0_MR_Pos) /*!< 0x0000FFFF */
#define TPM_01234567_MR0_MR     TPM_01234567_MR0_MR_Msk              /*!< Timer counter match value */

/* Bits definition for CT16Bn MR1 (n=0,1,2,3,4,5) register */
#define TPM_012345_MR1_MR_Pos (0U)
#define TPM_012345_MR1_MR_Msk (0xFFFFU << TPM_012345_MR1_MR_Pos) /*!< 0x0000FFFF */
#define TPM_012345_MR1_MR     TPM_012345_MR1_MR_Msk              /*!< Timer counter match value */

/* Bits definition for CT16Bn MR2 (n=0,1,2,5) register */
#define TPM_0125_MR2_MR_Pos (0U)
#define TPM_0125_MR2_MR_Msk (0xFFFFU << TPM_0125_MR2_MR_Pos) /*!< 0x0000FFFF */
#define TPM_0125_MR2_MR     TPM_0125_MR2_MR_Msk              /*!< Timer counter match value */

/* Bits definition for CT16Bn MR3 (n=0,1,2,5) register */
#define TPM_0125_MR3_MR_Pos (0U)
#define TPM_0125_MR3_MR_Msk (0xFFFFU << TPM_0125_MR3_MR_Pos) /*!< 0x0000FFFF */
#define TPM_0125_MR3_MR     TPM_0125_MR3_MR_Msk              /*!< Timer counter match value */

/* Bits definition for CT16Bn MR9 (n=0,1,2,3,4,5) register */
#define TPM_012345_MR9_MR_Pos (0U)
#define TPM_012345_MR9_MR_Msk (0xFFFFU << TPM_012345_MR9_MR_Pos) /*!< 0x0000FFFF */
#define TPM_012345_MR9_MR     TPM_012345_MR9_MR_Msk              /*!< Timer counter match value */

/* Bits definition for CT16Bn MR0To11 (n=8) register */
#define TPM_8_MR0To11_MR_Pos (0U)
#define TPM_8_MR0To11_MR_Msk (0xFFFFU << TPM_8_MR0To11_MR_Pos) /*!< 0x0000FFFF */
#define TPM_8_MR0To11_MR     TPM_8_MR0To11_MR_Msk              /*!< Timer counter match value */

/* Bits definition for CT16Bn MR_PERIOD (n=8) register */
#define TPM_8_MRPERIOD_MR_Pos (0U)
#define TPM_8_MRPERIOD_MR_Msk (0xFFFFU << TPM_8_MRPERIOD_MR_Pos) /*!< 0x0000FFFF */
#define TPM_8_MRPERIOD_MR     TPM_8_MRPERIOD_MR_Msk              /*!< Timer counter match value */

/* Bits definition for CT16Bn CAPCTRL (n=0,1,2,4,5) register */
#define TPM_01245_CAPCTRL_CAP0RE_Pos (0U)
#define TPM_01245_CAPCTRL_CAP0RE_Msk (0x1U << TPM_01245_CAPCTRL_CAP0RE_Pos) /*!< 0x00000001 */
#define TPM_01245_CAPCTRL_CAP0RE     TPM_01245_CAPCTRL_CAP0RE_Msk           /*!< Capture/Reset on CT16Bn_CAP0 signal rising edge */

#define TPM_01245_CAPCTRL_CAP0FE_Pos (1U)
#define TPM_01245_CAPCTRL_CAP0FE_Msk (0x1U << TPM_01245_CAPCTRL_CAP0FE_Pos) /*!< 0x00000002 */
#define TPM_01245_CAPCTRL_CAP0FE     TPM_01245_CAPCTRL_CAP0FE_Msk           /*!< Capture/Reset on CT16Bn_CAP0 signal falling edge */

#define TPM_01245_CAPCTRL_CAP0IE_Pos (2U)
#define TPM_01245_CAPCTRL_CAP0IE_Msk (0x1U << TPM_01245_CAPCTRL_CAP0IE_Pos) /*!< 0x00000004 */
#define TPM_01245_CAPCTRL_CAP0IE     TPM_01245_CAPCTRL_CAP0IE_Msk           /*!< Interrupt on CT16Bn_CAP0 event */

#define TPM_01245_CAPCTRL_CAP0EN_Pos (3U)
#define TPM_01245_CAPCTRL_CAP0EN_Msk (0x1U << TPM_01245_CAPCTRL_CAP0EN_Pos) /*!< 0x00000008 */
#define TPM_01245_CAPCTRL_CAP0EN     TPM_01245_CAPCTRL_CAP0EN_Msk           /*!< CAP0 function enable */

/* Bits definition for CT16Bn CAP0 (n=0,1,2,4,5) register */
#define TPM_01245_CAP0_CAP0_Pos (0U)
#define TPM_01245_CAP0_CAP0_Msk (0xFFFFU << TPM_01245_CAP0_CAP0_Pos) /*!< 0x0000FFFF */
#define TPM_01245_CAP0_CAP0     TPM_01245_CAP0_CAP0_Msk              /*!< Timer counter match value */

/* Bits definition for CT16Bn EM (n=0,1,2,5) register */
#define TPM_0125_EM_EM0_Pos (0U)
#define TPM_0125_EM_EM0_Msk (0x1U << TPM_0125_EM_EM0_Pos) /*!< 0x00000001 */
#define TPM_0125_EM_EM0     TPM_0125_EM_EM0_Msk           /*!< When EMC0 != 00b and MR0 != TC, this bit will drive the state of CT16Bn_PWM0 output */

#define TPM_0125_EM_EM1_Pos (1U)
#define TPM_0125_EM_EM1_Msk (0x1U << TPM_0125_EM_EM1_Pos) /*!< 0x00000002 */
#define TPM_0125_EM_EM1     TPM_0125_EM_EM1_Msk           /*!< When EMC1 != 00b and MR1 != TC, this bit will drive the state of CT16Bn_PWM1 output */

#define TPM_0125_EM_EM2_Pos (2U)
#define TPM_0125_EM_EM2_Msk (0x1U << TPM_0125_EM_EM2_Pos) /*!< 0x00000004 */
#define TPM_0125_EM_EM2     TPM_0125_EM_EM2_Msk           /*!< When EMC2 != 00b and MR2 != TC, this bit will drive the state of CT16Bn_PWM2 output */

#define TPM_0125_EM_EM3_Pos (3U)
#define TPM_0125_EM_EM3_Msk (0x1U << TPM_0125_EM_EM3_Pos) /*!< 0x00000008 */
#define TPM_0125_EM_EM3     TPM_0125_EM_EM3_Msk           /*!< When EMC3 != 00b and MR3 != TC, this bit will drive the state of CT16Bn_PWM3 output */

#define TPM_0125_EM_EMC0_Pos           (4U)
#define TPM_0125_EM_EMC0_Msk           (0x3U << TPM_0125_EM_EMC0_Pos) /*!< 0x00000030 */
#define TPM_0125_EM_EMC0               TPM_0125_EM_EMC0_Msk           /*!< CT16Bn_PWM0 functionality when MR0=TC */
#define TPM_0125_EM_EMC0_None          (0x0U << TPM_0125_EM_EMC0_Pos) /*!< Do nothing */
#define TPM_0125_EM_EMC0_PWM0PinLow    (0x1U << TPM_0125_EM_EMC0_Pos) /*!< CT16Bn_PWM0 pin is LOW */
#define TPM_0125_EM_EMC0_PWM0PinHigh   (0x2U << TPM_0125_EM_EMC0_Pos) /*!< CT16Bn_PWM0 pin is HIGH */
#define TPM_0125_EM_EMC0_PWM0PinToggle (0x3U << TPM_0125_EM_EMC0_Pos) /*!< CT16Bn_PWM0 pin Toggle */

#define TPM_0125_EM_EMC1_Pos           (6U)
#define TPM_0125_EM_EMC1_Msk           (0x3U << TPM_0125_EM_EMC1_Pos) /*!< 0x000000C0 */
#define TPM_0125_EM_EMC1               TPM_0125_EM_EMC1_Msk           /*!< CT16Bn_PWM1 functionality when MR1=TC */
#define TPM_0125_EM_EMC1_None          (0x0U << TPM_0125_EM_EMC1_Pos) /*!< Do nothing */
#define TPM_0125_EM_EMC1_PWM1PinLow    (0x1U << TPM_0125_EM_EMC1_Pos) /*!< CT16Bn_PWM1 pin is LOW */
#define TPM_0125_EM_EMC1_PWM1PinHigh   (0x2U << TPM_0125_EM_EMC1_Pos) /*!< CT16Bn_PWM1 pin is HIGH */
#define TPM_0125_EM_EMC1_PWM1PinToggle (0x3U << TPM_0125_EM_EMC1_Pos) /*!< CT16Bn_PWM1 pin Toggle */

#define TPM_0125_EM_EMC2_Pos           (8U)
#define TPM_0125_EM_EMC2_Msk           (0x3U << TPM_0125_EM_EMC2_Pos) /*!< 0x00000300 */
#define TPM_0125_EM_EMC2               TPM_0125_EM_EMC2_Msk           /*!< CT16Bn_PWM2 functionality when MR2=TC */
#define TPM_0125_EM_EMC2_None          (0x0U << TPM_0125_EM_EMC2_Pos) /*!< Do nothing */
#define TPM_0125_EM_EMC2_PWM2PinLow    (0x1U << TPM_0125_EM_EMC2_Pos) /*!< CT16Bn_PWM2 pin is LOW */
#define TPM_0125_EM_EMC2_PWM2PinHigh   (0x2U << TPM_0125_EM_EMC2_Pos) /*!< CT16Bn_PWM2 pin is HIGH */
#define TPM_0125_EM_EMC2_PWM2PinToggle (0x3U << TPM_0125_EM_EMC2_Pos) /*!< CT16Bn_PWM2 pin Toggle */

#define TPM_0125_EM_EMC3_Pos           (10U)
#define TPM_0125_EM_EMC3_Msk           (0x3U << TPM_0125_EM_EMC3_Pos) /*!< 0x00000C00 */
#define TPM_0125_EM_EMC3               TPM_0125_EM_EMC3_Msk           /*!< CT16Bn_PWM3 functionality when MR3=TC */
#define TPM_0125_EM_EMC3_None          (0x0U << TPM_0125_EM_EMC3_Pos) /*!< Do nothing */
#define TPM_0125_EM_EMC3_PWM3PinLow    (0x1U << TPM_0125_EM_EMC3_Pos) /*!< CT16Bn_PWM3 pin is LOW */
#define TPM_0125_EM_EMC3_PWM3PinHigh   (0x2U << TPM_0125_EM_EMC3_Pos) /*!< CT16Bn_PWM3 pin is HIGH */
#define TPM_0125_EM_EMC3_PWM3PinToggle (0x3U << TPM_0125_EM_EMC3_Pos) /*!< CT16Bn_PWM3 pin Toggle */

/* Bits definition for CT16Bn EM (n=3,4) register */
#define TPM_34_EM_EM0_Pos (0U)
#define TPM_34_EM_EM0_Msk (0x1U << TPM_34_EM_EM0_Pos) /*!< 0x00000001 */
#define TPM_34_EM_EM0     TPM_34_EM_EM0_Msk           /*!< When EMC0 != 00b and MR0 != TC, this bit will drive the state of CT16Bn_PWM0 output */

#define TPM_34_EM_EM1_Pos (1U)
#define TPM_34_EM_EM1_Msk (0x1U << TPM_34_EM_EM1_Pos) /*!< 0x00000002 */
#define TPM_34_EM_EM1     TPM_34_EM_EM1_Msk           /*!< When EMC1 != 00b and MR1 != TC, this bit will drive the state of CT16Bn_PWM1 output */

#define TPM_34_EM_EMC0_Pos           (4U)
#define TPM_34_EM_EMC0_Msk           (0x3U << TPM_34_EM_EMC0_Pos) /*!< 0x00000030 */
#define TPM_34_EM_EMC0               TPM_34_EM_EMC0_Msk           /*!< CT16Bn_PWM0 functionality when MR0=TC */
#define TPM_34_EM_EMC0_None          (0x0U << TPM_34_EM_EMC0_Pos) /*!< Do nothing */
#define TPM_34_EM_EMC0_PWM0PinLow    (0x1U << TPM_34_EM_EMC0_Pos) /*!< CT16Bn_PWM0 pin is LOW */
#define TPM_34_EM_EMC0_PWM0PinHigh   (0x2U << TPM_34_EM_EMC0_Pos) /*!< CT16Bn_PWM0 pin is HIGH */
#define TPM_34_EM_EMC0_PWM0PinToggle (0x3U << TPM_34_EM_EMC0_Pos) /*!< CT16Bn_PWM0 pin Toggle */

#define TPM_34_EM_EMC1_Pos           (6U)
#define TPM_34_EM_EMC1_Msk           (0x3U << TPM_34_EM_EMC1_Pos) /*!< 0x000000C0 */
#define TPM_34_EM_EMC1               TPM_34_EM_EMC1_Msk           /*!< CT16Bn_PWM1 functionality when MR1=TC */
#define TPM_34_EM_EMC1_None          (0x0U << TPM_34_EM_EMC1_Pos) /*!< Do nothing */
#define TPM_34_EM_EMC1_PWM1PinLow    (0x1U << TPM_34_EM_EMC1_Pos) /*!< CT16Bn_PWM1 pin is LOW */
#define TPM_34_EM_EMC1_PWM1PinHigh   (0x2U << TPM_34_EM_EMC1_Pos) /*!< CT16Bn_PWM1 pin is HIGH */
#define TPM_34_EM_EMC1_PWM1PinToggle (0x3U << TPM_34_EM_EMC1_Pos) /*!< CT16Bn_PWM1 pin Toggle */

/* Bits definition for CT16Bn EM (n=8) register */
#define TPM_8_EM_EM0_Pos  (0U)
#define TPM_8_EM_EM0_Msk  (0x1U << TPM_8_EM_EM0_Pos) /*!< 0x00000001 */
#define TPM_8_EM_EM0      TPM_8_EM_EM0_Msk           /*!< When EMC0 != 00b and MR0 != TC, this bit will drive the state of CT16Bn_PWM0 output */
#define TPM_8_EM_EM1_Pos  (1U)
#define TPM_8_EM_EM1_Msk  (0x1U << TPM_8_EM_EM1_Pos) /*!< 0x00000002 */
#define TPM_8_EM_EM1      TPM_8_EM_EM1_Msk           /*!< When EMC1 != 00b and MR1 != TC, this bit will drive the state of CT16Bn_PWM1 output */
#define TPM_8_EM_EM2_Pos  (2U)
#define TPM_8_EM_EM2_Msk  (0x1U << TPM_8_EM_EM2_Pos) /*!< 0x00000004 */
#define TPM_8_EM_EM2      TPM_8_EM_EM2_Msk           /*!< When EMC2 != 00b and MR2 != TC, this bit will drive the state of CT16Bn_PWM2 output */
#define TPM_8_EM_EM3_Pos  (3U)
#define TPM_8_EM_EM3_Msk  (0x1U << TPM_8_EM_EM3_Pos) /*!< 0x00000008 */
#define TPM_8_EM_EM3      TPM_8_EM_EM3_Msk           /*!< When EMC3 != 00b and MR3 != TC, this bit will drive the state of CT16Bn_PWM3 output */
#define TPM_8_EM_EM4_Pos  (4U)
#define TPM_8_EM_EM4_Msk  (0x1U << TPM_8_EM_EM4_Pos) /*!< 0x00000010 */
#define TPM_8_EM_EM4      TPM_8_EM_EM4_Msk           /*!< When EMC4 != 00b and MR4 != TC, this bit will drive the state of CT16Bn_PWM4 output */
#define TPM_8_EM_EM5_Pos  (5U)
#define TPM_8_EM_EM5_Msk  (0x1U << TPM_8_EM_EM5_Pos) /*!< 0x00000020 */
#define TPM_8_EM_EM5      TPM_8_EM_EM5_Msk           /*!< When EMC5 != 00b and MR5 != TC, this bit will drive the state of CT16Bn_PWM5 output */
#define TPM_8_EM_EM6_Pos  (6U)
#define TPM_8_EM_EM6_Msk  (0x1U << TPM_8_EM_EM6_Pos) /*!< 0x00000040 */
#define TPM_8_EM_EM6      TPM_8_EM_EM6_Msk           /*!< When EMC6 != 00b and MR6 != TC, this bit will drive the state of CT16Bn_PWM6 output */
#define TPM_8_EM_EM7_Pos  (7U)
#define TPM_8_EM_EM7_Msk  (0x1U << TPM_8_EM_EM7_Pos) /*!< 0x00000080 */
#define TPM_8_EM_EM7      TPM_8_EM_EM7_Msk           /*!< When EMC7 != 00b and MR7 != TC, this bit will drive the state of CT16Bn_PWM7 output */
#define TPM_8_EM_EM8_Pos  (8U)
#define TPM_8_EM_EM8_Msk  (0x1U << TPM_8_EM_EM8_Pos) /*!< 0x00000100 */
#define TPM_8_EM_EM8      TPM_8_EM_EM8_Msk           /*!< When EMC8 != 00b and MR8 != TC, this bit will drive the state of CT16Bn_PWM8 output */
#define TPM_8_EM_EM9_Pos  (9U)
#define TPM_8_EM_EM9_Msk  (0x1U << TPM_8_EM_EM9_Pos) /*!< 0x00000200 */
#define TPM_8_EM_EM9      TPM_8_EM_EM9_Msk           /*!< When EMC9 != 00b and MR9 != TC, this bit will drive the state of CT16Bn_PWM9 output */
#define TPM_8_EM_EM10_Pos (10U)
#define TPM_8_EM_EM10_Msk (0x1U << TPM_8_EM_EM10_Pos) /*!< 0x00000400 */
#define TPM_8_EM_EM10     TPM_8_EM_EM10_Msk           /*!< When EMC10 != 00b and MR10 != TC, this bit will drive the state of CT16Bn_PWM10 output */
#define TPM_8_EM_EM11_Pos (11U)
#define TPM_8_EM_EM11_Msk (0x1U << TPM_8_EM_EM11_Pos) /*!< 0x00000800 */
#define TPM_8_EM_EM11     TPM_8_EM_EM11_Msk           /*!< When EMC11 != 00b and MR11 != TC, this bit will drive the state of CT16Bn_PWM11 output */

/* Bits definition for CT16Bn EMC (n=8) register */
#define TPM_8_EMC_EMC0_Pos           (0U)
#define TPM_8_EMC_EMC0_Msk           (0x3U << TPM_8_EMC_EMC0_Pos) /*!< 0x0000003 */
#define TPM_8_EMC_EMC0               TPM_8_EMC_EMC0_Msk           /*!< CT16Bn_PWM0 functionality when MR0=TC */
#define TPM_8_EMC_EMC0_None          (0x0U << TPM_8_EMC_EMC0_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC0_PWM0PinLow    (0x1U << TPM_8_EMC_EMC0_Pos) /*!< CT16Bn_PWM0 pin is LOW */
#define TPM_8_EMC_EMC0_PWM0PinHigh   (0x2U << TPM_8_EMC_EMC0_Pos) /*!< CT16Bn_PWM0 pin is HIGH */
#define TPM_8_EMC_EMC0_PWM0PinToggle (0x3U << TPM_8_EMC_EMC0_Pos) /*!< CT16Bn_PWM0 pin Toggle */

#define TPM_8_EMC_EMC1_Pos           (2U)
#define TPM_8_EMC_EMC1_Msk           (0x3U << TPM_8_EMC_EMC1_Pos) /*!< 0x0000000C */
#define TPM_8_EMC_EMC1               TPM_8_EMC_EMC1_Msk           /*!< CT16Bn_PWM1 functionality when MR1=TC */
#define TPM_8_EMC_EMC1_None          (0x0U << TPM_8_EMC_EMC1_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC1_PWM1PinLow    (0x1U << TPM_8_EMC_EMC1_Pos) /*!< CT16Bn_PWM1 pin is LOW */
#define TPM_8_EMC_EMC1_PWM1PinHigh   (0x2U << TPM_8_EMC_EMC1_Pos) /*!< CT16Bn_PWM1 pin is HIGH */
#define TPM_8_EMC_EMC1_PWM1PinToggle (0x3U << TPM_8_EMC_EMC1_Pos) /*!< CT16Bn_PWM1 pin Toggle */

#define TPM_8_EMC_EMC2_Pos           (4U)
#define TPM_8_EMC_EMC2_Msk           (0x3U << TPM_8_EMC_EMC2_Pos) /*!< 0x00000030 */
#define TPM_8_EMC_EMC2               TPM_8_EMC_EMC2_Msk           /*!< CT16Bn_PWM2 functionality when MR2=TC */
#define TPM_8_EMC_EMC2_None          (0x0U << TPM_8_EMC_EMC2_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC2_PWM2PinLow    (0x1U << TPM_8_EMC_EMC2_Pos) /*!< CT16Bn_PWM2 pin is LOW */
#define TPM_8_EMC_EMC2_PWM2PinHigh   (0x2U << TPM_8_EMC_EMC2_Pos) /*!< CT16Bn_PWM2 pin is HIGH */
#define TPM_8_EMC_EMC2_PWM2PinToggle (0x3U << TPM_8_EMC_EMC2_Pos) /*!< CT16Bn_PWM2 pin Toggle */

#define TPM_8_EMC_EMC3_Pos           (6U)
#define TPM_8_EMC_EMC3_Msk           (0x3U << TPM_8_EMC_EMC3_Pos) /*!< 0x000000C0 */
#define TPM_8_EMC_EMC3               TPM_8_EMC_EMC3_Msk           /*!< CT16Bn_PWM3 functionality when MR3=TC */
#define TPM_8_EMC_EMC3_None          (0x0U << TPM_8_EMC_EMC3_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC3_PWM3PinLow    (0x1U << TPM_8_EMC_EMC3_Pos) /*!< CT16Bn_PWM3 pin is LOW */
#define TPM_8_EMC_EMC3_PWM3PinHigh   (0x2U << TPM_8_EMC_EMC3_Pos) /*!< CT16Bn_PWM3 pin is HIGH */
#define TPM_8_EMC_EMC3_PWM3PinToggle (0x3U << TPM_8_EMC_EMC3_Pos) /*!< CT16Bn_PWM3 pin Toggle */

#define TPM_8_EMC_EMC4_Pos           (8U)
#define TPM_8_EMC_EMC4_Msk           (0x3U << TPM_8_EMC_EMC4_Pos) /*!< 0x00000300 */
#define TPM_8_EMC_EMC4               TPM_8_EMC_EMC4_Msk           /*!< CT16Bn_PWM4 functionality when MR4=TC */
#define TPM_8_EMC_EMC4_None          (0x0U << TPM_8_EMC_EMC4_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC4_PWM4PinLow    (0x1U << TPM_8_EMC_EMC4_Pos) /*!< CT16Bn_PWM4 pin is LOW */
#define TPM_8_EMC_EMC4_PWM4PinHigh   (0x2U << TPM_8_EMC_EMC4_Pos) /*!< CT16Bn_PWM4 pin is HIGH */
#define TPM_8_EMC_EMC4_PWM4PinToggle (0x3U << TPM_8_EMC_EMC4_Pos) /*!< CT16Bn_PWM4 pin Toggle */

#define TPM_8_EMC_EMC5_Pos           (10U)
#define TPM_8_EMC_EMC5_Msk           (0x3U << TPM_8_EMC_EMC5_Pos) /*!< 0x00000C00 */
#define TPM_8_EMC_EMC5               TPM_8_EMC_EMC5_Msk           /*!< CT16Bn_PWM5 functionality when MR5=TC */
#define TPM_8_EMC_EMC5_None          (0x0U << TPM_8_EMC_EMC5_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC5_PWM5PinLow    (0x1U << TPM_8_EMC_EMC5_Pos) /*!< CT16Bn_PWM5 pin is LOW */
#define TPM_8_EMC_EMC5_PWM5PinHigh   (0x2U << TPM_8_EMC_EMC5_Pos) /*!< CT16Bn_PWM5 pin is HIGH */
#define TPM_8_EMC_EMC5_PWM5PinToggle (0x3U << TPM_8_EMC_EMC5_Pos) /*!< CT16Bn_PWM5 pin Toggle */

#define TPM_8_EMC_EMC6_Pos           (12U)
#define TPM_8_EMC_EMC6_Msk           (0x3U << TPM_8_EMC_EMC6_Pos) /*!< 0x00003000 */
#define TPM_8_EMC_EMC6               TPM_8_EMC_EMC6_Msk           /*!< CT16Bn_PWM6 functionality when MR6=TC */
#define TPM_8_EMC_EMC6_None          (0x0U << TPM_8_EMC_EMC6_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC6_PWM6PinLow    (0x1U << TPM_8_EMC_EMC6_Pos) /*!< CT16Bn_PWM6 pin is LOW */
#define TPM_8_EMC_EMC6_PWM6PinHigh   (0x2U << TPM_8_EMC_EMC6_Pos) /*!< CT16Bn_PWM6 pin is HIGH */
#define TPM_8_EMC_EMC6_PWM6PinToggle (0x3U << TPM_8_EMC_EMC6_Pos) /*!< CT16Bn_PWM6 pin Toggle */

#define TPM_8_EMC_EMC7_Pos           (14U)
#define TPM_8_EMC_EMC7_Msk           (0x3U << TPM_8_EMC_EMC7_Pos) /*!< 0x0000C000 */
#define TPM_8_EMC_EMC7               TPM_8_EMC_EMC7_Msk           /*!< CT16Bn_PWM7 functionality when MR7=TC */
#define TPM_8_EMC_EMC7_None          (0x0U << TPM_8_EMC_EMC7_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC7_PWM7PinLow    (0x1U << TPM_8_EMC_EMC7_Pos) /*!< CT16Bn_PWM7 pin is LOW */
#define TPM_8_EMC_EMC7_PWM7PinHigh   (0x2U << TPM_8_EMC_EMC7_Pos) /*!< CT16Bn_PWM7 pin is HIGH */
#define TPM_8_EMC_EMC7_PWM7PinToggle (0x3U << TPM_8_EMC_EMC7_Pos) /*!< CT16Bn_PWM7 pin Toggle */

#define TPM_8_EMC_EMC8_Pos           (16U)
#define TPM_8_EMC_EMC8_Msk           (0x3U << TPM_8_EMC_EMC8_Pos) /*!< 0x00030000 */
#define TPM_8_EMC_EMC8               TPM_8_EMC_EMC8_Msk           /*!< CT16Bn_PWM8 functionality when MR8=TC */
#define TPM_8_EMC_EMC8_None          (0x0U << TPM_8_EMC_EMC8_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC8_PWM8PinLow    (0x1U << TPM_8_EMC_EMC8_Pos) /*!< CT16Bn_PWM8 pin is LOW */
#define TPM_8_EMC_EMC8_PWM8PinHigh   (0x2U << TPM_8_EMC_EMC8_Pos) /*!< CT16Bn_PWM8 pin is HIGH */
#define TPM_8_EMC_EMC8_PWM8PinToggle (0x3U << TPM_8_EMC_EMC8_Pos) /*!< CT16Bn_PWM8 pin Toggle */

#define TPM_8_EMC_EMC9_Pos           (18U)
#define TPM_8_EMC_EMC9_Msk           (0x3U << TPM_8_EMC_EMC9_Pos) /*!< 0x000C0000 */
#define TPM_8_EMC_EMC9               TPM_8_EMC_EMC9_Msk           /*!< CT16Bn_PWM9 functionality when MR9=TC */
#define TPM_8_EMC_EMC9_None          (0x0U << TPM_8_EMC_EMC9_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC9_PWM9PinLow    (0x1U << TPM_8_EMC_EMC9_Pos) /*!< CT16Bn_PWM9 pin is LOW */
#define TPM_8_EMC_EMC9_PWM9PinHigh   (0x2U << TPM_8_EMC_EMC9_Pos) /*!< CT16Bn_PWM9 pin is HIGH */
#define TPM_8_EMC_EMC9_PWM9PinToggle (0x3U << TPM_8_EMC_EMC9_Pos) /*!< CT16Bn_PWM9 pin Toggle */

#define TPM_8_EMC_EMC10_Pos            (20U)
#define TPM_8_EMC_EMC10_Msk            (0x3U << TPM_8_EMC_EMC10_Pos) /*!< 0x00300000 */
#define TPM_8_EMC_EMC10                TPM_8_EMC_EMC10_Msk           /*!< CT16Bn_PWM10 functionality when MR10=TC */
#define TPM_8_EMC_EMC10_None           (0x0U << TPM_8_EMC_EMC10_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC10_PWM10PinLow    (0x1U << TPM_8_EMC_EMC10_Pos) /*!< CT16Bn_PWM10 pin is LOW */
#define TPM_8_EMC_EMC10_PWM10PinHigh   (0x2U << TPM_8_EMC_EMC10_Pos) /*!< CT16Bn_PWM10 pin is HIGH */
#define TPM_8_EMC_EMC10_PWM10PinToggle (0x3U << TPM_8_EMC_EMC10_Pos) /*!< CT16Bn_PWM10 pin Toggle */

#define TPM_8_EMC_EMC11_Pos            (22U)
#define TPM_8_EMC_EMC11_Msk            (0x3U << TPM_8_EMC_EMC11_Pos) /*!< 0x00C00000 */
#define TPM_8_EMC_EMC11                TPM_8_EMC_EMC11_Msk           /*!< CT16Bn_PWM11 functionality when MR11=TC */
#define TPM_8_EMC_EMC11_None           (0x0U << TPM_8_EMC_EMC11_Pos) /*!< Do nothing */
#define TPM_8_EMC_EMC11_PWM11PinLow    (0x1U << TPM_8_EMC_EMC11_Pos) /*!< CT16Bn_PWM11 pin is LOW */
#define TPM_8_EMC_EMC11_PWM11PinHigh   (0x2U << TPM_8_EMC_EMC11_Pos) /*!< CT16Bn_PWM11 pin is HIGH */
#define TPM_8_EMC_EMC11_PWM11PinToggle (0x3U << TPM_8_EMC_EMC11_Pos) /*!< CT16Bn_PWM11 pin Toggle */

/* PWM mode note for PWMCTRL register */
/*!< PWM output mode reference, n=0~8, and n = 8 is different
    < PWM mode 1 :
       PWMx is 0 when TC<=MRx during Up-counting period
       PWMx is 0 when TC<=MRx during Down-counting period (n=8,PWM2 to PWM11 not support)
    < PWM mode 2 :
       PWMx is 1 when TC<MRx during Up-counting period
       PWMx is 1 when TC<=MRx during Down-counting period (n=8,PWM2 to PWM11 not support)
    < PWM mode 3 :
       PWMx is forced to 0
    < PWM mode 4 :
       PWMx is forced to 1
*/
/* Bits definition for CT16Bn PWMCTRL (n=0) register */
#define TPM_0_PWMCTRL_PWM0EN_Pos (0U)
#define TPM_0_PWMCTRL_PWM0EN_Msk (0x1U << TPM_0_PWMCTRL_PWM0EN_Pos) /*!< 0x00000001 */
#define TPM_0_PWMCTRL_PWM0EN     TPM_0_PWMCTRL_PWM0EN_Msk           /*!< PWM0 enable */
#define TPM_0_PWMCTRL_PWM1EN_Pos (1U)
#define TPM_0_PWMCTRL_PWM1EN_Msk (0x1U << TPM_0_PWMCTRL_PWM1EN_Pos) /*!< 0x00000002 */
#define TPM_0_PWMCTRL_PWM1EN     TPM_0_PWMCTRL_PWM1EN_Msk           /*!< PWM1 enable */
#define TPM_0_PWMCTRL_PWM2EN_Pos (2U)
#define TPM_0_PWMCTRL_PWM2EN_Msk (0x1U << TPM_0_PWMCTRL_PWM2EN_Pos) /*!< 0x00000004 */
#define TPM_0_PWMCTRL_PWM2EN     TPM_0_PWMCTRL_PWM2EN_Msk           /*!< PWM2 enable */
#define TPM_0_PWMCTRL_PWM3EN_Pos (3U)
#define TPM_0_PWMCTRL_PWM3EN_Msk (0x1U << TPM_0_PWMCTRL_PWM3EN_Pos) /*!< 0x00000008 */
#define TPM_0_PWMCTRL_PWM3EN     TPM_0_PWMCTRL_PWM3EN_Msk           /*!< PWM3 enable */

#define TPM_0_PWMCTRL_PWM0MODE_Pos    (4U)
#define TPM_0_PWMCTRL_PWM0MODE_Msk    (0x3U << TPM_0_PWMCTRL_PWM0MODE_Pos) /*!< 0x00000030 */
#define TPM_0_PWMCTRL_PWM0MODE        TPM_0_PWMCTRL_PWM0MODE_Msk           /*!< PWM0 output mode */
#define TPM_0_PWMCTRL_PWM0MODE_Mode_1 (0x0U << TPM_0_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 1 */
#define TPM_0_PWMCTRL_PWM0MODE_Mode_2 (0x1U << TPM_0_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 2 */
#define TPM_0_PWMCTRL_PWM0MODE_Mode_3 (0x2U << TPM_0_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 3 */
#define TPM_0_PWMCTRL_PWM0MODE_Mode_4 (0x3U << TPM_0_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 4 */

#define TPM_0_PWMCTRL_PWM1MODE_Pos    (6U)
#define TPM_0_PWMCTRL_PWM1MODE_Msk    (0x3U << TPM_0_PWMCTRL_PWM1MODE_Pos) /*!< 0x000000C0 */
#define TPM_0_PWMCTRL_PWM1MODE        TPM_0_PWMCTRL_PWM1MODE_Msk           /*!< PWM1 output mode */
#define TPM_0_PWMCTRL_PWM1MODE_Mode_1 (0x0U << TPM_0_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 1 */
#define TPM_0_PWMCTRL_PWM1MODE_Mode_2 (0x1U << TPM_0_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 2 */
#define TPM_0_PWMCTRL_PWM1MODE_Mode_3 (0x2U << TPM_0_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 3 */
#define TPM_0_PWMCTRL_PWM1MODE_Mode_4 (0x3U << TPM_0_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 4 */

#define TPM_0_PWMCTRL_PWM2MODE_Pos    (8U)
#define TPM_0_PWMCTRL_PWM2MODE_Msk    (0x3U << TPM_0_PWMCTRL_PWM2MODE_Pos) /*!< 0x00000300 */
#define TPM_0_PWMCTRL_PWM2MODE        TPM_0_PWMCTRL_PWM2MODE_Msk           /*!< PWM2 output mode */
#define TPM_0_PWMCTRL_PWM2MODE_Mode_1 (0x0U << TPM_0_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 1 */
#define TPM_0_PWMCTRL_PWM2MODE_Mode_2 (0x1U << TPM_0_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 2 */
#define TPM_0_PWMCTRL_PWM2MODE_Mode_3 (0x2U << TPM_0_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 3 */
#define TPM_0_PWMCTRL_PWM2MODE_Mode_4 (0x3U << TPM_0_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 4 */

#define TPM_0_PWMCTRL_PWM3MODE_Pos    (10U)
#define TPM_0_PWMCTRL_PWM3MODE_Msk    (0x3U << TPM_0_PWMCTRL_PWM3MODE_Pos) /*!< 0x00000C00 */
#define TPM_0_PWMCTRL_PWM3MODE        TPM_0_PWMCTRL_PWM3MODE_Msk           /*!< PWM3 output mode */
#define TPM_0_PWMCTRL_PWM3MODE_Mode_1 (0x0U << TPM_0_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 1 */
#define TPM_0_PWMCTRL_PWM3MODE_Mode_2 (0x1U << TPM_0_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 2 */
#define TPM_0_PWMCTRL_PWM3MODE_Mode_3 (0x2U << TPM_0_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 3 */
#define TPM_0_PWMCTRL_PWM3MODE_Mode_4 (0x3U << TPM_0_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 4 */

#define TPM_0_PWMCTRL_PWM0IOEN_Pos (20U)
#define TPM_0_PWMCTRL_PWM0IOEN_Msk (0x1U << TPM_0_PWMCTRL_PWM0IOEN_Pos) /*!< 0x00100000 */
#define TPM_0_PWMCTRL_PWM0IOEN     TPM_0_PWMCTRL_PWM0IOEN_Msk           /*!< CT16Bn_PWM0/GPIO selection */
#define TPM_0_PWMCTRL_PWM1IOEN_Pos (21U)
#define TPM_0_PWMCTRL_PWM1IOEN_Msk (0x1U << TPM_0_PWMCTRL_PWM1IOEN_Pos) /*!< 0x00200000 */
#define TPM_0_PWMCTRL_PWM1IOEN     TPM_0_PWMCTRL_PWM1IOEN_Msk           /*!< CT16Bn_PWM1/GPIO selection */
#define TPM_0_PWMCTRL_PWM2IOEN_Pos (22U)
#define TPM_0_PWMCTRL_PWM2IOEN_Msk (0x1U << TPM_0_PWMCTRL_PWM2IOEN_Pos) /*!< 0x00400000 */
#define TPM_0_PWMCTRL_PWM2IOEN     TPM_0_PWMCTRL_PWM2IOEN_Msk           /*!< CT16Bn_PWM2/GPIO selection */
#define TPM_0_PWMCTRL_PWM3IOEN_Pos (23U)
#define TPM_0_PWMCTRL_PWM3IOEN_Msk (0x1U << TPM_0_PWMCTRL_PWM3IOEN_Pos) /*!< 0x00400000 */
#define TPM_0_PWMCTRL_PWM3IOEN     TPM_0_PWMCTRL_PWM3IOEN_Msk           /*!< CT16Bn_PWM3/GPIO selection */

#define TPM_0_PWMCTRL_PWM0NIOEN_Pos          (24U)
#define TPM_0_PWMCTRL_PWM0NIOEN_Msk          (0x3U << TPM_0_PWMCTRL_PWM0NIOEN_Pos) /*!< 0x03000000 */
#define TPM_0_PWMCTRL_PWM0NIOEN              TPM_0_PWMCTRL_PWM0NIOEN_Msk           /*!< CT16Bn_PWM0N/GPIO selection bit */
#define TPM_0_PWMCTRL_PWM0NIOEN_GPIO         (0x0U << TPM_0_PWMCTRL_PWM0NIOEN_Pos) /*!< CT16Bn_PWM0N pin is act as GPIO */
#define TPM_0_PWMCTRL_PWM0NIOEN_Inverse_High (0x1U << TPM_0_PWMCTRL_PWM0NIOEN_Pos) /*!< PWM0N pin outputs the inverse signal with dead-band of PWM0,but same High during dead-band period */
#define TPM_0_PWMCTRL_PWM0NIOEN_Inverse_Low  (0x2U << TPM_0_PWMCTRL_PWM0NIOEN_Pos) /*!< PWM0N pin outputs the inverse signal with dead-band of PWM0,but same Low during dead-band period */
#define TPM_0_PWMCTRL_PWM0NIOEN_Same         (0x3U << TPM_0_PWMCTRL_PWM0NIOEN_Pos) /*!< PWM0N pin outputs the same signal with dead-band of PWM0 */

#define TPM_0_PWMCTRL_PWM1NIOEN_Pos          (26U)
#define TPM_0_PWMCTRL_PWM1NIOEN_Msk          (0x3U << TPM_0_PWMCTRL_PWM1NIOEN_Pos) /*!< 0x0C000000 */
#define TPM_0_PWMCTRL_PWM1NIOEN              TPM_0_PWMCTRL_PWM1NIOEN_Msk           /*!< CT16Bn_PWM0N/GPIO selection bit */
#define TPM_0_PWMCTRL_PWM1NIOEN_GPIO         (0x0U << TPM_0_PWMCTRL_PWM1NIOEN_Pos) /*!< CT16Bn_PWM1N pin is act as GPIO */
#define TPM_0_PWMCTRL_PWM1NIOEN_Inverse_High (0x1U << TPM_0_PWMCTRL_PWM1NIOEN_Pos) /*!< PWM1N pin outputs the inverse signal with dead-band of PWM1,but same High during dead-band period */
#define TPM_0_PWMCTRL_PWM1NIOEN_Inverse_Low  (0x2U << TPM_0_PWMCTRL_PWM1NIOEN_Pos) /*!< PWM1N pin outputs the inverse signal with dead-band of PWM1,but same Low during dead-band period */
#define TPM_0_PWMCTRL_PWM1NIOEN_Same         (0x3U << TPM_0_PWMCTRL_PWM1NIOEN_Pos) /*!< PWM1N pin outputs the same signal with dead-band of PWM1 */

#define TPM_0_PWMCTRL_PWM2NIOEN_Pos          (28U)
#define TPM_0_PWMCTRL_PWM2NIOEN_Msk          (0x3U << TPM_0_PWMCTRL_PWM2NIOEN_Pos) /*!< 0x30000000 */
#define TPM_0_PWMCTRL_PWM2NIOEN              TPM_0_PWMCTRL_PWM2NIOEN_Msk           /*!< CT16Bn_PWM0N/GPIO selection bit */
#define TPM_0_PWMCTRL_PWM2NIOEN_GPIO         (0x0U << TPM_0_PWMCTRL_PWM2NIOEN_Pos) /*!< CT16Bn_PWM2N pin is act as GPIO */
#define TPM_0_PWMCTRL_PWM2NIOEN_Inverse_High (0x1U << TPM_0_PWMCTRL_PWM2NIOEN_Pos) /*!< PWM2N pin outputs the inverse signal with dead-band of PWM2,but same High during dead-band period */
#define TPM_0_PWMCTRL_PWM2NIOEN_Inverse_Low  (0x2U << TPM_0_PWMCTRL_PWM2NIOEN_Pos) /*!< PWM2N pin outputs the inverse signal with dead-band of PWM2,but same Low during dead-band period */
#define TPM_0_PWMCTRL_PWM2NIOEN_Same         (0x3U << TPM_0_PWMCTRL_PWM2NIOEN_Pos) /*!< PWM2N pin outputs the same signal with dead-band of PWM2 */

#define TPM_0_PWMCTRL_PWM3NIOEN_Pos          (30U)
#define TPM_0_PWMCTRL_PWM3NIOEN_Msk          (0x3U << TPM_0_PWMCTRL_PWM3NIOEN_Pos) /*!< 0xC0000000 */
#define TPM_0_PWMCTRL_PWM3NIOEN              TPM_0_PWMCTRL_PWM3NIOEN_Msk           /*!< CT16Bn_PWM3N/GPIO selection bit */
#define TPM_0_PWMCTRL_PWM3NIOEN_GPIO         (0x0U << TPM_0_PWMCTRL_PWM3NIOEN_Pos) /*!< CT16Bn_PWM3N pin is act as GPIO */
#define TPM_0_PWMCTRL_PWM3NIOEN_Inverse_High (0x1U << TPM_0_PWMCTRL_PWM3NIOEN_Pos) /*!< PWM3N pin outputs the inverse signal with dead-band of PWM3,but same High during dead-band period */
#define TPM_0_PWMCTRL_PWM3NIOEN_Inverse_Low  (0x2U << TPM_0_PWMCTRL_PWM3NIOEN_Pos) /*!< PWM3N pin outputs the inverse signal with dead-band of PWM3,but same Low during dead-band period */
#define TPM_0_PWMCTRL_PWM3NIOEN_Same         (0x3U << TPM_0_PWMCTRL_PWM3NIOEN_Pos) /*!< PWM3N pin outputs the same signal with dead-band of PWM3 */

/* Bits definition for CT16Bn PWMCTRL (n=1,2,5) register */
#define TPM_125_PWMCTRL_PWM0EN_Pos (0U)
#define TPM_125_PWMCTRL_PWM0EN_Msk (0x1U << TPM_125_PWMCTRL_PWM0EN_Pos) /*!< 0x00000001 */
#define TPM_125_PWMCTRL_PWM0EN     TPM_125_PWMCTRL_PWM0EN_Msk           /*!< PWM0 enable */
#define TPM_125_PWMCTRL_PWM1EN_Pos (1U)
#define TPM_125_PWMCTRL_PWM1EN_Msk (0x1U << TPM_125_PWMCTRL_PWM1EN_Pos) /*!< 0x00000002 */
#define TPM_125_PWMCTRL_PWM1EN     TPM_125_PWMCTRL_PWM1EN_Msk           /*!< PWM1 enable */
#define TPM_125_PWMCTRL_PWM2EN_Pos (2U)
#define TPM_125_PWMCTRL_PWM2EN_Msk (0x1U << TPM_125_PWMCTRL_PWM2EN_Pos) /*!< 0x00000004 */
#define TPM_125_PWMCTRL_PWM2EN     TPM_125_PWMCTRL_PWM2EN_Msk           /*!< PWM2 enable */
#define TPM_125_PWMCTRL_PWM3EN_Pos (3U)
#define TPM_125_PWMCTRL_PWM3EN_Msk (0x1U << TPM_125_PWMCTRL_PWM3EN_Pos) /*!< 0x00000008 */
#define TPM_125_PWMCTRL_PWM3EN     TPM_125_PWMCTRL_PWM3EN_Msk           /*!< PWM3 enable */

#define TPM_125_PWMCTRL_PWM0MODE_Pos    (4U)
#define TPM_125_PWMCTRL_PWM0MODE_Msk    (0x3U << TPM_125_PWMCTRL_PWM0MODE_Pos) /*!< 0x00000030 */
#define TPM_125_PWMCTRL_PWM0MODE        TPM_125_PWMCTRL_PWM0MODE_Msk           /*!< PWM0 output mode */
#define TPM_125_PWMCTRL_PWM0MODE_Mode_1 (0x0U << TPM_125_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 1 */
#define TPM_125_PWMCTRL_PWM0MODE_Mode_2 (0x1U << TPM_125_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 2 */
#define TPM_125_PWMCTRL_PWM0MODE_Mode_3 (0x2U << TPM_125_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 3 */
#define TPM_125_PWMCTRL_PWM0MODE_Mode_4 (0x3U << TPM_125_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 4 */

#define TPM_125_PWMCTRL_PWM1MODE_Pos    (6U)
#define TPM_125_PWMCTRL_PWM1MODE_Msk    (0x3U << TPM_125_PWMCTRL_PWM1MODE_Pos) /*!< 0x000000C0 */
#define TPM_125_PWMCTRL_PWM1MODE        TPM_125_PWMCTRL_PWM1MODE_Msk           /*!< PWM1 output mode */
#define TPM_125_PWMCTRL_PWM1MODE_Mode_1 (0x0U << TPM_125_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 1 */
#define TPM_125_PWMCTRL_PWM1MODE_Mode_2 (0x1U << TPM_125_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 2 */
#define TPM_125_PWMCTRL_PWM1MODE_Mode_3 (0x2U << TPM_125_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 3 */
#define TPM_125_PWMCTRL_PWM1MODE_Mode_4 (0x3U << TPM_125_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 4 */

#define TPM_125_PWMCTRL_PWM2MODE_Pos    (8U)
#define TPM_125_PWMCTRL_PWM2MODE_Msk    (0x3U << TPM_125_PWMCTRL_PWM2MODE_Pos) /*!< 0x00000300 */
#define TPM_125_PWMCTRL_PWM2MODE        TPM_125_PWMCTRL_PWM2MODE_Msk           /*!< PWM2 output mode */
#define TPM_125_PWMCTRL_PWM2MODE_Mode_1 (0x0U << TPM_125_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 1 */
#define TPM_125_PWMCTRL_PWM2MODE_Mode_2 (0x1U << TPM_125_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 2 */
#define TPM_125_PWMCTRL_PWM2MODE_Mode_3 (0x2U << TPM_125_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 3 */
#define TPM_125_PWMCTRL_PWM2MODE_Mode_4 (0x3U << TPM_125_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 4 */

#define TPM_125_PWMCTRL_PWM3MODE_Pos    (10U)
#define TPM_125_PWMCTRL_PWM3MODE_Msk    (0x3U << TPM_125_PWMCTRL_PWM3MODE_Pos) /*!< 0x00000C00 */
#define TPM_125_PWMCTRL_PWM3MODE        TPM_125_PWMCTRL_PWM3MODE_Msk           /*!< PWM3 output mode */
#define TPM_125_PWMCTRL_PWM3MODE_Mode_1 (0x0U << TPM_125_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 1 */
#define TPM_125_PWMCTRL_PWM3MODE_Mode_2 (0x1U << TPM_125_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 2 */
#define TPM_125_PWMCTRL_PWM3MODE_Mode_3 (0x2U << TPM_125_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 3 */
#define TPM_125_PWMCTRL_PWM3MODE_Mode_4 (0x3U << TPM_125_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 4 */

#define TPM_125_PWMCTRL_PWM0IOEN_Pos (20U)
#define TPM_125_PWMCTRL_PWM0IOEN_Msk (0x1U << TPM_125_PWMCTRL_PWM0IOEN_Pos) /*!< 0x00100000 */
#define TPM_125_PWMCTRL_PWM0IOEN     TPM_125_PWMCTRL_PWM0IOEN_Msk           /*!< CT16Bn_PWM0/GPIO selection */
#define TPM_125_PWMCTRL_PWM1IOEN_Pos (21U)
#define TPM_125_PWMCTRL_PWM1IOEN_Msk (0x1U << TPM_125_PWMCTRL_PWM1IOEN_Pos) /*!< 0x00200000 */
#define TPM_125_PWMCTRL_PWM1IOEN     TPM_125_PWMCTRL_PWM1IOEN_Msk           /*!< CT16Bn_PWM1/GPIO selection */
#define TPM_125_PWMCTRL_PWM2IOEN_Pos (22U)
#define TPM_125_PWMCTRL_PWM2IOEN_Msk (0x1U << TPM_125_PWMCTRL_PWM2IOEN_Pos) /*!< 0x00400000 */
#define TPM_125_PWMCTRL_PWM2IOEN     TPM_125_PWMCTRL_PWM2IOEN_Msk           /*!< CT16Bn_PWM2/GPIO selection */
#define TPM_125_PWMCTRL_PWM3IOEN_Pos (23U)
#define TPM_125_PWMCTRL_PWM3IOEN_Msk (0x1U << TPM_125_PWMCTRL_PWM3IOEN_Pos) /*!< 0x00400000 */
#define TPM_125_PWMCTRL_PWM3IOEN     TPM_125_PWMCTRL_PWM3IOEN_Msk           /*!< CT16Bn_PWM3/GPIO selection */

/* Bits definition for CT16Bn PWMCTRL (n=3,4) register */
#define TPM_34_PWMCTRL_PWM0EN_Pos (0U)
#define TPM_34_PWMCTRL_PWM0EN_Msk (0x1U << TPM_34_PWMCTRL_PWM0EN_Pos) /*!< 0x00000001 */
#define TPM_34_PWMCTRL_PWM0EN     TPM_34_PWMCTRL_PWM0EN_Msk           /*!< PWM0 enable */
#define TPM_34_PWMCTRL_PWM1EN_Pos (1U)
#define TPM_34_PWMCTRL_PWM1EN_Msk (0x1U << TPM_34_PWMCTRL_PWM1EN_Pos) /*!< 0x00000002 */
#define TPM_34_PWMCTRL_PWM1EN     TPM_34_PWMCTRL_PWM1EN_Msk           /*!< PWM1 enable */

#define TPM_34_PWMCTRL_PWM0MODE_Pos    (4U)
#define TPM_34_PWMCTRL_PWM0MODE_Msk    (0x3U << TPM_34_PWMCTRL_PWM0MODE_Pos) /*!< 0x00000030 */
#define TPM_34_PWMCTRL_PWM0MODE        TPM_34_PWMCTRL_PWM0MODE_Msk           /*!< PWM0 output mode */
#define TPM_34_PWMCTRL_PWM0MODE_Mode_1 (0x0U << TPM_34_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 1 */
#define TPM_34_PWMCTRL_PWM0MODE_Mode_2 (0x1U << TPM_34_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 2 */
#define TPM_34_PWMCTRL_PWM0MODE_Mode_3 (0x2U << TPM_34_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 3 */
#define TPM_34_PWMCTRL_PWM0MODE_Mode_4 (0x3U << TPM_34_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 4 */

#define TPM_34_PWMCTRL_PWM1MODE_Pos    (6U)
#define TPM_34_PWMCTRL_PWM1MODE_Msk    (0x3U << TPM_34_PWMCTRL_PWM1MODE_Pos) /*!< 0x000000C0 */
#define TPM_34_PWMCTRL_PWM1MODE        TPM_34_PWMCTRL_PWM1MODE_Msk           /*!< PWM1 output mode */
#define TPM_34_PWMCTRL_PWM1MODE_Mode_1 (0x0U << TPM_34_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 1 */
#define TPM_34_PWMCTRL_PWM1MODE_Mode_2 (0x1U << TPM_34_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 2 */
#define TPM_34_PWMCTRL_PWM1MODE_Mode_3 (0x2U << TPM_34_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 3 */
#define TPM_34_PWMCTRL_PWM1MODE_Mode_4 (0x3U << TPM_34_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 4 */

#define TPM_34_PWMCTRL_PWM0IOEN_Pos (20U)
#define TPM_34_PWMCTRL_PWM0IOEN_Msk (0x1U << TPM_34_PWMCTRL_PWM0IOEN_Pos) /*!< 0x00100000 */
#define TPM_34_PWMCTRL_PWM0IOEN     TPM_34_PWMCTRL_PWM0IOEN_Msk           /*!< CT16Bn_PWM0/GPIO selection */
#define TPM_34_PWMCTRL_PWM1IOEN_Pos (21U)
#define TPM_34_PWMCTRL_PWM1IOEN_Msk (0x1U << TPM_34_PWMCTRL_PWM1IOEN_Pos) /*!< 0x00200000 */
#define TPM_34_PWMCTRL_PWM1IOEN     TPM_34_PWMCTRL_PWM1IOEN_Msk           /*!< CT16Bn_PWM1/GPIO selection */

#define TPM_34_PWMCTRL_PWM0NIOEN_Pos          (24U)
#define TPM_34_PWMCTRL_PWM0NIOEN_Msk          (0x3U << TPM_34_PWMCTRL_PWM0NIOEN_Pos) /*!< 0x00000C00 */
#define TPM_34_PWMCTRL_PWM0NIOEN              TPM_34_PWMCTRL_PWM0NIOEN_Msk           /*!< CT16Bn_PWM0N/GPIO selection bit */
#define TPM_34_PWMCTRL_PWM0NIOEN_GPIO         (0x0U << TPM_34_PWMCTRL_PWM0NIOEN_Pos) /*!< CT16Bn_PWM0N pin is act as GPIO */
#define TPM_34_PWMCTRL_PWM0NIOEN_Inverse_High (0x1U << TPM_34_PWMCTRL_PWM0NIOEN_Pos) /*!< PWM0N pin outputs the inverse signal with dead-band of PWM0,but same High during dead-band period */
#define TPM_34_PWMCTRL_PWM0NIOEN_Inverse_Low  (0x2U << TPM_34_PWMCTRL_PWM0NIOEN_Pos) /*!< PWM0N pin outputs the inverse signal with dead-band of PWM0,but same Low during dead-band period */
#define TPM_34_PWMCTRL_PWM0NIOEN_Same         (0x3U << TPM_34_PWMCTRL_PWM0NIOEN_Pos) /*!< PWM0N pin outputs the same signal with dead-band of PWM0 */

#define TPM_34_PWMCTRL_PWM1NIOEN_Pos          (26U)
#define TPM_34_PWMCTRL_PWM1NIOEN_Msk          (0x3U << TPM_34_PWMCTRL_PWM1NIOEN_Pos) /*!< 0x00000C00 */
#define TPM_34_PWMCTRL_PWM1NIOEN              TPM_34_PWMCTRL_PWM1NIOEN_Msk           /*!< CT16Bn_PWM0N/GPIO selection bit */
#define TPM_34_PWMCTRL_PWM1NIOEN_GPIO         (0x0U << TPM_34_PWMCTRL_PWM1NIOEN_Pos) /*!< CT16Bn_PWM1N pin is act as GPIO */
#define TPM_34_PWMCTRL_PWM1NIOEN_Inverse_High (0x1U << TPM_34_PWMCTRL_PWM1NIOEN_Pos) /*!< PWM1N pin outputs the inverse signal with dead-band of PWM0,but same High during dead-band period */
#define TPM_34_PWMCTRL_PWM1NIOEN_Inverse_Low  (0x2U << TPM_34_PWMCTRL_PWM1NIOEN_Pos) /*!< PWM1N pin outputs the inverse signal with dead-band of PWM0,but same Low during dead-band period */
#define TPM_34_PWMCTRL_PWM1NIOEN_Same         (0x3U << TPM_34_PWMCTRL_PWM1NIOEN_Pos) /*!< PWM1N pin outputs the same signal with dead-band of PWM0 */

/* Bits definition for CT16Bn PWMCTRL (n=8) register */
#define TPM_8_PWMCTRL_PWM0MODE_Pos    (0U)
#define TPM_8_PWMCTRL_PWM0MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM0MODE_Pos) /*!< 0x00000003 */
#define TPM_8_PWMCTRL_PWM0MODE        TPM_8_PWMCTRL_PWM0MODE_Msk           /*!< PWM0 output mode */
#define TPM_8_PWMCTRL_PWM0MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 1 */
#define TPM_8_PWMCTRL_PWM0MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 2 */
#define TPM_8_PWMCTRL_PWM0MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 3 */
#define TPM_8_PWMCTRL_PWM0MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM0MODE_Pos) /*!< PWM0 mode 4 */

#define TPM_8_PWMCTRL_PWM1MODE_Pos    (2U)
#define TPM_8_PWMCTRL_PWM1MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM1MODE_Pos) /*!< 0x0000000C */
#define TPM_8_PWMCTRL_PWM1MODE        TPM_8_PWMCTRL_PWM1MODE_Msk           /*!< PWM1 output mode */
#define TPM_8_PWMCTRL_PWM1MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 1 */
#define TPM_8_PWMCTRL_PWM1MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 2 */
#define TPM_8_PWMCTRL_PWM1MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 3 */
#define TPM_8_PWMCTRL_PWM1MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM1MODE_Pos) /*!< PWM1 mode 4 */

#define TPM_8_PWMCTRL_PWM2MODE_Pos    (4U)
#define TPM_8_PWMCTRL_PWM2MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM2MODE_Pos) /*!< 0x00000030 */
#define TPM_8_PWMCTRL_PWM2MODE        TPM_8_PWMCTRL_PWM2MODE_Msk           /*!< PWM2 output mode */
#define TPM_8_PWMCTRL_PWM2MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 1 */
#define TPM_8_PWMCTRL_PWM2MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 2 */
#define TPM_8_PWMCTRL_PWM2MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 3 */
#define TPM_8_PWMCTRL_PWM2MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM2MODE_Pos) /*!< PWM2 mode 4 */

#define TPM_8_PWMCTRL_PWM3MODE_Pos    (6U)
#define TPM_8_PWMCTRL_PWM3MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM3MODE_Pos) /*!< 0x000000C0 */
#define TPM_8_PWMCTRL_PWM3MODE        TPM_8_PWMCTRL_PWM3MODE_Msk           /*!< PWM3 output mode */
#define TPM_8_PWMCTRL_PWM3MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 1 */
#define TPM_8_PWMCTRL_PWM3MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 2 */
#define TPM_8_PWMCTRL_PWM3MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 3 */
#define TPM_8_PWMCTRL_PWM3MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM3MODE_Pos) /*!< PWM3 mode 4 */

#define TPM_8_PWMCTRL_PWM4MODE_Pos    (8U)
#define TPM_8_PWMCTRL_PWM4MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM4MODE_Pos) /*!< 0x00000300 */
#define TPM_8_PWMCTRL_PWM4MODE        TPM_8_PWMCTRL_PWM4MODE_Msk           /*!< PWM4 output mode */
#define TPM_8_PWMCTRL_PWM4MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM4MODE_Pos) /*!< PWM4 mode 1 */
#define TPM_8_PWMCTRL_PWM4MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM4MODE_Pos) /*!< PWM4 mode 2 */
#define TPM_8_PWMCTRL_PWM4MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM4MODE_Pos) /*!< PWM4 mode 3 */
#define TPM_8_PWMCTRL_PWM4MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM4MODE_Pos) /*!< PWM4 mode 4 */

#define TPM_8_PWMCTRL_PWM5MODE_Pos    (10U)
#define TPM_8_PWMCTRL_PWM5MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM5MODE_Pos) /*!< 0x00000C00 */
#define TPM_8_PWMCTRL_PWM5MODE        TPM_8_PWMCTRL_PWM5MODE_Msk           /*!< PWM5 output mode */
#define TPM_8_PWMCTRL_PWM5MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM5MODE_Pos) /*!< PWM5 mode 1 */
#define TPM_8_PWMCTRL_PWM5MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM5MODE_Pos) /*!< PWM5 mode 2 */
#define TPM_8_PWMCTRL_PWM5MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM5MODE_Pos) /*!< PWM5 mode 3 */
#define TPM_8_PWMCTRL_PWM5MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM5MODE_Pos) /*!< PWM5 mode 4 */

#define TPM_8_PWMCTRL_PWM6MODE_Pos    (12U)
#define TPM_8_PWMCTRL_PWM6MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM6MODE_Pos) /*!< 0x00003000 */
#define TPM_8_PWMCTRL_PWM6MODE        TPM_8_PWMCTRL_PWM6MODE_Msk           /*!< PWM6 output mode */
#define TPM_8_PWMCTRL_PWM6MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM6MODE_Pos) /*!< PWM6 mode 1 */
#define TPM_8_PWMCTRL_PWM6MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM6MODE_Pos) /*!< PWM6 mode 2 */
#define TPM_8_PWMCTRL_PWM6MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM6MODE_Pos) /*!< PWM6 mode 3 */
#define TPM_8_PWMCTRL_PWM6MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM6MODE_Pos) /*!< PWM6 mode 4 */

#define TPM_8_PWMCTRL_PWM7MODE_Pos    (14U)
#define TPM_8_PWMCTRL_PWM7MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM7MODE_Pos) /*!< 0x0000C000 */
#define TPM_8_PWMCTRL_PWM7MODE        TPM_8_PWMCTRL_PWM7MODE_Msk           /*!< PWM7 output mode */
#define TPM_8_PWMCTRL_PWM7MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM7MODE_Pos) /*!< PWM7 mode 1 */
#define TPM_8_PWMCTRL_PWM7MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM7MODE_Pos) /*!< PWM7 mode 2 */
#define TPM_8_PWMCTRL_PWM7MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM7MODE_Pos) /*!< PWM7 mode 3 */
#define TPM_8_PWMCTRL_PWM7MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM7MODE_Pos) /*!< PWM7 mode 4 */

#define TPM_8_PWMCTRL_PWM8MODE_Pos    (16U)
#define TPM_8_PWMCTRL_PWM8MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM8MODE_Pos) /*!< 0x00030000 */
#define TPM_8_PWMCTRL_PWM8MODE        TPM_8_PWMCTRL_PWM8MODE_Msk           /*!< PWM8 output mode */
#define TPM_8_PWMCTRL_PWM8MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM8MODE_Pos) /*!< PWM8 mode 1 */
#define TPM_8_PWMCTRL_PWM8MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM8MODE_Pos) /*!< PWM8 mode 2 */
#define TPM_8_PWMCTRL_PWM8MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM8MODE_Pos) /*!< PWM8 mode 3 */
#define TPM_8_PWMCTRL_PWM8MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM8MODE_Pos) /*!< PWM8 mode 4 */

#define TPM_8_PWMCTRL_PWM9MODE_Pos    (18U)
#define TPM_8_PWMCTRL_PWM9MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM9MODE_Pos) /*!< 0x000C0000 */
#define TPM_8_PWMCTRL_PWM9MODE        TPM_8_PWMCTRL_PWM9MODE_Msk           /*!< PWM9 output mode */
#define TPM_8_PWMCTRL_PWM9MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM9MODE_Pos) /*!< PWM9 mode 1 */
#define TPM_8_PWMCTRL_PWM9MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM9MODE_Pos) /*!< PWM9 mode 2 */
#define TPM_8_PWMCTRL_PWM9MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM9MODE_Pos) /*!< PWM9 mode 3 */
#define TPM_8_PWMCTRL_PWM9MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM9MODE_Pos) /*!< PWM9 mode 4 */

#define TPM_8_PWMCTRL_PWM10MODE_Pos    (20U)
#define TPM_8_PWMCTRL_PWM10MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM10MODE_Pos) /*!< 0x00300000 */
#define TPM_8_PWMCTRL_PWM10MODE        TPM_8_PWMCTRL_PWM10MODE_Msk           /*!< PWM10 output mode */
#define TPM_8_PWMCTRL_PWM10MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM10MODE_Pos) /*!< PWM10 mode 1 */
#define TPM_8_PWMCTRL_PWM10MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM10MODE_Pos) /*!< PWM10 mode 2 */
#define TPM_8_PWMCTRL_PWM10MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM10MODE_Pos) /*!< PWM10 mode 3 */
#define TPM_8_PWMCTRL_PWM10MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM10MODE_Pos) /*!< PWM10 mode 4 */

#define TPM_8_PWMCTRL_PWM11MODE_Pos    (22U)
#define TPM_8_PWMCTRL_PWM11MODE_Msk    (0x3U << TPM_8_PWMCTRL_PWM11MODE_Pos) /*!< 0x00C00000 */
#define TPM_8_PWMCTRL_PWM11MODE        TPM_8_PWMCTRL_PWM11MODE_Msk           /*!< PWM11 output mode */
#define TPM_8_PWMCTRL_PWM11MODE_Mode_1 (0x0U << TPM_8_PWMCTRL_PWM11MODE_Pos) /*!< PWM11 mode 1 */
#define TPM_8_PWMCTRL_PWM11MODE_Mode_2 (0x1U << TPM_8_PWMCTRL_PWM11MODE_Pos) /*!< PWM11 mode 2 */
#define TPM_8_PWMCTRL_PWM11MODE_Mode_3 (0x2U << TPM_8_PWMCTRL_PWM11MODE_Pos) /*!< PWM11 mode 3 */
#define TPM_8_PWMCTRL_PWM11MODE_Mode_4 (0x3U << TPM_8_PWMCTRL_PWM11MODE_Pos) /*!< PWM11 mode 4 */

/* Bits definition for CT16Bn PWMENB (n=8) register */
#define TPM_8_PWMENB_PWM0EN_Pos  (0U)
#define TPM_8_PWMENB_PWM0EN_Msk  (0x1U << TPM_8_PWMENB_PWM0EN_Pos) /*!< 0x00000001 */
#define TPM_8_PWMENB_PWM0EN      TPM_8_PWMENB_PWM0EN_Msk           /*!< PWM0 enable */
#define TPM_8_PWMENB_PWM1EN_Pos  (1U)
#define TPM_8_PWMENB_PWM1EN_Msk  (0x1U << TPM_8_PWMENB_PWM1EN_Pos) /*!< 0x00000002 */
#define TPM_8_PWMENB_PWM1EN      TPM_8_PWMENB_PWM1EN_Msk           /*!< PWM1 enable */
#define TPM_8_PWMENB_PWM2EN_Pos  (2U)
#define TPM_8_PWMENB_PWM2EN_Msk  (0x1U << TPM_8_PWMENB_PWM2EN_Pos) /*!< 0x00000004 */
#define TPM_8_PWMENB_PWM2EN      TPM_8_PWMENB_PWM2EN_Msk           /*!< PWM2 enable */
#define TPM_8_PWMENB_PWM3EN_Pos  (3U)
#define TPM_8_PWMENB_PWM3EN_Msk  (0x1U << TPM_8_PWMENB_PWM3EN_Pos) /*!< 0x00000008 */
#define TPM_8_PWMENB_PWM3EN      TPM_8_PWMENB_PWM3EN_Msk           /*!< PWM3 enable */
#define TPM_8_PWMENB_PWM4EN_Pos  (4U)
#define TPM_8_PWMENB_PWM4EN_Msk  (0x1U << TPM_8_PWMENB_PWM4EN_Pos) /*!< 0x00000010 */
#define TPM_8_PWMENB_PWM4EN      TPM_8_PWMENB_PWM4EN_Msk           /*!< PWM4 enable */
#define TPM_8_PWMENB_PWM5EN_Pos  (5U)
#define TPM_8_PWMENB_PWM5EN_Msk  (0x1U << TPM_8_PWMENB_PWM5EN_Pos) /*!< 0x00000020 */
#define TPM_8_PWMENB_PWM5EN      TPM_8_PWMENB_PWM5EN_Msk           /*!< PWM5 enable */
#define TPM_8_PWMENB_PWM6EN_Pos  (6U)
#define TPM_8_PWMENB_PWM6EN_Msk  (0x1U << TPM_8_PWMENB_PWM6EN_Pos) /*!< 0x00000040 */
#define TPM_8_PWMENB_PWM6EN      TPM_8_PWMENB_PWM6EN_Msk           /*!< PWM6 enable */
#define TPM_8_PWMENB_PWM7EN_Pos  (7U)
#define TPM_8_PWMENB_PWM7EN_Msk  (0x1U << TPM_8_PWMENB_PWM7EN_Pos) /*!< 0x00000080 */
#define TPM_8_PWMENB_PWM7EN      TPM_8_PWMENB_PWM7EN_Msk           /*!< PWM7 enable */
#define TPM_8_PWMENB_PWM8EN_Pos  (8U)
#define TPM_8_PWMENB_PWM8EN_Msk  (0x1U << TPM_8_PWMENB_PWM8EN_Pos) /*!< 0x00000100 */
#define TPM_8_PWMENB_PWM8EN      TPM_8_PWMENB_PWM8EN_Msk           /*!< PWM8 enable */
#define TPM_8_PWMENB_PWM9EN_Pos  (9U)
#define TPM_8_PWMENB_PWM9EN_Msk  (0x1U << TPM_8_PWMENB_PWM9EN_Pos) /*!< 0x00000200 */
#define TPM_8_PWMENB_PWM9EN      TPM_8_PWMENB_PWM9EN_Msk           /*!< PWM9 enable */
#define TPM_8_PWMENB_PWM10EN_Pos (10U)
#define TPM_8_PWMENB_PWM10EN_Msk (0x1U << TPM_8_PWMENB_PWM10EN_Pos) /*!< 0x00000400 */
#define TPM_8_PWMENB_PWM10EN     TPM_8_PWMENB_PWM10EN_Msk           /*!< PWM10 enable */
#define TPM_8_PWMENB_PWM11EN_Pos (11U)
#define TPM_8_PWMENB_PWM11EN_Msk (0x1U << TPM_8_PWMENB_PWM11EN_Pos) /*!< 0x00000800 */
#define TPM_8_PWMENB_PWM11EN     TPM_8_PWMENB_PWM11EN_Msk           /*!< PWM11 enable */

/* Bits definition for CT16Bn PWMIOENB (n=8) register */
#define TPM_8_PWMIOENB_PWM0IOEN_Pos  (0U)
#define TPM_8_PWMIOENB_PWM0IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM0IOEN_Pos) /*!< 0x00000001 */
#define TPM_8_PWMIOENB_PWM0IOEN      TPM_8_PWMIOENB_PWM0IOEN_Msk           /*!< CT16Bn_PWM0/GPIO selection */
#define TPM_8_PWMIOENB_PWM1IOEN_Pos  (1U)
#define TPM_8_PWMIOENB_PWM1IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM1IOEN_Pos) /*!< 0x00000002 */
#define TPM_8_PWMIOENB_PWM1IOEN      TPM_8_PWMIOENB_PWM1IOEN_Msk           /*!< CT16Bn_PWM1/GPIO selection */
#define TPM_8_PWMIOENB_PWM2IOEN_Pos  (2U)
#define TPM_8_PWMIOENB_PWM2IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM2IOEN_Pos) /*!< 0x00000004 */
#define TPM_8_PWMIOENB_PWM2IOEN      TPM_8_PWMIOENB_PWM2IOEN_Msk           /*!< CT16Bn_PWM2/GPIO selection */
#define TPM_8_PWMIOENB_PWM3IOEN_Pos  (3U)
#define TPM_8_PWMIOENB_PWM3IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM3IOEN_Pos) /*!< 0x00000008 */
#define TPM_8_PWMIOENB_PWM3IOEN      TPM_8_PWMIOENB_PWM3IOEN_Msk           /*!< CT16Bn_PWM3/GPIO selection */
#define TPM_8_PWMIOENB_PWM4IOEN_Pos  (4U)
#define TPM_8_PWMIOENB_PWM4IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM4IOEN_Pos) /*!< 0x00000010 */
#define TPM_8_PWMIOENB_PWM4IOEN      TPM_8_PWMIOENB_PWM4IOEN_Msk           /*!< CT16Bn_PWM4/GPIO selection */
#define TPM_8_PWMIOENB_PWM5IOEN_Pos  (5U)
#define TPM_8_PWMIOENB_PWM5IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM5IOEN_Pos) /*!< 0x00000020 */
#define TPM_8_PWMIOENB_PWM5IOEN      TPM_8_PWMIOENB_PWM5IOEN_Msk           /*!< CT16Bn_PWM5/GPIO selection */
#define TPM_8_PWMIOENB_PWM6IOEN_Pos  (6U)
#define TPM_8_PWMIOENB_PWM6IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM6IOEN_Pos) /*!< 0x00000040 */
#define TPM_8_PWMIOENB_PWM6IOEN      TPM_8_PWMIOENB_PWM6IOEN_Msk           /*!< CT16Bn_PWM6/GPIO selection */
#define TPM_8_PWMIOENB_PWM7IOEN_Pos  (7U)
#define TPM_8_PWMIOENB_PWM7IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM7IOEN_Pos) /*!< 0x00000080 */
#define TPM_8_PWMIOENB_PWM7IOEN      TPM_8_PWMIOENB_PWM7IOEN_Msk           /*!< CT16Bn_PWM7/GPIO selection */
#define TPM_8_PWMIOENB_PWM8IOEN_Pos  (8U)
#define TPM_8_PWMIOENB_PWM8IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM8IOEN_Pos) /*!< 0x00000100 */
#define TPM_8_PWMIOENB_PWM8IOEN      TPM_8_PWMIOENB_PWM8IOEN_Msk           /*!< CT16Bn_PWM8/GPIO selection */
#define TPM_8_PWMIOENB_PWM9IOEN_Pos  (9U)
#define TPM_8_PWMIOENB_PWM9IOEN_Msk  (0x1U << TPM_8_PWMIOENB_PWM9IOEN_Pos) /*!< 0x00000200 */
#define TPM_8_PWMIOENB_PWM9IOEN      TPM_8_PWMIOENB_PWM9IOEN_Msk           /*!< CT16Bn_PWM9/GPIO selection */
#define TPM_8_PWMIOENB_PWM10IOEN_Pos (10U)
#define TPM_8_PWMIOENB_PWM10IOEN_Msk (0x1U << TPM_8_PWMIOENB_PWM10IOEN_Pos) /*!< 0x00000400 */
#define TPM_8_PWMIOENB_PWM10IOEN     TPM_8_PWMIOENB_PWM10IOEN_Msk           /*!< CT16Bn_PWM10/GPIO selection */
#define TPM_8_PWMIOENB_PWM11IOEN_Pos (11U)
#define TPM_8_PWMIOENB_PWM11IOEN_Msk (0x1U << TPM_8_PWMIOENB_PWM11IOEN_Pos) /*!< 0x00000800 */
#define TPM_8_PWMIOENB_PWM11IOEN     TPM_8_PWMIOENB_PWM11IOEN_Msk           /*!< CT16Bn_PWM11/GPIO selection */

/* Bits definition for CT16Bn RIS (n=0) register */
#define TPM_0_RIS_MR0IF_Pos  (0U)
#define TPM_0_RIS_MR0IF_Msk  (0x1U << TPM_0_RIS_MR0IF_Pos) /*!< 0x00000001 */
#define TPM_0_RIS_MR0IF      TPM_0_RIS_MR0IF_Msk           /*!< Match channel 0 interrupt flag */
#define TPM_0_RIS_MR1IF_Pos  (1U)
#define TPM_0_RIS_MR1IF_Msk  (0x1U << TPM_0_RIS_MR1IF_Pos) /*!< 0x00000002 */
#define TPM_0_RIS_MR1IF      TPM_0_RIS_MR1IF_Msk           /*!< Match channel 1 interrupt flag */
#define TPM_0_RIS_MR2IF_Pos  (2U)
#define TPM_0_RIS_MR2IF_Msk  (0x1U << TPM_0_RIS_MR2IF_Pos) /*!< 0x00000004 */
#define TPM_0_RIS_MR2IF      TPM_0_RIS_MR2IF_Msk           /*!< Match channel 2 interrupt flag */
#define TPM_0_RIS_MR3IF_Pos  (3U)
#define TPM_0_RIS_MR3IF_Msk  (0x1U << TPM_0_RIS_MR3IF_Pos) /*!< 0x00000008 */
#define TPM_0_RIS_MR3IF      TPM_0_RIS_MR3IF_Msk           /*!< Match channel 3 interrupt flag */
#define TPM_0_RIS_CAP0IF_Pos (4U)
#define TPM_0_RIS_CAP0IF_Msk (0x1U << TPM_0_RIS_CAP0IF_Pos) /*!< 0x00000010 */
#define TPM_0_RIS_CAP0IF     TPM_0_RIS_CAP0IF_Msk           /*!< Capture channel 0 interrupt flag */
#define TPM_0_RIS_MR9IF_Pos  (5U)
#define TPM_0_RIS_MR9IF_Msk  (0x1U << TPM_0_RIS_MR9IF_Pos) /*!< 0x00000020 */
#define TPM_0_RIS_MR9IF      TPM_0_RIS_MR9IF_Msk           /*!< Match channel 9 interrupt flag */
#define TPM_0_RIS_BRKIF_Pos  (31U)
#define TPM_0_RIS_BRKIF_Msk  (0x1U << TPM_0_RIS_BRKIF_Pos) /*!< 0x80000000 */
#define TPM_0_RIS_BRKIF      TPM_0_RIS_BRKIF_Msk           /*!< Break Interrupt flag */

/* Bits definition for CT16Bn RIS (n=1,2,5) register */
#define TPM_125_RIS_MR0IF_Pos  (0U)
#define TPM_125_RIS_MR0IF_Msk  (0x1U << TPM_125_RIS_MR0IF_Pos) /*!< 0x00000001 */
#define TPM_125_RIS_MR0IF      TPM_125_RIS_MR0IF_Msk           /*!< Match channel 0 interrupt flag */
#define TPM_125_RIS_MR1IF_Pos  (1U)
#define TPM_125_RIS_MR1IF_Msk  (0x1U << TPM_125_RIS_MR1IF_Pos) /*!< 0x00000002 */
#define TPM_125_RIS_MR1IF      TPM_125_RIS_MR1IF_Msk           /*!< Match channel 1 interrupt flag */
#define TPM_125_RIS_MR2IF_Pos  (2U)
#define TPM_125_RIS_MR2IF_Msk  (0x1U << TPM_125_RIS_MR2IF_Pos) /*!< 0x00000004 */
#define TPM_125_RIS_MR2IF      TPM_125_RIS_MR2IF_Msk           /*!< Match channel 2 interrupt flag */
#define TPM_125_RIS_MR3IF_Pos  (3U)
#define TPM_125_RIS_MR3IF_Msk  (0x1U << TPM_125_RIS_MR3IF_Pos) /*!< 0x00000008 */
#define TPM_125_RIS_MR3IF      TPM_125_RIS_MR3IF_Msk           /*!< Match channel 3 interrupt flag */
#define TPM_125_RIS_CAP0IF_Pos (4U)
#define TPM_125_RIS_CAP0IF_Msk (0x1U << TPM_125_RIS_CAP0IF_Pos) /*!< 0x00000010 */
#define TPM_125_RIS_CAP0IF     TPM_125_RIS_CAP0IF_Msk           /*!< Capture channel 0 interrupt flag */
#define TPM_125_RIS_MR9IF_Pos  (5U)
#define TPM_125_RIS_MR9IF_Msk  (0x1U << TPM_125_RIS_MR9IF_Pos) /*!< 0x00000020 */
#define TPM_125_RIS_MR9IF      TPM_125_RIS_MR9IF_Msk           /*!< Match channel 9 interrupt flag */

/* Bits definition for CT16Bn RIS (n=3) register */
#define TPM_3_RIS_MR0IF_Pos (0U)
#define TPM_3_RIS_MR0IF_Msk (0x1U << TPM_3_RIS_MR0IF_Pos) /*!< 0x00000001 */
#define TPM_3_RIS_MR0IF     TPM_3_RIS_MR0IF_Msk           /*!< Match channel 0 interrupt flag */
#define TPM_3_RIS_MR1IF_Pos (1U)
#define TPM_3_RIS_MR1IF_Msk (0x1U << TPM_3_RIS_MR1IF_Pos) /*!< 0x00000002 */
#define TPM_3_RIS_MR1IF     TPM_3_RIS_MR1IF_Msk           /*!< Match channel 1 interrupt flag */
#define TPM_3_RIS_MR9IF_Pos (3U)
#define TPM_3_RIS_MR9IF_Msk (0x1U << TPM_3_RIS_MR9IF_Pos) /*!< 0x00000008 */
#define TPM_3_RIS_MR9IF     TPM_3_RIS_MR9IF_Msk           /*!< Match channel 9 interrupt flag */

/* Bits definition for CT16Bn RIS (n=4) register */
#define TPM_4_RIS_MR0IF_Pos  (0U)
#define TPM_4_RIS_MR0IF_Msk  (0x1U << TPM_4_RIS_MR0IF_Pos) /*!< 0x00000001 */
#define TPM_4_RIS_MR0IF      TPM_4_RIS_MR0IF_Msk           /*!< Match channel 0 interrupt flag */
#define TPM_4_RIS_MR1IF_Pos  (1U)
#define TPM_4_RIS_MR1IF_Msk  (0x1U << TPM_4_RIS_MR1IF_Pos) /*!< 0x00000002 */
#define TPM_4_RIS_MR1IF      TPM_4_RIS_MR1IF_Msk           /*!< Match channel 1 interrupt flag */
#define TPM_4_RIS_CAP0IF_Pos (2U)
#define TPM_4_RIS_CAP0IF_Msk (0x1U << TPM_4_RIS_CAP0IF_Pos) /*!< 0x00000004 */
#define TPM_4_RIS_CAP0IF     TPM_4_RIS_CAP0IF_Msk           /*!< Capture channel 0 interrupt flag */
#define TPM_4_RIS_MR9IF_Pos  (3U)
#define TPM_4_RIS_MR9IF_Msk  (0x1U << TPM_4_RIS_MR9IF_Pos) /*!< 0x00000008 */
#define TPM_4_RIS_MR9IF      TPM_4_RIS_MR9IF_Msk           /*!< Match channel 9 interrupt flag */

/* Bits definition for CT16Bn RIS (n=6,7) register */
#define TPM_67_RIS_MR0IF_Pos (0U)
#define TPM_67_RIS_MR0IF_Msk (0x1U << TPM_67_RIS_MR0IF_Pos) /*!< 0x00000001 */
#define TPM_67_RIS_MR0IF     TPM_67_RIS_MR0IF_Msk           /*!< Match channel 0 interrupt flag */

/* Bits definition for CT16Bn RIS (n=8) register */
#define TPM_8_RIS_MR0IF_Pos      (0U)
#define TPM_8_RIS_MR0IF_Msk      (0x1U << TPM_8_RIS_MR0IF_Pos) /*!< 0x00000001 */
#define TPM_8_RIS_MR0IF          TPM_8_RIS_MR0IF_Msk           /*!< Match channel 0 interrupt flag */
#define TPM_8_RIS_MR1IF_Pos      (1U)
#define TPM_8_RIS_MR1IF_Msk      (0x1U << TPM_8_RIS_MR1IF_Pos) /*!< 0x00000002 */
#define TPM_8_RIS_MR1IF          TPM_8_RIS_MR1IF_Msk           /*!< Match channel 1 interrupt flag */
#define TPM_8_RIS_MR2IF_Pos      (2U)
#define TPM_8_RIS_MR2IF_Msk      (0x1U << TPM_8_RIS_MR2IF_Pos) /*!< 0x00000004 */
#define TPM_8_RIS_MR2IF          TPM_8_RIS_MR2IF_Msk           /*!< Match channel 2 interrupt flag */
#define TPM_8_RIS_MR3IF_Pos      (3U)
#define TPM_8_RIS_MR3IF_Msk      (0x1U << TPM_8_RIS_MR3IF_Pos) /*!< 0x00000008 */
#define TPM_8_RIS_MR3IF          TPM_8_RIS_MR3IF_Msk           /*!< Match channel 3 interrupt flag */
#define TPM_8_RIS_MR4IF_Pos      (4U)
#define TPM_8_RIS_MR4IF_Msk      (0x1U << TPM_8_RIS_MR4IF_Pos) /*!< 0x00000010 */
#define TPM_8_RIS_MR4IF          TPM_8_RIS_MR4IF_Msk           /*!< Match channel 4 interrupt flag */
#define TPM_8_RIS_MR5IF_Pos      (5U)
#define TPM_8_RIS_MR5IF_Msk      (0x1U << TPM_8_RIS_MR5IF_Pos) /*!< 0x00000020 */
#define TPM_8_RIS_MR5IF          TPM_8_RIS_MR5IF_Msk           /*!< Match channel 5 interrupt flag */
#define TPM_8_RIS_MR6IF_Pos      (6U)
#define TPM_8_RIS_MR6IF_Msk      (0x1U << TPM_8_RIS_MR6IF_Pos) /*!< 0x00000040 */
#define TPM_8_RIS_MR6IF          TPM_8_RIS_MR6IF_Msk           /*!< Match channel 6 interrupt flag */
#define TPM_8_RIS_MR7IF_Pos      (7U)
#define TPM_8_RIS_MR7IF_Msk      (0x1U << TPM_8_RIS_MR7IF_Pos) /*!< 0x00000080 */
#define TPM_8_RIS_MR7IF          TPM_8_RIS_MR7IF_Msk           /*!< Match channel 7 interrupt flag */
#define TPM_8_RIS_MR8IF_Pos      (8U)
#define TPM_8_RIS_MR8IF_Msk      (0x1U << TPM_8_RIS_MR8IF_Pos) /*!< 0x00000100 */
#define TPM_8_RIS_MR8IF          TPM_8_RIS_MR8IF_Msk           /*!< Match channel 8 interrupt flag */
#define TPM_8_RIS_MR9IF_Pos      (9U)
#define TPM_8_RIS_MR9IF_Msk      (0x1U << TPM_8_RIS_MR9IF_Pos) /*!< 0x00000200 */
#define TPM_8_RIS_MR9IF          TPM_8_RIS_MR9IF_Msk           /*!< Match channel 9 interrupt flag */
#define TPM_8_RIS_MR10IF_Pos     (10U)
#define TPM_8_RIS_MR10IF_Msk     (0x1U << TPM_8_RIS_MR10IF_Pos) /*!< 0x00000400 */
#define TPM_8_RIS_MR10IF         TPM_8_RIS_MR10IF_Msk           /*!< Match channel 10 interrupt flag */
#define TPM_8_RIS_MR11IF_Pos     (11U)
#define TPM_8_RIS_MR11IF_Msk     (0x1U << TPM_8_RIS_MR11IF_Pos) /*!< 0x00000800 */
#define TPM_8_RIS_MR11IF         TPM_8_RIS_MR11IF_Msk           /*!< Match channel 11 interrupt flag */
#define TPM_8_RIS_MRPERIODIF_Pos (13U)
#define TPM_8_RIS_MRPERIODIF_Msk (0x1U << TPM_8_RIS_MRPERIODIF_Pos) /*!< 0x00002000 */
#define TPM_8_RIS_MRPERIODIF     TPM_8_RIS_MRPERIODIF_Msk           /*!< Match channel of Period interrupt flag */

/* Bits definition for CT16Bn IC (n=0) register */
#define TPM_0_IC_MR0IC_Pos  (0U)
#define TPM_0_IC_MR0IC_Msk  (0x1U << TPM_0_IC_MR0IC_Pos) /*!< 0x00000001 */
#define TPM_0_IC_MR0IC      TPM_0_IC_MR0IC_Msk           /*!< MR0IF clear bit */
#define TPM_0_IC_MR1IC_Pos  (1U)
#define TPM_0_IC_MR1IC_Msk  (0x1U << TPM_0_IC_MR1IC_Pos) /*!< 0x00000002 */
#define TPM_0_IC_MR1IC      TPM_0_IC_MR1IC_Msk           /*!< MR1IF clear bit */
#define TPM_0_IC_MR2IC_Pos  (2U)
#define TPM_0_IC_MR2IC_Msk  (0x1U << TPM_0_IC_MR2IC_Pos) /*!< 0x00000004 */
#define TPM_0_IC_MR2IC      TPM_0_IC_MR2IC_Msk           /*!< MR2IF clear bit */
#define TPM_0_IC_MR3IC_Pos  (3U)
#define TPM_0_IC_MR3IC_Msk  (0x1U << TPM_0_IC_MR3IC_Pos) /*!< 0x00000008 */
#define TPM_0_IC_MR3IC      TPM_0_IC_MR3IC_Msk           /*!< MR3IF clear bit */
#define TPM_0_IC_CAP0IC_Pos (4U)
#define TPM_0_IC_CAP0IC_Msk (0x1U << TPM_0_IC_CAP0IC_Pos) /*!< 0x00000010 */
#define TPM_0_IC_CAP0IC     TPM_0_IC_CAP0IC_Msk           /*!< CAP0IF clear bit */
#define TPM_0_IC_MR9IC_Pos  (5U)
#define TPM_0_IC_MR9IC_Msk  (0x1U << TPM_0_IC_MR9IC_Pos) /*!< 0x00000020 */
#define TPM_0_IC_MR9IC      TPM_0_IC_MR9IC_Msk           /*!< MR9IF clear bit */
#define TPM_0_IC_BRKIC_Pos  (31U)
#define TPM_0_IC_BRKIC_Msk  (0x1U << TPM_0_IC_BRKIC_Pos) /*!< 0x80000000 */
#define TPM_0_IC_BRKIC      TPM_0_IC_BRKIC_Msk           /*!< BRKIF clear bit */

/* Bits definition for CT16Bn IC (n=1,2,5) register */
#define TPM_125_IC_MR0IC_Pos  (0U)
#define TPM_125_IC_MR0IC_Msk  (0x1U << TPM_125_IC_MR0IC_Pos) /*!< 0x00000001 */
#define TPM_125_IC_MR0IC      TPM_125_IC_MR0IC_Msk           /*!< MR0IF clear bit */
#define TPM_125_IC_MR1IC_Pos  (1U)
#define TPM_125_IC_MR1IC_Msk  (0x1U << TPM_125_IC_MR1IC_Pos) /*!< 0x00000002 */
#define TPM_125_IC_MR1IC      TPM_125_IC_MR1IC_Msk           /*!< MR1IF clear bit */
#define TPM_125_IC_MR2IC_Pos  (2U)
#define TPM_125_IC_MR2IC_Msk  (0x1U << TPM_125_IC_MR2IC_Pos) /*!< 0x00000004 */
#define TPM_125_IC_MR2IC      TPM_125_IC_MR2IC_Msk           /*!< MR2IF clear bit */
#define TPM_125_IC_MR3IC_Pos  (3U)
#define TPM_125_IC_MR3IC_Msk  (0x1U << TPM_125_IC_MR3IC_Pos) /*!< 0x00000008 */
#define TPM_125_IC_MR3IC      TPM_125_IC_MR3IC_Msk           /*!< MR3IF clear bit */
#define TPM_125_IC_CAP0IC_Pos (4U)
#define TPM_125_IC_CAP0IC_Msk (0x1U << TPM_125_IC_CAP0IC_Pos) /*!< 0x00000010 */
#define TPM_125_IC_CAP0IC     TPM_125_IC_CAP0IC_Msk           /*!< CAP0IF clear bit */
#define TPM_125_IC_MR9IC_Pos  (5U)
#define TPM_125_IC_MR9IC_Msk  (0x1U << TPM_125_IC_MR9IC_Pos) /*!< 0x00000020 */
#define TPM_125_IC_MR9IC      TPM_125_IC_MR9IC_Msk           /*!< MR9IF clear bit */

/* Bits definition for CT16Bn IC (n=3) register */
#define TPM_3_IC_MR0IC_Pos (0U)
#define TPM_3_IC_MR0IC_Msk (0x1U << TPM_3_IC_MR0IC_Pos) /*!< 0x00000001 */
#define TPM_3_IC_MR0IC     TPM_3_IC_MR0IC_Msk           /*!< MR0IF clear bit */
#define TPM_3_IC_MR1IC_Pos (1U)
#define TPM_3_IC_MR1IC_Msk (0x1U << TPM_3_IC_MR1IC_Pos) /*!< 0x00000002 */
#define TPM_3_IC_MR1IC     TPM_3_IC_MR1IC_Msk           /*!< MR1IF clear bit */
#define TPM_3_IC_MR9IC_Pos (3U)
#define TPM_3_IC_MR9IC_Msk (0x1U << TPM_3_IC_MR9IC_Pos) /*!< 0x00000008 */
#define TPM_3_IC_MR9IC     TPM_3_IC_MR9IC_Msk           /*!< MR9IF clear bit */

/* Bits definition for CT16Bn IC (n=4) register */
#define TPM_4_IC_MR0IC_Pos  (0U)
#define TPM_4_IC_MR0IC_Msk  (0x1U << TPM_4_IC_MR0IC_Pos) /*!< 0x00000001 */
#define TPM_4_IC_MR0IC      TPM_4_IC_MR0IC_Msk           /*!< MR0IF clear bit */
#define TPM_4_IC_MR1IC_Pos  (1U)
#define TPM_4_IC_MR1IC_Msk  (0x1U << TPM_4_IC_MR1IC_Pos) /*!< 0x00000002 */
#define TPM_4_IC_MR1IC      TPM_4_IC_MR1IC_Msk           /*!< MR1IF clear bit */
#define TPM_4_IC_CAP0IC_Pos (2U)
#define TPM_4_IC_CAP0IC_Msk (0x1U << TPM_4_IC_CAP0IC_Pos) /*!< 0x00000004 */
#define TPM_4_IC_CAP0IC     TPM_4_IC_CAP0IC_Msk           /*!< CAP0IF clear bit */
#define TPM_4_IC_MR9IC_Pos  (3U)
#define TPM_4_IC_MR9IC_Msk  (0x1U << TPM_4_IC_MR9IC_Pos) /*!< 0x00000008 */
#define TPM_4_IC_MR9IC      TPM_4_IC_MR9IC_Msk           /*!< MR9IF clear bit */

/* Bits definition for CT16Bn IC (n=6,7) register */
#define TPM_67_IC_MR0IC_Pos (0U)
#define TPM_67_IC_MR0IC_Msk (0x1U << TPM_67_IC_MR0IC_Pos) /*!< 0x00000001 */
#define TPM_67_IC_MR0IC     TPM_67_IC_MR0IC_Msk           /*!< MR0IF clear bit */

/* Bits definition for CT16Bn IC (n=8) register */
#define TPM_8_IC_MR0IC_Pos      (0U)
#define TPM_8_IC_MR0IC_Msk      (0x1U << TPM_8_IC_MR0IC_Pos) /*!< 0x00000001 */
#define TPM_8_IC_MR0IC          TPM_8_IC_MR0IC_Msk           /*!< MR0IF clear bit */
#define TPM_8_IC_MR1IC_Pos      (1U)
#define TPM_8_IC_MR1IC_Msk      (0x1U << TPM_8_IC_MR1IC_Pos) /*!< 0x00000002 */
#define TPM_8_IC_MR1IC          TPM_8_IC_MR1IC_Msk           /*!< MR1IF clear bit */
#define TPM_8_IC_MR2IC_Pos      (2U)
#define TPM_8_IC_MR2IC_Msk      (0x1U << TPM_8_IC_MR2IC_Pos) /*!< 0x00000004 */
#define TPM_8_IC_MR2IC          TPM_8_IC_MR2IC_Msk           /*!< MR2IF clear bit */
#define TPM_8_IC_MR3IC_Pos      (3U)
#define TPM_8_IC_MR3IC_Msk      (0x1U << TPM_8_IC_MR3IC_Pos) /*!< 0x00000008 */
#define TPM_8_IC_MR3IC          TPM_8_IC_MR3IC_Msk           /*!< MR3IF clear bit */
#define TPM_8_IC_MR4IC_Pos      (4U)
#define TPM_8_IC_MR4IC_Msk      (0x1U << TPM_8_IC_MR4IC_Pos) /*!< 0x00000010 */
#define TPM_8_IC_MR4IC          TPM_8_IC_MR4IC_Msk           /*!< MR4IF clear bit */
#define TPM_8_IC_MR5IC_Pos      (5U)
#define TPM_8_IC_MR5IC_Msk      (0x1U << TPM_8_IC_MR5IC_Pos) /*!< 0x00000020 */
#define TPM_8_IC_MR5IC          TPM_8_IC_MR5IC_Msk           /*!< MR5IF clear bit */
#define TPM_8_IC_MR6IC_Pos      (6U)
#define TPM_8_IC_MR6IC_Msk      (0x1U << TPM_8_IC_MR6IC_Pos) /*!< 0x00000040 */
#define TPM_8_IC_MR6IC          TPM_8_IC_MR6IC_Msk           /*!< MR6IF clear bit */
#define TPM_8_IC_MR7IC_Pos      (7U)
#define TPM_8_IC_MR7IC_Msk      (0x1U << TPM_8_IC_MR7IC_Pos) /*!< 0x00000080 */
#define TPM_8_IC_MR7IC          TPM_8_IC_MR7IC_Msk           /*!< MR7IF clear bit */
#define TPM_8_IC_MR8IC_Pos      (8U)
#define TPM_8_IC_MR8IC_Msk      (0x1U << TPM_8_IC_MR8IC_Pos) /*!< 0x00000100 */
#define TPM_8_IC_MR8IC          TPM_8_IC_MR8IC_Msk           /*!< MR8IF clear bit */
#define TPM_8_IC_MR9IC_Pos      (9U)
#define TPM_8_IC_MR9IC_Msk      (0x1U << TPM_8_IC_MR9IC_Pos) /*!< 0x00000200 */
#define TPM_8_IC_MR9IC          TPM_8_IC_MR9IC_Msk           /*!< MR9IF clear bit */
#define TPM_8_IC_MR10IC_Pos     (10U)
#define TPM_8_IC_MR10IC_Msk     (0x1U << TPM_8_IC_MR10IC_Pos) /*!< 0x00000400 */
#define TPM_8_IC_MR10IC         TPM_8_IC_MR10IC_Msk           /*!< MR10IF clear bit */
#define TPM_8_IC_MR11IC_Pos     (11U)
#define TPM_8_IC_MR11IC_Msk     (0x1U << TPM_8_IC_MR11IC_Pos) /*!< 0x00000800 */
#define TPM_8_IC_MR11IC         TPM_8_IC_MR11IC_Msk           /*!< MR11IF clear bit */
#define TPM_8_IC_MRPERIODIC_Pos (13U)
#define TPM_8_IC_MRPERIODIC_Msk (0x1U << TPM_8_IC_MRPERIODIC_Pos) /*!< 0x00001000 */
#define TPM_8_IC_MRPERIODIC     TPM_8_IC_MRPERIODIC_Msk           /*!< MRPERIODIF clear bit */

/* Bits definition for CT16Bn PWMmNDB (n=0, m=0,1,2,3;  n=3,4, m=0,1) register */
#define TPM_034_PWMmNDB_DB_Pos (0U)
#define TPM_034_PWMmNDB_DB_Msk (0x3FFU << TPM_034_PWMmNDB_DB_Pos) /*!< 0x000003FF */
#define TPM_034_PWMmNDB_DB     TPM_034_PWMmNDB_DB_Msk             /*!< Count of PWMmN output dead-band period time \
                                                                       PWM0N output dead-band period time=DB*CT16Bn_PCLK*(PR+1) cycle */

/* Bits definition for CT16Bn LOADCTRL (n=0,1,2,5) register */
#define TPM_0125_LOADCTRL_LOADMR0_Pos           (0U)
#define TPM_0125_LOADCTRL_LOADMR0_Msk           (0x3U << TPM_0125_LOADCTRL_LOADMR0_Pos) /*!< 0x00000003 */
#define TPM_0125_LOADCTRL_LOADMR0               TPM_0125_LOADCTRL_LOADMR0_Msk           /*!< MR0 load mode selection in center-aligned mode */
#define TPM_0125_LOADCTRL_LOADMR0_None          (0x0U << TPM_0125_LOADCTRL_LOADMR0_Pos) /*!< No effect */
#define TPM_0125_LOADCTRL_LOADMR0_LoadAndFixed  (0x1U << TPM_0125_LOADCTRL_LOADMR0_Pos) /*!< Load MR0 value at TC=0 and MR0 value is unchanged at TC=MR9 */
#define TPM_0125_LOADCTRL_LOADMR0_LoadAndChange (0x3U << TPM_0125_LOADCTRL_LOADMR0_Pos) /*!< Load MR0 value at TC=0 and TC=MR9 */

#define TPM_0125_LOADCTRL_LOADMR1_Pos           (2U)
#define TPM_0125_LOADCTRL_LOADMR1_Msk           (0x3U << TPM_0125_LOADCTRL_LOADMR1_Pos) /*!< 0x0000000C */
#define TPM_0125_LOADCTRL_LOADMR1               TPM_0125_LOADCTRL_LOADMR1_Msk           /*!< MR1 load mode selection in center-aligned mode */
#define TPM_0125_LOADCTRL_LOADMR1_None          (0x0U << TPM_0125_LOADCTRL_LOADMR1_Pos) /*!< No effect */
#define TPM_0125_LOADCTRL_LOADMR1_LoadAndFixed  (0x1U << TPM_0125_LOADCTRL_LOADMR1_Pos) /*!< Load MR1 value at TC=0 and MR1 value is unchanged at TC=MR9 */
#define TPM_0125_LOADCTRL_LOADMR1_LoadAndChange (0x3U << TPM_0125_LOADCTRL_LOADMR1_Pos) /*!< Load MR1 value at TC=0 and TC=MR9 */

#define TPM_0125_LOADCTRL_LOADMR2_Pos           (4U)
#define TPM_0125_LOADCTRL_LOADMR2_Msk           (0x3U << TPM_0125_LOADCTRL_LOADMR2_Pos) /*!< 0x00000030 */
#define TPM_0125_LOADCTRL_LOADMR2               TPM_0125_LOADCTRL_LOADMR2_Msk           /*!< MR2 load mode selection in center-aligned mode */
#define TPM_0125_LOADCTRL_LOADMR2_None          (0x0U << TPM_0125_LOADCTRL_LOADMR2_Pos) /*!< No effect */
#define TPM_0125_LOADCTRL_LOADMR2_LoadAndFixed  (0x1U << TPM_0125_LOADCTRL_LOADMR2_Pos) /*!< Load MR2 value at TC=0 and MR2 value is unchanged at TC=MR9 */
#define TPM_0125_LOADCTRL_LOADMR2_LoadAndChange (0x3U << TPM_0125_LOADCTRL_LOADMR2_Pos) /*!< Load MR2 value at TC=0 and TC=MR9 */

#define TPM_0125_LOADCTRL_LOADMR3_Pos           (6U)
#define TPM_0125_LOADCTRL_LOADMR3_Msk           (0x3U << TPM_0125_LOADCTRL_LOADMR3_Pos) /*!< 0x000000C0 */
#define TPM_0125_LOADCTRL_LOADMR3               TPM_0125_LOADCTRL_LOADMR3_Msk           /*!< MR3 load mode selection in center-aligned mode */
#define TPM_0125_LOADCTRL_LOADMR3_None          (0x0U << TPM_0125_LOADCTRL_LOADMR3_Pos) /*!< No effect */
#define TPM_0125_LOADCTRL_LOADMR3_LoadAndFixed  (0x1U << TPM_0125_LOADCTRL_LOADMR3_Pos) /*!< Load MR3 value at TC=0 and MR3 value is unchanged at TC=MR9 */
#define TPM_0125_LOADCTRL_LOADMR3_LoadAndChange (0x3U << TPM_0125_LOADCTRL_LOADMR3_Pos) /*!< Load MR3 value at TC=0 and TC=MR9 */

/* Bits definition for CT16Bn DMA (n=0,1,2,5) register */
#define TPM_0125_DMA_DMAMR0_Pos  (0U)
#define TPM_0125_DMA_DMAMR0_Msk  (0x1U << TPM_0125_DMA_DMAMR0_Pos) /*!< 0x00000001 */
#define TPM_0125_DMA_DMAMR0      TPM_0125_DMA_DMAMR0_Msk           /*!< MR0 DMA request active enable */
#define TPM_0125_DMA_DMAMR1_Pos  (1U)
#define TPM_0125_DMA_DMAMR1_Msk  (0x1U << TPM_0125_DMA_DMAMR1_Pos) /*!< 0x00000002 */
#define TPM_0125_DMA_DMAMR1      TPM_0125_DMA_DMAMR1_Msk           /*!< MR1 DMA request active enable */
#define TPM_0125_DMA_DMAMR2_Pos  (2U)
#define TPM_0125_DMA_DMAMR2_Msk  (0x1U << TPM_0125_DMA_DMAMR2_Pos) /*!< 0x00000004 */
#define TPM_0125_DMA_DMAMR2      TPM_0125_DMA_DMAMR2_Msk           /*!< MR2 DMA request active enable */
#define TPM_0125_DMA_DMAMR3_Pos  (3U)
#define TPM_0125_DMA_DMAMR3_Msk  (0x1U << TPM_0125_DMA_DMAMR3_Pos) /*!< 0x00000008 */
#define TPM_0125_DMA_DMAMR3      TPM_0125_DMA_DMAMR3_Msk           /*!< MR3 DMA request active enable */
#define TPM_0125_DMA_DMACAP0_Pos (4U)
#define TPM_0125_DMA_DMACAP0_Msk (0x1U << TPM_0125_DMA_DMACAP0_Pos) /*!< 0x00000010 */
#define TPM_0125_DMA_DMACAP0     TPM_0125_DMA_DMACAP0_Msk           /*!< CAP0 DMA request active enable */
#define TPM_0125_DMA_DMAMR9_Pos  (5U)
#define TPM_0125_DMA_DMAMR9_Msk  (0x1U << TPM_0125_DMA_DMAMR9_Pos) /*!< 0x00000020 */
#define TPM_0125_DMA_DMAMR9      TPM_0125_DMA_DMAMR9_Msk           /*!< MR9 DMA request active enable */

/* Bits definition for CT16Bn DMA (n=3) register */
#define TPM_3_DMA_DMAMR0_Pos (0U)
#define TPM_3_DMA_DMAMR0_Msk (0x1U << TPM_3_DMA_DMAMR0_Pos) /*!< 0x00000001 */
#define TPM_3_DMA_DMAMR0     TPM_3_DMA_DMAMR0_Msk           /*!< MR0 DMA request active enable */
#define TPM_3_DMA_DMAMR1_Pos (1U)
#define TPM_3_DMA_DMAMR1_Msk (0x1U << TPM_3_DMA_DMAMR1_Pos) /*!< 0x00000002 */
#define TPM_3_DMA_DMAMR1     TPM_3_DMA_DMAMR1_Msk           /*!< MR1 DMA request active enable */
#define TPM_3_DMA_DMAMR9_Pos (3U)
#define TPM_3_DMA_DMAMR9_Msk (0x1U << TPM_3_DMA_DMAMR9_Pos) /*!< 0x00000008 */
#define TPM_3_DMA_DMAMR9     TPM_3_DMA_DMAMR9_Msk           /*!< MR9 DMA request active enable */

/* Bits definition for CT16Bn DMA (n=4) register */
#define TPM_4_DMA_DMAMR0_Pos  (0U)
#define TPM_4_DMA_DMAMR0_Msk  (0x1U << TPM_4_DMA_DMAMR0_Pos) /*!< 0x00000001 */
#define TPM_4_DMA_DMAMR0      TPM_4_DMA_DMAMR0_Msk           /*!< MR0 DMA request active enable */
#define TPM_4_DMA_DMAMR1_Pos  (1U)
#define TPM_4_DMA_DMAMR1_Msk  (0x1U << TPM_4_DMA_DMAMR1_Pos) /*!< 0x00000002 */
#define TPM_4_DMA_DMAMR1      TPM_4_DMA_DMAMR1_Msk           /*!< MR1 DMA request active enable */
#define TPM_4_DMA_DMACAP0_Pos (2U)
#define TPM_4_DMA_DMACAP0_Msk (0x1U << TPM_4_DMA_DMACAP0_Pos) /*!< 0x00000004 */
#define TPM_4_DMA_DMACAP0     TPM_4_DMA_DMACAP0_Msk           /*!< CAP0 DMA request active enable */
#define TPM_4_DMA_DMAMR9_Pos  (3U)
#define TPM_4_DMA_DMAMR9_Msk  (0x1U << TPM_4_DMA_DMAMR9_Pos) /*!< 0x00000008 */
#define TPM_4_DMA_DMAMR9      TPM_4_DMA_DMAMR9_Msk           /*!< MR9 DMA request active enable */

/* Bits definition for CT16Bn DMA (n=6,7) register */
#define TPM_67_DMA_DMAMR0_Pos (0U)
#define TPM_67_DMA_DMAMR0_Msk (0x1U << TPM_67_DMA_DMAMR0_Pos) /*!< 0x00000001 */
#define TPM_67_DMA_DMAMR0     TPM_67_DMA_DMAMR0_Msk           /*!< MR0 DMA request active enable */

/* Bits definition for CT16Bn DMA (n=8) register */
#define TPM_8_DMA_DMAMRPERIOD_Pos (13U)
#define TPM_8_DMA_DMAMRPERIOD_Msk (0x1U << TPM_8_DMA_DMAMRPERIOD_Pos) /*!< 0x00002000 */
#define TPM_8_DMA_DMAMRPERIOD     TPM_8_DMA_DMAMRPERIOD_Msk           /*!< MR_PERIOD DMA request active enable */

/* Bits definition for CT16Bn DMAMRA1 (n=0,1,2,3,4,5,8) register */
#define TPM_0123458_DMAMRA1_MR0ALIAS_Pos (0U)
#define TPM_0123458_DMAMRA1_MR0ALIAS_Msk (0xFFFFU << TPM_0123458_DMAMRA1_MR0ALIAS_Pos) /*!< 0x0000FFFF */
#define TPM_0123458_DMAMRA1_MR0ALIAS     TPM_0123458_DMAMRA1_MR0ALIAS_Msk              /*!< MR0 alias for DMA access */
#define TPM_0123458_DMAMRA1_MR1ALIAS_Pos (16U)
#define TPM_0123458_DMAMRA1_MR1ALIAS_Msk (0xFFFFU << TPM_0123458_DMAMRA1_MR1ALIAS_Pos) /*!< 0xFFFF0000 */
#define TPM_0123458_DMAMRA1_MR1ALIAS     TPM_0123458_DMAMRA1_MR1ALIAS_Msk              /*!< MR1 alias for DMA access */

/* Bits definition for CT16Bn DMAMRA2 (n=0,1,2,5,8) register */
#define TPM_01258_DMAMRA2_MR2ALIAS_Pos (0U)
#define TPM_01258_DMAMRA2_MR2ALIAS_Msk (0xFFFFU << TPM_01258_DMAMRA2_MR2ALIAS_Pos) /*!< 0x0000FFFF */
#define TPM_01258_DMAMRA2_MR2ALIAS     TPM_01258_DMAMRA2_MR2ALIAS_Msk              /*!< MR2 alias for DMA access */
#define TPM_01258_DMAMRA2_MR3ALIAS_Pos (16U)
#define TPM_01258_DMAMRA2_MR3ALIAS_Msk (0xFFFFU << TPM_01258_DMAMRA2_MR3ALIAS_Pos) /*!< 0xFFFF0000 */
#define TPM_01258_DMAMRA2_MR3ALIAS     TPM_01258_DMAMRA2_MR3ALIAS_Msk              /*!< MR3 alias for DMA access */

/* Bits definition for CT16Bn DMAMRA3 (n=8) register */
#define TPM_8_DMAMRA3_MR4ALIAS_Pos (0U)
#define TPM_8_DMAMRA3_MR4ALIAS_Msk (0xFFFFU << TPM_8_DMAMRA3_MR4ALIAS_Pos) /*!< 0x0000FFFF */
#define TPM_8_DMAMRA3_MR4ALIAS     TPM_8_DMAMRA3_MR4ALIAS_Msk              /*!< MR4 alias for DMA access */
#define TPM_8_DMAMRA3_MR5ALIAS_Pos (16U)
#define TPM_8_DMAMRA3_MR5ALIAS_Msk (0xFFFFU << TPM_8_DMAMRA3_MR5ALIAS_Pos) /*!< 0xFFFF0000 */
#define TPM_8_DMAMRA3_MR5ALIAS     TPM_8_DMAMRA3_MR5ALIAS_Msk              /*!< MR5 alias for DMA access */

/* Bits definition for CT16Bn DMAMRA4 (n=8) register */
#define TPM_8_DMAMRA4_MR6ALIAS_Pos (0U)
#define TPM_8_DMAMRA4_MR6ALIAS_Msk (0xFFFFU << TPM_8_DMAMRA4_MR6ALIAS_Pos) /*!< 0x0000FFFF */
#define TPM_8_DMAMRA4_MR6ALIAS     TPM_8_DMAMRA4_MR6ALIAS_Msk              /*!< MR6 alias for DMA access */
#define TPM_8_DMAMRA4_MR7ALIAS_Pos (16U)
#define TPM_8_DMAMRA4_MR7ALIAS_Msk (0xFFFFU << TPM_8_DMAMRA4_MR7ALIAS_Pos) /*!< 0xFFFF0000 */
#define TPM_8_DMAMRA4_MR7ALIAS     TPM_8_DMAMRA4_MR7ALIAS_Msk              /*!< MR7 alias for DMA access */

/* Bits definition for CT16Bn DMAMRA5 (n=8) register */
#define TPM_8_DMAMRA5_MR8ALIAS_Pos (0U)
#define TPM_8_DMAMRA5_MR8ALIAS_Msk (0xFFFFU << TPM_8_DMAMRA5_MR8ALIAS_Pos) /*!< 0x0000FFFF */
#define TPM_8_DMAMRA5_MR8ALIAS     TPM_8_DMAMRA5_MR8ALIAS_Msk              /*!< MR8 alias for DMA access */
#define TPM_8_DMAMRA5_MR9ALIAS_Pos (16U)
#define TPM_8_DMAMRA5_MR9ALIAS_Msk (0xFFFFU << TPM_8_DMAMRA5_MR9ALIAS_Pos) /*!< 0xFFFF0000 */
#define TPM_8_DMAMRA5_MR9ALIAS     TPM_8_DMAMRA5_MR9ALIAS_Msk              /*!< MR9 alias for DMA access */

/* Bits definition for CT16Bn DMAMRA6 (n=8) register */
#define TPM_8_DMAMRA6_MR10ALIAS_Pos (0U)
#define TPM_8_DMAMRA6_MR10ALIAS_Msk (0xFFFFU << TPM_8_DMAMRA6_MR10ALIAS_Pos) /*!< 0x0000FFFF */
#define TPM_8_DMAMRA6_MR10ALIAS     TPM_8_DMAMRA6_MR10ALIAS_Msk              /*!< MR10 alias for DMA access */
#define TPM_8_DMAMRA6_MR11ALIAS_Pos (16U)
#define TPM_8_DMAMRA6_MR11ALIAS_Msk (0xFFFFU << TPM_8_DMAMRA6_MR11ALIAS_Pos) /*!< 0xFFFF0000 */
#define TPM_8_DMAMRA6_MR11ALIAS     TPM_8_DMAMRA6_MR11ALIAS_Msk              /*!< MR11 alias for DMA access */

/* Bits definition for CT16Bn BRKCTRL (n=0) register */
#define TPM_0_BRKCTRL_BRKSEL_Pos      (0U)
#define TPM_0_BRKCTRL_BRKSEL_Msk      (0x7U << TPM_0_BRKCTRL_BRKSEL_Pos) /*!< 0x00000007 */
#define TPM_0_BRKCTRL_BRKSEL          TPM_0_BRKCTRL_BRKSEL_Msk           /*!< The trigger source selection of PWM channels break function */
#define TPM_0_BRKCTRL_BRKSEL_Disable  (0x0U << TPM_0_BRKCTRL_BRKSEL_Pos) /*!< Disable */
#define TPM_0_BRKCTRL_BRKSEL_BreakPin (0x4U << TPM_0_BRKCTRL_BRKSEL_Pos) /*!< Break pin (BRK) */
#define TPM_0_BRKCTRL_BRKLEVEL_Pos    (3U)
#define TPM_0_BRKCTRL_BRKLEVEL_Msk    (0x1U << TPM_0_BRKCTRL_BRKLEVEL_Pos) /*!< 0x00000008 */
#define TPM_0_BRKCTRL_BRKLEVEL        TPM_0_BRKCTRL_BRKLEVEL_Msk           /*!< The trigger source selection of PWM channels break function */
#define TPM_0_BRKCTRL_BRKDB_Pos       (4U)
#define TPM_0_BRKCTRL_BRKDB_Msk       (0x7U << TPM_0_BRKCTRL_BRKDB_Pos) /*!< 0x00000070 */
#define TPM_0_BRKCTRL_BRKDB           TPM_0_BRKCTRL_BRKDB_Msk           /*!< Break pin (BRK) debounce time */
#define TPM_0_BRKCTRL_BRKIE_Pos       (30U)
#define TPM_0_BRKCTRL_BRKIE_Msk       (0x1U << TPM_0_BRKCTRL_BRKIE_Pos) /*!< 0x40000000 */
#define TPM_0_BRKCTRL_BRKIE           TPM_0_BRKCTRL_BRKIE_Msk           /*!< Enable generating an interrupt when BRKIF = 1 */

/******************************************************************************/
/* UART                                                                       */
/******************************************************************************/

/* Bits definition for Window UART n Receiver Buffer register */
#define UART_RB_RB_Pos (0U)
#define UART_RB_RB_Msk (0xFFU << UART_RB_RB_Pos) /*!< 0x000000FF */
#define UART_RB_RB     UART_RB_RB_Msk            /*!< The received byte in UART RX FIFO */

/* Bits definition for Window UART n Transmitter Holding register */
#define UART_TH_TH_Pos (0U)
#define UART_TH_TH_Msk (0xFFU << UART_TH_TH_Pos) /*!< 0x000000FF */
#define UART_TH_TH     UART_TH_TH_Msk            /*!< The byte to be transmitted in UART TX FIFO when transmitter is available */

/* Bits definition for Window UART n Divisor Latch LSB registers */
#define UART_DLL_DLL_Pos (0U)
#define UART_DLL_DLL_Msk (0xFFU << UART_DLL_DLL_Pos) /*!< 0x000000FF */
#define UART_DLL_DLL     UART_DLL_DLL_Msk            /*!< LSB of UART baud rate divisor, along with the DLM register, determines the baud rate of the UART*/

/* UART n Divisor Latch MSB register */
#define UART_DLM_DLM_Pos (0U)
#define UART_DLM__DLMMsk (0xFFU << UART_DLM_DLM_Pos) /*!< 0x000000FF */
#define UART_DLM_DLM     UART_DLM__DLMMsk            /*!< MSB of UART baud rate divisor, along with the DLL register, determines the baud rate of the UART*/

/* Bits definition for Window UART n Interrupt Enable register */
#define UART_IE_RDAIE_Pos  (0U)
#define UART_IE_RDAIE_Msk  (0x1U << UART_IE_RDAIE_Pos) /*!< 0x00000001 */
#define UART_IE_RDAIE      UART_IE_RDAIE_Msk           /*!< RDA interrupt enable */
#define UART_IE_THREIE_Pos (1U)
#define UART_IE_THREIE_Msk (0x1U << UART_IE_THREIE_Pos) /*!< 0x00000002 */
#define UART_IE_THREIE     UART_IE_THREIE_Msk           /*!< THRE interrupt enable */
#define UART_IE_RLSIE_Pos  (2U)
#define UART_IE_RLSIE_Msk  (0x1U << UART_IE_RLSIE_Pos) /*!< 0x00000004 */
#define UART_IE_RLSIE      UART_IE_RLSIE_Msk           /*!< RLS interrupt enable */
#define UART_IE_MSIE_Pos   (3U)
#define UART_IE_MSIE_Msk   (0x1U << UART_IE_MSIE_Pos) /*!< 0x00000008 */
#define UART_IE_MSIE       UART_IE_MSIE_Msk           /*!< Modem status interrupt enable */
#define UART_IE_RTSEN_Pos  (4U)
#define UART_IE_RTSEN_Msk  (0x1U << UART_IE_RTSEN_Pos) /*!< 0x00000010 */
#define UART_IE_RTSEN      UART_IE_RTSEN_Msk           /*!< RTS flow control enable bit.When RTSEN is set to 1,the loopback mode must not be set to 1 and FIFO must be enabled */
#define UART_IE_CTSEN_Pos  (5U)
#define UART_IE_CTSEN_Msk  (0x1U << UART_IE_CTSEN_Pos) /*!< 0x00000020 */
#define UART_IE_CTSEN      UART_IE_CTSEN_Msk           /*!< CTS flow control enable bit.When CTSEN is set to 1,the loopback mode must not be set to 1 and FIFO must be enabled */

/* Bits definition for Window UART n Interrupt Identification register */
#define UART_II_INTSTATUS_Pos  (0U)
#define UART_II_INTSTATUS_Msk  (0x1U << UART_II_INTSTATUS_Pos) /*!< 0x00000001 */
#define UART_II_INTSTATUS      UART_II_INTSTATUS_Msk           /*!< Interrupt status */
#define UART_II_INTID_Pos      (1U)
#define UART_II_INTID_Msk      (0x7U << UART_II_INTID_Pos) /*!< 0x0000000E */
#define UART_II_INTID          UART_II_INTID_Msk           /*!< Interrupt identification which identifies an interrupt corresponding to the UARTn RX FIFO */
#define UART_II_TXFIFOFULL_Pos (4U)
#define UART_II_TXFIFOFULL_Msk (0x1U << UART_II_TXFIFOFULL_Pos) /*!< 0x00000010 */
#define UART_II_TXFIFOFULL     UART_II_TXFIFOFULL_Msk           /*!< TX FIFO full */
#define UART_II_FIFOEN_Pos     (6U)
#define UART_II_FIFOEN_Msk     (0x3U << UART_II_FIFOEN_Pos) /*!< 0x000000C0 */
#define UART_II_FIFOEN         UART_II_FIFOEN_Msk           /*!< Equal to FIFOEN bit in UARTn_FIFOCTRL register */

/* Bits definition for Window UART n FIFO Control register */
#define UART_FIFOCTRL_FIFOEN_Pos    (0U)
#define UART_FIFOCTRL_FIFOEN_Msk    (0x1U << UART_FIFOCTRL_FIFOEN_Pos) /*!< 0x00000001 */
#define UART_FIFOCTRL_FIFOEN        UART_FIFOCTRL_FIFOEN_Msk           /*!< FIFO enable */
#define UART_FIFOCTRL_RXFIFORST_Pos (1U)
#define UART_FIFOCTRL_RXFIFORST_Msk (0x1U << UART_FIFOCTRL_RXFIFORST_Pos) /*!< 0x00000002 */
#define UART_FIFOCTRL_RXFIFORST     UART_FIFOCTRL_RXFIFORST_Msk           /*!< RX FIFO reset */
#define UART_FIFOCTRL_TXFIFORST_Pos (2U)
#define UART_FIFOCTRL_TXFIFORST_Msk (0x1U << UART_FIFOCTRL_TXFIFORST_Pos) /*!< 0x00000004 */
#define UART_FIFOCTRL_TXFIFORST     UART_FIFOCTRL_TXFIFORST_Msk           /*!< TX FIFO reset */
#define UART_FIFOCTRL_RXTL_Pos      (6U)
#define UART_FIFOCTRL_RXTL_Msk      (0x3U << UART_FIFOCTRL_RXTL_Pos) /*!< 0x000000C0 */
#define UART_FIFOCTRL_RXTL          UART_FIFOCTRL_RXTL_Msk           /*!< Trigger level of RX FIFO interrupt */
#define UART_FIFOCTRL_RXTL_1        (0x0U << UART_FIFOCTRL_RXTL_Pos) /*!< 1 character */
#define UART_FIFOCTRL_RXTL_4        (0x1U << UART_FIFOCTRL_RXTL_Pos) /*!< 4 character */
#define UART_FIFOCTRL_RXTL_8        (0x2U << UART_FIFOCTRL_RXTL_Pos) /*!< 8 character */
#define UART_FIFOCTRL_RXTL_14       (0x3U << UART_FIFOCTRL_RXTL_Pos) /*!< 14 character */

/* Bits definition for Window UART n FIFO Prescaler register */
#define UART_PRE_PRE_Pos (0U)
#define UART_PRE_PRE_Msk (0x1FU << UART_PRE_PRE_Pos) /*!< 0x0000001F */
#define UART_PRE         UART_PRE_PRE_Msk            /*!< The byte to be transmitted in UART TX FIFO when transmitter is available */

/* Bits definition for Window UART n Line Control register */
#define UART_LC_WLS_Pos  (0U)
#define UART_LC_WLS_Msk  (0x3U << UART_LC_WLS_Pos) /*!< 0x00000003 */
#define UART_LC_WLS      UART_LC_WLS_Msk           /*!< Word length selection */
#define UART_LC_SBS_Pos  (2U)
#define UART_LC_SBS_Msk  (0x1U << UART_LC_SBS_Pos) /*!< 0x00000004 */
#define UART_LC_SBS      UART_LC_SBS_Msk           /*!< Stop bit selection */
#define UART_LC_PE_Pos   (3U)
#define UART_LC_PE_Msk   (0x1U << UART_LC_PE_Pos) /*!< 0x00000008 */
#define UART_LC_PE       UART_LC_PE_Msk           /*!< Parity enable */
#define UART_LC_PS_Pos   (4U)
#define UART_LC_PS_Msk   (0x3U << UART_LC_PS_Pos) /*!< 0x00000030 */
#define UART_LC_PS       UART_LC_PS_Msk           /*!< Parity selection */
#define UART_LC_BC_Pos   (6U)
#define UART_LC_BC_Msk   (0x1U << UART_LC_BC_Pos) /*!< 0x00000040 */
#define UART_LC_BC       UART_LC_BC_Msk           /*!< Break control */
#define UART_LC_DLAB_Pos (7U)
#define UART_LC_DLAB_Msk (0x1U << UART_LC_DLAB_Pos) /*!< 0x00000080 */
#define UART_LC_DLAB     UART_LC_DLAB_Msk           /*!< Divisor Latch access */

/* Bits definition for Window UART n Modem Control register */
#define UART_MC_RTSCTRL_Pos (1U)
#define UART_MC_RTSCTRL_Msk (0x1U << UART_MC_RTSCTRL_Pos) /*!< 0x00000002 */
#define UART_MC_RTSCTRL     UART_MC_RTSCTRL_Msk           /*!< Source from modem output (RTSn) pin. RTSn pin is always forced to inactive state (high) in modem loopback mode */
#define UART_MC_LMS_Pos     (4U)
#define UART_MC_LMS_Msk     (0x1U << UART_MC_LMS_Pos) /*!< 0x00000010 */
#define UART_MC_LMS         UART_MC_LMS_Msk           /*!< Loopback mode enable.When UART is set in UART loopback mode, UTXDn/URXDn/UCTSn/URTSn pins are not used and UTXDn pin is set to inactive state.When UART is set in SIR loopback mode, IRDA_RXDL pin is not used but IRDA_TXDn pin is still in active state */

/* Bits definition for Window UART n Line Status register */
#define UART_LS_RDR_Pos  (0U)
#define UART_LS_RDR_Msk  (0x1U << UART_LS_RDR_Pos) /*!< 0x00000001 */
#define UART_LS_RDR      UART_LS_RDR_Msk           /*!< Receiver data ready flag */
#define UART_LS_OE_Pos   (1U)
#define UART_LS_OE_Msk   (0x1U << UART_LS_OE_Pos) /*!< 0x00000002 */
#define UART_LS_OE       UART_LS_OE_Msk           /*!< Overrun error flag */
#define UART_LS_PE_Pos   (2U)
#define UART_LS_PE_Msk   (0x1U << UART_LS_PE_Pos) /*!< 0x00000004 */
#define UART_LS_PE       UART_LS_PE_Msk           /*!< Parity error flag */
#define UART_LS_FE_Pos   (3U)
#define UART_LS_FE_Msk   (0x1U << UART_LS_FE_Pos) /*!< 0x00000008 */
#define UART_LS_FE       UART_LS_FE_Msk           /*!< Framing error flag */
#define UART_LS_BI_Pos   (4U)
#define UART_LS_BI_Msk   (0x1U << UART_LS_BI_Pos) /*!< 0x00000010 */
#define UART_LS_BI       UART_LS_BI_Msk           /*!< Break interrupt flag */
#define UART_LS_THRE_Pos (5U)
#define UART_LS_THRE_Msk (0x1U << UART_LS_THRE_Pos) /*!< 0x00000020 */
#define UART_LS_THRE     UART_LS_THRE_Msk           /*!< THR empty flag */
#define UART_LS_TEMT_Pos (6U)
#define UART_LS_TEMT_Msk (0x1U << UART_LS_TEMT_Pos) /*!< 0x00000040 */
#define UART_LS_TEMT     UART_LS_TEMT_Msk           /*!< Transmitter Empty flag */
#define UART_LS_RXFE_Pos (7U)
#define UART_LS_RXFE_Msk (0x1U << UART_LS_RXFE_Pos) /*!< 0x00000080 */
#define UART_LS_RXFE     UART_LS_RXFE_Msk           /*!< Receiver FIFO error flag */

/* Bits definition for Window UART n Modem Status register */
#define UART_MS_DCTS_Pos (0U)
#define UART_MS_DCTS_Msk (0x1U << UART_MS_DCTS_Pos) /*!< 0x00000001 */
#define UART_MS_DCTS     UART_MS_DCTS_Msk           /*!< Delta CTS */
#define UART_MS_CTS_Pos  (4U)
#define UART_MS_CTS_Msk  (0x1U << UART_MS_CTS_Pos) /*!< 0x00000010 */
#define UART_MS_CTS      UART_MS_CTS_Msk           /*!< Complement of CTS pin input signal */

/* Bits definition for Window UART n Scratch Pad register */
#define UART_SP_PID_Pos (0U)
#define UART_SP_PID_Msk (0xFFU << UART_SP_PID_Pos) /*!< 0x000000FF */
#define UART_SP_PID     UART_SP_PID_Msk            /*!< Pad information */

/* Bits definition for Window UART n Mode Definition register */
#define UART_MD_MODE_Pos      (0U)
#define UART_MD_MODE_Msk      (0x3U << UART_MD_MODE_Pos) /*!< 0x00000003 */
#define UART_MD_MODE          UART_MD_MODE_Msk           /*!< UART mode */
#define UART_MD_IRDAINVTX_Pos (6U)
#define UART_MD_IRDAINVTX_Msk (0x1U << UART_MD_IRDAINVTX_Pos) /*!< 0x00000040 */
#define UART_MD_IRDAINVTX     UART_MD_IRDAINVTX_Msk           /*!< IRDA SIR pulse invert */

/* Bits definition for Window UART n Auxiliary register */
#define UART_AUX_TXEN_Pos  (0U)
#define UART_AUX_TXEN_Msk  (0x1U << UART_AUX_TXEN_Pos) /*!< 0x00000001 */
#define UART_AUX_TXEN      UART_AUX_TXEN_Msk           /*!< IrDA TX enable */
#define UART_AUX_RXEN_Pos  (1U)
#define UART_AUX_RXEN_Msk  (0x1U << UART_AUX_RXEN_Pos) /*!< 0x00000002 */
#define UART_AUX_RXEN      UART_AUX_RXEN_Msk           /*!< IrDA RX enable */
#define UART_AUX_SIRPW_Pos (7U)
#define UART_AUX_SIRPW_Msk (0x1U << UART_AUX_SIRPW_Pos) /*!< 0x00000080 */
#define UART_AUX_SIRPW     UART_AUX_SIRPW_Msk           /*!< IRDA SIR pulse width */

/* Bits definition for Window UART n RX FIFO Count register */
#define UART_RXFFCNT_COUNT_Pos (0U)
#define UART_RXFFCNT_COUNT_Msk (0x1FU << UART_RXFFCNT_COUNT_Pos) /*!< 0x0000001F */
#define UART_RXFFCNT_COUNT     UART_RXFFCNT_COUNT_Msk            /*!< The number of data bytes in RX FIFO */

/******************************************************************************/
/* WWDT                                                                       */
/******************************************************************************/
/* Bits definition for Window Watchdog Key register */
#define WWDT_KEY_KEY_Pos (0U)
#define WWDT_KEY_KEY_Msk (0xFFFFU << WWDT_KEY_KEY_Pos) /*!< 0x0000FFFF */
#define WWDT_KEY_KEY     WWDT_KEY_KEY_Msk              /*!< WWDT key value */

/* Bits definition for Window Watchdog Prescaler register */
#define WWDT_PRESCALER_PR_Pos (0U)
#define WWDT_PRESCALER_PR_Msk (0x7U << WWDT_PRESCALER_PR_Pos) /*!< 0x00000007 */
#define WWDT_PRESCALER_PR     WWDT_PRESCALER_PR_Msk           /*!< WWDT prescaler divider */

/* Bits definition for Window Watchdog Lock Status register */
#define WWDT_LOCKST_RELOAD_LOCK_Pos    (0U)
#define WWDT_LOCKST_RELOAD_LOCK_Msk    (0x1U << WWDT_LOCKST_RELOAD_LOCK_Pos) /*!< 0x00000001 */
#define WWDT_LOCKST_RELOAD_LOCK        WWDT_LOCKST_RELOAD_LOCK_Msk           /*!< WWDT counter reload value update */
#define WWDT_LOCKST_PRESCALER_LOCK_Pos (1U)
#define WWDT_LOCKST_PRESCALER_LOCK_Msk (0x1U << WWDT_LOCKST_PRESCALER_LOCK_Pos) /*!< 0x00000002 */
#define WWDT_LOCKST_PRESCALER_LOCK     WWDT_LOCKST_PRESCALER_LOCK_Msk           /*!< WWDT prescaler value update */

/* Bits definition for Window Watchdog  Interrupt Status register */
#define WWDT_RIS_OVF_Pos (0U)
#define WWDT_RIS_OVF_Msk (0x1U << WWDT_RIS_OVF_Pos) /*!< 0x00000001 */
#define WWDT_RIS_OVF     WWDT_RIS_OVF_Msk           /*!< Whether WWDT counter reaches 0 or not */
#define WWDT_RIS_UDF_Pos (1U)
#define WWDT_RIS_UDF_Msk (0x1U << WWDT_RIS_UDF_Pos) /*!< 0x00000002 */
#define WWDT_RIS_UDF     WWDT_RIS_UDF_Msk           /*!< Whether WWDT enters underflow state or not */

/* Bits definition for Window Watchdog Control register */
#define WWDT_CTRL_WWDTEN_Pos (0U)
#define WWDT_CTRL_WWDTEN_Msk (0x1U << WWDT_CTRL_WWDTEN_Pos) /*!< 0x00000001 */
#define WWDT_CTRL_WWDTEN     WWDT_CTRL_WWDTEN_Msk           /*!< WWDT enable bit */
#define WWDT_CTRL_IE_Pos     (1U)
#define WWDT_CTRL_IE_Msk     (0x1U << WWDT_CTRL_IE_Pos) /*!< 0x00000002 */
#define WWDT_CTRL_IE         WWDT_CTRL_IE_Msk           /*!< WWDT timeout (OVF) interrupt enable bit */
#define WWDT_CTRL_RSTEN_Pos  (2U)
#define WWDT_CTRL_RSTEN_Msk  (0x1U << WWDT_CTRL_RSTEN_Pos) /*!< 0x00000004 */
#define WWDT_CTRL_RSTEN      WWDT_CTRL_RSTEN_Msk           /*!< WWDT reset enable bit */
#define WWDT_CTRL_UDFIE_Pos  (6U)
#define WWDT_CTRL_UDFIE_Msk  (0x1U << WWDT_CTRL_UDFIE_Pos) /*!< 0x00000014 */
#define WWDT_CTRL_UDFIE      WWDT_CTRL_UDFIE_Msk           /*!< WWDT UDF interrupt enable bit */
#define WWDT_CTRL_UDFEN_Pos  (7U)
#define WWDT_CTRL_UDFEN_Msk  (0x1U << WWDT_CTRL_UDFEN_Pos) /*!< 0x00000018 */
#define WWDT_CTRL_UDFEN      WWDT_CTRL_UDFEN_Msk           /*!< WWDT underflow function enable bit */

/* Bits definition for Window Watchdog Interrupt Length register */
#define WWDT_INTL_LEN_Pos (0U)
#define WWDT_INTL_LEN_Msk (0xFFU << WWDT_INTL_LEN_Pos) /*!< 0x000000FF */
#define WWDT_INTL_LEN     WWDT_INTL_LEN_Msk            /*!< The duration of the assertion of reset and interrupt. These bits are written access protected */

/* Bits definition for Window Watchdog Status register */
#define WWDT_STATUS_IS_Pos    (0U)
#define WWDT_STATUS_IS_Msk    (0x1U << WWDT_STATUS_IS_Pos) /*!< 0x00000001 */
#define WWDT_STATUS_IS        WWDT_STATUS_IS_Msk           /*!< Interrupt status */
#define WWDT_STATUS_UDFST_Pos (16U)
#define WWDT_STATUS_UDFST_Msk (0x1U << WWDT_STATUS_UDFST_Pos) /*!< 0x00010000 */
#define WWDT_STATUS_UDFST     WWDT_STATUS_UDFST_Msk           /*!< Underflow status */
#define WWDT_STATUS_OVFST_Pos (24U)
#define WWDT_STATUS_OVFST_Msk (0x1U << WWDT_STATUS_OVFST_Pos) /*!< 0x01000000 */
#define WWDT_STATUS_OVFST     WWDT_STATUS_OVFST_Msk           /*!< Overflow status */

#ifdef __cplusplus
}
#endif

#endif /* _ll_Reg_Msk_H_ */
