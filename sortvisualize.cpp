#include "sortvisualize.hpp"
#include "element.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsRectItem>
#include <thread>

struct SortVisual::pdata {
    std::vector<element<int>> m_array;
    std::vector<QGraphicsRectItem *> m_bars;
    int m_height;
};

SortVisual::SortVisual(int count, int height, int maxwidth)
    : m_data(new pdata()) {
    m_data->m_height = height;

    std::shared_ptr<std::function<void()>> update =
        std::make_shared<std::function<void()>>();
    *update = [this]() {
        int t = 0;
        int w = -1;
        for (int i = 0, s = m_data->m_bars.size(); i < s; i++) {
            auto b = m_data->m_bars[i];
            w = std::max(m_data->m_array[i].weight(), w);
            b->setRect(0, t, m_data->m_array[i].weight(), m_data->m_height);
            t += m_data->m_height;
        }
        using namespace std::literals::chrono_literals;
        qApp->processEvents();
        std::this_thread::sleep_for(10ms);
    };

    int left = 0, t = 0;
    m_data->m_array.reserve(count);
    m_data->m_bars.reserve(count);
    for (int i = 0; i < count; i++) {
        auto r = addRect(QRect(left, t, rand() % maxwidth, height),
                         QPen(Qt::transparent), QBrush(Qt::red));
        t += height;
        m_data->m_array.emplace_back(update, rand() % 500);
        m_data->m_bars.push_back(r);
    }
}

SortVisual::~SortVisual() { delete m_data; }

void SortVisual::start(void sorter(element<int> *, element<int> *)) {
    std::sort(m_data->m_array.begin(), m_data->m_array.end());
    assert(std::is_sorted(m_data->m_array.begin(), m_data->m_array.end()));
}


