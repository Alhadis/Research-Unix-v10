.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
mov %r0,%r8
call _exit; nop
L1:
ret; restore
.global _nested
.align 4
.proc 4
_nested:
cmp %r8,4; bge L6; nop
cmp %r9,114; be L9; nop
L6:
cmp %r8,1; bne L8; nop
cmp %r9,104; be L9; nop
cmp %r9,105; be L9; nop
L8:
cmp %r8,2; bne L3; nop
cmp %r9,111; be L9; nop
cmp %r9,121; bne L3; nop
L9:
mov %r9,%r8
L3:
L2:
retl; nop
.global _s
.align 4
.proc 4
_s:
L10:
retl; nop
.seg "data"
.global _Dy
.align 4; _Dy:.word 0
.skip 4
.global _Dz
.align 4; _Dz:.word 1
.skip 4
.seg "text"
.global _Dfunc
.align 4
.proc 4
_Dfunc:save %sp,-96,%sp
ret; restore
.global _f
.align 4
.proc 4
_f:
retl; nop
.global _f1
.align 4
.proc 4
_f1:
retl; nop
.global _f2
.align 4
.proc 4
_f2:save %sp,-96,%sp
ret; restore
.global _g
.align 4
.proc 4
_g:save %sp,-96,%sp
ret; restore
.global _h
.align 4
.proc 4
_h:save %sp,-96,%sp
ret; restore
.global _h1
.align 4
.proc 4
_h1:save %sp,-96,%sp
ret; restore
.global _h2
.align 4
.proc 4
_h2:
retl; nop
.seg "data"
.align 4; _21:.word 1
.seg "text"
.global _set1
.align 4
.proc 4
_set1:
retl; nop
.seg "data"
.align 4; _23:.word 2
.seg "text"
.global _set2
.align 4
.proc 4
_set2:
retl; nop
.align 4
.proc 4
_goo:
retl; nop
.global _sss
.align 4
.proc 4
_sss:
retl; nop
.seg "bss"
.align 4; .reserve _27,4
.seg "text"
.global _rrr
.align 4
.proc 4
_rrr:
retl; nop
.global _setstatic
.align 4
.proc 4
_setstatic:
retl; nop
.global _gx1
.align 4
.proc 4
_gx1:save %sp,-96,%sp
st %r24,[%fp+68]
st %r25,[%fp+72]
ret; restore
.global _ff1
.align 4
.proc 4
_ff1:mov %fp,%g7; mov %sp,%fp
st %r8,[%fp+68]
st %r9,[%fp+72]
retl; mov %g7,%fp
.global _gg1
.align 4
.proc 4
_gg1:
retl; nop
.global _hh1
.align 4
.proc 4
_hh1:
retl; nop
.global _cmp
.align 4
.proc 4
_cmp:save %sp,-96,%sp
ret; restore
.global _sort
.align 4
.proc 4
_sort:save %sp,-96,%sp
ret; restore
.global _onearg
.align 4
.proc 4
_onearg:save %sp,-96,%sp
ret; restore
.seg "bss"
.global _ss4
.align 4; .common _ss4,4
.align 4; .reserve _ss2,4
.align 4; .reserve _ss5,4
.global _ss3
.align 4; .common _ss3,4
.align 4; .reserve _ss1,4
.align 4; .reserve _yy,4
.global _z
.align 4; .common _z,4
.global _y
.align 4; .common _y,4
.global _x
.align 4; .common _x,4
.global _b
.align 4; .common _b,4
.global _a
.align 4; .common _a,4
