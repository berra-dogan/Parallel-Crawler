#pragma once

#include <vector>
#include <list>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <functional>

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
    explicit RefinableHashSet(int capacity = 256) {
        table.resize(capacity);
        locks.resize(capacity);
        for (int i = 0; i < capacity; ++i) {
            locks[i] = std::make_unique<std::mutex>();
        }
        owner.store({std::thread::id(), false});
    }

    /**
     * @brief Acquires the lock associated with the hash of a string key.
     * Used internally to lock individual buckets.
     *
     * @param x The key whose bucket lock is to be acquired.
     */
    void acquire(const std::string& x) {
        OwnerInfo who;
        std::thread::id me = std::this_thread::get_id();
        while (true) {
            do {
                who = owner.load();
            } while (who.mark && who.owner_id != me);

            auto* oldLocks = &locks;
            std::mutex* oldLock = (*oldLocks)[std::hash<std::string>{}(x) % oldLocks->size()].get();
            oldLock->lock();

            who = owner.load();
            if ((!who.mark || who.owner_id == me) && oldLocks == &locks) {
                return;
            } else {
                oldLock->unlock();
            }
        }
    }

    /**
     * @brief Releases the lock associated with the hash of a string key.
     *
     * @param x The key whose bucket lock is to be released.
     */
    void release(const std::string& x) {
        locks[std::hash<std::string>{}(x) % locks.size()]->unlock();
    }

    /**
     * @brief Resizes the hash table when load factor exceeds a threshold.
     * Uses the lock refinement technique to prevent concurrent modifications.
     */
    void resize() {
        int oldCapacity = table.size();
        OwnerInfo expected = {std::thread::id(), false};
        std::thread::id me = std::this_thread::get_id();
        int newCapacity = 2 * oldCapacity;
        if (owner.compare_exchange_strong(expected, {me, true})) {
            try {
                if (int(table.size()) != oldCapacity) return; // Already resized

                quiesce();

                auto oldTable = table;
                table = std::vector<std::list<Page*>>(newCapacity);

                for (const auto& bucket : oldTable) {
                    for (const auto& item : bucket) {
                        table[std::hash<std::string>{}(item->url) % table.size()].push_back(item);
                    }
                }

                locks = std::vector<std::unique_ptr<std::mutex>>(newCapacity);
                for (int j = 0; j < newCapacity; ++j) {
                    locks[j] = std::make_unique<std::mutex>();
                }

            } catch (...) {
                owner.store({std::thread::id(), false});
                throw;
            }
            owner.store({std::thread::id(), false});
        }
    }

    /**
     * @brief Ensures all locks are temporarily idle.
     * Used before resizing to safely reallocate buckets.
     */
    void quiesce() {
        for (auto& lock : locks) {
            while (!lock->try_lock()) {}
            lock->unlock();
        }
    }

    /**
     * @brief Adds a new Page* to the set.
     * 
     * @param x Pointer to the Page object.
     * @return True if the page was added; false if it already existed.
     */
    bool add(Page* x) {
        acquire(x->url);
        auto& bucket = table[std::hash<std::string>{}(x->url) % table.size()];
        for (const auto* item : bucket) {
            if (item->url == x->url) {
                release(x->url);
                return false;
            }
        }
        bucket.push_back(x);
        size.fetch_add(1);
        release(x->url);

        if (size.load() > 0.75 * table.size()) {
            resize();
        }
        return true;
    }

    /**
     * @brief Removes a Page from the set by its URL.
     * 
     * @param x The URL of the Page to be removed.
     * @return True if the page was found and removed; false otherwise.
     */
    bool remove(const std::string x) {
        acquire(x);
        auto& bucket = table[std::hash<std::string>{}(x) % table.size()];
        auto it = std::find_if(bucket.begin(), bucket.end(), [&](const Page* item) {
            return item->url == x;
        });

        if (it != bucket.end()) {
            bucket.erase(it);
            size.fetch_sub(1);
            release(x);
            return true;
        }
        release(x);
        return false;
    }

    /**
     * @brief Checks if a Page with a given URL exists in the set.
     * 
     * @param x The URL to search for.
     * @return True if found; false otherwise.
     */
    bool contains(const std::string x) {
        acquire(x);
        auto& bucket = table[std::hash<std::string>{}(x) % table.size()];
        for (const auto* item : bucket) {
            if (item->url == x) {
                release(x);
                return true;
            }
        }
        release(x);
        return false;
    }

    /**
     * @brief Retrieves a pointer to the Page object with the given URL.
     * 
     * @param x The URL to search for.
     * @return Pointer to the Page if found; nullptr otherwise.
     */
    Page* get_obj(const std::string x) {
        acquire(x);
        auto& bucket = table[std::hash<std::string>{}(x) % table.size()];
        for (Page* item : bucket) {
            if (item->url == x) {
                release(x);
                return item;
            }
        }
        release(x);
        return nullptr;
    }
};
