#include <iostream>

#include "brick_game/backend.hpp"
#include "brick_game/snake/backend_s.hpp"
#include "brick_game/snake/consol_snake.h"
#include "brick_game/tetris/backend_t.h"
#include "brick_game/tetris/consol_tetris.h"
#include "gui/cli/tetris/frontend_t.h"

/** @file */
/**
 * @brief Функция инициализирует библиотеку ncurses, цветовые схемы и запускает
 * инициализауию игрового поля. После осуществляется запуск игрового цикла.
 * @return статус выполнения функции
 */

int main() {
  init_ncurses();
  init_colors();
  int key = (choose_the_game());
  if (key == KEY_LEFT) {
    tetris();
  } else if (key == KEY_RIGHT) {
    s21::snake_g();
  }
  endwin();

  return 0;
}
