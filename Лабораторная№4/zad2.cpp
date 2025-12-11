#include <iostream>
#include <set>
using namespace std;
set<int> findUniqueElementsLessThanT(const set<int>& setA, const set<int>& setB, int t) {
    set<int> result;
    for (int x : setA) {
        if (setB.find(x) == setB.end() && x < t) {
            result.insert(x);
        }
    }
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");

    int sizeA;
    cout << "Введите количество элементов в множестве A: ";
    cin >> sizeA;

    set<int> setA;
    cout << "Введите элементы множества A:\n";
    while (setA.size() < sizeA) {
        int x;
        cin >> x;
        setA.insert(x);
    }

    int sizeB;
    cout << "Введите количество элементов в множестве B: ";
    cin >> sizeB;

    set<int> setB;
    cout << "Введите элементы множества B:\n";
    while (setB.size() < sizeB) {
        int x;
        cin >> x;
        setB.insert(x);
    }

    int t;
    cout << "Введите значение t: ";
    cin >> t;

    set<int> result = findUniqueElementsLessThanT(setA, setB, t);

    cout << "Результат: ";
    if (result.empty()) {
        cout << "Нет элементов, которые только в A и меньше t\n";
    }
    else {
        for (int x : result) {
            cout << x << " ";
        }
        cout << endl;
    }

    return 0;
}

