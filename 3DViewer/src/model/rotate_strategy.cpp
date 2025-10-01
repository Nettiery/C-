/**
 * @file rotate_strategy.cpp
 * @brief Реализация стратегий вращения модели вокруг осей X, Y и Z.
 *
 * Используется паттерн **Стратегия** (Strategy): каждая ось реализуется
 * в виде отдельного класса (`RotateX`, `RotateY`, `RotateZ`), а поведение
 * выбирается в рантайме через `Apply_rotation_strategy`.
 */

#include "rotate_strategy.h"

#include <cmath>

namespace s21 {
Rotate_strategy::Rotate_strategy(float angle_deg) {
  float angle_rad = angle_deg * (PI / 180.f);
  cosA = cos(angle_rad);
  sinA = sin(angle_rad);
}

Apply_rotation_strategy::Apply_rotation_strategy(Rotate_strategy *r)
    : rotate_strategy(r) {}

void RotateX::rotation(Vertices &vertices) {
  for (auto &v : vertices) rotate2D(v.y, v.z, false);
}

void RotateY::rotation(Vertices &vertices) {
  for (auto &v : vertices) rotate2D(v.x, v.z, true);
}

void RotateZ::rotation(Vertices &vertices) {
  for (auto &v : vertices) rotate2D(v.x, v.y, false);
}

void Rotate_strategy::rotate2D(float &a, float &b, bool invertSin = false) {
  float sin_ = invertSin ? -sinA : sinA;
  float a_ = a * cosA - b * sin_;
  float b_ = a * sin_ + b * cosA;
  a = a_;
  b = b_;
}

// Using strategy
void Apply_rotation_strategy::apply_rotation(Vertices &vertices) {
  this->rotate_strategy->rotation(vertices);
}

}  // namespace s21