#ifndef DINO_CONFIG_H
#define DINO_CONFIG_H

#include <stdio.h>
#include "ak.h"
#include <stdbool.h> 

#ifdef __cplusplus
extern "C"
{
#endif

#define DINO_GROUND_Y 		(50)
#define DINO_JUMP_HEIGHT 	(10)
#define DINO_GRAVITY 		(1)

#define DINO_MAX_HISTORY 	(5)

typedef enum {
	DINO_GAMESTATE_MENU = 0,
	DINO_GAMESTATE_PLAYING,
	DINO_GAMESTATE_PAUSED,
	DINO_GAMESTATE_GAMEOVER
} dino_game_state_t;

typedef enum {
	DINO_GAME_MODE_FIRST = 1,
	DINO_GAME_MODE_EASY,
	DINO_GAME_MODE_NORMAL,
	DINO_GAME_MODE_HARD,
	DINO_GAME_MODE_END
} dino_game_mode_t;

#define DINO_MODE_OPTION_MAX (DINO_GAME_MODE_END - DINO_GAME_MODE_FIRST - 1)

typedef struct {
	int16_t x;
	int16_t y;
} dino_coordinate_t;

typedef struct {
	bool dino_setting_sound;
	dino_game_mode_t dino_setting_game_mode;
} dino_game_setting_t;

typedef enum {
	DINO_STATE_RUNNING = 0,
	DINO_STATE_JUMPING,
	DINO_STATE_FALLING,
	DINO_STATE_DEAD
} dino_state_t;

#define DINO_MENU_SELECT_SIG 		(AK_USER_DEFINE_SIG + 1)
#define DINO_MENU_UP_SIG 			(AK_USER_DEFINE_SIG + 2)
#define DINO_MENU_DOWN_SIG 			(AK_USER_DEFINE_SIG + 3)
#define DINO_MENU_LONG_PRESS_SIG 	(AK_USER_DEFINE_SIG + 4)

#define DINO_GAME_PAUSE_SIG 		(AK_USER_DEFINE_SIG + 5)
#define DINO_GAME_RESTART_SIG 		(AK_USER_DEFINE_SIG + 6)
#define DINO_GAME_OVER_SIG 			(AK_USER_DEFINE_SIG + 7)

#define DINO_PLAYER_JUMP_SIG 		(AK_USER_DEFINE_SIG + 8)
#define DINO_PLAYER_DUCK_SIG 		(AK_USER_DEFINE_SIG + 9)

#define AC_DISPLAY_UPDATE_SIG 		(AK_USER_DEFINE_SIG + 20)
#define AC_DISPLAY_GAME_TICK_INTERVAL (33)

#ifdef __cplusplus
}
#endif

#endif 	// DINO_CONFIG_H