#include <iostream>
#include <cstdlib>
#include <cstdio>
//#include <pc.h>
#include <dos.h>
//#include <dpmi.h>
//#include <go32.h>
//#include <sys\farptr.h>
#include "init.h"
#include "main.h"
#include "hardware.h"
#include "graphics.h"
#include "vector.h"
#include "object.h"

using namespace std;

//_go32_dpmi_meminfo freememinfo;
//_go32_dpmi_seginfo si_okbint,si_kbint;
//_go32_dpmi_seginfo si_opitint,si_pitint;

unsigned long requiredmemory = 0;
int           exittype = EXIT_BREAK;

void pitint(),kbint(),
     initpit(),initkb(),
     resetpit(),resetkb(),
     init(),uninit();


//---------------------------------------------------------------------------
void pitint()
{/*
  numints++;

  rock.angle.a++;
  rock.angle.b -= 2;
  outportb(0x20,0x20);*/
}

//---------------------------------------------------------------------------
void kbint()
{/*
  unsigned char key = inportb(PORT_KB_DATA);

  switch (key) {
    case SCAN_ESCAPE :
      exittype = EXIT_USER;
      outportb(0x20,0x20);
      exit(EXIT_SUCCESS);
    case SCAN_P :
      pause = true;
      break;
    case SCAN_RELEASE + SCAN_P :
      pause = false;
  }
  outportb(0x20,0x20);*/
}

//---------------------------------------------------------------------------
void initpit()
{/*
  _go32_dpmi_get_protected_mode_interrupt_vector(INT_PIT,&si_opitint);
  si_pitint.pm_selector = _go32_my_cs();
  si_pitint.pm_offset = (unsigned long)pitint;
  _go32_dpmi_allocate_iret_wrapper(&si_pitint);
  _go32_dpmi_set_protected_mode_interrupt_vector(INT_PIT,&si_pitint);

  outportb(PORT_PIT_CMD,PIT_CONTROL);
  outportb(PORT_PIT_DATA,PIT_COUNTER % 0x100);
  outportb(PORT_PIT_DATA,PIT_COUNTER / 0x100);*/
}

//---------------------------------------------------------------------------
void initkb()
{/*
  _go32_dpmi_get_protected_mode_interrupt_vector(INT_KB,&si_okbint);
  si_kbint.pm_selector = _go32_my_cs();
  si_kbint.pm_offset = (unsigned long)kbint;
  _go32_dpmi_allocate_iret_wrapper(&si_kbint);
  _go32_dpmi_set_protected_mode_interrupt_vector(INT_KB,&si_kbint);*/
}

//---------------------------------------------------------------------------
void resetpit()
{/*
  outportb(PORT_PIT_CMD,PIT_CONTROL);
  outportb(PORT_PIT_DATA,0x00);
  outportb(PORT_PIT_DATA,0x00);

  _go32_dpmi_set_protected_mode_interrupt_vector(INT_KB,&si_opitint);
  _go32_dpmi_free_iret_wrapper(&si_pitint);*/
}

//---------------------------------------------------------------------------
void resetkb()
{/*
  _go32_dpmi_set_protected_mode_interrupt_vector(INT_KB,&si_okbint);
  _go32_dpmi_free_iret_wrapper(&si_kbint);*/
}

//---------------------------------------------------------------------------
void init()
{
//  _go32_dpmi_get_free_memory_information(&freememinfo);
//  setcbrk(false);
  atexit(uninit);
//  setgraph();
  setpalette();
  initpit();
  initkb();
  if (videobuffer == NULL) {
    exittype = EXIT_NOMEM;
    exit(EXIT_FAILURE);
  }
//  _farsetsel(_dos_ds);
}

//---------------------------------------------------------------------------
void uninit()
{
  resetpit();
  resetkb();

  switch (exittype) {
    case EXIT_BREAK :
      cerr << endl
           << "Abnormal program termination" << endl;
      break;
    case EXIT_USER :
//      settext();
      cout << "User exit" << endl;
    case EXIT_NORMAL :
      if (numints != 0)
        cout << SPEED * numframes / numints << " fps" << endl;
      break;
    case EXIT_NOMEM :
//      settext();
      cerr << "Not enough memory" << endl
//           << freememinfo.available_memory / 1024
//           << " kbytes memory free" << endl
           << (requiredmemory + 1023) / 1024
           << " kbytes memory required" << endl;
  }
}

