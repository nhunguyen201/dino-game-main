#include "scr_menu.h"
#include "app_dbg.h"
#include "screen_manager.h"
#include "view_render.h"
#include "scr_game_on.h" 
#include "scr_setting.h" 
#include "dino_bitmap.h"
#include "buzzer.h" 
#include <cstddef> 

/*Định nghĩa menu*/
#define MENU_ITEM_COUNT 2
static const char* menu_items[MENU_ITEM_COUNT] = {
	"Start Game",
	"Setting"
};

static int menu_index = 0;

/*Khai báo hàm nội bộ*/
static void view_scr_menu();

/*Cấu hình màn hình động*/
view_dynamic_t dyn_view_menu = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_menu
};

/*Đối tượng màn hình MENU*/
view_screen_t scr_menu = {
	&dyn_view_menu,
	nullptr, 
	nullptr,
	.focus_item = 0,
};

/*Hiển thị MENU*/
static void view_scr_menu() {
	view_render.clear();
	view_render.setTextColor(WHITE);
	view_render.setTextSize(2);

	// Tiêu đề
	view_render.setCursor(20, 10);
	view_render.print("DINO GAME");

	// Hiển thị danh sách lựa chọn
	view_render.setTextSize(1); 
	for (int i = 0; i < MENU_ITEM_COUNT; i++) {
		view_render.setCursor(20, 43 + i * 22);
		
		if (i == menu_index) {
			view_render.setTextColor(BLACK);
			view_render.fillRoundRect(10, 40 + i * 22, 110, 20, 3, WHITE);
			view_render.print(menu_items[i]);
			view_render.setTextColor(WHITE); 
		} else {
			view_render.drawRoundRect(10, 40 + i * 22, 110, 20, 3, WHITE);
			view_render.print(menu_items[i]);
		}
	}
}

extern "C" void task_scr_menu_handle(ak_msg_t* msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY: MENU\n");
			menu_index = 0;
			break;
		}

		case AC_DISPLAY_BUTON_UP_RELEASED: {
			if (menu_index > 0) {
				menu_index--;
				buzzer_playMenuMove();
			}
			break;
		}

		case AC_DISPLAY_BUTON_DOWN_RELEASED: {
			if (menu_index < MENU_ITEM_COUNT - 1) {
				menu_index++;
				buzzer_playMenuMove();
			}
			break;
		}

		case AC_DISPLAY_BUTON_MODE_PRESSED: {
			buzzer_playMenuSelect();

			switch (menu_index) {
				case 0: // Start Game
					SCREEN_TRAN(task_scr_game_on_handle, &scr_game_on);
					break;

				case 1: // Setting
					SCREEN_TRAN(task_scr_setting_handle, &scr_setting);
					break;

				default:
					break;
			}
			break;
		}

		default:
			break;
	}
}