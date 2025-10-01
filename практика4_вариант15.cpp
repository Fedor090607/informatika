#include <iostream>
#include <cmath>
#include <locale>
int main() {
    setlocale(LC_ALL, "Russian");
    system("chcp 1251");

    float x, y;
    std::cout << "Введите X и Y: ";
    std::cin >> x >> y;
  
    float R = asinf(y / x);
    float S = exp(-3 * x) + cbrt(y);

    std::cout << "R = " << R << std::endl;
    std::cout << "S = " << S << std::endl;

    float C = std::fmax(R, S);

    std::cout << "Максимальное значение C = " << C << std::endl;
    system("pause");
    return 0;
}









