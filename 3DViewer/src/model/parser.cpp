/**
 * @file parser.cpp
 * @brief Реализация парсера .obj-файлов.
 *
 * Содержит реализацию разбора строк с вершинами и полигонами, а также
 * валидацию и триангуляцию. Используется в Model для загрузки модели.
 */

#include "parser.h"

namespace s21 {

Parser::Parser() = default;
// Parser::Parser(const std::string &filename) { initParser(filename); }

void Parser::initParser(const std::string filename) {
  std::setlocale(LC_NUMERIC, "C");
  vertices.clear();
  polygons.clear();
  raw_polygons.clear();
  response = Response::BadFile;
  std::ifstream file(filename);
  std::string line;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      parsing(line);
    }
    if (vertices.size() >= 3 && !raw_polygons.empty()) check_validation();
    if (!polygons.empty() || !raw_polygons.empty())
      response = Response::NormalDone;
  }
  std::setlocale(LC_NUMERIC, "");
}

void Parse_vertex::parse_vertex(std::string line, Vertices *vertices) {
  std::istringstream iss(line.substr(1));  // Пропускаем 'v'
  float x = 0, y = 0, z = 0;

  if (iss >> x >> y >> z) {
    Vertex temp_vertex{x, y, z};
    vertices->push_back(temp_vertex);
  } else {
#ifdef DEBUG
    std::cerr << "Невозможно прочитать вершину из строки: " << line << "\n";
#endif
  }
}

void Parse_poligon::parse_poligon(std::string line,
                                  std::vector<std::vector<int>> *raw_polygons) {
  std::vector<int> temp_polygon;
  std::istringstream iss(line);
  std::string token;
  iss >> token;  // пропускаем "f"
  while (iss >> token) {
    size_t slash_pos = token.find('/');
    std::string vertex_index_str;
    if (slash_pos != std::string::npos) {
      // Есть слэш — берем всё до него
      vertex_index_str = token.substr(0, slash_pos);
    } else {
      // Нет слэша — берем всё слово
      vertex_index_str = token;
    }
    // Преобразуем в число
    try {
      int vertex_index = std::stoi(vertex_index_str);
      temp_polygon.push_back(vertex_index);
    } catch (const std::exception &e) {
#ifdef DEBUG
      std::cerr << "Невозможно разобрать индекс вершины из '" << token << "'\n";
#endif
    }
  }
  if (temp_polygon.size() >= 3) {
    raw_polygons->push_back(temp_polygon);
  } else {
#ifdef DEBUG
    std::cerr << "Предупреждение: полигон содержит менее 3 вершин — "
                 "проигнорирован.\n";
#endif
  }
}

void Parser::check_validation() {
  int countVertex = vertices.size();
  polygons.reserve(raw_polygons.size());
  for (int i = raw_polygons.size() - 1; i >= 0; i--) {
    std::vector<int> &temp = raw_polygons[i];
    for (int j = temp.size() - 1; j >= 0; j--) {
      int idx = temp[j];
      if (idx < 0)
        idx = countVertex + idx;
      else
        idx = idx - 1;
      if (idx >= countVertex || idx < 0)
        temp.erase(temp.begin() + j);
      else
        temp[j] = idx;
    }
    if (temp.size() == 3) {
      polygons.push_back({temp[0], temp[1], temp[2]});
      raw_polygons.erase(raw_polygons.begin() + i);
    } else if (temp.size() < 3)
      raw_polygons.erase(raw_polygons.begin() + i);
  }
}

void Parser::parsing(std::string line) {
  if (line.empty() || line[0] == '#') return;
  if ((line[0] == 'v' || line[0] == 'V') && isspace(line[1])) {
    parse_vertex.parse_vertex(line, &vertices);
  } else if ((line[0] == 'f' || line[0] == 'F') && isspace(line[1])) {
    parse_poligon.parse_poligon(line, &raw_polygons);
  }
}
}  // namespace s21
