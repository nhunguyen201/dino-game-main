#ifndef DINO_CLOUD_H
#define DINO_CLOUD_H

#include <stdint.h>
#include "dino_config.h"

// Số lượng mây
#define DINO_MAX_CLOUDS 3

typedef struct {
	int16_t x;
	int16_t y;
} dino_cloud_t;

#ifdef __cplusplus
extern "C"
{
#endif
extern dino_cloud_t dino_clouds[DINO_MAX_CLOUDS];

void dino_cloud_init(void);
void dino_cloud_update(void);
void dino_cloud_draw(void);

#ifdef __cplusplus
}
#endif

#endif // DINO_CLOUD_H