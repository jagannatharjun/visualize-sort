#ifndef SORTVISUALIZE_HPP
#define SORTVISUALIZE_HPP

#include <QWidget>

#include <vector>

class Bar;

class Sortable {
    int weight;
    Bar *bar;

public:
    Sortable(QWidget *parent, QRect rect, QColor color);
    Sortable(Sortable &&t);
    Sortable &operator=(Sortable &&t);

    Sortable() = delete;
    Sortable(Sortable &) = delete;
    Sortable &operator=(const Sortable &) = delete;
    int getWeight() const;


    operator int() const { return getWeight(); }
};

class SortVisualize : public QWidget {
    Q_OBJECT
public:
    explicit SortVisualize(int barCount, int barHeight,
                           QWidget *parent = nullptr);
    virtual ~SortVisualize() {}

    QSize minimumSizeHint() const override {
        return QSize(width(), m_bars.size() * 10);
    }

    void start();

    int barHeight() const;


signals:

public slots:

private:
    std::vector<Sortable> m_bars;
   // std::vector<Bar *> m_bars;
    int m_barHeight;
};

#endif // SORTVISUALIZE_HPP
