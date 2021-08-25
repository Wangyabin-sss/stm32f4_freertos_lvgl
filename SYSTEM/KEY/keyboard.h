#ifndef _KEY_BOARD_H
#define _KEY_BOARD_H

#include "Sys.h"

#define line1 PAout(8)
#define line2 PEout(6)
#define line3 PBout(6)
#define line4 PCout(9)
#define line5 PCin(7)
#define line6 PGin(15)
#define line7 PAin(4)
#define line8 PBin(7)

void key_board_init(void);
uint32_t key_board_scan(void);


#endif 
