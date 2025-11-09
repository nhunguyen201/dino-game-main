#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "ak.h"
#include "task.h"
#include "timer.h"

/* TASK ID SYSTEM */
#define TASK_TIMER_TICK_ID 		0x01
#define AC_TASK_SYSTEM_ID 		0x02
#define AC_TASK_FW_ID 			0x03
#define AC_TASK_SHELL_ID 		0x04
#define AC_TASK_LIFE_ID 		0x05
#define AC_TASK_IF_ID 			0x06
#define AC_TASK_UART_IF_ID 		0x07
#define AC_TASK_DBG_ID 			0x08
#define AC_TASK_DISPLAY_ID 		0x09

/* DINO GAME TASK */
#define AC_TASK_SCR_MENU_ID 	0x20
#define AC_TASK_SCR_GAME_ON_ID 	0x21
#define AC_TASK_SCR_SETTING_ID 	0x22

/* LINK TASK */
#define AC_LINK_ID 				0x30 	 
#define AC_LINK_MAC_ID 			0x31 	 
#define AC_LINK_PHY_ID 			0x32 	 

/* END OF TASK */
#define AK_TASK_EOT_ID 			0xFF
#define AK_TASK_POLLING_EOT_ID 	0xFF

/* EXTERN TASK HANDLERS */
/* SYSTEM TASKS */
extern void task_timer_tick(ak_msg_t* msg);
extern void task_system(ak_msg_t* msg);
extern void task_fw(ak_msg_t* msg);
extern void task_shell(ak_msg_t* msg);
extern void task_life(ak_msg_t* msg);
extern void task_if(ak_msg_t* msg);
extern void task_uart_if(ak_msg_t* msg);
extern void task_dbg(ak_msg_t* msg);
extern void task_display(ak_msg_t* msg);

/* DINO GAME HANDLERS */
extern void task_scr_menu_handle(ak_msg_t* msg);
extern void task_scr_game_on_handle(ak_msg_t* msg);
extern void task_scr_setting_handle(ak_msg_t* msg);

extern void task_link_handle(ak_msg_t* msg); 		
extern void task_link_mac_handle(ak_msg_t* msg); 	
extern void task_link_phy_handle(ak_msg_t* msg); 	

extern const task_t app_task_table[];
extern const task_polling_t app_task_polling_table[];

#ifdef __cplusplus
}
#endif

#endif /* __TASK_LIST_H__ */