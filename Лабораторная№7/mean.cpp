#include "pch.h"
#define STDMODULE_EXPORTS
#include "mean.h"  
#include <chrono>

STDMODULE_API float calculate_mean(float* array, size_t size, double* elapsed_time_ms) {

    auto start = std::chrono::high_resolution_clock::now();

    float sum = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        sum += array[i];
    }
    float mean = sum / static_cast<float>(size);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_ms = end - start;

    *elapsed_time_ms = duration_ms.count();

    return mean;
}

