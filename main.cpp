#include "CustomWidgetsDialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CustomWidgetsDialog w;
    w.show();

    return a.exec();
}
