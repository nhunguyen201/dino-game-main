#ifndef DINO_BITMAP_H
#define DINO_BITMAP_H

#include "pgmspace.h"

// DINO BITMAPS
extern const unsigned char dino_standing_bitmap[];
extern const unsigned char dino_run1_bitmap[];
extern const unsigned char dino_run2_bitmap[];
extern const unsigned char dino_dead_bitmap[];

// CACTUS 
extern const unsigned char cactus_small_bitmap[];
extern const unsigned char cactus_big_bitmap[];

// BIRD (CHIM)
extern const unsigned char bird_fly1_bitmap[];
extern const unsigned char bird_fly2_bitmap[];

// CLOUD (MÂY)
extern const unsigned char cloud_bitmap[];

// KÍCH THƯỚC CỦA TỪNG BITMAP 
#define DINO_WIDTH        20
#define DINO_HEIGHT       24

#define BIRD_WIDTH        20
#define BIRD_HEIGHT       17

#define CACTUS_SMALL_W    12
#define CACTUS_SMALL_H    12
#define CACTUS_BIG_W      18
#define CACTUS_BIG_H      12

#define CLOUD_WIDTH       12
#define CLOUD_HEIGHT      12


#endif  //DINO_BITMAP_H