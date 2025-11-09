#include "task_list.h"

/* MAIN TASK TABLE */
const task_t app_task_table[] = {
    /* SYSTEM TASK */
    {TASK_TIMER_TICK_ID, TASK_PRI_LEVEL_7, task_timer_tick},
    {AC_TASK_SYSTEM_ID,  TASK_PRI_LEVEL_2, task_system},
    {AC_TASK_FW_ID,      TASK_PRI_LEVEL_2, task_fw},
    {AC_TASK_SHELL_ID,   TASK_PRI_LEVEL_2, task_shell},
    {AC_TASK_LIFE_ID,    TASK_PRI_LEVEL_6, task_life},
    {AC_TASK_IF_ID,      TASK_PRI_LEVEL_4, task_if},
    {AC_TASK_UART_IF_ID, TASK_PRI_LEVEL_4, task_uart_if},
    {AC_TASK_DBG_ID,     TASK_PRI_LEVEL_4, task_dbg},
    {AC_TASK_DISPLAY_ID, TASK_PRI_LEVEL_4, task_display},

    /* DINO GAME TASK */
    {AC_TASK_SCR_MENU_ID,     TASK_PRI_LEVEL_4, task_scr_menu_handle},
    {AC_TASK_SCR_GAME_ON_ID,  TASK_PRI_LEVEL_4, task_scr_game_on_handle},
    {AC_TASK_SCR_SETTING_ID,  TASK_PRI_LEVEL_4, task_scr_setting_handle},

    /* END OF TABLE */
    {AK_TASK_EOT_ID, TASK_PRI_LEVEL_0, nullptr}
};


/* POLLING TASK TABLE (optional) */
const task_polling_t app_task_polling_table[] = {
    {AK_TASK_POLLING_EOT_ID, AK_DISABLE, nullptr},
};
