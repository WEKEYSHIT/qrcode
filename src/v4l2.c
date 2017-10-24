#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "v4l2.h"

#define V_ERROR(COND, ERRNO) if(COND < 0){perror(__FUNCTION__);exit(ERRNO);}
#define V_WARNING(COND) if(COND){perror(__FUNCTION__);}

#define MMAP_NUM 4
static VIDEO_BUFF video_bufs[MMAP_NUM]; 
static int fd = -1;

static void v4l2_open(const char* devname)
{
    fd = open("/dev/video0", O_RDWR);
    V_ERROR(fd, -1);
}

static void v4l2_close()
{
    close(fd);
}


static void v4l2_capability()
{
    struct v4l2_capability cap;
    int ret = ioctl(fd,VIDIOC_QUERYCAP,&cap);
    V_ERROR(ret, -1);
    printf("Driver Name:%s\nCard Name:%s\nBus info:%s\nDriver Version:%u.%u.%u\n",cap.driver,cap.card,cap.bus_info,(cap.version>>16)&0xFF, (cap.version>>8)&0xFF,cap.version&0xFF);

    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index=0;
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    printf("Support format:\n");
    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
    {
        printf("\t%d.%s\n",fmtdesc.index+1,fmtdesc.description);
        fmtdesc.index++;
    }
}

static void v4l2_curVideoFmt()
{
    struct v4l2_format fmt;
    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret = ioctl(fd,VIDIOC_G_FMT,&fmt);
    V_ERROR(ret, -1);
    printf("Current data format information:\n\twidth:%d\n\theight:%d\n",fmt.fmt.pix.width,fmt.fmt.pix.height);

    struct v4l2_fmtdesc fmtdesc;
    fmtdesc.index=0;
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while(ioctl(fd,VIDIOC_ENUM_FMT,&fmtdesc)!=-1)
    {
        if(fmtdesc.pixelformat & fmt.fmt.pix.pixelformat)
        {
            printf("\tformat:%s\n",fmtdesc.description);
            break;
        }
        fmtdesc.index++;
    } 
}

static void v4l2_setVideoFmt(int width, int height)
{
    struct v4l2_format fmt;
    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
   // V4L2_PIX_FMT_GREY
   // V4L2_PIX_FMT_YUYV
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    fmt.fmt.pix.width = width;
    fmt.fmt.pix.height = height;
    int ret = ioctl(fd,VIDIOC_S_FMT,&fmt);
    V_ERROR(ret, -1);
}


static void v4l2_reqBuffer()
{
    struct v4l2_requestbuffers req;
    req.count = MMAP_NUM;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory = V4L2_MEMORY_MMAP;
    int ret = ioctl(fd,VIDIOC_REQBUFS,&req);
    V_ERROR(ret, -1);
}

static void v4l2_MmapBuffer()
{
    for (unsigned int __i = 0; __i < MMAP_NUM; ++__i)
    {
        struct v4l2_buffer buf;
        memset(&buf,0,sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = __i;
        // 查询序号为__i 的缓冲区，得到其起始物理地址和大小
        int ret = ioctl(fd, VIDIOC_QUERYBUF, &buf);
        V_ERROR(ret, -1);
        video_bufs[__i].length = buf.length;
        // 映射内存
        video_bufs[__i].data = mmap (NULL, buf.length, PROT_READ | PROT_WRITE , MAP_SHARED, fd, buf.m.offset);
        ret = MAP_FAILED == video_bufs[__i].data ? -1 : 0;
        V_ERROR(ret, -2);
        printf("map %d:%x=>%p len:%d\n", __i, buf.m.offset, video_bufs[__i].data, video_bufs[__i].length);
    }
}

static void v4l2_MunmapBuffer()
{
    for (unsigned int __i = 0; __i < MMAP_NUM; ++__i)
    {
        int ret = munmap(video_bufs[__i].data, video_bufs[__i].length);
        V_ERROR(ret, -1);
    }
}


static void v4l2_CaptureStart()
{
    for (int i = 0; i < MMAP_NUM; ++i)
    {
        struct v4l2_buffer buf;
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        int ret = ioctl (fd, VIDIOC_QBUF, &buf);
        V_ERROR(ret, -1);
    }
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret = ioctl (fd, VIDIOC_STREAMON, &type);
    V_ERROR(ret, -2);
}

static void v4l2_CaptureStop()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    int ret = ioctl (fd, VIDIOC_STREAMOFF, &type);
    V_ERROR(ret, -1);
}

static void v4l2_loop()
{
    struct v4l2_buffer buf;
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    while(1)
    {
        // 从缓冲区取出一个缓冲帧
        int ret = ioctl(fd, VIDIOC_DQBUF, &buf);
        // 图像处理
        V4L2_PROCESS(&video_bufs[buf.index]);
        // 将取出的缓冲帧放回缓冲区
        ret = ioctl(fd, VIDIOC_QBUF, &buf); 
    }
}


void video_init(const char* device, int width, int height)
{
    v4l2_open(device);
    v4l2_capability();
    v4l2_setVideoFmt(width, height);
    v4l2_curVideoFmt();
    v4l2_reqBuffer();
    v4l2_MmapBuffer();

}

void video_start()
{
    v4l2_CaptureStart();
    v4l2_loop();
}

void video_exit()
{
    v4l2_MunmapBuffer();
    v4l2_close();
}

