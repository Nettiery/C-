#include "frontend_t.h"
/** @file */

/**
 * @brief Инициализация NCURSES.
 *
 * @return статус выполнения функции.
 */

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, 1);
  nodelay(stdscr, 1);
  scrollok(stdscr, 1);
  curs_set(0);
  mouseinterval(1);
}

/**
 * @brief Инициализация цветовых схем.
 *
 * @return статус выполнения функции.
 */

void init_colors() {
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  init_pair(3, COLOR_BLUE, COLOR_BLUE);
  init_pair(4, COLOR_RED, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_YELLOW);
  init_pair(6, COLOR_GREEN, COLOR_BLACK);
}

/**
 * @brief Функция отрисовывает игровое поле, справочную информацию и поле
 * игровой информации.
 *
 * @param status Имеет одно из значений типа GameStatus_t.
 * @param win булевая переменная. Максимальный уровень достигнут - true, не
 * достигнут false.
 * @param gi Структура содержит параметры игрового поля, необходимые для
 * графического отображения игры на каждом шаге.
 */

void render(GameInfo_t gi) {
  refresh();

  WINDOW *controls_window = print_controls();
  refresh();
  wrefresh(controls_window);

  WINDOW *game_window = NULL;

  game_window = print_game_field(gi);
  wrefresh(game_window);

  WINDOW *info_window = print_game_info(gi);
  wrefresh(info_window);

  if (game_window != NULL) delwin(game_window);

  delwin(info_window);
  delwin(controls_window);
}

/**
 * @brief Функция печатает окно со стартовой информацией.
 *
 * Появляется в начале игры. Изчезает при нажатии кнопки R.
 */

void print_start_menu() {
  WINDOW *start_menu_window =
      newwin(START_MENU_N, START_MENU_M, (GAME_FIELD_N - START_MENU_N) / 2,
             GAME_FIELD_M - (GAME_FIELD_M - START_MENU_M) / 2 + 1);
  box(start_menu_window, 0, 0);

  int text_width = 20;
  int text_height = 5;

  mvwprintw(start_menu_window, (START_MENU_N - text_height) / 2 + 2,
            (START_MENU_M - text_width) / 2 + 1, "Press 'r' to start");
  wattroff(start_menu_window, COLOR_PAIR(2));

  wrefresh(start_menu_window);
  delwin(start_menu_window);
}

/**
 * @brief Функция печатает окно, отображаемое во время паузы.
 *
 * Появляется при нажатии кнопки P. Изчезает при повторном нажатии.
 */

void print_pause_menu() {
  WINDOW *pause_menu_window =
      newwin(PAUSE_MENU_N, PAUSE_MENU_M, (GAME_FIELD_N - PAUSE_MENU_N) / 2,
             CONTROLS_M + (GAME_FIELD_M - PAUSE_MENU_M) / 2 + 1);
  box(pause_menu_window, 0, 0);

  int text_width = 6;
  int text_height = 1;

  mvwprintw(pause_menu_window, (PAUSE_MENU_N - text_height) / 2,
            (PAUSE_MENU_M - text_width) / 2, "PAUSED");

  wrefresh(pause_menu_window);
  delwin(pause_menu_window);
}

/**
 * @brief Функция печатает окно, отображаемое при выйгрыше.
 *
 * Функция печатает окно, отображаемое при достижении максимального уровня.
 */

void print_win(GameInfo_t gi) {
  WINDOW *win_window =
      newwin(FINISH_GAME_N, FINISH_GAME_M, (GAME_FIELD_N - FINISH_GAME_N) / 2,
             CONTROLS_M + (GAME_FIELD_M - FINISH_GAME_M) / 2 + 1);
  box(win_window, 0, 0);
  wbkgd(win_window, COLOR_PAIR(6));

  int text_width = 11;
  int text_height = 2;

  mvwprintw(win_window, (FINISH_GAME_N - text_height) / 2 + 1,
            (FINISH_GAME_M - text_width) / 2, "YOU WIN! ");
  mvwprintw(win_window, (FINISH_GAME_N - text_height) / 2 + 3,
            (FINISH_GAME_M - text_width) / 2, "Score  %d", gi.score);

  wrefresh(win_window);
  delwin(win_window);
}

/**
 * @brief Функция печатает окно, отображаемое при проигрыше.
 *
 * Функция печатает окно, отображаемое при заполнении верхней строки игрового
 * поля.
 */

void print_game_over(GameInfo_t gi) {
  WINDOW *game_over_window =
      newwin(FINISH_GAME_N, FINISH_GAME_M, (GAME_FIELD_N - FINISH_GAME_N) / 2,
             CONTROLS_M + (GAME_FIELD_M - FINISH_GAME_M) / 2 + 1);
  box(game_over_window, 0, 0);
  wbkgd(game_over_window, COLOR_PAIR(4));

  int text_width = 12;
  int text_height = 4;

  mvwprintw(game_over_window, (FINISH_GAME_N - text_height) / 2,
            (FINISH_GAME_M - text_width) / 2, "GAME OVER");
  mvwprintw(game_over_window, (FINISH_GAME_N - text_height) / 2 + 2,
            (FINISH_GAME_M - text_width) / 2, "Score  %d", gi.score);
  mvwprintw(game_over_window, (FINISH_GAME_N - text_height) / 2 + 3,
            (FINISH_GAME_M - text_width) / 2, "Level  %d", gi.level);

  wrefresh(game_over_window);
  delwin(game_over_window);
}

/**
 * @brief Функция печатает окно, содержащее справочную информацию.
 *
 * Функция печатает окно, содержащее справочную информация о кпонках контроля.
 * @return Указатель на структуру с данными об окне со справочной информацией.
 */

WINDOW *print_controls() {
  WINDOW *controls_window =
      newwin(GAME_FIELD_N, CONTROLS_M, TOP_MARGIN, LEFT_MARGIN);
  box(controls_window, 0, 0);

  mvwprintw(controls_window, 0, (CONTROLS_M - 8) / 2, "Controls");

  mvwprintw(controls_window, 2, 2, "START  r");
  mvwprintw(controls_window, 4, 2, "PAUSE  p");
  mvwprintw(controls_window, 6, 2, "ROTATE z, up arrow");
  mvwprintw(controls_window, 8, 2, "LEFT   left arrow");
  mvwprintw(controls_window, 10, 2, "RIGHT  right arrow");
  mvwprintw(controls_window, 12, 2, "DOWN   down arrow");
  mvwprintw(controls_window, 14, 2, "QUITE   q");

  return controls_window;
}

/**
 * @brief Функция печатает игровое поле.
 *
 * @return Указатель на структуру с данными об окне с игровым полем.
 */

WINDOW *print_game_field(GameInfo_t gi) {
  WINDOW *game_window =
      newwin(GAME_FIELD_N, GAME_FIELD_M, TOP_MARGIN, CONTROLS_M + 1);

  keypad(game_window, 1);
  nodelay(game_window, 1);

  for (int i = 0; i < FIELD_N; i++) {
    for (int j = 0; j < FIELD_M; j++) {
      if (gi.field[i][j] == 1) {
        wattron(game_window, COLOR_PAIR(5));
        mvwprintw(game_window, i + 1, 2 * j + 1, "  ");
        wattroff(game_window, COLOR_PAIR(5));
      } else if (gi.field[i][j] == 0) {
        wattron(game_window, COLOR_PAIR(3));
        mvwprintw(game_window, i + 1, 2 * j + 1, "  ");
        wattroff(game_window, COLOR_PAIR(3));
      } else {
        wattron(game_window, COLOR_PAIR(1));
        mvwprintw(game_window, i + 1, 2 * j + 1, "  ");
        wattroff(game_window, COLOR_PAIR(1));
      }
    }
  }
  return game_window;
}

/**
 * @brief Функция печатает игровую информацию.
 *
 * Функция печатает игровую информацию:
 * - следующая фигура
 * - максимальный счет
 * - текущий счет
 * - уровень
 * - скорость
 *
 * @return Указатель на структуру с данными об окне с игровой информацией.
 */

WINDOW *print_game_info(GameInfo_t gi) {
  WINDOW *info_window = newwin(GAME_FIELD_N, GAME_INFO_M, TOP_MARGIN,
                               CONTROLS_M + GAME_FIELD_M + 1);
  box(info_window, 0, 0);

  mvwprintw(info_window, 0, (GAME_INFO_M - 9) / 2, "Game_Info");

  print_next_figure(info_window, gi);
  mvwprintw(info_window, 8, 2, "High score  %d", gi.high_score);
  mvwprintw(info_window, 11, 2, "Score       %d", gi.score);
  mvwprintw(info_window, 14, 2, "Level       %d", gi.level);
  mvwprintw(info_window, 17, 2, "Speed       %d", gi.speed);

  return info_window;
}

/**
 * @brief Функция печатает в окно с игровой информацией следующую фигуру.
 *
 * Считывает значения из струтуры GameInfo_t и печатает справа от игрового поля
 * следующую фигуру.
 *
 */

void print_next_figure(WINDOW *info_window, GameInfo_t gi) {
  mvwprintw(info_window, 2, 2, "Next");

  for (int i = 0; i < gi.next_size; i++) {
    for (int j = 0; j < gi.next_size; j++) {
      if (gi.next[i][j] == 1) {
        wattron(info_window, COLOR_PAIR(5));
        mvwprintw(info_window, i + 4, j * 2 + 4, "  ");
        wattroff(info_window, COLOR_PAIR(5));
      }
    }
  }
}