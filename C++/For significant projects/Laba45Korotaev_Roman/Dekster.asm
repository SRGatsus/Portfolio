.586
.MODEL FLAT, C
.CODE
mul_v PROC C
push ebp
  mov ebp, esp
  sub esp, 32
  mov DWORD PTR [ebp-24], 1000000 ;int big_num= 1000000;
  mov DWORD PTR [ebp-4], 0 ; int  minMas = 0;
  mov DWORD PTR [ebp-8], 0 ;	int index_min = 0;
  mov DWORD PTR [ebp-12], 0 ; int i = 0
L9:
  mov eax, DWORD PTR [ebp+12] ;n
  dec eax ;n- 1 
  cmp DWORD PTR [ebp-12], eax ; i >n-1 =>L2 
  jge L2
  mov eax, DWORD PTR [ebp-24]
  mov DWORD PTR [ebp-4], eax; minMas=big_num
  mov DWORD PTR [ebp-16], 0 ; int j = 0
L5:
  mov eax, DWORD PTR [ebp-16] ;j
  cmp eax, DWORD PTR [ebp+12] ; j >n = >L3
  jge L3
  mov edx, DWORD PTR [ebp-16];j
  mov eax, DWORD PTR [ebp+24];node
  add eax, edx; node[j]
  mov al, BYTE PTR [eax] ;
  xor eax, 1 
  test al, al ;проверка на 0
  je L4
  mov eax, DWORD PTR [ebp-16]
  lea edx, [0+eax*4]
  mov eax, DWORD PTR [ebp+28]
  add eax, edx ;pos[j]
  mov eax, DWORD PTR [eax]
  cmp DWORD PTR [ebp-4], eax ; pos[j] < minMas
  jle L4
  mov eax, DWORD PTR [ebp-16] 
  lea edx, [0+eax*4] 
  mov eax, DWORD PTR [ebp+28]
  add eax, edx
  mov eax, DWORD PTR [eax]
  mov DWORD PTR [ebp-4], eax ; minMas = pos[j]
  mov eax, DWORD PTR [ebp-16]
  mov DWORD PTR [ebp-8], eax ;index_min = j;
L4:
  inc DWORD PTR [ebp-16] ;j++
  jmp L5
L3:
  mov edx, DWORD PTR [ebp-8]
  mov eax, DWORD PTR [ebp+24]
  add eax, edx
  mov BYTE PTR [eax], 1 ; node[index_min] = true; 
  mov DWORD PTR [ebp-20], 0 ; int j = 0
L8:
  mov eax, DWORD PTR [ebp-20] 
  cmp eax, DWORD PTR [ebp+12] ;j < FGH
  jge L6
  mov eax, DWORD PTR [ebp-8] ;index_min
  lea edx, [0+eax*4]
  mov eax, DWORD PTR [ebp+28]
  add eax, edx ;pos[index_min]
  mov ecx, DWORD PTR [eax] ; pos[index_min]
  mov edx, DWORD PTR [ebp-12] ;i
  mov eax, edx 
  sal eax, 3
  sub eax, edx
  sal eax, 2
  mov edx, eax
  mov eax, DWORD PTR [ebp+8]; MassMatrix
  add edx, eax;MassMatrix[i]
  mov eax, DWORD PTR [ebp-20];j
  mov eax, DWORD PTR [edx+eax*4];MassMatrix[i][j]
  add eax, ecx
  mov DWORD PTR [ebp-28], eax sum
  mov edx, DWORD PTR [ebp-20];j
  mov eax, DWORD PTR [ebp+24];node
  add eax, edx
  mov al, BYTE PTR [eax]
  xor eax, 1
  test al, al ;!node[j]
  je L7
  mov edx, DWORD PTR [ebp-12]
  mov eax, edx
  sal eax, 3
  sub eax, edx
  sal eax, 2
  mov edx, eax
  mov eax, DWORD PTR [ebp+8]
  add edx, eax
  mov eax, DWORD PTR [ebp-20]
  mov eax, DWORD PTR [edx+eax*4]
  test eax, eax ;MassMatrix[i][j]>0
  jle L7
  mov eax, DWORD PTR [ebp-8]
  lea edx, [0+eax*4]
  mov eax, DWORD PTR [ebp+28]
  add eax, edx
  mov eax, DWORD PTR [eax]
  cmp DWORD PTR [ebp-24], eax; pos[index_min] != big_num
  je L7
  mov eax, DWORD PTR [ebp-20]
  lea edx, [0+eax*4]
  mov eax, DWORD PTR [ebp+28]
  add eax, edx
  mov eax, DWORD PTR [eax]
  cmp DWORD PTR [ebp-28], eax;Sum < pos[j]
  jge L7
  mov eax, DWORD PTR [ebp-20]
  lea edx, [0+eax*4]
  mov eax, DWORD PTR [ebp+28]
  add edx, eax
  mov eax, DWORD PTR [ebp-28]
  mov DWORD PTR [edx], eax  pos[j] = sum
L7:
  inc DWORD PTR [ebp-20]
  jmp L8
L6:
  inc DWORD PTR [ebp-12]
  jmp L9
L2:
  nop
  leave
  ret
  mul_v ENDP
END