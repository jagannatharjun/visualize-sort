#include <QApplication>
#include <QDebug>
#include <QGraphicsView>

#include "element.h"
#include "sortvisualize.hpp"

void insertion_sort(int *a, int *b,
                    const std::function<bool(int a, int b)> &cmp) {
    for (int *i = a + 1; i != b; i++) {
        for (int *j = i; j != a && cmp(*j, *(j - 1)); j--) {
            std::swap(*j, *(j - 1));
        }
    }
}

#include <QTimer>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    SortVisual sv(200, 2, 800);

    QGraphicsView v;
    v.setScene(&sv);
    v.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    v.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    v.show();

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&] () {
        sv.start(insertion_sort);
        timer.stop();
    });
    timer.setInterval(1000);
    timer.start();
    auto t = a.exec();
    return t;
}
