#include <list>
#include <vector>
#include <algorithm>

struct page {
    int index;
    char data[60];
};

void slow_get_page(int n, page *p);

const int max_size = 64;

static std::vector<int> future_req;
static size_t curr_idx = 0;

void set_belady_future(const std::vector<int>& future) {
    future_req = future;
    curr_idx = 0;
}

struct MyBeladyCache {
    std::list<page> pages;

    void get_page(int number, page* result) {
        auto it = pages.begin();
        for (; it != pages.end(); ++it) {
            if (it->index == number)
                break;
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
                auto next = std::find(future_req.begin() + curr_idx, future_req.end(), p->index);
                
                if (next == future_req.end()) {
                    evict = p;
                    break;
                }
                
                size_t pos = next - future_req.begin();
                if (pos > farthest) {
                    farthest = pos;
                    evict = p;
                }
            }
            
            if (evict != pages.end())
                pages.erase(evict);
        }

        pages.push_front(page{});
        pages.front().index = number;
        slow_get_page(number, &pages.front());
        *result = pages.front();
        curr_idx++;
    }
};

MyBeladyCache my_belady_cache;

void student_get_page(int id, page *p) {
    my_belady_cache.get_page(id, p);
}