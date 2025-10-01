#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(350, 200);
    this->setWindowTitle("Выберите игру");
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_snake_clicked()
{
    close();
    snake_game w;
    w.setModal(true);
    w.exec();
}


void MainWindow::on_tetris_clicked()
{
    close();
    tetris_game w;
    w.setModal(true);
    w.exec();

}

