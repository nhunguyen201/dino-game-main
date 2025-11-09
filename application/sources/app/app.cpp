/**
 ******************************************************************************
 * @author: GaoKong / chỉnh sửa bởi Minh Như
 * @date:   31/10/2025
 * @desc:   Main application file for DINO GAME on AK Base Kit
 ******************************************************************************
 **/

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <queue>
#include <map>
#include <functional>
#include <iterator> 

/* kernel include */
#include "ak.h"
#include "fsm.h"
#include "message.h"
#include "timer.h"

/* driver include */
#include "button.h"
#include "flash.h"
#include "led.h"

/* app include */
#include "app.h"
#include "app_bsp.h" 
#include "app_dbg.h"
#include "app_flash.h"
#include "app_non_clear_ram.h"
#include "task_display.h"
#include "task_if.h"
#include "task_life.h"
#include "task_list.h"
#include "task_shell.h"
#include "task_uart_if.h"

#include "screen_manager.h"
#include "buzzer.h"

/* arduino include */
#include "HardwareSerial.h"
#include "SPI.h"
#include "WString.h"

/* sys include */
#include "sys_boot.h"
#include "sys_ctrl.h"
#include "sys_dbg.h"
#include "sys_io.h"
#include "sys_irq.h"

/* DINO includes */
#include "scr_menu.h"
#include "scr_game_on.h"
#include "scr_setting.h"
#include "scr_game_over.h"
#include "scr_startup.h" 

/***********************************************************
 * VERSION INFO
 ***********************************************************/
#if defined(RELEASE)
const char* app_run_mode = "RELEASE";
#else
static const char* app_run_mode = "DEBUG";
#endif

const app_info_t app_info{
	APP_MAGIC_NUMBER,
	APP_VER,
};

/***********************************************************
 * STATIC VARIABLES
 ***********************************************************/
static boot_app_share_data_t boot_app_share_data;

static void app_power_on_reset();
static void app_start_timer();
static void app_init_state_machine();
static void app_task_init();

/***********************************************************
 * MAIN APP FUNCTION
 ***********************************************************/
int main_app() {
	APP_PRINT("=== DINO GAME START ===\n");
	APP_PRINT("Mode: %s | Version: %d.%d.%d.%d\n",
				app_run_mode,
				app_info.version[0], app_info.version[1],
				app_info.version[2], app_info.version[3]);

	sys_soft_reboot_counter++;

	/* Init kernel tasks */
	ENTRY_CRITICAL();
	task_init();
	task_create((task_t*)app_task_table);
	task_polling_create((task_polling_t*)app_task_polling_table);
	EXIT_CRITICAL();

	/***********************************************************
	 * Hardware init
	 ***********************************************************/
	sys_ctrl_independent_watchdog_init(); /* 32s watchdog */
	sys_ctrl_soft_watchdog_init(200); 	/* 20s soft watchdog */

	SPI.begin();

	io_cfg_adc1();
	adc_bat_io_cfg();
	flash_io_ctrl_init();

	/* Initialize boot system */
	sys_boot_init();

	/* LED and button init */
	led_init(&led_life, led_life_init, led_life_on, led_life_off);

	button_init(&btn_mode, 10, BUTTON_MODE_ID, io_button_mode_init, io_button_mode_read, btn_mode_callback);
	button_init(&btn_up, 10, BUTTON_UP_ID, io_button_up_init, io_button_up_read, btn_up_callback);
	button_init(&btn_down, 10, BUTTON_DOWN_ID, io_button_down_init, io_button_down_read, btn_down_callback);

	button_enable(&btn_mode);
	button_enable(&btn_up);
	button_enable(&btn_down);

	/* Buzzer init */
	BUZZER_Init();
	BUZZER_PlayTones(tones_startup);

	/* Read share data */
	flash_read(APP_FLASH_INTTERNAL_SHARE_DATA_SECTOR_1,
				reinterpret_cast<uint8_t*>(&boot_app_share_data),
				sizeof(boot_app_share_data_t));

	if (boot_app_share_data.is_power_on_reset == SYS_POWER_ON_RESET) {
		app_power_on_reset();
	}

	/* Timer setup */
	app_init_state_machine();
	app_start_timer();

	/* Initialize tasks */
	app_task_init();

	/* Start scheduler */
#if !defined(IF_LINK_UART_EN)
	sys_ctrl_shell_sw_to_nonblock();
#endif

	return task_run();
}

/***********************************************************
 * TIMER SETUP
 ***********************************************************/
void app_start_timer() {
	timer_set(AC_TASK_LIFE_ID, AC_LIFE_SYSTEM_CHECK,
				AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL, TIMER_PERIODIC);

	timer_set(AC_TASK_FW_ID, FW_CHECKING_REQ,
				FW_UPDATE_REQ_INTERVAL, TIMER_ONE_SHOT);

	timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_INITIAL,
				AC_DISPLAY_INITIAL_INTERVAL, TIMER_ONE_SHOT);
}

/***********************************************************
 * STATE MACHINE INIT
 ***********************************************************/
void app_init_state_machine() {
	/* No FSM init needed for this simple app */
}

/***********************************************************
 * TASK INITIALIZATION
 ***********************************************************/
void app_task_init() {
	SCREEN_CTOR(&scr_mng_app, task_scr_dino_startup_handle, &scr_startup);

	task_post_pure_msg(AC_TASK_UART_IF_ID, AC_UART_IF_INIT);
}

/***********************************************************
 * TIMER IRQ - 10ms
 ***********************************************************/
void sys_irq_timer_10ms() {
	button_timer_polling(&btn_mode);
	button_timer_polling(&btn_up);
	button_timer_polling(&btn_down);
}

/***********************************************************
 * POWER ON RESET HANDLER
 ***********************************************************/
void app_power_on_reset() {
	sys_soft_reboot_counter = 0;
}

/***********************************************************
 * BOOT DATA ACCESS
 ***********************************************************/
void* app_get_boot_share_data() {
	return static_cast<void*>(&boot_app_share_data);
}