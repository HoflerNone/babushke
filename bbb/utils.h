#pragma once
#include <list>
#include <unordered_map>
#include <functional>
#include <vector>
#include <cstring>
#include <string>
#include <random>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <thread>

class lru_cache;

extern lru_cache* reference_cache;


struct page {
    int index;
    char data[60];
};

void slow_get_page(int n, page* p);
void get_page(int id, page* p);
void student_get_page(int id, page* p);


struct Config {
    int cache_capacity = 16;
    int network_delay_ms = 5;
    int test_sequence_size = 200;
    int unique_pages = 32;
    int skew_percent = 80;
};

extern Config* global_config;

Config parse_config(int argc, char* argv[]);

page generate_page_content(int id);

double measure_time(std::function<void()> workload);

std::vector<int> generate_test_sequence(int total_requests, int unique_pages, int skew);

void run_benchmark(std::function<void(int, page*)> target_get_page, const std::vector<int>& requests);

void compare_implementations(
    std::function<void(int, page*)> reference,
    std::function<void(int, page*)> student,
    const std::vector<int>& requests);

class lru_cache {
public:
    explicit lru_cache(int capacity) : capacity_(capacity) {}

    bool try_get(int id, page* out);

    void put(int id, const page& p);

private:
    int capacity_;
    std::list<std::pair<int, page>> access_list_;
    std::unordered_map<int, std::list<std::pair<int, page>>::iterator> cache_;
};