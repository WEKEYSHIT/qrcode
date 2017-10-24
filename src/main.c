#include "v4l2.h"
#include "qrdecode.h"
#include "jpeg2bmp.h"

#include <stdio.h>
#include <math.h>

#define WIDTH 640
#define HEIGHT 480

unsigned int suma(unsigned char* buff)
{
    register int size = HEIGHT*WIDTH;
    register unsigned char* pbuff = buff;
    register unsigned int sum = 0;
    while(size--)
        sum += *pbuff++;
    return sum;
}

int moveCheck(unsigned char* buff)
{
    static unsigned last = 0, new = 0;
    new = suma(buff);
    unsigned int move = abs(new - last);
    last = new;
    return move>>18;
}

void process_image(VIDEO_BUFF* buf)
{
//    static int count = 0;
    unsigned char grayImage[HEIGHT*WIDTH];
    jpeg2grayscale(buf->data, buf->length, grayImage);
//    if(moveCheck(grayImage))
    {
        Qr_Decode(grayImage, WIDTH, HEIGHT);
    }
}

int main()
{
    Qr_Init();
    video_init("dev/video0", WIDTH, HEIGHT);
    video_start();
    video_exit();
    Qr_Exit();
}

