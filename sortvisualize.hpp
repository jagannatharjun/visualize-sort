#ifndef SORTVISUALIZE_HPP
#define SORTVISUALIZE_HPP

#include <QGraphicsScene>
#include <functional>
#include <chrono>

class SortVisual : public QGraphicsScene {
public:
    SortVisual(int count, int height, int maxwidth, int margin);
    ~SortVisual();

    void start(void sorter(int *b, int *e,
                           const std::function<bool(int a, int b)> &cmp));

    std::chrono::microseconds sortTime() const;

private:
    struct pdata;
    pdata *m_data;
};

#endif // SORTVISUALIZE_HPP
