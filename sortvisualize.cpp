#include "sortvisualize.hpp"
#include <QApplication>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QTimer>
#include <queue>

using namespace std::literals;

struct SortVisual::pdata {
    std::vector<int> m_array;
    std::vector<QGraphicsRectItem *> m_bars;
    std::queue<std::vector<int>> ArraySnapShots;
    int m_height;
};

SortVisual::SortVisual(int count, int height, int maxwidth)
    : m_data(new pdata()) {
    m_data->m_height = height;

    int left = 0, t = 0;
    m_data->m_array.reserve(count);
    m_data->m_bars.reserve(count);
    for (int i = 0; i < count; i++) {
        auto w = rand() % maxwidth;
        auto r = addRect(QRect(left, t, w, height), QPen(Qt::transparent),
                         QBrush(Qt::red));
        t += height;
        m_data->m_array.emplace_back(w);
        m_data->m_bars.push_back(r);
    }

    auto updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, [this]() {
        if (m_data->ArraySnapShots.empty())
            return;
        auto &array = m_data->ArraySnapShots.front();
        int t = 0;
        for (int i = 0, s = m_data->m_bars.size(); i < s; i++) {
            auto b = m_data->m_bars[i];
            b->setRect(0, t, array[i], m_data->m_height);
            t += m_data->m_height;
        }
        m_data->ArraySnapShots.pop();
    });
    updateTimer->setInterval(2);
    updateTimer->start();
}

SortVisual::~SortVisual() { delete m_data; }

void SortVisual::start(
    void sorter(int *b, int *e, const std::function<bool(int a, int b)> &cmp)) {

    sorter(m_data->m_array.data(),
           m_data->m_array.data() + m_data->m_array.size(),
           [this](int a, int b) {
               m_data->ArraySnapShots.push(m_data->m_array);
               return a < b;
           });
    assert(std::is_sorted(m_data->m_array.begin(), m_data->m_array.end()));
}
