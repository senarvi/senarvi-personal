#include "init.h"
#include "graphics.h"
#include "object.h"

volatile bool pause = false;
signed long numframes = 0,numints = 0;


//---------------------------------------------------------------------------
// Main program
//---------------------------------------------------------------------------

int main(int argc,char *argv[])
{
  svector rock_location = {0,0,500 << FRACBITS};
  sangle  rock_angle = {0,0};

  init();
  rock.init(ROCK_VERTICES,ROCK_FACES,rock_vertextab,rock_facetab,
            rock_location,rock_angle);
  clearbuffer();
  copybuffer();
  while (true) {
    rock.rotate(); /* Rotate object */
    rock.draw();   /* Draw object to video buffer */
    rock.show();   /* Move object from video buffer to screen */
    numframes++;   /* Increase frames shown */
    while (pause); /* Wait if pause */
  }
  exittype = EXIT_NORMAL;
}

