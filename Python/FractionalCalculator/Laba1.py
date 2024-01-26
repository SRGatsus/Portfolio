from MultiplicationLargeNumbers import *
import re

MassDecimal=[]

def EnterNumbers(text):
    flag = False
    entry = ""
    while(not flag):
        entry = input(text)
        if(re.match('^-?\d+$',entry) is None):
            print("Некорректный ввод.")
            continue
        flag = True
    return entry

def EnterPositiveNumber(text):
    flag = False
    entry = ""
    while(not flag):
        entry = input(text)
        if(re.match('^\d+$',entry) is None):
            print("Некорректный ввод.")
            continue
        flag = True
    return entry

def DefinSign(znak1, znak2):
    znak1 = znak1+znak2
    if(znak1 == "+-" or znak1 == "-+"):
        return "-"
    elif(znak1 == "--" or znak1 == "++"):
        return "+"

def MarkSelection(val):
    if(val[0] == "-"):
        return ["-",val[1::]]
    elif(val[0] == "+"):
        return ["+", val[1::]]
    return ["+",val]

def addFractions(count):
    index =0
    while(index<count):
        print(f'Заполните {index+1} дробь')
        wholePart = MarkSelection(EnterNumbers("Введите целую часть: "))
        denominator = EnterPositiveNumber("Введите знаменатель: ")
        numerator = EnterPositiveNumber("Введите числитель: ")
        numerator = wholePart[0]+summation(multiplication(wholePart[1],denominator),numerator)
        print(f'Дробь имеет ввид: {numerator}/{denominator}')
        flag = True
        while(flag):
            val = input("Все корректно(Y/N): ")
            val =val.lower()
            if(val=="y"):
                index +=1
                decimal = [numerator,denominator]
                MassDecimal.append(decimal)
                break
            elif(val == "n"):
                print("Введите заново")
                break
            else:
                print("Некорректный ввод.")

def printDecimal():
    i=1
    for x in MassDecimal:
        print(f'Дробь №{i}: {x[0]}/{x[1]}')
        i +=1

def SumOrSub(val1,val2,operat):
    if(operat =="+"):
        return summation(val1,val2)
    elif(operat == "-"):
        return difference(val1,val2)
    return ""

def ArithmeticOperation(index1,index2,operat):
    val1_1=MarkSelection(MassDecimal[index1][0])
    val1_2=MassDecimal[index1][1]
    val2_1=MarkSelection(MassDecimal[index2][0])
    val2_2=MassDecimal[index2][1]
    del MassDecimal[index2]
    del MassDecimal[index1]
    if(operat == "/"):
        MassDecimal.append([DefinSign(val1_1[0], val2_1[0]) + multiplication(val1_1[1], val2_2), multiplication(val2_1[1], val1_2)])
    elif(operat =="*"):
        MassDecimal.append([DefinSign(val1_1[0], val2_1[0]) + multiplication(val1_1[1], val2_1[1]), multiplication(val2_2, val1_2)])
    elif(operat == "+" or operat == "-"):
        denominator = multiplication(val1_2, val2_2)
        val1 =multiplication(val1_1[1], val2_2)
        val2 =multiplication(val2_1[1], val1_2)
        znak=DefinSign(operat, val2_1[0])
        if(val1_1[0] =="-"):
            val1 = SumOrSub(val2, val1, znak)
        else:
            val1 = SumOrSub(val1,val2,znak)
        if(val1 == "0"): denominator="0"
        MassDecimal.append([val1,denominator])
    else:
        print("Ошибка такой операции нет")

def Check(com, text ="Введите индекс дроби: "):
    count = 0
    if (len(com) == 0):
        count = EnterPositiveNumber(text)
    elif (com[0].isdigit()):
        count = int(com[0])
    else:
        print("Некорректный ввод.")
        count = EnterPositiveNumber(text)
    return int(count)

def __main__():
    while True:
        com = input(">>")
        com = com.split(" ")
        commanda = com[0].lower()
        del com[0]
        if(commanda == "help"):
            print("add: добавляет дроби в массив, количество дробей определяется параметром")
            print("operation: операция над дробя, 3 параметра - индекс первой дроби, индекс второй дроби, операция(+,-,/,*)")
            print("remove: удаляет дробь из массива, индекс дроби")
            print("print: вывод всех дробей")
        elif(commanda == "add"):
            count = Check(com, "Введите количество дробей: ")
            addFractions(count)
        elif(commanda == "oper"):
            count = Check(com, "Введите индекс первой дроби: ")
            while(count <= 0 or count > len(MassDecimal)):
                print("Некорректный ввод")
                count = int(EnterPositiveNumber("Введите индекс первой дроби: "))
            if(len(com)>0): del com[0]
            count2 = Check(com, "Введите индекс второй дроби: ")
            while(count2 <= 0 or count2 > len(MassDecimal)):
                print("Некорректный ввод")
                count2 = int(EnterPositiveNumber("Введите индекс второй дроби: "))
            if (len(com) > 0): del com[0]
            oper="" if(len(com)==0 or not com[0] in "+-/*") else com[0]
            while(not oper in ["+","-","/","*"]):
                oper = input("Введите операцию: ")
            ArithmeticOperation(count-1,count2-1,oper)
        elif(commanda == "remove"):
            count = Check(com)
            while(count <= 0 or count > len(MassDecimal)):
                print("Некорректный ввод")
                count = int(EnterPositiveNumber("Введите индекс дроби: "))
            del MassDecimal[count-1]
        elif(commanda == "print"):
            printDecimal()
        elif (commanda == "exit"):
            print("программа завершена....")
            break
        else:
            print("Команда не найдена")


if __name__ == '__main__':
    __main__()
