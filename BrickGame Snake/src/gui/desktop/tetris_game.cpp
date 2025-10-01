#include "tetris_game.h"


tetris_game::tetris_game(QWidget *parent)
    : QDialog(parent), ui(new Ui::tetris_game), timer(new QTimer(this)) {
    ui->setupUi(this);
    this->setFixedSize(470, 605);
    this->setWindowTitle("Тетрис");
    setFocusPolicy(Qt::StrongFocus);
    // GameState_t gs;
    gs.x = START_X;
    gs.y = START_Y;
    gs.status = Moving;
    gs.score = 0;
    gs.level = 1;
    gs.speed = 1000;
    gs.pause = true;
    gs.is_playing = true;
    gs.win = false;
    gs.time = get_time();
    gs.field = create_matrix(FIELD_N, FIELD_M);
    srand(get_time());
    int size;
    gs.next_figure = generate_figure(&size);
    gs.next_figure_size = size;

    gs.figure = create_matrix(size, size);
    gs.figure_size = size;
    copy_matrix(size, size, gs.next_figure, gs.figure);

    FILE *file = fopen("high_score.txt", "r");
    if (file) {
        fscanf(file, "%d", &gs.high_score);
        fclose(file);
    }
    connect(timer, &QTimer::timeout, this, &tetris_game::update_paint);
    timer->stop();
}

tetris_game::~tetris_game() {
    delete ui;
    delete timer;
    finish_game(&gs);
}

void tetris_game::update_paint(void) {
    if (gs.is_playing){
         if (gs.status == Spawn) {
            spawn_figure(&gs);
        }   else
            if (gs.status == Attaching) {
                attach_figure(&gs);
            repaint();
            }
        else
            if (gs.status == GameOver) {
            gs.is_playing=false;
            timer->stop();
            timer->start(gs.speed);
            }
            else
            move_figure(&gs, Down);
        timer->stop();
        timer->start(gs.speed);
    }
    if (!gs.is_playing){
        timer->stop();
    }
    repaint();
}

void tetris_game::paintEvent(QPaintEvent *) {

    QPainter p;
    p.begin(this);
    p.drawRect(0, 0, 300, 600);

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (gs.field[i][j] == 1)
                p.setBrush(Qt::black);
            else
                p.setBrush(Qt::white);
            p.drawRect(30 * j, 30 * i, 30, 30);
        }
    }

    for (int i = 0; i < gs.figure_size; i++) {
        for (int j = 0; j < gs.figure_size; j++) {
            int x = gs.x + j;
            int y = gs.y + i;
            if (y > -1 && y < FIELD_N && x > -1 &&
                x < FIELD_M){
            if (gs.figure[i][j] == 1)
                p.setBrush(Qt::black);
            else{
                if (gs.field[y][x]!=1)
                     p.setBrush(Qt::white);
                else if (gs.field[y][x]==1)
                    p.setBrush(Qt::black);
            }
            p.drawRect(30 * x, 30 * y, 30, 30);
            }
        }
    }


    for (int i = 0; i < gs.next_figure_size; i++) {
        for (int j = 0; j < gs.next_figure_size; j++) {
                if (gs.next_figure[i][j]) {
                    p.setBrush(Qt::black);
                } else {
                    p.setBrush(Qt::white);
                }
            p.drawRect(30 * j + 310, 30 * i + 30, 30, 30);
        }
    }

    QFont font;
    font.setPointSize(16);
    p.setFont(font);
    p.drawText(305, 25, "Next step:");
    std::string s = "Score: " + std::to_string(gs.score);
    p.drawText(305, 205, s.data());
    std::string l = "Level: " + std::to_string(gs.level);
    p.drawText(305, 230, l.data());
    std::string hs = "Max points: " + std::to_string(gs.high_score);
    p.drawText(305, 255, hs.data());
    std::string sp = "Speed: " + std::to_string(gs.speed);
    p.drawText(305, 280, sp.data());
    QPen pen(Qt::red);

    p.setPen(pen);
    if (gs.pause) {
        p.drawText(305, 525, "Pause");
        p.drawText(305, 550, "Continue: 'R'");
    }

    if (!gs.is_playing) {
        p.drawText(305, 525, "YOU LOST");
        p.drawText(305, 550, "EXIT: 'Q'");
    }

    QPen pen2(Qt::blue);
    p.setPen(pen2);
    std::string controls =
        "LEFT: arroy left\nRIGHT: arroy right\nDOWN: arroy down\nUP: arroy up\nFASTER: 'SPACE'\nPAUSE: "
        "'R'\nQUIT: 'Q'\nSTART: R";
    QRect controlRect(305, 325, 160, 150);
    p.drawText(controlRect, Qt::AlignLeft | Qt::TextWordWrap, controls.c_str());
    p.end();
}


void tetris_game::keyPressEvent(QKeyEvent *e) {
    if (gs.is_playing) {
        if (e->key() == Qt::Key_R) {
            if (gs.pause) {
                timer->start(gs.speed);
                gs.pause = !gs.pause;
            } else {
                timer->stop();
                gs.pause = !gs.pause;
            }
        }
    }
    UserAction_t action =   Uninitialized;
    if (e->key() == Qt::Key_R) {
        action = Start;
    } else if (e->key() == Qt::Key_P)
        action = Pause;
    else if (e->key() == Qt::Key_Q)
        action = Terminate;
    else if (e->key() == Qt::Key_Left)
    {
        action = Left;
        if (gs.status!=GameOver) gs.status = Moving;
    }
    else if (e->key() == Qt::Key_Right)
    {
        action = Right;
        if (gs.status!=GameOver) gs.status = Moving;
    }
    else if (e->key()  == Qt::Key_Up)
        action = Up;
    else if (e->key()  == Qt::Key_Down)
    {action = Down;
        if (gs.status!=GameOver) gs.status = Moving;
    }
    else if (e->key()  == Qt::Key_Space)
        action = Action;

    if (action == Terminate){
      if (gs.status==GameOver){
        gs.is_playing = false;
        timer->stop();
        timer->start(gs.speed);
      } else {
        gs.is_playing = false;
        // timer->stop();
        // EXIT w;
        // w.setModal(true);
        // w.exec();
        // timer->start(gs.speed);
      }
    }

    if (gs.status == Spawn) {
        if (action == Terminate || gs.win)
          gs.is_playing = false;
        else {
            spawn_figure(&gs);
        }
    }
    else
    if (gs.status == Moving) {
        move_figure(&gs, action);}
    else if (gs.status == Shifting) {
        move_down(&gs);
    } else if (gs.status == Attaching) {
        attach_figure(&gs);
        repaint();
    }
    if (gs.status == GameOver) {
        timer->stop();
        timer->start(gs.speed);
    }
       repaint();

}

