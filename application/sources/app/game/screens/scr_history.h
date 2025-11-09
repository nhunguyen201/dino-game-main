#ifndef SCR_HISTORY_H
#define SCR_HISTORY_H

#include "screen_manager.h"
#include "ak.h"

#ifdef __cplusplus
extern "C"
{
#endif

extern view_dynamic_t dyn_view_history;
extern view_screen_t scr_history;

extern void task_scr_history_handle(ak_msg_t *msg);

#ifdef __cplusplus
}
#endif

#endif // SCR_HISTORY_H