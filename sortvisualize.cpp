#include "sortvisualize.hpp"

#include <QApplication>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

#include <algorithm>
#include <assert.h>
#include <chrono>
#include <thread>

class Bar : public QWidget {
public:
    using QWidget::QWidget;

    QColor color() const { return m_color; }
    void setColor(const QColor &color) { m_color = color; }

private:
    QColor m_color;

    void paintEvent(QPaintEvent *event) override;
};

SortVisualize::SortVisualize(int barCount, int barHeight, QWidget *parent)
    : QWidget(parent), m_barHeight(barHeight) {
    const int m = 0;
    int left = 0, top = 0;

    m_bars.reserve(barCount);
    for (int i = 0; i < barCount; i++) {
        m_bars.emplace_back(this, QRect(left, top, rand() % width(), barHeight),
                            Qt::red);
        top += barHeight + m;
    }
}

void SortVisualize::start() {
    auto weights = [&]() {
        std::map<int, int> a;
        for (const auto &b : m_bars)
            a[b.getWeight()]++;
        return a;
    };
    auto b = weights();
    std::sort(m_bars.begin(), m_bars.end());
    qDebug() << "done";
    assert(std::is_sorted(m_bars.begin(), m_bars.end()));
    assert(weights() == b);
}

int SortVisualize::barHeight() const { return m_barHeight; }

void Bar::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setBrush(QBrush(color()));
    painter.setPen(color());
    painter.drawRect(event->rect());
}

int Sortable::getWeight() const { return weight; }

Sortable::Sortable(QWidget *parent, QRect rect, QColor color) {
    bar = new Bar(parent);
    bar->setGeometry(rect);
    bar->setColor(color);
    weight = rect.width();
}

Sortable::Sortable(Sortable &&t) : bar{t.bar}, weight{t.weight} {
    //    if (this == &t)
    //        return;
}

Sortable &Sortable::operator=(Sortable &&t) {
    if (this == &t)
        return *this;

    if (t.bar != bar) {
        auto w = t.bar->width(), h = t.bar->height();
        t.bar->resize(bar->width(), bar->height());
        bar->resize(w, h);

        using namespace std::literals;
        QApplication::processEvents();
        std::this_thread::sleep_for(2ms);

        std::swap(t.weight, weight);
        std::swap(t.bar, bar);
    }
    return *this;
}
