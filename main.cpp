#include <QApplication>
#include <QDebug>
#include <QGraphicsView>
#include <QTimer>

#include "sortvisualize.hpp"

void insertion_sort(int *a, int *b,
                    const std::function<bool(int a, int b)> &cmp) {
    for (int *i = a + 1; i != b; i++) {
        for (int *j = i; j != a && cmp(*j, *(j - 1)); j--) {
            std::swap(*j, *(j - 1));
        }
    }
}
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget w;

    SortVisual sv(400, 2, 800);

    QGraphicsView v(&w);
    v.setScene(&sv);
    v.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    v.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    w.show();

    sv.start(std::sort);

    return a.exec();
}
