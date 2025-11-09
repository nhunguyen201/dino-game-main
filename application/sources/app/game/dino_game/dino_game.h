#ifndef DINO_GAME_H
#define DINO_GAME_H

#include <stdint.h>
#include <stdbool.h>
#include "dino_config.h"
#include "task_list.h"

extern float game_speed;

#ifdef __cplusplus
extern "C"
{
#endif

#define DINO_TASK_GAME_ID 	AC_TASK_DISPLAY_ID

/* Khai báo các hàm chính của game */
void dino_game_init(void);
void dino_game_update(void);
void dino_game_draw(void);
void dino_game_handle_input(uint32_t sig); 

#ifdef __cplusplus
}
#endif

#endif // DINO_GAME_H