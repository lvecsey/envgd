
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <math.h>

int main(int argc, char *argv[]) {

  long int num_lines;

  long int lineno;

  char *color;

  double rad0, rad1;

  double freq = 3.0;

  double t0, t1;

  num_lines = 100;

  color = "WHITE";
  
  for (lineno = 1; lineno < num_lines; lineno++) {

    rad0 = (2.0 * M_PI * freq * (lineno - 1)) / num_lines;
    rad1 = (2.0 * M_PI * freq * lineno) / num_lines;    

    t0 = (lineno - 1); t0 /= num_lines;
    t1 = lineno; t1 /= num_lines;
    
    printf("%s, %g, %g, %g, %g\n", color, t0, t1, 0.2 * sin(rad0), 0.2 * sin(rad1));
    
  }
  
  return 0;

}
