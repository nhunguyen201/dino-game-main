#include "dino_cloud.h"
#include "dino_bitmap.h"
#include "task_display.h" 
#include "view_render.h"
#include "dino_game.h" 
#include "scr_startup.h" 

#include <cstdlib> 
#define CLOUD_SPEED_RATIO (0.2f)


dino_cloud_t dino_clouds[DINO_MAX_CLOUDS];
static bool cloud_srand_called = false;


/* Khởi tạo mây*/
extern "C" void dino_cloud_init() {
	if (!cloud_srand_called) {
		srand(12345); 
		cloud_srand_called = true;
	}

	for (uint8_t i = 0; i < DINO_MAX_CLOUDS; i++) {
		dino_clouds[i].x = rand() % LCD_WIDTH;
		dino_clouds[i].y = (rand() % 20) + 5; /* Cho mây cao lên một chút */
	}
}

/*Cập nhật chuyển động mây*/
extern "C" void dino_cloud_update() {
	float cloud_speed_float = game_speed * CLOUD_SPEED_RATIO;
	if (cloud_speed_float < 1.0f) {
		cloud_speed_float = 1.0f;
	}

	int16_t cloud_speed_int = static_cast<int16_t>(cloud_speed_float);

	for (uint8_t i = 0; i < DINO_MAX_CLOUDS; i++) {
		dino_clouds[i].x -= cloud_speed_int;

		/* Nếu mây bay ra khỏi màn hình -> reset lại */
		if (dino_clouds[i].x < -CLOUD_WIDTH) {
			dino_clouds[i].x = LCD_WIDTH;
			dino_clouds[i].y = (rand() % 20) + 5;
		}
	}
}

/*Vẽ mây*/
extern "C" void dino_cloud_draw() {
	for (uint8_t i = 0; i < DINO_MAX_CLOUDS; i++) {
		view_render.drawBitmap(
			dino_clouds[i].x,
			dino_clouds[i].y,
			cloud_bitmap,
			CLOUD_WIDTH,
			CLOUD_HEIGHT,
			WHITE
		);
	}
}