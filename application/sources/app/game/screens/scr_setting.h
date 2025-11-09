#ifndef SCR_SETTING_H
#define SCR_SETTING_H

#include "screen_manager.h"
#include "ak.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern view_screen_t scr_setting;

void task_scr_setting_handle(ak_msg_t* msg);

#ifdef __cplusplus
}
#endif

#endif // SCR_SETTING_H