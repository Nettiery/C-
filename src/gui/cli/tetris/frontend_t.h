#ifndef FRONTEND_T_H

#define GAME_FIELD_N (FIELD_N + 2)     /*!< Высота игрового поля*/
#define GAME_FIELD_M (FIELD_M * 2 + 2) /*!< Ширина игрового поля*/
#define START_MENU_N 5                 /*!< Высота стартового окна*/
#define START_MENU_M 22                /*!< Ширина стартового окна*/
#define PAUSE_MENU_N 5                 /*!< Высота окна паузы*/
#define PAUSE_MENU_M 22                /*!< Ширина окна паузы*/
#define FINISH_GAME_N 6                /*!< Высота окна финиша*/
#define FINISH_GAME_M 22               /*!< Ширина окна финиша*/
#define GAME_INFO_M 25                 /*!< Ширина поля с игровой информацией*/
#define CONTROLS_M 22 /*!< Ширина поля со справочной информацией*/

#define TOP_MARGIN 0  /*!< Верхняя граница поля*/
#define LEFT_MARGIN 0 /*!< Левая граница поля*/

#include <ncurses.h>

#include "../frontend.hpp"

/** @file */

#ifdef __cplusplus
extern "C" {
#endif

void init_ncurses();
void init_colors();
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
void render(GameInfo_t gi);
WINDOW *print_controls();
WINDOW *print_game_field(GameInfo_t gi);
void print_start_menu();
void print_pause_menu();
void print_win(GameInfo_t gi);
void print_game_over(GameInfo_t gi);
WINDOW *print_game_info(GameInfo_t gi);
void print_next_figure(WINDOW *info_window, GameInfo_t gi);

#ifdef __cplusplus
}
#endif

#endif