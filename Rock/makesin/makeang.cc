#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

#define WIDTH           320
#define HEIGHT          200
#define NUM_ANGLES      255
#define NUM_COLUMNS     8

static const double PI = 3.14159265358979323846;
static const double PI2 = PI + PI;

FILE          *outputfile;
unsigned char *angletab;

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

  angletab=(unsigned char *) malloc(WIDTH*HEIGHT*sizeof(signed char));
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
  unsigned long x,y;
  unsigned char angle;

  if (init(argc,argv) != 0) exit(-1);

  for (y=1; y <= HEIGHT/2; y++)
    for (x=1; x <= WIDTH/2; x++)
    {
      angle=(unsigned char)(atan((double)y/(double)x)/PI2*(NUM_ANGLES/4-1));
angletab[(HEIGHT/2-1+y)*WIDTH+ WIDTH/2-1+x]=                angle;
angletab[(HEIGHT/2-1+y)*WIDTH+ WIDTH/2  -x]= 1*NUM_ANGLES/4+(NUM_ANGLES/4-angle);
angletab[(HEIGHT/2  -y)*WIDTH+ WIDTH/2  -x]= 2*NUM_ANGLES/4+angle;
angletab[(HEIGHT/2  -y)*WIDTH+ WIDTH/2-1+x]= 3*NUM_ANGLES/4+(NUM_ANGLES/4-angle);
    }

  for (y=0; y < HEIGHT; y++)
  {
    fprintf(outputfile,"\n");
    for (x=0; x < WIDTH; x++)
    {
      if (x%NUM_COLUMNS == 0) fprintf(outputfile,"\n\t\tdb\t");
      else fprintf(outputfile,", ");
      fprintf(outputfile,"%u",angletab[y*WIDTH+x]);
    }
  }
  fprintf(outputfile,"\n");

  return 0;
}
