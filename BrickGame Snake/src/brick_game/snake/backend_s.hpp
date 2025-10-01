#ifndef BACKEND_HPP
#define BACKEND_HPP

#include <stdbool.h>
#include <sys/time.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "../../controller.h"

// #define GAME_FIELD_N (FIELD_N + 2) /*!< Высота игрового поля*/
// #define GAME_FIELD_M (FIELD_M * 2 + 2) /*!< Ширина игрового поля*/
// #define START_MENU_N 5   /*!< Высота стартового окна*/
// #define START_MENU_M 22  /*!< Ширина стартового окна*/
// #define PAUSE_MENU_N 5   /*!< Высота окна паузы*/
// #define PAUSE_MENU_M 22  /*!< Ширина окна паузы*/
// #define FINISH_GAME_N 6  /*!< Высота окна финиша*/
// #define FINISH_GAME_M 22 /*!< Ширина окна финиша*/
// #define GAME_INFO_M 25 /*!< Ширина поля с игровой информацией*/
// #define CONTROLS_M 22 /*!< Ширина поля со справочной информацией*/

namespace s21 {

/** @file backend_s.hpp*/

/**
 * @brief Содержит координаты точки поля
 *
 * @struct position
 */

typedef struct {
  int x;
  int y;
} position;

/**
 * @enum direction_type
 * @brief Перечисление направлений движения змейки.
 *
 */

typedef enum {
  Left_dir,   ///< движение влево
  Up_dir,     ///< движение вверх
  Right_dir,  ///< движение вправо
  Down_dir    ///< движение вниз
} direction_type;

const int ROWS = 20; /*!< Высота игрового поля*/
const int COLS = 10; /*!< Ширина игрового поля*/
const int levels[10] = {
    1000, 900, 800, 700, 600, 500,
    400,  300, 200, 100}; /*!< Отвечает за увеличние скорости в зависимости от
                             уровня*/

/**
 * @brief класс, содержащий основные данные игровой сессии
 *
 */

class game {
 public:
  game();   // Конструктор
  ~game();  // Деструктор

  bool lost; /*!<  Выйграл/проиграл пользователь */

  GameInfo_t updateCurrentState();
  void save_settings();
  int **create_matrix(int height, int width);
  void delete_matrix(int **matrix, int H);

  bool is_playing;     /*!< Игра продолжается Игра окончена*/
  int score;           /*!< /Счет*/
  int score_for_speed; /*!< Преобразованные счет для подсчета скорости */
  int **field; /*!< Двумерная целочисленная матрица, содержащая состояние всего
                * игрового поля */
  int high_score; /*!< Содержит максимальное количество очков, заработанное в
                   * этой игре, считывается из файла high_score.txt */
  int level;      /*!< Содержит текущий уровень, он увеличивается с увеличением
                   * очков, и влияет на скорость игры */
  int speed;     /*!< Содержит целочисленное число, описывающее скорость падения
      фигуры */
  int direction; /*!< Принимает одно из значений direction, и влияет направление
                    движения змейки */
  bool pause; /*!< Булевая переменная, true - если игра на паузе, false -  если
  игра в активной фазе */
  UserAction_t action; /*!< Переменная типа UserAction_t. Принимает одно из
                          значений UserAction_t*/
  position apple;      /*!< Содержит координаты яблока */
  position last;       /*!< Содержит координаты последней ячейки змейки */
  std::vector<position> snake; /*!< Вектор, содержащий координаты змейки */

  WINDOW *print_controls();
  void print_rules();
  void print_field();
  void print_game();
  void print_snake();
  void print_apple();
  void print_score();
  void print_high_score();
  void print_level();
  void print_start();
  void print_resume();
  void print_lost();

  void first_step();
  void userInput(UserAction_t action, bool hold);
  void check_in_game();
  void move();
  void create_apple();
  void free_game_info_s(GameInfo_t *gi);
  void free_matrix_s(int N, int **matrix);

  void game_loop(s21::game *current_game);
  void wait_for_start(s21::game *current_game);
  void wait_for_resume(s21::game *current_game);
  void wait_for_exit(s21::game *current_game);
  void wait_for_esc_win(s21::game *current_game);
};

}  // namespace s21

#endif  // BACKEND_HPP