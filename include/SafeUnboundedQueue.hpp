#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include "Page.hpp"

/**
 * @class SafeUnboundedQueue
 * @brief A thread-safe unbounded queue for Page* pointers.
 *
 * This class provides a thread-safe wrapper around std::queue using a mutex
 * and condition variable to allow concurrent push and pop operations.
 */
class SafeUnboundedQueue {
private:
    std::queue<Page*> elements;            ///< Internal queue storing pointers to Page objects
    mutable std::mutex lock;               ///< Mutex to protect access to the queue
    std::condition_variable not_empty;     ///< Condition variable for blocking pops when queue is empty

public:
    /**
     * @brief Default constructor for SafeUnboundedQueue.
     */
    SafeUnboundedQueue();

    /**
     * @brief Pushes a new element into the queue.
     * 
     * If the queue was empty before the push, all waiting threads are notified.
     *
     * @param element Pointer to a Page object to be inserted into the queue.
     */
    void push(const Page* element);

    /**
     * @brief Pops an element from the queue.
     * 
     * Blocks the calling thread if the queue is empty until an element is available.
     *
     * @return Pointer to the Page object that was removed from the front of the queue.
     */
    Page* pop();

    Page* pop_non_empty();

    /**
     * @brief Checks whether the queue is currently empty.
     *
     * @return true if the queue is empty, false otherwise.
     */
    bool is_empty() const;
};

