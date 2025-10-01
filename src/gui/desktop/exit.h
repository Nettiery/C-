#ifndef EXIT_H
#define EXIT_H

#include <QDialog>
#include "tetris_game.h"

namespace Ui {
class EXIT;
}

class EXIT : public QDialog
{
    Q_OBJECT

public:
    explicit EXIT(QWidget *parent = nullptr);
    ~EXIT();

private slots:

    void on_YES_clicked();

    void on_NO_clicked();

private:
    Ui::EXIT *ui;
};

#endif // EXIT_H
