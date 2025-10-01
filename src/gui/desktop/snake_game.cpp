#include "snake_game.h"
#include <ncurses.h>



snake_game::snake_game(QWidget *parent)
    : QDialog(parent), ui(new Ui::snake_game), timer(new QTimer(this)) {
    ui->setupUi(this);
    this->setFixedSize(470, 605);
    this->setWindowTitle("Змейка");
    setFocusPolicy(Qt::StrongFocus);
    connect(timer, &QTimer::timeout, this, &snake_game::update_paint);
    timer->stop();
}

snake_game::~snake_game()
{
    delete ui;
    delete timer;
}

void snake_game::update_paint(void) {
    if (game_main.is_playing) {
        game_main.move();
        timer->stop();
        timer->start(game_main.speed);
        game_main.check_in_game();
    }
    if (!game_main.is_playing) {
        timer->stop();
    }
    repaint();
}


void snake_game::paintEvent(QPaintEvent *) {
    QPainter p;
    p.begin(this);
    p.drawRect(0, 0, 300, 600);
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            p.setBrush(Qt::white);
            p.drawRect(30 * j, 30 * i, 30, 30);
        }
    }
    if (game_main.is_playing) {
        for (size_t i = 0; i < game_main.snake.size(); i++) {
            p.setBrush(Qt::blue);
            p.drawRect(30 * game_main.snake[i].x, 30 * game_main.snake[i].y, 30, 30);
        }
    }

    p.setBrush(Qt::red);
    p.drawRect(30 * game_main.apple.x, 30 * game_main.apple.y, 30, 30);

    QFont font;
    font.setPointSize(16);
    p.setFont(font);
    std::string s = "Score: " + std::to_string(game_main.score);
    p.drawText(305, 35, s.data());
    std::string l = "Level: " + std::to_string(game_main.level);
    p.drawText(305, 60, l.data());
    std::string hs = "High Score: " + std::to_string(game_main.high_score);
    p.drawText(305, 85, hs.data());
    std::string sp = "Speed: " + std::to_string(game_main.speed);
    p.drawText(305, 110, sp.data());

    QPen pen(Qt::blue);
    p.setPen(pen);
    if (game_main.pause) {
        p.drawText(305, 355, "Pause");
        p.drawText(305, 380, "Continue: 'R'");
    }

    if (!game_main.is_playing && game_main.lost == true) {
        p.drawText(305, 355, "YOU LOST");
        p.drawText(305, 380, "EXIT: 'Q'");
    }

    QPen pen3(Qt::green);
    p.setPen(pen3);
    if (!game_main.is_playing && game_main.lost == false) {
        p.drawText(305, 355, "YOU WIN");
        p.drawText(305, 380, "EXIT: 'Q'");
    }

    QPen pen2(Qt::blue);
    p.setPen(pen2);
    std::string controls =
        "LEFT: arroy left\nRIGHT: arroy right\nDOWN: arroy down\nUP: arroy up\nFASTER: 'SPACE'\nPAUSE: "
        "'R'\nQUIT: 'Q'\nSTART: R";
    QRect controlRect(305, 155, 160, 150);
    p.drawText(controlRect, Qt::AlignLeft | Qt::TextWordWrap, controls.c_str());
    p.end();
}

void snake_game::keyPressEvent(QKeyEvent *e) {
    if (game_main.is_playing) {
        if (e->key() == Qt::Key_R) {
            if (game_main.pause) {
                timer->start(game_main.speed);
                game_main.pause = !game_main.pause;
            } else {
                timer->stop();
                game_main.pause = !game_main.pause;
            }
        }
    }
    if (!game_main.pause) {
        if (e->key() == Qt::Key_Space) {
            game_main.move();
            timer->stop();
            timer->start(game_main.speed);
            game_main.check_in_game();
            if (game_main.is_playing)repaint();
        } else if (e->key() == Qt::Key_Up) {
            game_main.direction = s21::Up_dir;
            game_main.check_in_game();
            if (game_main.is_playing)repaint();
        } else if (e->key() == Qt::Key_Left) {
            game_main.direction = s21::Left_dir;
            game_main.check_in_game();
            if (game_main.is_playing)repaint();
        } else if (e->key() == Qt::Key_Right) {
            game_main.direction = s21::Right_dir;
            game_main.check_in_game();
            if (game_main.is_playing)repaint();
        } else if (e->key() == Qt::Key_Down) {
            game_main.direction = s21::Down_dir;
            game_main.check_in_game();
            if (game_main.is_playing)repaint();
        } else if (e->key() == Qt::Key_Q) {
            timer->stop();
            EXIT w;
            w.setModal(true);
            w.exec();
            timer->start(game_main.speed);
        }
    }
}



