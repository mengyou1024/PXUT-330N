#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "sysdef.h"
#include "M10Era.h"

#define RGB(r, g ,b)  ((uint32_t) (((uint8_t) (r) | \
    ((uint16_t) (g) << 8)) | \
    (((uint32_t) (uint8_t) (b)) << 16)))


void delay(uint32_t interval);
uint32_t drawpixel(uint32_t x,uint32_t y,uint32_t color);

void   _trace(int x, int y,char   *prompt,   ...);

//#define _DEBUG

#ifdef _DEBUG
#define   TRACE      // _trace
#else
#define   TRACE
#endif

extern int g_ypos;
extern int g_xpos;




#endif
