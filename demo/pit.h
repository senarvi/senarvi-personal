#ifndef _PIT_H_INCLUDED


#define PIT0_PORT   (0x40 + 0)
#define PIT1_PORT   (0x40 + 1)
#define PIT2_PORT   (0x40 + 2)
#define PITC_PORT   (0x40 + 3)


#define PIT_SEL0    (0 << 6)
#define PIT_SEL1    (1 << 6)
#define PIT_SEL2    (2 << 6)

#define PIT_LATCH   (0 << 4)
#define PIT_LOADLSB (1 << 4)
#define PIT_LOADMSB (2 << 4)
#define PIT_LOADALL (3 << 4)

#define PIT_MODE0   (0 << 1)
#define PIT_MODE1   (1 << 1)
#define PIT_MODE2   (2 << 1)
#define PIT_MODE3   (3 << 1)
#define PIT_MODE4   (4 << 1)
#define PIT_MODE5   (5 << 1)

#define PIT_BIN16   (0 << 0)
#define PIT_BCD4    (1 << 0)

#define PIT_FREQUENCY 0x1234dd

#define PIT_CONTROLW (PIT_SEL0 + PIT_LOADALL + PIT_MODE2 + PIT_BIN16)


extern void set_pit_freq( uword frequency );
extern void reset_pit_freq();


#define _PIT_H_INCLUDED
#endif
