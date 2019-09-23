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
    int m_height, margin;
    QTimer updateTimer;
    std::chrono::microseconds sortTime;
};

SortVisual::SortVisual(int count, int height, int maxwidth, int margin)
    : m_data(new pdata()) {

    m_data->margin = margin;
    m_data->m_height = height;

    int left = 0, t = 0;
    m_data->m_array.reserve(count);
    m_data->m_bars.reserve(count);
    for (int i = 0; i < count; i++) {
        auto w = rand() % maxwidth;
        auto r = addRect(QRect(left, t, w, height), QPen(Qt::transparent),
                         QBrush(Qt::red));
        t += height + margin;
        m_data->m_array.emplace_back(w);
        m_data->m_bars.push_back(r);
    }
    setSceneRect(0, 0, maxwidth, t);

    connect(&m_data->updateTimer, &QTimer::timeout, [this]() {
        if (m_data->ArraySnapShots.empty())
            return;
        auto &array = m_data->ArraySnapShots.front();
        int t = 0;
        for (int i = 0, s = m_data->m_bars.size(); i < s; i++) {
            auto b = m_data->m_bars[i];
            b->setRect(0, t, array[i], m_data->m_height);
            t += m_data->m_height + m_data->margin;
        }
        m_data->ArraySnapShots.pop();
    });
    m_data->updateTimer.setInterval(2);
    m_data->updateTimer.start();
}

SortVisual::~SortVisual() { delete m_data; }

#include <map>

void SortVisual::start(
    void sorter(int *b, int *e, const std::function<bool(int a, int b)> &cmp)) {

#ifndef NDEBUG
    auto calcMap = [&](const auto &b) {
        std::map<int, int> t;
        for (auto i : b)
            t[i]++;
        return t;
    };
    auto i = calcMap(m_data->m_array);
#endif

    auto now = std::chrono::system_clock::now();
    sorter(m_data->m_array.data(),
           m_data->m_array.data() + m_data->m_array.size(),
           [this](int a, int b) {
               if (m_data->ArraySnapShots.empty() ||
                   m_data->ArraySnapShots.back() != m_data->m_array)
                   m_data->ArraySnapShots.push(m_data->m_array);
               return a < b;
           });

    auto t = (std::chrono::system_clock::now() - now);
    m_data->sortTime = std::chrono::duration_cast<std::chrono::microseconds>(t);
    m_data->ArraySnapShots.push(m_data->m_array);

#ifndef NDEBUG
    assert(i == calcMap(m_data->m_array));
    assert(std::is_sorted(m_data->m_array.begin(), m_data->m_array.end()));
#endif
}

std::chrono::microseconds SortVisual::sortTime() const {}

void SortVisual::setUpdateTime(int msec) {
    m_data->updateTimer.setInterval(msec);
}
