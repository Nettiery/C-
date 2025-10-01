/**
 * @file common.h
 * @brief Общие типы и структуры, используемые в 3D Viewer.
 *
 * Содержит определения вершин, треугольников, статусов модели и
 * пользовательских действий. Используется во всей архитектуре MVC.
 */

#pragma once

#include <cmath>      // std::abs
#include <iostream>   // std::ostream
#include <stdexcept>  // std::runtime_error
#include <vector>     // std::vector

namespace s21 {

// обозначение осей
typedef enum { uninitalized, X, Y, Z } Axis;

/**
 * @struct Vertex
 * @brief Представляет 3D-вершину модели.
 *
 * Содержит координаты (x, y, z) и операции над векторами:
 * сложение, вычитание, масштабирование и сравнение с учётом погрешности.
 */
struct Vertex {
  float x, y, z;

  static constexpr float kTol = 1e-6;

  Vertex operator*(float number) const {
    return Vertex{x * number, y * number, z * number};
  }
  Vertex operator/(float number) const {
    if (number == 0.0) throw std::runtime_error("Деление на 0");
    return Vertex{x / number, y / number, z / number};
  }
  Vertex operator+(Vertex other) const {
    return Vertex{x + other.x, y + other.y, z + other.z};
  }
  Vertex operator-(Vertex other) const {
    return Vertex{x - other.x, y - other.y, z - other.z};
  }
  bool operator==(const Vertex &other) const {
    return std::abs(x - other.x) < kTol && std::abs(y - other.y) < kTol &&
           std::abs(z - other.z) < kTol;
  }
  bool operator!=(const Vertex &other) const { return !(*this == other); }
};

/**
 * @brief Перегруженный вывод вершины в формате .obj.
 */
inline std::ostream &operator<<(std::ostream &os, const Vertex &v) {
  os << "v " << v.x << " " << v.y << " " << v.z;
  return os;
}

/**
 * @struct Triangle
 * @brief Представляет треугольник по индексам трёх вершин.
 */
struct Triangle {
  int v1, v2, v3;
};

/**
 * @brief Перегруженный вывод треугольника в формате .obj.
 */
inline std::ostream &operator<<(std::ostream &os, const Triangle &p) {
  os << "f " << p.v1 + 1 << " " << p.v2 + 1 << " " << p.v3 + 1;
  return os;
}

/**
 * @brief Удобный псевдоним для списка вершин.
 */
using Vertices = std::vector<Vertex>;

/**
 * @brief Удобный псевдоним для списка треугольников.
 */
using Polygons = std::vector<Triangle>;

/**
 * @enum Response
 * @brief Ответ от модели во View.
 *
 * Используется для управления отображением и определяет, нужно ли
 * перерисовывать или возникла ошибка загрузки.
 */
enum class Response {
  NormalDone,  // нужна перерисовка
  NothingUpdate,  // например, при повороте на 0 или 360 градусов
  BadFile  // нет такого файла или файл не валидный
};

/**
 * @enum UserAction
 * @brief Возможные действия пользователя.
 *
 * Используется контроллером или моделью для определения типа команды.
 */
enum class UserAction {
  FileOpen,      /**< Открытие нового файла. */
  Normalization, /**< Нормализация и центрирование. */
  Translate,     /**< Сдвиг. */
  Scale,         /**< Масштабирование. */
  Rotate,        /**< Поворот по оси. */
  Terminate      /**< Закрытие программы. */
};

}  // namespace s21