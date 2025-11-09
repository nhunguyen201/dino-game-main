#ifndef __SCR_IDLE_H__
#define __SCR_IDLE_H__
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

#ifdef __cplusplus
extern "C"
{
#endif

extern view_dynamic_t dyn_view_idle;
extern view_screen_t scr_idle;

extern void task_scr_idle_handle(ak_msg_t* msg);

#ifdef __cplusplus
}
#endif

#endif //__SCR_IDLE_H__