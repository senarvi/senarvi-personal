#ifndef _TUNNEL_H_INCLUDED
#ifdef __cplusplus
extern "C" {
#endif

#include "system.h"
#include "video.h"


extern udword tunnel_fwd, tunnel_roll;
extern udword tunneldepth[ GR_SIZE ];


extern void _settunnel(), _setwave();
extern void _drawtunnel();
extern void set_tunnel();


#define _TUNNEL_H_INCLUDED
#ifdef __cplusplus
};
#endif
#endif

