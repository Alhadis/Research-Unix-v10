.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-160,%sp
set 1,%r29
set _c,%r28
stb %r29,[%r28]
ldsb [%r28],%r29
set _s,%r28
sth %r29,[%r28]
set _i,%r27
st %r29,[%r27]
mov %r29,%r26
set _C,%r25
stb %r26,[%r25]
set _S,%r24
sth %r26,[%r24]
set _I,%r23
st %r26,[%r23]
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f29
set _f,%r26
st %f29,[%r26]
set _d,%r22
st %f30,[%r22]; st %f31,[%r22+4]
set L2,%r8
mov %r29,%r9
ldsh [%r28],%r10
ld [%r27],%r11
ldub [%r25],%r12
lduh [%r24],%r13
ld [%r23],%r29
st %r29,[%sp+4*6+68]
ld [%r26],%f31
fstod %f31,%f30
st %f30,[%sp+4*7+68]
st %f31,[%sp+4*8+68]
ld [%r22],%f30; ld [%r22+4],%f31
st %f30,[%sp+4*9+68]
st %f31,[%sp+4*10+68]
call _printf; nop
set 2,%r29
set _s,%r28
sth %r29,[%r28]
ldsh [%r28],%r29
set _c,%r28
stb %r29,[%r28]
set _i,%r27
st %r29,[%r27]
mov %r29,%r26
set _C,%r25
stb %r26,[%r25]
set _S,%r24
sth %r26,[%r24]
set _I,%r23
st %r26,[%r23]
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f29
set _f,%r26
st %f29,[%r26]
set _d,%r22
st %f30,[%r22]; st %f31,[%r22+4]
set L2,%r8
ldsb [%r28],%r9
mov %r29,%r10
ld [%r27],%r11
ldub [%r25],%r12
lduh [%r24],%r13
ld [%r23],%r29
st %r29,[%sp+4*6+68]
ld [%r26],%f31
fstod %f31,%f30
st %f30,[%sp+4*7+68]
st %f31,[%sp+4*8+68]
ld [%r22],%f30; ld [%r22+4],%f31
st %f30,[%sp+4*9+68]
st %f31,[%sp+4*10+68]
call _printf; nop
set 3,%r29
set _i,%r28
st %r29,[%r28]
ld [%r28],%r29
set _c,%r28
stb %r29,[%r28]
set _s,%r27
sth %r29,[%r27]
mov %r29,%r26
set _C,%r25
stb %r26,[%r25]
set _S,%r24
sth %r26,[%r24]
set _I,%r23
st %r26,[%r23]
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f29
set _f,%r26
st %f29,[%r26]
set _d,%r22
st %f30,[%r22]; st %f31,[%r22+4]
set L2,%r8
ldsb [%r28],%r9
ldsh [%r27],%r10
mov %r29,%r11
ldub [%r25],%r12
lduh [%r24],%r13
ld [%r23],%r29
st %r29,[%sp+4*6+68]
ld [%r26],%f31
fstod %f31,%f30
st %f30,[%sp+4*7+68]
st %f31,[%sp+4*8+68]
ld [%r22],%f30; ld [%r22+4],%f31
st %f30,[%sp+4*9+68]
st %f31,[%sp+4*10+68]
call _printf; nop
set 4,%r29
set _C,%r28
stb %r29,[%r28]
ldub [%r28],%r29
mov %r29,%r28
set _c,%r27
stb %r28,[%r27]
set _s,%r27
sth %r28,[%r27]
set _i,%r27
st %r28,[%r27]
set _S,%r28
sth %r29,[%r28]
set _I,%r28
st %r29,[%r28]
set _C,%r29
ldub [%r29],%r29
cmp %r29,%r0; bl L4; nop
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-8]; st %f31,[%fp+-8+4]
ba L5; nop
L4:
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L6,%r29
ld [%r29],%f28; ld [%r29+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-8]; st %f31,[%fp+-8+4]
L5:
ld [%fp+-8],%f30; ld [%fp+-8+4],%f31
fdtos %f30,%f31
set _f,%r29
st %f31,[%r29]
set _C,%r29
ldub [%r29],%r29
cmp %r29,%r0; bl L8; nop
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-16]; st %f31,[%fp+-16+4]
ba L9; nop
L8:
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L6,%r29
ld [%r29],%f28; ld [%r29+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-16]; st %f31,[%fp+-16+4]
L9:
ld [%fp+-16],%f30; ld [%fp+-16+4],%f31
set _d,%r29
st %f30,[%r29]; st %f31,[%r29+4]
set L2,%r8
set _c,%r29
ldsb [%r29],%r9
set _s,%r29
ldsh [%r29],%r10
set _i,%r29
ld [%r29],%r11
set _C,%r29
ldub [%r29],%r12
set _S,%r29
lduh [%r29],%r13
set _I,%r29
ld [%r29],%r29
st %r29,[%sp+4*6+68]
set _f,%r29
ld [%r29],%f31
fstod %f31,%f30
st %f30,[%sp+4*7+68]
st %f31,[%sp+4*8+68]
set _d,%r29
ld [%r29],%f30; ld [%r29+4],%f31
st %f30,[%sp+4*9+68]
st %f31,[%sp+4*10+68]
call _printf; nop
set 5,%r29
set _S,%r28
sth %r29,[%r28]
lduh [%r28],%r29
mov %r29,%r28
set _c,%r27
stb %r28,[%r27]
set _s,%r27
sth %r28,[%r27]
set _i,%r27
st %r28,[%r27]
set _C,%r28
stb %r29,[%r28]
set _I,%r28
st %r29,[%r28]
set _S,%r29
lduh [%r29],%r29
cmp %r29,%r0; bl L11; nop
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ba L12; nop
L11:
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L6,%r29
ld [%r29],%f28; ld [%r29+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
L12:
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
fdtos %f30,%f31
set _f,%r29
st %f31,[%r29]
set _S,%r29
lduh [%r29],%r29
cmp %r29,%r0; bl L14; nop
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-32]; st %f31,[%fp+-32+4]
ba L15; nop
L14:
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L6,%r29
ld [%r29],%f28; ld [%r29+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-32]; st %f31,[%fp+-32+4]
L15:
ld [%fp+-32],%f30; ld [%fp+-32+4],%f31
set _d,%r29
st %f30,[%r29]; st %f31,[%r29+4]
set L2,%r8
set _c,%r29
ldsb [%r29],%r9
set _s,%r29
ldsh [%r29],%r10
set _i,%r29
ld [%r29],%r11
set _C,%r29
ldub [%r29],%r12
set _S,%r29
lduh [%r29],%r13
set _I,%r29
ld [%r29],%r29
st %r29,[%sp+4*6+68]
set _f,%r29
ld [%r29],%f31
fstod %f31,%f30
st %f30,[%sp+4*7+68]
st %f31,[%sp+4*8+68]
set _d,%r29
ld [%r29],%f30; ld [%r29+4],%f31
st %f30,[%sp+4*9+68]
st %f31,[%sp+4*10+68]
call _printf; nop
set 6,%r29
set _I,%r28
st %r29,[%r28]
ld [%r28],%r29
mov %r29,%r28
set _c,%r27
stb %r28,[%r27]
set _s,%r27
sth %r28,[%r27]
set _i,%r27
st %r28,[%r27]
set _C,%r28
stb %r29,[%r28]
set _S,%r28
sth %r29,[%r28]
set _I,%r29
ld [%r29],%r29
cmp %r29,%r0; bl L17; nop
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ba L18; nop
L17:
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L6,%r29
ld [%r29],%f28; ld [%r29+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
L18:
ld [%fp+-40],%f30; ld [%fp+-40+4],%f31
fdtos %f30,%f31
set _f,%r29
st %f31,[%r29]
set _I,%r29
ld [%r29],%r29
cmp %r29,%r0; bl L20; nop
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-48]; st %f31,[%fp+-48+4]
ba L21; nop
L20:
st %r29,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L6,%r29
ld [%r29],%f28; ld [%r29+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-48]; st %f31,[%fp+-48+4]
L21:
ld [%fp+-48],%f30; ld [%fp+-48+4],%f31
set _d,%r29
st %f30,[%r29]; st %f31,[%r29+4]
set L2,%r8
set _c,%r29
ldsb [%r29],%r9
set _s,%r29
ldsh [%r29],%r10
set _i,%r29
ld [%r29],%r11
set _C,%r29
ldub [%r29],%r12
set _S,%r29
lduh [%r29],%r13
set _I,%r29
ld [%r29],%r29
st %r29,[%sp+4*6+68]
set _f,%r29
ld [%r29],%f31
fstod %f31,%f30
st %f30,[%sp+4*7+68]
st %f31,[%sp+4*8+68]
set _d,%r29
ld [%r29],%f30; ld [%r29+4],%f31
st %f30,[%sp+4*9+68]
st %f31,[%sp+4*10+68]
call _printf; nop
set L22,%r26
ld [%r26],%f31
set _f,%r26
st %f31,[%r26]
ld [%r26],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
set _c,%r25
stb %r26,[%r25]
set _s,%r25
sth %r26,[%r25]
set _i,%r25
st %r26,[%r25]
set _f,%r26
ld [%r26],%f31
fstod %f31,%f30
set L26,%r26
ld [%r26],%f28; ld [%r26+4],%f29
fcmped %f30,%f28; nop; fbul L24; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
set 0x80000000,%r25
add %r26,%r25,%r29
ba L25; nop
L24:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
mov %r26,%r29
L25:
set _C,%r26
stb %r29,[%r26]
set _f,%r26
ld [%r26],%f31
fstod %f31,%f30
set L26,%r26
ld [%r26],%f28; ld [%r26+4],%f29
fcmped %f30,%f28; nop; fbul L28; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
set 0x80000000,%r25
add %r26,%r25,%r28
ba L29; nop
L28:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
mov %r26,%r28
L29:
set _S,%r26
sth %r28,[%r26]
set _f,%r26
ld [%r26],%f31
fstod %f31,%f30
set L26,%r26
ld [%r26],%f28; ld [%r26+4],%f29
fcmped %f30,%f28; nop; fbul L31; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
set 0x80000000,%r25
add %r26,%r25,%r27
ba L32; nop
L31:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
mov %r26,%r27
L32:
set _I,%r26
st %r27,[%r26]
set _f,%r26
ld [%r26],%f31
fstod %f31,%f30
set _d,%r26
st %f30,[%r26]; st %f31,[%r26+4]
set L2,%r8
set _c,%r25
ldsb [%r25],%r9
set _s,%r25
ldsh [%r25],%r10
set _i,%r25
ld [%r25],%r11
set _C,%r25
ldub [%r25],%r12
set _S,%r25
lduh [%r25],%r13
set _I,%r25
ld [%r25],%r25
st %r25,[%sp+4*6+68]
st %f30,[%sp+4*7+68]
st %f31,[%sp+4*8+68]
ld [%r26],%f30; ld [%r26+4],%f31
st %f30,[%sp+4*9+68]
st %f31,[%sp+4*10+68]
call _printf; nop
set L33,%r26
ld [%r26],%f30; ld [%r26+4],%f31
set _d,%r26
st %f30,[%r26]; st %f31,[%r26+4]
ld [%r26],%f30; ld [%r26+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
set _c,%r25
stb %r26,[%r25]
set _s,%r25
sth %r26,[%r25]
set _i,%r25
st %r26,[%r25]
set _d,%r26
ld [%r26],%f30; ld [%r26+4],%f31
set L26,%r26
ld [%r26],%f28; ld [%r26+4],%f29
fcmped %f30,%f28; nop; fbul L35; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
set 0x80000000,%r25
add %r26,%r25,%r29
ba L36; nop
L35:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
mov %r26,%r29
L36:
set _C,%r26
stb %r29,[%r26]
set _d,%r26
ld [%r26],%f30; ld [%r26+4],%f31
set L26,%r26
ld [%r26],%f28; ld [%r26+4],%f29
fcmped %f30,%f28; nop; fbul L38; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
set 0x80000000,%r25
add %r26,%r25,%r28
ba L39; nop
L38:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
mov %r26,%r28
L39:
set _S,%r26
sth %r28,[%r26]
set _d,%r26
ld [%r26],%f30; ld [%r26+4],%f31
set L26,%r26
ld [%r26],%f28; ld [%r26+4],%f29
fcmped %f30,%f28; nop; fbul L41; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
set 0x80000000,%r25
add %r26,%r25,%r27
ba L42; nop
L41:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r26
mov %r26,%r27
L42:
set _I,%r26
st %r27,[%r26]
set _d,%r26
ld [%r26],%f30; ld [%r26+4],%f31
fdtos %f30,%f29
set _f,%r26
st %f29,[%r26]
set L2,%r8
set _c,%r25
ldsb [%r25],%r9
set _s,%r25
ldsh [%r25],%r10
set _i,%r25
ld [%r25],%r11
set _C,%r25
ldub [%r25],%r12
set _S,%r25
lduh [%r25],%r13
set _I,%r25
ld [%r25],%r25
st %r25,[%sp+4*6+68]
ld [%r26],%f29
fstod %f29,%f28
st %f28,[%sp+4*7+68]
st %f29,[%sp+4*8+68]
st %f30,[%sp+4*9+68]
st %f31,[%sp+4*10+68]
call _printf; nop
L1:
ret; restore
.seg "bss"
.global _d2
.align 8; .common _d2,8
.global _d1
.align 8; .common _d1,8
.global _d
.align 8; .common _d,8
.global _f2
.align 4; .common _f2,4
.global _f1
.align 4; .common _f1,4
.global _f
.align 4; .common _f,4
.global _I
.align 4; .common _I,4
.global _S
.align 2; .common _S,2
.global _C
.common _C,1
.global _i2
.align 4; .common _i2,4
.global _i1
.align 4; .common _i1,4
.global _i
.align 4; .common _i,4
.global _s
.align 2; .common _s,2
.global _c2
.common _c2,1
.global _c1
.common _c1,1
.global _c
.common _c,1
.seg "text"
.align 8; L33:.word 0x40200000,0x0
.align 8; L26:.word 0x41e00000,0x0
.align 4; L22:.word 0x40e00000
.align 8; L6:.word 0x41f00000,0x0
L2:.ascii "%d %d %d %d %d %d %f %f\012\000"
