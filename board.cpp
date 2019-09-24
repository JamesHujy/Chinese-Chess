#include "board.h"
#include "ui_board.h"


Board::Board(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Board)
{
    ui->setupUi(this);
    QFont font;
    font.setPointSize(14);
    ui->label_2->setFont(font);
    ui->lcdNumber->display(lefttime);
    ui->lcdNumber->hide();
    ui->label_2->hide();
    timer = new QTimer;
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(timeoutslot()));

    initial();
    self_prepare = false;
    other_prepare = false;
    for(int i = 0;i<ROW*COLUMN+5;i++)
        selected[i] = false;
    dead = false;
    for(int i = 0;i<ROW*COLUMN+5;i++)
        Map[i] = INITIAL[i];
    selectednumber = 0;
    selectedindex = 0;
    legal = true;
    havechosen = false;
    blackturn = false;
    redturn = true;
    connect(this,SIGNAL(winbyeat()),this,SLOT(showwinbyeat()));
    connect(this,SIGNAL(losebyeat()),this,SLOT(showlosebyeat()));
}

Board::~Board()
{
    delete ui;
}

void Board::showwinbyeat()
{
    timer->stop();
    winmusic();
    initial();
    QMessageBox::about(nullptr,tr("胜利"),tr("恭喜你赢得了比赛！"));
}

void Board::showlosebyeat()
{
    timer->stop();
    losemusic();
    initial();
    QMessageBox::about(nullptr,tr("失败"),tr("很遗憾你输掉了比赛，继续努力～"));
}

void Board::showtimewin()
{
    timer->stop();
    winmusic();
    initial();
    QMessageBox::about(nullptr,tr("胜利"),tr("对手已超时，恭喜你赢得了比赛！"));
}

void Board::showgiveupwin()
{
    timer->stop();
    winmusic();
    initial();
    QMessageBox::about(nullptr,tr("胜利"),tr("对手已认输，恭喜你赢得了比赛！"));
}

void Board::winmusic()
{
    QMediaPlayer* player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/Win.wav"));
    player->setVolume(30);
    player->play();
}

void Board::losemusic()
{
    QMediaPlayer* player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/Loss.wav"));
    player->setVolume(30);
    player->play();
}

void Board::timeoutslot()
{
    lefttime--;
    ui->lcdNumber->display(lefttime);
    if(lefttime==0)
    {
        timer->stop();
        losemusic();
        initial();
        QMessageBox::about(nullptr,tr("超时"),tr("您已超时。很遗憾，您输了"));
        emit youwinfortime();
    }
}

int Board::getturn()
{
    if(redturn) return 0;
    else return 1;
}

int* Board::getMap(){return Map;}

void Board::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    int xx = 0;
    int yy = 0;
    int index = 0;

    QPainter p(this);
    p.translate(DEPARTX,DEPARTY);
    p.drawPixmap(0,0,background);
    loadPixmap(Map);
    for(int row = 0; row < ROW; ++row)
        for (int column = 0; column < COLUMN; ++column)
        {
            index = row * COLUMN + column;
            getPixmapPos(row, column, xx, yy);
            p.drawPixmap(xx, yy ,chess[index]);
        }
    if(!begin)
    {
        if(side==RED)
        {
            p.setPen(Qt::transparent);
            p.setBrush(Qt::black);
            p.drawEllipse(QPoint(width()-90,height()/5),20,20);
            p.setBrush(Qt::red);
            p.drawEllipse(QPoint(width()-90,height()/3*2),20,20);
            p.setPen(Qt::black);
            p.setBrush(Qt::black);
            p.drawText(QPoint(width()-60,height()/5+5),tr("黑方"));
            p.setBrush(Qt::black);
            p.drawText(QPoint(width()-60,height()/3*2+5),tr("红方"));
        }
        else if(side==BLACK)
        {
            p.setPen(Qt::transparent);
            p.setBrush(Qt::black);
            p.drawEllipse(QPoint(width()-90,height()/3*2),20,20);
            p.setBrush(Qt::red);
            p.drawEllipse(QPoint(width()-90,height()/5),20,20);
            p.setPen(Qt::black);
            p.setBrush(Qt::black);
            p.drawText(QPoint(width()-60,height()/3*2+5),tr("黑方"));
            p.setBrush(Qt::black);
            p.drawText(QPoint(width()-60,height()/5+5),tr("红方"));
        }
        if(!self_prepare&&!other_prepare)
        {
            QFont font("Microsoft Yahei",15,QFont::Bold,true);
            p.setFont(font);
            p.drawText(QPoint(width()-90,height()/3*2+35),tr("等待准备"));
            p.drawText(QPoint(width()-90,height()/5+35),tr("等待准备"));
        }
        if(self_prepare&&!other_prepare)
        {
            QFont font("Microsoft Yahei",15,QFont::Bold,true);
            p.setFont(font);
            p.drawText(QPoint(width()-90,height()/3*2+35),tr("已准备"));
            p.drawText(QPoint(width()-90,height()/5+35),tr("等待准备"));
        }
        if(!self_prepare&&other_prepare)
        {
            QFont font("Microsoft Yahei",15,QFont::Bold,true);
            p.setFont(font);
            p.drawText(QPoint(width()-90,height()/5+35),tr("已准备"));
            p.drawText(QPoint(width()-90,height()/3*2+35),tr("等待准备"));
        }
    }
    if(begin)
    {
        bool myturn = (side==RED&&redturn)||(side==BLACK&&blackturn);
        if(myturn)
        {
            p.setPen(Qt::transparent);
            if(side==RED)p.setBrush(Qt::red);
            else p.setBrush(Qt::black);
            p.drawEllipse(QPoint(width()-70,height()/3*2),20,20);
        }
        else
        {
            p.setPen(Qt::transparent);
            if(side==RED)p.setBrush(Qt::black);
            else p.setBrush(Qt::red);
            p.drawEllipse(QPoint(width()-70,height()/5),20,20);
        }
    }
}

void Board::loadPixmap(int *chessman)
{
    char pc = 0;
    int index = 0;

    background.load(":/images/images/WOOD.GIF");
    for(int row = 0; row < ROW; ++row)
    {
        for (int column = 0; column < COLUMN; ++column)
        {
            index = row * COLUMN + column;
            pc = chessman[index];
            chess[index].load(getPic(pc, selected[index], dead));
        }
    }
}

void Board::mousePressEvent(QMouseEvent* event)
{

    int x = event->pos().x();
    int y = event->pos().y();

    if (event->button() == Qt::LeftButton&&begin)
    {
        int row = 0;
        int column = 0;
        getPixmapIndex(x, y, row, column);
        index = row * COLUMN + column;
        for(int i = 0;i<ROW*COLUMN+5;i++)
            selected[i] = false;
        selected[index] = true;
        //different
        bool tempjudge1 = Map[index]>0&&Map[index]<=7&&selectednumber>=8&&selectednumber<15;
        bool tempjudge2 = selectednumber>0&&selectednumber<=7&&Map[index]>=8&&Map[index]<15;
        //same color
        bool tempjudge3 = Map[index]>0&&Map[index]<=7&&selectednumber<=7&&selectednumber>0;
        bool tempjudge4 = Map[index]<15&&Map[index]>7&&selectednumber>7&&selectednumber<15;

        if(!havechosen&&Map[index]==0) {update();return;}
        if((side==BLACK&&redturn)||(side==RED&&blackturn)) {update();return;}
        if(!havechosen)
        {
            if((side==BLACK||blackturn)&&Map[index]>7)
            {
                QMediaPlayer* player = new QMediaPlayer(this);
                player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/Forbid.wav"));
                player->setVolume(30);
                player->play();
                return;
            }
            if((side==RED||redturn)&&Map[index]<=7)
            {
                QMediaPlayer* player = new QMediaPlayer(this);
                player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/Forbid.wav"));
                player->setVolume(30);
                player->play();
                return;
            }
        }
        if(Map[index]>0&&selectednumber==0)
        {
            selectednumber = Map[index];
            selectedindex = index;
            havechosen = true;
            update();
            return;
        }

        if(Map[index]>0&&(tempjudge3||tempjudge4))
        {
            selectednumber = Map[index];
            selectedindex = index;
            havechosen = true;
            update();
            return;
        }

        judge();

        if(legal)
        {
            bool judge_1 = Map[index]>0&&(tempjudge1||tempjudge2);
            bool judge_2 = Map[index]==0;
            if(judge_1||judge_2)
            {
                if(selectednumber>0)
                {
                    Map[selectedindex] = 0;
                    Map[index] = selectednumber;
                    selectednumber = 0;
                    selected[index] = false;

                    blackturn = !blackturn;
                    redturn = !redturn;

                    havechosen = false;
                    if(judge_1)
                    {
                        QMediaPlayer* player = new QMediaPlayer(this);
                        player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/Eat.wav"));
                        player->setVolume(30);
                        player->play();
                    }
                    if(judge_2)
                    {
                        QMediaPlayer* player = new QMediaPlayer(this);
                        player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/Move.wav"));
                        player->setVolume(30);
                        player->play();
                    }
                    if(attackgeneral())
                    {
                        QMediaPlayer* player = new QMediaPlayer(this);
                        player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/AttackKing.wav"));
                        player->setVolume(30);
                        player->play();
                    }
                }
            }
            update();
            timer->stop();
            lefttime = 20;
            ui->lcdNumber->display(lefttime);
            judgewin();
            emit changeboard();
        }
        else
        {
            QMediaPlayer* player = new QMediaPlayer(this);
            player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/Forbid.wav"));
            player->setVolume(30);
            player->play();
        }
    }
}

void Board::judge()
{
    switch (Map[selectedindex]) {
    case 1:
    case 8:
        judgegeneral();
        break;
    case 2:
    case 9:
        judgechorit();
        break;
    case 3:
    case 10:
        judgehorse();
        break;
    case 4:
    case 11:
        judgecannon();
        break;
    case 5:
    case 12:
        judgeadvisor();
        break;
    case 6:
    case 13:
        judgeminister();
        break;
    case 7:
    case 14:
        judgesoldier();
        break;
    }
}

void Board::judgechorit()
{
    if(selectednumber==0) {legal=true;return;}
    if(index/COLUMN==selectedindex/COLUMN)
    {
        if(index>selectedindex)
        {
            for(int i = selectedindex+1;i<index;i++)
            {
                if(Map[i]>0){legal = false;return;}
            }
            legal = true;
            return;
        }
        else
        {
            for(int i = index+1;i<selectedindex;i++)
            {
                if(Map[i]>0){legal = false;return;}
            }
            legal = true;
            return;
        }
    }
    else if(index/COLUMN!=selectedindex/COLUMN&&abs(index-selectedindex)%COLUMN==0)
    {
        if(index>selectedindex)
        {
            for(int i = selectedindex+COLUMN;i<index;i+=COLUMN)
            {
                if(Map[i]>0){legal = false;return;}
            }
            legal = true;
            return;
        }
        else
        {
            for(int i = index+COLUMN;i<selectedindex;i+=COLUMN)
            {
                if(Map[i]>0){legal = false;return;}
            }
            legal = true;
            return;
        }
    }
    legal = false;
}

void Board::judgecannon()
{
    int num = 0;
    bool tempjudge1 = Map[index]>0&&Map[index]<=7&&selectednumber>=8&&selectednumber<15;
    bool tempjudge2 = selectednumber>0&&selectednumber<=7&&Map[index]>=8&&Map[index]<15;
    if(selectednumber==0) {legal=true;return;}
    if(index/COLUMN==selectedindex/COLUMN)
    {
        if(index>selectedindex)
        {
            for(int i = selectedindex+1;i<index;i++)
                if(Map[i]>0)
                    num++;
            if(num>1) {legal = false;return;}
            else if(num==1)
            {
                if(Map[index]>0&&(tempjudge1||tempjudge2)){legal = true;return;}
                else {legal = false;return;}
            }
            else
            {
                if(Map[index]>0){legal = false;return;}
                else {legal = true;return;}
            }
        }
        else
        {
            for(int i = index+1;i<selectedindex;i++)
                if(Map[i]>0)
                    num++;
            if(num>1) {legal = false;return;}
            else if(num==1)
            {
                if(Map[index]>0&&(tempjudge1||tempjudge2)){legal = true;return;}
                else {legal = false;return;}
            }
            else
            {
                if(Map[index]>0){legal = false;return;}
                else {legal = true;return;}
            }
        }
    }
    else if(index/COLUMN!=selectedindex/COLUMN&&abs(index-selectedindex)%COLUMN==0)
    {
        if(index>selectedindex)
        {
            for(int i = selectedindex+COLUMN;i<index;i+=COLUMN)
                if(Map[i]>0)
                    num++;
            if(num>1) {legal = false;return;}
            else if(num==1)
            {
                if(Map[index]>0&&(tempjudge1||tempjudge2)){legal = true;return;}
                else {legal = false;return;}
            }
            else
            {
                if(Map[index]>0){legal = false;return;}
                else {legal = true;return;}
            }
        }
        else
        {
            for(int i = index+COLUMN;i<selectedindex;i+=COLUMN)
                if(Map[i]>0)
                    num++;
            if(num>1) {legal = false;return;}
            else if(num==1)
            {
                if(Map[index]>0&&(tempjudge1||tempjudge2)){legal = true;return;}
                else {legal = false;return;}
            }
            else
            {
                if(Map[index]>0){legal = false;return;}
                else {legal = true;return;}
            }
        }
    }
    legal = false;
}

void Board::judgehorse()
{
    if(selectednumber==0) {legal = true;return;}
    int rownow = index/COLUMN;
    int columnnow = index%COLUMN;
    int rowprevious = selectedindex/COLUMN;
    int columnprevious = selectedindex%COLUMN;

    bool judge1 = abs(rownow-rowprevious)==1&& columnnow-columnprevious==2;
    bool judge2 = rownow-rowprevious==2&& abs(columnnow-columnprevious)==1;
    bool judge3 = abs(rownow-rowprevious)==1&& columnnow-columnprevious==-2;
    bool judge4 = rownow-rowprevious==-2&& abs(columnnow-columnprevious)==1;

    if(judge1)
    {
        if(Map[selectedindex+1]>0) {legal = false;return;}
        else legal = true;
    }
    else if(judge3)
    {
        if(Map[selectedindex-1]>0){legal = false;return;}
        else legal = true;
    }
    else if(judge2)
    {
        if(Map[selectedindex+COLUMN]>0) {legal = false;return;}
        else legal = true;
    }
    else if(judge4) {
        if(Map[selectedindex-COLUMN]>0) {legal = false;return;}
        else legal = true;
    }
    else legal = false;
}

void Board::judgeminister()
{
    if(selectednumber==0) {legal = true;return;}
    int rownow = index/COLUMN;
    int columnnow = index%COLUMN;
    int rowprevious = selectedindex/COLUMN;
    int columnprevious = selectedindex%COLUMN;

    bool judge1 = rownow-rowprevious==2&& columnnow-columnprevious==2;
    bool judge2 = rownow-rowprevious==-2&& columnnow-columnprevious==2;
    bool judge3 = rownow-rowprevious==-2&& columnnow-columnprevious==-2;
    bool judge4 = rownow-rowprevious==2&& columnnow-columnprevious==-2;
    //if(Map[selectedindex]<=7&&rownow>4){legal = false;return;}
    //if(Map[selectedindex]>=7&&rownow<5){legal = false;return;}
    if(rownow<5){legal = false;return;}
    if(judge1)
    {
        if(Map[selectedindex+COLUMN+1]>0) {legal = false;return;}
        else legal = true;
    }
    else if(judge2)
    {
        if(Map[selectedindex+1-COLUMN]>0){legal = false;return;}
        else legal = true;
    }
    else if(judge3)
    {
        if(Map[selectedindex-COLUMN-1]>0) {legal = false;return;}
        else legal = true;
    }
    else if(judge4) {
        if(Map[selectedindex+COLUMN-1]>0) {legal = false;return;}
        else legal = true;
    }
    else legal = false;
}

void Board::judgeadvisor()
{
    if(selectednumber==0) {legal = true;return;}
    int rownow = index/COLUMN;
    int columnnow = index%COLUMN;
    int rowprevious = selectedindex/COLUMN;
    int columnprevious = selectedindex%COLUMN;
    //if(Map[selectedindex]<=7&&(rownow>2||columnnow>5||columnnow<3)){legal = false;return;}
    //if(Map[selectedindex]>=7&&(rownow<7||columnnow>5||columnnow<3)){legal = false;return;}
    if(rownow<7||columnnow>5||columnnow<3){legal = false;return;}
    if(abs(rownow-rowprevious)==1&&abs(columnnow-columnprevious)==1){legal = true;return;}
    else legal = false;
}

void Board::judgegeneral()
{
    if(selectednumber==0) {legal = true;return;}
    int rownow = index/COLUMN;
    int columnnow = index%COLUMN;
    int rowprevious = selectedindex/COLUMN;
    int columnprevious = selectedindex%COLUMN;
    //if(Map[selectedindex]==1&&(rownow>2||columnnow>5||columnnow<3)){legal = false;return;}
    //if(Map[selectedindex]==8&&(rownow<7||columnnow>5||columnnow<3)){legal = false;return;}
    if(rownow<7||columnnow>5||columnnow<3){legal = false;return;}
    bool judge1 = (abs(rownow-rowprevious)==1&&columnnow==columnprevious);
    bool judge2 = (rownow==rowprevious&&abs(columnnow-columnprevious)==1);
    if(judge1||judge2){legal = true;return;}
    else legal = false;
}

void Board::judgesoldier()
{
    if(selectednumber==0) {legal = true;return;}
    int rownow = index/COLUMN;
    int columnnow = index%COLUMN;
    int rowprevious = selectedindex/COLUMN;
    int columnprevious = selectedindex%COLUMN;
    if(rownow<5)
    {
        if((abs(columnnow-columnprevious)==1&&rownow==rowprevious)
                ||(abs(rownow-rowprevious)==1&&columnnow==columnprevious))
        {
            if(columnnow==columnprevious&&rownow-rowprevious==1) {legal = false;return;}
            else {legal = true;return;}
        }
        else {legal = false;return;}
    }
    else
    {
        if(columnnow==columnprevious&&rownow-rowprevious==-1) {legal = true;return;}
        else {legal = false;return;}
    }
}

void Board::generalpos()
{
    for(int i = 0;i<ROW*COLUMN;i++) if(Map[i]==1) blackgeneralpos = i;
    for(int i = 0;i<ROW*COLUMN;i++) if(Map[i]==8) redgeneralpos = i;
}

bool Board::attackgeneral()
{
    generalpos();
    if(blackturn)
        return attackgeneral(RED_CHARIOT) || attackgeneral(RED_HORSE)
               ||attackgeneral(RED_CANNON) || attackgeneral(RED_SOLDIER);
    else return attackgeneral(BLACK_CANNON) || attackgeneral(BLACK_CHARIOT)
            ||attackgeneral(BLACK_HORSE) || attackgeneral(BLACK_SOLDIER);
}

bool Board::attackgeneral(int chess)
{
    bool attack = false;
    bool templegal = legal;
    int tempselectednumber = selectednumber;
    int tempselectedindex =selectedindex;
    int tempindex = index;
    for(int i = 0; i < ROW*COLUMN; ++i)
        {
            if(Map[i]==chess)
            {
                selectedindex = i;
                selectednumber = chess;
                if(blackturn) index = blackgeneralpos;
                else index = redgeneralpos;
                legal = false;
                judge();
                if(legal) {
                    attack = true;break;
                }
            }
        }
    legal = templegal;
    selectedindex = tempselectedindex;
    selectednumber = tempselectednumber;
    index = tempindex;
    return attack;
}

void Board::refreshBoard()
{
    redturn = !redturn;
    blackturn = !blackturn;
    reverse();
    timer->start();
    attackgeneral();
    judgewin();
    update();
}

void Board::otherprepare()
{
    other_prepare = true;
    lefttime = 20;
    if(self_prepare)begingame();
    update();

}

void Board::setRed()
{
    //ui->label->setText(tr("红方"));
    for(int i = 0;i<ROW*COLUMN+5;i++)
        Map[i] = INITIAL[i];
    side = RED;
    update();
}

void Board::setBlack()
{
    for(int i = 0;i<ROW*COLUMN+5;i++)
        Map[i] = INITIAL[i];
    reverse();
    side = BLACK;
    update();
}

void Board::getchangeturn()
{
    //redturn = false;
    //blackturn = true;
}

void Board::on_actiongive_up_triggered()
{
    timer->stop();
    int ret = QMessageBox::warning(0, tr("MainWindow"), tr("你确定要认输吗？"),
                                   QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        initial();
        losemusic();
        emit youwinforgiveup();
        QMessageBox::about(nullptr,tr("认输"),tr("很遗憾。继续努力～"));
    }
    else timer->start();
}

void Board::reverse()
{
    int* newMap = new int[ROW*COLUMN];
    for(int i = 0;i<ROW*COLUMN;i++)
    {
        int row = i/COLUMN;
        int column = i%COLUMN;
        int newrow = 9-row;
        int newcolumn = 8 - column;
        int newindex = newrow*COLUMN+newcolumn;
        newMap[newindex] = Map[i];
    }
    for(int i = 0;i<ROW*COLUMN;i++)
        Map[i]=newMap[i];
}

void Board::judgewin()
{
    bool havered = false;
    bool haveblack = false;
    for(int i = 0;i<ROW*COLUMN;i++)
    {
        if(Map[i]==1) haveblack = true;
        if(Map[i]==8) havered = true;
    }
    if(judgeencounter())
    {
        bool myturn = (side==RED&&redturn)||(side==BLACK&&blackturn);
        if(myturn) emit winbyeat();
        else emit losebyeat();
    }
    if(side==RED&&!haveblack) emit winbyeat();
    if(side==BLACK&&!havered) emit winbyeat();
    if(side==RED&&!havered) emit losebyeat();
    if(side==BLACK&&!haveblack) emit losebyeat();
}

bool Board::judgeencounter()
{
    int red = -1;
    int black = -1;
    for(int i = 0;i<ROW*COLUMN;i++)
    {
        if(Map[i]==1) black = i;
        if(Map[i]==8) red = i;
    }
    int redcolumn = red%COLUMN;
    int blackcolumn = black%COLUMN;
    if(redcolumn!=blackcolumn) return false;
    else
    {
        int num = 0;
        if(side==RED)
            for(int i = black+COLUMN;i<red;i+=COLUMN)
                if(Map[i]!=0) num++;
        if(side==BLACK)
            for(int i = red+COLUMN;i<black;i+=COLUMN)
                if(Map[i]!=0) num++;
        if(num==0) return true;
        else return false;
    }

}

void Board::on_actionnew_game_triggered()
{
    for(int i = 0;i<ROW*COLUMN+5;i++)
        Map[i] = INITIAL[i];
    if(side==BLACK) reverse();
    initial();
    update();
}

QVector<int> Board::GetChessPos(int index)
{
    QVector<int> ans;
    int row;
    int column;
    for(int i = 0;i<ROW*COLUMN;i++)
    {
        if(Map[i]==index)
        {
            row = i/COLUMN;
            column = i % COLUMN;
            ans.push_back(column);
            ans.push_back(9-row);
        }
    }
    return ans;
}

QString Board::transformstring(int index)
{
    int num1 = GetChessPos(index).size()/2;
    QString str1;
    str1.clear();
    str1.append(QString::number(num1));
    for(int i = 0;i<GetChessPos(index).size();i++)
    {
        if((i+1)%2==1)
        {
            str1.append(" <");
            str1.append(QString::number(GetChessPos(index)[i]));
            str1.append(",");
        }
        else
        {
            str1.append(QString::number(GetChessPos(index)[i]));
            str1.append(">");
        }
    }
    return str1;
}

void Board::solvetxt(int index,QString str)
{
//qDebug()<<index;
//qDebug()<<str;
    QString numstr;
    numstr.append(str[0]);
    int num = numstr.toInt();
    if(num == 0) return;
    QString rowstr;
    rowstr.clear();
    QString columnstr;
    rowstr.clear();
    int row;
    int column;
    for(int i = 0;i<str.length();i++)
    {
        if(i%6==3)
        {
            columnstr.append(str[i]);
            column = columnstr.toInt();
            columnstr.clear();

        }
        if(i%6==5)
        {
            rowstr.append(str[i]);
            row = rowstr.toInt();
            rowstr.clear();
            row = 9 - row;
            Map[row*COLUMN+column] = index;
        }
    }
}

void Board::on_actionsave_triggered()
{
    QString fn = QFileDialog::getSaveFileName(this, tr("保存棋局"), QString(), tr("Plain text Files(*.txt);;All Files (*)"));
    if (fn.isEmpty())
    {
        return;
    }
    if (! fn.endsWith(".txt",Qt::CaseInsensitive))
    {
        fn += ".txt";
    }
    QFile file(fn);
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        return;
    }
    QTextStream out(&file);
    if(redturn)
    {
        out<<"red"<<endl;
        out<<transformstring(8)<<endl;
        out<<transformstring(12)<<endl;
        out<<transformstring(13)<<endl;
        out<<transformstring(10)<<endl;
        out<<transformstring(9)<<endl;
        out<<transformstring(11)<<endl;
        out<<transformstring(14)<<endl;
        out<<"black"<<endl;
        out<<transformstring(1)<<endl;
        out<<transformstring(5)<<endl;
        out<<transformstring(6)<<endl;
        out<<transformstring(3)<<endl;
        out<<transformstring(2)<<endl;
        out<<transformstring(4)<<endl;
        out<<transformstring(7)<<endl;
    }
    else
    {
        out<<"black"<<endl;
        out<<transformstring(1)<<endl;
        out<<transformstring(5)<<endl;
        out<<transformstring(6)<<endl;
        out<<transformstring(3)<<endl;
        out<<transformstring(2)<<endl;
        out<<transformstring(4)<<endl;
        out<<transformstring(7)<<endl;
        out<<"red"<<endl;
        out<<transformstring(8)<<endl;
        out<<transformstring(12)<<endl;
        out<<transformstring(13)<<endl;
        out<<transformstring(10)<<endl;
        out<<transformstring(9)<<endl;
        out<<transformstring(11)<<endl;
        out<<transformstring(14)<<endl;
    }
}

void Board::on_actionimport_triggered()
{
    for(int i = 0;i<ROW*COLUMN;i++)
            Map[i]=0;    
    QFileDialog*fileDialog=new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Text"));
    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        qDebug()<<path;
        QFile*file=new QFile(path);
        QString line;
        if(file->open(QFile::ReadOnly))
        {
            QTextStream in(file);
            in.setCodec("UTF-8");
            line=in.readLine();
            if(line=="red")
            {
                line=in.readLine();solvetxt(8,line);
                line=in.readLine();solvetxt(12,line);
                line=in.readLine();solvetxt(13,line);
                line=in.readLine();solvetxt(10,line);
                line=in.readLine();solvetxt(9,line);
                line=in.readLine();solvetxt(11,line);
                line=in.readLine();solvetxt(14,line);
                line=in.readLine();

                line=in.readLine();solvetxt(1,line);
                line=in.readLine();solvetxt(5,line);
                line=in.readLine();solvetxt(6,line);
                line=in.readLine();solvetxt(3,line);
                line=in.readLine();solvetxt(2,line);
                line=in.readLine();solvetxt(4,line);
                line=in.readLine();solvetxt(7,line);
            }
            else
            {
                redturn = false;blackturn = true;
                line=in.readLine();solvetxt(1,line);
                line=in.readLine();solvetxt(5,line);
                line=in.readLine();solvetxt(6,line);
                line=in.readLine();solvetxt(3,line);
                line=in.readLine();solvetxt(2,line);
                line=in.readLine();solvetxt(4,line);
                line=in.readLine();solvetxt(7,line);
                line=in.readLine();
                line=in.readLine();solvetxt(8,line);
                line=in.readLine();solvetxt(12,line);
                line=in.readLine();solvetxt(13,line);
                line=in.readLine();solvetxt(10,line);
                line=in.readLine();solvetxt(9,line);
                line=in.readLine();solvetxt(11,line);
                line=in.readLine();solvetxt(14,line);
            }
        }
    }

    if(side==BLACK) reverse();
    update();
    emit changeboard();
    initial();
    if(blackturn) emit sendchangeturn();
}

void Board::preparegame()
{
    self_prepare = true;
    emit haveprepare();
    lefttime = 20;
    if(other_prepare) begingame();
    update();
}

void Board::on_actionbegin_triggered()
{
    preparegame();
}

void Board::begingame()
{
    ui->lcdNumber->show();
    ui->lcdNumber->display(lefttime);
    ui->label_2->show();
    timer->stop();
    QMediaPlayer* player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile("/Users/kingjames/Desktop/audios/NewGame.wav"));
    player->setVolume(30);
    player->play();
    begin = true;
    blackturn = false;
    redturn = true;
    if(side==RED)
        timer->start();
    lefttime = 20;
    ui->actiongive_up->setEnabled(true);
    ui->actionsave->setEnabled(true);
    ui->actionnew_game->setEnabled(false);
    ui->actionimport->setEnabled(false);
    ui->actionbegin->setEnabled(false);
}

void Board::initial()
{
    self_prepare = false;
    other_prepare = false;
    begin = false;
    redturn = true;
    blackturn = false;
    lefttime = 20;
    timer->stop();
    ui->lcdNumber->hide();
    ui->label_2->hide();
    ui->actiongive_up->setEnabled(false);
    ui->actionsave->setEnabled(false);
    ui->actionnew_game->setEnabled(true);
    ui->actionimport->setEnabled(true);
    ui->actionbegin->setEnabled(true);
}
