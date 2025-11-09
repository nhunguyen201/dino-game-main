#ifndef __SCR_STARTUP_H__
#define __SCR_STARTUP_H__

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"
#include "app.h"
#include "app_dbg.h"
#include "task_list.h"
#include "task_display.h"
#include "view_render.h"
#include "buzzer.h" 
#include "dino_config.h"
#ifdef __cplusplus
extern "C"
{
#endif

extern view_dynamic_t dyn_view_startup;
extern view_screen_t scr_startup;

extern void task_scr_dino_startup_handle(ak_msg_t* msg);

extern dino_game_state_t dino_state_game;
extern dino_game_setting_t dino_game_setting;
extern uint16_t dino_game_score;
extern uint16_t dino_game_score_history[DINO_MAX_HISTORY];

#ifdef __cplusplus
}
#endif

#endif // __SCR_STARTUP_H__