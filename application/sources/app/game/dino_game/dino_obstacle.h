#ifndef DINO_OBSTACLE_H
#define DINO_OBSTACLE_H

#include <stdint.h>
#include "rect.h" 

#ifdef __cplusplus
extern "C"
{
#endif

void dino_obstacle_init(void);
void dino_obstacle_update(void);
void dino_obstacle_draw(void);
bool dino_obstacle_check_collision(rect_t dino_rect);

#ifdef __cplusplus
}
#endif

#endif // DINO_OBSTACLE_H