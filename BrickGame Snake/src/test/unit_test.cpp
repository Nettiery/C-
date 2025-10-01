#include <gtest/gtest.h>

#include <cstdio>
#include <iostream>

// #include "../brick_game/backend.hpp"
#include "../brick_game/snake/backend_s.cpp"
#include "../brick_game/snake/backend_s.hpp"
#include "../controller.h"
// #include "../brick_game/tetris/backend_t.h"
// #include "../gui/cli/tetris/frontend_t.h"

using namespace s21;

TEST(SNAKE, Constructor_Destructor) {
  game new_game;
  ASSERT_EQ(new_game.score, 0);
  ASSERT_EQ(new_game.level, 1);
  ASSERT_EQ(new_game.action, Uninitialized);
  ASSERT_EQ(new_game.direction, 0);
  ASSERT_EQ(new_game.pause, true);
  ASSERT_EQ(new_game.lost, true);
  ASSERT_EQ(new_game.speed, 1000);
  ASSERT_EQ(new_game.snake[0].x, 2);
  ASSERT_EQ(new_game.snake[0].y, 10);
  ASSERT_EQ(new_game.snake[1].x, 3);
  ASSERT_EQ(new_game.snake[1].y, 10);
  ASSERT_EQ(new_game.snake[2].x, 4);
  ASSERT_EQ(new_game.snake[2].y, 10);
  ASSERT_EQ(new_game.snake[3].x, 5);
  ASSERT_EQ(new_game.snake[3].y, 10);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      ASSERT_EQ(new_game.field[i][j], 0);
    }
  }
  // delete new_game;
}

// TEST(SNAKE, create_matrix) {
//   game new_game;
//   int **field = new_game.create_matrix(5, 10);
//   ASSERT_EQ(field[0][0], NULL);
//   new_game.delete_matrix(field, 5);
// }

TEST(SNAKE, updateCurrentState) {
  game new_game;
  new_game.action = Start;
  new_game.userInput(Start, false);
  GameInfo_t gi = new_game.updateCurrentState();
  ASSERT_EQ(gi.level, 1);
  // free_matrix(FIELD_N, gi.field);
  // free_matrix(gi.next_size, gi.next);
  new_game.free_game_info_s(&gi);
}

TEST(SNAKE, CheckInGame1) {
  game new_game;
  new_game.check_in_game();
  ASSERT_EQ(new_game.is_playing, true);
}

TEST(SNAKE, Wait_for_resume) {
  game new_game;
  new_game.action = Start;
  new_game.userInput(Start, false);
  ASSERT_EQ(new_game.is_playing, true);
}

TEST(SNAKE, CheckInGame2) {
  game new_game;
  for (int i = 0; i < 5; i++) {
    new_game.move();
  }
  new_game.check_in_game();
  ASSERT_EQ(new_game.is_playing, false);
}

TEST(SNAKE, CheckInGame3) {
  game new_game;
  new_game.snake.push_back({6, 10});
  new_game.action = Up;
  new_game.userInput(Up, false);
  new_game.action = Right;
  new_game.userInput(Right, false);
  new_game.action = Down;
  new_game.userInput(Down, false);
  new_game.check_in_game();
  ASSERT_EQ(new_game.is_playing, false);
}

TEST(SNAKE, Move1) {
  game new_game;
  new_game.score = 4;
  new_game.score_for_speed = 4;
  new_game.apple = {1, 10};
  new_game.move();
  ASSERT_EQ(new_game.snake[0].x, 1);
  ASSERT_EQ(new_game.snake[0].y, 10);
  ASSERT_EQ(new_game.snake[1].x, 2);
  ASSERT_EQ(new_game.snake[1].y, 10);
  ASSERT_EQ(new_game.snake[2].x, 3);
  ASSERT_EQ(new_game.snake[2].y, 10);
  ASSERT_EQ(new_game.snake[3].x, 4);
  ASSERT_EQ(new_game.snake[3].y, 10);
  ASSERT_EQ(new_game.score, 5);
  ASSERT_EQ(new_game.score_for_speed, 0);
  ASSERT_EQ(new_game.level, 2);
  ASSERT_EQ(new_game.speed, 800);
}

TEST(SNAKE, WorkWithUserInput1) {
  game new_game;
  new_game.action = Terminate;
  new_game.userInput(Terminate, false);
  ASSERT_EQ(new_game.is_playing, false);
}

TEST(SNAKE, WorkWithUserInput2) {
  game new_game;
  new_game.action = Action;
  new_game.userInput(Action, false);
  ASSERT_EQ(new_game.snake[0].x, 1);
  ASSERT_EQ(new_game.snake[0].y, 10);
  ASSERT_EQ(new_game.snake[1].x, 2);
  ASSERT_EQ(new_game.snake[1].y, 10);
  ASSERT_EQ(new_game.snake[2].x, 3);
  ASSERT_EQ(new_game.snake[2].y, 10);
  ASSERT_EQ(new_game.snake[3].x, 4);
  ASSERT_EQ(new_game.snake[3].y, 10);
}

TEST(SNAKE, WorkWithUserInput3) {
  game new_game;
  new_game.action = Up;
  new_game.userInput(Up, false);
  ASSERT_EQ(new_game.snake[0].x, 2);
  ASSERT_EQ(new_game.snake[0].y, 9);
  ASSERT_EQ(new_game.snake[1].x, 2);
  ASSERT_EQ(new_game.snake[1].y, 10);
  ASSERT_EQ(new_game.snake[2].x, 3);
  ASSERT_EQ(new_game.snake[2].y, 10);
  ASSERT_EQ(new_game.snake[3].x, 4);
  ASSERT_EQ(new_game.snake[3].y, 10);
}

TEST(SNAKE, WorkWithUserInput4) {
  s21::game new_game;
  new_game.action = Down;
  new_game.userInput(Down, false);
  ASSERT_EQ(new_game.snake[0].x, 2);
  ASSERT_EQ(new_game.snake[0].y, 11);
  ASSERT_EQ(new_game.snake[1].x, 2);
  ASSERT_EQ(new_game.snake[1].y, 10);
  ASSERT_EQ(new_game.snake[2].x, 3);
  ASSERT_EQ(new_game.snake[2].y, 10);
  ASSERT_EQ(new_game.snake[3].x, 4);
  ASSERT_EQ(new_game.snake[3].y, 10);
}

TEST(SNAKE, WorkWithUserInput5) {
  s21::game new_game;
  new_game.action = Down;
  new_game.userInput(Down, false);
  new_game.action = Left;
  new_game.userInput(Left, false);
  ASSERT_EQ(new_game.snake[0].x, 1);
  ASSERT_EQ(new_game.snake[0].y, 11);
  ASSERT_EQ(new_game.snake[1].x, 2);
  ASSERT_EQ(new_game.snake[1].y, 11);
  ASSERT_EQ(new_game.snake[2].x, 2);
  ASSERT_EQ(new_game.snake[2].y, 10);
  ASSERT_EQ(new_game.snake[3].x, 3);
  ASSERT_EQ(new_game.snake[3].y, 10);
}

TEST(SNAKE, WorkWithUserInput6) {
  s21::game new_game;
  new_game.action = Down;
  new_game.userInput(Down, false);
  new_game.action = Right;
  new_game.userInput(Right, false);
  ASSERT_EQ(new_game.snake[0].x, 3);
  ASSERT_EQ(new_game.snake[0].y, 11);
  ASSERT_EQ(new_game.snake[1].x, 2);
  ASSERT_EQ(new_game.snake[1].y, 11);
  ASSERT_EQ(new_game.snake[2].x, 2);
  ASSERT_EQ(new_game.snake[2].y, 10);
  ASSERT_EQ(new_game.snake[3].x, 3);
  ASSERT_EQ(new_game.snake[3].y, 10);
}

TEST(SNAKE, WorkWithUserInput7) {
  s21::game new_game;
  new_game.userInput(Start, false);
  ASSERT_EQ(new_game.snake[0].x, 1);
  ASSERT_EQ(new_game.snake[0].y, 10);
  ASSERT_EQ(new_game.snake[1].x, 2);
  ASSERT_EQ(new_game.snake[1].y, 10);
  ASSERT_EQ(new_game.snake[2].x, 3);
  ASSERT_EQ(new_game.snake[2].y, 10);
  ASSERT_EQ(new_game.snake[3].x, 4);
  ASSERT_EQ(new_game.snake[3].y, 10);
}

TEST(SNAKE, SaveSettings) {
  s21::game new_game;
  new_game.high_score = 10;
  new_game.save_settings();
  int result = 0;
  FILE *file2 = fopen("high_score_snake.txt", "r");
  fscanf(file2, "%d", &result);
  fclose(file2);
  ASSERT_EQ(new_game.high_score, result);
}

TEST(SNAKE, Move2) {
  s21::game new_game;
  new_game.score = 199;
  new_game.apple = {1, 10};
  new_game.move();
  ASSERT_EQ(new_game.is_playing, 0);
  ASSERT_EQ(new_game.lost, false);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
