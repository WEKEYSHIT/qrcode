#include "v4l2.h"
#include "qrdecode.h"
#include "jpeg2bmp.h"

#define WIDTH 640
#define HEIGHT 480

void process_image(VIDEO_BUFF* buf)
{
    unsigned char grayImage[HEIGHT*WIDTH];
    jpeg2grayscale(buf->data, buf->length, grayImage);
    Qr_Decode(grayImage, WIDTH, HEIGHT);
}

int main()
{
    Qr_Init();
    video_init("dev/video0", WIDTH, HEIGHT);
    video_start();
    video_exit();
    Qr_Exit();
}

