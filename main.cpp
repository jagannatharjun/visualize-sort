#include <QApplication>
#include <QDebug>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>

#include "sortvisualize.hpp"

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
            scene.start(sort);
            //             labl.setText(
            //                 labl.text() + " " +
            //                 QString::number(
            //                     std::chrono::duration_cast<std::chrono::milliseconds>(
            //                         scene.sortTime())
            //                         .count()));
        });
        thread->start();
    }

    ~SortWidget() { delete thread; }
};

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QWidget w;
    QVBoxLayout l;
    w.setLayout(&l);
    l.setAlignment(Qt::AlignLeft);

    QHBoxLayout v1, v2;
    l.addLayout(&v1);
    l.addLayout(&v2);

    SortWidget stdsort("std::sort", 200, 1, 400, 0, v1),
        stdstablesort("std::stable_sort", 200, 1, 400, 0, v1),
        insertionSort("insertion_sort", 200, 1, 400, 0, v2),
        selectionSort("selection_sort", 200, 1, 400, 0, v2);

    w.show();

    stdsort.start(std::sort);
    stdstablesort.start(std::stable_sort);
    insertionSort.start(insertion_sort);

    selectionSort.scene.setUpdateTime(64);
    selectionSort.start(selection_sort);

    return a.exec();
}
