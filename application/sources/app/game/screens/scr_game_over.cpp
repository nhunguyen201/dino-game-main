#include "scr_game_over.h"
#include "app_dbg.h"
#include "screen_manager.h"
#include "view_render.h"
#include "scr_menu.h"
#include "scr_game_on.h"
#include "buzzer.h"
#include <cstddef> 

static int selected_item = 0;
static const char* game_over_items[] = {
	"Replay",
	"Menu"
};
#define GAME_OVER_ITEM_COUNT 2

static void view_scr_game_over();

view_dynamic_t dyn_view_game_over = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_over
};

view_screen_t scr_game_over = {
	&dyn_view_game_over,
	nullptr,
	nullptr,
	.focus_item = 0,
};

static void view_scr_game_over() {
	view_render.clear();
	view_render.setTextColor(WHITE);
	view_render.setTextSize(2);
	view_render.setCursor(20, 10);
	view_render.print("GAME OVER");

	view_render.setTextSize(1);
	for (int i = 0; i < GAME_OVER_ITEM_COUNT; i++) {
		view_render.setCursor(25, 44 + i * 20);

		if (i == selected_item) {
			view_render.fillRect(15, 40 + i * 20, 90, 18, WHITE);
			view_render.setTextColor(BLACK);
			view_render.print(game_over_items[i]);
			view_render.setTextColor(WHITE);
		} else {
			view_render.drawRect(15, 40 + i * 20, 90, 18, WHITE);
			view_render.print(game_over_items[i]);
		}
	}
}

extern "C" void task_scr_game_over_handle(ak_msg_t* msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY: GAME_OVER\n");
			selected_item = 0;
			break;
		}

		case AC_DISPLAY_BUTON_UP_RELEASED: {
			if (selected_item > 0) {
				selected_item--;
				buzzer_playMenuMove();
			}
			break;
		}

		case AC_DISPLAY_BUTON_DOWN_RELEASED: {
			if (selected_item < GAME_OVER_ITEM_COUNT - 1) {
				selected_item++;
				buzzer_playMenuMove();
			}
			break;
		}

		case AC_DISPLAY_BUTON_MODE_PRESSED: {
			buzzer_playMenuSelect();
			if (selected_item == 0) {
				SCREEN_TRAN(task_scr_game_on_handle, &scr_game_on);
			} else if (selected_item == 1) {
				SCREEN_TRAN(task_scr_menu_handle, &scr_menu);
			}
			break;
		}

		default:
			break;
	}
}