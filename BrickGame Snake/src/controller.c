#include "controller.h"

#include <stdio.h>
#include <stdlib.h>
/** @file */

/**
 * @brief Функция обрабатывает ввод пользователя
 *
 * В зависимости от нажатой клавиши функция пристаивает переменной UserAction_t
 * action  отдно из значений из перечисления UserAction_t.
 *
 */

UserAction_t get_user_action(int input) {
  UserAction_t action = Uninitialized;
  if (input == KEY_R) {
    action = Start;
  } else if (input == KEY_P)
    action = Pause;
  else if (input == KEY_Q)
    action = Terminate;
  else if (input == KEY_LEFT)
    action = Left;
  else if (input == KEY_RIGHT)
    action = Right;
  else if (input == KEY_UP)
    action = Up;
  else if (input == KEY_DOWN)
    action = Down;
  else if (input == KEY_SPACE)
    action = Action;
  return action;
}
