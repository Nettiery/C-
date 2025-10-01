#ifndef BACKEND_T_H
#define BACKEND_T_H

#define START_X 4            /*!< Начальная координата фигуры по горизонтали*/
#define START_Y 0            /*!< Начальная координата фигуры по вертикали*/
#define NUM_FIGURES 7        /*!< Количество фигур*/
#define MAX_LEVEL 10         /*!< Максимальный уровень*/
#define SCORE_PER_LEVEL 1000 /*!< Количество очков на 1 уровне*/
#define DELAY_MS \
  1100 /*!< Задержка, используется для вычисления скорости падения фигуры*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../controller.h"
#include "../../gui/cli/tetris/frontend_t.h"
#include "consol_tetris.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @file */

/**
 * @enum GameStatus_t
 * @brief Перечисление возможных статусов игры.
 *
 * Перечень статусов игры.
 */

typedef enum {
  Initial,    ///< Инициализация игры
  Spawn,      ///< Генерирование следующей фигуры
  Moving,     ///< Движение влево, вправо, вниз
  Shifting,   ///< Смещение фигуры
  Attaching,  ///< Прилипание фигуры
  GameOver    ///< Конец игры
} GameStatus_t;

/**
 * @brief Структура содержит параметры игрового поля, необходимые для
 * функционирования игры на каждом шаге
 *
 * @struct GameState_t
 */

typedef struct {
  int **field;  /*!< Двумерная целочисленная матрица, содержащая состояние всего
                 * игрового поля */
  int **figure; /*!< Двумерная целочисленная матрица, содержащая фигуру,
                 * находящуюся на поле */
  int figure_size; /*!< Целочисленный размер стороны матрицы, содержащей фигуру,
                    * находящуюся на поле. */
  int **next_figure;    /*!< Двумерная целочисленная матрица, содержащая фигуру,
                         * которая появится следующей на поле */
  int next_figure_size; /*!< Целочисленный размер стороны матрицы, содержащей
                         * фигуру, которая появится следующей на поле */
  int x; /*!< Целочисленная координата по горизонтали, указывающая
          * местоположение фигуры на поле.*/
  int y; /*!< Целочисленная координата по вертикали, указывающая местоположение
          * фигуры на поле */
  int status; /*!< Принимает одно из значений GameStatus_t, и влияет на то что
               * будет происходить в следующий шаг игры */
  int score;  /*!< Содержит текущее количество очков, заработанное игроком */
  int high_score; /*!< Содержит максимальное количество очков, заработанное в
                   * этой игре, считывается из файла high_score.txt */
  int level;      /*!< Содержит текущий уровень, он увеличивается с увеличением
                   * очков, и влияет на скорость игры */
  int speed;  /*!< Содержит целочисленное число, описывающее скорость падения
                 фигуры */
  bool pause; /*!< Булевая переменная, true - если игра на паузе, false -  если
                 игра в активной фазе */
  bool is_playing; /*!< Булевая переменная, true - если игра в активной фазе,
                      false -  если игра закончена (выигрыш или проигрыш)  */
  bool win; /*!< Булевая переменная, true - если игра в выйграна (достигнут
               максимальный уровень), false - во всех других случаях */
  long long time; /*!< Участвует при расчете времени задержки*/
} GameState_t;

void init_game();
int **generate_figure(int *size);
void fill_figure(int number, int **figure);
GameInfo_t updateCurrentState_(GameState_t *gs);
void update_field(GameState_t *gs, int **field);
GameState_t *get_current_state();
void userInput_(UserAction_t action, GameState_t *gs);
void finish_game(GameState_t *gs);
void spawn_figure(GameState_t *gs);
void move_figure(GameState_t *gs, UserAction_t action);
void move_left(GameState_t *gs);
void move_right(GameState_t *gs);
void move_down(GameState_t *gs);
void rotate(GameState_t *gs);
void attach_figure(GameState_t *gs);
bool check_top_line(GameState_t *gs);
void check_full_lines(GameState_t *gs);
void shift_lines(GameState_t *gs, int i);
void fill_top_line(GameState_t *gs);
void update_score_and_level(GameState_t *gs, int num_full_lines);

// Utils
GameState_t *create_game_state();
int **create_matrix(int N, int M);
void copy_matrix(int N, int M, int **src_matrix, int **dest_matrix);
void free_matrix(int N, int **matrix);
bool figure_is_attaching(GameState_t *gs);
long long get_time();
bool timer(GameState_t *gs, int delay);
void free_game_info(GameInfo_t *gi);

#ifdef __cplusplus
}
#endif

#endif
