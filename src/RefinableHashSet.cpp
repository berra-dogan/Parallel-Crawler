


#include "../include/RefinableHashSet.hpp"




RefinableHashSet::RefinableHashSet(int capacity) {
    table.resize(capacity);
    locks.resize(capacity);
    for (int i = 0; i < capacity; ++i) {
        locks[i] = std::make_unique<std::mutex>();
    }
    owner.store({std::thread::id(), false});
}

void RefinableHashSet::acquire(const std::string& x) {
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


void RefinableHashSet::release(const std::string& x) {
    locks[std::hash<std::string>{}(x) % locks.size()]->unlock();
}

void RefinableHashSet::resize() {
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

void RefinableHashSet::quiesce() {
    for (auto& lock : locks) {
        while (!lock->try_lock()) {}
        lock->unlock();
    }
}


bool RefinableHashSet::add(Page* x) {
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



bool RefinableHashSet::remove(const std::string x) {
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

bool RefinableHashSet::contains(const std::string x) {
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

Page* RefinableHashSet::get_obj(const std::string x) {
    //std::cout << "getting" << std::endl;
    acquire(x);
    //std::cout << "got" << std::endl;
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


    





