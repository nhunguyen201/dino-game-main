#include "scr_startup.h"
#include "app_eeprom.h"
#include "eeprom.h"
#include "scr_menu.h"
#include "scr_idle.h" 
#include "dino_game.h"
#include "dino_bitmap.h"
#include "view_render.h"
#include <cstddef> 

dino_game_setting_t dino_game_setting;
dino_game_state_t 	dino_state_game = DINO_GAMESTATE_MENU; 
uint16_t 			dino_game_score = 0;
uint16_t 			dino_game_score_history[DINO_MAX_HISTORY] = {0};


extern "C" void dino_init_setting() {
	dino_game_setting.dino_setting_game_mode = DINO_GAME_MODE_EASY;
	dino_game_setting.dino_setting_sound = true;
}


static void view_scr_dino_startup() {
#define DINO_LOGO_AXIS_X (23)
#define DINO_LOGO_TEXT (DINO_LOGO_AXIS_X + 4)

	view_render.clear();
	view_render.setTextSize(1);
	view_render.setTextColor(WHITE);
	view_render.setCursor(DINO_LOGO_AXIS_X, 3);
	view_render.print("   __    _ _ ");
	view_render.setCursor(DINO_LOGO_AXIS_X, 10);
	view_render.print("  /__\\  ( )/ )");
	view_render.setCursor(DINO_LOGO_AXIS_X, 20);
	view_render.print(" /(__)\\ (   (");
	view_render.setCursor(DINO_LOGO_AXIS_X, 30);
	view_render.print("(__)(__)(_)\\_)");
	view_render.setCursor(DINO_LOGO_TEXT, 42);
	view_render.print("DINO GAME");
}

/*KHAI BÁO VIEW + SCREEN*/
view_dynamic_t dyn_view_startup = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_dino_startup
};

view_screen_t scr_startup = {
	&dyn_view_startup,
	nullptr, 
	nullptr,
	.focus_item = 0,
};

/*TASK STARTUP HANDLE */
extern "C" void task_scr_dino_startup_handle(ak_msg_t *msg) {
	switch (msg->sig) {
	case AC_DISPLAY_INITIAL: {
		view_render.initialize();
		view_render_display_on(); 

		/* Đọc dữ liệu EEPROM */
		eeprom_read(EEPROM_HISTORY_ADDR,
					reinterpret_cast<uint8_t*>(&dino_game_score_history),
					sizeof(dino_game_score_history)); 

		eeprom_read(EEPROM_SETTING_ADDR,
					reinterpret_cast<uint8_t*>(&dino_game_setting),
					sizeof(dino_game_setting));

		/* Nếu EEPROM trống, khởi tạo mặc định */
		if (dino_game_setting.dino_setting_game_mode < DINO_GAME_MODE_FIRST ||
			dino_game_setting.dino_setting_game_mode >= DINO_GAME_MODE_END) {
			dino_init_setting();
		}

		timer_set(AC_TASK_DISPLAY_ID,
					AC_DISPLAY_SHOW_MENU,
					AC_DISPLAY_STARTUP_INTERVAL, 
					TIMER_ONE_SHOT);
		break;
	}

	
	case AC_DISPLAY_BUTON_MODE_RELEASED:
		timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_MENU); 
		SCREEN_TRAN(task_scr_menu_handle, &scr_menu);
		break;

	
	case AC_DISPLAY_SHOW_MENU:
		SCREEN_TRAN(task_scr_menu_handle, &scr_menu);
		break;

	
	case AC_DISPLAY_SHOW_IDLE:
		SCREEN_TRAN(task_scr_idle_handle, &scr_idle);
		break;

	default:
		break;
	}
}