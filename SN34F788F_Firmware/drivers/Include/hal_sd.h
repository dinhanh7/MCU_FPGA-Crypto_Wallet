/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _HAL_SD_H_
#define _HAL_SD_H_

/* Includes ------------------------------------------------------------------*/
#include "ll_sd.h"

/**
 * @brief  SD Card IRQ Event Flag
 * \ingroup sd_struct_define
 */
typedef struct
{
    //	uint8_t	CommandComplete	;
    //	uint8_t	TransferComplete	;
    uint8_t BlockGap;       /*!< Block gap event interrupt flag             */
    uint8_t DMA;            /*!< DMA interrupt flag                         */
    uint8_t BufferWrite;    /*!< Buffer write ready interrupt flag          */
    uint8_t BufferRead;     /*!< Buffer raed ready interrupt flag           */
    uint8_t CardInsert;     /*!< Card insertion interrupt flag              */
    uint8_t CardRemove;     /*!< Card remove interrupt flag                 */
    uint8_t CardInterrupt;  /*!< Card interrupt flag                        */
    uint8_t Error;          /*!< Error Interrupt                            */
    uint8_t CommandTimeout; /*!< Command timeout error interrupt flag       */
    uint8_t CommandCRC;     /*!< Command CRC error interrupt flag           */
    uint8_t CommandEndBit;  /*!< Command end bit error interrupt flag       */
    uint8_t CommandIndex;   /*!< Command index error interrupt flag         */
    uint8_t DataTimeout;    /*!< Data timeout error interrupt flag          */
    uint8_t DataCRC;        /*!< Data CRC error interrupt flag              */
    uint8_t DataEndBit;     /*!< Data end bit error interrupt flag          */
    uint8_t CurrentLimit;   /*!< Current limit error interrupt flag         */
    uint8_t AutoCMDError;   /*!< Auto CMD12 error interrupt flag            */
    uint8_t ADMAError;      /*!< ADMA error interrupt flag                  */
} SD_Event_Flag_T;

/**
 * @brief  SD Card Information Structure definition
 * \ingroup sd_struct_define
 */
typedef struct
{
    uint32_t CardType; /*!< Specifies the card Type                            */

    uint32_t CardVersion; /*!< Specifies the card version                      */

    uint32_t Class; /*!< Specifies the class of the card class                 */

    uint32_t RelCardAdd; /*!< Specifies the Relative Card Address              */

    uint32_t BlockNbr; /*!< Specifies the Card Capacity in blocks              */

    uint32_t BlockSize; /*!< Specifies one block size in bytes                 */

    uint32_t LogBlockNbr; /*!< Specifies the Card logical Capacity in blocks   */

    uint32_t LogBlockSize; /*!< Specifies logical block size in bytes          */

    uint32_t CardSpeed; /*!< Specifies the card Speed                          */
#if (configUSE_SD_AUTOCMD_ENABLE == 1)
    uint32_t Cmd23Support; /*!< Specifies the card Speed                       */
#endif

} HAL_SD_CardInfoTypeDef;

/**
 * @brief  Card Specific Data: CSD Register
 * \ingroup sd_struct_define
 */
typedef struct
{
    __IO uint8_t  CSDStruct;           /*!< CSD structure                         */
    __IO uint8_t  SysSpecVersion;      /*!< System specification version          */
    __IO uint8_t  Reserved1;           /*!< Reserved                              */
    __IO uint8_t  TAAC;                /*!< Data read access time 1               */
    __IO uint8_t  NSAC;                /*!< Data read access time 2 in CLK cycles */
    __IO uint8_t  MaxBusClkFrec;       /*!< Max. bus clock frequency              */
    __IO uint16_t CardComdClasses;     /*!< Card command classes                  */
    __IO uint8_t  RdBlockLen;          /*!< Max. read data block length           */
    __IO uint8_t  PartBlockRead;       /*!< Partial blocks for read allowed       */
    __IO uint8_t  WrBlockMisalign;     /*!< Write block misalignment              */
    __IO uint8_t  RdBlockMisalign;     /*!< Read block misalignment               */
    __IO uint8_t  DSRImpl;             /*!< DSR implemented                       */
    __IO uint8_t  Reserved2;           /*!< Reserved                              */
    __IO uint32_t DeviceSize;          /*!< Device Size                           */
    __IO uint8_t  MaxRdCurrentVDDMin;  /*!< Max. read current @ VDD min           */
    __IO uint8_t  MaxRdCurrentVDDMax;  /*!< Max. read current @ VDD max           */
    __IO uint8_t  MaxWrCurrentVDDMin;  /*!< Max. write current @ VDD min          */
    __IO uint8_t  MaxWrCurrentVDDMax;  /*!< Max. write current @ VDD max          */
    __IO uint8_t  DeviceSizeMul;       /*!< Device size multiplier                */
    __IO uint8_t  EraseGrSize;         /*!< Erase group size                      */
    __IO uint8_t  EraseGrMul;          /*!< Erase group size multiplier           */
    __IO uint8_t  WrProtectGrSize;     /*!< Write protect group size              */
    __IO uint8_t  WrProtectGrEnable;   /*!< Write protect group enable            */
    __IO uint8_t  ManDeflECC;          /*!< Manufacturer default ECC              */
    __IO uint8_t  WrSpeedFact;         /*!< Write speed factor                    */
    __IO uint8_t  MaxWrBlockLen;       /*!< Max. write data block length          */
    __IO uint8_t  WriteBlockPaPartial; /*!< Partial blocks for write allowed      */
    __IO uint8_t  Reserved3;           /*!< Reserved                              */
    __IO uint8_t  ContentProtectAppli; /*!< Content protection application        */
    __IO uint8_t  FileFormatGroup;     /*!< File format group                     */
    __IO uint8_t  CopyFlag;            /*!< Copy flag (OTP)                       */
    __IO uint8_t  PermWrProtect;       /*!< Permanent write protection            */
    __IO uint8_t  TempWrProtect;       /*!< Temporary write protection            */
    __IO uint8_t  FileFormat;          /*!< File format                           */
    __IO uint8_t  ECC;                 /*!< ECC code                              */
    __IO uint8_t  CSD_CRC;             /*!< CSD CRC                               */
    __IO uint8_t  Reserved4;           /*!< Always 1                              */
} HAL_SD_CardCSDTypeDef;

/**
 * @brief  Card Identification Data: CID Register
 * \ingroup sd_struct_define
 */
typedef struct
{
    __IO uint8_t  ManufacturerID; /*!< Manufacturer ID       */
    __IO uint16_t OEM_AppliID;    /*!< OEM/Application ID    */
    __IO uint32_t ProdName1;      /*!< Product Name part1    */
    __IO uint8_t  ProdName2;      /*!< Product Name part2    */
    __IO uint8_t  ProdRev;        /*!< Product Revision      */
    __IO uint32_t ProdSN;         /*!< Product Serial Number */
    __IO uint8_t  Reserved1;      /*!< Reserved1             */
    __IO uint16_t ManufactDate;   /*!< Manufacturing Date    */
    __IO uint8_t  CID_CRC;        /*!< CID CRC               */
    __IO uint8_t  Reserved2;      /*!< Always 1              */

} HAL_SD_CardCIDTypeDef;

/**
 * @brief  SD Card Status returned by ACMD13
 * \ingroup sd_struct_define
 */
typedef struct
{
    __IO uint8_t  DataBusWidth;          /*!< Shows the currently defined data bus width                 */
    __IO uint8_t  SecuredMode;           /*!< Card is in secured mode of operation                       */
    __IO uint16_t CardType;              /*!< Carries information about card type                        */
    __IO uint32_t ProtectedAreaSize;     /*!< Carries information about the capacity of protected area   */
    __IO uint8_t  SpeedClass;            /*!< Carries information about the speed class of the card      */
    __IO uint8_t  PerformanceMove;       /*!< Carries information about the card's performance move      */
    __IO uint8_t  AllocationUnitSize;    /*!< Carries information about the card's allocation unit size  */
    __IO uint16_t EraseSize;             /*!< Determines the number of AUs to be erased in one operation */
    __IO uint8_t  EraseTimeout;          /*!< Determines the timeout for any number of AU erase          */
    __IO uint8_t  EraseOffset;           /*!< Carries information about the erase offset                 */
    __IO uint8_t  UhsSpeedGrade;         /*!< Carries information about the speed grade of UHS card      */
    __IO uint8_t  UhsAllocationUnitSize; /*!< Carries information about the UHS card's allocation unit size  */
    __IO uint8_t  VideoSpeedClass;       /*!< Carries information about the Video Speed Class of UHS card    */

} HAL_SD_CardStatusTypeDef;

/**
 * @brief  SD handle Structure definition
 * \ingroup sd_struct_define
 */
#if defined(configUSE_SD_REGISTER_CALLBACKS) && (configUSE_SD_REGISTER_CALLBACKS == 1U)
typedef struct __SD_Handle_T
#else
typedef struct
#endif /* configUSE_SD_REGISTER_CALLBACKS */
{
    SN_SDIO_Type          *instance;   /*!< SD registers base address           */
    SD_Init_T              Init;       /*!< SD required parameters              */
    __IO int32_t           mutex;      /*!< ADC protected process   */
    uint8_t               *pTxBuffPtr; /*!< Pointer to SD Tx transfer Buffer    */
    uint32_t               TxXferSize; /*!< SD Tx Transfer size                 */
    uint8_t               *pRxBuffPtr; /*!< Pointer to SD Rx transfer Buffer    */
    uint32_t               RxXferSize; /*!< SD Rx Transfer size                 */
    __IO uint32_t          Context;    /*!< SD transfer context                 */
    __IO HAL_State_T       state;      /*!< SD card State                       */
    __IO uint32_t          ErrorCode;  /*!< SD Card Error codes                 */
    HAL_SD_CardInfoTypeDef SdCard;     /*!< SD Card information                 */
    uint32_t               CSD[4];     /*!< SD card specific data table         */
    uint32_t               CID[4];     /*!< SD card identification number table */
    uint32_t               SCR[2];     /*!< SD card Configuration Register value */
    SD_Event_Flag_T        EventFlag;

#if (configUSE_SD_REGISTER_CALLBACKS == 1)
    void (*TxCpltCallback)(struct __SD_Handle_T *hsd);    /*!< The register callback function for SD transfer done */
    void (*RxCpltCallback)(struct __SD_Handle_T *hsd);    /*!< The register callback function for SD recevie done */
    void (*ErrorCallback)(struct __SD_Handle_T *hsd);     /*!< The register callback function for SD error notify */
    void (*AbortCpltCallback)(struct __SD_Handle_T *hsd); /*!< The register callback function for SD abort current transfer */

    void (*MspInitCallback)(struct __SD_Handle_T *hsd);   /*!< SD card MSP init callback function */
    void (*MspDeInitCallback)(struct __SD_Handle_T *hsd); /*!< SD card MSP deinit callback function */
#endif                                                    /* configUSE_SD_REGISTER_CALLBACKS */
} SD_Handle_T;

#if (configUSE_SD_REGISTER_CALLBACKS == 1)
/**
 * @brief  HAL SD Callback ID enumeration definition
 * \ingroup sd_struct_define
 */
typedef enum
{
    HAL_SD_TX_CPLT_CB_ID = 0x00U, /*!< SD Tx Complete Callback ID                     */
    HAL_SD_RX_CPLT_CB_ID = 0x01U, /*!< SD Rx Complete Callback ID                     */
    HAL_SD_ERROR_CB_ID   = 0x02U, /*!< SD Error Callback ID                           */
    HAL_SD_ABORT_CB_ID   = 0x03U, /*!< SD Abort Callback ID                           */

    HAL_SD_MSP_INIT_CB_ID   = 0x10U, /*!< SD MspInit Callback ID                         */
    HAL_SD_MSP_DEINIT_CB_ID = 0x11U  /*!< SD MspDeInit Callback ID                       */
} HAL_SD_CallbackIDTypeDef;

/* Callback in non blocking modes (DMA) */
void HAL_SD_TxCpltCallback(SD_Handle_T *hsd);
void HAL_SD_RxCpltCallback(SD_Handle_T *hsd);
void HAL_SD_ErrorCallback(SD_Handle_T *hsd);
void HAL_SD_AbortCallback(SD_Handle_T *hsd);

/**
 *   SD Callback pointer definition
 */
typedef void (*pSD_Callback_T)(SD_Handle_T *hsd);

#endif /* configUSE_SD_REGISTER_CALLBACKS */

#define BLOCKSIZE 512 /*!< Block size is 512 bytes */

/**
 * @brief  SD Error status enumeration Structure definition
 * \ingroup sd_error_code
 * @{
 */
#define HAL_SD_ERROR_NONE            SDMMC_ERROR_NONE                          /*!< No error                                                       */
#define HAL_SD_ERROR_CMD_CRC_FAIL    SDMMC_ERROR_CMD_CRC_FAIL                  /*!< Command response received (but CRC check failed)               */
#define HAL_SD_ERROR_DATA_CRC_FAIL   SDMMC_ERROR_DATA_CRC_FAIL                 /*!< Data block sent/received (CRC check failed)                    */
#define HAL_SD_ERROR_CMD_RSP_TIMEOUT SDMMC_ERROR_CMD_RSP_TIMEOUT               /*!< Command response timeout                                       */
#define HAL_SD_ERROR_DATA_TIMEOUT    SDMMC_ERROR_DATA_TIMEOUT                  /*!< Data timeout                                                   */
#define HAL_SD_ERROR_TX_UNDERRUN     SDMMC_ERROR_TX_UNDERRUN                   /*!< Transmit FIFO underrun                                         */
#define HAL_SD_ERROR_RX_OVERRUN      SDMMC_ERROR_RX_OVERRUN                    /*!< Receive FIFO overrun                                           */
#define HAL_SD_ERROR_ADDR_MISALIGNED SDMMC_ERROR_ADDR_MISALIGNED               /*!< Misaligned address                                             */
#define HAL_SD_ERROR_BLOCK_LEN_ERR   SDMMC_ERROR_BLOCK_LEN_ERR                 /*!< Transferred block length is not allowed for the card or the \
                                                                                    number of transferred bytes does not match the block length    */
#define HAL_SD_ERROR_ERASE_SEQ_ERR        SDMMC_ERROR_ERASE_SEQ_ERR            /*!< An error in the sequence of erase command occurs               */
#define HAL_SD_ERROR_BAD_ERASE_PARAM      SDMMC_ERROR_BAD_ERASE_PARAM          /*!< An invalid selection for erase groups                          */
#define HAL_SD_ERROR_WRITE_PROT_VIOLATION SDMMC_ERROR_WRITE_PROT_VIOLATION     /*!< Attempt to program a write protect block                       */
#define HAL_SD_ERROR_LOCK_UNLOCK_FAILED   SDMMC_ERROR_LOCK_UNLOCK_FAILED       /*!< Sequence or password error has been detected in unlock \
                                                                                    command or if there was an attempt to access a locked card     */
#define HAL_SD_ERROR_COM_CRC_FAILED       SDMMC_ERROR_COM_CRC_FAILED           /*!< CRC check of the previous command failed                       */
#define HAL_SD_ERROR_ILLEGAL_CMD          SDMMC_ERROR_ILLEGAL_CMD              /*!< Command is not legal for the card state                        */
#define HAL_SD_ERROR_CARD_ECC_FAILED      SDMMC_ERROR_CARD_ECC_FAILED          /*!< Card internal ECC was applied but failed to correct the data   */
#define HAL_SD_ERROR_CC_ERR               SDMMC_ERROR_CC_ERR                   /*!< Internal card controller error                                 */
#define HAL_SD_ERROR_GENERAL_UNKNOWN_ERR  SDMMC_ERROR_GENERAL_UNKNOWN_ERR      /*!< General or unknown error                                       */
#define HAL_SD_ERROR_STREAM_READ_UNDERRUN SDMMC_ERROR_STREAM_READ_UNDERRUN     /*!< The card could not sustain data reading in stream rmode        */
#define HAL_SD_ERROR_STREAM_WRITE_OVERRUN SDMMC_ERROR_STREAM_WRITE_OVERRUN     /*!< The card could not sustain data programming in stream mode     */
#define HAL_SD_ERROR_CID_CSD_OVERWRITE    SDMMC_ERROR_CID_CSD_OVERWRITE        /*!< CID/CSD overwrite error                                        */
#define HAL_SD_ERROR_WP_ERASE_SKIP        SDMMC_ERROR_WP_ERASE_SKIP            /*!< Only partial address space was erased                          */
#define HAL_SD_ERROR_CARD_ECC_DISABLED    SDMMC_ERROR_CARD_ECC_DISABLED        /*!< Command has been executed without using internal ECC           */
#define HAL_SD_ERROR_ERASE_RESET          SDMMC_ERROR_ERASE_RESET              /*!< Erase sequence was cleared before executing because an out \
                                                                                    of erase sequence command was received                         */
#define HAL_SD_ERROR_AKE_SEQ_ERR            SDMMC_ERROR_AKE_SEQ_ERR            /*!< Error in sequence of authentication                            */
#define HAL_SD_ERROR_INVALID_VOLTRANGE      SDMMC_ERROR_INVALID_VOLTRANGE      /*!< Error in case of invalid voltage range                         */
#define HAL_SD_ERROR_ADDR_OUT_OF_RANGE      SDMMC_ERROR_ADDR_OUT_OF_RANGE      /*!< Error when addressed block is out of range                     */
#define HAL_SD_ERROR_REQUEST_NOT_APPLICABLE SDMMC_ERROR_REQUEST_NOT_APPLICABLE /*!< Error when command request is not applicable                   */
#define HAL_SD_ERROR_PARAM                  SDMMC_ERROR_INVALID_PARAMETER      /*!< the used parameter is not valid                                */
#define HAL_SD_ERROR_UNSUPPORTED_FEATURE    SDMMC_ERROR_UNSUPPORTED_FEATURE    /*!< Error when feature is not insupported                          */
#define HAL_SD_ERROR_BUSY                   SDMMC_ERROR_BUSY                   /*!< Error when transfer process is busy                            */
#define HAL_SD_ERROR_DMA                    SDMMC_ERROR_DMA                    /*!< Error while DMA transfer                                       */
#define HAL_SD_ERROR_TIMEOUT                SDMMC_ERROR_TIMEOUT                /*!< Timeout error                                                  */
#define HAL_SD_ERROR_CMD_END_BIT            SDMMC_ERROR_CMD_END_BIT            /*!< The end bif of the comand response is '0'                      */
#define HAL_SD_ERROR_CMD_INDEX              SDMMC_ERROR_CMD_INDEX              /*!< The comand index in the response is different from the command */
#define HAL_SD_ERROR_DATA_END_BIT           SDMMC_ERROR_DATA_END_BIT           /*!< The end bif of the read data is '0'                            */
#define HAL_SD_ERROR_AUTO_CMD               SDMMC_ERROR_AUTO_CMD               /*!< The end bif of the read data is '0'                            */
#define HAL_SD_ERROR_ADMA                   SDMMC_ERROR_ADMA                   /*!< ADMA ERROR                                                     */

#if defined(configUSE_SD_REGISTER_CALLBACKS) && (configUSE_SD_REGISTER_CALLBACKS == 1U)
#define HAL_SD_ERROR_INVALID_CALLBACK SDMMC_ERROR_INVALID_PARAMETER /*!< Invalid callback error                                        */
#endif
/* configUSE_SD_REGISTER_CALLBACKS */
/**
 * @}
 */

/**
 * \brief SD_VER SD Supported Version
 * \ingroup SD_control
 * @{
 */
#define CARD_V1_X 0x00000000U /**< SD Version 1.X */
#define CARD_V2_X 0x00000001U /**< SD Version 2.X */
/**
 * @}
 */

/* Exported macro ------------------------------------------------------------*/
/**
 * @brief  Enable the SD device interrupt.
 * @param  __HANDLE__: SD Handle
 * @param  __INTERRUPT__: specifies the SDMMC interrupt sources to be enabled.
 *         This parameter can be one or a combination of the following values:
 *            @arg SDMMC_IT_CMDCPLIE: Command complete interrupt enable
 *            @arg SDMMC_IT_TRANSCPLIE: Transfer complete interrupt enable
 *            @arg SDMMC_IT_BLKGAPIE: Block gap event interrupt enable
 *            @arg SDMMC_IT_DMAIE: DMA interrupt enable
 *            @arg SDMMC_IT_BUFWIE: Buffer write ready interrupt enable
 *            @arg SDMMC_IT_BUFRIE: Buffer raed ready interrupt enable
 *            @arg SDMMC_IT_CARDINSTIE: Card insertion interrupt enable
 *            @arg SDMMC_IT_CARDRMIE: Card remove interrupt enable
 *            @arg SDMMC_IT_CARDIE: Card interrupt enable
 *            @arg SDMMC_IT_CMDTOEIE: Command timeout error interrupt enable
 *            @arg SDMMC_IT_CMDCRCEIE: Command CRC error interrupt enable
 *            @arg SDMMC_IT_CMDEBEIE: Command end bit error interrupt enable
 *            @arg SDMMC_IT_CMDINDEIE: Command index error interrupt enable
 *            @arg SDMMC_IT_DATTOEIE: Data timeout error interrupt enable
 *            @arg SDMMC_IT_DATCRCEIE: Data CRC error interrupt enable
 *            @arg SDMMC_IT_DATEBEIE: Data end bit error interrupt enable
 *            @arg SDMMC_IT_CURLMTEIE: Current limit error interrupt enable
 *            @arg SDMMC_IT_ATCMDEIE: Auto CMD12 error interrupt enable
 *            @arg SDMMC_IT_ADMAEIE: ADMA error interrupt enable
 * @retval None
 * @{
 */
#define __HAL_SD_ENABLE_IT(__HANDLE__, __INTERRUPT__) __SDMMC_ENABLE_IT((__HANDLE__)->Instance, (__INTERRUPT__)) /**< Enable the SD device interrupt */
/**
 * @}
 */

/**
 * \defgroup SD_SPPED SD Speed
 * \ingroup sd_control
 * @{
 */
#define SDMMC_INIT_CLK_DIV          ((uint8_t)0x78) /**< SD bus speed 400k */
#define SDMMC_TRANSFER_CLK_DIV      ((uint8_t)0x2)  /**< SD bus speed 25MHz */
#define SDMMC_HIGH_TRANSFER_CLK_DIV ((uint8_t)0x01) /**< SD bus speed 50MHz */

#define SDMMC_INIT_SPEED         (400 * 1000)       /**< SD bus speed 400k */
#define SDMMC_TRANSFER_SPEED     (24 * 1000 * 1000) /**< SD bus speed 25MHz */
#define SDMMC_HIGH_TRANSFER_PEED (48 * 1000 * 1000) /**< SD bus speed 50MHz */
/**
 * @}
 */

/**
 * \defgroup SD_BUS_WIDE SD Bus Wide
 * \ingroup sd_control
 * @{
 */
#define SDMMC_BUS_WIDE_1B 0x00000000U /**< SD bus wide 1 bit */
#define SDMMC_BUS_WIDE_4B 0x00000800U /**< SD bus wide 4 bit */
#define SDMMC_BUS_WIDE_8B 0x00001000U /**< SD bus wide 8 bit */
/**
 * @}
 */

/**
 * \defgroup SD_BUS_WIDE_check SD Bus Wide check
 * \ingroup sd_control
 * @{
 */
#define IS_SDMMC_BUS_WIDE(WIDE) (((WIDE) == SDMMC_BUS_WIDE_1B) || \
                                 ((WIDE) == SDMMC_BUS_WIDE_4B) || \
                                 ((WIDE) == SDMMC_BUS_WIDE_8B)) /**< Check the SD bus wide */
/**
 * @}
 */

/**
 * \defgroup SD_SPPED_MODE SD Speed Mode
 * \ingroup sd_control
 * @{
 */
#define SDMMC_SPEED_MODE_AUTO    ((uint32_t)0x00000000U) /**< SD Speed Mode Auto */
#define SDMMC_SPEED_MODE_DEFAULT ((uint32_t)0x00000001U) /**< SD Speed Mode Default */
#define SDMMC_SPEED_MODE_HIGH    ((uint32_t)0x00000002U) /**< SD Speed Mode high */
#define SDMMC_SPEED_MODE_ULTRA   ((uint32_t)0x00000003U) /**< SD Speed Mode ultra */
#define SDMMC_SPEED_MODE_DDR     ((uint32_t)0x00000004U) /**< SD Speed Mode ddr */
/**
 * @}
 */

/**
 * \defgroup SD_SPPED_MODE_check SD Speed Mode check
 * \ingroup sd_control
 * @{
 */
#define IS_SDMMC_SPEED_MODE(MODE) (((MODE) == SDMMC_SPEED_MODE_AUTO) ||    \
                                   ((MODE) == SDMMC_SPEED_MODE_DEFAULT) || \
                                   ((MODE) == SDMMC_SPEED_MODE_HIGH) ||    \
                                   ((MODE) == SDMMC_SPEED_MODE_ULTRA) ||   \
                                   ((MODE) == SDMMC_SPEED_MODE_DDR)) /**< Check the SD Speed Mode */
/**
 * @}
 */

/**
 * @brief  SD Card State enumeration structure
 * \ingroup sd_struct_define
 */
typedef uint32_t HAL_SD_CardState_T;

/**
 * \brief SD_Card_State SD Card State
 * \ingroup SD_control
 * @{
 */
#define HAL_SD_CARD_READY          0x00000001U /*!< Card state is ready                     */
#define HAL_SD_CARD_IDENTIFICATION 0x00000002U /*!< Card is in identification state         */
#define HAL_SD_CARD_STANDBY        0x00000003U /*!< Card is in standby state                */
#define HAL_SD_CARD_TRANSFER       0x00000004U /*!< Card is in transfer state               */
#define HAL_SD_CARD_SENDING        0x00000005U /*!< Card is sending an operation            */
#define HAL_SD_CARD_RECEIVING      0x00000006U /*!< Card is receiving operation information */
#define HAL_SD_CARD_PROGRAMMING    0x00000007U /*!< Card is in programming state            */
#define HAL_SD_CARD_DISCONNECTED   0x00000008U /*!< Card is disconnected                    */
#define HAL_SD_CARD_ERROR          0x000000FFU /*!< Card response Error                     */
/**
 * @}
 */

/**
 * Input and Output operation functions
 */
/* Blocking mode: Polling */
HAL_Status_T HAL_SD_ReadBlocks(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout);
HAL_Status_T HAL_SD_WriteBlocks(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout);
HAL_Status_T HAL_SD_Erase(SD_Handle_T *hsd, uint32_t BlockStartAdd, uint32_t BlockEndAdd);
/* Non-Blocking mode: IT */
HAL_Status_T HAL_SD_ReadBlocks_IT(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
HAL_Status_T HAL_SD_WriteBlocks_IT(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
/*
 *Non-Blocking mode: DMA
 *Replace DMA With SDMA
 */
//  HAL_Status_T HAL_SD_ReadBlocks_DMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
//  HAL_Status_T HAL_SD_WriteBlocks_DMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
#define HAL_SD_ReadBlocks_SDMA  HAL_SD_ReadBlocks_DMA  /*!< Replace HAL_SD_ReadBlocks_DMA with HAL_SD_ReadBlocks_SDMA */
#define HAL_SD_WriteBlocks_SDMA HAL_SD_WriteBlocks_DMA /*!< Replace HAL_SD_WriteBlocks_DMA with HAL_SD_WriteBlocks_SDMA */
/* Non-Blocking mode: SDMA */
HAL_Status_T HAL_SD_ReadBlocks_SDMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
HAL_Status_T HAL_SD_WriteBlocks_SDMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
/* Non-Blocking mode: ADMA */
HAL_Status_T HAL_SD_ReadBlocks_ADMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
HAL_Status_T HAL_SD_WriteBlocks_ADMA(SD_Handle_T *hsd, uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);

void HAL_SD_IRQHandler(SD_Handle_T *hsd);

/* Callback in non blocking modes (DMA) */
void HAL_SD_TxCpltCallback(SD_Handle_T *hsd);
void HAL_SD_RxCpltCallback(SD_Handle_T *hsd);
void HAL_SD_ErrorCallback(SD_Handle_T *hsd);
// void HAL_SD_AbortCallback(SD_Handle_T *hsd);

#if defined(configUSE_SD_REGISTER_CALLBACKS) && (configUSE_SD_REGISTER_CALLBACKS == 1U)
/* SD callback registering/unregistering */
HAL_Status_T HAL_SD_RegisterCallback(SD_Handle_T *hsd, HAL_SD_CallbackIDTypeDef CallbackId, pSD_Callback_T pCallback);
HAL_Status_T HAL_SD_UnRegisterCallback(SD_Handle_T *hsd, HAL_SD_CallbackIDTypeDef CallbackId);
#endif /* configUSE_SD_REGISTER_CALLBACKS */

/**
 *  Peripheral Control functions
 */
HAL_Status_T HAL_SD_ConfigWideBusOperation(SD_Handle_T *hsd, uint32_t WideMode);
HAL_Status_T HAL_SD_ConfigSpeedBusOperation(SD_Handle_T *hsd, uint32_t SpeedMode);

/**
 *  SD card related functions
 */
// HAL_Status_T       HAL_SD_SendSDStatus(SD_Handle_T *hsd, uint32_t *pSDstatus);
HAL_SD_CardState_T HAL_SD_GetCardState(SD_Handle_T *hsd);
HAL_Status_T       HAL_SD_GetCardCID(SD_Handle_T *hsd, HAL_SD_CardCIDTypeDef *pCID);
HAL_Status_T       HAL_SD_GetCardCSD(SD_Handle_T *hsd, HAL_SD_CardCSDTypeDef *pCSD);
HAL_Status_T       HAL_SD_GetCardStatus(SD_Handle_T *hsd, HAL_SD_CardStatusTypeDef *pStatus);
HAL_Status_T       HAL_SD_GetCardInfo(SD_Handle_T *hsd, HAL_SD_CardInfoTypeDef *pCardInfo);

/**
 * Peripheral State and Errors functions
 */
HAL_State_T HAL_SD_GetState(SD_Handle_T *hsd);
uint32_t    HAL_SD_GetError(SD_Handle_T *hsd);

/**
 *  Peripheral Abort management
 */
// HAL_Status_T HAL_SD_Abort(SD_Handle_T *hsd);
// HAL_Status_T HAL_SD_Abort_IT(SD_Handle_T *hsd);

/**
 * Initialization and de-initialization functions
 */
HAL_Status_T HAL_SD_Init(SD_Handle_T *hsd);
HAL_Status_T HAL_SD_InitCard(SD_Handle_T *hsd);
HAL_Status_T HAL_SD_DeInit(SD_Handle_T *hsd);
void         HAL_SD_MspInit(SD_Handle_T *hsd);
void         HAL_SD_MspDeInit(SD_Handle_T *hsd);

/**
 * SD Card Detect functions
 */
#if (configUSE_SD_CARD_DETECT == 1)
uint32_t SD_CARD_DETECT_GPIO_Init(void);
uint8_t  HAL_SD_IsDetected(void);
#endif

#include "ll_sd.h"

#endif /* _HAL_SD_H_ */
