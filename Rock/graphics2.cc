#include <cstdlib>
//#include <pc.h>
#include "graphics.h"
//#include "object.h"
//#include "types.h"

using namespace std;


unsigned char *videobuffer = (unsigned char *)
  malloc(GRAPH_MAX_X * GRAPH_MAX_Y * sizeof(unsigned char));
unsigned char *GRAPH_MEM = (unsigned char *)0x000a0000;


//---------------------------------------------------------------------------
void setpalette()
{/*
  word count;

  for (count = 0; count <= 255; count++) {
    outportb(0x3c8,(unsigned char)count);
    outportb(0x3c9,(unsigned char)(63 * count / 255));
    outportb(0x3c9,(unsigned char)(63 * count / 255));
    outportb(0x3c9,(unsigned char)(63 * count / 255));
  }*/
}

