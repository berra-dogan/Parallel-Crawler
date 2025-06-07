#pragma once

#include <vector>
#include <list>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <functional>

#include "Page.hpp"

/**
 * @class RefinableHashSet
 * @brief A thread-safe hash set using fine-grained locking and lock refinement for dynamic resizing.
 *
 * Inspired by the implementation in *The Art of Multiprocessor Programming* by Herlihy & Shavit (Chapter 13),
 * this class stores pointers to `Page` objects and supports concurrent addition, removal, and lookup.
 * 
 * The resizing mechanism uses lock refinement, where a resizing thread marks itself as the owner and waits
 * for all locks to be quiesced before reallocating buckets and locks.
 */
class RefinableHashSet {
private:
    std::vector<std::list<Page*>> table;  ///< Hash table buckets storing pointers to `Page` objects.
    std::vector<std::unique_ptr<std::mutex>> locks; ///< One lock per bucket for fine-grained synchronization.
    std::atomic<size_t> size{0}; ///< Total number of elements stored.

    /**
     * @struct OwnerInfo
     * @brief Holds metadata about resizing ownership.
     */
    struct OwnerInfo {
        std::thread::id owner_id; ///< Thread ID of the resizing owner.
        bool mark;                ///< True if a resize is in progress.
    };

    std::atomic<OwnerInfo> owner; ///< Atomic marker for managing exclusive access during resize.

public:
    /**
     * @brief Constructs a RefinableHashSet with optional initial capacity.
     * 
     * @param capacity The initial number of buckets. Defaults to 256.
     */
    explicit RefinableHashSet(int capacity = 256);

    /**
     * @brief Acquires the lock associated with the hash of a string key.
     * Used internally to lock individual buckets.
     *
     * @param x The key whose bucket lock is to be acquired.
     */
    void acquire(const std::string& x);

    /**
     * @brief Releases the lock associated with the hash of a string key.
     *
     * @param x The key whose bucket lock is to be released.
     */
    void release(const std::string& x);

    /**
     * @brief Resizes the hash table when load factor exceeds a threshold.
     * Uses the lock refinement technique to prevent concurrent modifications.
     */
    void resize();
    /**
     * @brief Ensures all locks are temporarily idle.
     * Used before resizing to safely reallocate buckets.
     */
    void quiesce();

    /**
     * @brief Adds a new Page* to the set.
     * 
     * @param x Pointer to the Page object.
     * @return True if the page was added; false if it already existed.
     */
    bool add(Page* x);

    /**
     * @brief Removes a Page from the set by its URL.
     * 
     * @param x The URL of the Page to be removed.
     * @return True if the page was found and removed; false otherwise.
     */
    bool remove(const std::string x);

    /**
     * @brief Checks if a Page with a given URL exists in the set.
     * 
     * @param x The URL to search for.
     * @return True if found; false otherwise.
     */
    bool contains(const std::string x);

    /**
     * @brief Retrieves a pointer to the Page object with the given URL.
     * 
     * @param x The URL to search for.
     * @return Pointer to the Page if found; nullptr otherwise.
     */
    Page* get_obj(const std::string x);
};
