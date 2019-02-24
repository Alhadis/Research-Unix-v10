.seg "text"
.global _main
.align 4
.proc 4
_main:
L1:
retl; nop
.global _f
.align 4
.proc 4
_f:save %sp,-96,%sp
call _f; nop
mov %r8,%r29
call _f; nop
add %r29,%r8,%r24
L2:
ret; restore
.global _f2
.align 4
.proc 4
_f2:save %sp,-96,%sp
call _f; nop
mov %r8,%r28
cmp %r24,%r0; be L6; nop
call _f; nop
mov %r8,%r29
ba L7; nop
L6:
set 1,%r29
L7:
add %r28,%r29,%r24
L4:
ret; restore
.global _f3
.align 4
.proc 4
_f3:save %sp,-104,%sp
mov %r0,%r29
mov %r0,%r28
mov %r0,%r27
mov %r0,%r26
mov %r0,%r23
mov %r0,%r22
mov %r0,%r21
mov %r0,%r20
st %r0,[%fp+-4]
st %r0,[%fp+-8]
mov %r25,%r19
add %r19,4,%r25
st %r24,[%fp+-12]
ld [%fp+-12],%r18
cmp %r18,%r0; be L11; nop
call _f; nop
st %r8,[%fp+-12]
L11:
ld [%fp+-12],%r18
st %r18,[%r19]
L9:
ret; restore
.global _f4
.align 4
.proc 4
_f4:save %sp,-96,%sp
mov %r0,%r29
mov %r0,%r28
mov %r0,%r27
mov %r0,%r26
mov %r0,%r25
mov %r0,%r24
set _i,%r22
ld [%r22],%r22
sll %r22,3,%r21
set _a,%r20
add %r21,%r20,%r20
ld [%r20],%f30; ld [%r20+4],%f31
set _b,%r20
add %r21,%r20,%r21
ld [%r21],%f28; ld [%r21+4],%f29
faddd %f30,%f28,%f26
set L17,%r21
ld [%r21],%f24; ld [%r21+4],%f25
fcmped %f26,%f24; nop; fbue L15; nop
cmp %r22,%r0; be L15; nop
fsubd %f30,%f28,%f30
fcmped %f30,%f24; nop; fbue L15; nop
set 1,%r23
ba L16; nop
L15:
mov %r0,%r23
L16:
set _i,%r22
st %r23,[%r22]
L13:
ret; restore
.global _f5
.align 4
.proc 4
_f5:save %sp,-96,%sp
set _k,%r29
ld [%r29],%r29
set _m,%r28
ld [%r28],%r28
mov %r28,%o1; call .mul,2; mov %r29,%o0; mov %o0,%r27
set _j,%r26
ld [%r26],%r26
mov %r28,%o1; call .mul,2; mov %r26,%o0; mov %o0,%r28
set _n,%r25
ld [%r25],%r25
mov %r25,%o1; call .mul,2; mov %r29,%o0; mov %o0,%r29
mov %r25,%o1; call .mul,2; mov %r26,%o0; mov %o0,%r26
sll %r27,3,%r27
set _A,%r25
ld [%r25],%r25
add %r27,%r25,%r27
ld [%r27],%f30; ld [%r27+4],%f31
sll %r28,3,%r28
add %r28,%r25,%r28
ld [%r28],%f28; ld [%r28+4],%f29
fmuld %f30,%f28,%f30
sll %r29,3,%r29
set _B,%r25
ld [%r25],%r25
add %r29,%r25,%r29
ld [%r29],%f28; ld [%r29+4],%f29
sll %r26,3,%r26
add %r26,%r25,%r26
ld [%r26],%f26; ld [%r26+4],%f27
fmuld %f28,%f26,%f28
faddd %f30,%f28,%f30
set _x,%r25
st %f30,[%r25]; st %f31,[%r25+4]
ld [%r27],%f30; ld [%r27+4],%f31
ld [%r26],%f28; ld [%r26+4],%f29
fmuld %f30,%f28,%f30
ld [%r29],%f28; ld [%r29+4],%f29
ld [%r28],%f26; ld [%r28+4],%f27
fmuld %f28,%f26,%f28
fsubd %f30,%f28,%f30
st %f30,[%r25]; st %f31,[%r25+4]
L18:
ret; restore
.seg "bss"
.global _x
.align 8; .common _x,8
.global _B
.align 4; .common _B,4
.global _A
.align 4; .common _A,4
.global _n
.align 4; .common _n,4
.global _m
.align 4; .common _m,4
.global _k
.align 4; .common _k,4
.global _j
.align 4; .common _j,4
.global _i
.align 4; .common _i,4
.global _b
.align 8; .common _b,80
.global _a
.align 8; .common _a,80
.seg "text"
.align 8; L17:.word 0x0,0x0
