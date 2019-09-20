#include <QApplication>

#include "sortvisualize.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SortVisualize sv(200, 2);
    sv.show();

    sv.start();
    return a.exec();
}
