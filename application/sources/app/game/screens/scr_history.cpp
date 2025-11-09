#include "scr_history.h"
#include "app_dbg.h"
#include "screen_manager.h"
#include "view_render.h"
#include "scr_menu.h"
#include "buzzer.h" 
#include "dino_config.h" 
#include "scr_startup.h" 
#include <stdio.h> 

static int focus_index = 0;

static void view_scr_history();
view_dynamic_t dyn_view_history = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_history
};

view_screen_t scr_history = {
	&dyn_view_history,
	nullptr, 
	nullptr,
	.focus_item = 0,
};


static void view_scr_history() {
	char score_text[20]; 
	view_render.clear();
	view_render.setTextColor(WHITE);
	view_render.setTextSize(2);
	view_render.setCursor(30, 10);
	view_render.print("HISTORY");

	view_render.setTextSize(1);
	
	for (int i = 0; i < DINO_MAX_HISTORY; i++) {
		view_render.setCursor(20, 40 + i * 12);
		if (focus_index == i)
			view_render.print("> ");
		else
			view_render.print("  ");

		snprintf(score_text, sizeof(score_text), "Top %d: %u", 
				 i + 1, 
				 dino_game_score_history[i]); 
		
		view_render.print(score_text);
	}
}

extern "C" void task_scr_history_handle(ak_msg_t* msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY: HISTORY\n");
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
			if (focus_index < (DINO_MAX_HISTORY - 1)) {
				focus_index++;
				buzzer_playMenuMove();
			}
			break;
		}

		case AC_DISPLAY_BUTON_MODE_PRESSED: {
			buzzer_playMenuSelect();
			SCREEN_TRAN(task_scr_menu_handle, &scr_menu);
			break;
		}

		default:
			break;
	}
}