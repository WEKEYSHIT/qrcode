#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "barcodeservice_linux_IF.h"

#include "Server.h"
static void* server_obj = NULL;
#define SERVER_INIT() {server_obj = NewServerSimple();}
#define SERVER_EXIT() {DelServer(server_obj);}
#define SERVER_SEND(MESG, LEN) {Broadcast(server_obj, MESG, LEN);}

static inline void qinya_init()
{
	while(imagealgorithmlab_bs_init("/dev/atsha0") != 1)
	{
        printf("BS_SAMPLE : imagealgorithmlab_bs_init error!\n");
		sleep(1);
	}
    CONFIG_MENU_t pTestMenu = 
	{
		.cmd = BS_CONFIG_SYMBOLOGY_QR_CODE_EN,
		.value = BS_CONFIG_SYMBOLOGY_ENABLE,
	};
	while(imagealgorithmlab_bs_setConfig(&pTestMenu) != 1)
	{
		printf("BS_SAMPLE : imagealgorithmlab_bs_setConfig error!\n");
	}
	printf("BS_SAMPLE : %s\n", imagealgorithmlab_bs_get_lib_reversion());

    SERVER_INIT();
}

static inline void qinya_exit()
{
    SERVER_EXIT();

	while(imagealgorithmlab_bs_close() != 1)
	{
		printf("BS_SAMPLE : imagealgorithmlab_bs_close error!\n");
	}
}

static inline void qinya_decode(unsigned char *image, int width, int height)
{
    BS_IMAGE_PARAMETER_T pTestImage;
    BS_DECODER_RESULT_t pTestResult;

    pTestImage.ImageLength = height;
    pTestImage.ImageWidth = width;
    pTestImage.pImage = image;

	if(!imagealgorithmlab_bs_decoder(&pTestImage, &pTestResult))
		return;
	if(1 != pTestResult.Decoder_Succeed)
        return;
    SERVER_SEND(pTestResult.Data, pTestResult.Length);
}

void Qr_Init()
{
    qinya_init();
}

void Qr_Decode(unsigned char *image, int width, int height)
{
    qinya_decode(image, width, height);
}

void Qr_Exit()
{
    qinya_exit();
}

