.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
cmp %r24,1; bg L2; nop
set L4,%r26
ld [%r26],%f31
st %f31,[%fp+-4]
ba L3; nop
L2:
ld [%r25+4],%r8
call _atof; nop
set L5,%r26
ld [%r26],%f31
fdivs %f0,%f31,%f31
st %f31,[%fp+-4]
L3:
mov %r0,%r29
L6:
mov %r29,%r26
add %r26,1,%r29
set L4,%r23
ld [%r23],%f31
sll %r26,2,%r26
set _f,%r23
st %f31,[%r26+%r23]
L7:
cmp %r29,127; ble L6; nop
mov %r0,%r28
ba L11; nop
L10:
sll %r27,2,%r26
set _f,%r23
add %r26,%r23,%r26
ld [%r26],%f31
set L13,%r23
ld [%r23],%f30
fadds %f31,%f30,%f31
st %f31,[%r26]
add %r28,1,%r28
L11:
call _getchar; nop
mov %r8,%r27
cmp %r8,-1; bne L10; nop
set L14,%r8
call _printf; nop
mov %r0,%r29
L15:
sll %r29,2,%r26
set _f,%r23
ld [%r26+%r23],%f31
set L4,%r26
ld [%r26],%f30
fcmpes %f31,%f30; nop; fbue L19; nop
st %r28,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fdivs %f31,%f30,%f31
ld [%fp+-4],%f30
fcmpes %f31,%f30; nop; fbul L19; nop
cmp %r29,32; bg L21; nop
set L23,%r8
mov %r29,%r9
call _printf; nop
ba L22; nop
L21:
set L24,%r8
mov %r29,%r9
call _printf; nop
L22:
set L25,%r8
set L5,%r26
ld [%r26],%f31
sll %r29,2,%r26
set _f,%r23
ld [%r26+%r23],%f30
fmuls %f31,%f30,%f31
st %r28,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fdivs %f31,%f30,%f31
fstod %f31,%f30
st %f30,[%sp+4*1+68]
st %f31,[%sp+4*2+68]
ld [%sp+4*1+68],%o1
ld [%sp+4*2+68],%o2
call _printf; nop
L19:
L16:
add %r29,1,%r29
cmp %r29,127; ble L15; nop
mov %r0,%r24
L1:
ret; restore
.seg "bss"
.global _f
.align 4; .common _f,512
.seg "text"
L25:.ascii "\011%.1f\012\000"
L24:.byte 37,99,0
L23:.ascii "%03o\000"
L14:.ascii "char\011freq\012\000"
.align 4; L13:.word 0x3f800000
.align 4; L5:.word 0x42c80000
.align 4; L4:.word 0x0
