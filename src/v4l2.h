#ifndef __V4L2_H__
#define __V4L2_H__

typedef struct
{
    unsigned char* data;
    unsigned int length;
}VIDEO_BUFF;

extern void process_image(VIDEO_BUFF* buf);
#define V4L2_PROCESS(BUFF) process_image(BUFF)

void video_init(const char* device, int width, int height);
void video_start();
void video_exit();

#endif

