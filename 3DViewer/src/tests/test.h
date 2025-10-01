#pragma once
#include <gtest/gtest.h>

#include <iostream>
#include <queue>
#include <stack>

#include "../controller/controller.h"
// #include "../model/model.h"
#include "../model/parser.h"
#include "../model/rotate_strategy.h"

#define TOL 1e-6  // Точность сравнения
namespace s21 {
bool verticesEq(Vertices v1, Vertices v2);
void printVertices(Vertices vertices);
void printPolygons(std::vector<Triangle> polygons);
void printRawPolygons(std::vector<std::vector<int>> raw_polygons);
}  // namespace s21