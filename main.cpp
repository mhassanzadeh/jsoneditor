#include <QtGui/QApplication>
#include "jsoneditormain.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    JsonEditorMain w;
    w.show();
    return a.exec();
}
