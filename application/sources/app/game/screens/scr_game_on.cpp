#include "scr_game_on.h"
#include "app_dbg.h"
#include "screen_manager.h"
#include "view_render.h"
#include "scr_menu.h"
#include "scr_game_over.h"
#include "timer.h" 

#include "scr_startup.h" 

#include "dino_game.h"
#include "dino_player.h"
#include "dino_config.h"
#include "buzzer.h" 
#include <cstddef> 

static void view_scr_game_on();

view_dynamic_t dyn_view_game_on = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_game_on
};

view_screen_t scr_game_on = {
	&dyn_view_game_on,
	nullptr,
	nullptr,
	.focus_item = 0,
};

static void view_scr_game_on() {
	dino_game_draw();
}

/*(Game Loop)*/
extern "C" void task_scr_game_on_handle(ak_msg_t* msg) {
	switch (msg->sig) {

		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY: GAME_ON\n");
			dino_game_init(); 
			dino_state_game = DINO_GAMESTATE_PLAYING; 
			
			timer_set(AC_TASK_DISPLAY_ID,
						AC_DISPLAY_UPDATE_SIG,
						AC_DISPLAY_GAME_TICK_INTERVAL,
						TIMER_PERIODIC);
			break;
		}

		case AC_DISPLAY_UPDATE_SIG: {
			dino_game_update(); 

			if (dino_state_game == DINO_GAMESTATE_GAMEOVER) {
				timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE_SIG);
				SCREEN_TRAN(task_scr_game_over_handle, &scr_game_over);
			}
			break;
		}

		case AC_DISPLAY_BUTON_UP_PRESSED: {
			if (dino_state_game == DINO_GAMESTATE_PLAYING) {
				dino_jump();
			}
			break;
		}

		case AC_DISPLAY_BUTON_MODE_PRESSED: {
			if (dino_state_game == DINO_GAMESTATE_PLAYING) {
				dino_state_game = DINO_GAMESTATE_PAUSED;
				timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE_SIG);
			}
			break;
		}
		
		case SCREEN_EXIT: {
			APP_DBG_SIG("SCREEN_EXIT: GAME_ON\n");
			timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_UPDATE_SIG);
			break;
		}

		default:
			break;
	}
}