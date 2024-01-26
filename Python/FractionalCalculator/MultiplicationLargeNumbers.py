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

def difference(a:str,b:str):
    transfer = 0
    result = ""
    sign = ""

    if (len(a) <len(b) or (len(a)==len(b) and int(a[0])<int(b[0]))):
        a, b = b,a
        sign="-"
        b = "0"*(len(a) - len(b)) + b
    else:
        b = "0" * (len(a) - len(b)) + b
    iA = len(a) - 1
    while (iA > -1):
        intA = int(a[iA])
        intB = int(b[iA])
        if ((intA - intB)<=0):
            result += str(10 +intA - intB+transfer)
            transfer = -1
        else:
            result += str(intA - intB +transfer)
            transfer = 0
        iA -= 1
    iResult = 0
    i=0
    result = result[::-1]
    while(iResult<len(result) and result[iResult] == "0"):
        i += 1
        iResult +=1
    result = result[i::]
    if(result == ""):
        return "0"
    else:
        return sign+result

def __main__():
    #['5501834426610619167279671950130991725550114562014', '3628803217234414110847706768347841928480']]
    #['+97897', '89798798']
    #[['+538613084861899784619178044901973696956179565277484558', '325862167086183271388362828854100651071505967040']]
    print(summation("1","23"))
    var1 = multiplication("48845815413382601343308042688","80980890")
    var2 = multiplication("12515749302707","44810611704000957643805924686032")
    print(difference( var1, var2))
if __name__ == '__main__':
    __main__()