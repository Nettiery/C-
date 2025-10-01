/**
 * @file model.h
 * @brief Заголовочный файл для класса Model, реализующего работу с 3D-моделью.
 */

#pragma once

#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include "common.h"

namespace s21 {

/**
 * @class Model
 * @brief Класс для управления 3D-моделью: загрузка, трансформация,
 * нормализация.
 */
class Model {
 public:
  Model();

  /**
   * @brief Вершины 3D-модели.
   */
  Vertices vertices;

  /**
   * @brief Статус обработки модели (успешно, ошибка и т.п.).
   */
  Response response = Response::NormalDone;

  /**
   * @brief Полигоны модели после триангуляции.
   */
  Polygons polygons;

  /**
   * @brief Исходные (сырые) полигоны модели до триангуляции.
   */
  std::vector<std::vector<int>> raw_polygons;

 private:
  /**
   * @brief Коэффициент масштабирования при нормализации.
   */
  static constexpr float kNormalizationScale = 0.9f;

  /**
   * @brief Шаг масштабирования модели.
   */
  static constexpr float kScaleStep = 1.3f;

  /**
   * @brief Шаг перемещения модели.
   */
  static constexpr float kMoveStep = 0.2f;

  /**
   * @brief Угол поворота по умолчанию (в градусах).
   */
  static constexpr float kRotAngle = 10.0f;

 public:
  /**
   * @brief Перемещает модель на заданный вектор.
   * @param direction Вектор смещения.
   * @param defValue Если true — direction умножается на фиксированную
   * величину.
   */
  void translate(Vertex direction, bool defValue);

  /**
   * @brief Масштабирует модель.
   * @param f Коэффициент масштабирования.
   * @param zoomOut Если true — используется обратный коэффициент (1 / f).
   */
  void scale(float f, bool zoomOut);

  /**
   * @brief Определяет ось вращения и угол.
   * @param rotate_param Вектор с ненулевым компонентом (ось + угол).
   * @param angle Указатель, куда записывается значение угла.
   * @return Ось вращения.
   */
  Axis parse_angle(Vertex rotate_param, float *angle);

  /**
   * @brief Выполняет поворот модели.
   * @param rotate_param Параметр поворота (вектор с углом по оси).
   * @param defValue Если true — масштабирует угол на фиксированную величину.
   */
  void rotate(Vertex rotate_param, bool defValue);

  /**
   * @brief Центрирует и нормализует размер модели.
   */
  void normalization();

  /**
   * @brief Загружает модель из файла.
   * @param fname Путь к файлу.
   */
  void openModel(const std::string fname);

 private:
  /**
   * @brief Выполняет триангуляцию многоугольников модели.
   * @param raw_polygons Массив полигонов (списков индексов вершин).
   */
  void triangulation(std::vector<std::vector<int>> raw_polygons);
};

}  // namespace s21