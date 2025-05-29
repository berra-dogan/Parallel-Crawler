#include <vector>
#include <list>
#include <mutex>
#include <thread>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <functional>





/*
    Following the implementation from the textbook The Art of Multiprocessor Programming by Maurice Herlihy and Nir Shavit
    Chapter 13
*/
template <typename T, typename Hasher = std::hash<T>, typename Equal = std::equal_to<T>>
class RefinableHashSet {
private:
    std::vector<std::list<T>> table;
    std::vector<std::unique_ptr<std::mutex>> locks;

    // total number of items in the class
    // used to know when to resize
    std::atomic<size_t> size{0};


    // Simulate AtomicMarkableReference from the textbook
    // used to know which thread has the
    // put both in a struct to be able to modify both at same time using atomic
    // Used for resizing
    struct OwnerInfo {
        std::thread::id owner_id;
        bool mark;
    };

    // making it atomic like that can only be accessed by one thread  at a time
    std::atomic<OwnerInfo> owner;

public:
    RefinableHashSet(int capacity) {
        table.resize(capacity);
        locks.resize(capacity);
        for (int i = 0; i < capacity; ++i) {
            locks[i] = std::make_unique<std::mutex>();
        }
        owner.store({std::thread::id(), false});
    }

    void acquire(const T& x) {
        OwnerInfo who;
        std::thread::id me = std::this_thread::get_id();
        while (true) {
            do {
                who = owner.load();
            } while (who.mark && who.owner_id != me);

            std::vector<std::unique_ptr<std::mutex>>& oldLocks = &locks;
            // Using the Hasher function associated with the data
            std::mutex* oldLock = (*oldLocks)[Hasher{}(x) % oldLocks->size()].get();
            oldLock->lock();

            who = owner.load();
            if ((!who.mark || who.owner_id == me) && oldLocks == &locks) {
                return;
            } else {
                oldLock->unlock();
            }
        }
    }

    void release(const T& x) {
        locks[std::hash<T>{}(x) % locks.size()]->unlock();
    }

    void resize() {
        int oldCapacity = table.size();
        OwnerInfo expected = {std::thread::id(), false};
        std::thread::id me = std::this_thread::get_id();
        int newCapacity = 2 * oldCapacity;
        if (owner.compare_exchange_strong(expected, {me, true})) {
            try {
                if (table.size() != oldCapacity){
                    // already has been resized
                    return;
                } 
                quiesce();

                std::vector<std::list<T>> oldTable = table;
                table = std::vector<std::list<T>>(newCapacity);

                // in the textbook initializefrom() but we do it as we initalize the lists
                for (const auto& bucket : oldTable) {
                    for (const auto& item : bucket) {
                        table[std::hash<T>{}(item) % table.size()].push_back(item);
                    }
                }
                // initialize the locks
                locks = std::vector<std::unique_ptr<std::mutex>>(newCapacity);
                for (int j = 0; j < newCapacity; ++j) {
                    locks[j] = std::make_unique<std::mutex>();
                }

            } catch (...) {
                owner.store({std::thread::id(), false});
            }
            owner.store({std::thread::id(), false});
        }
    }

    void quiesce() {
        for (auto& lock : locks) {
            while (lock->try_lock() == false) {}
            lock->unlock();
        }
    }


    bool add(const T& x) {
        acquire(x);
        std::list<T>& bucket = table[std::hash<T>{}(x) % table.size()];
        for (const T& item : bucket) {
            if (Equal{}(item, x)) {
                release(x);
                return false;
            }
        }
        bucket.push_back(x);
        size.fetch_add(1);
        release(x);

        // resize whenever the load is bigger than 0.75
        // resize outside the lock to avoid deadlock
        if (size.load() > 0.75 * table.size()) {
            resize();
        }

        return true;
    }

    bool remove(const T& x) {
        acquire(x);

        std::list<T>& bucket = table[std::hash<T>{}(x) % table.size()];

        // use the past Equal checker function to find
        T it  = std::find_if(bucket.begin(), bucket.end(), [&](const T& item) {
            return Equal{}(item, x);
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


    bool contains(const T& x) {
        acquire(x);
        std::list<T>& bucket = table[std::hash<T>{}(x) % table.size()];
        for (const T& item : bucket) {
            if (item == x) {
                release(x);
                return true;
            }
        }
        release(x);
        return false;
    }

};
