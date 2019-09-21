#include "sortvisualize.hpp"
#include "element.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsRectItem>
#include <thread>
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

    std::shared_ptr<std::function<void()>> update =
        std::make_shared<std::function<void()>>();
    *update = [this]() {
        int t = 0;
        int w = -1;
        for (int i = 0, s = m_data->m_bars.size(); i < s; i++) {
            auto b = m_data->m_bars[i];
            w = std::max(m_data->m_array[i], w);
            b->setRect(0, t, m_data->m_array[i], m_data->m_height);
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
        auto w = rand() % maxwidth;
        auto r = addRect(QRect(left, t, w, height), QPen(Qt::transparent),
                         QBrush(Qt::red));
        t += height;
        m_data->m_array.emplace_back(w);
        m_data->m_bars.push_back(r);
    }

}

SortVisual::~SortVisual() { delete m_data; }

void SortVisual::start(
    void sorter(int *b, int *e, const std::function<bool(int a, int b)> &cmp)) {

    sorter(m_data->m_array.data(), m_data->m_array.data() + m_data->m_array.size(),
              [this](int a, int b) {
                  int t = 0;
                  this->blockSignals(true);
                  for (int i = 0, s = m_data->m_bars.size(); i < s; i++) {
                      auto b = m_data->m_bars[i];
                      b->setRect(0, t, m_data->m_array[i], m_data->m_height);
                      t += m_data->m_height;
                  }
                  this->blockSignals(false);
                  qApp->processEvents();
                  std::this_thread::sleep_for(1ms);
                  return a < b;
              });
    assert(std::is_sorted(m_data->m_array.begin(), m_data->m_array.end()));
}
