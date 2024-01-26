from tkinter import *
import tkinter.messagebox as mb
import re
from MultiplicationLargeNumbers import *

MassDecimal = []
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
    del MassDecimal[index1]
    if(index2 == 1):
        del MassDecimal[0]
    else:
        del MassDecimal[index2]
    if(operat == ":"):
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

def Cal():
    global MassDecimal
    MassDecimal.clear()
    text = textEnt.get(1.0, END)
    text  = text.replace("\r", "")
    text  = text.replace("\n", "")
    if(text == ""):
        mb.showinfo("info","Вы ничего не ввели")
    arr1 = [el.group() for el in re.finditer("(\d+)?[(][+-]?[0-9]+[:*+-][0-9]*[)]",text)]
    arr2 = (re.sub("(\d+)?[(][+-]?[0-9]+[:*+-][0-9]*[)]","",text)).replace(" ","")
    for mult in arr2:
        if(not mult in ["*","-",":","+"]):
            msg = "Ошибка в вводе"
            mb.showerror("Ошибка", msg)
            return
    s = ""
    index = 0
    for num in arr1:
        arrNum = num.split("(")
        arrNum[1] = arrNum[1][0:len(arrNum[1])-1]
        wholePart = arrNum[0] if not arrNum[0]== "" else "0"
        x = re.split("(?<=([0-9]))([*]|[-]|[+]|[:])",arrNum[1])
        denominator = x[3]
        numerator = MarkSelection(x[0])
        print(wholePart,denominator,numerator,x)
        numerator = numerator[0] + summation(multiplication(wholePart, denominator), numerator[1])
        s += f'({numerator}:{denominator})'
        if(index<len(arr2)):
            s+= arr2[index]
            index+=1
        decimal = [numerator, denominator]
        MassDecimal.append(decimal)
    textEnt.delete('0.0', END)
    textEnt.insert('0.0', s)
    index1 = 0
    index2 = 1
    print(MassDecimal)
    for mult in arr2:
        ArithmeticOperation(index1,index2,mult)
        index1 = len(MassDecimal)-1
        index2 = 0
    print(f'Дробь имеет ввид: {MassDecimal[0][0]}/{MassDecimal[0][1]}')
    s= f'Ответ: ({MassDecimal[0][0]}:{MassDecimal[0][1]})'
    result['state'] = NORMAL
    result.delete('0.0', END)
    result.insert('0.0', s)
    result['state'] = DISABLED


window = Tk()
window.title("Арифметический калькулятор дробей")
window.geometry('600x750')
btnCal = Button(window, text="Посчитать", command=Cal)
btnCal.grid(column=3, row=1,padx=(10,0),pady=(10,0))
info = Label(window, text="Формат ввода: 1233213(+3213:213123) * (-3213/213123):1233213(3213:213123)....")
info.grid(column=0, row=1,columnspan=3,padx=(10,0),pady=(10,0))
textEnt = Text(window, undo = True, height = 20, width = 70)
textEnt.grid(column=0, row=2,pady=(10,0),padx=(10,0), columnspan=4,rowspan = 9)
LabelResult = Label(window, text="Ответ: ")
LabelResult.grid(column=0, row=12,padx=(10,0),columnspan=1)
result = Text(window, height = 20, width = 70,state='disabled')
result.grid(column=0, row=13,pady=(10,0),padx=(10,0), columnspan=4,rowspan = 4)

window.mainloop()
