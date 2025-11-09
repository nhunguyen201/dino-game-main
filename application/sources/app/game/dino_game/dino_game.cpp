#include "dino_game.h"
#include "dino_config.h"
#include "dino_player.h"
#include "dino_obstacle.h"
#include "dino_cloud.h" 
#include "view_render.h"
#include "buzzer.h"
#include "rect.h" 
#include "scr_startup.h" 

#include <stdio.h> 
#include <string.h> 
#include <cstddef> 

/* Định nghĩa biến tốc độ game*/
float game_speed = 3.0f; /* Tốc độ bắt đầu */
const float GAME_SPEED_INCREASE = 0.001f;
const float GAME_SPEED_MAX = 10.0f;


#define DINO_WIDTH 	20
#define DINO_HEIGHT 24

/*Màn hình game*/
static view_screen_t dino_screen;
static view_rectangle_t score_item;
static view_rectangle_t game_over_item; 

/*Khai báo các hàm render tĩnh*/
static void render_player() {
	dino_player_draw();
}

static void render_obstacles() {
	dino_obstacle_draw();
}
static void render_clouds() {
	dino_cloud_draw();
}

static view_dynamic_t dino_item;
static view_dynamic_t obstacle_item;
static view_dynamic_t cloud_item;


/*Khởi tạo game*/
extern "C" void dino_game_init() {
	dino_player_init();
	dino_obstacle_init();
	dino_cloud_init();

	dino_game_score = 0;
	game_speed = 3.0f; /* Reset tốc độ */
	
	view_render_init(); 

	/* Thiết lập item điểm */
	score_item.x = 5;
	score_item.y = 5;
	score_item.width = 50;
	score_item.height = 10;
	score_item.font_size = 1;
	score_item.type = BACK_GND_STYLE_NONE_OUTLINE;
	strcpy(reinterpret_cast<char*>(score_item.text), "SCORE: 0");

	/* Thiết lập item Game Over (ẩn) */
	game_over_item.x = 40;
	game_over_item.y = 30;
	game_over_item.width = 50;
	game_over_item.height = 10;
	game_over_item.font_size = 1;
	game_over_item.type = BACK_GND_STYLE_NONE_OUTLINE;
	strcpy(reinterpret_cast<char*>(game_over_item.text), "GAME OVER");

	/* Thiết lập dynamic items */
	dino_item.render = &render_player;
	/* dust_item.render = &render_dust; // Đã xóa */
	obstacle_item.render = &render_obstacles;
	cloud_item.render = &render_clouds;

	/* Thêm tất cả item vào màn hình (Sửa lại thứ tự) */
	dino_screen.item[0] = reinterpret_cast<view_item_t*>(&score_item);
	dino_screen.item[1] = reinterpret_cast<view_item_t*>(&dino_item);
	dino_screen.item[2] = reinterpret_cast<view_item_t*>(&obstacle_item); 
	dino_screen.item[3] = reinterpret_cast<view_item_t*>(&cloud_item); 	 
	dino_screen.item[4] = reinterpret_cast<view_item_t*>(&game_over_item); 
	dino_screen.item[5] = nullptr; 
	dino_screen.focus_item = 0;
}

/* Cập nhật logic game*/
extern "C" void dino_game_update() {
	if(dino_state_game == DINO_GAMESTATE_PLAYING) {
		dino_player_update();
		dino_obstacle_update();
		dino_cloud_update();

		/* Tăng tốc độ game */
		if (game_speed < GAME_SPEED_MAX) {
			game_speed += GAME_SPEED_INCREASE;
		}

		/* Kiểm tra va chạm */
		rect_t player_rect = { dino.pos.x, dino.pos.y, DINO_WIDTH, DINO_HEIGHT };
		if (dino_obstacle_check_collision(player_rect)) {
			dino_state_game = DINO_GAMESTATE_GAMEOVER;
			buzzer_playGameOver();
			dino_player_die(); 
		}

		/* Tăng điểm */
		dino_game_score++;

		/* Cập nhật text điểm */
		snprintf(reinterpret_cast<char*>(score_item.text), sizeof(score_item.text), "SCORE: %u", dino_game_score);
	}
}

/*Vẽ game ra LCD*/
extern "C" void dino_game_draw() {
	view_render.clear(); 

	/* Ẩn/Hiện item "GAME OVER" */
	if (dino_state_game == DINO_GAMESTATE_GAMEOVER) {
		game_over_item.type = BACK_GND_STYLE_NONE_OUTLINE; /* Hiện (Type 2) */
	} else {
		game_over_item.type = 3; /* Ẩn item (bằng cách dùng type không tồn tại) */
	}

	view_render_screen(&dino_screen); 
}

/*Xử lý Input*/
extern "C" void dino_game_handle_input(uint32_t sig) {
	if (dino_state_game == DINO_GAMESTATE_PLAYING) {
	}
}