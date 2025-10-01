/**
 * @file commands.h
 * @brief Реализация паттерна Команда для управления моделью.
 */

#pragma once

#include "../model/model.h"

namespace s21 {

/**
 * @interface ICommand
 * @brief Интерфейс команды, реализующий паттерн Command.
 *
 * Каждая команда инкапсулирует одно действие, которое можно выполнить над
 * моделью. Это позволяет отделить инициатора команды (Controller) от её
 * получателя (Model).
 */
class ICommand {
 public:
  virtual ~ICommand() = default;

  /**
   * @brief Выполняет операцию над моделью.
   * @param model Ссылка на модель, к которой применяется команда.
   */
  virtual void execute(Model &model) = 0;
};

// ==================== Конкретные команды ====================
/**
 * @class OpenFileCommand
 * @brief Команда открытия .obj-файла.
 */
class OpenFileCommand : public ICommand {
 public:
  explicit OpenFileCommand(std::string fname) : filename_(std::move(fname)) {}
  void execute(Model &model) override { model.openModel(filename_); }

 private:
  std::string filename_;
};

/**
 * @class NormalizeCommand
 * @brief Команда нормализации и центрирования модели.
 */
class NormalizeCommand : public ICommand {
 public:
  void execute(Model &model) override { model.normalization(); }
};

/**
 * @class ScaleCommand
 * @brief Команда масштабирования модели.
 */
class ScaleCommand : public ICommand {
 public:
  /**
   * @brief Конструктор.
   * @param factor Множитель масштабирования.
   * @param zoomOut Показатель уменьшения (factor=1/factor).
   */
  ScaleCommand(float factor, bool zoomOut) : factor_(factor), def_(zoomOut) {}
  void execute(Model &model) override { model.scale(factor_, def_); }

 private:
  float factor_;
  bool def_;
};

/**
 * @class TranslateCommand
 * @brief Команда сдвига модели.
 */
class TranslateCommand : public ICommand {
 public:
  /**
   * @brief Конструктор.
   * @param v Вектор смещения.
   * @param useDefault  Используется значение по умолчанию - 0.2.
   */
  TranslateCommand(Vertex v, bool useDefault) : vertex_(v), def_(useDefault) {}
  void execute(Model &model) override { model.translate(vertex_, def_); }

 private:
  Vertex vertex_;
  bool def_;
};

/**
 * @class RotateCommand
 * @brief Команда вращения модели.
 */
class RotateCommand : public ICommand {
 public:
  /**
   * @brief Конструктор.
   * @param v Углы вращения по каждой оси.
   * @param useDefault Используется значение по умолчанию - 15 градусов.
   */
  RotateCommand(Vertex v, bool useDefault) : vertex_(v), def_(useDefault) {}
  void execute(Model &model) override { model.rotate(vertex_, def_); }

 private:
  Vertex vertex_;
  bool def_;
};

}  // namespace s21
