#include "app_bsp.h"
#include "app.h"
#include "app_dbg.h"
#include "app_if.h"
#include "task_list.h"
#include "task_list_if.h"
#include <cstddef> 


button_t btn_mode;
button_t btn_up;
button_t btn_down;

extern "C" void btn_mode_callback(void* b) {
	button_t* me_b = static_cast<button_t*>(b); 
	switch (me_b->state) {
		case BUTTON_SW_STATE_PRESSED:
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTON_MODE_PRESSED);
			break;
		case BUTTON_SW_STATE_RELEASED:
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTON_MODE_RELEASED);
			break;
		case BUTTON_SW_STATE_LONG_PRESSED:
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTON_MODE_LONG_PRESSED);
			break;
		default:
			break;
	}
}

extern "C" void btn_up_callback(void* b) {
	button_t* me_b = static_cast<button_t*>(b); 
	switch (me_b->state) {
		case BUTTON_SW_STATE_PRESSED:
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTON_UP_PRESSED);
			break;
		case BUTTON_SW_STATE_RELEASED:
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTON_UP_RELEASED);
			break;
		default:
			break;
	}
}

extern "C" void btn_down_callback(void* b) {
	button_t* me_b = static_cast<button_t*>(b); 
	switch (me_b->state) {
		case BUTTON_SW_STATE_PRESSED:
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTON_DOWN_PRESSED);
			break;
		case BUTTON_SW_STATE_RELEASED:
			task_post_pure_msg(AC_TASK_DISPLAY_ID, AC_DISPLAY_BUTON_DOWN_RELEASED);
			break;
		default:
			break;
	}
}