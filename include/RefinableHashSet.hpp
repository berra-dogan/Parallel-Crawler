#ifndef HASH_SET
#define HASH_SET

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
    this takes and stores pointers to Pages
*/
class RefinableHashSet {
private:
    std::vector<std::list<Page*>> table;
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
    RefinableHashSet(int capacity = 256) {
        table.resize(capacity);
        locks.resize(capacity);
        for (int i = 0; i < capacity; ++i) {
            locks[i] = std::make_unique<std::mutex>();
        }
        owner.store({std::thread::id(), false});
    }

    void acquire(const std::string& x) {
        OwnerInfo who;
        std::thread::id me = std::this_thread::get_id();
        while (true) {
            do {
                who = owner.load();
            } while (who.mark && who.owner_id != me);

            std::vector<std::unique_ptr<std::mutex>>* oldLocks = &locks;
            // Using the Hasher function associated with the data
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

    void release(const std::string& x) {
        locks[std::hash<std::string>{}(x) % locks.size()]->unlock();
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

                std::vector<std::list<Page*>> oldTable = table;
                table = std::vector<std::list<Page*>>(newCapacity);

                // in the textbook initializefrom() but we do it as we initalize the lists
                for (const auto& bucket : oldTable) {
                    for (const auto& item : bucket) {
                        table[std::hash<std::string>{}(item->url) % table.size()].push_back(item);
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


    bool add(Page* x) {
        acquire(x->url);
        std::list<Page*>& bucket = table[std::hash<std::string>{}(x->url) % table.size()];
        for (const Page* item : bucket) {
            if (item->url == x->url) {
                release(x->url);
                return false;
            }
        }
        bucket.push_back(x);
        size.fetch_add(1);
        release(x->url);

        // resize whenever the load is bigger than 0.75
        // resize outside the lock to avoid deadlock
        if (size.load() > 0.75 * table.size()) {
            resize();
        }

        return true;
    }

    bool remove(const std::string x) {
        acquire(x);

        std::list<Page*>& bucket = table[std::hash<std::string>{}(x) % table.size()];

        // use the past Equal checker function to find
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


    bool contains(const std::string x) {
        acquire(x);
        std::list<Page*>& bucket = table[std::hash<std::string>{}(x) % table.size()];
        for (const Page* item : bucket) {
            if (item->url == x) {
                release(x);
                return true;
            }
        }
        release(x);
        return false;
    }



    Page* get_obj(const std::string x) {
        acquire(x);
        std::list<Page*>& bucket = table[std::hash<std::string>{}(x) % table.size()];
        for (Page* item : bucket) {
            if (item->url == x) {
                release(x);
                return item;
            }
        }
        release(x);
        return NULL;
    }

};



#endif