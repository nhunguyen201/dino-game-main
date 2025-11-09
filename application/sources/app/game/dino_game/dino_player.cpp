#include "dino_player.h"
#include "dino_bitmap.h" 
#include "view_render.h" 
#include "buzzer.h" 
#include "scr_startup.h" 
#include <cstddef> 

dino_player_t dino;
static uint8_t anim_counter = 0;


/*KHỞI TẠO PLAYER*/
extern "C" void dino_player_init(void) {
	dino.pos.x = 10;
	dino.pos.y = DINO_GROUND_Y;
	dino.velocityY = 0;
	dino.state = DINO_STATE_RUNNING;
	dino.is_alive = true;
	dino.anim_frame = 0;
	anim_counter = 0;
}

/*HÀM NHẢY*/
extern "C" void dino_jump(void) {
	if (dino.state == DINO_STATE_RUNNING) {
		dino.velocityY = -DINO_JUMP_HEIGHT;
		dino.state = DINO_STATE_JUMPING;
		buzzer_playJump(); 
	}
}

/*HÀM DEAD*/
extern "C" void dino_player_die(void) {
	dino.is_alive = false;
	dino.state = DINO_STATE_DEAD;
}

/*CẬP NHẬT PLAYER*/
extern "C" void dino_player_update(void) {
	if (!dino.is_alive) return;

	/* Logic Nhảy/Rơi */
	if (dino.state == DINO_STATE_JUMPING || dino.state == DINO_STATE_FALLING) {
		dino.velocityY += DINO_GRAVITY;
		dino.pos.y += dino.velocityY;

		if (dino.pos.y >= DINO_GROUND_Y) {
			dino.pos.y = DINO_GROUND_Y;
			dino.velocityY = 0;
			dino.state = DINO_STATE_RUNNING;
		} else if (dino.velocityY > 0) {
			dino.state = DINO_STATE_FALLING;
		}
	}
	/* Logic Chạy (Animation) */
	else if (dino.state == DINO_STATE_RUNNING) {
		anim_counter++;
		if (anim_counter > 5) { /
			anim_counter = 0;
			dino.anim_frame = !dino.anim_frame; 
		}
	}
}

/*VẼ PLAYER*/
extern "C" void dino_player_draw(void) {
	const unsigned char* bitmap_to_draw = dino_standing_bitmap;

	if (!dino.is_alive) {
		bitmap_to_draw = dino_dead_bitmap;
	}
	else if (dino.state == DINO_STATE_JUMPING || dino.state == DINO_STATE_FALLING) {
		bitmap_to_draw = dino_standing_bitmap;
	}
	else if (dino.state == DINO_STATE_RUNNING) {
		/* Luân phiên 2 ảnh chạy */
		if (dino.anim_frame == 0) {
			bitmap_to_draw = dino_run1_bitmap;
		} else {
			bitmap_to_draw = dino_run2_bitmap;
		}
	}

	/* Gọi hàm vẽ */
	view_render.drawBitmap(
		dino.pos.x,
		dino.pos.y,
		bitmap_to_draw,
		DINO_WIDTH,
		DINO_HEIGHT,
		WHITE
	);
}