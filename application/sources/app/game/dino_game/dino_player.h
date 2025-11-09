#ifndef DINO_PLAYER_H
#define DINO_PLAYER_H

#include "dino_config.h"
typedef struct {
	dino_coordinate_t pos;
	int velocityY;
	dino_state_t state;
	bool is_alive;
	uint8_t anim_frame; 
} dino_player_t;


#ifdef __cplusplus
extern "C"
{
#endif

extern dino_player_t dino;
void dino_player_init(void);
void dino_jump(void);
void dino_player_update(void);
void dino_player_draw(void);
void dino_player_die(void); 

#ifdef __cplusplus
}
#endif

#endif // DINO_PLAYER_H