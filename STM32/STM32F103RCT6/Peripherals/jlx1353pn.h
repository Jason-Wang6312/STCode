#ifndef __JLX1353PN_H
#define __JLX1353PN_H
#include "sys.h"
#include "delay.h"

#define JLX_CS  PBout(3)
#define JLX_RST PBout(4)
#define JLX_RS  PBout(5)
#define JLX_SDA PBout(6)
#define JLX_SLK PBout(7)


void JLX1353PNInit(void);
void JLXClearScreen(void);

#endif
