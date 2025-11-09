#include "view_render.h"
#include <cstring> 	
#include <cstddef>

Adafruit_ssd1306syp view_render;

int view_render_rectangle(void* rectangle);
int view_render_dynamic(void* dynamic);

static uint8_t number_item;

static view_render_item render_list[] = {
	view_render_rectangle,
	view_render_dynamic
};

void view_render_init() {
	number_item = sizeof(render_list)/sizeof(view_render_item);
	view_render.initialize();
}

int view_render_rectangle(void* rectangle) {
	uint8_t x, y, len;
	uint8_t x_forcus, y_forcus, w_forcus, h_forcus;

	view_rectangle_t* rect = reinterpret_cast<view_rectangle_t*>(rectangle);
	if (rect == nullptr) { 
		return -1;
	}

	// Paint background
	switch (rect->type) {
		case BACK_GND_STYLE_FILL:
			view_render.fillRect(rect->x, rect->y, rect->width, rect->height, WHITE);
			view_render.setTextColor(BLACK);
			break;
		case BACK_GND_STYLE_OUTLINE:
			view_render.drawRect(rect->x, rect->y, rect->width, rect->height, WHITE);
			view_render.setTextColor(WHITE);
			break;
		case BACK_GND_STYLE_NONE_OUTLINE:
			view_render.setTextColor(WHITE);
			break;
		default:
			return 0; 
	}

	len = strlen(reinterpret_cast<const char*>(rect->text));
	x = rect->x + (rect->width - len * X_SIZE_FONT * rect->font_size)/2 - len;
	y = rect->y + (rect->height - rect->font_size * Y_SIZE_FONT)/2;

	view_render.setTextSize(rect->font_size);
	view_render.setCursor(x, y);
	view_render.print(rect->text);

	// Paint focus border
	if (rect->border_width != 0) {
		x_forcus = rect->focus_cursor * X_SIZE_FONT * rect->font_size + rect->focus_cursor*2 + x - 2;
		y_forcus = rect->y + 1;

		w_forcus = rect->focus_size * (X_SIZE_FONT * rect->font_size + 3);
		h_forcus = rect->height - 2;

		if (rect->type == BACK_GND_STYLE_FILL) {
			view_render.drawRect(x_forcus, y_forcus, w_forcus, h_forcus, BLACK);
		} else if (rect->type == BACK_GND_STYLE_OUTLINE) {
			view_render.drawRect(x_forcus, y_forcus, w_forcus, h_forcus, WHITE);
		}
	}

	return 0;
}

int view_render_dynamic(void* dynamic) {
	/* Dùng C++ cast */
	view_dynamic_t* dyn = reinterpret_cast<view_dynamic_t*>(dynamic);
	if (dyn->render) { 
		dyn->render();
	}
	return 0;
}

int view_render_screen(view_screen_t* screen) {
	view_render.clear(); 

	view_item_t* focused_item = reinterpret_cast<view_item_t*>(screen->item[screen->focus_item]);

	if (focused_item != nullptr) {
		if (focused_item->item_type == ITEM_TYPE_RECTANGLE) {
			(reinterpret_cast<view_rectangle_t*>(focused_item))->type = BACK_GND_STYLE_FILL;
		}
	}

	/* Vòng lặp 1: Cập nhật trạng thái (focus/non-focus) */
	for (uint8_t i = 0; i < NUMBER_SCREEN_ITEMS_MAX; i++) {
		view_item_t* current_item = reinterpret_cast<view_item_t*>(screen->item[i]);
		
		if (current_item == nullptr) continue; 

		if (current_item->item_type == ITEM_TYPE_RECTANGLE) {
			view_rectangle_t* rect_item = reinterpret_cast<view_rectangle_t*>(current_item);
			if (i != screen->focus_item && rect_item->type != BACK_GND_STYLE_NONE_OUTLINE) {
				rect_item->type = BACK_GND_STYLE_OUTLINE;
			}
		}
	}

	/* Vòng lặp 2: Vẽ tất cả các item */
	for (int i = 0; i < NUMBER_SCREEN_ITEMS_MAX; i++) {
		view_item_t* current_item = reinterpret_cast<view_item_t*>(screen->item[i]);
		
		if (current_item != nullptr) {
			if (current_item->item_type < number_item) {
				render_list[current_item->item_type](current_item);
			}
		}
	}

	/* Cập nhật màn hình (gửi buffer lên) */
	view_render.update();
	return 0;
}

void view_render_display_on() {
	view_render.display_on();
}

void view_render_display_off() {
	view_render.display_off();
}