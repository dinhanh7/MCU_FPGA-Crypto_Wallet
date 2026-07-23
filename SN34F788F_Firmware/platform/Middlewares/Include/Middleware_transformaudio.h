#ifndef __M_TRANSFORM_AUDIO_H__
#define __M_TRANSFORM_AUDIO_H__

#include "sn34f78x_hal.h"

//=============================================================================
//                  Macro Definition
//=============================================================================

//=============================================================================
//                  Structure Declaration
//=============================================================================

typedef enum _SONIX_AUDIO_CODEC_FORMAT
{
	SONIX_AUDIO_CODEC_MP3DECODE,
	SONIX_AUDIO_CODEC_MP3ENCODE,
	SONIX_AUDIO_CODEC_A32DECODE,
	SONIX_AUDIO_CODEC_A32ENCODE,
	SONIX_AUDIO_CODEC_IMADECODE,
	SONIX_AUDIO_CODEC_IMAENCODE,
} SONIX_AUDIO_CODEC_FORMAT;


/**
  \brief Transform_Audio Info
*/
typedef struct
{
  uint32_t codec_format;
	uint8_t *input_buffer;
	uint8_t *output_buffer;
	void *codec_instance;
	int32_t samplerate;
	int32_t bitrate;
	int32_t channel;
	int32_t current_input_size;
	int32_t current_output_size;
	int32_t block_size;
	int32_t frame_timing_limit;
	int32_t frame_timeout;
} M_TRANSFORM_AUDIO_INFO;


typedef struct
{
	int32_t max_input_buffer_size;
	int32_t max_output_buffer_size;
} M_TRANSFORM_AUDIO_BUFFER_INFO;
/**
  \brief Transform_Audio Status
*/
typedef struct
{
  uint8_t busy;
} M_TRANSFORM_AUDIO_STATUS;

int32_t HAL_MTRANS_AUDIO_Init(M_TRANSFORM_AUDIO_INFO *info, uint32_t codec_format, int32_t samplerate, int32_t bitrate, int32_t channel);
int32_t HAL_MTRANS_AUDIO_UnInit(M_TRANSFORM_AUDIO_INFO *info);
int32_t HAL_MTRANS_AUDIO_Transform(M_TRANSFORM_AUDIO_INFO *info);
int32_t HAL_MTRANS_AUDIO_SetBufferAddress(M_TRANSFORM_AUDIO_INFO *info, uint8_t *input_buffer, uint8_t *output_buffer);
int32_t HAL_MTRANS_AUDIO_GetMaxPayloadSize(M_TRANSFORM_AUDIO_INFO *info, M_TRANSFORM_AUDIO_BUFFER_INFO *buffer_info); 
#endif /* __M_TRANSFORM_AUDIO_H__ */
