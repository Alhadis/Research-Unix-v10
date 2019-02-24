.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
set L6,%r29
ba L5; nop
L2:
ldsb [%r29],%r8
call _backslash; nop
mov %r8,%r27
set L7,%r8
ldsb [%r29],%r9
mov %r27,%r10
call _printf; nop
L3:
add %r29,1,%r29
L5:
ldsb [%r29],%r27
cmp %r27,%r0; bne L2; nop
call _f; nop
call _g; nop
call _h; nop
set 16777216,%r28
ba L11; nop
L8:
mov %r28,%r8
call _big; nop
L9:
set 16777216,%r27
add %r28,%r27,%r28
L11:
set 0x7000000,%r27
and %r28,%r27,%r27
cmp %r27,%r0; bne L8; nop
call _limit; nop
mov %r0,%r24
L1:
ret; restore
.global _backslash
.align 4
.proc 4
_backslash:
cmp %r8,110; be L18; nop
cmp %r8,110; bg L22; nop
cmp %r8,102; be L17; nop
cmp %r8,102; bg L14; nop
cmp %r8,98; be L16; nop
ba L14; nop
L22:
cmp %r8,114; be L19; nop
cmp %r8,116; be L20; nop
cmp %r8,118; be L21; nop
ba L14; nop
L16:
set 8,%r8
ba L13; nop
L17:
set 12,%r8
ba L13; nop
L18:
set 10,%r8
ba L13; nop
L19:
set 13,%r8
ba L13; nop
L20:
set 9,%r8
ba L13; nop
L21:
set 11,%r8
ba L13; nop
L14:
L13:
retl; nop
.global _f
.align 4
.proc 4
_f:save %sp,-96,%sp
mov %r0,%r28
set L24,%r8
call _printf; nop
mov %r0,%r29
L25:
mov %r29,%r27
cmp %r29,1; bl L29; nop
cmp %r29,20; bg L29; nop
sll %r29,2,%r26
set L42-4,%r25
ld [%r26+%r25],%r26
jmp %r26; nop
L42:
.align 4; .word L31
.align 4; .word L32
.align 4; .word L29
.align 4; .word L29
.align 4; .word L29
.align 4; .word L29
.align 4; .word L33
.align 4; .word L34
.align 4; .word L35
.align 4; .word L29
.align 4; .word L29
.align 4; .word L29
.align 4; .word L29
.align 4; .word L29
.align 4; .word L29
.align 4; .word L36
.align 4; .word L37
.align 4; .word L38
.align 4; .word L39
.align 4; .word L40
L31:
mov %r29,%r28
ba L30; nop
L32:
mov %r29,%r28
ba L30; nop
L33:
mov %r29,%r28
ba L30; nop
L34:
mov %r29,%r28
ba L30; nop
L35:
mov %r29,%r28
ba L30; nop
L36:
mov %r29,%r28
ba L30; nop
L37:
mov %r29,%r28
ba L30; nop
L38:
mov %r29,%r28
ba L30; nop
L39:
mov %r29,%r28
ba L30; nop
L40:
mov %r29,%r28
L29:
L30:
set L44,%r8
mov %r28,%r9
call _printf; nop
L26:
add %r29,1,%r29
cmp %r29,20; ble L25; nop
L23:
ret; restore
.global _g
.align 4
.proc 4
_g:save %sp,-96,%sp
set L46,%r8
call _printf; nop
set 1,%r29
L47:
cmp %r29,1001; bl L65; nop
cmp %r29,1004; bg L64; nop
sll %r29,2,%r28
set L66-4004,%r27
ld [%r28+%r27],%r28
jmp %r28; nop
L66:
.align 4; .word L60
.align 4; .word L60
.align 4; .word L60
.align 4; .word L60
L65:
cmp %r29,1; bl L51; nop
cmp %r29,8; bg L51; nop
sll %r29,2,%r28
set L69-4,%r27
ld [%r28+%r27],%r28
jmp %r28; nop
L69:
.align 4; .word L53
.align 4; .word L53
.align 4; .word L55
.align 4; .word L55
.align 4; .word L55
.align 4; .word L57
.align 4; .word L57
.align 4; .word L57
L68:
L64:
cmp %r29,3001; bl L51; nop
cmp %r29,3004; bg L51; nop
sll %r29,2,%r28
set L72-12004,%r27
ld [%r28+%r27],%r28
jmp %r28; nop
L72:
.align 4; .word L62
.align 4; .word L62
.align 4; .word L62
.align 4; .word L62
L53:
set L54,%r8
mov %r29,%r9
call _printf; nop
ba L52; nop
L55:
set L56,%r8
mov %r29,%r9
call _printf; nop
ba L52; nop
L57:
set L58,%r8
mov %r29,%r9
call _printf; nop
L51:
set L59,%r8
mov %r29,%r9
call _printf; nop
ba L52; nop
L60:
set L61,%r8
mov %r29,%r9
call _printf; nop
ba L52; nop
L62:
set L63,%r8
mov %r29,%r9
call _printf; nop
L52:
L48:
add %r29,1,%r29
cmp %r29,10; ble L47; nop
L45:
ret; restore
.global _h
.align 4
.proc 4
_h:save %sp,-96,%sp
mov %r0,%r28
set L75,%r8
call _printf; nop
set 1,%r29
L76:
cmp %r29,128; be L82; nop
cmp %r29,128; bg L95; nop
cmp %r29,16; be L84; nop
cmp %r29,16; bg L96; nop
cmp %r29,8; be L85; nop
ba L80; nop
L96:
cmp %r29,120; be L86; nop
ba L80; nop
L95:
cmp %r29,280; be L87; nop
cmp %r29,280; bg L97; nop
cmp %r29,264; be L88; nop
cmp %r29,264; bg L98; nop
cmp %r29,248; be L89; nop
ba L80; nop
L98:
cmp %r29,272; be L90; nop
ba L80; nop
L97:
cmp %r29,304; be L91; nop
cmp %r29,304; bg L99; nop
cmp %r29,296; be L92; nop
cmp %r29,296; bg L80; nop
cmp %r29,288; be L93; nop
ba L80; nop
L99:
cmp %r29,312; be L94; nop
ba L80; nop
L80:
add %r28,1,%r28
ba L77; nop
L82:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L84:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L85:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L86:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L87:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L88:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L89:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L90:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L91:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L92:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L93:
set L83,%r8
mov %r29,%r9
call _printf; nop
ba L81; nop
L94:
set L83,%r8
mov %r29,%r9
call _printf; nop
L81:
L77:
add %r29,1,%r29
cmp %r29,500; ble L76; nop
set L100,%r8
mov %r28,%r9
call _printf; nop
L74:
ret; restore
.global _big
.align 4
.proc 4
_big:save %sp,-96,%sp
set 0x6000000,%r28
and %r24,%r28,%r29
mov %r29,%r28
set 33554432,%r27
cmp %r28,%r27; be L107; nop
cmp %r28,%r27; bg L110; nop
mov %r29,%r28
cmp %r28,-2; be L105; nop
cmp %r28,-1; be L105; nop
cmp %r28,%r0; be L105; nop
ba L102; nop
L110:
set 67108864,%r28
cmp %r29,%r28; be L108; nop
ba L102; nop
L105:
set L106,%r8
mov %r24,%r9
call _printf; nop
ba L103; nop
L107:
set L106,%r8
mov %r24,%r9
call _printf; nop
ba L103; nop
L108:
set L106,%r8
mov %r24,%r9
call _printf; nop
ba L103; nop
L102:
set L109,%r8
mov %r24,%r9
call _printf; nop
L103:
L101:
ret; restore
.global _limit
.align 4
.proc 4
_limit:save %sp,-96,%sp
set -2147483648,%r29
L112:
set -2147483648,%r28
cmp %r29,%r28; bl L116; nop
set -2147483644,%r27
cmp %r29,%r27; bg L116; nop
sll %r29,2,%r27
sll %r28,2,%r28
sub %r27,%r28,%r28
set L130,%r27
ld [%r28+%r27],%r28
jmp %r28; nop
L130:
.align 4; .word L118
.align 4; .word L120
.align 4; .word L122
.align 4; .word L124
.align 4; .word L126
L118:
set L119,%r8
call _printf; nop
ba L117; nop
L120:
set L121,%r8
call _printf; nop
ba L117; nop
L122:
set L123,%r8
call _printf; nop
ba L117; nop
L124:
set L125,%r8
call _printf; nop
ba L117; nop
L126:
set L127,%r8
call _printf; nop
ba L117; nop
L116:
set L128,%r8
call _printf; nop
L117:
L113:
add %r29,1,%r29
set -2147483643,%r28
cmp %r29,%r28; ble L112; nop
set 2147483647,%r29
L131:
set 2147483643,%r28
cmp %r29,%r28; bl L135; nop
set 2147483647,%r27
cmp %r29,%r27; bg L135; nop
sll %r29,2,%r27
sll %r28,2,%r28
sub %r27,%r28,%r28
set L143,%r27
ld [%r28+%r27],%r28
jmp %r28; nop
L143:
.align 4; .word L141
.align 4; .word L140
.align 4; .word L139
.align 4; .word L138
.align 4; .word L137
L137:
set L119,%r8
call _printf; nop
ba L136; nop
L138:
set L121,%r8
call _printf; nop
ba L136; nop
L139:
set L123,%r8
call _printf; nop
ba L136; nop
L140:
set L125,%r8
call _printf; nop
ba L136; nop
L141:
set L127,%r8
call _printf; nop
ba L136; nop
L135:
set L128,%r8
call _printf; nop
L136:
L132:
sub %r29,1,%r29
set 2147483642,%r28
cmp %r29,%r28; bge L131; nop
L111:
ret; restore
.seg "text"
L128:.byte 53,10,0
L127:.byte 52,10,0
L125:.byte 51,10,0
L123:.byte 50,10,0
L121:.byte 49,10,0
L119:.byte 48,10,0
L109:.ascii "x = 0x%x (default)\012\000"
L106:.ascii "x = 0x%x\012\000"
L100:.ascii "%d defaults\012\000"
L83:.ascii "i = %d\012\000"
L75:.byte 104,58,10,0
L63:.ascii "6 %d\012\000"
L61:.ascii "5 %d\012\000"
L59:.ascii "d %d\012\000"
L58:.ascii "3 %d\012\000"
L56:.ascii "2 %d\012\000"
L54:.ascii "1 %d\012\000"
L46:.byte 103,58,10,0
L44:.ascii "x = %d\012\000"
L24:.byte 102,58,10,0
L7:.ascii "%c = 0x%x\012\000"
L6:.ascii "bfnrtvx\000"
