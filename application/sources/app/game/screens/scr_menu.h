#ifndef SCR_MENU_H
#define SCR_MENU_H

#include "ak.h"
#include "view_render.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern view_screen_t scr_menu;
extern void task_scr_menu_handle(ak_msg_t* msg);

#ifdef __cplusplus
}
#endif

#endif // SCR_MENU_H