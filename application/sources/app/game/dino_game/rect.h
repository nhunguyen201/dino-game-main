#ifndef RECT_H
#define RECT_H

#include <stdint.h>
#include <stdbool.h>

// Cấu trúc mô tả hình chữ nhật
typedef struct {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
} rect_t;

// Kiểm tra xem hai hình chữ nhật có giao nhau không
bool rect_intersect(const rect_t* a, const rect_t* b);

#endif // RECT_H
