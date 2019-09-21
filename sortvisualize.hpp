#ifndef SORTVISUALIZE_HPP
#define SORTVISUALIZE_HPP

#include <QGraphicsScene>
#include <functional>

class SortVisual : public QGraphicsScene {
public:
    SortVisual(int count, int height, int maxwidth);
    ~SortVisual();

    void start(void sorter(int *b, int *e,
                           const std::function<bool(int a, int b)> &cmp));

private:
    struct pdata;
    pdata *m_data;
};

#endif // SORTVISUALIZE_HPP
