#ifndef SCR_GAME_OVER_H
#define SCR_GAME_OVER_H

#include "ak.h"
#include "view_render.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern view_screen_t scr_game_over;
extern void task_scr_game_over_handle(ak_msg_t* msg);

#ifdef __cplusplus
}
#endif

#endif // SCR_GAME_OVER_H