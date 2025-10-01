#include "frontend.hpp"

extern "C" {
void print_rectangle(int top_y, int bottom_y, int left_x, int right_x);
}

void choose_game_print() {
  print_rectangle(5, 13, 10, 43);
  mvprintw(7, 19, "CHOOSE THE GAME!");
  mvprintw(9, 13, "Press <- to play the Tetris");
  mvprintw(10, 13, "Press -> to play the Snake");
  mvprintw(11, 13, "Press ESC to exit");
}

void print_rectangle(int top_y, int bottom_y, int left_x, int right_x) {
  mvaddch(top_y, left_x, ACS_ULCORNER);
  mvaddch(top_y, right_x, ACS_URCORNER);
  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  mvaddch(bottom_y, right_x, ACS_LRCORNER);
  for (int i = left_x + 1; i < right_x; i++) mvaddch(top_y, i, ACS_HLINE);
  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }
  for (int i = left_x + 1; i < right_x; i++) mvaddch(bottom_y, i, ACS_HLINE);
}