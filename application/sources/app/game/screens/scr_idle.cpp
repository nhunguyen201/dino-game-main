#include "scr_idle.h"
#include "scr_menu.h"
#include "app_dbg.h"
#include "screen_manager.h"
#include "view_render.h"
#include "buzzer.h"
#include <cmath> 	
#include <cstdlib> 	
#include <array> 	
#include <algorithm> 

#define MAX_BALL_DISPLAY (16)

/***********************************************************
 * Class ball 
 ***********************************************************/
class ball {
public:
	static int total;

	int id, x, y, x_speed, y_speed, radius;

	ball() {
		x_speed = ((rand() % 2) + 1) * ((rand() % 2) ? 1 : -1);
		y_speed = ((rand() % 2) + 1) * ((rand() % 2) ? 1 : -1);
		
		radius = (rand() % 7) + 6;
		x = (rand() % (LCD_WIDTH - radius * 2)) + radius;
		y = (rand() % (LCD_HEIGHT - radius * 2)) + radius;
	}

	int distance_squared(ball& __ball) {
		int dx = x - __ball.x;
		int dy = y - __ball.y;
		return (dx * dx + dy * dy);
	}

	bool is_hit_to_other(ball& __ball) {
		int r = radius + __ball.radius;
		return (r * r) >= distance_squared(__ball);
	}

	void moving() {
		x += x_speed;
		y += y_speed;

		if (x > (LCD_WIDTH - radius) || x < radius) {
			x_speed = -x_speed;
			x = std::max(radius, std::min(x, LCD_WIDTH - radius));
		}

		if (y > (LCD_HEIGHT - radius) || y < radius) {
			y_speed = -y_speed;
			y = std::max(radius, std::min(y, LCD_HEIGHT - radius));
		}
	}
};

/***********************************************************
 * Khai báo biến
 ***********************************************************/
static void view_scr_idle();
static std::array<ball, MAX_BALL_DISPLAY> v_idle_ball;
static int ball_count = 0;
static bool srand_called = false;

/* SỬA LỖI: Định nghĩa 'ball::total' (giờ đã hợp lệ) */
int ball::total = 0; 

/***********************************************************
 * Đối tượng view và screen
 ***********************************************************/
view_dynamic_t dyn_view_idle = {
	{
		.item_type = ITEM_TYPE_DYNAMIC,
	},
	view_scr_idle
};

view_screen_t scr_idle = {
	&dyn_view_idle,
	nullptr,
	nullptr,
	.focus_item = 0,
};


 /*Hàm hiển thị*/

static void view_scr_idle() {
	view_render.clear();
	for (int i = 0; i < ball_count; i++) {
		view_render.drawCircle(v_idle_ball[i].x, v_idle_ball[i].y, v_idle_ball[i].radius, WHITE);
	}
}

/*Hàm xử lý tín hiệu*/
extern "C" void task_scr_idle_handle(ak_msg_t* msg) {
	switch (msg->sig) {
		case SCREEN_ENTRY: {
			APP_DBG_SIG("SCREEN_ENTRY: IDLE\n");

			if (!srand_called) {
				srand(12345); 
				srand_called = true;
			}
			
			if (ball_count == 0) {
				ball new_ball;
				new_ball.id = ball::total++;
				v_idle_ball[0] = new_ball;
				ball_count = 1;
			}

			timer_set(AC_TASK_DISPLAY_ID,
						AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE,
						AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE_INTERAL,
						TIMER_PERIODIC);
			break;
		}

		case AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE: {
			for (int i = 0; i < ball_count; i++) {
				v_idle_ball[i].moving();
			}
			break;
		}

		case AC_DISPLAY_BUTON_MODE_PRESSED: {
			APP_DBG_SIG("AC_DISPLAY_BUTON_MODE_PRESSED\n");
			timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);

			SCREEN_TRAN(task_scr_menu_handle, &scr_menu);
			break;
		}

		case AC_DISPLAY_BUTON_UP_RELEASED: {
			APP_DBG_SIG("AC_DISPLAY_BUTON_UP_RELEASED\n");

			if (ball_count == 0) {
				timer_set(AC_TASK_DISPLAY_ID,
							AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE,
							AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE_INTERAL,
							TIMER_PERIODIC);
			}

			if (ball_count < MAX_BALL_DISPLAY) {
				ball new_ball;
				new_ball.id = ball::total++;
				v_idle_ball[ball_count] = new_ball;
				ball_count++;
			} else {
				buzzer_playMenuSelect();
			}
			break;
		}

		case AC_DISPLAY_BUTON_DOWN_RELEASED: {
			APP_DBG_SIG("AC_DISPLAY_BUTON_DOWN_RELEASED\n");

			if (ball_count > 0) {
				ball::total--;
				ball_count--;
			}

			if (ball_count == 0) {
				timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_IDLE_BALL_MOVING_UPDATE);
			}
			break;
		}

		default:
			break;
	}
}