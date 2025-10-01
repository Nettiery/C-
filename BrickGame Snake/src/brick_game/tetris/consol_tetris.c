#include "consol_tetris.h"

#include <stdio.h>
#include <stdlib.h>

/** @file */

/**
 * @brief Основная функция игры tetris
 *
 * Функция инициализирует игру и запускает основной цикл игры.
 *
 */

void tetris() {
  bool is_playing = true;
  init_game();
  while (is_playing) {
    GameState_t *gs = create_game_state();
    GameInfo_t gi = updateCurrentState_(gs);
    int ch = getch();
    render(gi);
    free_game_info(&gi);
    if (gs->status == GameOver || gs->win) {
      while (ch != 'r' && ch != 'q') {
        ch = getch();
        print_game_over(updateCurrentState_(gs));
      }
      init_game();
    }
    timeout(10);
    userInput_(get_user_action(ch), gs);
    is_playing = gs->is_playing;
  }
}
