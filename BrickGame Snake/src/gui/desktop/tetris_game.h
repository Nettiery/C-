
#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include <QDialog>

#include <QKeyEvent>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <iostream>

#include "../../brick_game/tetris/backend_t.h"
#include "../cli/tetris/frontend_t.h"
#include <ncurses.h>
#include "ui_tetris_game.h"
#include "exit.h"


namespace Ui {
class tetris_game;
}

class tetris_game : public QDialog
{
    Q_OBJECT


public:
    explicit tetris_game(QWidget *parent = nullptr);
    ~tetris_game();

private:
    Ui::tetris_game *ui;
    GameState_t gs;
    QTimer *timer;

protected:
           void paintEvent(QPaintEvent *) override;
           void update_paint(void);
           void keyPressEvent(QKeyEvent *e) override;
};

#endif // TETRIS_H
