#ifndef __VIEW_RENDER_H__
#define __VIEW_RENDER_H__

#include "view_item.h"           /
#include "Adafruit_ssd1306syp.h"

#define X_SIZE_FONT 5
#define Y_SIZE_FONT 7

#define BACK_GND_STYLE_FILL       0
#define BACK_GND_STYLE_OUTLINE    1
#define BACK_GND_STYLE_NONE_OUTLINE 2

typedef int (*view_render_item)(void*);

extern Adafruit_ssd1306syp view_render;

void view_render_init();
int view_render_screen(view_screen_t* screen);
void view_render_display_on();
void view_render_display_off();

int view_render_rectangle(void* rectangle);
int view_render_dynamic(void* dynamic);

#endif // __VIEW_RENDER_H__
