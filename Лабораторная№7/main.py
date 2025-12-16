import ctypes
import time
import random
from collections import namedtuple

try:
    cpp_lib = ctypes.CDLL('./lab_7.dll')
except FileNotFoundError as e:
    print(f"Ошибка: Не найден файл DLL. Убедитесь, что lab_7.dll находится в той же папке, что и этот скрипт.")
    print(f"Текущая ошибка: {e}")
    exit(1)

cpp_calculate_mean = cpp_lib.calculate_mean
cpp_calculate_mean.argtypes = [ctypes.POINTER(ctypes.c_float), ctypes.c_size_t, ctypes.POINTER(ctypes.c_double)]
cpp_calculate_mean.restype = ctypes.c_float

def python_calculate_mean(array):
    return sum(array) / len(array)

def generate_test_array(size):
    return [random.uniform(0.0, 100.0) for _ in range(size)]

def run_test(num_iterations, array_size):
    print(f"\nтест: {num_iterations} итераций, размер массива {array_size}")

    test_data_sets = [generate_test_array(array_size) for _ in range(num_iterations)]

    cpp_total_time_ms = 0.0
    cpp_results = []

    for data_set in test_data_sets:
        c_array = (ctypes.c_float * array_size)(*data_set)
        c_elapsed_time_ms = ctypes.c_double(0.0)

        cpp_mean = cpp_calculate_mean(c_array, array_size, ctypes.byref(c_elapsed_time_ms))

        cpp_results.append(cpp_mean)
        cpp_total_time_ms += c_elapsed_time_ms.value

    cpp_avg_time_sec = cpp_total_time_ms / 1000.0

    python_total_time_sec = 0.0
    python_results = []

    for data_set in test_data_sets:
        start_time = time.perf_counter()
        py_mean = python_calculate_mean(data_set)
        end_time = time.perf_counter()

        python_results.append(py_mean)
        python_total_time_sec += (end_time - start_time)

    if abs(cpp_results[0] - python_results[0]) < 1e-5:
        print(f"[OK] Результаты C++ ({cpp_results[0]:.6f}) и Python ({python_results[0]:.6f}) совпадают.")
    else:
        print(f"[ERROR] Результаты различаются! C++: {cpp_results[0]:.6f}, Python: {python_results[0]:.6f}")

    print(f"C++  общее время: {cpp_total_time_ms:.2f} мс, среднее за итерацию: {cpp_total_time_ms/num_iterations:.2f} мс")
    print(f"Python общее время: {python_total_time_sec:.2f} с, среднее за итерацию: {python_total_time_sec/num_iterations*1000:.2f} мс")

    return cpp_avg_time_sec, python_total_time_sec

if __name__ == "__main__":
    TestConfig = namedtuple('TestConfig', ['iterations', 'array_size'])
    test_configs = [
        TestConfig(10000, 1000),
        TestConfig(50000, 1000),
    ]

    print("=" * 60)
    print("СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ: C++ vs PYTHON")
    print("Задача: вычисление среднего арифметического по массиву")
    print("=" * 60)

    results_table = []

    for i, config in enumerate(test_configs, start=1):
        cpp_time, py_time = run_test(config.iterations, config.array_size)
        results_table.append({
            'test_num': i,
            'iterations': config.iterations,
            'cpp_time_sec': round(cpp_time, 2),
            'py_time_sec': round(py_time, 2)
        })

    print("ИТОГОВАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ")
    print("=" * 60)
    print(f"{'№ Теста':<10} {'Итерации':<12} {'C++ (с)':<10} {'Python (с)':<12} {'Ускорение (раз)':<15}")
    print("-" * 60)

    for row in results_table:
        speedup = row['py_time_sec'] / row['cpp_time_sec'] if row['cpp_time_sec'] > 0 else 0
        print(f"{row['test_num']:<10} {row['iterations']:<12} {row['cpp_time_sec']:<10.2f} {row['py_time_sec']:<12.2f} {speedup:<15.2f}")

    print("\nВЫВОД:")
    avg_speedup = sum(r['py_time_sec'] / r['cpp_time_sec'] for r in results_table) / len(results_table)
    print(f"C++ модуль в среднем в {avg_speedup:.2f} раз быстрее Python.")

