/**
 * @file main.cpp
 * @brief The entry point of the application.
 */

#include <QApplication>

#include "mainwindow.h"

/**
 * @brief The main function of the application.
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return The exit code of the application.
 */
int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  s21::MainWindow w;
  w.show();
  return app.exec();
}
