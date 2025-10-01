#include "backend_s.hpp"

#include <cstdio>
#include <iostream>

namespace s21 {

/**
 * @brief Конструткор класса
 */

game::game() {
  field = new int *[FIELD_N];
  for (int i = 0; i < FIELD_N; ++i) {
    field[i] = new int[FIELD_M];
  }
  for (int i = 0; i < FIELD_N; i++) {
    for (int j = 0; j < FIELD_M; j++) {
      field[i][j] = 0;
    }
  }
  FILE *file1 = fopen("high_score_snake.txt", "r");
  if (file1 != NULL) {
    if (!fscanf(file1, "%d", &high_score)) high_score = 0;
    fclose(file1);
  } else {
    high_score = 0;
    perror("Open error");
  }
  score = 0;
  level = 1;
  action = Uninitialized;
  direction = Left_dir;
  pause = true;
  is_playing = true;
  lost = true;
  speed = 1000;
  score_for_speed = 0;
  for (int i = 0; i < 4; i++) {
    position pos = {i + 2, 10};
    snake.push_back(pos);
  }
  create_apple();
}

/**
 * @brief Деструктор класса
 */

game::~game() {
  if (field != nullptr) {
    for (int i = 0; i < FIELD_N; ++i) {
      delete[] field[i];
    }
    delete[] field;
  }
  field = nullptr;
}

/**
 * @brief Функция создает динамическую матрицу
 *
 * Выделяет память под матрицу заданного размера.
 *
 * @return указатель матрицу.
 */

int **game::create_matrix(int height, int width) {
  int **matrix = new int *[height];

  for (int i = 0; i < height; i++) {
    int *row = new int[width];
    matrix[i] = row;

    for (int j = 0; j < width; j++) {
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

/**
 * @brief Функция проверяет остается змейка в рамках игрового поля
 *
 * Функция проверяет остается змейка в рамках игрового поля.
 * Если произошло столкновение с краями поля, либо с самой змейкой,
 * переменная is_playing принимает значение false.
 */

void game::check_in_game() {
  if (snake[0].x < 0 || snake[0].x > FIELD_M - 1 || snake[0].y < 0 ||
      snake[0].y > FIELD_N - 1)
    is_playing = false;
  for (size_t i = 1; i < snake.size(); i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      is_playing = false;
      break;
    }
  }
}

/**
 * @brief Функция генерирует координаты яблока
 *
 * Функция случайным образом генерирует координаты яблока.
 */

void game::create_apple() {
  bool flag = true;
  while (flag) {
    srand(time(NULL));  // обнуление рандомайзера
    apple.x = rand() % 10;
    srand(time(NULL));  // обнуление рандомайзера
    apple.y = rand() % 20;
    flag = false;
    for (size_t i = 0; i < snake.size(); i++) {
      if (snake[i].x == apple.x && snake[i].y == apple.y) {
        flag = true;
      }
    }
  }
}

/**
 * @brief Функция передвигает змейку по полю
 *
 * Функция проверяет значение переменной action. И, в
 * зависимости от результата, передвигает змейку по игровому полю.
 * Если координаты первого элемента змейки совпадают с координатами яблока,
 * происходит увеличение змейки и генерауция нового яблока
 */

void game::move() {
  position prev;
  for (size_t i = 0; i < snake.size(); i++) {
    int y_temp = snake[i].y;
    int x_temp = snake[i].x;
    if (i == 0) {
      prev = snake[i];
      switch (direction) {
        case Up_dir: {
          y_temp--;
          if (y_temp >= 0 && y_temp < FIELD_N)
            snake[0].y--;
          else
            is_playing = false;
        } break;
        case Down_dir: {
          y_temp++;
          if (y_temp >= 0 && y_temp < FIELD_N)
            snake[0].y++;
          else
            is_playing = false;
        } break;
        case Left_dir: {
          x_temp--;
          if (x_temp < 0 || x_temp > FIELD_M - 1)
            is_playing = false;
          else
            snake[0].x--;
          break;
        }
        case Right_dir:
          x_temp++;
          if (x_temp >= 0 && x_temp < FIELD_M)
            snake[0].x++;
          else
            is_playing = false;
          break;
      }
    } else if ((i == snake.size() - 1) && is_playing) {
      position next = snake[i];
      snake[i] = prev;
      last = next;
    } else {
      if (is_playing) {
        position next = snake[i];
        snake[i] = prev;
        prev = next;
      }
    }
  }
  if (snake[0].x == apple.x && snake[0].y == apple.y && is_playing) {
    position pos = {last.x, last.y};
    snake.push_back(pos);
    score++;
    if (score > high_score) {
      high_score = score;
    }
    score_for_speed++;
    if (score_for_speed == 5 && level < 10) {
      score_for_speed = 0;
      level++;
      speed = levels[level];
    }
    if (score >= 200) {
      is_playing = false;
      lost = false;
    }
    create_apple();
  }
}

/**
 * @brief Функция обрабатывает нажатие пользователя.
 *
 * Функция проверяет значение переменной action. И, в
 * зависимости от результата, запускает функции, модифицирующие данные игровой
 * сессии.
 *
 * @param action Действие пользователя.
 * @param hold Параметр отвечающий за удерживание клавиши
 */

void game::userInput(UserAction_t action, bool hold) {
  if (action == Terminate && hold == false) {
    is_playing = false;
  } else if (action == Action) {
    move();
  } else if (action == Up) {
    if (direction == Left_dir || direction == Right_dir) direction = Up_dir;
    move();
  } else if (action == Left) {
    if (direction == Up_dir || direction == Down_dir) direction = Left_dir;
    move();
  } else if (action == Right) {
    if (direction == Up_dir || direction == Down_dir) direction = Right_dir;
    move();
  } else if (action == Down) {
    if (direction == Left_dir || direction == Right_dir) direction = Down_dir;
    move();
  } else
    move();
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

GameInfo_t game::updateCurrentState() {
  GameInfo_t gi;

  gi.score = score;
  gi.level = level;
  gi.speed = speed;

  gi.field = create_matrix(FIELD_N, FIELD_M);

  for (size_t i = 0; i < snake.size(); i++) {
    gi.field[snake[i].y][snake[i].x] = 1;
  }
  gi.field[apple.y][apple.x] = 2;

  gi.next = create_matrix(1, 1);
  gi.next_size = 1;

  int high_score = 0;
  FILE *file = fopen("high_score_snake.txt", "r");
  if (file) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }

  gi.high_score = high_score;

  return gi;
}

/**
 * @brief Функция сохранет максимальный счет в файл
 *
 * Функция открывает файл на запись и сохранет в него максимальный счет
 */

void game::save_settings() {
  FILE *file = fopen("high_score_snake.txt", "w");
  if (file != NULL) {
    fprintf(file, "%d", high_score);
    fclose(file);
  }
}
/**
 * @brief Функция освобождает память переменной класса GameInfo_t
 *
 * @param gi cсылка на объект класса GameInfo_t
 */
void game::free_game_info_s(GameInfo_t *gi) {
  free_matrix_s(FIELD_N, gi->field);
  free_matrix_s(gi->next_size, gi->next);
}

/**
 * @brief Функция освобождает память
 *
 *
 * @param N размер матрицы.
 * @param matrix указатель на матрицу.
 */

void game::free_matrix_s(int N, int **matrix) {
  if (matrix != NULL) {
    for (int i = 0; i < N; i++) {
      free(matrix[i]);
    }
    free(matrix);
  }

  matrix = NULL;
}

}  // namespace s21