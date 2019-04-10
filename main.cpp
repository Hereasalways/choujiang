#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.setFixedSize(800, 600);
    w.setWindowFlags(Qt::WindowCloseButtonHint);
    w.show();

    return a.exec();
}

