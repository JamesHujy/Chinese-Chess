#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "initialcover.h"
#include <QMainWindow>
#include "board.h"
#include <QtNetwork>
#include <QLineEdit>
#include <QDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QSignalMapper>
#include <vector>
#include <algorithm>
#include <QStringList>
#include <QEventLoop>
#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <stdlib.h>
#include <QPalette>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMediaPlayer>
#include <QApplication>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initialize();
    void connectFail();

private slots:

    void initServer();
    void initServer_2();
    void endprogram();
    void connectHost();
    void connectHost_2();
    void acceptConnection();
    void connected();
    void recvMessage();

    void sendMap();
    void sendtimewin();
    void sendgiveupwin();
    void sendprepare();

    void sendturn();

signals:
    void refreshBoard();
    void winthegamefortime();
    void winthegameforgiveup();
    void acceptprepare();
    void sendchangeturn();


private:
    Ui::MainWindow *ui;
    Board* chessboard;
    QDialog *waitingDialog;
    InitialCover *cover;
    QDialog *connectingDialog;
    QDialog *sueDialog;
    QTcpServer  *listenSocket;
    QTcpSocket *readWriteSocket;
    QLineEdit *line_1;
    QLineEdit *line_2;
    QString address;
    QDialog *newWaitingDialog;
    QLineEdit *edit;
};

#endif // MAINWINDOW_H
