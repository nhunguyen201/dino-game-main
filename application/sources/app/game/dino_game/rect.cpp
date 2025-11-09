#include "rect.h"

// Hàm kiểm tra va chạm giữa hai hình chữ nhật
bool rect_intersect(const rect_t* a, const rect_t* b) {
    return (a->x < b->x + b->w) &&
           (a->x + a->w > b->x) &&
           (a->y < b->y + b->h) &&
           (a->y + a->h > b->y);
}
