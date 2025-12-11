#include <iostream>
using namespace std;
const int RAZMER = 5;
void printArray(const int* arr, int size) {
    for (int i = 0; i < size; i++) {
        cout << *(arr + i) << " ";
    }
    cout << endl;
}
int findMax(const int* arr, int size) {
    int max = *arr;
    for (int i = 1; i < size; i++) {
        if (*(arr + i) > max) {
            max = *(arr + i);
        }
    }
    return max;
}
void multiplyArray(int* arr, int size, int multiplier) {
    for (int i = 0; i < size; i++) {
        *(arr + i) *= multiplier;
    }
}
void copyArray(const int* source, int* destination, int size) {
    for (int i = 0; i < size; i++) {
        *(destination + i) = *(source + i);
    }
}
int main() {
    setlocale(LC_ALL, "Russian");
    int arrayA[RAZMER];
    int arrayB[RAZMER];
    int originalA[RAZMER];
    int originalB[RAZMER];
    cout << "введите 5 чисел для массива A:" << endl;
    for (int i = 0; i < RAZMER; i++) {
        cout << "введите " << i + 1 << "-е число: ";
        cin >> *(arrayA + i);
    }
    cout << "\nвведите 5 чисел для массива B:" << endl;
    for (int i = 0; i < RAZMER; i++) {
        cout << "введите " << i + 1 << "-е число: ";
        cin >> *(arrayB + i);
    }
    copyArray(arrayA, originalA, RAZMER);
    copyArray(arrayB, originalB, RAZMER);
    int maxA = findMax(arrayA, RAZMER);
    int maxB = findMax(arrayB, RAZMER);
    multiplyArray(arrayA, RAZMER, maxA);
    multiplyArray(arrayB, RAZMER, maxB);
    cout << "\nисходный массив A: ";
    printArray(originalA, RAZMER);
    cout << "преобразованный массив A (умножен на " << maxA << "): ";
    printArray(arrayA, RAZMER);
    cout << "\nисходный массив B: ";
    printArray(originalB, RAZMER);
    cout << "преобразованный массив B (умножен на " << maxB << "): ";
    printArray(arrayB, RAZMER);
    return 0;
}
