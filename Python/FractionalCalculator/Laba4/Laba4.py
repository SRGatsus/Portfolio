import random  
from time import perf_counter

COUNT = 20000 # Change this value depending on the speed of your computer

def randomStr():
    result = ""
    for d in range(COUNT):
        result += str(random.randint(1,9))
    return result

def summation(a:str,b:str):
    transfer = 0
    result = ""
    if (len(a) < len(b)):
        a, b = b, a
        b = "0"*(len(a) - len(b)) + b
    else:
        b = "0" * (len(a) - len(b)) + b
    iA = len(a) - 1
    iB = len(b) - 1
    while(iA>-1):
        intA=int(a[iA])
        intB=int(b[iB])
        sum=intA+intB+transfer
        intC=sum%10
        transfer = sum//10
        result +=str(intC)
        iA -= 1
        iB -= 1
    result +=str(transfer)
    iResult = 0
    i=0
    result = result[::-1]
    while(result[iResult] == "0" and iResult<len(result)-1):
        i += 1
        iResult +=1
    return result[i::]

def multiplication(a:str,b:str):
    transfer = 0
    result = "0"
    iA = len(a) - 1
    iB = len(b) - 1
    while (iA>-1):
        intA = int(a[iA])
        iB = len(b) - 1
        interimResult = "0"*((len(a)-1)-iA)
        transfer=0
        while(iB>-1):
            intB = int(b[iB])
            sum = intA*intB + transfer
            intC = sum % 10
            transfer = sum // 10
            interimResult += str(intC)
            iB -=1
        interimResult +=str(transfer)
        interimResult = interimResult[::-1]
        result = summation(result,interimResult)
        iA -=1
    iResult = 0
    i=0
    while(result[iResult] == "0" and iResult<len(result)-1):
        i += 1
        iResult +=1
    return result[i::]

from LibPython import fast_tanh
if __name__ == "__main__":
    a = randomStr()
    start = perf_counter()
    result =  multiplication(a,a)
    duration = perf_counter() - start
    print('{} took {:.3f} seconds\n\n'.format("Python", duration))
    start = perf_counter()
    result2 =  fast_tanh(a)
    duration = perf_counter() - start
    print('{} took {:.3f} seconds\n\n'.format("CPP", duration))
