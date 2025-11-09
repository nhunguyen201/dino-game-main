#include "dino_obstacle.h"
#include "dino_config.h"
#include "dino_bitmap.h" 
#include "view_render.h"
#include "rect.h"
#include "dino_game.h" 
#include "scr_startup.h" 
#include <cstdlib> 
#include <algorithm>
#include <cstddef>

#define DINO_OBSTACLE_MAX 		1 	/* Chỉ 1 vật cản tại 1 thời điểm */

/* STRUCT & VARIABLE*/
typedef enum {
	TYPE_CACTUS_SMALL,
	TYPE_CACTUS_BIG,
	TYPE_BIRD,
	TYPE_MAX
} obstacle_type_t;

typedef struct {
	int16_t x, y, w, h;
	bool active;
	obstacle_type_t type;
} dino_obstacle_t;

static dino_obstacle_t obstacles[DINO_OBSTACLE_MAX];

/*Hàm nội bộ để tạo vật cản mới*/
static void respawn_obstacle(dino_obstacle_t* obs) {
	/* Chọn ngẫu nhiên một loại */
	obs->type = static_cast<obstacle_type_t>(rand() % TYPE_MAX);

	obs->x = LCD_WIDTH + (rand() % 30); /* Hơi ngẫu nhiên ở bên phải */
	obs->active = true;

	switch (obs->type) {
		case TYPE_CACTUS_SMALL:
			obs->w = CACTUS_SMALL_W;
			obs->h = CACTUS_SMALL_H;
			obs->y = DINO_GROUND_Y - obs->h;
			break;
		case TYPE_CACTUS_BIG:
			obs->w = CACTUS_BIG_W;
			obs->h = CACTUS_BIG_H;
			obs->y = DINO_GROUND_Y - obs->h;
			break;

		case TYPE_BIRD: { 
			obs->w = BIRD_WIDTH;
			obs->h = BIRD_HEIGHT;
			int bird_level = rand() % 3;
			if (bird_level == 0) obs->y = DINO_GROUND_Y - obs->h; 
			else if (bird_level == 1) obs->y = DINO_GROUND_Y - obs->h - 10; 
			else obs->y = DINO_GROUND_Y - obs->h - 18; 
			break;
		} 
		case TYPE_MAX:
		default:
			obs->active = false;
			break;
	}
}


/**INIT / RESET*/
extern "C" void dino_obstacle_init() {
	for (int i = 0; i < DINO_OBSTACLE_MAX; i++) {
		obstacles[i].active = false;
	}

	/* Tạo vật cản đầu tiên */
	respawn_obstacle(&obstacles[0]);
}

/* UPDATE (MOVE OBSTACLE)*/
extern "C" void dino_obstacle_update() {
	for (int i = 0; i < DINO_OBSTACLE_MAX; i++) {
		if (!obstacles[i].active) continue;

		obstacles[i].x -= static_cast<int16_t>(game_speed);

		/* Nếu chạy ra khỏi màn hình → reset lại */
		if (obstacles[i].x + obstacles[i].w < 0) {
			respawn_obstacle(&obstacles[i]);
		}
	}
}

/* DRAW OBSTACLE*/
extern "C" void dino_obstacle_draw() {
	const unsigned char* bitmap_to_draw = nullptr;

	for (int i = 0; i < DINO_OBSTACLE_MAX; i++) {
		if (!obstacles[i].active) continue;
		switch (obstacles[i].type) {
			case TYPE_CACTUS_SMALL:
				bitmap_to_draw = cactus_small_bitmap;
				break;
			case TYPE_CACTUS_BIG:
				bitmap_to_draw = cactus_big_bitmap;
				break;
			case TYPE_BIRD:
				bitmap_to_draw = bird_fly1_bitmap; 
				break;
			default:
				bitmap_to_draw = nullptr;
				break;
		}

		if (bitmap_to_draw) {
			view_render.drawBitmap(
				obstacles[i].x,
				obstacles[i].y,
				bitmap_to_draw,
				obstacles[i].w, 
				obstacles[i].h,
				WHITE
			);
		}
	}
}

/***********************************************************
* CHECK COLLISION
***********************************************************/
extern "C" bool dino_obstacle_check_collision(rect_t dino_rect) {
	for (int i = 0; i < DINO_OBSTACLE_MAX; i++) {
		if (!obstacles[i].active) continue;

		rect_t obs_rect = {
			obstacles[i].x,
			obstacles[i].y,
			obstacles[i].w,
			obstacles[i].h
		};

		if (rect_intersect(&dino_rect, &obs_rect)) {
			return true;
		}
	}
	return false;
}