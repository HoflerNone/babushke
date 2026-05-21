#include "utils.h"
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

extern lru_cache *reference_cache;

const int max_size = 64;

static std::vector<int> future_req;
static size_t curr_idx = 0;
static std::vector<std::vector<size_t>> next_positions;

void set_belady_future(const std::vector<int>& future) {
    future_req = future;
    curr_idx = 0;
    int max_id = 0;
    for (int id : future) if (id > max_id) max_id = id;
    next_positions.assign(max_id + 1, std::vector<size_t>());
    for (size_t i = 0; i < future.size(); ++i) {
        next_positions[future[i]].push_back(i);
    }
}

struct MyBeladyCache {
    std::list<page> pages;

    void get_page(int number, page* result) {
        auto it = pages.begin();
        for (; it != pages.end(); ++it) {
            if (it->index == number) break;
        }

        if (it != pages.end()) {
            page tmp = *it;
            pages.erase(it);
            pages.push_front(tmp);
            *result = pages.front();
            curr_idx++;
            return;
        }

        if (pages.size() >= max_size) {
            auto evict = pages.end();
            size_t farthest = 0;
            for (auto p = pages.begin(); p != pages.end(); ++p) {
                const auto& pos = next_positions[p->index];
                auto nit = std::upper_bound(pos.begin(), pos.end(), curr_idx);
                size_t next = (nit == pos.end()) ? SIZE_MAX : *nit;
                if (next > farthest) {
                    farthest = next;
                    evict = p;
                }
            }
            if (evict != pages.end()) pages.erase(evict);
        }

        pages.push_front(page{});
        pages.front().index = number;
        slow_get_page(number, &pages.front());
        *result = pages.front();
        curr_idx++;
    }
};

MyBeladyCache my_belady_cache;

void my_student_belady_get_page(int id, page *p) {
    my_belady_cache.get_page(id, p);
}

int main(int argc, char* argv[]) {
    Config cfg = parse_config(argc, argv);
    global_config = &cfg;

    reference_cache = new lru_cache(cfg.cache_capacity);

    auto test_requests = generate_test_sequence(cfg.test_sequence_size, cfg.unique_pages, cfg.skew_percent);
    set_belady_future(test_requests);

    compare_implementations(get_page, my_student_belady_get_page, test_requests);

    delete reference_cache;
    return 0;
}