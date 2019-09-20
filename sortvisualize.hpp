#ifndef SORTVISUALIZE_HPP
#define SORTVISUALIZE_HPP

#include <QGraphicsScene>

template<typename T> class element;

class SortVisual : public QGraphicsScene
{
public:
    SortVisual(int count, int height, int maxwidth);
    ~SortVisual();

    void start(void sorter(element<int> *b, element<int> *e));

private:
    struct pdata;
    pdata * m_data;
};


#endif // SORTVISUALIZE_HPP
