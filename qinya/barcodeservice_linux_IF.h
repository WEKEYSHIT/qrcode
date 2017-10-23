//=====================================================================================================
// barcodeservice_linux_IF.h
//=====================================================================================================
 
 
//-----------------------------------------------------------------------------------------------------
//
//  Module Interface Description:
//      This file contains the interface definitions to decoder and set config and get config of lib.
//
//-----------------------------------------------------------------------------------------------------


/* Copyright 2017, Image Algorithm Lab.  All Rights Reserved */


/* Preamble */
#ifndef _BS_LINUX_H_INCLUDED
#define _BS_LINUX_H_INCLUDED

typedef unsigned char   uint8_t;
typedef char            sint8_t;
typedef unsigned short  uint16_t;
typedef short           sint16_t;
typedef unsigned int    uint32_t;
typedef int             sint32_t;

#define MAX_DECODED_DATA_LENGTH		(8*1024)

#define BS_API

typedef struct {
	uint8_t     Data[MAX_DECODED_DATA_LENGTH];
	uint32_t    Length;
    uint32_t    Symbology;
    uint32_t    Decoder_Succeed;
} BS_DECODER_RESULT_t;

typedef struct {
	uint8_t     *pImage;
	uint32_t    ImageWidth;
	uint32_t    ImageLength;
} BS_IMAGE_PARAMETER_T;

typedef enum
{
    BS_CONFIG_KILL_DECODER,
    BS_CONFIG_SYMBOLOGY_ALL_SYMBOLOGIES_EN,     
    BS_CONFIG_SYMBOLOGY_UPCA_EN,
    BS_CONFIG_SYMBOLOGY_UPCE_EN,
    BS_CONFIG_SYMBOLOGY_EAN8_EN,
    BS_CONFIG_SYMBOLOGY_EAN13_EN,
    BS_CONFIG_SYMBOLOGY_CODE_11_EN,
    BS_CONFIG_SYMBOLOGY_CODE_39_EN,
    BS_CONFIG_SYMBOLOGY_CODE_128_EN,  
    BS_CONFIG_SYMBOLOGY_CODE_93_EN,
    BS_CONFIG_SYMBOLOGY_INTERLEAVED_25_EN,
    BS_CONFIG_SYMBOLOGY_GS1_DATABAR_EN,
    BS_CONFIG_SYMBOLOGY_CODEBLOCK_F_EN,
    BS_CONFIG_SYMBOLOGY_PDF417_EN,
    BS_CONFIG_SYMBOLOGY_MICROPDF_EN,
    BS_CONFIG_SYMBOLOGY_QR_CODE_EN,
    BS_CONFIG_SYMBOLOGY_DATA_MATRIX_EN,
    BS_CONFIG_SYMBOLOGY_AZTEC_EN,
    BS_CONFIG_SYMBOLOGY_HAXIN_EN,
    BS_CONFIG_SYMBOLOGY_MATRIX_25_EN,
    BS_CONFIG_SYMBOLOGY_TRIOPTIC_EN,
    BS_CONFIG_SYMBOLOGY_STRAIGHT_25_EN,
    BS_CONFIG_SYMBOLOGY_TELEPEN_EN,
    BS_CONFIG_SYMBOLOGY_CODABAR_EN,
    BS_CONFIG_MENU_CODABAR_START_STOP_TRANSMIT,
    BS_CONFIG_MENU_HANXIN_ECI_HEADER_TRANSMIT,
    BS_CONFIG_MENU_PDF417_ESC_TRANSMIT,
    BS_CONFIG_MENU_C128_SHORT_MARGIN,
    BS_CONFIG_MENU_DECODER_MIRRORED_IMAGE
} BS_CONFIG_MENU_CMD_t;

typedef enum
{     
    BS_CONFIG_SYMBOLOGY_TYPE_UPCA,
    BS_CONFIG_SYMBOLOGY_TYPE_UPCE,
    BS_CONFIG_SYMBOLOGY_TYPE_EAN8,
    BS_CONFIG_SYMBOLOGY_TYPE_EAN13,
    BS_CONFIG_SYMBOLOGY_TYPE_CODE_11,
    BS_CONFIG_SYMBOLOGY_TYPE_CODE_39,
    BS_CONFIG_SYMBOLOGY_TYPE_CODE_128,  
    BS_CONFIG_SYMBOLOGY_TYPE_CODE_93,
    BS_CONFIG_SYMBOLOGY_TYPE_INTERLEAVED_25,
    BS_CONFIG_SYMBOLOGY_TYPE_GS1_DATABAR,
    BS_CONFIG_SYMBOLOGY_TYPE_CODEBLOCK_F,
    BS_CONFIG_SYMBOLOGY_TYPE_PDF417,
    BS_CONFIG_SYMBOLOGY_TYPE_QR_CODE,
    BS_CONFIG_SYMBOLOGY_TYPE_DATA_MATRIX,
    BS_CONFIG_SYMBOLOGY_TYPE_AZTEC,
    BS_CONFIG_SYMBOLOGY_TYPE_HAXIN,
    BS_CONFIG_SYMBOLOGY_TYPE_MATRIX_25,
    BS_CONFIG_SYMBOLOGY_TYPE_TRIOPTIC,
    BS_CONFIG_SYMBOLOGY_TYPE_STRAIGHT_25,
    BS_CONFIG_SYMBOLOGY_TYPE_TELEPEN,
    BS_CONFIG_SYMBOLOGY_TYPE_CODABAR
} BS_CONFIG_SYMBOLOGY_TYPE_t;

typedef enum 
{
    BS_CONFIG_SYMBOLOGY_DISABLE,
    BS_CONFIG_SYMBOLOGY_ENABLE
} BS_CONFIG_MENU_CMD_SYMBOLOGY_EN_t;

typedef enum 
{
    BS_CONFIG_MENU_DISABLE,
    BS_CONFIG_MENU_ENABLE
} BS_CONFIG_MENU_EN_t;

typedef struct 
{
    BS_CONFIG_MENU_CMD_t    cmd;
    uint32_t                 value;
} CONFIG_MENU_t;


/** 
 *  \brief initialize the decoder lib.
 *  
 *  \param [in] devname -> Device name of IIC.
 *  \return Non-zero if the initialization was successful.
 *  \       0->Fail
 *  \       1->Success 
 *  \       others->Activation Error
 */
BS_API sint32_t imagealgorithmlab_bs_init(sint8_t *devname);


/** 
 *  \brief close the decoder lib.
 *  
 *  \return Non-zero if the operation was successful.
 *  \       0->Fail
 *  \       1->Success 
 */
BS_API sint32_t imagealgorithmlab_bs_close(void);

/** 
 *  \brief Decodes the image data.
 *  
 *  \param [in] pImageParameter -> Data to be decoded.
 *  \param [out] pDecoderResult -> Decoder result.
 *  \return Non-zero if the decoder was successful.
 *  \       0->Fail
 *  \       1->Success   
 */
BS_API sint32_t imagealgorithmlab_bs_decoder(BS_IMAGE_PARAMETER_T *pImageParameter,BS_DECODER_RESULT_t *pDecoderResult);

/** 
 *  \brief Configures the lib with the data.
 *  
 *  \param [in] cmd     -> Lib configuration cmd.
 *  \param [in] value   -> Value to be set.
 *  \return Non-zero if the data was stored successfully.  
 *  \       0->Fail
 *  \       1->Success
 *  \       2->Not Support 
 */
BS_API sint32_t imagealgorithmlab_bs_setConfig(CONFIG_MENU_t *menu);

/** 
 *  \brief Copies the value of the API tag to the memory space pointed to by data.
 *  
 *  \param [in] cmd     -> Lib configuration cmd.
 *  \param [out] value  -> Lib configuration value.
 *  \return Non-zero if the data was retrieved successfully.
 *  \       0->Fail
 *  \       1->Success
 *  \       2->Not Support 
 */
BS_API sint32_t imagealgorithmlab_bs_getConfig(CONFIG_MENU_t *menu);

/** 
 *  \brief get the lib reversion
 *
 *  \return lib reversion.
 */
BS_API sint8_t *imagealgorithmlab_bs_get_lib_reversion(void);

/* Error Codes */

#endif /* !BS_LINUX_H_INCLUDED */

