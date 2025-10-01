#include "test.h"
namespace s21 {
int main(int argc, char **argv) {
  try {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

  } catch (const std::exception &e) {  // Ловим все стандартные исключения
    std::cerr << "Ошибка: " << e.what() << std::endl;
  }

  return 0;
}

bool verticesEq(Vertices v1, Vertices v2) {
  bool result = true;
  if (v1.size() != v2.size()) result = false;
  for (size_t i = 0; i < v1.size() && result; ++i) {
    if (v1[i] != v2[i]) result = false;
  }
  return result;
}

void printVertices(Vertices vertices) {
  for (const auto &v : vertices) std::cout << v << std::endl;
}
void printPolygons(std::vector<Triangle> polygons) {
  for (const auto &p : polygons) std::cout << p << std::endl;
}
void printRawPolygons(std::vector<std::vector<int>> raw_polygons) {
  for (const auto &rp : raw_polygons) {
    for (const auto &idx : rp) std::cout << idx << " ";
    std::cout << std::endl;
  }
}
}  // namespace s21