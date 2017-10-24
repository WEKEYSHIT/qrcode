noinst_PROGRAMS = QrCameraV2

libdir=${top_builddir}/src
headdir=${top_builddir}/src/incs

QrCameraV2_SOURCES=jpeg2bmp.c main.c qrdecode.c v4l2.c
QrCameraV2_LDADD=${top_builddir}/server/libServer.a
QrCameraV2_LDADD+=-lstdc++ -lpthread
QrCameraV2_LDADD+=${top_builddir}/jpeg/.libs/libturbojpeg.a
QrCameraV2_LDADD+=-L${top_builddir}/qinya -lBarcodeServiceLinux_0.02_01_03.20171019
QrCameraV2_CFLAGS=-I ${top_builddir}/server
QrCameraV2_CFLAGS+=-I ${top_builddir}/jpeg
QrCameraV2_CFLAGS+=-I ${top_builddir}/qinya
QrCameraV2_CFLAGS+=-std=c99

release:${noinst_PROGRAMS}
	arm-linux-gnueabi-strip ${noinst_PROGRAMS}

pack:release
	fput ${noinst_PROGRAMS}
