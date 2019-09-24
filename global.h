#ifndef GLOBAL_H
#define GLOBAL_H
#include <QString>
#include <QObject>


static const int ROW = 10;
static const int COLUMN = 9;

static const int RED = 233;
static const int BLACK = 2333;

static const int INITIAL[95] = {
      2,  3,  6,  5,  1,  5,  6,  3,  2,
      0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  4,  0,  0,  0,  0,  0,  4,  0,
      7,  0,  7,  0,  7,  0,  7,  0,  7,
      0,  0,  0,  0,  0,  0,  0,  0,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,
     14,  0, 14,  0, 14,  0, 14,  0, 14,
      0, 11,  0,  0,  0,  0,  0, 11,  0,
      0,  0,  0,  0,  0,  0,  0,  0,  0,
      9, 10, 13,  12,  8, 12, 13, 10, 9
};

//棋子编号
static const int BLACK_GENERAL = 1;     //黑将
static const int BLACK_CHARIOT = 2;     //黑车
static const int BLACK_HORSE = 3;       //黑马
static const int BLACK_CANNON = 4;      //黑炮
static const int BLACK_ADVISOR = 5;     //黑士
static const int BLACK_MINISTER = 6;    //黑象
static const int BLACK_SOLDIER = 7;     //黑卒

static const int RED_GENERAL = 8;       //红帅
static const int RED_CHARIOT = 9;       //红车
static const int RED_HORSE = 10;        //红马
static const int RED_CANNON = 11;       //红炮
static const int RED_ADVISOR = 12;      //红仕
static const int RED_MINISTER = 13;     //红相
static const int RED_SOLDIER = 14;      //红兵

static const int SIZE = 57;
static const int BOARDER = 4;
static const int DEPARTY = 120;
static const int DEPARTX = 10;

inline QString getPic(int chessman, bool selected, bool dead)
{
    QString Pic;
    switch(chessman)
    {
    case 0:
        Pic = selected ? QObject::tr(":/images/images/WOOD/OOS.GIF"): QObject::tr(":/images/images/WOOD/OO.GIF");
        break;
    case 1:
        Pic = dead ? QObject::tr(":/images/images/WOOD/BKM.GIF") : (selected ? QObject::tr(":/images/images/WOOD/BKS.GIF"): QObject::tr(":/images/images/WOOD/BK.GIF"));
        break;
    case 2:
        Pic = selected ? QObject::tr(":/images/images/WOOD/BRS.GIF"): QObject::tr(":/images/images/WOOD/BR.GIF");
        break;
    case 3:
        Pic = selected ? QObject::tr(":/images/images/WOOD/BNS.GIF"): QObject::tr(":/images/images/WOOD/BN.GIF");
        break;
    case 4:
        Pic = selected ? QObject::tr(":/images/images/WOOD/BCS.GIF"): QObject::tr(":/images/images/WOOD/BC.GIF");
        break;
    case 5:
        Pic = selected ? QObject::tr(":/images/images/WOOD/BAS.GIF"): QObject::tr(":/images/images/WOOD/BA.GIF");
        break;
    case 6:
        Pic = selected ? QObject::tr(":/images/images/WOOD/BBS.GIF"): QObject::tr(":/images/images/WOOD/BB.GIF");
        break;
    case 7:
        Pic = selected ? QObject::tr(":/images/images/WOOD/BPS.GIF"): QObject::tr(":/images/images/WOOD/BP.GIF");
        break;
    case 8:
        Pic = dead ? QObject::tr(":/images/images/WOOD/RKM.GIF") : (selected ? QObject::tr(":/images/images/WOOD/RKS.GIF"): QObject::tr(":/images/images/WOOD/RK.GIF"));
        break;
    case 9:
        Pic = selected ? QObject::tr(":/images/images/WOOD/RRS.GIF"): QObject::tr(":/images/images/WOOD/RR.GIF");
        break;
    case 10:
        Pic = selected ? QObject::tr(":/images/images/WOOD/RNS.GIF"): QObject::tr(":/images/images/WOOD/RN.GIF");
        break;
    case 11:
        Pic = selected ? QObject::tr(":/images/images/WOOD/RCS.GIF"): QObject::tr(":/images/images/WOOD/RC.GIF");
        break;
    case 12:
        Pic = selected ? QObject::tr(":/images/images/WOOD/RAS.GIF"): QObject::tr(":/images/images/WOOD/RA.GIF");
        break;
    case 13:
        Pic = selected ? QObject::tr(":/images/images/WOOD/RBS.GIF"): QObject::tr(":/images/images/WOOD/RB.GIF");
        break;
    case 14:
        Pic = selected ? QObject::tr(":/images/images/WOOD/RPS.GIF"): QObject::tr(":/images/images/WOOD/RP.GIF");
        break;
    default:
        break;
    }
    return Pic;
}

inline void getPixmapPos(int row, int column, int &xx, int &yy)
{
    xx = BOARDER + column * SIZE;
    yy = BOARDER + row * SIZE;
}

inline void getPixmapIndex(int xx, int yy, int &row, int &column)
{
    row = (yy - BOARDER-DEPARTY) / SIZE;
    column = (xx - BOARDER-DEPARTX) / SIZE;
}

#endif // GLOBAL_H
