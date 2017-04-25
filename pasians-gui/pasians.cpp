#include "pasians.h"
#include "ui_pasians.h"
#include <QPixmap>
#include <QLabel>

Pasians::Pasians(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Pasians)
{
    ui->setupUi(this);

    QLabel *lbl = new QLabel("test");
    ui->gridLayout->addWidget(lbl, 500, 300);
    QPixmap mypix (":/res/ace_of_spades.png");
    lbl->setPixmap(mypix);

}

Pasians::~Pasians()
{
    delete ui;
}
