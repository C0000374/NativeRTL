;
; Copyright (C) C0000374
;

.686p
.XMM
.model flat

.code
public __ftol2
__ftol2:      push  eax
              push  eax
              fstcw word ptr [esp]
              mov   ax, word ptr [esp]
              or    word ptr [esp], 0C00h
              fldcw word ptr [esp]
              mov   word ptr [esp], ax
              fistp dword ptr [esp + 4]
              fldcw word ptr [esp]
              mov   eax, dword ptr [esp + 4]
              xor   edx, edx
              pop   ecx
              pop   ecx
              retn

public __ftol2_sse
__ftol2_sse:  push      eax
              push      eax
              fstp      qword ptr [esp]
              cvttsd2si eax, qword ptr [esp]
              pop       ecx
              pop       ecx
              retn
end