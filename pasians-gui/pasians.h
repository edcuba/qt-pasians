#ifndef PASIANS_H
#define PASIANS_H

#include <QMainWindow>
#include <QRect>

namespace Ui {
class Pasians;
}

class Pasians : public QMainWindow
{
    Q_OBJECT

public:
    explicit Pasians(QWidget *parent = 0);
    ~Pasians();

private:
    Ui::Pasians *ui;
    QRect *card;
};

#endif // PASIANS_H
