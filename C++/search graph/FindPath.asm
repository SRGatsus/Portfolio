.MODEL FLAT, STDCALL
.DATA
.CODE
_FindPath PROC endmatr:dword, begin:dword, a:ptr dword, weight:dword, d:ptr dword, ver:ptr dword, k:dword

START_WHILE:

mov eax, [endmatr]		; загрузка в регистр eax значения конечного пункта - endmatr
cmp eax, [begin]		; сравниваем конечный и начальный пункт
je END_WHILE			; если они уже равны, то цикл while не выполняется. Переход в конец цикла 

FOR_START:

mov esi, 0				; в esi - заносим ноль. (i)

FOR_LOOP:				; метка для возврата назад в цикле for

; =========== тело цикла for ============

; if (a[i][end] != 0)   // если связь есть

mov ebx, [a]			; адрес начала массива
mov ecx, [endmatr]		; в ecx адрес конечного элемента поиска ----------------------- в eax уже адрес конечного элемента!!!

push eax				; сохраняем значение eax в стеке, чтобы потом вернуть
mov eax, 64				; в регистр eax - число 64 -> 4 байта * 16 столбцов матрицы
mul esi					; умножаем esi на eax -> результат в eax
add ebx, eax			; прибавляем к начальному адресу массива a - смещение

pop eax

mov edx, dword ptr [ebx+ecx*4] ; записываем в edx значение ячейки массива a[i][end]
cmp edx, 0				; сравнение ячейки двумерного массива с нулем - ЗДЕСЬ может быть ошибка!!!
je ENDIF1				; если равно нулю, то перемещение за пределы IF, если не равно, то выполняется тело if

; =========== тело if 1============

push eax

;int temp = weight - a[i][end]; // определяем вес пути из предыдущей вершины

mov eax, [weight]
sub eax, edx		; eax - a[i][end];
mov edi, eax		; temp в edi

pop eax

; if (temp == d[i])

mov ebx, [d]		; адрес начала массива d
mov ecx, [ebx+esi*4]; значение ячейки d[i]
cmp edi, ecx		; if (temp == d[i])
jne ENDIF2

; =========== тело if 2============


mov [weight], edi	; weight = temp

mov [endmatr], esi	 ; end = i

;ver[k] = i + 1

mov ebx, [ver]
mov ecx, esi
inc ecx
mov edx, [k]
mov [ebx+edx*4], ecx

;k++

inc edx
mov [k], edx

; =========== тело if 2============

ENDIF2:

; =========== тело if 1============

ENDIF1:

; =========== тело цикла for ============

inc esi
cmp esi, 16				; количество итераций для цикла for - 16 вершин максимум, поэтому сравниваем с 16
jnz FOR_LOOP			; если не равно 16,то переход к FOR_LOOP для итерации
END_FOR:				; если esi = 16, то выход из цикла for. Все итерации выполнены.
jmp START_WHILE			; возврат к началу цикла while
END_WHILE:				; конец цикла while

ret
_FindPath ENDP
END