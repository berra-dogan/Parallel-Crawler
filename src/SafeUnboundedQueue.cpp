#include "../include/SafeUnboundedQueue.hpp"

SafeUnboundedQueue::SafeUnboundedQueue() {}

void SafeUnboundedQueue::push(const Page* element) {
    std::lock_guard<std::mutex> lk(lock);

    bool wasEmpty = elements.empty();
    elements.push(const_cast<Page*>(element));  // Cast away constness if needed

    if (wasEmpty) {
        not_empty.notify_all();
    }
}

Page* SafeUnboundedQueue::pop() {
    std::unique_lock<std::mutex> lk(lock);

    while (elements.empty()) {
        not_empty.wait(lk);
    }

    Page* popped_elt = elements.front();
    elements.pop();

    return popped_elt;
}
Page* SafeUnboundedQueue::pop_non_empty() {
    std::unique_lock<std::mutex> lk(lock);

    if (elements.empty()) {
        return NULL;
    }

    Page* popped_elt = elements.front();
    elements.pop();

    return popped_elt;
}



bool SafeUnboundedQueue::is_empty() const {
    std::lock_guard<std::mutex> lk(lock);
    return elements.empty();
}
