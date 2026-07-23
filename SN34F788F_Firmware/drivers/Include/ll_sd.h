/***********************
 * SN34F78X SDK V2.1.4 *
 ***********************/

#ifndef _LL_SD_H_
#define _LL_SD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sn34f78x_hal_def.h"

/**
 * @brief SD Configuration Structure definition
 * \ingroup sd_struct_define
 */
typedef struct
{
    uint32_t BusWide;  /*!< Specifies the SD bus width.
                            This parameter can be a value of @ref SD_BUS_WIDE                   */
    uint32_t ClockDiv; /*!< Specifies the clock frequency of the SD controller.
                            This parameter can be a value of @ref SD_SPPED                   */

} SD_Init_T;

/* Exported constants --------------------------------------------------------*/
/** @defgroup SDMMC_LL_Exported_Constants SDMMC_LL Exported Constants
 * @{
 */
#define SDMMC_ERROR_NONE            0U         /*!< No error                                                       */
#define SDMMC_ERROR_CMD_CRC_FAIL    1U         /*!< Command response received (but CRC check failed)               */
#define SDMMC_ERROR_DATA_CRC_FAIL   2U         /*!< Data block sent/received (CRC check failed)                    */
#define SDMMC_ERROR_CMD_RSP_TIMEOUT 3U         /*!< Command response timeout                                       */
#define SDMMC_ERROR_DATA_TIMEOUT    4U         /*!< Data timeout                                                   */
#define SDMMC_ERROR_TX_UNDERRUN     5U         /*!< Transmit FIFO underrun                                         */
#define SDMMC_ERROR_RX_OVERRUN      6U         /*!< Receive FIFO overrun                                           */
#define SDMMC_ERROR_ADDR_MISALIGNED 7U         /*!< Misaligned address                                             */
#define SDMMC_ERROR_BLOCK_LEN_ERR   8U         /*!< Transferred block length is not allowed for the card or the \
                                                    number of transferred bytes does not match the block length   */
#define SDMMC_ERROR_ERASE_SEQ_ERR        9U    /*!< An error in the sequence of erase command occurs               */
#define SDMMC_ERROR_BAD_ERASE_PARAM      10U   /*!< An invalid selection for erase groups                          */
#define SDMMC_ERROR_WRITE_PROT_VIOLATION 11U   /*!< Attempt to program a write protect block                       */
#define SDMMC_ERROR_LOCK_UNLOCK_FAILED   12U   /*!< Sequence or password error has been detected in unlock command \
                                                    or if there was an attempt to access a locked card    */
#define SDMMC_ERROR_COM_CRC_FAILED       13U   /*!< CRC check of the previous command failed                       */
#define SDMMC_ERROR_ILLEGAL_CMD          14U   /*!< Command is not legal for the card state                        */
#define SDMMC_ERROR_CARD_ECC_FAILED      15U   /*!< Card internal ECC was applied but failed to correct the data   */
#define SDMMC_ERROR_CC_ERR               16U   /*!< Internal card controller error                                 */
#define SDMMC_ERROR_GENERAL_UNKNOWN_ERR  17U   /*!< General or unknown error                                       */
#define SDMMC_ERROR_STREAM_READ_UNDERRUN 18U   /*!< The card could not sustain data reading in stream rmode        */
#define SDMMC_ERROR_STREAM_WRITE_OVERRUN 19U   /*!< The card could not sustain data programming in stream mode     */
#define SDMMC_ERROR_CID_CSD_OVERWRITE    20U   /*!< CID/CSD overwrite error                                        */
#define SDMMC_ERROR_WP_ERASE_SKIP        21U   /*!< Only partial address space was erased                          */
#define SDMMC_ERROR_CARD_ECC_DISABLED    22U   /*!< Command has been executed without using internal ECC           */
#define SDMMC_ERROR_ERASE_RESET          23U   /*!< Erase sequence was cleared before executing because an out of \
                                                    erase sequence command was received     */
#define SDMMC_ERROR_AKE_SEQ_ERR            24U /*!< Error in sequence of authentication                            */
#define SDMMC_ERROR_INVALID_VOLTRANGE      25U /*!< Error in case of invalid voltage range                         */
#define SDMMC_ERROR_ADDR_OUT_OF_RANGE      26U /*!< Error when addressed block is out of range                     */
#define SDMMC_ERROR_REQUEST_NOT_APPLICABLE 27U /*!< Error when command request is not applicable                   */
#define SDMMC_ERROR_INVALID_PARAMETER      28U /*!< the used parameter is not valid                                */
#define SDMMC_ERROR_UNSUPPORTED_FEATURE    29U /*!< Error when feature is not insupported                          */
#define SDMMC_ERROR_BUSY                   30U /*!< Error when transfer process is busy                            */
#define SDMMC_ERROR_DMA                    31U /*!< Error while DMA transfer                                       */
#define SDMMC_ERROR_TIMEOUT                32U /*!< Timeout error                                                  */
#define SDMMC_ERROR_CMD_END_BIT            33U /*!< The end bif of the comand response is '0'                      */
#define SDMMC_ERROR_CMD_INDEX              34U /*!< The comand index in the response is different from the command */
#define SDMMC_ERROR_DATA_END_BIT           35U /*!< The end bif of the read data is '0'                            */
#define SDMMC_ERROR_AUTO_CMD               36U /*!< The end bif of the read data is '0'                            */
#define SDMMC_ERROR_ADMA                   37U /*!< ADMA ERROR                                                     */

/**
 * @brief SDMMC Commands Index
 */
#define SDMMC_CMD_GO_IDLE_STATE     0U     /*!< Resets the SD memory card.                                                               */
#define SDMMC_CMD_SEND_OP_COND      1U     /*!< Sends host capacity support information and activates the card's initialization process. */
#define SDMMC_CMD_ALL_SEND_CID      2U     /*!< Asks any card connected to the host to send the CID numbers on the CMD line.             */
#define SDMMC_CMD_SET_REL_ADDR      3U     /*!< Asks the card to publish a new relative address (RCA).                                   */
#define SDMMC_CMD_SET_DSR           4U     /*!< Programs the DSR of all cards.                                                           */
#define SDMMC_CMD_SDMMC_SEN_OP_COND 5U     /*!< Sends host capacity support information (HCS) and asks the accessed card to send its \
                                                operating condition register (OCR) content in the response on the CMD line.              */
#define SDMMC_CMD_HS_SWITCH       6U       /*!< Checks switchable function (mode 0) and switch card function (mode 1).                   */
#define SDMMC_CMD_SEL_DESEL_CARD  7U       /*!< Selects the card by its own relative address and gets deselected by any other address    */
#define SDMMC_CMD_HS_SEND_EXT_CSD 8U       /*!< Sends SD Memory Card interface condition, which includes host supply voltage information \
                                                and asks the card whether card supports voltage.                                         */
#define SDMMC_CMD_SEND_CSD            9U   /*!< Addressed card sends its card specific data (CSD) on the CMD line.                       */
#define SDMMC_CMD_SEND_CID            10U  /*!< Addressed card sends its card identification (CID) on the CMD line.                      */
#define SDMMC_CMD_READ_DAT_UNTIL_STOP 11U  /*!< SD card doesn't support it.                                                              */
#define SDMMC_CMD_STOP_TRANSMISSION   12U  /*!< Forces the card to stop transmission.                                                    */
#define SDMMC_CMD_SEND_STATUS         13U  /*!< Addressed card sends its status register.                                                */
#define SDMMC_CMD_HS_BUSTEST_READ     14U  /*!< Reserved                                                                                 */
#define SDMMC_CMD_GO_INACTIVE_STATE   15U  /*!< Sends an addressed card into the inactive state.                                         */
#define SDMMC_CMD_SET_BLOCKLEN        16U  /*!< Sets the block length (in bytes for SDSC) for all following block commands     \
                                                (read, write, lock). Default block length is fixed to 512 Bytes. Not effective \
                                                for SDHS and SDXC.                                                                       */
#define SDMMC_CMD_READ_SINGLE_BLOCK 17U    /*!< Reads single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of \
                                                fixed 512 bytes in case of SDHC and SDXC.                                                */
#define SDMMC_CMD_READ_MULT_BLOCK 18U      /*!< Continuously transfers data blocks from card to host until interrupted by \
                                                STOP_TRANSMISSION command.                                                               */
#define SDMMC_CMD_HS_BUSTEST_WRITE     19U /*!< 64 bytes tuning pattern is sent for SDR50 and SDR104.                                    */
#define SDMMC_CMD_WRITE_DAT_UNTIL_STOP 20U /*!< Speed class control command.                                                             */
#define SDMMC_CMD_SET_BLOCK_COUNT      23U /*!< Specify block count for CMD18 and CMD25.                                                 */
#define SDMMC_CMD_WRITE_SINGLE_BLOCK   24U /*!< Writes single block of size selected by SET_BLOCKLEN in case of SDSC, and a block of \
                                                fixed 512 bytes in case of SDHC and SDXC.                                                */
#define SDMMC_CMD_WRITE_MULT_BLOCK   25U   /*!< Continuously writes blocks of data until a STOP_TRANSMISSION follows.                    */
#define SDMMC_CMD_PROG_CID           26U   /*!< Reserved for manufacturers.                                                              */
#define SDMMC_CMD_PROG_CSD           27U   /*!< Programming of the programmable bits of the CSD.                                         */
#define SDMMC_CMD_SET_WRITE_PROT     28U   /*!< Sets the write protection bit of the addressed group.                                    */
#define SDMMC_CMD_CLR_WRITE_PROT     29U   /*!< Clears the write protection bit of the addressed group.                                  */
#define SDMMC_CMD_SEND_WRITE_PROT    30U   /*!< Asks the card to send the status of the write protection bits.                           */
#define SDMMC_CMD_SD_ERASE_GRP_START 32U   /*!< Sets the address of the first write block to be erased. (For SD card only).              */
#define SDMMC_CMD_SD_ERASE_GRP_END   33U   /*!< Sets the address of the last write block of the continuous range to be erased.           */
#define SDMMC_CMD_ERASE_GRP_START    35U   /*!< Sets the address of the first write block to be erased. Reserved for each command \
                                                system set by switch function command (CMD6).                                            */
#define SDMMC_CMD_ERASE_GRP_END 36U        /*!< Sets the address of the last write block of the continuous range to be erased. \
                                                Reserved for each command system set by switch function command (CMD6).                  */
#define SDMMC_CMD_ERASE        38U         /*!< Reserved for SD security applications.                                                   */
#define SDMMC_CMD_FAST_IO      39U         /*!< SD card doesn't support it (Reserved).                                                   */
#define SDMMC_CMD_GO_IRQ_STATE 40U         /*!< SD card doesn't support it (Reserved).                                                   */
#define SDMMC_CMD_LOCK_UNLOCK  42U         /*!< Sets/resets the password or lock/unlock the card. The size of the data block is set by \
                                                the SET_BLOCK_LEN command.                                                               */
#define SDMMC_CMD_APP_CMD 55U              /*!< Indicates to the card that the next command is an application specific command rather \
                                                than a standard command.                                                                 */
#define SDMMC_CMD_GEN_CMD 56U              /*!< Used either to transfer a data block to the card or to get a data block from the card \
                                                for general purpose/application specific commands.                                       */
#define SDMMC_CMD_NO_CMD 64U               /*!< No command                                                                               */

/**
 * @brief Following commands are SD Card Specific commands.
 *        SDMMC_APP_CMD should be sent before sending these commands.
 */
#define SDMMC_CMD_APP_SD_SET_BUSWIDTH 6U           /*!< (ACMD6) Defines the data bus width to be used for data transfer. The allowed data bus \
                                                         widths are given in SCR register.                                                       */
#define SDMMC_CMD_SD_APP_STATUS                13U /*!< (ACMD13) Sends the SD status.                                                            */
#define SDMMC_CMD_SD_APP_SEND_NUM_WRITE_BLOCKS 22U /*!< (ACMD22) Sends the number of the written (without errors) write blocks. Responds with \
                                                        32bit+CRC data block.                                                                    */
#define SDMMC_CMD_SD_APP_OP_COND 41U               /*!< (ACMD41) Sends host capacity support information (HCS) and asks the accessed card to \
                                                        send its operating condition register (OCR) content in the response on the CMD line.     */
#define SDMMC_CMD_SD_APP_SET_CLR_CARD_DETECT 42U   /*!< (ACMD42) Connect/Disconnect the 50 KOhm pull-up resistor on CD/DAT3 (pin 1) of the card  */
#define SDMMC_CMD_SD_APP_SEND_SCR            51U   /*!< Reads the SD Configuration Register (SCR).                                               */
#define SDMMC_CMD_SDMMC_RW_DIRECT            52U   /*!< For SD I/O card only, reserved for security specification.                               */
#define SDMMC_CMD_SDMMC_RW_EXTENDED          53U   /*!< For SD I/O card only, reserved for security specification.                               */

/**
 * @brief Following commands are SD Card Specific security commands.
 *        SDMMC_CMD_APP_CMD should be sent before sending these commands.
 */
#define SDMMC_CMD_SD_APP_GET_MKB                     43U
#define SDMMC_CMD_SD_APP_GET_MID                     44U
#define SDMMC_CMD_SD_APP_SET_CER_RN1                 45U
#define SDMMC_CMD_SD_APP_GET_CER_RN2                 46U
#define SDMMC_CMD_SD_APP_SET_CER_RES2                47U
#define SDMMC_CMD_SD_APP_GET_CER_RES1                48U
#define SDMMC_CMD_SD_APP_SECURE_READ_MULTIPLE_BLOCK  18U
#define SDMMC_CMD_SD_APP_SECURE_WRITE_MULTIPLE_BLOCK 25U
#define SDMMC_CMD_SD_APP_SECURE_ERASE                38U
#define SDMMC_CMD_SD_APP_CHANGE_SECURE_AREA          49U
#define SDMMC_CMD_SD_APP_SECURE_WRITE_MKB            48U

/**
 * @brief  Masks for errors Card Status R1 (OCR Register)
 */
#define SDMMC_OCR_ADDR_OUT_OF_RANGE     0x80000000U
#define SDMMC_OCR_ADDR_MISALIGNED       0x40000000U
#define SDMMC_OCR_BLOCK_LEN_ERR         0x20000000U
#define SDMMC_OCR_ERASE_SEQ_ERR         0x10000000U
#define SDMMC_OCR_BAD_ERASE_PARAM       0x08000000U
#define SDMMC_OCR_WRITE_PROT_VIOLATION  0x04000000U
#define SDMMC_OCR_LOCK_UNLOCK_FAILED    0x01000000U
#define SDMMC_OCR_COM_CRC_FAILED        0x00800000U
#define SDMMC_OCR_ILLEGAL_CMD           0x00400000U
#define SDMMC_OCR_CARD_ECC_FAILED       0x00200000U
#define SDMMC_OCR_CC_ERROR              0x00100000U
#define SDMMC_OCR_GENERAL_UNKNOWN_ERROR 0x00080000U
#define SDMMC_OCR_STREAM_READ_UNDERRUN  0x00040000U
#define SDMMC_OCR_STREAM_WRITE_OVERRUN  0x00020000U
#define SDMMC_OCR_CID_CSD_OVERWRITE     0x00010000U
#define SDMMC_OCR_WP_ERASE_SKIP         0x00008000U
#define SDMMC_OCR_CARD_ECC_DISABLED     0x00004000U
#define SDMMC_OCR_ERASE_RESET           0x00002000U
#define SDMMC_OCR_AKE_SEQ_ERROR         0x00000008U
#define SDMMC_OCR_ERRORBITS             0xFDFFE008U

/**
 * @brief  Masks for R6 Response
 */
#define SDMMC_R6_GENERAL_UNKNOWN_ERROR 0x00002000U
#define SDMMC_R6_ILLEGAL_CMD           0x00004000U
#define SDMMC_R6_COM_CRC_FAILED        0x00008000U

#define SDMMC_VOLTAGE_WINDOW_SD     0x80100000U
#define SDMMC_HIGH_CAPACITY         0x40000000U
#define SDMMC_STD_CAPACITY          0x00000000U
#define SDMMC_CHECK_PATTERN         0x000001AAU
#define SD_SWITCH_1_8V_CAPACITY     0x01000000U
#define SDMMC_DDR50_SWITCH_PATTERN  0x80FFFF04U
#define SDMMC_SDR104_SWITCH_PATTERN 0x80FF1F03U
#define SDMMC_SDR50_SWITCH_PATTERN  0x80FF1F02U
#define SDMMC_SDR25_SWITCH_PATTERN  0x80FFFF01U

#define SDMMC_MAX_VOLT_TRIAL 0x0000FFFFU

#define SDMMC_MAX_TRIAL 0x0000FFFFU

#define SDMMC_ALLZERO 0x00000000U

#define SDMMC_WIDE_BUS_SUPPORT   0x00000400U
#define SDMMC_SINGLE_BUS_SUPPORT 0x00000100U
#define SDMMC_CARD_LOCKED        0x02000000U
#define SDMMC_CMD23_SUPPORT      0x02000000U

#ifndef SDMMC_DATATIMEOUT
#define SDMMC_DATATIMEOUT 0xFFFFFFFFU
#endif /* SDMMC_DATATIMEOUT */

#define SDMMC_0TO7BITS        0x000000FFU
#define SDMMC_8TO15BITS       0x0000FF00U
#define SDMMC_16TO23BITS      0x00FF0000U
#define SDMMC_24TO31BITS      0xFF000000U
#define SDMMC_MAX_DATA_LENGTH 0x01FFFFFFU

#define SDMMC_HALFFIFO      0x00000008U
#define SDMMC_HALFFIFOBYTES 0x00000020U

/**
 * @brief  Command Class supported
 */
#define SDMMC_CCCC_ERASE ((uint32_t)0x00000020U)

#define SDMMC_CMDTIMEOUT          ((uint32_t)5000)       /* Command send and response timeout - 64T     */
#define SDMMC_MAXERASETIMEOUT     ((uint32_t)63000U)     /* Max erase Timeout 63 s                */
#define SDMMC_STOPTRANSFERTIMEOUT ((uint32_t)100000000U) /* Timeout for STOP TRANSMISSION command */

/*
 * SDIO_LL_Present_State Present State
 */
#define SDMMC_STA_CMDINBCMD (1 << 0)  /* Command Inhibit (CMD) */
#define SDMMC_STA_CMDINBDAT (1 << 1)  /* Command Inhibit (DAT)*/
#define SDMMC_STA_DATACT    (1 << 2)  /* Data line active */
#define SDMMC_STA_WACT      (1 << 8)  /* Write transfer active */
#define SDMMC_STA_RACT      (1 << 9)  /* Read transfer active */
#define SDMMC_STA_BUFWEN    (1 << 10) /* Buffer write enable status */
#define SDMMC_STA_BUFREN    (1 << 11) /* Buffer read enable status */
#define SDMMC_STA_CARDINST  (1 << 16) /* Card inserted status */
#define SDMMC_STA_CARDSTB   (1 << 17) /* Card state stable */
#define SDMMC_STA_CDPINLV   (1 << 18) /* Card detect pin level */
#define SDMMC_STA_WPPINLV   (1 << 19) /* Write protect pin level */
#define SDMMC_STA_DAT0LV    (1 << 20) /* Data 0 line level */

/*
 * SDIO_LL_Interrupt_sources Interrupt Sources
 */
#define SDMMC_IT_CMDCPLIE   (1 << 0)  /* Command complete interrupt enable */
#define SDMMC_IT_TRANSCPLIE (1 << 1)  /* Transfer complete interrupt enable */
#define SDMMC_IT_BLKGAPIE   (1 << 2)  /* Block gap event interrupt enable */
#define SDMMC_IT_DMAIE      (1 << 3)  /* DMA interrupt enable */
#define SDMMC_IT_BUFWIE     (1 << 4)  /* Buffer write ready interrupt enable */
#define SDMMC_IT_BUFRIE     (1 << 5)  /* Buffer read ready interrupt enable */
#define SDMMC_IT_CARDINSTIE (1 << 6)  /* Card insertion interrupt enable */
#define SDMMC_IT_CARDRMIE   (1 << 7)  /* Card remove interrupt enable */
#define SDMMC_IT_CARDIE     (1 << 8)  /* Card interrupt enable */
#define SDMMC_IT_CMDTOEIE   (1 << 16) /* Command timeout error interrupt enable */
#define SDMMC_IT_CMDCRCEIE  (1 << 17) /* Command CRC error interrupt enable */
#define SDMMC_IT_CMDEBEIE   (1 << 18) /* Command end bit error interrupt enable */
#define SDMMC_IT_CMDINDEIE  (1 << 19) /* Command index error interrupt enable */
#define SDMMC_IT_DATTOEIE   (1 << 20) /* Data timeout error interrupt enable */
#define SDMMC_IT_DATCRCEIE  (1 << 21) /* Data CRC error interrupt enable */
#define SDMMC_IT_DATEBEIE   (1 << 22) /* Data end bit error interrupt enable */
#define SDMMC_IT_CURLMTEIE  (1 << 23) /* Current limit error interrupt enable */
#define SDMMC_IT_ATCMDEIE   (1 << 24) /* Auto CMD12 error interrupt enable */
#define SDMMC_IT_ADMAEIE    (1 << 25) /* ADMA error interrupt enable */

/**
 * SDMMC_LL_Interrupt_flags Interrupt Flags
 */
#define SDMMC_INTR_STS_ERR_ADMA         (1 << 25) /* ADMA error interrupt enable */
#define SDMMC_INTR_STS_ERR_AutoCMD      (1 << 24) /* Auto CMD12 error interrupt flag */
#define SDMMC_INTR_STS_ERR_CURR_LIMIT   (1 << 23) /* Current limit error interrupt flag */
#define SDMMC_INTR_STS_ERR_DATA_ENDBIT  (1 << 22) /* Data end bit error interrupt flag */
#define SDMMC_INTR_STS_ERR_DATA_CRC     (1 << 21) /* Data CRC error interrupt flag */
#define SDMMC_INTR_STS_ERR_DATA_TIMEOUT (1 << 20) /* Data timeout error interrupt flag */
#define SDMMC_INTR_STS_ERR_CMD_INDEX    (1 << 19) /* Command index error interrupt flag */
#define SDMMC_INTR_STS_ERR_CMD_ENDBIT   (1 << 18) /* Command end bit error interrupt flag */
#define SDMMC_INTR_STS_ERR_CMD_CRC      (1 << 17) /* Command CRC error interrupt flag */
#define SDMMC_INTR_STS_ERR_CMD_TIMEOUT  (1 << 16) /* Command timeout error interrupt flag */
#define SDMMC_INTR_STS_ERR              (1 << 15) /* Error Interrupt */
#define SDMMC_INTR_STS_CARD_INTR        (1 << 8)  /* Card interrupt flag */
#define SDMMC_INTR_STS_CARD_REMOVE      (1 << 7)  /* Card remove interrupt flag */
#define SDMMC_INTR_STS_CARD_INSERT      (1 << 6)  /* Card insertion interrupt flag */
#define SDMMC_INTR_STS_BUFF_READ_READY  (1 << 5)  /* Buffer raed ready interrupt flag */
#define SDMMC_INTR_STS_BUFF_WRITE_READY (1 << 4)  /* Buffer write ready interrupt flag */
#define SDMMC_INTR_STS_DMA              (1 << 3)  /* DMA interrupt flag */
#define SDMMC_INTR_STS_BLKGAP           (1 << 2)  /* Block gap event interrupt flag */
#define SDMMC_INTR_STS_TXR_COMPLETE     (1 << 1)  /* Transfer complete interrupt flag */
#define SDMMC_INTR_STS_CMD_COMPLETE     (1 << 0)  /* Command complete interrupt flag */

#define SDMMC_INTR_STS_ERR_CMD_LINE (SDMMC_INTR_STS_ERR_CMD_INDEX | SDMMC_INTR_STS_ERR_CMD_ENDBIT | SDMMC_INTR_STS_ERR_CMD_CRC | SDMMC_INTR_STS_ERR_CMD_TIMEOUT) /* Command error flag */
#define SDMMC_INTR_STS_ERR_DAT_LINE (SDMMC_INTR_STS_ERR_DATA_ENDBIT | SDMMC_INTR_STS_ERR_DATA_CRC | SDMMC_INTR_STS_ERR_DATA_TIMEOUT)                             /* Data error flag */

/**
 * SDMMC_LL_Soft_Reset Soft Reset
 */
#define SDMMC_SOFTRST_ALL (1 << 0) /* Software reset all */
#define SDMMC_SOFTRST_CMD (1 << 1) /* Software reset for cmd line */
#define SDMMC_SOFTRST_DAT (1 << 2) /* Software reset for data line */

/* Exported macro ------------------------------------------------------------*/

/**
 * @brief  Checks whether the specified SDMMC Present State is set or not.
 * @param  __INSTANCE__ Pointer to SDMMC register base
 * @param  __STS__ specifies the status to check.
 *          This parameter can be one of the following values:
 *          @arg  SDMMC_STA_CMDINBCMD: Command Inhibit (CMD)
 *          @arg SDMMC_STA_CMDINBDAT: Command Inhibit (DAT)
 *          @arg SDMMC_STA_DATACT: Data line active
 *          @arg SDMMC_STA_WACT: Write transfer active
 *          @arg SDMMC_STA_RACT: Read transfer active
 *          @arg SDMMC_STA_BUFWEN: Buffer write enable status
 *          @arg SDMMC_STA_BUFREN: Buffer read enable status
 *          @arg SDMMC_STA_CARDINST: Card inserted status
 *          @arg SDMMC_STA_CARDSTB: Card state stable
 *          @arg SDMMC_STA_CDPINLV: Card detect pin level
 *          @arg SDMMC_STA_WPPINLV: Write protect pin level
 * @retval The new state of SDMMC_PRSSTA(SET or RESET).*/

#define __SDMMC_GET_STA(__INSTANCE__, __STS__) (((__INSTANCE__)->PRSSTA & (__STS__)) != 0U)

/**
 * @brief  Enable the SDIO device interrupt.
 * @param  __INSTANCE__ : Pointer to SDIO register base
 * @param  __INTERRUPT__ : specifies the SDIO interrupt sources to be enabled.
 *         This parameter can be one or a combination of the following values:
 *            This parameter can be one or a combination of the following values:
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
 */
#define __SDMMC_ENABLE_IT(__INSTANCE__, __INTERRUPT__)     ((__INSTANCE__)->ISTEN |= (__INTERRUPT__))
#define __SDMMC_ENABLE_IT_SIG(__INSTANCE__, __INTERRUPT__) ((__INSTANCE__)->ISGEN |= (__INTERRUPT__))

/**
 * @brief  Disable the SDIO device interrupt.
 * @param  __INSTANCE__ : Pointer to SDIO register base
 * @param  __INTERRUPT__ : specifies the SDIO interrupt sources to be disabled.
 *          This parameter can be one or a combination of the following values:
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
 */
#define __SDMMC_DISABLE_IT(__INSTANCE__, __INTERRUPT__)     ((__INSTANCE__)->ISTEN &= ~(__INTERRUPT__))
#define __SDMMC_DISABLE_IT_SIG(__INSTANCE__, __INTERRUPT__) ((__INSTANCE__)->ISGEN &= ~(__INTERRUPT__))

/**
 * @brief  Checks whether the specified SDMMC flag is set or not.
 * @param  __INSTANCE__ Pointer to SDMMC register base
 * @param  __FLAG__ specifies the flag to check.
 *          This parameter can be one of the following values:
 *            @arg SDMMC_INTR_STS_ERR_ADMA:   ADMA error interrupt enable
 *            @arg SDMMC_INTR_STS_ERR_AutoCMD:Auto CMD12 error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CURR_LIMIT:Current limit error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_DATA_ENDBIT: Data end bit error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_DATA_CRC: Data CRC error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_DATA_TIMEOUT:Data timeout error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CMD_INDEX: Command index error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CMD_ENDBIT: Command end bit error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CMD_CRC: Command CRC error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CMD_TIMEOUT: Command timeout error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR: Error Interrupt flag
 *            @arg SDMMC_INTR_STS_CARD_INTR: Card interrupt flag
 *            @arg SDMMC_INTR_STS_CARD_REMOVE: Card remove interrupt flag
 *            @arg SDMMC_INTR_STS_CARD_INSERT: Card insertion interrupt flag
 *            @arg SDMMC_INTR_STS_BUFF_READ_READY: Buffer raed ready interrupt flag
 *            @arg SDMMC_INTR_STS_BUFF_WRITE_READY: Buffer write ready interrupt flag
 *            @arg SDMMC_INTR_STS_DMA: DMA interrupt flag
 *            @arg SDMMC_INTR_STS_BLKGAP: Block gap event interrupt flag
 *            @arg SDMMC_INTR_STS_TXR_COMPLETE: Transfer complete interrupt flag
 *            @arg SDMMC_INTR_STS_CMD_COMPLETE(: Command complete interrupt flag
 * @retval The new state of SDMMC_FLAG(SET or RESET).*/

#define __SDMMC_GET_FLAG(__INSTANCE__, __FLAG__) (((__INSTANCE__)->RIS & (__FLAG__)) != 0U)

/**
 * @brief  Clears the SDMMC pending flags.
 * @param  __INSTANCE__ Pointer to SDMMC register base
 * @param  __FLAG__ specifies the flag to clear.
 *          This parameter can be one or a combination of the following values:
 *            @arg SDMMC_INTR_STS_ERR_ADMA:   ADMA error interrupt enable
 *            @arg SDMMC_INTR_STS_ERR_AutoCMD:Auto CMD12 error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CURR_LIMIT:Current limit error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_DATA_ENDBIT: Data end bit error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_DATA_CRC: Data CRC error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_DATA_TIMEOUT:Data timeout error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CMD_INDEX: Command index error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CMD_ENDBIT: Command end bit error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CMD_CRC: Command CRC error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR_CMD_TIMEOUT: Command timeout error interrupt flag
 *            @arg SDMMC_INTR_STS_ERR: Error Interrupt
 *            @arg SDMMC_INTR_STS_CARD_INTR: Card interrupt flag
 *            @arg SDMMC_INTR_STS_CARD_REMOVE: Card remove interrupt flag
 *            @arg SDMMC_INTR_STS_CARD_INSERT: Card insertion interrupt flag
 *            @arg SDMMC_INTR_STS_BUFF_READ_READY: Buffer raed ready interrupt flag
 *            @arg SDMMC_INTR_STS_BUFF_WRITE_READY: Buffer write ready interrupt flag
 *            @arg SDMMC_INTR_STS_DMA: DMA interrupt flag
 *            @arg SDMMC_INTR_STS_BLKGAP: Block gap event interrupt flag
 *            @arg SDMMC_INTR_STS_TXR_COMPLETE: Transfer complete interrupt flag
 *            @arg SDMMC_INTR_STS_CMD_COMPLETE(: Command complete interrupt flag
 * @retval None
 */
#define __SDMMC_CLEAR_FLAG(__INSTANCE__, __FLAG__) ((__INSTANCE__)->RIS = (__FLAG__))

/**
 * @brief  Reset the SDMMC.
 * @param  __INSTANCE__ Pointer to SDMMC register base
 * @param  __RST__ specifies the cmd line or data line to reset.
 *          This parameter can be one or a combination of the following values:
 *            @arg SDMMC_SOFTRST_ALL: Software reset all
 *            @arg SDMMC_SOFTRST_CMD: Software reset for cmd line
 *            @arg SDMMC_SOFTRST_DAT:Software reset for data line
 * @retval None
 */
#define __SDMMC_SWRST_SET(__INSTANCE__, __RST__) ((__INSTANCE__)->SWRST |= (__RST__))

/**
 * @brief  Reset the SDMMC.
 * @param  __INSTANCE__ Pointer to SDMMC register base
 * @param  __RST__ specifies the cmd line or data line to reset.
 *          This parameter can be one or a combination of the following values:
 *            @arg SDMMC_SOFTRST_ALL: Software reset all
 *            @arg SDMMC_SOFTRST_CMD: Software reset for cmd line
 *            @arg SDMMC_SOFTRST_DAT:Software reset for data line
 * @retval The new state of SDMMD_SWRST flag
 */
#define __SDMMC_SWRST_GET_FLAG(__INSTANCE__, __FLAG__) (((__INSTANCE__)->SWRST & (__FLAG__)) != 0U)

//=========================================================================
//  SD_Exported_Constansts_Group3 SD Supported Memory Cards
//=========================================================================
#define CARD_SDSC      0x00000000U /*!< SD Standard Capacity <2Go                          */
#define CARD_SDHC_SDXC 0x00000001U /*!< SD High Capacity <32Go, SD Extended Capacity <2To  */
#define CARD_SECURED   0x00000003U

//=========================================================================
// SD Response Type
//=========================================================================
#define SDMMC_CMD_NO_RESPONSE    0x0  // For no response command
#define SDMMC_CMD_RTYPE_R2       0x9  // For R2
#define SDMMC_CMD_RTYPE_R3R4     0x2  // For R3,R4
#define SDMMC_CMD_RTYPE_R1R5R6R7 0x1A // For R1,R5,R6,R7
#define SDMMC_CMD_RTYPE_R1BR5B   0x1B // For R1b, R5b

//=========================================================================
// SD Command Type
//=========================================================================

#define SDMMC_CMD_TYPE_NORMAL  0x0
#define SDMMC_CMD_TYPE_SUSPEND 0x1
#define SDMMC_CMD_TYPE_RESUME  0x2
#define SDMMC_CMD_TYPE_ABORT   0x3

//=========================================================================
// SD Response Register
//=========================================================================
#define SDMMC_RESP0 0x00000000U
#define SDMMC_RESP1 0x00000004U
#define SDMMC_RESP2 0x00000008U
#define SDMMC_RESP3 0x0000000CU

//=========================================================================
// SD Transfer Direction
//=========================================================================
#define SDMMC_TRANSFER_DIR_TO_CARD 0x00000000U
#define SDMMC_TRANSFER_DIR_TO_SDIO 0x00000001U

//=========================================================================
// SD Transfer DMA TYPE
//=========================================================================
#define SDMMC_TRANSFER_SDMA 0x00000000U
#define SDMMC_TRANSFER_ADMA 0x00000002U

//=========================================================================
// SD context enumeration
//=========================================================================
#define SD_CONTEXT_NONE                 0x00000000U /*!< None                             */
#define SD_CONTEXT_READ_SINGLE_BLOCK    0x00000001U /*!< Read single block operation      */
#define SD_CONTEXT_READ_MULTIPLE_BLOCK  0x00000002U /*!< Read multiple blocks operation   */
#define SD_CONTEXT_WRITE_SINGLE_BLOCK   0x00000010U /*!< Write single block operation     */
#define SD_CONTEXT_WRITE_MULTIPLE_BLOCK 0x00000020U /*!< Write multiple blocks operation  */
#define SD_CONTEXT_IT                   0x00000008U /*!< Process in Interrupt mode        */
#define SD_CONTEXT_SDMA                 0x00000100U /*!< Process in SDMA mode             */
#define SD_CONTEXT_ADMA                 0x00000200U /*!< Process in ADMA mode             */

//=========================================================================
// SD Supported Memory Cards
//=========================================================================
#define CARD_NORMAL_SPEED     ((uint32_t)0x00000000U) /*!< Normal Speed Card <12.5Mo/s , Spec Version 1.01    */
#define CARD_HIGH_SPEED       ((uint32_t)0x00000100U) /*!< High Speed Card <25Mo/s , Spec version 2.00        */
#define CARD_ULTRA_HIGH_SPEED ((uint32_t)0x00000200U) /*!< UHS-I SD Card <50Mo/s for SDR50, DDR5 Cards \
                                                           and <104Mo/s for SDR104, Spec version 3.01          */

//=========================================================================
// SD Register 0x38: IntrGen
//=========================================================================
// #define SDMMC_INTR_SIG_EN_ALL         (0xF0CC)
// #define SDMMC_INTR_SIG_EN             (0x10CC)
#define SDMMC_INTR_SIG_EN       (SDMMC_INTR_STS_CARD_REMOVE | SDMMC_INTR_STS_CARD_INSERT | SDMMC_INTR_STS_CMD_COMPLETE | SDMMC_INTR_STS_TXR_COMPLETE)
#define SDMMC_INTR_SIGN_EN_SDMA (SDMMC_INTR_SIG_EN | SDMMC_INTR_STS_DMA | SDMMC_INTR_STS_BLKGAP)
#define SDMMC_INTR_SIGN_EN_ADMA (SDMMC_INTR_SIG_EN | SDMMC_INTR_STS_DMA)
#define SDMMC_INTR_SIGN_EN_PIO  (SDMMC_INTR_SIG_EN | SDMMC_INTR_STS_BLKGAP)
#define SDMMC_ERR_SIG_EN_ALL    (0xF7FF)

//=========================================================================
// ADMA2 Descriptor Table Generator
//=========================================================================
#define SDMMC_ADMA2_ENTRY_VALID (1 << 0)
#define SDMMC_ADMA2_ENTRY_END   (1 << 1)
#define SDMMC_ADMA2_ENTRY_INT   (1 << 2)

#define SDMMC_ADMA2_NOP  (0 << 4)
#define SDMMC_ADMA2_SET  (1 << 4)
#define SDMMC_ADMA2_TRAN (2 << 4)
#define SDMMC_ADMA2_LINK (3 << 4)

typedef struct
{
    uint8_t  TransferDir;
    uint8_t  MultiBlockEnable;
    uint8_t  AutoCMD12Enable;
    uint8_t  AutoCMD23Enable;
    uint8_t  DMAEnable;
    uint8_t  DMAType;
    uint8_t  BlockEnable;
    uint32_t BlockCount;
    uint32_t BlockSize;
    void    *ptr_Buf;
} SDIO_DataInitTypeDef;

/**
 * @brief  SDMMC Command Control structure
 */
typedef struct
{
    uint32_t Argument; /*!< Specifies the SDMMC command argument which is sent
                            to a card as part of a command message. If a command
                            contains an argument, it must be loaded into this register
                            before writing the command to the command register.              */

    uint32_t CmdIndex; /*!< Specifies the SDMMC command index. It must be Min_Data = 0 and
                            Max_Data = 64                                                    */

    uint32_t RspType; /*!< Specifies the SDMMC response type.
                            This parameter can be a value of @ref SDMMC_LL_Response_Type         */

    //	uint32_t Flags; /*!< Specifies the SDMMC response type.
    //                          This parameter can be a value of @ref SDMMC_LL_Response_Type         */
    uint32_t CmdType;
    uint32_t DatPres;
    uint32_t CmdIdxChk;
    uint32_t CmdCrcChk;
} SDIO_CmdInit_T;

uint32_t SDMMC_GetResponse(SN_SDIO_Type *SDIOx, uint32_t Response);
int8_t   SDMMC_ConfigData(SN_SDIO_Type *SDIOx, SDIO_DataInitTypeDef XfrCfg);
uint32_t SDMMC_ReadData(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length);
uint32_t SDMMC_WriteData(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length);
uint32_t SDMMC_TransferSDMA(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length);
uint32_t SDMMC_TransferDMA(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length);
uint32_t SDMMC_TransferADMA(SN_SDIO_Type *SDIOx, uint32_t *buffer, uint32_t length);

void     SDMMC_Init(SN_SDIO_Type *SDIOx, SD_Init_T Init);
void     SDMMC_Speed_Config(SN_SDIO_Type *SDIOx, uint32_t ClockDiv);
void     SDMMC_BusWide_Config(SN_SDIO_Type *SDIOx, uint32_t BusWide);
uint32_t SDMMC_ReadFIFO(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_WriteFIFO(SN_SDIO_Type *SDIOx, uint32_t *pWriteData);
void     SDMMC_PowerState_ON(SN_SDIO_Type *SDIOx);
void     SDMMC_PowerState_OFF(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_GetPowerState(SN_SDIO_Type *SDIOx);

/* SDMMC Commands management functions */
uint32_t SDMMC_CmdBlockLength(SN_SDIO_Type *SDIOx, uint32_t BlockSize);
uint32_t SDMMC_CmdReadSingleBlock(SN_SDIO_Type *SDIOx, uint32_t ReadAdd);
uint32_t SDMMC_CmdReadMultiBlock(SN_SDIO_Type *SDIOx, uint32_t ReadAdd);
uint32_t SDMMC_CmdWriteSingleBlock(SN_SDIO_Type *SDIOx, uint32_t WriteAdd);
uint32_t SDMMC_CmdWriteMultiBlock(SN_SDIO_Type *SDIOx, uint32_t WriteAdd);
uint32_t SDMMC_CmdSetBlockCount(SN_SDIO_Type *SDIOx, uint32_t BlockCnt);
uint32_t SDMMC_CmdEraseStartAdd(SN_SDIO_Type *SDIOx, uint32_t StartAdd);
uint32_t SDMMC_CmdSDEraseStartAdd(SN_SDIO_Type *SDIOx, uint32_t StartAdd);
uint32_t SDMMC_CmdEraseEndAdd(SN_SDIO_Type *SDIOx, uint32_t EndAdd);
uint32_t SDMMC_CmdSDEraseEndAdd(SN_SDIO_Type *SDIOx, uint32_t EndAdd);
uint32_t SDMMC_CmdErase(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_CmdStopTransfer(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_CmdSelDesel(SN_SDIO_Type *SDIOx, uint64_t Addr);
uint32_t SDMMC_CmdGoIdleState(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_CmdOperCond(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_CmdAppCommand(SN_SDIO_Type *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdAppOperCommand(SN_SDIO_Type *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdBusWidth(SN_SDIO_Type *SDIOx, uint32_t BusWidth);
uint32_t SDMMC_CmdSendSCR(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_CmdSendCID(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_CmdSendCSD(SN_SDIO_Type *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdSendEXTCSD(SN_SDIO_Type *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdSetRelAdd(SN_SDIO_Type *SDIOx, uint16_t *pRCA);
uint32_t SDMMC_CmdSendStatus(SN_SDIO_Type *SDIOx, uint32_t Argument, uint32_t *resp);
uint32_t SDMMC_CmdStatusRegister(SN_SDIO_Type *SDIOx);
uint32_t SDMMC_CmdOpCondition(SN_SDIO_Type *SDIOx, uint32_t Argument);
uint32_t SDMMC_CmdSwitch(SN_SDIO_Type *SDIOx, uint32_t Argument);

#ifdef __cplusplus
}
#endif

#endif /* _LL_SD_H_ */
