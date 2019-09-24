#include "initialcover.h"
#include "ui_initialcover.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QPixmap>

InitialCover::InitialCover(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitialCover)
{
    ui->setupUi(this);
    button_1 = new QPushButton;
    button_2 = new QPushButton;
    QVBoxLayout *layout = new QVBoxLayout;
    QHBoxLayout *layout_1 = new QHBoxLayout;
    QLabel *label_0 = new QLabel(this);
    label_0->setText(tr(""));
    button_1->setText(tr("创建房间"));
    button_2->setText(tr("进入房间"));
    layout_1->addWidget(button_1);
    layout_1->addWidget(button_2);
    layout->addWidget(label_0);
    layout->addLayout(layout_1);
    this->setLayout(layout);
    this->setAutoFillBackground(true);
    QPalette palette;
    QPixmap cover(":/images/images/cover.png");
    palette.setBrush(QPalette::Window,QBrush(cover));
    this->setPalette(palette);
}

InitialCover::~InitialCover()
{
    delete ui;
}
