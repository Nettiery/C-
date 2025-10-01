#include "consol_snake.h"

#include "backend_s.hpp"

/** @file */

using namespace s21;
namespace s21 {

/**
 * @brief Основная функция игры Snake
 *
 * Функция инициализирует игру и запускает основной цикл игры.
 *
 */

void snake_g() {
  s21::game current_game;
  current_game.wait_for_start(&current_game);
  current_game.game_loop(&current_game);
}

/**
 * @brief Содержит основной цикл игры
 *
 * Функция содержит основной цикл: каждый шаг игры считывает ввод пользователя,
 * обрабатывает его и двигает змейку.
 *
 */

void game::game_loop(s21::game *current_game) {
  using namespace s21;
  GameInfo_t gi = current_game->updateCurrentState();
  render(gi);
  current_game->free_game_info_s(&gi);
  while (current_game->is_playing) {
    current_game->action = get_user_action(getch());
    if (current_game->action == Pause) {
      current_game->wait_for_resume(current_game);
    } else {
      current_game->userInput(current_game->action, false);
      current_game->check_in_game();
      flushinp();
      if (current_game->is_playing) {
        GameInfo_t gi = current_game->updateCurrentState();
        render(gi);
        current_game->free_game_info_s(&gi);
        if (current_game->action != Action) napms(current_game->speed);
      }
    }
  }
  timeout(10);
  if (current_game->is_playing == false) {
    current_game->wait_for_exit(current_game);
  } else
    current_game->wait_for_esc_win(current_game);
  current_game->save_settings();
}

/**
 * @brief Функция печатает окно, при паузе в игре
 *
 * Функция печатает окно, при паузе в игре. Печать происходит до нажатия кнопки
 * "R".
 *
 */

void game::wait_for_resume(s21::game *current_game) {
  current_game->pause = true;
  int a = -1;
  while (a != KEY_R) {
    print_pause_menu();
    a = getch();
  }
  current_game->pause = false;
}

/**
 * @brief Функция печатает окно, при выходе из игры
 *
 * Функция печатает окно, при выходе из игры. Печать происходит до нажатия
 * кнопки "Q".
 *
 */

void game::wait_for_exit(s21::game *current_game) {
  current_game->pause = true;
  int a = -1;
  while (a != KEY_Q) {
    print_game_over(current_game->updateCurrentState());
    a = getch();
  }
}

/**
 * @brief Функция печатает окно, при начале игры.
 *
 * Функция печатает окно, при начале игры. Печать происходит до нажатия кнопки
 * "R".
 *
 */

void game::wait_for_start(s21::game *current_game) {
  int a = -1;
  while (a != KEY_R) {
    print_start_menu();
    a = getch();
  }
  current_game->pause = false;
}

/**
 * @brief Функция печатает окно при выйгрыше.
 *
 * Функция печатает окно при выйгрыше. Печать происходит до нажатия кнопки "Q".
 *
 */

void game::wait_for_esc_win(s21::game *current_game) {
  current_game->pause = true;
  int a = -1;
  while (a != KEY_Q) {
    print_win(current_game->updateCurrentState());
    a = getch();
  }
}
}  // namespace s21