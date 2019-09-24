#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QFile>
#include <QMediaPlayer>
#include <QUrl>
#include "global.h"
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QVector>
#include <QTextStream>

namespace Ui {
class Board;
}

class Board : public QMainWindow
{
    Q_OBJECT

public:
    explicit Board(QWidget *parent = 0);
    ~Board();
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void loadPixmap(int *chessman);

    int getturn();
    int* getMap();
    int Map[ROW*COLUMN+5];

    void setBlack();
    void setRed();

private slots:
    void refreshBoard();
    void otherprepare();
    void timeoutslot();
    void showtimewin();
    void showgiveupwin();
    void showwinbyeat();
    void showlosebyeat();
    void getchangeturn();
    void on_actiongive_up_triggered();

    void on_actionnew_game_triggered();

    void on_actionsave_triggered();

    void on_actionimport_triggered();

    void on_actionbegin_triggered();

signals:
    void changeboard();
    void youwinfortime();
    void youwinforgiveup();
    void winbyeat();
    void losebyeat();
    void haveprepare();
    void sendchangeturn();

private:
    Ui::Board *ui;
    QPixmap chess[ROW*COLUMN+5];   //the pixmap of each chess
    QPixmap background;            //the board
    bool selected[ROW*COLUMN+5];
    bool dead;
    bool legal;
    bool redturn;
    bool blackturn;
    bool havechosen;
    bool self_prepare;
    bool other_prepare;
    bool begin;
    int redgeneralpos;
    int blackgeneralpos;
    int side;
    int selectednumber;
    int selectedindex;
    int index;
    int lefttime;
    QTimer* timer;

    QVector<int> GetChessPos(int index);
    QString transformstring(int index);
    void solvetxt(int index,QString str);
    void preparegame();
    void begingame();
    void initial();
    void winmusic();
    void losemusic();
    bool judgeencounter();
    void judgechorit();
    void judgecannon();
    void judgehorse();
    void judgeminister();
    void judgeadvisor();
    void judgegeneral();
    void judgesoldier();
    void judge();
    void judgewin();
    void reverse();
    bool attackgeneral();
    bool attackgeneral(int chess);
    void generalpos();

};

#endif // BOARD_H
