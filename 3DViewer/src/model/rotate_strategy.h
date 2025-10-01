/**
 * @file rotate_strategy.h
 * @brief Интерфейсы и реализации стратегий поворота модели.
 *
 * Содержит абстрактную стратегию вращения и конкретные реализации по
 * осям X, Y, Z. Применяется через контекст Apply_rotation_strategy.
 *
 * Используется паттерн «Стратегия» для выбора направления вращения во
 * время выполнения.
 */

#pragma once

#include <numbers>
#include <string>
#include <vector>

#include "common.h"
// #include "model.h"

namespace s21 {
/**
 * @class Rotate_strategy
 * @brief Абстрактная стратегия вращения вершин в 2D-плоскости.
 *
 * Является базовым классом для конкретных стратегий вращения вокруг осей X, Y и
 * Z.
 *
 * Используется паттерн **Стратегия** (Strategy): позволяет задавать
 * алгоритм вращения независимо от клиента, который его использует.
 */
class Rotate_strategy {
 public:
  /**
   * @brief Конструктор стратегии вращения.
   * @param angle Угол в градусах, на который нужно повернуть.
   *
   * Конструктор преобразует угол в радианы и вычисляет синус и косинус
   * для последующего использования при вращении.
   */
  Rotate_strategy(float angle);
  /**
   * @brief Абстрактный метод вращения модели.
   * @param vertices Список вершин, которые необходимо повернуть.
   *
   * Должен быть переопределён в подклассах для конкретной оси вращения.
   */
  virtual void rotation(Vertices &vertices) = 0;

 protected:
  /**
   * @brief Выполняет вращение пары координат в 2D-плоскости.
   * @param a Первая координата (например, x или y).
   * @param b Вторая координата (например, y или z).
   * @param invertSin Если true, то синус берётся с противоположным знаком.
   *
   * Используется в `RotateX`, `RotateY`, `RotateZ` для вращения в
   * соответствующих плоскостях.
   */
  void rotate2D(float &a, float &b, bool invertSin);

 private:
  /**
   * @brief Предварительно вычисленное значение косинуса угла.
   */
  float cosA;

  /**
   * @brief Предварительно вычисленное значение синуса угла.
   */
  float sinA;

  /**
   * @brief Константа числа π (пи).
   */
  static constexpr float PI = 3.14159265358979323846f;
};
/**
 * @class RotateX
 * @brief Конкретная стратегия вращения модели вокруг оси X.
 *
 * Реализация паттерна **Стратегия**.
 */
class RotateX : public Rotate_strategy {
  using Rotate_strategy::Rotate_strategy;

 public:
  void rotation(Vertices &vertices) override;
};
/**
 * @class RotateY
 * @brief Конкретная стратегия вращения модели вокруг оси Y.
 *
 * Реализация паттерна **Стратегия**.
 */
class RotateY : public Rotate_strategy {
  using Rotate_strategy::Rotate_strategy;

 public:
  void rotation(Vertices &vertices) override;
};
/**
 * @class RotateZ
 * @brief Конкретная стратегия вращения модели вокруг оси Z.
 *
 * Реализация паттерна **Стратегия**.
 */
class RotateZ : public Rotate_strategy {
  using Rotate_strategy::Rotate_strategy;

 public:
  void rotation(Vertices &vertices) override;
};
/**
 * @class Apply_rotation_strategy
 * @brief Контекст для применения стратегии вращения.
 *
 * Хранит ссылку на объект стратегии вращения (`Rotate_strategy`) и вызывает её
 * метод `rotation()`.
 *
 * Позволяет динамически подменять стратегию без изменения клиентского кода.
 *
 * @note Применяется паттерн **Стратегия**.
 */
class Apply_rotation_strategy {
 private:
  /**
   * @brief Указатель на текущую стратегию вращения.
   *
   * Объект, реализующий конкретную стратегию (RotateX, RotateY или RotateZ).
   */
  Rotate_strategy *rotate_strategy;

 public:
  /**
   * @brief Конструктор контекста вращения.
   * @param r Указатель на стратегию вращения, которую следует использовать.
   */
  Apply_rotation_strategy(Rotate_strategy *r);

  /**
   * @brief Применяет текущую стратегию вращения к вершинам.
   * @param vertices Список вершин, к которым применяется вращение.
   */
  void apply_rotation(Vertices &vertices);
};

}  // namespace s21