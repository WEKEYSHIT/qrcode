#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>
#include "jpeg2bmp.h"

struct my_error_mgr {
  struct jpeg_error_mgr pub;	/* "public" fields */

  jmp_buf setjmp_buffer;	/* for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

/*
 * Here's the routine that will replace the standard error_exit method:
 */

METHODDEF(void)
my_error_exit (j_common_ptr cinfo)
{
  /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
  my_error_ptr myerr = (my_error_ptr) cinfo->err;

  /* Always display the message. */
  /* We could postpone this until after returning, if we chose. */
  (*cinfo->err->output_message) (cinfo);

  /* Return control to the setjmp point */
  longjmp(myerr->setjmp_buffer, 1);
}


GLOBAL(int)
jpeg2grayscale(char *jpeg, int jpegSize, char* grayBitmap)
{
  struct jpeg_decompress_struct cinfo;
  
  struct my_error_mgr jerr;
  int row_stride;		/* physical row width in output buffer */

  /* Step 1: allocate and initialize JPEG decompression object */

  cinfo.err = jpeg_std_error(&jerr.pub);
  jerr.pub.error_exit = my_error_exit;
  if (setjmp(jerr.setjmp_buffer)) {
    jpeg_destroy_decompress(&cinfo);
    return 0;
  }
  jpeg_create_decompress(&cinfo);

  /* Step 2: specify data source (eg, a file) */

  jpeg_mem_src(&cinfo, jpeg, jpegSize);

  /* Step 3: read file parameters with jpeg_read_header() */

  (void) jpeg_read_header(&cinfo, TRUE);

  /* Step 4: set parameters for decompression */
  cinfo.out_color_space = JCS_GRAYSCALE;
  cinfo.desired_number_of_colors = 256;
  cinfo.dct_method = JDCT_IFAST;

  /* Step 5: Start decompressor */

  (void) jpeg_start_decompress(&cinfo);

  row_stride = cinfo.output_width * cinfo.output_components;

  /* Step 6: while (scan lines remain to be read) */
  /*           jpeg_read_scanlines(...); */
  while (cinfo.output_scanline < cinfo.output_height) {
    char* buff[1] = {grayBitmap};
    grayBitmap+=row_stride;
    (void) jpeg_read_scanlines(&cinfo, (JSAMPARRAY)buff, 1);
//    CopyToDest(obuff, out_buffer[0], row_stride);
  }

  /* Step 7: Finish decompression */

  (void) jpeg_finish_decompress(&cinfo);

  /* Step 8: Release JPEG decompression object */
  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_decompress(&cinfo);

  return 1;
}

