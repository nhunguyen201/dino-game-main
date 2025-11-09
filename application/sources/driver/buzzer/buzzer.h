#ifndef __BUZZER_H__
#define __BUZZER_H__

/* * Khai báo extern "C" này chỉ dành cho trình biên dịch C++.
 * Trình biên dịch C sẽ bỏ qua nó.
 */
#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "io_cfg.h"

/* * SỬA LẠI: Dùng chuẩn C99 <stdbool.h>
 * (Vì hệ thống của bạn đã include nó ở file khác)
 */
#include <stdbool.h>


/* * Định nghĩa cấu trúc giai điệu
 * Duration: Đơn vị là 10ms (ví dụ: 5 = 50ms)
 */
typedef struct {
	uint16_t frequency;
	uint8_t  duration;
} Tone_TypeDef;

/* --- Khai báo các giai điệu (Định nghĩa nằm trong .c) --- */
extern const Tone_TypeDef tones_startup[];
extern const Tone_TypeDef tones_3beep[];
extern const Tone_TypeDef tones_SMB[];
extern const Tone_TypeDef tones_merryChrismast[];

/* --- Khai báo các giai điệu game (Đã hợp nhất từ buzzer_def) --- */
extern const Tone_TypeDef tones_jump[];
extern const Tone_TypeDef tones_game_over[];
extern const Tone_TypeDef tones_score_up[];
extern const Tone_TypeDef tones_menu_move[];
extern const Tone_TypeDef tones_menu_select[];
extern const Tone_TypeDef tones_toggle_sound[];
extern const Tone_TypeDef tones_start_game[];


/* --- Function prototypes (Driver chính) --- */
void BUZZER_Init(void);
void BUZZER_Enable(uint16_t freq, uint32_t duration);
void BUZZER_Disable(void);
void BUZZER_PlayTones(const Tone_TypeDef * melody);

/* --- Function prototypes (Helper) --- */
void buzzer_playJump(void);
void buzzer_playGameOver(void);
void buzzer_playScoreUp(void);
void buzzer_playMenuMove(void);
void buzzer_playMenuSelect(void);
void buzzer_playToggleSound(void);
void buzzer_playStartGame(void);

extern void buzzer_irq( void );

#ifdef __cplusplus
}
#endif

#endif /* __BUZZER_H__ */