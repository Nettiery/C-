/**
 * @file model.cpp
 * @brief Реализация класса Model для работы с 3D-моделями (загрузка,
 * трансформации).
 *
 * Использует Parser для чтения моделей, применяет нормализацию и геометрические
 * преобразования. Также реализует повороты через стратегию (паттерн Strategy).
 */

#include "model.h"

#include <memory>

#include "parser.h"
#include "rotate_strategy.h"

namespace s21 {

Model::Model() = default;

void Model::translate(Vertex direction, bool defValue) {
  if (defValue) direction = direction * kMoveStep;
  for (auto &v : vertices) v = v + direction;
}

void Model::scale(float f, bool zoomOut) {
  if (!f) f = kScaleStep;
  if (zoomOut) f = 1 / f;
  for (auto &point : vertices) {
    point = point * f;
  }
}

void Model::normalization() {
  Vertex center = {0.0f, 0.0f, 0.0f};
  float mx = std::numeric_limits<float>::lowest();
  float mn = std::numeric_limits<float>::max();
  Vertex max = {mx, mx, mx};
  Vertex min = {mn, mn, mn};
  for (const auto &v : vertices) {
    max = {std::max(max.x, v.x), std::max(max.y, v.y), std::max(max.z, v.z)};
    min = {std::min(min.x, v.x), std::min(min.y, v.y), std::min(min.z, v.z)};
  }
  center = (max + min) / 2;
  max = max - center;
  min = min - center;
  float absMax = 0.0f;
  absMax = std::max({std::abs(max.x), std::abs(max.y), std::abs(max.z),
                     std::abs(min.x), std::abs(min.y), std::abs(min.z)});
  if (absMax == 0.0f)
    absMax = 1;
  else
    absMax = absMax / kNormalizationScale;
  for (auto &v : vertices) v = (v - center) / absMax;
}

void Model::openModel(const std::string fname) {
  Parser parser;
  parser.initParser(fname);
  response = parser.response;
  vertices = parser.vertices;
  polygons = parser.polygons;
  raw_polygons = parser.raw_polygons;
  if (response != Response::BadFile) {
    normalization();
    if (raw_polygons.size() > 0) triangulation(parser.raw_polygons);
  } else {
    vertices.clear();
    polygons.clear();
    raw_polygons.clear();
  }
}

void Model::triangulation(std::vector<std::vector<int>> raw_polygons) {
  polygons.reserve(polygons.size() + raw_polygons.size() * 2);
  for (auto &rp : raw_polygons) {
    for (size_t i = 0; i < rp.size() - 2; i++) {
      Triangle triangle = {rp[0], rp[i + 1], rp[i + 2]};
      polygons.push_back(triangle);
    }
  }
}

Axis Model::parse_angle(Vertex rotate_param, float *angle) {
  Axis axis = uninitalized;
  if (rotate_param.x != 0) {
    *angle = rotate_param.x;
    axis = X;
  } else if (rotate_param.y != 0) {
    *angle = rotate_param.y;
    axis = Y;
  } else if (rotate_param.z != 0) {
    *angle = rotate_param.z;
    axis = Z;
  }
  return axis;
}

void Model::rotate(Vertex rotate_param, bool defValue) {
  float angle = 0;
  if (defValue) rotate_param = rotate_param * kRotAngle;
  Axis axis = parse_angle(rotate_param, &angle);
  if (axis != uninitalized) {
    std::unique_ptr<Rotate_strategy> strategy;

    switch (axis) {
      case X:
        strategy = std::make_unique<RotateX>(angle);
        break;
      case Y:
        strategy = std::make_unique<RotateY>(angle);
        break;
      case Z:
        strategy = std::make_unique<RotateZ>(angle);
        break;
      default:
        return;
    }
    Apply_rotation_strategy affin_transform(strategy.get());
    affin_transform.apply_rotation(vertices);
  }
}
}  // namespace s21