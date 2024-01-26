.MODEL FLAT, C
.data

.CODE

mul_v PROC mas:DWORD, n: DWORD, a: DWORD, b: DWORD, node: DWORD, pos: DWORD,

       push    ebp
        mov     ebp, esp
        push    ebx
        sub     esp, 32
        mov     DWORD PTR [ebp-32], 1000000
        mov     DWORD PTR [ebp-8], 0
        mov     DWORD PTR [ebp-12], 0
        mov     DWORD PTR [ebp-16], 0
L3:
        mov     eax, DWORD PTR [ebp-16]
        cmp     eax, DWORD PTR [ebp+12]
        jge     L2
        mov     eax, DWORD PTR [ebp-16]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     edx, eax
        mov     eax, DWORD PTR [ebp-32]
        mov     DWORD PTR [edx], eax
        mov     eax, DWORD PTR [ebp-16]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+24]
        add     eax, edx
        mov     DWORD PTR [eax], 0
        inc     DWORD PTR [ebp-16]
        jmp     L3
L2:
        mov     eax, DWORD PTR [ebp+16]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     eax, edx
        mov     DWORD PTR [eax], 0
        mov     DWORD PTR [ebp-20], 0
L11:
        mov     eax, DWORD PTR [ebp+12]
        dec     eax
        cmp     eax, DWORD PTR [ebp-20]
        jle     L4
        mov     eax, DWORD PTR [ebp-32]
        mov     DWORD PTR [ebp-8], eax
        mov     DWORD PTR [ebp-24], 0
L7:
        mov     eax, DWORD PTR [ebp-24]
        cmp     eax, DWORD PTR [ebp+12]
        jge     L5
        mov     eax, DWORD PTR [ebp-24]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+24]
        add     eax, edx
        mov     eax, DWORD PTR [eax]
        test    eax, eax
        jne     L6
        mov     eax, DWORD PTR [ebp-24]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     eax, edx
        mov     eax, DWORD PTR [eax]
        cmp     eax, DWORD PTR [ebp-8]
        jge     L6
        mov     eax, DWORD PTR [ebp-24]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     eax, edx
        mov     eax, DWORD PTR [eax]
        mov     DWORD PTR [ebp-8], eax
        mov     eax, DWORD PTR [ebp-24]
        mov     DWORD PTR [ebp-12], eax
L6:
        inc     DWORD PTR [ebp-24]
        jmp     L7
L5:
        mov     eax, DWORD PTR [ebp-12]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+24]
        add     eax, edx
        mov     DWORD PTR [eax], 1
        mov     DWORD PTR [ebp-28], 0
L10:
        mov     eax, DWORD PTR [ebp-28]
        cmp     eax, DWORD PTR [ebp+12]
        jge     L8
        mov     eax, DWORD PTR [ebp-28]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+24]
        add     eax, edx
        mov     eax, DWORD PTR [eax]
        test    eax, eax
        jne     L9
        mov     edx, DWORD PTR [ebp-12]
        mov     eax, edx
        sal     eax, 2
        add     eax, edx
        sal     eax, 3
        mov     edx, eax
        mov     eax, DWORD PTR [ebp+8]
        add     edx, eax
        mov     eax, DWORD PTR [ebp-28]
        mov     eax, DWORD PTR [edx+eax*4]
        test    eax, eax
        jle     L9
        mov     eax, DWORD PTR [ebp-12]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     eax, edx
        mov     eax, DWORD PTR [eax]
        cmp     eax, DWORD PTR [ebp-32]
        je      L9
        mov     eax, DWORD PTR [ebp-12]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     eax, edx
        mov     ecx, DWORD PTR [eax]
        mov     edx, DWORD PTR [ebp-12]
        mov     eax, edx
        sal     eax, 2
        add     eax, edx
        sal     eax, 3
        mov     edx, eax
        mov     eax, DWORD PTR [ebp+8]
        add     edx, eax
        mov     eax, DWORD PTR [ebp-28]
        mov     eax, DWORD PTR [edx+eax*4]
        lea     edx, [ecx+eax]
        mov     eax, DWORD PTR [ebp-28]
        lea     ecx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     eax, ecx
        mov     eax, DWORD PTR [eax]
        cmp     edx, eax
        jge     L9
        mov     eax, DWORD PTR [ebp-28]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        lea     ecx, [edx+eax]
        mov     eax, DWORD PTR [ebp-12]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     eax, edx
        mov     ebx, DWORD PTR [eax]
        mov     edx, DWORD PTR [ebp-12]
        mov     eax, edx
        sal     eax, 2
        add     eax, edx
        sal     eax, 3
        mov     edx, eax
        mov     eax, DWORD PTR [ebp+8]
        add     edx, eax
        mov     eax, DWORD PTR [ebp-28]
        mov     eax, DWORD PTR [edx+eax*4]
        add     eax, ebx
        mov     DWORD PTR [ecx], eax
L9:
        inc     DWORD PTR [ebp-28]
        jmp     L10
L8:
        inc     DWORD PTR [ebp-20]
        jmp     L11
L4:
        mov     eax, DWORD PTR [ebp+20]
        lea     edx, [0+eax*4]
        mov     eax, DWORD PTR [ebp+28]
        add     eax, edx
        mov     eax, DWORD PTR [eax]
        cmp     eax, DWORD PTR [ebp-32]
        jne     L12
        mov     al, 0
        jmp     L13
L12:
        mov     al, 1
L13:
        add     esp, 32
        pop     ebx
        pop     ebp
        ret
mul_v ENDP

END