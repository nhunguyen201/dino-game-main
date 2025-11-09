#include "scr_setting.h" 
#include "view_render.h"
#include "scr_menu.h"
#include "buzzer.h" 
#include "dino_config.h" 
#include "scr_startup.h"
#include <cstddef> 

static int focus_index = 0;

static void view_scr_setting();

view_dynamic_t dyn_view_setting = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_setting
};

view_screen_t scr_setting = {
	&dyn_view_setting,
	nullptr, 
	nullptr,
	.focus_item = 0,
};

static void view_scr_setting() {
	// Xóa màn hình
	view_render.clear();
	view_render.setTextColor(WHITE);
	view_render.setTextSize(2);
	view_render.setCursor(20, 10);
	view_render.print("SETTINGS"); 

	view_render.setTextSize(1);

	// Sound
	view_render.setCursor(10, 40);
	if (focus_index == 0) view_render.print("> ");
	else view_render.print("  ");
	view_render.print("Sound: ");
	view_render.print(dino_game_setting.dino_setting_sound ? "ON" : "OFF");

	// Mode
	view_render.setCursor(10, 55);
	if (focus_index == 1) view_render.print("> ");
	else view_render.print("  ");
	view_render.print("Mode: ");
	view_render.print(dino_game_setting.dino_setting_game_mode == DINO_GAME_MODE_HARD ? "Hard" : "Easy");

	// Exit
	view_render.setCursor(10, 70);
	if (focus_index == 2) view_render.print("> ");
	else view_render.print("  ");
	view_render.print("Exit");
}

extern "C" void task_scr_setting_handle(ak_msg_t* msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY: SETTING\n");
			focus_index = 0;
			break;
		}

		case AC_DISPLAY_BUTON_UP_RELEASED: {
			if (focus_index > 0) {
				focus_index--;
				buzzer_playMenuMove();
			}
			break;
		}

		case AC_DISPLAY_BUTON_DOWN_RELEASED: {
			if (focus_index < 2) { 
				focus_index++;
				buzzer_playMenuMove();
			}
			break;
		}

		case AC_DISPLAY_BUTON_MODE_PRESSED: {
			if (focus_index == 0) {
				dino_game_setting.dino_setting_sound = !dino_game_setting.dino_setting_sound;
				buzzer_playToggleSound(); 
			} 
			else if (focus_index == 1) {
				if (dino_game_setting.dino_setting_game_mode == DINO_GAME_MODE_EASY) {
					dino_game_setting.dino_setting_game_mode = DINO_GAME_MODE_HARD;
				} else {
					dino_game_setting.dino_setting_game_mode = DINO_GAME_MODE_EASY;
				}
				buzzer_playToggleSound(); 
			} 
			else if (focus_index == 2) {
				buzzer_playMenuSelect(); 
				SCREEN_TRAN(task_scr_menu_handle, &scr_menu);
			}
			break;
		}

		default:
			break;
	}
}