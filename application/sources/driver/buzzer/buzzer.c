#include <stm32l1xx_rcc.h>
#include <stm32l1xx_gpio.h>
#include <misc.h>
#include <stddef.h> 
#include "buzzer.h" 


volatile uint32_t 		 _beep_duration;
volatile bool 			 _tones_playing; 
volatile const Tone_TypeDef *_tones;

GPIO_InitTypeDef GPIO_InitStructure;

void buzzer_irq( void ) {
	if (BUZZER_TIM->SR & TIM_SR_UIF) {
		BUZZER_TIM->SR &= ~TIM_SR_UIF;

		_beep_duration--;
		if (_beep_duration == 0) {
			if (_tones_playing) {
				_tones++;
				if (_tones->frequency == 0 && _tones->duration == 0) {
					BUZZER_Disable();
					_tones_playing = false;
					_tones = NULL;
				} else {
					if (_tones->frequency == 0) {
						BUZZER_TIM->ARR = SystemCoreClock / (100 * BUZZER_TIM->PSC) - 1;
						BUZZER_TIM->CCR3 = 0; 
						_beep_duration = _tones->duration + 1;
					} else {
						BUZZER_Enable(_tones->frequency, _tones->duration);
					}
				}
			} else {
				BUZZER_Disable();
			}
		}
	}
}


void BUZZER_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(BUZZER_TIM_PERIPH, ENABLE);

	/* Cấu hình Chân GPIO (Pin) */
	GPIO_InitStructure.GPIO_Pin = BUZZER_IO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(BUZZER_IO_PORT, &GPIO_InitStructure);
	GPIO_PinAFConfig(BUZZER_IO_PORT, BUZZER_IO_SOURCE, BUZZER_IO_AF);
	GPIO_ResetBits(BUZZER_IO_PORT, BUZZER_IO_PIN);

	/* Cấu hình Timer Base */
	TIM_DeInit(BUZZER_TIM);
	TIM_TimeBaseStructure.TIM_Prescaler = SystemCoreClock / 4000000;
	TIM_TimeBaseStructure.TIM_Period = 999;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(BUZZER_TIM, &TIM_TimeBaseStructure);

	/* Cấu hình PWM (Output Compare) */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_Pulse = 499; 0
	TIM_OC3Init(BUZZER_TIM, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(BUZZER_TIM, TIM_OCPreload_Enable);

	NVIC_InitStruct.NVIC_IRQChannel = BUZZER_TIM_IRQ;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);

	TIM_ARRPreloadConfig(BUZZER_TIM, ENABLE);
	TIM_ITConfig(BUZZER_TIM, TIM_IT_Update, ENABLE);
	TIM_Cmd(BUZZER_TIM, ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BUZZER_IO_PORT, &GPIO_InitStructure);
}

void BUZZER_Enable(uint16_t freq, uint32_t duration) {
	if (freq < 100 || freq > 8000 || duration == 0) {
		BUZZER_Disable();
	} else {
		_beep_duration = (freq / 100) * duration + 1;

		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(BUZZER_IO_PORT, &GPIO_InitStructure);

		RCC->APB1ENR |= BUZZER_TIM_PERIPH; 
		BUZZER_TIM->ARR = SystemCoreClock / (freq * BUZZER_TIM->PSC) - 1;
		BUZZER_TIM->CCR3 = BUZZER_TIM->ARR >> 1; 
		BUZZER_TIM->CR1 |= TIM_CR1_CEN; 
	}
}

void BUZZER_Disable(void) {
	BUZZER_TIM->CR1 &= ~TIM_CR1_CEN;
	RCC->APB1ENR &= ~BUZZER_TIM_PERIPH;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BUZZER_IO_PORT, &GPIO_InitStructure);
}

void BUZZER_PlayTones(const Tone_TypeDef * tones) {
	if (tones == NULL) return;
	_tones = tones;
	_tones_playing = true;
	BUZZER_Enable(_tones->frequency, _tones->duration);
}

const Tone_TypeDef tones_startup[] = {
	{2000,3}, { 0,3}, {3000,3}, { 0,3}, {4000,3}, { 0,3}, {1200,4}, { 0,6}, {4500,6}, { 0,0},
	{2000,3}, { 0,3}, {3000,3}, { 0,3}, {4000,3}, { 0,3}, {1200,4}, { 0,6}, {4500,6}, { 0,0},
	{2000,3}, { 0,3}, {3000,3}, { 0,3}, {4000,3}, { 0,3}, {1200,4}, { 0,6}, {4500,6}, { 0,0}
};

const Tone_TypeDef tones_3beep[] = {
	{4000, 3}, { 0,10}, {1000, 6}, { 0,10}, {4000, 3}, { 0, 0}
};

const Tone_TypeDef tones_SMB[] = {
	{2637,18}, { 0, 9}, {2637, 9}, { 0, 9}, {2093, 9}, {2637, 9}, { 0, 9}, {3136, 9}, { 0,27}, {1586, 9}, { 0,27},
	{2093, 9}, { 0,18}, {1586, 9}, { 0,18}, {1319, 9}, { 0,18}, {1760, 9}, { 0, 9}, {1976, 9}, { 0, 9}, {1865, 9}, {1760, 9}, { 0, 9},
	{1586,12}, {2637,12}, {3136,12}, {3520, 9}, { 0, 9}, {2794, 9}, {3136, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2093, 9}, {2349, 9}, {1976, 9}, { 0,18},
	{2093, 9}, { 0,18}, {1586, 9}, { 0,18}, {1319, 9}, { 0,18}, {1760, 9}, { 0, 9}, {1976, 9}, { 0, 9}, {1865, 9}, {1760, 9}, { 0, 9},
	{1586,12}, {2637,12}, {3136,12}, {3520, 9}, { 0, 9}, {2794, 9}, {3136, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2093, 9}, {2349, 9}, {1976, 9},
	{ 0, 0}
};

const Tone_TypeDef tones_merryChrismast[] = {
	{2637, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2637, 9}, { 0,18},
	{2637, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2637, 9}, { 0,18},
	{2637, 9}, { 0, 9}, {3136, 9}, { 0, 9}, {2093, 9}, { 0, 9}, {2349, 9}, { 0, 9}, {2637, 9}, { 0,24},
	{2794, 9}, { 0, 9}, {2794, 9}, { 0, 9}, {2794, 9}, { 0, 9}, {2794, 9}, { 0, 9}, {2794, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2349, 9}, { 0, 9}, {2349, 9}, { 0, 9}, {2637, 9}, { 0, 9}, {2349, 9}, { 0, 9}, {3136, 9},
	{ 0, 0}
};

const Tone_TypeDef tones_jump[] = {
	{1000, 5}, {0, 2}, {0, 0}
};

const Tone_TypeDef tones_game_over[] = {
	{400, 20}, {300, 20}, {200, 20}, {0, 5}, {0, 0}
};

const Tone_TypeDef tones_score_up[] = {
	{1500, 5}, {1600, 5}, {1700, 5}, {0, 5}, {0, 0}
};

const Tone_TypeDef tones_menu_move[] = {
	{1200, 5}, {0, 2}, {0, 0}
};

const Tone_TypeDef tones_menu_select[] = {
	{1800, 5}, {0, 2}, {0, 0}
};

const Tone_TypeDef tones_toggle_sound[] = {
	{1400, 5}, {0, 2}, {0, 0}
};

const Tone_TypeDef tones_start_game[] = {
	{2000, 10}, {0, 5}, {2500, 10}, {0, 5}, {0, 0}
};

void buzzer_playJump(void) {
	BUZZER_PlayTones(tones_jump);
}

void buzzer_playGameOver(void) {
	BUZZER_PlayTones(tones_game_over);
}

void buzzer_playScoreUp(void) {
	BUZZER_PlayTones(tones_score_up);
}

void buzzer_playMenuMove(void) {
	BUZZER_PlayTones(tones_menu_move);
}

void buzzer_playMenuSelect(void) {
	BUZZER_PlayTones(tones_menu_select);
}

void buzzer_playToggleSound(void) {
	BUZZER_PlayTones(tones_toggle_sound);
}

void buzzer_playStartGame(void) {
	BUZZER_PlayTones(tones_start_game);
}