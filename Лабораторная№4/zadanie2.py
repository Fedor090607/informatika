def TolkoIzA_MensheT(S1, S2, t):
    Result = []
    for a in S1:
        if a not in S2 and a < t:
            Result.append(a)
    return Result
S1 = []
S2 = []
while True:
    try:
        Size1 = int(input("\nсколько чисел будет в множестве A: "))
        break
    except ValueError:
        print("ошибка, введите ЦЕЛОЕ ЧИСЛО")
print(f"введите {Size1} чисел для множества A:")
for i in range(Size1):
    while True:
        try:
            N = int(input())
            S1.append(N)
            break
        except ValueError:
            print("ошибка, введите целые ЧИСЛА")

while True:
    try:
        Size2 = int(input("\nсколько чисел будет в множестве B: "))
        break
    except ValueError:
        print("ошибка, введите ЦЕЛОЕ ЧИСЛО")
print(f"введите {Size2} чисел для множества B:")
for i in range(Size2):
    while True:
        try:
            N = int(input())
            S2.append(N)
            break
        except ValueError:
            print("ошибка, введите целые ЧИСЛА")

while True:
    try:
        t = int(input("\nвведите значение порога t: "))
        break
    except ValueError:
        print("ошибка, введите ЦЕЛОЕ ЧИСЛО")

Itogi = TolkoIzA_MensheT(S1, S2, t)

print("\nрезультат")
if len(Itogi) == 0:
    print("нет элементов, которые только в A и меньше t")
else:
    print(f"элементы только из A и меньше {t}: {Itogi}")






