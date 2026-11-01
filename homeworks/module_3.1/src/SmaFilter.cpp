#include "SmaFilter.hpp"

SmaFilter::SmaFilter(size_t window) : window_size(window), history(window, 0) {}

int32_t SmaFilter::process(int32_t new_sample) {
    sum -= history[index];
    history[index] = new_sample;
    sum += new_sample;

    index++;
    if (index >= window_size) {
        index = 0;
        buffer_full = true;
    }

    if (!buffer_full) {
        return index > 0 ? sum / index : 0;
    }
    return sum / window_size;
}
