#pragma once
#include <vector>
#include <cstdint>
#include <cstddef>

class SmaFilter {
private:
    size_t window_size;
    std::vector<int32_t> history;
    size_t index = 0;
    int32_t sum = 0;
    bool buffer_full = false;

public:
    explicit SmaFilter(size_t window);
    int32_t process(int32_t new_sample);
};
