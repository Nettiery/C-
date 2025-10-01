/**
 * @file parser.h
 * @brief Интерфейсы парсинга .obj-файлов.
 *
 * Реализует классы парсинга вершин и полигонов, а также фасад Parser
 * для работы с .obj-файлом в целом. Используется моделью.
 *
 * Используется паттерн «Фасад» для упрощённого взаимодействия с разными
 * типами парсеров.
 */

#pragma once

#include <cstdio>
#include <fstream>
#include <locale>
#include <sstream>
#include <vector>

#include "common.h"

namespace s21 {
/**
 * @class Parse_vertex
 * @brief Класс для парсинга строк, содержащих вершины (v x y z).
 *
 * Используется парсером для извлечения координат вершины из строки и
 * добавления их в общий список вершин.
 */
class Parse_vertex {
 public:
  /**
   * @brief Парсит строку с вершиной и добавляет её в список.
   * @param line Строка, начинающаяся с 'v' и содержащая координаты.
   * @param vertices Указатель на контейнер для добавления результата.
   *
   * Формат строки: "v x y z"
   */
  void parse_vertex(std::string line, Vertices *vertices);
};

/**
 * @class Parse_poligon
 * @brief Класс для парсинга строк, содержащих индексы вершин полигона (f ...).
 *
 * Обрабатывает строки с гранями модели и собирает индексы для последующей
 * валидации и триангуляции.
 */

class Parse_poligon {
 public:
  /**
   * @brief Парсит строку с полигонами и добавляет результат в список.
   * @param line Строка, начинающаяся с 'f' и содержащая индексы вершин.
   * @param raw_polygons Указатель на контейнер для добавления результата.
   *
   * Поддерживает как строки формата "f 1 2 3", так и "f 1/1 2/2 3/3".
   * Игнорирует полигоны с менее чем 3 вершинами.
   */
  void parse_poligon(std::string line,
                     std::vector<std::vector<int>> *raw_polygons);
};

/**
 * @class Parser
 * @brief Класс для парсинга .obj-файлов: извлекает вершины и полигоны.
 *
 * Реализован паттерн **Фасад**.
 * Внутри использует классы Parse_vertex и Parse_poligon для обработки
 * отдельных частей формата.
 *
 * @note Используется паттерн Фасад для упрощения взаимодействия с парсером.
 */
class Parser {
 public:
  Parser();
  // Parser(const std::string &filename);
  /**
   * @brief Распарсенные вершины.
   */
  Vertices vertices;

  /**
   * @brief Триангулированные полигоны.
   */
  Polygons polygons;

  /**
   * @brief Текущий статус парсинга (успешно / ошибка).
   */
  Response response = Response::BadFile;

  /**
   * @brief Исходные полигоны (ещё не разбиты на треугольники).
   */
  std::vector<std::vector<int>> raw_polygons;  // до триангуляции

  /**
   * @brief Выполняет полную инициализацию парсера и загрузку данных.
   * @param filename Имя файла.
   */
  void initParser(const std::string filename);

 private:
  /**
   * @brief Выполняет первичную обработку строки.
   * @param line Строка из .obj-файла.
   *
   * Игнорирует пустые и комментарии. В зависимости от типа вызывает
   * соответствующий парсер.
   */
  void parsing(std::string line);

  /**
   * @brief Объект для разбора вершин.
   */
  Parse_vertex parse_vertex;

  /**
   * @brief Объект для разбора полигонов.
   */
  Parse_poligon parse_poligon;

  /**
   * @brief Проверяет валидность и диапазоны индексов в полигонах.
   *
   * Обрабатывает отрицательные индексы, удаляет некорректные и
   * переносит треугольники в итоговый список.
   */
  void check_validation();
};
}  // namespace s21
