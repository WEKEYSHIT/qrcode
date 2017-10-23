#ifndef __QRDECODE_H__
#define __QRDECODE_H__

void Qr_Init();
void Qr_Exit();
void Qr_Decode(unsigned char *image, int width, int height);

#endif

