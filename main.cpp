#include "fm_principal.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Fm_principal w;
    w.show();
    return a.exec();
}
