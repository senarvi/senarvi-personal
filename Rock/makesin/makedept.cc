#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH           320
#define HEIGHT          200
#define NUM_COLUMNS     16
#define RADIUS          64
#define LENSDIST        256

static const double PI = 3.14159265358979323846;
static const double PI2 = PI + PI;

FILE   *outputfile;
double *angletab;

void uninit();

int init(int numparams,char *paramtab[])
{
  atexit(uninit);

  if (numparams < 1)
  {
    printf("usage: makeang <outfile.asm>\n");
    return -1;
  }
  outputfile = fopen(paramtab[1],"w");
  if (outputfile == NULL)
  {
    printf("File not found: %s\n",paramtab[1]);
    return -1;
  }

  angletab=(double *) malloc(WIDTH*HEIGHT*sizeof(double));
  if (angletab == NULL)
  {
    printf("Not enough memory\n");
    return -1;
  }
  return 0;
}

void uninit() {
  fclose(outputfile);
  free(angletab);
}

int main(int argc,char *argv[])
{
  double        count;
  unsigned long x,y;
  double        angle;

  if (init(argc,argv) != 0) exit(-1);

  for (y=1; y <= HEIGHT/2; y++)
    for (x=1; x <= WIDTH/2; x++)
    {
      angle=atan((double)y/(double)x);
      angletab[(HEIGHT/2-1+y)*WIDTH+ WIDTH/2-1+x]= angle;
      angletab[(HEIGHT/2-1+y)*WIDTH+ WIDTH/2  -x]= PI2-angle;
      angletab[(HEIGHT/2  -y)*WIDTH+ WIDTH/2  -x]= angle;
      angletab[(HEIGHT/2  -y)*WIDTH+ WIDTH/2-1+x]= PI2-angle;
    }

  for (y=0; y < HEIGHT; y++)
  {
    fprintf(outputfile,"\n");
    for (x=0; x < WIDTH; x++)
    {
      if (x%NUM_COLUMNS == 0) fprintf(outputfile,"\ndb  ");
      else fprintf(outputfile,", ");
      fprintf(outputfile,"%i",(unsigned char)
              (cos(angletab[y*WIDTH+x])*RADIUS*LENSDIST/(WIDTH/2+x))%256);
    }
  }
  fprintf(outputfile,"\n");

  return 0;
}
