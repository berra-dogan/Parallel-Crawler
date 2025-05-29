#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include "Page.hpp"

class SafeUnboundedQueue {
private:
    std::queue<Page*> elements;
    mutable std::mutex lock;
    std::condition_variable not_empty;

public:
    SafeUnboundedQueue();

    void push(const Page* element);
    Page* pop();
    bool is_empty() const;
};

