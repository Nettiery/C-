#include "backend_t.h"

#include <stdio.h>
#include <stdlib.h>

/** @file */

/**
 * @brief Функция инициализирует игровое поле, пристваивая полям структуры
 * GameState_t стартовые значения.
 * @return статус выполнения функции
 */

void init_game() {
  GameState_t *gs = create_game_state();
  gs->x = START_X;
  gs->y = START_Y;
  gs->status = Initial;
  gs->score = 0;
  gs->level = 1;
  gs->speed = 1000;
  gs->pause = false;
  gs->is_playing = true;
  gs->win = false;
  gs->time = get_time();
  gs->field = create_matrix(FIELD_N, FIELD_M);
  srand(get_time());

  int size;
  gs->next_figure = generate_figure(&size);
  gs->next_figure_size = size;

  gs->figure = create_matrix(size, size);
  gs->figure_size = size;
  copy_matrix(size, size, gs->next_figure, gs->figure);

  FILE *file = fopen("high_score.txt", "r");
  if (file) {
    fscanf(file, "%d", &gs->high_score);
    fclose(file);
  }
}

/**
 * @brief Функция генерирует новую фигуру.
 *
 *  Функция случайным образом генерирует тип фигуры. Далее, с помощью функции
 * fill_figure(), заполняет матрицу, содержащую фигуру.
 *
 * @return указатель на двумерную матрицу, содержащую фигиру.
 */

int **generate_figure(int *size) {
  int random_type = rand() % NUM_FIGURES;

  *size = (random_type == 0) ? 4 : (random_type == 3) ? 2 : 3;

  int **figure = create_matrix(*size, *size);

  if (figure != NULL) fill_figure(random_type, figure);

  return figure;
}

/**
 * @brief Функция заполняет матрицу в соответствии с типом фигуры.
 *
 *  Функция случайным образом генерирует тип фигуры. Далее, с помощью функции
 * fill_figure(), заполняет матрицу, содержащую фигуру.
 *
 * @param random_type Тип фигуры.
 * @param figure Указатель на двумерную матрицу, которую необходимо заполнить.
 */

GameState_t *get_current_state() {
  static GameState_t gs;
  return &gs;
}

void fill_figure(int random_type, int **figure) {
  switch (random_type) {
    case 0:
      figure[0][0] = 1;
      figure[0][1] = 1;
      figure[0][2] = 1;
      figure[0][3] = 1;
      break;
    case 1:
      figure[0][0] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      break;
    case 2:
      figure[0][2] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      break;
    case 3:
      figure[0][0] = 1;
      figure[0][1] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      break;
    case 4:
      figure[0][1] = 1;
      figure[0][2] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      break;
    case 5:
      figure[0][1] = 1;
      figure[1][0] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      break;
    case 6:
      figure[0][0] = 1;
      figure[0][1] = 1;
      figure[1][1] = 1;
      figure[1][2] = 1;
      break;
    default:
      break;
  }
}

/**
 * @brief Функция выполняет запись данных из структуры GameState_t в GameInfo_t
 * для дальнейшей графической прорисовки
 *
 * Функция копирует значения атрибутов из структуры GameState_t в GameInfo_t,
 * для дальнейшей отрисовки.
 *
 * @param gs Структура содержит параметры игрового.
 * @return Структура содержит параметры игрового, необходимые для графической
 * отрисовки
 */

GameInfo_t updateCurrentState_(GameState_t *gs) {
  GameInfo_t gi;

  gi.score = gs->score;
  gi.level = gs->level;
  gi.speed = gs->speed;

  gi.field = create_matrix(FIELD_N, FIELD_M);
  copy_matrix(FIELD_N, FIELD_M, gs->field, gi.field);

  if (gi.field != NULL) update_field(gs, gi.field);

  int size = gs->next_figure_size;
  gi.next = create_matrix(size, size);
  copy_matrix(size, size, gs->next_figure, gi.next);
  gi.next_size = size;

  gi.high_score = gs->high_score;
  gi.pause = gs->pause;

  return gi;
}

/**
 * @brief Функция записывает в матрицу игрового поля фигуру.
 *
 * @param gs Структура, содержащая параметры игрового поля.
 * @param field Матрица, содержащая фигуру.
 */

void update_field(GameState_t *gs, int **field) {
  for (int i = 0; i < gs->figure_size; i++) {
    for (int j = 0; j < gs->figure_size; j++) {
      int x = gs->x + j;
      int y = gs->y + i;

      if (gs->figure[i][j] == 1 && y > -1 && y < FIELD_N && x > -1 &&
          x < FIELD_M)
        field[y][x] = 1;
    }
  }
}

/**
 * @brief Функция обрабатывает нажатие пользователя.
 *
 * Функция проверяет значение атрибута gs.status и переменной action. И, в
 * зависимости от результата, запускает функции, модифицирующие структуру gs.
 *
 * @param gs Структура, содержащая параметры игрового поля.
 * @param action Действие пользователя.
 */

void userInput_(UserAction_t action, GameState_t *gs) {
  if (gs->status == Initial) {
    if (action == Terminate)
      finish_game(gs);
    else if (action == Start)
      gs->status = Spawn;

  } else if (gs->status == Spawn) {
    if (action == Terminate || gs->win)
      finish_game(gs);
    else {
      spawn_figure(gs);
    }
  } else if (gs->status == Moving) {
    move_figure(gs, action);
  } else if (gs->status == Shifting) {
    move_down(gs);
  } else if (gs->status == Attaching) {
    attach_figure(gs);
  } else if (gs->status == GameOver) {
    finish_game(gs);
  }
}

/**
 * @brief Функция заканчивает игру.
 *
 * Освобождается память, выделенная для матрицы, содержащей поле игры, текущей и
 * следующей фигуры.
 *
 * @param gs Структура, содержащая параметры игрового поля.
 */

void finish_game(GameState_t *gs) {
  if (gs->status != GameOver && !gs->win) gs->is_playing = false;

  free_matrix(FIELD_N, gs->field);
  free_matrix(gs->figure_size, gs->figure);
  free_matrix(gs->next_figure_size, gs->next_figure);
}

/**
 * @brief Функция генерирует новую следующую фигуру.
 *
 * Функция копирует next_figure в figure. Затем вызывает функцию
 * generate_figure() и создает новую следующую фигуру.
 *
 * @param gs Структура содержит параметры игрового.
 */

void spawn_figure(GameState_t *gs) {
  free_matrix(gs->figure_size, gs->figure);
  gs->figure = gs->next_figure;
  gs->figure_size = gs->next_figure_size;

  int size;
  gs->next_figure = generate_figure(&size);
  gs->next_figure_size = size;

  gs->x = (gs->figure_size == 2) ? 4 : 3;
  gs->y = 0;

  gs->time = get_time();
  gs->status = Moving;
}

/**
 * @brief Функция перемещает фигуру на один шаг.
 *
 * Функция перемещает фигуру: влево, вправо, вниз, поворот, если не нажата
 * кнопка Пауза.
 *
 * @param gs Структура содержит параметры игрового.
 * @param action Действие пользователя.
 */

void move_figure(GameState_t *gs, UserAction_t action) {
  if (action == Left && !gs->pause)
    move_left(gs);
  else if (action == Right && !gs->pause)
    move_right(gs);
  else if (action == Down && !gs->pause)
    move_down(gs);
  else if ((action == Action || action == Up) && !gs->pause)
    rotate(gs);
  else if (action == Pause)
    gs->pause = !gs->pause;
  else if (action == Terminate)
    finish_game(gs);
  if (timer(gs, gs->speed) && !gs->pause) gs->status = Shifting;
}

/**
 * @brief Функция двигает фигуру влево.
 *
 * Функция проверяет возможно ли перемещение, и, если возможно, двигает фигуру
 * влево. Затем, осуществляется проверка достижения нижнего края.
 *
 * @param gs Структура содержит параметры игрового.
 */

void move_left(GameState_t *gs) {
  bool can_move = true;

  for (int i = 0; i < gs->figure_size; i++) {
    for (int j = 0; j < gs->figure_size; j++) {
      int x = gs->x + j - 1;
      int y = gs->y + i;

      if (gs->figure[i][j] == 1 &&
          (x >= FIELD_M || x < 0 || gs->field[y][x] == 1))
        can_move = false;
    }
  }

  if (can_move) gs->x--;

  gs->status = figure_is_attaching(gs) ? Attaching : Moving;
}

/**
 * @brief Функция двигает фигуру вправо.
 *
 * Функция проверяет возможно ли перемещение, и, если возможно, двигает фигуру
 * вправо. Затем, осуществляется проверка достижения нижнего края и фигура либо
 * прикрепляется, либо движется вниз дальше.
 *
 * @param gs Структура содержит параметры игрового.
 */

void move_right(GameState_t *gs) {
  bool can_move = true;

  for (int i = 0; i < gs->figure_size; i++) {
    for (int j = 0; j < gs->figure_size; j++) {
      int x = gs->x + j + 1;
      int y = gs->y + i;

      if (gs->figure[i][j] == 1 &&
          (x >= FIELD_M || x < 0 || gs->field[y][x] == 1))
        can_move = false;
    }
  }

  if (can_move) gs->x++;

  gs->status = figure_is_attaching(gs) ? Attaching : Moving;
}

/**
 * @brief Функция двигает фигуру вниз.
 *
 * Функция проверяет возможно ли перемещение, и, если возможно, ускоряет
 * движение фигуры вниз. Затем, осуществляется проверка достижения нижнего края
 * и фигура либо прикрепляется, либо движется вниз дальше.
 *
 * @param gs Структура содержит параметры игрового.
 */

void move_down(GameState_t *gs) {
  if (!figure_is_attaching(gs)) {
    gs->y++;
    gs->status = Moving;
  } else {
    gs->status = Attaching;
  }
}

/**
 * @brief Функция поворачивает фигуру.
 *
 * Функция записывает повернутую фигуру во временную матрицу. Далее происходит
 * проверка, сможет ли перевернутая фигура разместиться на поле. Если возможно,
 * новая фигура записывается в gs->figure.
 *
 * @param gs Структура содержит параметры игрового.
 */

void rotate(GameState_t *gs) {
  bool can_move = true;

  int **temp_figure = create_matrix(gs->figure_size, gs->figure_size);

  for (int i = 0; i < gs->figure_size; i++) {
    for (int j = 0; j < gs->figure_size; j++) {
      temp_figure[j][gs->figure_size - i - 1] = gs->figure[i][j];
    }
  }

  for (int i = 0; i < gs->figure_size; i++) {
    for (int j = 0; j < gs->figure_size; j++) {
      int x = gs->x + j;
      int y = gs->y + i;

      if (temp_figure[i][j] == 1 && (y >= FIELD_N || y < 0 || x >= FIELD_M ||
                                     x < 0 || gs->field[y][x] == 1))
        can_move = false;
    }
  }

  if (can_move) {
    free_matrix(gs->figure_size, gs->figure);
    gs->figure = temp_figure;
  } else {
    free_matrix(gs->figure_size, temp_figure);
  }
}

/**
 * @brief Функция останавливает фигуру после достижения нижней границы поля или
 * соприкосновения с другой фигурой.
 *
 * Функция проверяет достижение верхней границы игрового поля. Если граница поля
 * не достигнута, фигура останавливается, происходит подсчет полных линий и
 * вслед за этим происходит генерация следующей. Если верхняя граница
 * достигнута, игра заканчивается.
 *
 * @param gs Структура содержит параметры игрового.
 */

void attach_figure(GameState_t *gs) {
  bool is_game_over = check_top_line(gs);
  for (int i = 0; i < gs->figure_size && !is_game_over; i++) {
    for (int j = 0; j < gs->figure_size; j++) {
      if (gs->figure[i][j] == 1) {
        gs->field[gs->y + i][gs->x + j] = 1;
      }
    }
  }
  if (check_top_line(gs)) {
    gs->status = GameOver;
  } else {
    check_full_lines(gs);
    gs->status = Spawn;
  }
}

/**
 * @brief Функция проверяет достижение верхней границы игрового поля.
 *
 * Функция проверяет ячейки верхней границы игрового поля, и, если хоть одна
 * ячейка заполнена, считается что достигнута верхняя граница.
 *
 * @param gs Структура содержит параметры игрового.
 * @return Булевое значение: верхняя граница заполнена - true, не заполнена -
 * false.
 */

bool check_top_line(GameState_t *gs) {
  bool is_filled = false;

  for (int j = 0; j < FIELD_M && !is_filled; j++) {
    is_filled = gs->field[0][j] == 1;
  }

  return is_filled;
}

/**
 * @brief Функция подсчитывает количество заполненных строк.
 *
 * Функция проверяет ячейки каждой строки игрового поля. Если все ячейки одной
 * строки заполнены, количество заполненных строк увеличивается и заполненная
 * строка исчезает. Обновляется счет и уровень.
 *
 * @param gs Структура содержит параметры игрового.
 */

void check_full_lines(GameState_t *gs) {
  int num_full_lines = 0;

  for (int i = FIELD_N - 1; i >= 0; i--) {
    bool is_full = true;

    for (int j = 0; j < FIELD_M && is_full; j++) {
      if (gs->field[i][j] == 0) {
        is_full = false;
      }
    }

    if (is_full) {
      shift_lines(gs, i);
      fill_top_line(gs);
      num_full_lines++;
      i++;
    }
  }
  update_score_and_level(gs, num_full_lines);
}

/**
 * @brief Функция уничтожает заполненные строки.
 *
 * Функция смещает строки на одну ячейку вниз, начиная с заполненной строки.
 *
 * @param gs Структура содержит параметры игрового.
 */

void shift_lines(GameState_t *gs, int i) {
  for (int j = i; j > 0; j--) {
    for (int k = 0; k < FIELD_M; k++) {
      gs->field[j][k] = gs->field[j - 1][k];
    }
  }

  for (int i = 0; i < gs->figure_size; i++) {
    for (int j = 0; j < gs->figure_size; j++) {
      gs->figure[i][j] = 0;
    }
  }
}

/**
 * @brief Функция заполняет верхнюю строку пустыми ячейками.
 *
 * @param gs Структура содержит параметры игрового.
 */

void fill_top_line(GameState_t *gs) {
  for (int j = 0; j < FIELD_M; j++) {
    gs->field[0][j] = 0;
  }
}

/**
 * @brief Функция подсчитывает количество заработанных очков и обновляет
 * уровень.
 *
 * Функция подсчитывает количество заработанных очков, в зависимости от
 * количества заполненных строк, и обновляет уровень, если необходимо.
 *
 * @param gs Структура содержит параметры игрового.
 */

void update_score_and_level(GameState_t *gs, int num_full_lines) {
  switch (num_full_lines) {
    case 1:
      gs->score += 100;
      break;
    case 2:
      gs->score += 300;
      break;
    case 3:
      gs->score += 700;
      break;
    case 4:
      gs->score += 1500;
      break;
    default:
      break;
  }

  if (gs->score >= SCORE_PER_LEVEL) {
    gs->level = gs->score / SCORE_PER_LEVEL + 1;
    gs->speed = DELAY_MS * pow(0.9, gs->level);
  }

  if (gs->level > MAX_LEVEL) gs->win = true;

  if (gs->score > gs->high_score) {
    FILE *file = fopen("high_score.txt", "w");

    if (file) {
      fprintf(file, "%d", gs->score);
      fclose(file);
    }
  }
}

/**
 * @brief Функция создает переменную с параметрами игрового поля типа
 * GameState_t.
 *
 * @return gs Структура содержит параметры игрового.
 */

GameState_t *create_game_state() {
  static GameState_t gs;
  return &gs;
}

/**
 * @brief Функция создает динамическую матрицу
 *
 * Выделяет память под матрицу заданного размера.
 *
 * @return указатель матрицу.
 */

int **create_matrix(int N, int M) {
  int **result = (int **)calloc(N, sizeof(int *));

  if (result != NULL) {
    for (int i = 0; i < N; i++) {
      result[i] = (int *)calloc(M, sizeof(int));

      if (result[i] == NULL) {
        free_matrix(i, result);
        break;
      }
    }
  }

  return result;
}

/**
 * @brief Функция копирует значения одну матрицу в другую.
 *
 * Выделяет памят под матрицу заданного размера.
 *
 * @param N высота матрицы.
 * @param M ширина матрицы.
 * @param src_matrix указатель на матрицу - образец.
 * @param dest_matrix указатель на матрицу, в которую копируем.
 */

void copy_matrix(int N, int M, int **src_matrix, int **dest_matrix) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      dest_matrix[i][j] = src_matrix[i][j];
    }
  }
}

/**
 * @brief Функция освобождает память
 *
 *
 * @param N размер матрицы.
 * @param matrix указатель на матрицу.
 */

void free_matrix(int N, int **matrix) {
  if (matrix != NULL) {
    for (int i = 0; i < N; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }

  matrix = NULL;
}

/**
 * @brief Функция копирует значения одну матрицу в другую.
 *
 * Выделяет памят под матрицу заданного размера.
 *
 * @param N высота матрицы.
 * @param M ширина матрицы.
 * @param src_matrix указатель на матрицу - образец.
 * @param dest_matrix указатель на матрицу, в которую копируем.
 */

bool figure_is_attaching(GameState_t *gs) {
  bool is_attaching = false;

  for (int i = 0; i < gs->figure_size && !is_attaching; i++) {
    for (int j = 0; j < gs->figure_size && !is_attaching; j++) {
      int x = gs->x + j;
      int y = gs->y + i + 1;

      if (gs->figure[i][j] == 1 &&
          (y > FIELD_N - 1 || (y > -1 && gs->field[y][x] == 1)))
        is_attaching = true;
    }
  }

  return is_attaching;
}

/**
 * @brief Функция считывает текущее время и преобразует для дальнейшего
 * использования
 *
 * @return длинное целое, вычисляемое по формуле (секунды*1000 +
 * миллисекунды/1000)
 */

long long get_time() {
  struct timeval t;
  gettimeofday(&t, NULL);

  return (long long)t.tv_sec * 1000 + t.tv_usec / 1000;
}

/**
 * @brief Функция проверяет сколько времени прошло после последнего действия
 *
 * Обновляет значение переменной gs->time, с момнета последнего действия прошло
 * больше, чем установленная задержка
 *
 * @return булевое значение. True - если  gs->time обновлено, false - если
 * осталось прежним.
 */

bool timer(GameState_t *gs, int delay) {
  bool result = false;
  long long time = get_time();

  if (time - gs->time >= delay) {
    gs->time = time;
    result = true;
  }

  return result;
}

/**
 * @brief Функция освобождает память матрицы игрового поля.
 *
 * @param gi Структура содержит параметры игрового поля, необходимые для
 * графического отображения игры на каждом шаге.
 */

void free_game_info(GameInfo_t *gi) {
  free_matrix(FIELD_N, gi->field);
  free_matrix(gi->next_size, gi->next);
}
