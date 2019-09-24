#ifndef INITIALCOVER_H
#define INITIALCOVER_H

#include <QDialog>

namespace Ui {
class InitialCover;
}

class InitialCover : public QDialog
{
    Q_OBJECT

public:
    explicit InitialCover(QWidget *parent = 0);
    ~InitialCover();
    QPushButton *button_1;
    QPushButton *button_2;

private:
    Ui::InitialCover *ui;
};

#endif // INITIALCOVER_H
