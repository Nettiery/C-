/**
 * @file controller.h
 * @brief Интерфейс контроллера (паттерн Команда) для связи View и Model.
 */

#pragma once

#include "commands.h"

namespace s21 {
/**
 * @class Controller
 * @brief Реализация контроллера с использованием паттерна **Команда
 * (Command)**.
 *
 * Контроллер предоставляет единый интерфейс для выполнения любых действий
 * с моделью, передавая объект команды (`ICommand`). Поддерживает расширяемость
 * и инкапсуляцию логики, что упрощает реализацию паттерна **MVC**.
 */
class Controller {
 private:
  /**
   * @brief Объект модели, над которой выполняются команды.
   */
  Model model_;

 public:
  /**
   * @brief Выполняет переданную команду, применяя её к модели.
   * @param cmd Уникальный указатель на объект команды, реализующий интерфейс
   * ICommand.
   */
  void executeCommand(std::unique_ptr<ICommand> cmd) { cmd->execute(model_); }

  /**
   * @brief Возвращает модель (только для чтения).
   */
  const Model &getModel() const { return model_; }

  /**
   * @brief Возвращает текущие вершины модели.
   */
  const Vertices &getVertices() const { return model_.vertices; }

  /**
   * @brief Возвращает текущие полигоны модели.
   */
  const Polygons &getPolygons() const { return model_.polygons; }
};
}  // namespace s21