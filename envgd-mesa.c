
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include "mini_gxkit.h"

#include <GL/osmesa.h>

typedef struct {

  char color[20];

  double t0, t1;

  double sig0, sig1;
  
} input_row;

int main(int argc, char *argv[]) {

  image_t img;

  long int num_pixels;

  size_t img_sz;

  char *line;

  FILE *fp;

  size_t len;

  int retval;

  pixel_t fill_color;

  long int xpos, ypos;

  OSMesaContext mc;

  uint8_t *colorbuf;

  long int pixelno;

  GLboolean resbool;

  pixel_t black;
  
  img.xres = 960;
  img.yres = 960;

  num_pixels = img.xres * img.yres;
  img_sz = num_pixels * sizeof(pixel_t);

  img.rgb = malloc(img_sz);
  if (img.rgb == NULL) {
    perror("malloc");
    return -1;
  }

  colorbuf = malloc(num_pixels * 3);
  if (colorbuf == NULL) {
    perror("malloc");
    return -1;
  }
  
  line = NULL;
  fp = stdin;
  len = 0;

  memset(img.rgb, 0, img_sz);

  memset(colorbuf, 0, num_pixels * 3);
  
  mc = OSMesaCreateContext(GL_RGB, NULL);
  resbool = OSMesaMakeCurrent(mc, colorbuf, GL_UNSIGNED_BYTE, img.xres, img.yres);

  if (resbool == GL_FALSE) {
    fprintf(stderr, "%s: Trouble with call to OSMesaMakeCurrent.\n", __FUNCTION__);
    return -1;
  }
  
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glViewport ( 0, 0, img.xres, img.yres );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0f, 1.0f);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  black = (pixel_t) { .r = 0, .g = 0, .b = 0 };
  
  glBegin(GL_LINES);

  while ((retval = getline(&line,&len,fp)) != -1) {

    input_row ir;
    
    if (len > 0) {

      retval = sscanf(line, "%[^,], %lg, %lg, %lg, %lg", ir.color, &(ir.t0), &(ir.t1), &(ir.sig0), &(ir.sig1));

      if (retval != 5) {
	fprintf(stderr, "%s: sscanf retval %d\n", __FUNCTION__, retval);
	return -1;
      }
	
      {
	
	point_t pt1, pt2;

	fill_color = (pixel_t) { .r = 65535, .g = 65535, .b = 65535 };
	
	fprintf(stderr, "%s: Have values %s %g %g %g %g\n", __FUNCTION__, ir.color, ir.t0, ir.t1, ir.sig0, ir.sig1);

	pt1.x = -1.0 + 2.0 * ir.t0; pt2.x = -1.0 + 2.0 * ir.t1;

	pt1.y = ir.sig0; pt2.y = ir.sig1;

	glColor3d(1.0, 1.0, 1.0);
	
	glVertex3d(pt1.x, pt1.y, 0.0);
	glVertex3d(pt2.x, pt2.y, 0.0);
	
      }

    }
    
  }

  glEnd();

  glFlush();
  
  free(line);

  OSMesaDestroyContext(mc);

  for (pixelno = 0; pixelno < num_pixels; pixelno++) {
    pixel_t col;
    col.r = colorbuf[3*pixelno+0]; col.r <<= 8;
    col.g = colorbuf[3*pixelno+1]; col.g <<= 8;
    col.b = colorbuf[3*pixelno+2]; col.b <<= 8;    
    img.rgb[pixelno] = col;
  }
  
  for (xpos = 0; xpos < img.xres; xpos++) {

    for (ypos = 0; ypos < img.yres; ypos++) {

      if (!memcmp(img.rgb + ypos * img.xres + xpos, &black, sizeof(pixel_t))) {
	continue;
      }

      while(ypos < img.yres) {
	img.rgb[ypos * img.xres + xpos] = fill_color;
	ypos++;
      }

      ypos = img.yres;

    }
  }

  free(colorbuf);
  
  {

    int out_fd;

    ssize_t bytes_written;
    
    out_fd = 1;
    
    bytes_written = write(out_fd, img.rgb, img_sz);
    if (bytes_written != img_sz) {
      perror("write");
      return -1;
    }

  }

  free(img.rgb);
	 
  return 0;

}
