#include <QApplication>
#include <QGraphicsView>

#include "sortvisualize.hpp"
#include "element.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SortVisual sv(200,2, 800);

    QGraphicsView v;
    v.setScene(&sv);
    v.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    v.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    v.show();

    sv.start(std::sort);

    return a.exec();
}
