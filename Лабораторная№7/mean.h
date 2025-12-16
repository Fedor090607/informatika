#pragma once
#ifdef _WIN32
#ifdef STDMODULE_EXPORTS
#define STDMODULE_API __declspec(dllexport)
#else
#define STDMODULE_API __declspec(dllimport)
#endif
#else
#define STDMODULE_API __attribute__((visibility("default")))
#endif
#include <cstddef>

extern "C" {
    STDMODULE_API float calculate_mean(float* array, size_t size, double* elapsed_time_ms);
}

