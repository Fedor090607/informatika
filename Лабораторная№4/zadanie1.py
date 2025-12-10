def MultiplyByMax(arr):
    max_val = max(arr)
    return [x * max_val for x in arr]
print("задание1: умножение элементов массивов на их макс значения")
RazmerMass = 5
A = []
B = []

print(f"\nвведите {RazmerMass} чисел для массива A:")
for i in range(RazmerMass):
    a = input(f"введите {i+1}-е число: ")
    A.append(int(a))

print(f"\nвведите {RazmerMass} чисел для массива B:")
for i in range(RazmerMass):
    b = input(f"введите {i+1}-е число: ")
    B.append(int(b))
A_transformed = MultiplyByMax(A)
B_transformed = MultiplyByMax(B)

print("\nрезультаты преобразования:")
print(f"исходный массив A: {A}")
print(f"преобразованный массив A: {A_transformed}")
print(f"исходный массив B: {B}")
print(f"преобразованный массив B: {B_transformed}")
