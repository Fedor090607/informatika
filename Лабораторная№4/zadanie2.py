def TolkoIzAMensheT(S1, S2, t):
    return sorted({x for x in S1 - S2 if x < t})
while True:
    try:
        Size1 = int(input("cколько чисел будет в множестве A: "))
        break
    except ValueError:
        print("ошибка, введите ЦЕЛОЕ ЧИСЛО")

print(f"введите {Size1} чисел для множества A:")
S1 = set()
while len(S1) < Size1:
    try:
        N = int(input())
        S1.add(N)
    except ValueError:
        print("ошибка, введите целые ЧИСЛА")

while True:
    try:
        Size2 = int(input("cколько чисел будет в множестве B: "))
        break
    except ValueError:
        print("ошибка, введите ЦЕЛОЕ ЧИСЛО")

print(f"введите {Size2} чисел для множества B:")
S2 = set()
while len(S2) < Size2:
    try:
        N = int(input())
        S2.add(N)
    except ValueError:
        print("ошибка, введите целые ЧИСЛА")

while True:
    try:
        t = int(input("введите значение порога t: "))
        break
    except ValueError:
        print("ошибка, введите ЦЕЛОЕ ЧИСЛО")
Itogi = TolkoIzAMensheT(S1, S2, t)

print("результат:")
if not Itogi:
    print("нет элементов, которые только в A и меньше t")
else:
    print(f"элементы только из A и меньше {t}: {Itogi}")



