import os
import random as rand
from termcolor import colored

def clear():
    os.system("cls")

def hand_input(arr):
    print("Ввод")
    _ = True
    counter = 1
    while _:
        try:
            elem = float(input(f"number{counter}: "))
            counter += 1
            arr.append(elem)
        except:
            _ = False
            counter -= 1
    clear()
    print(f"Вы ввели {counter} чисел")

def random_input(arr):
    num = int(input("Сколько чисел генерировать?"))
    for i in range(num):
        arr.append(round((rand.random() * 100) - 30, 3))
    clear()

def file_input(arr):
    try:
        name = input("Введите имя текстового файла: ")
        file = open(name)
        line = file.read()
        return [float(x) for x in line.split()]
    except Exception as e:
        clear()
        print(e)
        return file_input(arr)

def task(arr):
    pos = 0
    f = lambda x: x >= 0
    while pos < len(arr) and f(arr[pos]):
        pos += 1
    if pos != len(arr):
        print(f"Первый отрицательный: {arr[pos]} под номером {pos+1}")
        s = 0
        str = ""
        for i in range(0, pos + 1):
            str += colored(f"{arr[i]}  ", "red")
        for i in range(pos + 1, len(arr)):
            str += colored(f"{arr[i]}  ", "green")
            s += abs(arr[i])
        print(str)
        print(f"sum: {s}")
    else:
        print("Нет отрицательных...")
        

def main():
    clear()
    _ = True
    while _:
        try:
            choice = int(input("1 - руками,\n2 - рандом,\n3 - файл:\n"))
            _ = False
            if choice not in (1,2,3):
                raise
        except:
            clear()
            _ = True
    clear()

    arr = []
    if choice == 1:
        hand_input(arr)
    elif choice == 2:
        random_input(arr)
    else:
        arr = file_input(arr)
    if len(arr):
        task(arr)
    else:
        print("Пустой массив!")
    print("-"*50)
    print()
    input("again?")
    clear()
    main()

if __name__ == "__main__":
    main()
