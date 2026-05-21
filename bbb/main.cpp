#include "utils.h"

extern lru_cache *reference_cache;

int main(int argc, char* argv[]) {
    Config cfg = parse_config(argc, argv);
    global_config = &cfg;

    reference_cache = new lru_cache(cfg.cache_capacity);

    auto test_requests = generate_test_sequence(
        cfg.test_sequence_size,
        cfg.unique_pages,
        cfg.skew_percent
    );

    compare_implementations(get_page, student_get_page, test_requests);

    delete reference_cache;
    return 0;
}