#include <QApplication>
#include <QDebug>
#include <QGraphicsView>
#include <QGridLayout>
#include <QTimer>

#include "flowlayout.h"
#include "sortvisualize.hpp"
#include <QThread>
#include <thread>

void insertion_sort(int *a, int *b,
                    const std::function<bool(int a, int b)> &cmp) {
    for (int *i = a + 1; i != b; i++) {
        for (int *j = i; j != a && cmp(*j, *(j - 1)); j--) {
            std::swap(*j, *(j - 1));
        }
    }
}

void selection_sort(int *a, int *b,
                    const std::function<bool(int a, int b)> &cmp) {
    for (int *i = a; i != b; i++) {
        int *min = i;
        for (int *j = i + 1; j != b; j++) {
            if (cmp(*j, *min))
                min = j;
        }
        std::swap(*min, *i);
    }
}

#include <QLabel>
#include <QVBoxLayout>
struct SortWidget {
    QWidget container;
    QLabel labl;
    QVBoxLayout layout;
    QGraphicsView view;
    SortVisual scene;
    QThread *thread;
    SortWidget(const char *name, int bcount, int barHeight, int maxWidth,
               int margin, QLayout &l)
        : scene(bcount, barHeight, maxWidth, margin) {
        container.setLayout(&layout);
        layout.addWidget(&labl);
        layout.addWidget(&view);
        labl.setText(name);
        labl.setAlignment(Qt::AlignCenter);
        labl.setFont(QFont("Courier New", 12, 8));
        view.setScene(&scene);
        view.setAlignment(Qt::AlignLeft | Qt::AlignTop);
        view.setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
        l.addWidget(&container);
    }
    void start(void sort(int *a, int *b,
                         const std::function<bool(int a, int b)> &cmp)) {
        thread = QThread::create([=]() {
            scene.start(sort);/*
            labl.setText(
                labl.text() + " " +
                QString::number(
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        scene.sortTime())
                        .count()));*/
        });
        thread->start();
    }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget w;
    FlowLayout l;
    w.setLayout(&l);
    l.setAlignment(Qt::AlignLeft);

    SortWidget stdsort("std::sort", 200, 1, 400, 0, l),
        stdstablesort("std::stable_sort", 200, 1, 400, 0, l),
        insertionSort("insertion_sort", 200, 1, 400, 0, l),
        selectionSort("selection_sort", 200, 1, 400, 0, l);

    w.resize(l.sizeHint() * 2);
    w.show();

    stdsort.start(std::sort);
    stdstablesort.start(std::stable_sort);
    insertionSort.start(insertion_sort);
    selectionSort.start(selection_sort);

    return a.exec();
}
