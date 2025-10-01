#include "exit.h"
#include "ui_exit.h"

EXIT::EXIT(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EXIT)
{
    ui->setupUi(this);
    this->setFixedSize(250, 120);
    this->setWindowTitle("Выход");
}

EXIT::~EXIT()
{
    delete ui;
}



void EXIT::on_YES_clicked()
{
    QCoreApplication::quit();
}


void EXIT::on_NO_clicked()
{
    this->close();
}

