#ifndef SNAKE_GAME_H
#define SNAKE_GAME_H

#include <QDialog>
#include <QKeyEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <iostream>

#include "../../brick_game/snake/backend_s.hpp"
#include "../cli/tetris/frontend_t.h"
#include "ui_snake_game.h"
#include <ncurses.h>
#include "exit.h"


namespace Ui {
class snake_game;
}

class snake_game : public QDialog
{
    Q_OBJECT

public:
    explicit snake_game(QWidget *parent = nullptr);
    ~snake_game();

private:
    Ui::snake_game *ui;
    s21::game game_main;
    QTimer *timer;

protected:
    void paintEvent(QPaintEvent *) override;
    void update_paint(void);
    void keyPressEvent(QKeyEvent *e) override;

};

#endif // SNAKE_GAME_H
