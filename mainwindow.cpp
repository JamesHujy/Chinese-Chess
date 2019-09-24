#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHostAddress>
#include <QVector>
#include <QChar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::initialize()
{
    cover = new InitialCover;
    cover->show();
    connect(cover->button_1,SIGNAL(clicked(bool)),this,SLOT(initServer()));
    connect(cover->button_2,SIGNAL(clicked(bool)),this,SLOT(connectHost()));
}

void MainWindow::initServer()
{
    waitingDialog = new QDialog;
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    QPushButton *button = new QPushButton;
    edit = new QLineEdit;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QString ipAddress;
        QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
        for (int i = 0; i < ipAddressesList.size(); ++i) {
            qDebug()<<ipAddressesList.at(i).toString();
             if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                 ipAddressesList.at(i).toIPv4Address()) {
                 ipAddress = ipAddressesList.at(i).toString();
                 break;
           }
        }
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    address = ipAddress;
    QString sentence_1;
    sentence_1.clear();
    sentence_1.append(tr("你的IP地址是:"));
    sentence_1.append(address);
    label_1->setText(sentence_1);
    label_2->setText(tr("设置端口号:"));
    button->setText(tr("确定"));
    layout_1->addWidget(label_2);
    layout_1->addWidget(edit);
    layout->addWidget(label_1);
    layout->addLayout(layout_1);
    layout->addWidget(button);
    waitingDialog->setLayout(layout);
    connect(button,SIGNAL(clicked(bool)),this,SLOT(initServer_2()));
    this->listenSocket =new QTcpServer;
    waitingDialog->show();
    waitingDialog->exec();
}

void MainWindow::initServer_2()
{
    waitingDialog->close();
    newWaitingDialog = new QDialog;
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    QLabel *label_3 = new QLabel;
    QString sentence_1;
    QString sentence_2;
    sentence_1.append("你的IP地址是:");
    sentence_1.append(address);
    label_2->setText(sentence_1);
    sentence_2.append("你的端口号是:");
    sentence_2.append(edit->text());
    label_3->setText(sentence_2);
    label_1->setText(tr("等待连接..."));
    QPushButton* cancel = new QPushButton(QObject::tr("cancel"));
    QVBoxLayout *layout_1 = new QVBoxLayout;
    layout_1->addWidget(label_2);
    layout_1->addWidget(label_3);
    layout_1->addWidget(label_1);
    layout_1->addWidget(cancel);
    newWaitingDialog->setLayout(layout_1);
    QString port = edit->text();
    int portInt = port.toInt();
    this->listenSocket->listen(QHostAddress::Any,portInt);
    QObject::connect(cancel,SIGNAL(clicked()),this,SLOT(endprogram()));
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
    newWaitingDialog->show();
    newWaitingDialog->exec();
}

void MainWindow::connectHost()
{
    connectingDialog = new QDialog;
    QLabel *label_1 = new QLabel;
    QLabel *label_2 = new QLabel;
    line_1 = new QLineEdit;
    line_2 = new QLineEdit;
    QPushButton *button = new QPushButton;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QHBoxLayout *layout_2 = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout;
    label_1->setText(tr("对方的IP地址:"));
    label_2->setText(tr("对方的端口号:"));
    button->setText(tr("确认连接"));
    layout_1->addWidget(label_1);
    layout_1->addWidget(line_1);
    layout_2->addWidget(label_2);
    layout_2->addWidget(line_2);
    layout->addLayout(layout_1);
    layout->addLayout(layout_2);
    layout->addWidget(button);
    connectingDialog->setLayout(layout);
    this->readWriteSocket = new QTcpSocket;
    connect(button,SIGNAL(clicked(bool)),this,SLOT(connectHost_2()));
    connectingDialog->show();
    connectingDialog->exec();
}

void MainWindow::connectHost_2()
{
    QString address = line_1->text();
    QString portString = line_2->text();
    int port = portString.toInt();
    this->readWriteSocket->connectToHost(QHostAddress(address),port);
    connect(readWriteSocket,SIGNAL(connected()),this,SLOT(connected()));
    if(!readWriteSocket->waitForConnected())
        connectFail();
}

void MainWindow::connected()
{
    connectingDialog->close();
    cover->close();
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    chessboard = new Board;
    chessboard->setBlack();
    chessboard->show();

    connect(this,SIGNAL(sendchangeturn()),chessboard,SLOT(getchangeturn()));
    connect(chessboard,SIGNAL(sendchangeturn()),this,SLOT(sendturn()));
    connect(this,SIGNAL(refreshBoard()),chessboard,SLOT(refreshBoard()));
    connect(chessboard,SIGNAL(changeboard()),this,SLOT(sendMap()));
    connect(chessboard,SIGNAL(youwinfortime()),this,SLOT(sendtimewin()));
    connect(chessboard,SIGNAL(youwinforgiveup()),this,SLOT(sendgiveupwin()));
    connect(chessboard,SIGNAL(haveprepare()),this,SLOT(sendprepare()));
    connect(this,SIGNAL(acceptprepare()),chessboard,SLOT(otherprepare()));
    connect(this,SIGNAL(winthegamefortime()),chessboard,SLOT(showtimewin()));
    connect(this,SIGNAL(winthegameforgiveup()),chessboard,SLOT(showgiveupwin()));
}

void MainWindow::acceptConnection()
{
    newWaitingDialog->close();
    cover->close();
    this->readWriteSocket = this->listenSocket->nextPendingConnection();
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
    chessboard = new Board;
    chessboard->setRed();
    chessboard->show();

    connect(this,SIGNAL(sendchangeturn()),chessboard,SLOT(getchangeturn()));
    connect(chessboard,SIGNAL(sendchangeturn()),this,SLOT(sendturn()));
    connect(this,SIGNAL(refreshBoard()),chessboard,SLOT(refreshBoard()));
    connect(chessboard,SIGNAL(changeboard()),this,SLOT(sendMap()));
    connect(chessboard,SIGNAL(youwinfortime()),this,SLOT(sendtimewin()));
    connect(chessboard,SIGNAL(haveprepare()),this,SLOT(sendprepare()));
    connect(this,SIGNAL(acceptprepare()),chessboard,SLOT(otherprepare()));
    connect(chessboard,SIGNAL(youwinforgiveup()),this,SLOT(sendgiveupwin()));
    connect(this,SIGNAL(winthegamefortime()),chessboard,SLOT(showtimewin()));
    connect(this,SIGNAL(winthegameforgiveup()),chessboard,SLOT(showgiveupwin()));}


void MainWindow::recvMessage()
{
    QString info;
    info += this->readWriteSocket->readAll();
    if(info.length()<90)
    {
        if(info=="timeout") emit winthegamefortime();
        if(info=="giveup") emit winthegameforgiveup();
        if(info=="prepare") emit acceptprepare();
        if(info=="turn") emit sendchangeturn();
    }
    else
    {
        for(int i = 0;i<ROW*COLUMN;i++)
            chessboard->Map[i]=info[i].toLatin1()-30;
        emit refreshBoard();
    }
}

void MainWindow::connectFail()
{
    QMessageBox::warning(NULL,tr("错误"),tr("请填入正确的IP地址和端口号！"));
}
void MainWindow::endprogram()
{
    newWaitingDialog->close();
    delete listenSocket;
}

void MainWindow::sendMap()
{
    QByteArray* array = new QByteArray();
    QString info;
    info.clear();
    for(int i = 0;i<ROW*COLUMN;i++)
    {
        char temp = chessboard->getMap()[i]+30;
        QChar *chartemp = new QChar(temp);
        info.append(*chartemp);
    }
//qDebug()<<info;
    array->append(info);
    this->readWriteSocket->write(array->data());

}

void MainWindow::sendturn()
{
    QByteArray* array = new QByteArray();
    QString info;
    info.clear();
    info.append("turn");
    array->append(info);
    this->readWriteSocket->write(array->data());
}

void MainWindow::sendprepare()
{
    QByteArray* array = new QByteArray();
    QString info;
    info.clear();
    info.append("prepare");
    array->append(info);
    this->readWriteSocket->write(array->data());
}
void MainWindow::sendtimewin()
{
    QByteArray* array = new QByteArray();
    QString info;
    info.clear();
    info.append("timeout");
    array->append(info);
    this->readWriteSocket->write(array->data());
}

void MainWindow::sendgiveupwin()
{
    QByteArray* array = new QByteArray();
    QString info;
    info.clear();
    info.append("giveup");
    array->append(info);
    this->readWriteSocket->write(array->data());
}

MainWindow::~MainWindow()
{
    delete ui;
}
