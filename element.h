#ifndef ELEMENT_H
#define ELEMENT_H

#include <functional>
#include <memory>

template <typename T> class element {
public:
    element(std::shared_ptr<std::function<void()>> onSwapRequest, T w)
        : m_onSwapRequest{std::move(onSwapRequest)}, m_weight(std::move(w)) {}
    element(element &&e) { swap(e); }
    element &operator=(element &&e) {
        swap(e);
        return *this;
    }

    element() = delete;
    element(const element &) = delete;
    element &operator=(const element &) = delete;
    ~element() = default;

    T weight() const { return m_weight; }
    bool operator<(const element &e) { return weight() < e.weight(); }

private:
    std::shared_ptr<std::function<void()>> m_onSwapRequest;
    T m_weight = 0;

    void swap(element &e) {
        if (&e == this)
            return;
        std::swap(m_weight, e.m_weight);
        std::swap(m_onSwapRequest, e.m_onSwapRequest);
        if (m_onSwapRequest)
            (*m_onSwapRequest)();
        if (e.m_onSwapRequest)
            (*(e.m_onSwapRequest))();
    }
};

#endif // ELEMENT_H
