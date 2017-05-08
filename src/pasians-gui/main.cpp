#include "pasians.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Pasians w;
    w.show();

    return a.exec();
}
