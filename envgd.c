
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

#include <pixel.h>
#include <image.h>

#include <point.h>

#include <line.h>

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
  
  img.xres = 960;
  img.yres = 960;

  num_pixels = img.xres * img.yres;
  img_sz = num_pixels * sizeof(pixel_t);

  img.rgb = malloc(img_sz);
  if (img.rgb == NULL) {
    perror("malloc");
    return -1;
  }
  
  line = NULL;
  fp = stdin;
  len = 0;

  memset(img.rgb, 0, img_sz);

  while ((retval = getline(&line,&len,fp)) != -1) {

    input_row ir;
    
    if (len > 0) {

      retval = sscanf(line, "%[^,], %lg, %lg, %lg, %lg", ir.color, &(ir.t0), &(ir.t1), &(ir.sig0), &(ir.sig1));

      if (retval != 5) {
	fprintf(stderr, "%s: sscanf retval %d\n", __FUNCTION__, retval);
	return -1;
      }
	
      {
	
	uint64_t x1, y1, x2, y2;

	point_t pt1, pt2;

	fill_color = (pixel_t) { .r = 65535, .g = 65535, .b = 65535 };
	
	fprintf(stderr, "%s: Have values %s %g %g %g %g\n", __FUNCTION__, ir.color, ir.t0, ir.t1, ir.sig0, ir.sig1);

	pt1.x = -1.0 + 2.0 * ir.t0; pt2.x = -1.0 + 2.0 * ir.t1;

	pt1.y = ir.sig0; pt2.y = ir.sig1;

	xpos = pt1.x * (img.xres >> 1); xpos += img.xres >> 1;
	ypos = pt1.y * (img.yres >> 1); ypos += img.yres >> 1;

	x1 = xpos;
	y1 = ypos;

	xpos = pt2.x * (img.xres >> 1); xpos += img.xres >> 1;
	ypos = pt2.y * (img.yres >> 1); ypos += img.yres >> 1;

	x2 = xpos;
	y2 = ypos;
	
	linev6(&img, x1, y1, x2, y2, fill_color);
	
      }

    }
    
  }

  free(line);

  for (xpos = 0; xpos < img.xres; xpos++) {

    for (ypos = 0; ypos < img.yres; ypos++) {
      if (!memcmp(img.rgb + ypos * img.xres + xpos, &fill_color, sizeof(fill_color))) {

	while(ypos < img.yres) {
	  img.rgb[ypos * img.xres + xpos] = fill_color;
	  ypos++;
	}

	ypos = img.yres;
	
      }
    }
  }
    
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
