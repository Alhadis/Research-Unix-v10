.seg "data"
.align 4; _2:.word _s22
.word _s241
.word _s243
.word _s244
.word _s25
.word _s26
.word _s4
.word _s61
.word _s626
.word _s71
.word _s72
.word _s757
.word _s7813
.word _s714
.word _s715
.word _s81
.word _s84
.word _s85
.word _s86
.word _s88
.word _s9
.seg "bss"
.align 4; .reserve _3,68
.align 4; .reserve _4,4
.seg "text"
.global _main
.align 4
.proc 4
_main:save %sp,-96,%sp
set 1,%r28
set _3+36,%r27
st %r28,[%r27]
set 1,%r28
set _3+40,%r27
st %r28,[%r27]
set 1,%r28
set _3+44,%r27
st %r28,[%r27]
set 1,%r28
set _3+48,%r27
st %r28,[%r27]
set _3,%r28
set _4,%r27
st %r28,[%r27]
mov %r0,%r29
ba L12; nop
L9:
set _4,%r28
ld [%r28],%r8
sll %r29,2,%r28
set _2,%r27
ld [%r28+%r27],%r28
call %r28; nop
set _3+52,%r28
st %r8,[%r28]
set _3+56,%r28
ld [%r28],%r28
set _3+52,%r27
ld [%r27],%r27
add %r28,%r27,%r28
set _3+56,%r27
st %r28,[%r27]
set _3+36,%r28
ld [%r28],%r28
cmp %r28,%r0; be L17; nop
set L20,%r8
set _3+60,%r9
set _3+52,%r28
ld [%r28],%r10
call _printf; nop
L17:
L10:
add %r29,1,%r29
L12:
cmp %r29,21; blu L9; nop
set _3+56,%r28
ld [%r28],%r28
cmp %r28,%r0; bne L23; nop
set L26,%r8
call _printf; nop
ba L24; nop
L23:
set L27,%r8
call _printf; nop
L24:
mov %r0,%r24
L1:
ret; restore
.seg "data"
_29:.ascii "s22,er%d\012\000"
_30:.ascii "s22    \000"
.seg "text"
.global _s22
.align 4
.proc 4
_s22:save %sp,-96,%sp
mov %r0,%r27
set _30,%r29
add %r24,60,%r28
L31:
L32:
mov %r28,%r20
add %r20,1,%r28
mov %r29,%r19
add %r19,1,%r29
ldsb [%r19],%r19
stb %r19,[%r20]
sll %r19,24,%g1; sra %g1,24,%r20
cmp %r20,%r0; bne L31; nop
set 1,%r26
set 2,%r23
set 3,%r22
set 4,%r25
add %r26,%r23,%r20
add %r20,%r22,%r20
add %r20,%r25,%r20
cmp %r20,10; be L34; nop
add %r27,1,%r27
ld [%r24+44],%r20
cmp %r20,%r0; be L36; nop
set _29,%r8
set 1,%r9
call _printf; nop
L36:
L34:
set 2,%r21
cmp %r21,%r26; bne L38; nop
add %r27,4,%r27
ld [%r24+44],%r20
cmp %r20,%r0; be L40; nop
set _29,%r8
set 4,%r9
call _printf; nop
L40:
L38:
mov %r27,%r24
L28:
ret; restore
.seg "data"
_43:.ascii "s241,er%d\012\000"
_44:.ascii "s241   \000"
.align 4; _45:.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 6
.word 0
.word 8
.word 0
.word 12
.word 0
.word 16
.word 0
.word 18
.word 0
.word 20
.word 0
.word 24
.word 0
.word 28
.word 0
.word 30
.word 0
.word 32
.word 0
.word 36
.seg "text"
.global _s241
.align 4
.proc 4
_s241:save %sp,-560,%sp
mov %r0,%r26
mov %r0,%r25
set _44,%r28
add %r24,60,%r27
L46:
L47:
mov %r27,%r23
add %r23,1,%r27
mov %r28,%r22
add %r22,1,%r28
ldsb [%r22],%r22
stb %r22,[%r23]
sll %r22,24,%g1; sra %g1,24,%r23
cmp %r23,%r0; bne L46; nop
ba L49; nop
add %r26,1,%r26
ld [%r24+44],%r23
cmp %r23,%r0; be L51; nop
set _43,%r8
set 1,%r9
call _printf; nop
L51:
L49:
ba L53; nop
add %r26,2,%r26
ld [%r24+44],%r23
cmp %r23,%r0; be L55; nop
set _43,%r8
set 2,%r9
call _printf; nop
L55:
L53:
ba L57; nop
add %r26,4,%r26
ld [%r24+44],%r23
cmp %r23,%r0; be L59; nop
set _43,%r8
set 4,%r9
call _printf; nop
L59:
L57:
ba L61; nop
add %r26,8,%r26
ld [%r24+44],%r23
cmp %r23,%r0; be L63; nop
set _43,%r8
set 8,%r9
call _printf; nop
L63:
L61:
mov %r0,%r29
L65:
sll %r29,2,%r23
set _45,%r22
st %r29,[%r23+%r22]
L66:
add %r29,1,%r29
cmp %r29,17; bl L65; nop
set 18,%r29
L69:
sll %r29,2,%r23
set _45,%r22
add %r23,%r22,%r23
ld [%r23],%r8
call _pow2; nop
st %r8,[%r23]
sll %r29,2,%r23
set _45,%r22
ld [%r23+%r22],%r22
sub %r22,1,%r22
set _45-4,%r21
st %r22,[%r23+%r21]
add %r29,2,%r29
L70:
cmp %r29,39; bl L69; nop
st %r0,[%fp+-156]
st %r0,[%fp+-312]
st %r0,[%fp+-468]
set 1,%r23
st %r23,[%fp+-152]
set 1,%r23
st %r23,[%fp+-308]
set 1,%r23
st %r23,[%fp+-464]
set 2,%r23
st %r23,[%fp+-148]
set 2,%r23
st %r23,[%fp+-304]
set 2,%r23
st %r23,[%fp+-460]
set 3,%r23
st %r23,[%fp+-144]
set 3,%r23
st %r23,[%fp+-300]
set 3,%r23
st %r23,[%fp+-456]
set 4,%r23
st %r23,[%fp+-140]
set 4,%r23
st %r23,[%fp+-296]
set 4,%r23
st %r23,[%fp+-452]
set 5,%r23
st %r23,[%fp+-136]
set 5,%r23
st %r23,[%fp+-292]
set 5,%r23
st %r23,[%fp+-448]
set 6,%r23
st %r23,[%fp+-132]
set 6,%r23
st %r23,[%fp+-288]
set 6,%r23
st %r23,[%fp+-444]
set 7,%r23
st %r23,[%fp+-128]
set 7,%r23
st %r23,[%fp+-284]
set 7,%r23
st %r23,[%fp+-440]
set 8,%r23
st %r23,[%fp+-124]
set 8,%r23
st %r23,[%fp+-280]
set 8,%r23
st %r23,[%fp+-436]
set 9,%r23
st %r23,[%fp+-120]
set 9,%r23
st %r23,[%fp+-276]
set 9,%r23
st %r23,[%fp+-432]
set 10,%r23
st %r23,[%fp+-116]
set 10,%r23
st %r23,[%fp+-272]
set 10,%r23
st %r23,[%fp+-428]
set 11,%r23
st %r23,[%fp+-112]
set 11,%r23
st %r23,[%fp+-268]
set 11,%r23
st %r23,[%fp+-424]
set 12,%r23
st %r23,[%fp+-108]
set 12,%r23
st %r23,[%fp+-264]
set 12,%r23
st %r23,[%fp+-420]
set 13,%r23
st %r23,[%fp+-104]
set 13,%r23
st %r23,[%fp+-260]
set 13,%r23
st %r23,[%fp+-416]
set 14,%r23
st %r23,[%fp+-100]
set 14,%r23
st %r23,[%fp+-256]
set 14,%r23
st %r23,[%fp+-412]
set 15,%r23
st %r23,[%fp+-96]
set 15,%r23
st %r23,[%fp+-252]
set 15,%r23
st %r23,[%fp+-408]
set 16,%r23
st %r23,[%fp+-92]
set 16,%r23
st %r23,[%fp+-248]
set 16,%r23
st %r23,[%fp+-404]
set 63,%r23
st %r23,[%fp+-88]
set 63,%r23
st %r23,[%fp+-244]
set 63,%r23
st %r23,[%fp+-400]
set 64,%r23
st %r23,[%fp+-84]
set 64,%r23
st %r23,[%fp+-240]
set 64,%r23
st %r23,[%fp+-396]
set 255,%r23
st %r23,[%fp+-80]
set 255,%r23
st %r23,[%fp+-236]
set 255,%r23
st %r23,[%fp+-392]
set 256,%r23
st %r23,[%fp+-76]
set 256,%r23
st %r23,[%fp+-232]
set 256,%r23
st %r23,[%fp+-388]
set 4095,%r23
st %r23,[%fp+-72]
set 4095,%r23
st %r23,[%fp+-228]
set 4095,%r23
st %r23,[%fp+-384]
set 4096,%r23
st %r23,[%fp+-68]
set 4096,%r23
st %r23,[%fp+-224]
set 4096,%r23
st %r23,[%fp+-380]
set 65535,%r23
st %r23,[%fp+-64]
set 65535,%r23
st %r23,[%fp+-220]
set 65535,%r23
st %r23,[%fp+-376]
set 65536,%r23
st %r23,[%fp+-60]
set 65536,%r23
st %r23,[%fp+-216]
set 65536,%r23
st %r23,[%fp+-372]
set 262143,%r23
st %r23,[%fp+-56]
set 262143,%r23
st %r23,[%fp+-212]
set 262143,%r23
st %r23,[%fp+-368]
set 262144,%r23
st %r23,[%fp+-52]
set 262144,%r23
st %r23,[%fp+-208]
set 262144,%r23
st %r23,[%fp+-364]
set 1048575,%r23
st %r23,[%fp+-48]
set 1048575,%r23
st %r23,[%fp+-204]
set 1048575,%r23
st %r23,[%fp+-360]
set 1048576,%r23
st %r23,[%fp+-44]
set 1048576,%r23
st %r23,[%fp+-200]
set 1048576,%r23
st %r23,[%fp+-356]
set 16777215,%r23
st %r23,[%fp+-40]
set 16777215,%r23
st %r23,[%fp+-196]
set 16777215,%r23
st %r23,[%fp+-352]
set 16777216,%r23
st %r23,[%fp+-36]
set 16777216,%r23
st %r23,[%fp+-192]
set 16777216,%r23
st %r23,[%fp+-348]
set 268435455,%r23
st %r23,[%fp+-32]
set 268435455,%r23
st %r23,[%fp+-188]
set 268435455,%r23
st %r23,[%fp+-344]
set 268435456,%r23
st %r23,[%fp+-28]
set 268435456,%r23
st %r23,[%fp+-184]
set 268435456,%r23
st %r23,[%fp+-340]
set 1073741823,%r23
st %r23,[%fp+-24]
set 1073741823,%r23
st %r23,[%fp+-180]
set 1073741823,%r23
st %r23,[%fp+-336]
set 1073741824,%r23
st %r23,[%fp+-20]
set 1073741824,%r23
st %r23,[%fp+-176]
set 1073741824,%r23
st %r23,[%fp+-332]
set 0xffffffff,%r23
st %r23,[%fp+-16]
set 0xffffffff,%r23
st %r23,[%fp+-172]
set 0xffffffff,%r23
st %r23,[%fp+-328]
set 2147483647,%r23
st %r23,[%fp+-12]
set 2147483647,%r23
st %r23,[%fp+-168]
set 2147483647,%r23
st %r23,[%fp+-324]
set 2147483647,%r23
st %r23,[%fp+-8]
set 2147483647,%r23
st %r23,[%fp+-164]
set 2147483647,%r23
st %r23,[%fp+-320]
set 2147483647,%r23
st %r23,[%fp+-4]
set 2147483647,%r23
st %r23,[%fp+-160]
set 2147483647,%r23
st %r23,[%fp+-316]
mov %r0,%r29
L188:
sll %r29,2,%r23
set _45,%r22
ld [%r23+%r22],%r22
add -156,%fp,%r21
ld [%r23+%r21],%r21
cmp %r22,%r21; bne L195; nop
add -312,%fp,%r22
ld [%r23+%r22],%r22
cmp %r21,%r22; bne L195; nop
add -468,%fp,%r21
ld [%r23+%r21],%r23
cmp %r22,%r23; be L192; nop
L195:
ld [%r24+40],%r23
cmp %r23,%r0; be L196; nop
set L198,%r8
call _printf; nop
set L199,%r8
call _printf; nop
L196:
L192:
L189:
add %r29,1,%r29
cmp %r29,39; bl L188; nop
cmp %r25,%r0; be L200; nop
set 16,%r26
L200:
mov %r26,%r24
L42:
ret; restore
.global _pow2
.align 4
.proc 4
_pow2:
set 1,%r13
ba L204; nop
L203:
sll %r13,1,%r13
L204:
mov %r8,%r12
sub %r12,1,%r8
cmp %r12,%r0; bne L203; nop
mov %r13,%r8
L202:
retl; nop
.seg "data"
_207:.ascii "s243,er%d\012\000"
_208:.ascii "s243   \000"
.seg "text"
.global _s243
.align 4
.proc 4
_s243:save %sp,-352,%sp
mov %r0,%r27
set _208,%r29
add %r24,60,%r28
L209:
L210:
mov %r28,%r26
add %r26,1,%r28
mov %r29,%r25
add %r25,1,%r29
ldsb [%r25],%r25
stb %r25,[%r26]
sll %r25,24,%g1; sra %g1,24,%r26
cmp %r26,%r0; bne L209; nop
add -256,%fp,%r8
call _zerofill; nop
set 1,%r26
stb %r26,[%fp+-159]
set 1,%r26
stb %r26,[%fp+-191]
set 1,%r26
stb %r26,[%fp+-130]
set 1,%r26
stb %r26,[%fp+-208]
set 1,%r26
stb %r26,[%fp+-158]
set 1,%r26
stb %r26,[%fp+-190]
set 1,%r26
stb %r26,[%fp+-223]
set 1,%r26
stb %r26,[%fp+-207]
set 1,%r26
stb %r26,[%fp+-157]
set 1,%r26
stb %r26,[%fp+-189]
set 1,%r26
stb %r26,[%fp+-222]
set 1,%r26
stb %r26,[%fp+-206]
set 1,%r26
stb %r26,[%fp+-156]
set 1,%r26
stb %r26,[%fp+-188]
set 1,%r26
stb %r26,[%fp+-221]
set 1,%r26
stb %r26,[%fp+-205]
set 1,%r26
stb %r26,[%fp+-155]
set 1,%r26
stb %r26,[%fp+-187]
set 1,%r26
stb %r26,[%fp+-219]
set 1,%r26
stb %r26,[%fp+-204]
set 1,%r26
stb %r26,[%fp+-154]
set 1,%r26
stb %r26,[%fp+-186]
set 1,%r26
stb %r26,[%fp+-218]
set 1,%r26
stb %r26,[%fp+-203]
set 1,%r26
stb %r26,[%fp+-153]
set 1,%r26
stb %r26,[%fp+-185]
set 1,%r26
stb %r26,[%fp+-216]
set 1,%r26
stb %r26,[%fp+-202]
set 1,%r26
stb %r26,[%fp+-152]
set 1,%r26
stb %r26,[%fp+-184]
set 1,%r26
stb %r26,[%fp+-215]
set 1,%r26
stb %r26,[%fp+-201]
set 1,%r26
stb %r26,[%fp+-151]
set 1,%r26
stb %r26,[%fp+-183]
set 1,%r26
stb %r26,[%fp+-161]
set 1,%r26
stb %r26,[%fp+-200]
set 1,%r26
stb %r26,[%fp+-150]
set 1,%r26
stb %r26,[%fp+-182]
set 1,%r26
stb %r26,[%fp+-195]
set 1,%r26
stb %r26,[%fp+-199]
set 1,%r26
stb %r26,[%fp+-149]
set 1,%r26
stb %r26,[%fp+-181]
set 1,%r26
stb %r26,[%fp+-211]
set 1,%r26
stb %r26,[%fp+-148]
set 1,%r26
stb %r26,[%fp+-180]
set 1,%r26
stb %r26,[%fp+-162]
set 1,%r26
stb %r26,[%fp+-147]
set 1,%r26
stb %r26,[%fp+-179]
set 1,%r26
stb %r26,[%fp+-132]
set 1,%r26
stb %r26,[%fp+-246]
set 1,%r26
stb %r26,[%fp+-146]
set 1,%r26
stb %r26,[%fp+-178]
set 1,%r26
stb %r26,[%fp+-247]
set 1,%r26
stb %r26,[%fp+-145]
set 1,%r26
stb %r26,[%fp+-177]
set 1,%r26
stb %r26,[%fp+-133]
set 1,%r26
stb %r26,[%fp+-248]
set 1,%r26
stb %r26,[%fp+-144]
set 1,%r26
stb %r26,[%fp+-176]
set 1,%r26
stb %r26,[%fp+-131]
set 1,%r26
stb %r26,[%fp+-243]
set 1,%r26
stb %r26,[%fp+-143]
set 1,%r26
stb %r26,[%fp+-175]
set 1,%r26
stb %r26,[%fp+-165]
set 1,%r26
stb %r26,[%fp+-244]
set 1,%r26
stb %r26,[%fp+-142]
set 1,%r26
stb %r26,[%fp+-174]
set 1,%r26
stb %r26,[%fp+-163]
set 1,%r26
stb %r26,[%fp+-141]
set 1,%r26
stb %r26,[%fp+-173]
set 1,%r26
stb %r26,[%fp+-213]
set 1,%r26
stb %r26,[%fp+-164]
set 1,%r26
stb %r26,[%fp+-140]
set 1,%r26
stb %r26,[%fp+-172]
set 1,%r26
stb %r26,[%fp+-197]
set 1,%r26
stb %r26,[%fp+-217]
set 1,%r26
stb %r26,[%fp+-139]
set 1,%r26
stb %r26,[%fp+-171]
set 1,%r26
stb %r26,[%fp+-214]
set 1,%r26
stb %r26,[%fp+-138]
set 1,%r26
stb %r26,[%fp+-170]
set 1,%r26
stb %r26,[%fp+-198]
set 1,%r26
stb %r26,[%fp+-256]
stb %r26,[%fp+-137]
set 1,%r26
stb %r26,[%fp+-169]
set 1,%r26
stb %r26,[%fp+-196]
set 1,%r26
stb %r26,[%fp+-224]
set 1,%r26
stb %r26,[%fp+-136]
set 1,%r26
stb %r26,[%fp+-168]
set 1,%r26
stb %r26,[%fp+-194]
set 1,%r26
stb %r26,[%fp+-135]
set 1,%r26
stb %r26,[%fp+-167]
set 1,%r26
stb %r26,[%fp+-212]
set 1,%r26
stb %r26,[%fp+-134]
set 1,%r26
stb %r26,[%fp+-166]
set 1,%r26
stb %r26,[%fp+-210]
set 1,%r26
stb %r26,[%fp+-193]
set 1,%r26
stb %r26,[%fp+-209]
add -256,%fp,%r8
call _sumof; nop
cmp %r8,98; be L309; nop
add %r27,1,%r27
ld [%r24+44],%r26
cmp %r26,%r0; be L311; nop
set _207,%r8
set 1,%r9
call _printf; nop
L311:
L309:
ba L313; nop
add %r27,8,%r27
ld [%r24+44],%r26
cmp %r26,%r0; be L315; nop
set _207,%r8
set 8,%r9
call _printf; nop
L315:
L313:
mov %r27,%r24
L206:
ret; restore
.global _zerofill
.align 4
.proc 4
_zerofill:
mov %r0,%r13
L318:
mov %r8,%r12
add %r12,1,%r8
stb %r0,[%r12]
L319:
add %r13,1,%r13
cmp %r13,256; bl L318; nop
L317:
retl; nop
.global _sumof
.align 4
.proc 4
_sumof:
mov %r8,%r11
mov %r0,%r13
mov %r0,%r12
L323:
mov %r11,%r10
add %r10,1,%r11
ldsb [%r10],%r10
add %r13,%r10,%r13
L324:
add %r12,1,%r12
cmp %r12,256; bl L323; nop
mov %r13,%r8
L322:
retl; nop
.seg "data"
_328:.ascii "s244,er%d\012\000"
_329:.ascii "s244   \000"
.seg "text"
.global _s244
.align 4
.proc 4
_s244:save %sp,-160,%sp
set _329,%r28
add %r24,60,%r27
L330:
L331:
mov %r27,%r23
add %r23,1,%r27
mov %r28,%r22
add %r22,1,%r28
ldsb [%r22],%r22
stb %r22,[%r23]
sll %r22,24,%g1; sra %g1,24,%r23
cmp %r23,%r0; bne L330; nop
mov %r0,%r25
mov %r0,%r26
set L333,%r23
ld [%r23],%f30; ld [%r23+4],%f31
st %f30,[%fp+-64]; st %f31,[%fp+-64+4]
st %f30,[%fp+-56]; st %f31,[%fp+-56+4]
set L333,%r23
ld [%r23],%f30; ld [%r23+4],%f31
st %f30,[%fp+-48]; st %f31,[%fp+-48+4]
set L333,%r23
ld [%r23],%f30; ld [%r23+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
set L333,%r23
ld [%r23],%f30; ld [%r23+4],%f31
st %f30,[%fp+-32]; st %f31,[%fp+-32+4]
set L333,%r23
ld [%r23],%f30; ld [%r23+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L333,%r23
ld [%r23],%f30; ld [%r23+4],%f31
st %f30,[%fp+-16]; st %f31,[%fp+-16+4]
set L333,%r23
ld [%r23],%f30; ld [%r23+4],%f31
st %f30,[%fp+-8]; st %f31,[%fp+-8+4]
mov %r0,%r26
mov %r0,%r29
L341:
sll %r29,3,%r23
add -64,%fp,%r22
add %r23,%r22,%r22
ld [%r22],%f30; ld [%r22+4],%f31
add -56,%fp,%r22
add %r23,%r22,%r23
ld [%r23],%f28; ld [%r23+4],%f29
fcmped %f30,%f28; nop; fbue L345; nop
set 1,%r26
L345:
L342:
add %r29,1,%r29
cmp %r29,7; bl L341; nop
cmp %r26,%r0; be L348; nop
ld [%r24+44],%r23
cmp %r23,%r0; be L350; nop
set _328,%r8
set 1,%r9
call _printf; nop
L350:
add %r25,1,%r25
L348:
ba L352; nop
ld [%r24+44],%r23
cmp %r23,%r0; be L354; nop
set _328,%r8
set 2,%r9
call _printf; nop
L354:
add %r25,2,%r25
L352:
mov %r25,%r24
L327:
ret; restore
.seg "data"
_357:.ascii "s25,er%d\012\000"
_358:.ascii "s25    \000"
.seg "text"
.global _s25
.align 4
.proc 4
_s25:save %sp,-96,%sp
set _358,%r26
add %r24,60,%r25
L359:
L360:
mov %r25,%r21
add %r21,1,%r25
mov %r26,%r20
add %r20,1,%r26
ldsb [%r20],%r20
stb %r20,[%r21]
sll %r20,24,%g1; sra %g1,24,%r21
cmp %r21,%r0; bne L359; nop
mov %r0,%r23
set L362,%r28
ldsb [%r28],%r21
ldsb [%r28+1],%r20
cmp %r21,%r20; bne L366; nop
ldsb [%r28+2],%r21
cmp %r20,%r21; bne L366; nop
cmp %r21,46; be L363; nop
L366:
add %r23,1,%r23
ld [%r24+44],%r21
cmp %r21,%r0; be L367; nop
set _357,%r8
set 1,%r9
call _printf; nop
L367:
L363:
ldsb [%r28+3],%r21
cmp %r21,%r0; be L369; nop
add %r23,4,%r23
ld [%r24+44],%r21
cmp %r21,%r0; be L371; nop
set _357,%r8
set 4,%r9
call _printf; nop
L371:
L369:
set L375+1,%r21
ldsb [%r21],%r21
cmp %r21,34; be L373; nop
add %r23,8,%r23
ld [%r24+44],%r21
cmp %r21,%r0; be L377; nop
set _357,%r8
set 8,%r9
call _printf; nop
L377:
L373:
set L379,%r28
ldsb [%r28],%r21
cmp %r21,10; bne L387; nop
ldsb [%r28+1],%r21
cmp %r21,9; bne L387; nop
ldsb [%r28+2],%r21
cmp %r21,8; bne L387; nop
ldsb [%r28+3],%r21
cmp %r21,13; bne L387; nop
ldsb [%r28+4],%r21
cmp %r21,12; bne L387; nop
ldsb [%r28+5],%r21
cmp %r21,92; bne L387; nop
ldsb [%r28+6],%r21
cmp %r21,39; be L380; nop
L387:
add %r23,16,%r23
ld [%r24+44],%r21
cmp %r21,%r0; be L388; nop
set _357,%r8
set 16,%r9
call _printf; nop
L388:
L380:
set L390,%r21
mov %r21,%r27
mov %r21,%r28
mov %r0,%r22
mov %r0,%r29
ba L394; nop
L391:
ldsb [%r29+%r28],%r21
ldsb [%r29+%r27],%r20
cmp %r21,%r20; be L395; nop
set 1,%r22
L395:
L392:
add %r29,1,%r29
L394:
cmp %r29,7; blu L391; nop
cmp %r22,%r0; be L397; nop
add %r23,32,%r23
ld [%r24+44],%r21
cmp %r21,%r0; be L399; nop
set _357,%r8
set 32,%r9
call _printf; nop
L399:
L397:
mov %r23,%r24
L356:
ret; restore
.seg "data"
_402:.ascii "s26    \000"
_403:.ascii "%3d bits in %ss.\012\000"
_404:.ascii "%e is the least number that can be added to 1. (%s).\012\000"
.seg "text"
.global _s26
.align 4
.proc 4
_s26:save %sp,-128,%sp
set _402,%r28
add %r24,60,%r27
L405:
L406:
mov %r27,%r25
add %r25,1,%r27
mov %r28,%r23
add %r23,1,%r28
ldsb [%r23],%r23
stb %r23,[%r25]
sll %r23,24,%g1; sra %g1,24,%r25
cmp %r25,%r0; bne L405; nop
st %r0,[%r24]
mov %r0,%r26
set 1,%r29
ba L409; nop
L408:
sll %r29,24,%g1; sra %g1,24,%r25
sll %r25,1,%r29
ld [%r24],%r25
add %r25,1,%r25
st %r25,[%r24]
L409:
sll %r26,24,%g1; sra %g1,24,%r25
sll %r29,24,%g1; sra %g1,24,%r23
cmp %r25,%r23; bne L408; nop
ld [%r24],%r25
sll %r25,2,%r25
st %r25,[%r24+4]
ld [%r24],%r25
sll %r25,1,%r25
st %r25,[%r24+8]
ld [%r24],%r25
sll %r25,2,%r25
st %r25,[%r24+12]
ld [%r24],%r25
sll %r25,2,%r25
st %r25,[%r24+16]
ld [%r24],%r25
sll %r25,2,%r25
st %r25,[%r24+20]
ld [%r24],%r25
sll %r25,3,%r25
st %r25,[%r24+24]
set L411,%r25
ld [%r25],%f31
st %f31,[%fp+-12]
st %f31,[%fp+-4]
set L412,%r25
ld [%r25],%f31
st %f31,[%fp+-8]
ba L414; nop
L413:
ld [%fp+-12],%f31
ld [%fp+-4],%f30
fadds %f31,%f30,%f31
st %f31,[%fp+-8]
fstod %f30,%f30
set L416,%r25
ld [%r25],%f28; ld [%r25+4],%f29
fdivd %f30,%f28,%f30
fdtos %f30,%f31
st %f31,[%fp+-4]
L414:
ld [%fp+-8],%f31
ld [%fp+-12],%f30
fcmpes %f31,%f30; nop; fbne L413; nop
set L417,%r25
ld [%r25],%f30; ld [%r25+4],%f31
ld [%fp+-4],%f29
fstod %f29,%f28
fmuld %f30,%f28,%f30
fdtos %f30,%f31
st %f31,[%r24+28]
set L418,%r25
ld [%r25],%f30; ld [%r25+4],%f31
st %f30,[%fp+-32]; st %f31,[%fp+-32+4]
set L411,%r25
ld [%r25],%f31
st %f31,[%fp+-4]
set L419,%r25
ld [%r25],%f30; ld [%r25+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ba L421; nop
L420:
ld [%fp+-32],%f30; ld [%fp+-32+4],%f31
ld [%fp+-4],%f29
fstod %f29,%f28
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L416,%r25
ld [%r25],%f30; ld [%r25+4],%f31
fdivd %f28,%f30,%f30
fdtos %f30,%f31
st %f31,[%fp+-4]
L421:
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-32],%f28; ld [%fp+-32+4],%f29
fcmped %f30,%f28; nop; fbne L420; nop
set L417,%r25
ld [%r25],%f30; ld [%r25+4],%f31
ld [%fp+-4],%f29
fstod %f29,%f28
fmuld %f30,%f28,%f30
fdtos %f30,%f31
st %f31,[%r24+32]
ld [%r24+40],%r25
cmp %r25,%r0; be L423; nop
set _403,%r8
ld [%r24],%r9
set L425,%r10
call _printf; nop
set _403,%r8
ld [%r24+4],%r9
set L426,%r10
call _printf; nop
set _403,%r8
ld [%r24+8],%r9
set L427,%r10
call _printf; nop
set _403,%r8
ld [%r24+12],%r9
set L428,%r10
call _printf; nop
set _403,%r8
ld [%r24+16],%r9
set L429,%r10
call _printf; nop
set _403,%r8
ld [%r24+20],%r9
set L430,%r10
call _printf; nop
set _403,%r8
ld [%r24+24],%r9
set L431,%r10
call _printf; nop
set _404,%r8
ld [%r24+28],%f31
fstod %f31,%f30
st %f30,[%sp+4*1+68]
st %f31,[%sp+4*2+68]
ld [%sp+4*1+68],%o1
ld [%sp+4*2+68],%o2
set L430,%r11
call _printf; nop
set _404,%r8
ld [%r24+32],%f31
fstod %f31,%f30
st %f30,[%sp+4*1+68]
st %f31,[%sp+4*2+68]
ld [%sp+4*1+68],%o1
ld [%sp+4*2+68],%o2
set L431,%r11
call _printf; nop
L423:
mov %r0,%r24
L401:
ret; restore
.seg "data"
_433:.ascii "s4,er%d\012\000"
_434:.ascii "s4     \000"
.seg "text"
.global _s4
.align 4
.proc 4
_s4:save %sp,-96,%sp
mov %r0,%r23
set _434,%r26
add %r24,60,%r25
L435:
L436:
mov %r25,%r19
add %r19,1,%r25
mov %r26,%r18
add %r18,1,%r26
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L435; nop
mov %r0,%r29
L438:
mov %r29,%r8
call _svtest; nop
st %r8,[%fp+-4]
call _zero; nop
ld [%fp+-4],%r19
cmp %r19,%r8; be L442; nop
set 1,%r23
ld [%r24+44],%r19
cmp %r19,%r0; be L444; nop
set _433,%r8
set 1,%r9
call _printf; nop
L444:
L442:
L439:
add %r29,1,%r29
cmp %r29,3; bl L438; nop
call _setev; nop
call _testev; nop
cmp %r8,%r0; be L446; nop
add %r23,2,%r23
ld [%r24+44],%r19
cmp %r19,%r0; be L448; nop
set _433,%r8
set 2,%r9
call _printf; nop
L448:
L446:
ba L450; nop
add %r23,4,%r23
ld [%r24+44],%r19
cmp %r19,%r0; be L452; nop
set _433,%r8
set 4,%r9
call _printf; nop
L452:
L450:
mov %r0,%r28
sub %r28,1,%r28
set 1,%r27
mov %r0,%r29
ba L457; nop
L454:
and %r27,%r28,%r27
srl %r28,1,%r28
L455:
add %r29,1,%r29
L457:
ld [%r24],%r19
sll %r19,2,%r19
cmp %r29,%r19; blu L454; nop
cmp %r27,1; bne L460; nop
cmp %r28,%r0; be L458; nop
L460:
add %r23,8,%r23
ld [%r24+44],%r19
cmp %r19,%r0; be L461; nop
set _433,%r8
set 8,%r9
call _printf; nop
L461:
L458:
mov %r23,%r24
L432:
ret; restore
.seg "bss"
.align 4; .reserve _465,4
.seg "text"
.global _svtest
.align 4
.proc 4
_svtest:
cmp %r8,%r0; be L468; nop
cmp %r8,1; be L469; nop
cmp %r8,2; be L472; nop
ba L466; nop
L468:
set 1978,%r12
set _465,%r11
st %r12,[%r11]
mov %r0,%r13
ba L467; nop
L469:
set _465,%r12
ld [%r12],%r12
cmp %r12,1978; be L470; nop
set 1,%r13
ba L467; nop
L470:
set 1929,%r12
set _465,%r11
st %r12,[%r11]
mov %r0,%r13
ba L467; nop
L472:
set _465,%r12
ld [%r12],%r12
cmp %r12,1929; be L473; nop
set 1,%r13
ba L467; nop
L473:
mov %r0,%r13
L466:
L467:
mov %r13,%r8
L464:
retl; nop
.seg "bss"
.align 4; .reserve _476,4
.seg "text"
.global _zero
.align 4
.proc 4
_zero:
set 2,%r12
set _476,%r11
st %r12,[%r11]
mov %r0,%r13
mov %r13,%r8
L475:
retl; nop
.global _testev
.align 4
.proc 4
_testev:
set _extvar,%r13
ld [%r13],%r13
cmp %r13,1066; be L478; nop
set 1,%r8
ba L477; nop
L478:
mov %r0,%r8
L477:
retl; nop
.seg "data"
_481:.ascii "s61,er%d\012\000"
_482:.ascii "s61    \000"
_483:.ascii "ABCDEFGHIJKLMNOPQRSTUVWXYZ\000"
_484:.ascii "abcdefghijklmnopqrstuvwxyz\000"
_485:.ascii "0123456789\000"
_486:.ascii "~!\"#%&()_=-^|{}[]+;*:<>,.?/\000"
_487:.byte 10,9,8,13,12,92,39,0
_488:.byte 32,0,0
.seg "text"
.global _s61
.align 4
.proc 4
_s61:save %sp,-120,%sp
set _482,%r27
add %r24,60,%r26
mov %r0,%r23
L489:
L490:
mov %r26,%r19
add %r19,1,%r26
mov %r27,%r18
add %r18,1,%r27
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L489; nop
set -19,%r20
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%fp+-28]
ld [%fp+-28],%r19
cmp %r19,-19; be L492; nop
add %r23,1,%r23
ld [%r24+44],%r19
cmp %r19,%r0; be L494; nop
set _481,%r8
set 1,%r9
call _printf; nop
L494:
L492:
set _483,%r19
st %r19,[%fp+-24]
set _484,%r19
st %r19,[%fp+-20]
set _485,%r19
st %r19,[%fp+-16]
set _486,%r19
st %r19,[%fp+-12]
set _487,%r19
st %r19,[%fp+-8]
set _488,%r19
st %r19,[%fp+-4]
mov %r0,%r28
mov %r0,%r29
ba L506; nop
L505:
sll %r29,2,%r19
add -24,%fp,%r18
add %r19,%r18,%r19
ld [%r19],%r18
add %r18,1,%r17
st %r17,[%r19]
ldsb [%r18],%r19
cmp %r19,%r0; bge L508; nop
set 1,%r28
L508:
L506:
sll %r29,2,%r19
add -24,%fp,%r18
ld [%r19+%r18],%r19
ldsb [%r19],%r19
cmp %r19,%r0; bne L505; nop
L502:
add %r29,1,%r29
cmp %r29,6; bl L506; nop
cmp %r28,%r0; be L510; nop
add %r23,2,%r23
ld [%r24+44],%r19
cmp %r19,%r0; be L512; nop
set _481,%r8
set 2,%r9
call _printf; nop
L512:
L510:
set 1048579,%r25
mov %r25,%r22
mov %r25,%r21
sll %r22,16,%g1; sra %g1,16,%r19
cmp %r19,%r25; be L517; nop
cmp %r19,3; bne L516; nop
L517:
sll %r21,24,%g1; sra %g1,24,%r19
cmp %r19,%r25; be L514; nop
cmp %r19,3; be L514; nop
L516:
add %r23,8,%r23
ld [%r24+44],%r19
cmp %r19,%r0; be L518; nop
set _481,%r8
set 8,%r9
call _printf; nop
L518:
L514:
mov %r23,%r24
L480:
ret; restore
.seg "data"
_521:.ascii "s626,er%d\012\000"
_522:.ascii "s626   \000"
.seg "text"
.global _s626
.align 4
.proc 4
_s626:save %sp,-200,%sp
set _522,%r26
add %r24,60,%r25
st %r0,[%fp+-76]
L523:
L524:
mov %r25,%r19
add %r19,1,%r25
mov %r26,%r18
add %r18,1,%r26
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L523; nop
set L411,%r19
ld [%r19],%f31
st %f31,[%fp+-32]
set 1,%r19
mov %r19,%r23
mov %r19,%r28
mov %r0,%r29
ba L529; nop
L526:
set L530,%r19
ld [%r19],%f31
ld [%fp+-32],%f30
fmuls %f31,%f30,%f31
st %f31,[%fp+-32]
sll %r28,1,%r19
or %r19,%r23,%r28
L527:
add %r29,1,%r29
L529:
ld [%r24+12],%r19
sub %r19,2,%r19
cmp %r29,%r19; bl L526; nop
st %r28,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
st %f31,[%fp+-84]
ld [%fp+-32],%f31
ld [%fp+-84],%f30
fsubs %f31,%f30,%f30
fdivs %f30,%f31,%f31
st %f31,[%fp+-32]
ld [%fp+-32],%f31
fstod %f31,%f30
set L416,%r19
ld [%r19],%f28; ld [%r19+4],%f29
ld [%r24+28],%f27
fstod %f27,%f26
fmuld %f28,%f26,%f28
fcmped %f30,%f28; nop; fbule L531; nop
ld [%fp+-76],%r19
add %r19,2,%r19
st %r19,[%fp+-76]
ld [%r24+44],%r19
cmp %r19,%r0; be L533; nop
set _521,%r8
set 2,%r9
call _printf; nop
L533:
L531:
set 125,%r21
set 125,%r20
set 125,%r19
st %r19,[%fp+-48]
set 15625,%r19
st %r19,[%fp+-68]
set 125,%r19
st %r19,[%fp+-52]
set 15625,%r19
st %r19,[%fp+-72]
set 125,%r22
set 15625,%r19
st %r19,[%fp+-80]
set L535,%r19
ld [%r19],%f31
st %f31,[%fp+-44]
set L536,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-64]; st %f31,[%fp+-64+4]
set L537,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
mov %r0,%r29
L538:
add -28,%fp,%r19
stb %r0,[%r29+%r19]
L539:
add %r29,1,%r29
cmp %r29,28; bl L538; nop
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-68],%r18
cmp %r19,%r18; be L542; nop
set 1,%r19
stb %r19,[%fp+-28]
L542:
sll %r20,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-68],%r18
cmp %r19,%r18; be L544; nop
set 1,%r19
stb %r19,[%fp+-27]
L544:
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-68],%r18
cmp %r19,%r18; be L547; nop
set 1,%r19
stb %r19,[%fp+-26]
L547:
ld [%fp+-48],%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-68],%r18
cmp %r19,%r18; be L550; nop
set 1,%r19
stb %r19,[%fp+-25]
L550:
ld [%fp+-48],%r19
sll %r20,16,%g1; sra %g1,16,%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-68],%r18
cmp %r19,%r18; be L553; nop
set 1,%r19
stb %r19,[%fp+-24]
L553:
ld [%fp+-48],%r19
mov %r19,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-68],%r18
cmp %r19,%r18; be L556; nop
set 1,%r19
stb %r19,[%fp+-23]
L556:
ld [%fp+-52],%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-72],%r18
cmp %r19,%r18; be L559; nop
set 1,%r19
stb %r19,[%fp+-22]
L559:
ld [%fp+-52],%r19
sll %r20,16,%g1; sra %g1,16,%r18
mov %r18,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-72],%r18
cmp %r19,%r18; be L562; nop
set 1,%r19
stb %r19,[%fp+-21]
L562:
ld [%fp+-52],%r19
ld [%fp+-48],%r18
mov %r18,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-72],%r18
cmp %r19,%r18; be L565; nop
set 1,%r19
stb %r19,[%fp+-20]
L565:
ld [%fp+-52],%r19
mov %r19,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-72],%r18
cmp %r19,%r18; be L568; nop
set 1,%r19
stb %r19,[%fp+-19]
L568:
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .mul,2; mov %r22,%o0; mov %o0,%r19
ld [%fp+-80],%r18
cmp %r19,%r18; be L571; nop
set 1,%r19
stb %r19,[%fp+-18]
L571:
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .mul,2; mov %r22,%o0; mov %o0,%r19
ld [%fp+-80],%r18
cmp %r19,%r18; be L574; nop
set 1,%r19
stb %r19,[%fp+-17]
L574:
ld [%fp+-48],%r19
mov %r19,%o1; call .mul,2; mov %r22,%o0; mov %o0,%r19
ld [%fp+-80],%r18
cmp %r19,%r18; be L577; nop
set 1,%r19
stb %r19,[%fp+-16]
L577:
mov %r22,%r19
ld [%fp+-52],%r18
mov %r18,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-72],%r18
cmp %r19,%r18; be L580; nop
set 1,%r19
stb %r19,[%fp+-15]
L580:
mov %r22,%o1; call .mul,2; mov %r22,%o0; mov %o0,%r19
ld [%fp+-80],%r18
cmp %r19,%r18; be L583; nop
set 1,%r19
stb %r19,[%fp+-14]
L583:
ld [%fp+-44],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L586; nop
set 1,%r19
stb %r19,[%fp+-13]
L586:
ld [%fp+-44],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L589; nop
set 1,%r19
stb %r19,[%fp+-12]
L589:
ld [%fp+-44],%f31
ld [%fp+-48],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L592; nop
set 1,%r19
stb %r19,[%fp+-11]
L592:
ld [%fp+-52],%r19
cmp %r19,%r0; bl L598; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-96]; st %f31,[%fp+-96+4]
ba L599; nop
L598:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-96]; st %f31,[%fp+-96+4]
L599:
ld [%fp+-44],%f31
ld [%fp+-96],%f28; ld [%fp+-96+4],%f29
fdtos %f28,%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L595; nop
set 1,%r19
stb %r19,[%fp+-10]
L595:
ld [%fp+-44],%f31
st %r22,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L602; nop
set 1,%r19
stb %r19,[%fp+-9]
L602:
ld [%fp+-44],%f31
fmuls %f31,%f31,%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L605; nop
set 1,%r19
stb %r19,[%fp+-8]
L605:
ld [%fp+-64],%f30; ld [%fp+-64+4],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fmuld %f30,%f28,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L608; nop
set 1,%r19
stb %r19,[%fp+-7]
L608:
ld [%fp+-64],%f30; ld [%fp+-64+4],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fmuld %f30,%f28,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L611; nop
set 1,%r19
stb %r19,[%fp+-6]
L611:
ld [%fp+-64],%f30; ld [%fp+-64+4],%f31
ld [%fp+-48],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fmuld %f30,%f28,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L614; nop
set 1,%r19
stb %r19,[%fp+-5]
L614:
ld [%fp+-52],%r19
cmp %r19,%r0; bl L620; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-104]; st %f31,[%fp+-104+4]
ba L621; nop
L620:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-104]; st %f31,[%fp+-104+4]
L621:
ld [%fp+-64],%f30; ld [%fp+-64+4],%f31
ld [%fp+-104],%f28; ld [%fp+-104+4],%f29
fmuld %f30,%f28,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L617; nop
set 1,%r19
stb %r19,[%fp+-4]
L617:
ld [%fp+-64],%f30; ld [%fp+-64+4],%f31
st %r22,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fmuld %f30,%f28,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L623; nop
set 1,%r19
stb %r19,[%fp+-3]
L623:
ld [%fp+-64],%f30; ld [%fp+-64+4],%f31
ld [%fp+-44],%f29
fstod %f29,%f28
fmuld %f30,%f28,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L626; nop
set 1,%r19
stb %r19,[%fp+-2]
L626:
ld [%fp+-64],%f30; ld [%fp+-64+4],%f31
fmuld %f30,%f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fcmped %f30,%f28; nop; fbue L629; nop
set 1,%r19
stb %r19,[%fp+-1]
L629:
mov %r0,%r27
mov %r0,%r29
L632:
sll %r27,24,%g1; sra %g1,24,%r19
add -28,%fp,%r18
ldsb [%r29+%r18],%r18
add %r19,%r18,%r27
L633:
add %r29,1,%r29
cmp %r29,28; bl L632; nop
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; be L636; nop
ld [%fp+-76],%r19
add %r19,4,%r19
st %r19,[%fp+-76]
ld [%r24+44],%r19
cmp %r19,%r0; be L638; nop
set _521,%r8
set 4,%r9
call _printf; nop
set L640,%r8
call _printf; nop
mov %r0,%r29
L641:
set L645,%r8
add -28,%fp,%r19
ldsb [%r29+%r19],%r9
call _printf; nop
L642:
add %r29,1,%r29
cmp %r29,28; bl L641; nop
set L646,%r8
call _printf; nop
L638:
L636:
set 32768,%r22
set 0x8000,%r19
cmp %r22,%r19; bleu L647; nop
ld [%fp+-76],%r19
add %r19,8,%r19
st %r19,[%fp+-76]
ld [%r24+44],%r19
cmp %r19,%r0; be L649; nop
set _521,%r8
set 8,%r9
call _printf; nop
L649:
L647:
ld [%fp+-76],%r24
L520:
ret; restore
.seg "data"
_652:.ascii "s71,er%d\012\000"
_653:.ascii "s71    \000"
_654:.byte 113
.seg "text"
.global _s71
.align 4
.proc 4
_s71:save %sp,-136,%sp
set _653,%r29
add %r24,60,%r28
mov %r0,%r27
L655:
L656:
mov %r28,%r23
add %r23,1,%r28
mov %r29,%r22
add %r22,1,%r29
ldsb [%r22],%r22
stb %r22,[%r23]
sll %r22,24,%g1; sra %g1,24,%r23
cmp %r23,%r0; bne L655; nop
set L660,%r23
ldsb [%r23],%r23
set _654,%r22
ldsb [%r22],%r22
cmp %r23,%r22; be L658; nop
add %r27,1,%r27
ld [%r24+44],%r23
cmp %r23,%r0; be L661; nop
set _652,%r8
set 1,%r9
call _printf; nop
L661:
L658:
ba L663; nop
add %r27,2,%r27
ld [%r24+44],%r23
cmp %r23,%r0; be L665; nop
set _652,%r8
set 2,%r9
call _printf; nop
L665:
L663:
set 1942,%r23
st %r23,[%fp+-20]
ld [%fp+-20],%r23
cmp %r23,1942; bne L673; nop
ld [%fp+-20],%r23
ld [%fp+-20],%r22
cmp %r23,%r22; be L668; nop
L673:
add %r27,4,%r27
ld [%r24+44],%r23
cmp %r23,%r0; be L674; nop
set _652,%r8
set 4,%r9
call _printf; nop
L674:
L668:
set -5,%r8
call _McCarthy; nop
cmp %r8,91; be L676; nop
add %r27,8,%r27
ld [%r24+44],%r23
cmp %r23,%r0; be L678; nop
set _652,%r8
set 8,%r9
call _printf; nop
L678:
L676:
set 2,%r26
set 3,%r23
st %r23,[%fp+-44]
add -44,%fp,%r25
mov %r26,%r8
mov %r25,%r9
call _clobber; nop
cmp %r26,2; bne L682; nop
ld [%fp+-44],%r23
cmp %r23,2; be L680; nop
L682:
add %r27,16,%r27
ld [%r24+44],%r23
cmp %r23,%r0; be L683; nop
set _652,%r8
set 16,%r9
call _printf; nop
L683:
L680:
ld [%r24+32],%f31
fcmpes %f31,%f31; nop; fbue L685; nop
add %r27,32,%r27
ld [%r24+44],%r23
cmp %r23,%r0; be L687; nop
set _652,%r8
set 32,%r9
call _printf; nop
L687:
L685:
mov %r27,%r24
L651:
ret; restore
.global _McCarthy
.align 4
.proc 4
_McCarthy:save %sp,-96,%sp
cmp %r24,100; ble L690; nop
sub %r24,10,%r24
ba L689; nop
L690:
add %r24,11,%r8
call _McCarthy; nop
call _McCarthy; nop
mov %r8,%r24
L689:
ret; restore
.global _clobber
.align 4
.proc 4
_clobber:
set 3,%r8
set 2,%r13
st %r13,[%r9]
L692:
retl; nop
.seg "data"
_694:.ascii "Local error %d.\012\000"
_695:.ascii "s714,er%d\012\000"
_696:.ascii "s714   \000"
.seg "text"
.global _s714
.align 4
.proc 4
_s714:save %sp,-368,%sp
set _696,%r19
st %r19,[%fp+-44]
add %r24,60,%r19
st %r19,[%fp+-48]
st %r0,[%fp+-52]
mov %r0,%r28
ld [%r24+48],%r29
L697:
L698:
ld [%fp+-48],%r19
add %r19,1,%r18
st %r18,[%fp+-48]
ld [%fp+-44],%r18
add %r18,1,%r17
st %r17,[%fp+-44]
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L697; nop
set 5,%r27
set 2,%r21
mov %r21,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L700; nop
set 1,%r28
cmp %r29,%r0; be L702; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L702:
L700:
set 5,%r27
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L704; nop
set 2,%r28
cmp %r29,%r0; be L706; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L706:
L704:
set 5,%r27
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L708; nop
set 3,%r28
cmp %r29,%r0; be L710; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L710:
L708:
set 5,%r27
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L712; nop
set 4,%r28
cmp %r29,%r0; be L714; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L714:
L712:
set 5,%r27
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L716; nop
set 5,%r28
cmp %r29,%r0; be L718; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L718:
L716:
set 5,%r27
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-28],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L720; nop
set 6,%r28
cmp %r29,%r0; be L722; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L722:
L720:
set 5,%r27
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-40],%f30; ld [%fp+-40+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L724; nop
set 7,%r28
cmp %r29,%r0; be L726; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L726:
L724:
set 5,%r26
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L728; nop
set 8,%r28
cmp %r29,%r0; be L730; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L730:
L728:
set 5,%r26
set 2,%r20
mov %r20,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L732; nop
set 9,%r28
cmp %r29,%r0; be L734; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L734:
L732:
set 5,%r26
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L736; nop
set 10,%r28
cmp %r29,%r0; be L738; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L738:
L736:
set 5,%r26
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L740; nop
set 11,%r28
cmp %r29,%r0; be L742; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L742:
L740:
set 5,%r26
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L744; nop
set 12,%r28
cmp %r29,%r0; be L746; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L746:
L744:
set 5,%r26
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-28],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L748; nop
set 13,%r28
cmp %r29,%r0; be L750; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L750:
L748:
set 5,%r26
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-40],%f30; ld [%fp+-40+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L752; nop
set 14,%r28
cmp %r29,%r0; be L754; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L754:
L752:
set 5,%r25
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%r25
cmp %r25,2; be L756; nop
set 15,%r28
cmp %r29,%r0; be L758; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L758:
L756:
set 5,%r25
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%r25
cmp %r25,2; be L760; nop
set 16,%r28
cmp %r29,%r0; be L762; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L762:
L760:
set 5,%r25
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r25
cmp %r25,2; be L764; nop
set 17,%r28
cmp %r29,%r0; be L766; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L766:
L764:
set 5,%r25
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r25
cmp %r25,2; be L768; nop
set 18,%r28
cmp %r29,%r0; be L770; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L770:
L768:
set 5,%r25
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r25
cmp %r25,2; be L772; nop
set 19,%r28
cmp %r29,%r0; be L774; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L774:
L772:
set 5,%r25
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-28],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,2; be L776; nop
set 20,%r28
cmp %r29,%r0; be L778; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L778:
L776:
set 5,%r25
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-40],%f30; ld [%fp+-40+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,2; be L780; nop
set 21,%r28
cmp %r29,%r0; be L782; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L782:
L780:
set 5,%r23
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%r23
cmp %r23,2; be L784; nop
set 22,%r28
cmp %r29,%r0; be L786; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L786:
L784:
set 5,%r23
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%r23
cmp %r23,2; be L788; nop
set 23,%r28
cmp %r29,%r0; be L790; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L790:
L788:
set 5,%r23
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r23
cmp %r23,2; be L792; nop
set 24,%r28
cmp %r29,%r0; be L794; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L794:
L792:
set 5,%r23
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r23
cmp %r23,2; be L796; nop
set 25,%r28
cmp %r29,%r0; be L798; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L798:
L796:
set 5,%r23
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r23
cmp %r23,2; be L800; nop
set 26,%r28
cmp %r29,%r0; be L802; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L802:
L800:
set 5,%r23
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-28],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,2; be L804; nop
set 27,%r28
cmp %r29,%r0; be L806; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L806:
L804:
set 5,%r23
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-40],%f30; ld [%fp+-40+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,2; be L808; nop
set 28,%r28
cmp %r29,%r0; be L810; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L810:
L808:
set 5,%r22
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%r22
cmp %r22,2; be L812; nop
set 29,%r28
cmp %r29,%r0; be L814; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L814:
L812:
set 5,%r22
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%r22
cmp %r22,2; be L816; nop
set 30,%r28
cmp %r29,%r0; be L818; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L818:
L816:
set 5,%r22
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r22
cmp %r22,2; be L820; nop
set 31,%r28
cmp %r29,%r0; be L822; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L822:
L820:
set 5,%r22
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r22
cmp %r22,2; be L824; nop
set 32,%r28
cmp %r29,%r0; be L826; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L826:
L824:
set 5,%r22
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r22
cmp %r22,2; be L828; nop
set 33,%r28
cmp %r29,%r0; be L830; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L830:
L828:
set 5,%r22
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-28],%f31
fstod %f31,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L833; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-56]
ba L834; nop
L833:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-56]
L834:
ld [%fp+-56],%r22
cmp %r22,2; be L836; nop
set 34,%r28
cmp %r29,%r0; be L838; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L838:
L836:
set 5,%r22
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-40],%f30; ld [%fp+-40+4],%f31
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L841; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-60]
ba L842; nop
L841:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-60]
L842:
ld [%fp+-60],%r22
cmp %r22,2; be L843; nop
set 35,%r28
cmp %r29,%r0; be L845; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L845:
L843:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L530,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L848; nop
set 36,%r28
cmp %r29,%r0; be L850; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L850:
L848:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L530,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L852; nop
set 37,%r28
cmp %r29,%r0; be L854; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L854:
L852:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L530,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L856; nop
set 38,%r28
cmp %r29,%r0; be L858; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L858:
L856:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L530,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L860; nop
set 39,%r28
cmp %r29,%r0; be L862; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L862:
L860:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L865; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-72]; st %f31,[%fp+-72+4]
ba L866; nop
L865:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-72]; st %f31,[%fp+-72+4]
L866:
ld [%fp+-72],%f30; ld [%fp+-72+4],%f31
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L530,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L867; nop
set 40,%r28
cmp %r29,%r0; be L869; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L869:
L867:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-28],%f30
st %f30,[%fp+-16]
ld [%fp+-16],%f30
fcmpes %f30,%f31; nop; fbue L871; nop
set 41,%r28
cmp %r29,%r0; be L873; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L873:
L871:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-40],%f30; ld [%fp+-40+4],%f31
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L530,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L875; nop
set 42,%r28
cmp %r29,%r0; be L877; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L877:
L875:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L416,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L880; nop
set 43,%r28
cmp %r29,%r0; be L882; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L882:
L880:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L416,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L884; nop
set 44,%r28
cmp %r29,%r0; be L886; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L886:
L884:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L416,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L888; nop
set 45,%r28
cmp %r29,%r0; be L890; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L890:
L888:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L416,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L892; nop
set 46,%r28
cmp %r29,%r0; be L894; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L894:
L892:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L897; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-80]; st %f31,[%fp+-80+4]
ba L898; nop
L897:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-80]; st %f31,[%fp+-80+4]
L898:
ld [%fp+-80],%f30; ld [%fp+-80+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L416,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L899; nop
set 47,%r28
cmp %r29,%r0; be L901; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L901:
L899:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-28],%f31
fstod %f31,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L416,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L903; nop
set 48,%r28
cmp %r29,%r0; be L905; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L905:
L903:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
st %f28,[%fp+-24]; st %f29,[%fp+-24+4]
ld [%fp+-24],%f28; ld [%fp+-24+4],%f29
fcmped %f28,%f30; nop; fbue L907; nop
set 49,%r28
cmp %r29,%r0; be L909; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L909:
L907:
set 5,%r27
set 2,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
add %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,7; be L911; nop
set 50,%r28
cmp %r29,%r0; be L913; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L913:
L911:
set 5,%r27
set 2,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
add %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,7; be L915; nop
set 51,%r28
cmp %r29,%r0; be L917; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L917:
L915:
set 5,%r27
set 2,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
add %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,7; be L919; nop
set 52,%r28
cmp %r29,%r0; be L921; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L921:
L919:
set 5,%r27
set 2,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
add %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,7; be L923; nop
set 53,%r28
cmp %r29,%r0; be L925; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L925:
L923:
set 5,%r27
set 2,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
add %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,7; be L927; nop
set 54,%r28
cmp %r29,%r0; be L929; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L929:
L927:
set 5,%r27
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
sll %r27,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fadds %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,7; be L931; nop
set 55,%r28
cmp %r29,%r0; be L933; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L933:
L931:
set 5,%r27
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
sll %r27,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
faddd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,7; be L935; nop
set 56,%r28
cmp %r29,%r0; be L937; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L937:
L935:
set 5,%r26
set 2,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
add %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,7; be L939; nop
set 57,%r28
cmp %r29,%r0; be L941; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L941:
L939:
set 5,%r26
set 2,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
add %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,7; be L943; nop
set 58,%r28
cmp %r29,%r0; be L945; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L945:
L943:
set 5,%r26
set 2,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
add %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,7; be L947; nop
set 59,%r28
cmp %r29,%r0; be L949; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L949:
L947:
set 5,%r26
set 2,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
add %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,7; be L951; nop
set 60,%r28
cmp %r29,%r0; be L953; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L953:
L951:
set 5,%r26
set 2,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
add %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,7; be L955; nop
set 61,%r28
cmp %r29,%r0; be L957; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L957:
L955:
set 5,%r26
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
sll %r26,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fadds %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,7; be L959; nop
set 62,%r28
cmp %r29,%r0; be L961; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L961:
L959:
set 5,%r26
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
sll %r26,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
faddd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,7; be L963; nop
set 63,%r28
cmp %r29,%r0; be L965; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L965:
L963:
set 5,%r25
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
add %r25,%r19,%r25
cmp %r25,7; be L967; nop
set 64,%r28
cmp %r29,%r0; be L969; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L969:
L967:
set 5,%r25
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
add %r25,%r19,%r25
cmp %r25,7; be L971; nop
set 65,%r28
cmp %r29,%r0; be L973; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L973:
L971:
set 5,%r25
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
add %r25,%r19,%r25
cmp %r25,7; be L975; nop
set 66,%r28
cmp %r29,%r0; be L977; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L977:
L975:
set 5,%r25
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
add %r25,%r19,%r25
cmp %r25,7; be L979; nop
set 67,%r28
cmp %r29,%r0; be L981; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L981:
L979:
set 5,%r25
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
add %r25,%r19,%r25
cmp %r25,7; be L983; nop
set 68,%r28
cmp %r29,%r0; be L985; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L985:
L983:
set 5,%r25
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
st %r25,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fadds %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,7; be L987; nop
set 69,%r28
cmp %r29,%r0; be L989; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L989:
L987:
set 5,%r25
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
st %r25,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
faddd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,7; be L991; nop
set 70,%r28
cmp %r29,%r0; be L993; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L993:
L991:
set 5,%r23
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
add %r23,%r19,%r23
cmp %r23,7; be L995; nop
set 71,%r28
cmp %r29,%r0; be L997; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L997:
L995:
set 5,%r23
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
add %r23,%r19,%r23
cmp %r23,7; be L999; nop
set 72,%r28
cmp %r29,%r0; be L1001; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1001:
L999:
set 5,%r23
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
add %r23,%r19,%r23
cmp %r23,7; be L1003; nop
set 73,%r28
cmp %r29,%r0; be L1005; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1005:
L1003:
set 5,%r23
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
add %r23,%r19,%r23
cmp %r23,7; be L1007; nop
set 74,%r28
cmp %r29,%r0; be L1009; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1009:
L1007:
set 5,%r23
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
add %r23,%r19,%r23
cmp %r23,7; be L1011; nop
set 75,%r28
cmp %r29,%r0; be L1013; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1013:
L1011:
set 5,%r23
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fadds %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,7; be L1015; nop
set 76,%r28
cmp %r29,%r0; be L1017; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1017:
L1015:
set 5,%r23
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
faddd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,7; be L1019; nop
set 77,%r28
cmp %r29,%r0; be L1021; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1021:
L1019:
set 5,%r22
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
add %r22,%r19,%r22
cmp %r22,7; be L1023; nop
set 78,%r28
cmp %r29,%r0; be L1025; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1025:
L1023:
set 5,%r22
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
add %r22,%r19,%r22
cmp %r22,7; be L1027; nop
set 79,%r28
cmp %r29,%r0; be L1029; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1029:
L1027:
set 5,%r22
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
add %r22,%r19,%r22
cmp %r22,7; be L1031; nop
set 80,%r28
cmp %r29,%r0; be L1033; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1033:
L1031:
set 5,%r22
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
add %r22,%r19,%r22
cmp %r22,7; be L1035; nop
set 81,%r28
cmp %r29,%r0; be L1037; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1037:
L1035:
set 5,%r22
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
add %r22,%r19,%r22
cmp %r22,7; be L1039; nop
set 82,%r28
cmp %r29,%r0; be L1041; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1041:
L1039:
set 5,%r22
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
mov %r22,%r19
cmp %r19,%r0; bl L1047; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-96]; st %f31,[%fp+-96+4]
ba L1048; nop
L1047:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-96]; st %f31,[%fp+-96+4]
L1048:
ld [%fp+-96],%f30; ld [%fp+-96+4],%f31
fdtos %f30,%f31
ld [%fp+-28],%f30
fadds %f31,%f30,%f31
fstod %f31,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L1045; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-84]
ba L1046; nop
L1045:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-84]
L1046:
ld [%fp+-84],%r22
cmp %r22,7; be L1049; nop
set 83,%r28
cmp %r29,%r0; be L1051; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1051:
L1049:
set 5,%r22
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
mov %r22,%r19
cmp %r19,%r0; bl L1057; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-112]; st %f31,[%fp+-112+4]
ba L1058; nop
L1057:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-112]; st %f31,[%fp+-112+4]
L1058:
ld [%fp+-112],%f30; ld [%fp+-112+4],%f31
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
faddd %f30,%f28,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L1055; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-100]
ba L1056; nop
L1055:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-100]
L1056:
ld [%fp+-100],%r22
cmp %r22,7; be L1059; nop
set 84,%r28
cmp %r29,%r0; be L1061; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1061:
L1059:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r21
ld [%fp+-16],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fadds %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1065,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1063; nop
set 85,%r28
cmp %r29,%r0; be L1066; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1066:
L1063:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r20
ld [%fp+-16],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fadds %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1065,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1068; nop
set 86,%r28
cmp %r29,%r0; be L1070; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1070:
L1068:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-16],%f31
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fadds %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1065,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1072; nop
set 87,%r28
cmp %r29,%r0; be L1074; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1074:
L1072:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-16],%f31
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fadds %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1065,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1076; nop
set 88,%r28
cmp %r29,%r0; be L1078; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1078:
L1076:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L1081; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-120]; st %f31,[%fp+-120+4]
ba L1082; nop
L1081:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-120]; st %f31,[%fp+-120+4]
L1082:
ld [%fp+-16],%f31
ld [%fp+-120],%f28; ld [%fp+-120+4],%f29
fdtos %f28,%f30
fadds %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1065,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1083; nop
set 89,%r28
cmp %r29,%r0; be L1085; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1085:
L1083:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-16],%f31
ld [%fp+-28],%f30
fadds %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1065,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1087; nop
set 90,%r28
cmp %r29,%r0; be L1089; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1089:
L1087:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-16],%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
faddd %f30,%f28,%f30
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1065,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1091; nop
set 91,%r28
cmp %r29,%r0; be L1093; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1093:
L1091:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r21
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1097,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1095; nop
set 92,%r28
cmp %r29,%r0; be L1098; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1098:
L1095:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r20
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1097,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1100; nop
set 93,%r28
cmp %r29,%r0; be L1102; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1102:
L1100:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1097,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1104; nop
set 94,%r28
cmp %r29,%r0; be L1106; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1106:
L1104:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1097,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1108; nop
set 95,%r28
cmp %r29,%r0; be L1110; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1110:
L1108:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L1113; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-128]; st %f31,[%fp+-128+4]
ba L1114; nop
L1113:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-128]; st %f31,[%fp+-128+4]
L1114:
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-128],%f28; ld [%fp+-128+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1097,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1115; nop
set 96,%r28
cmp %r29,%r0; be L1117; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1117:
L1115:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-28],%f29
fstod %f29,%f28
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1097,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1119; nop
set 97,%r28
cmp %r29,%r0; be L1121; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1121:
L1119:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1097,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1123; nop
set 98,%r28
cmp %r29,%r0; be L1125; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1125:
L1123:
set 5,%r27
set 2,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
sub %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,3; be L1127; nop
set 99,%r28
cmp %r29,%r0; be L1129; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1129:
L1127:
set 5,%r27
set 2,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
sub %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,3; be L1131; nop
set 100,%r28
cmp %r29,%r0; be L1133; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1133:
L1131:
set 5,%r27
set 2,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
sub %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,3; be L1135; nop
set 101,%r28
cmp %r29,%r0; be L1137; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1137:
L1135:
set 5,%r27
set 2,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
sub %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,3; be L1139; nop
set 102,%r28
cmp %r29,%r0; be L1141; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1141:
L1139:
set 5,%r27
set 2,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
sub %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,3; be L1143; nop
set 103,%r28
cmp %r29,%r0; be L1145; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1145:
L1143:
set 5,%r27
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
sll %r27,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fsubs %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,3; be L1147; nop
set 104,%r28
cmp %r29,%r0; be L1149; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1149:
L1147:
set 5,%r27
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
sll %r27,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fsubd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,3; be L1151; nop
set 105,%r28
cmp %r29,%r0; be L1153; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1153:
L1151:
set 5,%r26
set 2,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
sub %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,3; be L1155; nop
set 106,%r28
cmp %r29,%r0; be L1157; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1157:
L1155:
set 5,%r26
set 2,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
sub %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,3; be L1159; nop
set 107,%r28
cmp %r29,%r0; be L1161; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1161:
L1159:
set 5,%r26
set 2,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
sub %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,3; be L1163; nop
set 108,%r28
cmp %r29,%r0; be L1165; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1165:
L1163:
set 5,%r26
set 2,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
sub %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,3; be L1167; nop
set 109,%r28
cmp %r29,%r0; be L1169; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1169:
L1167:
set 5,%r26
set 2,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
sub %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,3; be L1171; nop
set 110,%r28
cmp %r29,%r0; be L1173; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1173:
L1171:
set 5,%r26
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
sll %r26,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fsubs %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,3; be L1175; nop
set 111,%r28
cmp %r29,%r0; be L1177; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1177:
L1175:
set 5,%r26
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
sll %r26,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fsubd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,3; be L1179; nop
set 112,%r28
cmp %r29,%r0; be L1181; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1181:
L1179:
set 5,%r25
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
sub %r25,%r19,%r25
cmp %r25,3; be L1183; nop
set 113,%r28
cmp %r29,%r0; be L1185; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1185:
L1183:
set 5,%r25
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
sub %r25,%r19,%r25
cmp %r25,3; be L1187; nop
set 114,%r28
cmp %r29,%r0; be L1189; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1189:
L1187:
set 5,%r25
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
sub %r25,%r19,%r25
cmp %r25,3; be L1191; nop
set 115,%r28
cmp %r29,%r0; be L1193; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1193:
L1191:
set 5,%r25
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
sub %r25,%r19,%r25
cmp %r25,3; be L1195; nop
set 116,%r28
cmp %r29,%r0; be L1197; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1197:
L1195:
set 5,%r25
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
sub %r25,%r19,%r25
cmp %r25,3; be L1199; nop
set 117,%r28
cmp %r29,%r0; be L1201; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1201:
L1199:
set 5,%r25
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
st %r25,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fsubs %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,3; be L1203; nop
set 118,%r28
cmp %r29,%r0; be L1205; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1205:
L1203:
set 5,%r25
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
st %r25,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fsubd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,3; be L1207; nop
set 119,%r28
cmp %r29,%r0; be L1209; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1209:
L1207:
set 5,%r23
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
sub %r23,%r19,%r23
cmp %r23,3; be L1211; nop
set 120,%r28
cmp %r29,%r0; be L1213; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1213:
L1211:
set 5,%r23
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
sub %r23,%r19,%r23
cmp %r23,3; be L1215; nop
set 121,%r28
cmp %r29,%r0; be L1217; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1217:
L1215:
set 5,%r23
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
sub %r23,%r19,%r23
cmp %r23,3; be L1219; nop
set 122,%r28
cmp %r29,%r0; be L1221; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1221:
L1219:
set 5,%r23
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
sub %r23,%r19,%r23
cmp %r23,3; be L1223; nop
set 123,%r28
cmp %r29,%r0; be L1225; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1225:
L1223:
set 5,%r23
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
sub %r23,%r19,%r23
cmp %r23,3; be L1227; nop
set 124,%r28
cmp %r29,%r0; be L1229; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1229:
L1227:
set 5,%r23
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fsubs %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,3; be L1231; nop
set 125,%r28
cmp %r29,%r0; be L1233; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1233:
L1231:
set 5,%r23
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fsubd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,3; be L1235; nop
set 126,%r28
cmp %r29,%r0; be L1237; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1237:
L1235:
set 5,%r22
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
sub %r22,%r19,%r22
cmp %r22,3; be L1239; nop
set 127,%r28
cmp %r29,%r0; be L1241; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1241:
L1239:
set 5,%r22
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
sub %r22,%r19,%r22
cmp %r22,3; be L1243; nop
set 128,%r28
cmp %r29,%r0; be L1245; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1245:
L1243:
set 5,%r22
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
sub %r22,%r19,%r22
cmp %r22,3; be L1247; nop
set 129,%r28
cmp %r29,%r0; be L1249; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1249:
L1247:
set 5,%r22
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
sub %r22,%r19,%r22
cmp %r22,3; be L1251; nop
set 130,%r28
cmp %r29,%r0; be L1253; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1253:
L1251:
set 5,%r22
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
sub %r22,%r19,%r22
cmp %r22,3; be L1255; nop
set 131,%r28
cmp %r29,%r0; be L1257; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1257:
L1255:
set 5,%r22
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
mov %r22,%r19
cmp %r19,%r0; bl L1263; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-144]; st %f31,[%fp+-144+4]
ba L1264; nop
L1263:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-144]; st %f31,[%fp+-144+4]
L1264:
ld [%fp+-144],%f30; ld [%fp+-144+4],%f31
fdtos %f30,%f31
ld [%fp+-28],%f30
fsubs %f31,%f30,%f31
fstod %f31,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L1261; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-132]
ba L1262; nop
L1261:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-132]
L1262:
ld [%fp+-132],%r22
cmp %r22,3; be L1265; nop
set 132,%r28
cmp %r29,%r0; be L1267; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1267:
L1265:
set 5,%r22
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
mov %r22,%r19
cmp %r19,%r0; bl L1273; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-160]; st %f31,[%fp+-160+4]
ba L1274; nop
L1273:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-160]; st %f31,[%fp+-160+4]
L1274:
ld [%fp+-160],%f30; ld [%fp+-160+4],%f31
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fsubd %f30,%f28,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L1271; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-148]
ba L1272; nop
L1271:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-148]
L1272:
ld [%fp+-148],%r22
cmp %r22,3; be L1275; nop
set 133,%r28
cmp %r29,%r0; be L1277; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1277:
L1275:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r21
ld [%fp+-16],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fsubs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1281,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1279; nop
set 134,%r28
cmp %r29,%r0; be L1282; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1282:
L1279:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r20
ld [%fp+-16],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fsubs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1281,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1284; nop
set 135,%r28
cmp %r29,%r0; be L1286; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1286:
L1284:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-16],%f31
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fsubs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1281,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1288; nop
set 136,%r28
cmp %r29,%r0; be L1290; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1290:
L1288:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-16],%f31
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fsubs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1281,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1292; nop
set 137,%r28
cmp %r29,%r0; be L1294; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1294:
L1292:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L1297; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-168]; st %f31,[%fp+-168+4]
ba L1298; nop
L1297:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-168]; st %f31,[%fp+-168+4]
L1298:
ld [%fp+-16],%f31
ld [%fp+-168],%f28; ld [%fp+-168+4],%f29
fdtos %f28,%f30
fsubs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1281,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1299; nop
set 138,%r28
cmp %r29,%r0; be L1301; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1301:
L1299:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-16],%f31
ld [%fp+-28],%f30
fsubs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1281,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1303; nop
set 139,%r28
cmp %r29,%r0; be L1305; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1305:
L1303:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-16],%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fsubd %f30,%f28,%f30
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1281,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1307; nop
set 140,%r28
cmp %r29,%r0; be L1309; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1309:
L1307:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r21
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fsubd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1313,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1311; nop
set 141,%r28
cmp %r29,%r0; be L1314; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1314:
L1311:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r20
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fsubd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1313,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1316; nop
set 142,%r28
cmp %r29,%r0; be L1318; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1318:
L1316:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fsubd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1313,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1320; nop
set 143,%r28
cmp %r29,%r0; be L1322; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1322:
L1320:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fsubd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1313,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1324; nop
set 144,%r28
cmp %r29,%r0; be L1326; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1326:
L1324:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L1329; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-176]; st %f31,[%fp+-176+4]
ba L1330; nop
L1329:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-176]; st %f31,[%fp+-176+4]
L1330:
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-176],%f28; ld [%fp+-176+4],%f29
fsubd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1313,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1331; nop
set 145,%r28
cmp %r29,%r0; be L1333; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1333:
L1331:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-28],%f29
fstod %f29,%f28
fsubd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1313,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1335; nop
set 146,%r28
cmp %r29,%r0; be L1337; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1337:
L1335:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fsubd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1313,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1339; nop
set 147,%r28
cmp %r29,%r0; be L1341; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1341:
L1339:
set 5,%r27
set 2,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,10; be L1343; nop
set 148,%r28
cmp %r29,%r0; be L1345; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1345:
L1343:
set 5,%r27
set 2,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,10; be L1347; nop
set 149,%r28
cmp %r29,%r0; be L1349; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1349:
L1347:
set 5,%r27
set 2,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,10; be L1351; nop
set 150,%r28
cmp %r29,%r0; be L1353; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1353:
L1351:
set 5,%r27
set 2,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,10; be L1355; nop
set 151,%r28
cmp %r29,%r0; be L1357; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1357:
L1355:
set 5,%r27
set 2,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,10; be L1359; nop
set 152,%r28
cmp %r29,%r0; be L1361; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1361:
L1359:
set 5,%r27
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
sll %r27,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,10; be L1363; nop
set 153,%r28
cmp %r29,%r0; be L1365; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1365:
L1363:
set 5,%r27
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
sll %r27,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fmuld %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,10; be L1367; nop
set 154,%r28
cmp %r29,%r0; be L1369; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1369:
L1367:
set 5,%r26
set 2,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,10; be L1371; nop
set 155,%r28
cmp %r29,%r0; be L1373; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1373:
L1371:
set 5,%r26
set 2,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,10; be L1375; nop
set 156,%r28
cmp %r29,%r0; be L1377; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1377:
L1375:
set 5,%r26
set 2,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,10; be L1379; nop
set 157,%r28
cmp %r29,%r0; be L1381; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1381:
L1379:
set 5,%r26
set 2,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
mov %r18,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,10; be L1383; nop
set 158,%r28
cmp %r29,%r0; be L1385; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1385:
L1383:
set 5,%r26
set 2,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,10; be L1387; nop
set 159,%r28
cmp %r29,%r0; be L1389; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1389:
L1387:
set 5,%r26
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
sll %r26,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,10; be L1391; nop
set 160,%r28
cmp %r29,%r0; be L1393; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1393:
L1391:
set 5,%r26
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
sll %r26,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fmuld %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,10; be L1395; nop
set 161,%r28
cmp %r29,%r0; be L1397; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1397:
L1395:
set 5,%r25
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .mul,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,10; be L1399; nop
set 162,%r28
cmp %r29,%r0; be L1401; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1401:
L1399:
set 5,%r25
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .mul,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,10; be L1403; nop
set 163,%r28
cmp %r29,%r0; be L1405; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1405:
L1403:
set 5,%r25
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .mul,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,10; be L1407; nop
set 164,%r28
cmp %r29,%r0; be L1409; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1409:
L1407:
set 5,%r25
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .mul,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,10; be L1411; nop
set 165,%r28
cmp %r29,%r0; be L1413; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1413:
L1411:
set 5,%r25
set 2,%r19
st %r19,[%fp+-12]
mov %r25,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,10; be L1415; nop
set 166,%r28
cmp %r29,%r0; be L1417; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1417:
L1415:
set 5,%r25
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
st %r25,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,10; be L1419; nop
set 167,%r28
cmp %r29,%r0; be L1421; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1421:
L1419:
set 5,%r25
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
st %r25,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fmuld %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,10; be L1423; nop
set 168,%r28
cmp %r29,%r0; be L1425; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1425:
L1423:
set 5,%r23
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .mul,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,10; be L1427; nop
set 169,%r28
cmp %r29,%r0; be L1429; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1429:
L1427:
set 5,%r23
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .mul,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,10; be L1431; nop
set 170,%r28
cmp %r29,%r0; be L1433; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1433:
L1431:
set 5,%r23
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .mul,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,10; be L1435; nop
set 171,%r28
cmp %r29,%r0; be L1437; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1437:
L1435:
set 5,%r23
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .mul,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,10; be L1439; nop
set 172,%r28
cmp %r29,%r0; be L1441; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1441:
L1439:
set 5,%r23
set 2,%r19
st %r19,[%fp+-12]
mov %r23,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .umul,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,10; be L1443; nop
set 173,%r28
cmp %r29,%r0; be L1445; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1445:
L1443:
set 5,%r23
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,10; be L1447; nop
set 174,%r28
cmp %r29,%r0; be L1449; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1449:
L1447:
set 5,%r23
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fmuld %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,10; be L1451; nop
set 175,%r28
cmp %r29,%r0; be L1453; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1453:
L1451:
set 5,%r22
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .umul,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,10; be L1455; nop
set 176,%r28
cmp %r29,%r0; be L1457; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1457:
L1455:
set 5,%r22
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .umul,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,10; be L1459; nop
set 177,%r28
cmp %r29,%r0; be L1461; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1461:
L1459:
set 5,%r22
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .umul,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,10; be L1463; nop
set 178,%r28
cmp %r29,%r0; be L1465; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1465:
L1463:
set 5,%r22
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .umul,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,10; be L1467; nop
set 179,%r28
cmp %r29,%r0; be L1469; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1469:
L1467:
set 5,%r22
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
mov %r19,%o1; call .umul,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,10; be L1471; nop
set 180,%r28
cmp %r29,%r0; be L1473; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1473:
L1471:
set 5,%r22
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
mov %r22,%r19
cmp %r19,%r0; bl L1479; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-192]; st %f31,[%fp+-192+4]
ba L1480; nop
L1479:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-192]; st %f31,[%fp+-192+4]
L1480:
ld [%fp+-192],%f30; ld [%fp+-192+4],%f31
fdtos %f30,%f31
ld [%fp+-28],%f30
fmuls %f31,%f30,%f31
fstod %f31,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L1477; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-180]
ba L1478; nop
L1477:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-180]
L1478:
ld [%fp+-180],%r22
cmp %r22,10; be L1481; nop
set 181,%r28
cmp %r29,%r0; be L1483; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1483:
L1481:
set 5,%r22
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
mov %r22,%r19
cmp %r19,%r0; bl L1489; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-208]; st %f31,[%fp+-208+4]
ba L1490; nop
L1489:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-208]; st %f31,[%fp+-208+4]
L1490:
ld [%fp+-208],%f30; ld [%fp+-208+4],%f31
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fmuld %f30,%f28,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L1487; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-196]
ba L1488; nop
L1487:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-196]
L1488:
ld [%fp+-196],%r22
cmp %r22,10; be L1491; nop
set 182,%r28
cmp %r29,%r0; be L1493; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1493:
L1491:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r21
ld [%fp+-16],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fmuls %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1497,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1495; nop
set 183,%r28
cmp %r29,%r0; be L1498; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1498:
L1495:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r20
ld [%fp+-16],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fmuls %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1497,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1500; nop
set 184,%r28
cmp %r29,%r0; be L1502; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1502:
L1500:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-16],%f31
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fmuls %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1497,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1504; nop
set 185,%r28
cmp %r29,%r0; be L1506; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1506:
L1504:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-16],%f31
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fmuls %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1497,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1508; nop
set 186,%r28
cmp %r29,%r0; be L1510; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1510:
L1508:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L1513; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-216]; st %f31,[%fp+-216+4]
ba L1514; nop
L1513:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-216]; st %f31,[%fp+-216+4]
L1514:
ld [%fp+-16],%f31
ld [%fp+-216],%f28; ld [%fp+-216+4],%f29
fdtos %f28,%f30
fmuls %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1497,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1515; nop
set 187,%r28
cmp %r29,%r0; be L1517; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1517:
L1515:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-16],%f31
ld [%fp+-28],%f30
fmuls %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1497,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1519; nop
set 188,%r28
cmp %r29,%r0; be L1521; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1521:
L1519:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-16],%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fmuld %f30,%f28,%f30
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
set L1497,%r19
ld [%r19],%f30
fcmpes %f31,%f30; nop; fbue L1523; nop
set 189,%r28
cmp %r29,%r0; be L1525; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1525:
L1523:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r21
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fmuld %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1529,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1527; nop
set 190,%r28
cmp %r29,%r0; be L1530; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1530:
L1527:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r20
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fmuld %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1529,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1532; nop
set 191,%r28
cmp %r29,%r0; be L1534; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1534:
L1532:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fmuld %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1529,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1536; nop
set 192,%r28
cmp %r29,%r0; be L1538; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1538:
L1536:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fmuld %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1529,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1540; nop
set 193,%r28
cmp %r29,%r0; be L1542; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1542:
L1540:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L1545; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-224]; st %f31,[%fp+-224+4]
ba L1546; nop
L1545:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-224]; st %f31,[%fp+-224+4]
L1546:
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-224],%f28; ld [%fp+-224+4],%f29
fmuld %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1529,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1547; nop
set 194,%r28
cmp %r29,%r0; be L1549; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1549:
L1547:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-28],%f29
fstod %f29,%f28
fmuld %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1529,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1551; nop
set 195,%r28
cmp %r29,%r0; be L1553; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1553:
L1551:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fmuld %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1529,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1555; nop
set 196,%r28
cmp %r29,%r0; be L1557; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1557:
L1555:
set 5,%r27
set 2,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L1559; nop
set 197,%r28
cmp %r29,%r0; be L1561; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1561:
L1559:
set 5,%r27
set 2,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L1563; nop
set 198,%r28
cmp %r29,%r0; be L1565; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1565:
L1563:
set 5,%r27
set 2,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L1567; nop
set 199,%r28
cmp %r29,%r0; be L1569; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1569:
L1567:
set 5,%r27
set 2,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L1571; nop
set 200,%r28
cmp %r29,%r0; be L1573; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1573:
L1571:
set 5,%r27
set 2,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .udiv,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L1575; nop
set 201,%r28
cmp %r29,%r0; be L1577; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1577:
L1575:
set 5,%r27
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
sll %r27,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fdivs %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L1579; nop
set 202,%r28
cmp %r29,%r0; be L1581; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1581:
L1579:
set 5,%r27
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
sll %r27,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fdivd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,2; be L1583; nop
set 203,%r28
cmp %r29,%r0; be L1585; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1585:
L1583:
set 5,%r26
set 2,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L1587; nop
set 204,%r28
cmp %r29,%r0; be L1589; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1589:
L1587:
set 5,%r26
set 2,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L1591; nop
set 205,%r28
cmp %r29,%r0; be L1593; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1593:
L1591:
set 5,%r26
set 2,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L1595; nop
set 206,%r28
cmp %r29,%r0; be L1597; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1597:
L1595:
set 5,%r26
set 2,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L1599; nop
set 207,%r28
cmp %r29,%r0; be L1601; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1601:
L1599:
set 5,%r26
set 2,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .udiv,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L1603; nop
set 208,%r28
cmp %r29,%r0; be L1605; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1605:
L1603:
set 5,%r26
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
sll %r26,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fdivs %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L1607; nop
set 209,%r28
cmp %r29,%r0; be L1609; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1609:
L1607:
set 5,%r26
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
sll %r26,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fdivd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,2; be L1611; nop
set 210,%r28
cmp %r29,%r0; be L1613; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1613:
L1611:
set 5,%r25
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .div,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,2; be L1615; nop
set 211,%r28
cmp %r29,%r0; be L1617; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1617:
L1615:
set 5,%r25
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .div,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,2; be L1619; nop
set 212,%r28
cmp %r29,%r0; be L1621; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1621:
L1619:
set 5,%r25
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .div,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,2; be L1623; nop
set 213,%r28
cmp %r29,%r0; be L1625; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1625:
L1623:
set 5,%r25
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .div,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,2; be L1627; nop
set 214,%r28
cmp %r29,%r0; be L1629; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1629:
L1627:
set 5,%r25
set 2,%r19
st %r19,[%fp+-12]
mov %r25,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .udiv,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,2; be L1631; nop
set 215,%r28
cmp %r29,%r0; be L1633; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1633:
L1631:
set 5,%r25
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
st %r25,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fdivs %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,2; be L1635; nop
set 216,%r28
cmp %r29,%r0; be L1637; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1637:
L1635:
set 5,%r25
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
st %r25,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fdivd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r25
cmp %r25,2; be L1639; nop
set 217,%r28
cmp %r29,%r0; be L1641; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1641:
L1639:
set 5,%r23
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .div,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,2; be L1643; nop
set 218,%r28
cmp %r29,%r0; be L1645; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1645:
L1643:
set 5,%r23
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .div,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,2; be L1647; nop
set 219,%r28
cmp %r29,%r0; be L1649; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1649:
L1647:
set 5,%r23
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .div,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,2; be L1651; nop
set 220,%r28
cmp %r29,%r0; be L1653; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1653:
L1651:
set 5,%r23
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .div,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,2; be L1655; nop
set 221,%r28
cmp %r29,%r0; be L1657; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1657:
L1655:
set 5,%r23
set 2,%r19
st %r19,[%fp+-12]
mov %r23,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .udiv,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,2; be L1659; nop
set 222,%r28
cmp %r29,%r0; be L1661; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1661:
L1659:
set 5,%r23
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
ld [%fp+-28],%f30
fdivs %f31,%f30,%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,2; be L1663; nop
set 223,%r28
cmp %r29,%r0; be L1665; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1665:
L1663:
set 5,%r23
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fdivd %f30,%f28,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
mov %r19,%r23
cmp %r23,2; be L1667; nop
set 224,%r28
cmp %r29,%r0; be L1669; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1669:
L1667:
set 5,%r22
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .udiv,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,2; be L1671; nop
set 225,%r28
cmp %r29,%r0; be L1673; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1673:
L1671:
set 5,%r22
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .udiv,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,2; be L1675; nop
set 226,%r28
cmp %r29,%r0; be L1677; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1677:
L1675:
set 5,%r22
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .udiv,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,2; be L1679; nop
set 227,%r28
cmp %r29,%r0; be L1681; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1681:
L1679:
set 5,%r22
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .udiv,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,2; be L1683; nop
set 228,%r28
cmp %r29,%r0; be L1685; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1685:
L1683:
set 5,%r22
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
mov %r19,%o1; call .udiv,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,2; be L1687; nop
set 229,%r28
cmp %r29,%r0; be L1689; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1689:
L1687:
set 5,%r22
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
mov %r22,%r19
cmp %r19,%r0; bl L1695; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-240]; st %f31,[%fp+-240+4]
ba L1696; nop
L1695:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-240]; st %f31,[%fp+-240+4]
L1696:
ld [%fp+-240],%f30; ld [%fp+-240+4],%f31
fdtos %f30,%f31
ld [%fp+-28],%f30
fdivs %f31,%f30,%f31
fstod %f31,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L1693; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-228]
ba L1694; nop
L1693:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-228]
L1694:
ld [%fp+-228],%r22
cmp %r22,2; be L1697; nop
set 230,%r28
cmp %r29,%r0; be L1699; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1699:
L1697:
set 5,%r22
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
mov %r22,%r19
cmp %r19,%r0; bl L1705; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-256]; st %f31,[%fp+-256+4]
ba L1706; nop
L1705:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-256]; st %f31,[%fp+-256+4]
L1706:
ld [%fp+-256],%f30; ld [%fp+-256+4],%f31
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fdivd %f30,%f28,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L1703; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-244]
ba L1704; nop
L1703:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-244]
L1704:
ld [%fp+-244],%r22
cmp %r22,2; be L1707; nop
set 231,%r28
cmp %r29,%r0; be L1709; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1709:
L1707:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r21
ld [%fp+-16],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fdivs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
fstod %f31,%f30
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1711; nop
set 232,%r28
cmp %r29,%r0; be L1714; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1714:
L1711:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r20
ld [%fp+-16],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fdivs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
fstod %f31,%f30
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1716; nop
set 233,%r28
cmp %r29,%r0; be L1718; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1718:
L1716:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-16],%f31
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fdivs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
fstod %f31,%f30
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1720; nop
set 234,%r28
cmp %r29,%r0; be L1722; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1722:
L1720:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-16],%f31
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fdivs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
fstod %f31,%f30
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1724; nop
set 235,%r28
cmp %r29,%r0; be L1726; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1726:
L1724:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L1729; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-264]; st %f31,[%fp+-264+4]
ba L1730; nop
L1729:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-264]; st %f31,[%fp+-264+4]
L1730:
ld [%fp+-16],%f31
ld [%fp+-264],%f28; ld [%fp+-264+4],%f29
fdtos %f28,%f30
fdivs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
fstod %f31,%f30
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1731; nop
set 236,%r28
cmp %r29,%r0; be L1733; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1733:
L1731:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-16],%f31
ld [%fp+-28],%f30
fdivs %f31,%f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
fstod %f31,%f30
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1735; nop
set 237,%r28
cmp %r29,%r0; be L1737; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1737:
L1735:
set L847,%r19
ld [%r19],%f31
st %f31,[%fp+-16]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-16],%f31
fstod %f31,%f30
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fdivd %f30,%f28,%f30
fdtos %f30,%f31
st %f31,[%fp+-16]
ld [%fp+-16],%f31
fstod %f31,%f30
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1739; nop
set 238,%r28
cmp %r29,%r0; be L1741; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1741:
L1739:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r21
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
sll %r21,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdivd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1743; nop
set 239,%r28
cmp %r29,%r0; be L1745; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1745:
L1743:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r20
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
sll %r20,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdivd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1747; nop
set 240,%r28
cmp %r29,%r0; be L1749; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1749:
L1747:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-4],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdivd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1751; nop
set 241,%r28
cmp %r29,%r0; be L1753; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1753:
L1751:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-8],%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdivd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1755; nop
set 242,%r28
cmp %r29,%r0; be L1757; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1757:
L1755:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
cmp %r19,%r0; bl L1760; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-272]; st %f31,[%fp+-272+4]
ba L1761; nop
L1760:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-272]; st %f31,[%fp+-272+4]
L1761:
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-272],%f28; ld [%fp+-272+4],%f29
fdivd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1762; nop
set 243,%r28
cmp %r29,%r0; be L1764; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1764:
L1762:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L530,%r19
ld [%r19],%f31
st %f31,[%fp+-28]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-28],%f29
fstod %f29,%f28
fdivd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1766; nop
set 244,%r28
cmp %r29,%r0; be L1768; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1768:
L1766:
set L879,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
set L416,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
ld [%fp+-40],%f28; ld [%fp+-40+4],%f29
fdivd %f30,%f28,%f30
st %f30,[%fp+-24]; st %f31,[%fp+-24+4]
ld [%fp+-24],%f30; ld [%fp+-24+4],%f31
set L1713,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbue L1770; nop
set 245,%r28
cmp %r29,%r0; be L1772; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1772:
L1770:
set 5,%r27
set 2,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .rem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1774; nop
set 246,%r28
cmp %r29,%r0; be L1776; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1776:
L1774:
set 5,%r27
set 2,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
mov %r18,%o1; call .rem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1778; nop
set 247,%r28
cmp %r29,%r0; be L1780; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1780:
L1778:
set 5,%r27
set 2,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
mov %r18,%o1; call .rem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1782; nop
set 248,%r28
cmp %r29,%r0; be L1784; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1784:
L1782:
set 5,%r27
set 2,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
mov %r18,%o1; call .rem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1786; nop
set 249,%r28
cmp %r29,%r0; be L1788; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1788:
L1786:
set 5,%r27
set 2,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .urem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1790; nop
set 250,%r28
cmp %r29,%r0; be L1792; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1792:
L1790:
set 5,%r26
set 2,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
mov %r18,%o1; call .rem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1794; nop
set 251,%r28
cmp %r29,%r0; be L1796; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1796:
L1794:
set 5,%r26
set 2,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
mov %r18,%o1; call .rem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1798; nop
set 252,%r28
cmp %r29,%r0; be L1800; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1800:
L1798:
set 5,%r26
set 2,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
mov %r18,%o1; call .rem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1802; nop
set 253,%r28
cmp %r29,%r0; be L1804; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1804:
L1802:
set 5,%r26
set 2,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
mov %r18,%o1; call .rem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1806; nop
set 254,%r28
cmp %r29,%r0; be L1808; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1808:
L1806:
set 5,%r26
set 2,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .urem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1810; nop
set 255,%r28
cmp %r29,%r0; be L1812; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1812:
L1810:
set 5,%r25
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .rem,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,1; be L1814; nop
set 256,%r28
cmp %r29,%r0; be L1816; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1816:
L1814:
set 5,%r25
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .rem,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,1; be L1818; nop
set 257,%r28
cmp %r29,%r0; be L1820; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1820:
L1818:
set 5,%r25
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .rem,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,1; be L1822; nop
set 258,%r28
cmp %r29,%r0; be L1824; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1824:
L1822:
set 5,%r25
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .rem,2; mov %r25,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,1; be L1826; nop
set 259,%r28
cmp %r29,%r0; be L1828; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1828:
L1826:
set 5,%r25
set 2,%r19
st %r19,[%fp+-12]
mov %r25,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .urem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r25
cmp %r25,1; be L1830; nop
set 260,%r28
cmp %r29,%r0; be L1832; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1832:
L1830:
set 5,%r23
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .rem,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,1; be L1834; nop
set 261,%r28
cmp %r29,%r0; be L1836; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1836:
L1834:
set 5,%r23
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .rem,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,1; be L1838; nop
set 262,%r28
cmp %r29,%r0; be L1840; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1840:
L1838:
set 5,%r23
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .rem,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,1; be L1842; nop
set 263,%r28
cmp %r29,%r0; be L1844; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1844:
L1842:
set 5,%r23
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .rem,2; mov %r23,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,1; be L1846; nop
set 264,%r28
cmp %r29,%r0; be L1848; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1848:
L1846:
set 5,%r23
set 2,%r19
st %r19,[%fp+-12]
mov %r23,%r19
ld [%fp+-12],%r18
mov %r18,%o1; call .urem,2; mov %r19,%o0; mov %o0,%r19
mov %r19,%r23
cmp %r23,1; be L1850; nop
set 265,%r28
cmp %r29,%r0; be L1852; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1852:
L1850:
set 5,%r22
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
mov %r19,%o1; call .urem,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,1; be L1854; nop
set 266,%r28
cmp %r29,%r0; be L1856; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1856:
L1854:
set 5,%r22
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
mov %r19,%o1; call .urem,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,1; be L1858; nop
set 267,%r28
cmp %r29,%r0; be L1860; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1860:
L1858:
set 5,%r22
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
mov %r19,%o1; call .urem,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,1; be L1862; nop
set 268,%r28
cmp %r29,%r0; be L1864; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1864:
L1862:
set 5,%r22
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
mov %r19,%o1; call .urem,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,1; be L1866; nop
set 269,%r28
cmp %r29,%r0; be L1868; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1868:
L1866:
set 5,%r22
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
mov %r19,%o1; call .urem,2; mov %r22,%o0; mov %o0,%r19
mov %r19,%r22
cmp %r22,1; be L1870; nop
set 270,%r28
cmp %r29,%r0; be L1872; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1872:
L1870:
set 5,%r27
set 2,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
sra %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1874; nop
set 271,%r28
cmp %r29,%r0; be L1876; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1876:
L1874:
set 5,%r27
set 2,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
sra %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1878; nop
set 272,%r28
cmp %r29,%r0; be L1880; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1880:
L1878:
set 5,%r27
set 2,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
sra %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1882; nop
set 273,%r28
cmp %r29,%r0; be L1884; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1884:
L1882:
set 5,%r27
set 2,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
sra %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1886; nop
set 274,%r28
cmp %r29,%r0; be L1888; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1888:
L1886:
set 5,%r27
set 2,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
sra %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,1; be L1890; nop
set 275,%r28
cmp %r29,%r0; be L1892; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1892:
L1890:
set 5,%r26
set 2,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
sra %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1894; nop
set 276,%r28
cmp %r29,%r0; be L1896; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1896:
L1894:
set 5,%r26
set 2,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
sra %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1898; nop
set 277,%r28
cmp %r29,%r0; be L1900; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1900:
L1898:
set 5,%r26
set 2,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
sra %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1902; nop
set 278,%r28
cmp %r29,%r0; be L1904; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1904:
L1902:
set 5,%r26
set 2,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
sra %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1906; nop
set 279,%r28
cmp %r29,%r0; be L1908; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1908:
L1906:
set 5,%r26
set 2,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
sra %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,1; be L1910; nop
set 280,%r28
cmp %r29,%r0; be L1912; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1912:
L1910:
set 5,%r25
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
sra %r25,%r19,%r25
cmp %r25,1; be L1914; nop
set 281,%r28
cmp %r29,%r0; be L1916; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1916:
L1914:
set 5,%r25
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
sra %r25,%r19,%r25
cmp %r25,1; be L1918; nop
set 282,%r28
cmp %r29,%r0; be L1920; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1920:
L1918:
set 5,%r25
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
sra %r25,%r19,%r25
cmp %r25,1; be L1922; nop
set 283,%r28
cmp %r29,%r0; be L1924; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1924:
L1922:
set 5,%r25
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
sra %r25,%r19,%r25
cmp %r25,1; be L1926; nop
set 284,%r28
cmp %r29,%r0; be L1928; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1928:
L1926:
set 5,%r25
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
sra %r25,%r19,%r25
cmp %r25,1; be L1930; nop
set 285,%r28
cmp %r29,%r0; be L1932; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1932:
L1930:
set 5,%r23
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
sra %r23,%r19,%r23
cmp %r23,1; be L1934; nop
set 286,%r28
cmp %r29,%r0; be L1936; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1936:
L1934:
set 5,%r23
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
sra %r23,%r19,%r23
cmp %r23,1; be L1938; nop
set 287,%r28
cmp %r29,%r0; be L1940; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1940:
L1938:
set 5,%r23
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
sra %r23,%r19,%r23
cmp %r23,1; be L1942; nop
set 288,%r28
cmp %r29,%r0; be L1944; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1944:
L1942:
set 5,%r23
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
sra %r23,%r19,%r23
cmp %r23,1; be L1946; nop
set 289,%r28
cmp %r29,%r0; be L1948; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1948:
L1946:
set 5,%r23
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
sra %r23,%r19,%r23
cmp %r23,1; be L1950; nop
set 290,%r28
cmp %r29,%r0; be L1952; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1952:
L1950:
set 5,%r22
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
srl %r22,%r19,%r22
cmp %r22,1; be L1954; nop
set 291,%r28
cmp %r29,%r0; be L1956; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1956:
L1954:
set 5,%r22
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
srl %r22,%r19,%r22
cmp %r22,1; be L1958; nop
set 292,%r28
cmp %r29,%r0; be L1960; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1960:
L1958:
set 5,%r22
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
srl %r22,%r19,%r22
cmp %r22,1; be L1962; nop
set 293,%r28
cmp %r29,%r0; be L1964; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1964:
L1962:
set 5,%r22
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
srl %r22,%r19,%r22
cmp %r22,1; be L1966; nop
set 294,%r28
cmp %r29,%r0; be L1968; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1968:
L1966:
set 5,%r22
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
srl %r22,%r19,%r22
cmp %r22,1; be L1970; nop
set 295,%r28
cmp %r29,%r0; be L1972; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1972:
L1970:
set 5,%r27
set 2,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
sll %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,20; be L1974; nop
set 296,%r28
cmp %r29,%r0; be L1976; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1976:
L1974:
set 5,%r27
set 2,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
sll %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,20; be L1978; nop
set 297,%r28
cmp %r29,%r0; be L1980; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1980:
L1978:
set 5,%r27
set 2,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
sll %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,20; be L1982; nop
set 298,%r28
cmp %r29,%r0; be L1984; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1984:
L1982:
set 5,%r27
set 2,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
sll %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,20; be L1986; nop
set 299,%r28
cmp %r29,%r0; be L1988; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1988:
L1986:
set 5,%r27
set 2,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
sll %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,20; be L1990; nop
set 300,%r28
cmp %r29,%r0; be L1992; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1992:
L1990:
set 5,%r26
set 2,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
sll %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,20; be L1994; nop
set 301,%r28
cmp %r29,%r0; be L1996; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L1996:
L1994:
set 5,%r26
set 2,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
sll %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,20; be L1998; nop
set 302,%r28
cmp %r29,%r0; be L2000; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2000:
L1998:
set 5,%r26
set 2,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
sll %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,20; be L2002; nop
set 303,%r28
cmp %r29,%r0; be L2004; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2004:
L2002:
set 5,%r26
set 2,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
sll %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,20; be L2006; nop
set 304,%r28
cmp %r29,%r0; be L2008; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2008:
L2006:
set 5,%r26
set 2,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
sll %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,20; be L2010; nop
set 305,%r28
cmp %r29,%r0; be L2012; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2012:
L2010:
set 5,%r25
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
sll %r25,%r19,%r25
cmp %r25,20; be L2014; nop
set 306,%r28
cmp %r29,%r0; be L2016; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2016:
L2014:
set 5,%r25
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
sll %r25,%r19,%r25
cmp %r25,20; be L2018; nop
set 307,%r28
cmp %r29,%r0; be L2020; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2020:
L2018:
set 5,%r25
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
sll %r25,%r19,%r25
cmp %r25,20; be L2022; nop
set 308,%r28
cmp %r29,%r0; be L2024; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2024:
L2022:
set 5,%r25
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
sll %r25,%r19,%r25
cmp %r25,20; be L2026; nop
set 309,%r28
cmp %r29,%r0; be L2028; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2028:
L2026:
set 5,%r25
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
sll %r25,%r19,%r25
cmp %r25,20; be L2030; nop
set 310,%r28
cmp %r29,%r0; be L2032; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2032:
L2030:
set 5,%r23
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
sll %r23,%r19,%r23
cmp %r23,20; be L2034; nop
set 311,%r28
cmp %r29,%r0; be L2036; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2036:
L2034:
set 5,%r23
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
sll %r23,%r19,%r23
cmp %r23,20; be L2038; nop
set 312,%r28
cmp %r29,%r0; be L2040; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2040:
L2038:
set 5,%r23
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
sll %r23,%r19,%r23
cmp %r23,20; be L2042; nop
set 313,%r28
cmp %r29,%r0; be L2044; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2044:
L2042:
set 5,%r23
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
sll %r23,%r19,%r23
cmp %r23,20; be L2046; nop
set 314,%r28
cmp %r29,%r0; be L2048; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2048:
L2046:
set 5,%r23
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
sll %r23,%r19,%r23
cmp %r23,20; be L2050; nop
set 315,%r28
cmp %r29,%r0; be L2052; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2052:
L2050:
set 5,%r22
set 2,%r21
sll %r21,24,%g1; sra %g1,24,%r19
sll %r22,%r19,%r22
cmp %r22,20; be L2054; nop
set 316,%r28
cmp %r29,%r0; be L2056; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2056:
L2054:
set 5,%r22
set 2,%r20
sll %r20,16,%g1; sra %g1,16,%r19
sll %r22,%r19,%r22
cmp %r22,20; be L2058; nop
set 317,%r28
cmp %r29,%r0; be L2060; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2060:
L2058:
set 5,%r22
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
sll %r22,%r19,%r22
cmp %r22,20; be L2062; nop
set 318,%r28
cmp %r29,%r0; be L2064; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2064:
L2062:
set 5,%r22
set 2,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
sll %r22,%r19,%r22
cmp %r22,20; be L2066; nop
set 319,%r28
cmp %r29,%r0; be L2068; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2068:
L2066:
set 5,%r22
set 2,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
sll %r22,%r19,%r22
cmp %r22,20; be L2070; nop
set 320,%r28
cmp %r29,%r0; be L2072; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2072:
L2070:
set 12,%r27
set 10,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
and %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,8; be L2074; nop
set 321,%r28
cmp %r29,%r0; be L2076; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2076:
L2074:
set 12,%r27
set 10,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
and %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,8; be L2078; nop
set 322,%r28
cmp %r29,%r0; be L2080; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2080:
L2078:
set 12,%r27
set 10,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
and %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,8; be L2082; nop
set 323,%r28
cmp %r29,%r0; be L2084; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2084:
L2082:
set 12,%r27
set 10,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
and %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,8; be L2086; nop
set 324,%r28
cmp %r29,%r0; be L2088; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2088:
L2086:
set 12,%r27
set 10,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
and %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,8; be L2090; nop
set 325,%r28
cmp %r29,%r0; be L2092; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2092:
L2090:
set 12,%r26
set 10,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
and %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,8; be L2094; nop
set 326,%r28
cmp %r29,%r0; be L2096; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2096:
L2094:
set 12,%r26
set 10,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
and %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,8; be L2098; nop
set 327,%r28
cmp %r29,%r0; be L2100; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2100:
L2098:
set 12,%r26
set 10,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
and %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,8; be L2102; nop
set 328,%r28
cmp %r29,%r0; be L2104; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2104:
L2102:
set 12,%r26
set 10,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
and %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,8; be L2106; nop
set 329,%r28
cmp %r29,%r0; be L2108; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2108:
L2106:
set 12,%r26
set 10,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
and %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,8; be L2110; nop
set 330,%r28
cmp %r29,%r0; be L2112; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2112:
L2110:
set 12,%r25
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
and %r25,%r19,%r25
cmp %r25,8; be L2114; nop
set 331,%r28
cmp %r29,%r0; be L2116; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2116:
L2114:
set 12,%r25
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
and %r25,%r19,%r25
cmp %r25,8; be L2118; nop
set 332,%r28
cmp %r29,%r0; be L2120; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2120:
L2118:
set 12,%r25
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
and %r25,%r19,%r25
cmp %r25,8; be L2122; nop
set 333,%r28
cmp %r29,%r0; be L2124; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2124:
L2122:
set 12,%r25
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
and %r25,%r19,%r25
cmp %r25,8; be L2126; nop
set 334,%r28
cmp %r29,%r0; be L2128; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2128:
L2126:
set 12,%r25
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
and %r25,%r19,%r25
cmp %r25,8; be L2130; nop
set 335,%r28
cmp %r29,%r0; be L2132; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2132:
L2130:
set 12,%r23
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
and %r23,%r19,%r23
cmp %r23,8; be L2134; nop
set 336,%r28
cmp %r29,%r0; be L2136; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2136:
L2134:
set 12,%r23
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
and %r23,%r19,%r23
cmp %r23,8; be L2138; nop
set 337,%r28
cmp %r29,%r0; be L2140; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2140:
L2138:
set 12,%r23
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
and %r23,%r19,%r23
cmp %r23,8; be L2142; nop
set 338,%r28
cmp %r29,%r0; be L2144; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2144:
L2142:
set 12,%r23
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
and %r23,%r19,%r23
cmp %r23,8; be L2146; nop
set 339,%r28
cmp %r29,%r0; be L2148; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2148:
L2146:
set 12,%r23
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
and %r23,%r19,%r23
cmp %r23,8; be L2150; nop
set 340,%r28
cmp %r29,%r0; be L2152; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2152:
L2150:
set 12,%r22
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
and %r22,%r19,%r22
cmp %r22,8; be L2154; nop
set 341,%r28
cmp %r29,%r0; be L2156; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2156:
L2154:
set 12,%r22
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
and %r22,%r19,%r22
cmp %r22,8; be L2158; nop
set 342,%r28
cmp %r29,%r0; be L2160; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2160:
L2158:
set 12,%r22
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
and %r22,%r19,%r22
cmp %r22,8; be L2162; nop
set 343,%r28
cmp %r29,%r0; be L2164; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2164:
L2162:
set 12,%r22
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
and %r22,%r19,%r22
cmp %r22,8; be L2166; nop
set 344,%r28
cmp %r29,%r0; be L2168; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2168:
L2166:
set 12,%r22
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
and %r22,%r19,%r22
cmp %r22,8; be L2170; nop
set 345,%r28
cmp %r29,%r0; be L2172; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2172:
L2170:
set 12,%r27
set 10,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
xor %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,6; be L2174; nop
set 346,%r28
cmp %r29,%r0; be L2176; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2176:
L2174:
set 12,%r27
set 10,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
xor %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,6; be L2178; nop
set 347,%r28
cmp %r29,%r0; be L2180; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2180:
L2178:
set 12,%r27
set 10,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
xor %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,6; be L2182; nop
set 348,%r28
cmp %r29,%r0; be L2184; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2184:
L2182:
set 12,%r27
set 10,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
xor %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,6; be L2186; nop
set 349,%r28
cmp %r29,%r0; be L2188; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2188:
L2186:
set 12,%r27
set 10,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
xor %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,6; be L2190; nop
set 350,%r28
cmp %r29,%r0; be L2192; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2192:
L2190:
set 12,%r26
set 10,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
xor %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,6; be L2194; nop
set 351,%r28
cmp %r29,%r0; be L2196; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2196:
L2194:
set 12,%r26
set 10,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
xor %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,6; be L2198; nop
set 352,%r28
cmp %r29,%r0; be L2200; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2200:
L2198:
set 12,%r26
set 10,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
xor %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,6; be L2202; nop
set 353,%r28
cmp %r29,%r0; be L2204; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2204:
L2202:
set 12,%r26
set 10,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
xor %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,6; be L2206; nop
set 354,%r28
cmp %r29,%r0; be L2208; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2208:
L2206:
set 12,%r26
set 10,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
xor %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,6; be L2210; nop
set 355,%r28
cmp %r29,%r0; be L2212; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2212:
L2210:
set 12,%r25
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
xor %r25,%r19,%r25
cmp %r25,6; be L2214; nop
set 356,%r28
cmp %r29,%r0; be L2216; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2216:
L2214:
set 12,%r25
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
xor %r25,%r19,%r25
cmp %r25,6; be L2218; nop
set 357,%r28
cmp %r29,%r0; be L2220; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2220:
L2218:
set 12,%r25
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
xor %r25,%r19,%r25
cmp %r25,6; be L2222; nop
set 358,%r28
cmp %r29,%r0; be L2224; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2224:
L2222:
set 12,%r25
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
xor %r25,%r19,%r25
cmp %r25,6; be L2226; nop
set 359,%r28
cmp %r29,%r0; be L2228; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2228:
L2226:
set 12,%r25
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
xor %r25,%r19,%r25
cmp %r25,6; be L2230; nop
set 360,%r28
cmp %r29,%r0; be L2232; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2232:
L2230:
set 12,%r23
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
xor %r23,%r19,%r23
cmp %r23,6; be L2234; nop
set 361,%r28
cmp %r29,%r0; be L2236; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2236:
L2234:
set 12,%r23
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
xor %r23,%r19,%r23
cmp %r23,6; be L2238; nop
set 362,%r28
cmp %r29,%r0; be L2240; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2240:
L2238:
set 12,%r23
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
xor %r23,%r19,%r23
cmp %r23,6; be L2242; nop
set 363,%r28
cmp %r29,%r0; be L2244; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2244:
L2242:
set 12,%r23
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
xor %r23,%r19,%r23
cmp %r23,6; be L2246; nop
set 364,%r28
cmp %r29,%r0; be L2248; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2248:
L2246:
set 12,%r23
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
xor %r23,%r19,%r23
cmp %r23,6; be L2250; nop
set 365,%r28
cmp %r29,%r0; be L2252; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2252:
L2250:
set 12,%r22
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
xor %r22,%r19,%r22
cmp %r22,6; be L2254; nop
set 366,%r28
cmp %r29,%r0; be L2256; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2256:
L2254:
set 12,%r22
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
xor %r22,%r19,%r22
cmp %r22,6; be L2258; nop
set 367,%r28
cmp %r29,%r0; be L2260; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2260:
L2258:
set 12,%r22
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
xor %r22,%r19,%r22
cmp %r22,6; be L2262; nop
set 368,%r28
cmp %r29,%r0; be L2264; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2264:
L2262:
set 12,%r22
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
xor %r22,%r19,%r22
cmp %r22,6; be L2266; nop
set 369,%r28
cmp %r29,%r0; be L2268; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2268:
L2266:
set 12,%r22
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
xor %r22,%r19,%r22
cmp %r22,6; be L2270; nop
set 370,%r28
cmp %r29,%r0; be L2272; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2272:
L2270:
set 12,%r27
set 10,%r21
sll %r27,24,%g1; sra %g1,24,%r19
sll %r21,24,%g1; sra %g1,24,%r18
or %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,14; be L2274; nop
set 371,%r28
cmp %r29,%r0; be L2276; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2276:
L2274:
set 12,%r27
set 10,%r20
sll %r27,24,%g1; sra %g1,24,%r19
sll %r20,16,%g1; sra %g1,16,%r18
or %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,14; be L2278; nop
set 372,%r28
cmp %r29,%r0; be L2280; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2280:
L2278:
set 12,%r27
set 10,%r19
st %r19,[%fp+-4]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-4],%r18
or %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,14; be L2282; nop
set 373,%r28
cmp %r29,%r0; be L2284; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2284:
L2282:
set 12,%r27
set 10,%r19
st %r19,[%fp+-8]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-8],%r18
or %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,14; be L2286; nop
set 374,%r28
cmp %r29,%r0; be L2288; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2288:
L2286:
set 12,%r27
set 10,%r19
st %r19,[%fp+-12]
sll %r27,24,%g1; sra %g1,24,%r19
ld [%fp+-12],%r18
or %r19,%r18,%r27
sll %r27,24,%g1; sra %g1,24,%r19
cmp %r19,14; be L2290; nop
set 375,%r28
cmp %r29,%r0; be L2292; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2292:
L2290:
set 12,%r26
set 10,%r21
sll %r26,16,%g1; sra %g1,16,%r19
sll %r21,24,%g1; sra %g1,24,%r18
or %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,14; be L2294; nop
set 376,%r28
cmp %r29,%r0; be L2296; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2296:
L2294:
set 12,%r26
set 10,%r20
sll %r26,16,%g1; sra %g1,16,%r19
sll %r20,16,%g1; sra %g1,16,%r18
or %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,14; be L2298; nop
set 377,%r28
cmp %r29,%r0; be L2300; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2300:
L2298:
set 12,%r26
set 10,%r19
st %r19,[%fp+-4]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-4],%r18
or %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,14; be L2302; nop
set 378,%r28
cmp %r29,%r0; be L2304; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2304:
L2302:
set 12,%r26
set 10,%r19
st %r19,[%fp+-8]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-8],%r18
or %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,14; be L2306; nop
set 379,%r28
cmp %r29,%r0; be L2308; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2308:
L2306:
set 12,%r26
set 10,%r19
st %r19,[%fp+-12]
sll %r26,16,%g1; sra %g1,16,%r19
ld [%fp+-12],%r18
or %r19,%r18,%r26
sll %r26,16,%g1; sra %g1,16,%r19
cmp %r19,14; be L2310; nop
set 380,%r28
cmp %r29,%r0; be L2312; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2312:
L2310:
set 12,%r25
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
or %r25,%r19,%r25
cmp %r25,14; be L2314; nop
set 381,%r28
cmp %r29,%r0; be L2316; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2316:
L2314:
set 12,%r25
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
or %r25,%r19,%r25
cmp %r25,14; be L2318; nop
set 382,%r28
cmp %r29,%r0; be L2320; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2320:
L2318:
set 12,%r25
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
or %r25,%r19,%r25
cmp %r25,14; be L2322; nop
set 383,%r28
cmp %r29,%r0; be L2324; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2324:
L2322:
set 12,%r25
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
or %r25,%r19,%r25
cmp %r25,14; be L2326; nop
set 384,%r28
cmp %r29,%r0; be L2328; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2328:
L2326:
set 12,%r25
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
or %r25,%r19,%r25
cmp %r25,14; be L2330; nop
set 385,%r28
cmp %r29,%r0; be L2332; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2332:
L2330:
set 12,%r23
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
or %r23,%r19,%r23
cmp %r23,14; be L2334; nop
set 386,%r28
cmp %r29,%r0; be L2336; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2336:
L2334:
set 12,%r23
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
or %r23,%r19,%r23
cmp %r23,14; be L2338; nop
set 387,%r28
cmp %r29,%r0; be L2340; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2340:
L2338:
set 12,%r23
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
or %r23,%r19,%r23
cmp %r23,14; be L2342; nop
set 388,%r28
cmp %r29,%r0; be L2344; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2344:
L2342:
set 12,%r23
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
or %r23,%r19,%r23
cmp %r23,14; be L2346; nop
set 389,%r28
cmp %r29,%r0; be L2348; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2348:
L2346:
set 12,%r23
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
or %r23,%r19,%r23
cmp %r23,14; be L2350; nop
set 390,%r28
cmp %r29,%r0; be L2352; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2352:
L2350:
set 12,%r22
set 10,%r21
sll %r21,24,%g1; sra %g1,24,%r19
or %r22,%r19,%r22
cmp %r22,14; be L2354; nop
set 391,%r28
cmp %r29,%r0; be L2356; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2356:
L2354:
set 12,%r22
set 10,%r20
sll %r20,16,%g1; sra %g1,16,%r19
or %r22,%r19,%r22
cmp %r22,14; be L2358; nop
set 392,%r28
cmp %r29,%r0; be L2360; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2360:
L2358:
set 12,%r22
set 10,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
or %r22,%r19,%r22
cmp %r22,14; be L2362; nop
set 393,%r28
cmp %r29,%r0; be L2364; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2364:
L2362:
set 12,%r22
set 10,%r19
st %r19,[%fp+-8]
ld [%fp+-8],%r19
or %r22,%r19,%r22
cmp %r22,14; be L2366; nop
set 394,%r28
cmp %r29,%r0; be L2368; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2368:
L2366:
set 12,%r22
set 10,%r19
st %r19,[%fp+-12]
ld [%fp+-12],%r19
or %r22,%r19,%r22
cmp %r22,14; be L2370; nop
set 395,%r28
cmp %r29,%r0; be L2372; nop
set _694,%r8
mov %r28,%r9
call _printf; nop
L2372:
L2370:
cmp %r28,%r0; be L2374; nop
set 1,%r19
st %r19,[%fp+-52]
ld [%r24+44],%r19
cmp %r19,%r0; be L2376; nop
set _695,%r8
set 1,%r9
call _printf; nop
L2376:
L2374:
ld [%fp+-52],%r24
L693:
ret; restore
.seg "data"
_2379:.ascii "s715,er%d\012\000"
_2380:.ascii "s715   \000"
.seg "text"
.global _s715
.align 4
.proc 4
_s715:save %sp,-96,%sp
mov %r0,%r22
mov %r0,%r25
set _2380,%r29
add %r24,60,%r28
mov %r0,%r26
L2381:
L2382:
mov %r28,%r21
add %r21,1,%r28
mov %r29,%r20
add %r20,1,%r29
ldsb [%r20],%r20
stb %r20,[%r21]
sll %r20,24,%g1; sra %g1,24,%r21
cmp %r21,%r0; bne L2381; nop
set 1,%r21
mov %r21,%r27
add %r27,1,%r27
add %r27,1,%r27
add %r27,1,%r27
add %r27,1,%r27
add %r27,1,%r21
mov %r21,%r27
cmp %r21,6; be L2384; nop
ld [%r24+44],%r21
cmp %r21,%r0; be L2386; nop
set _2379,%r8
set 1,%r9
call _printf; nop
L2386:
add %r26,1,%r26
L2384:
mov %r25,%r8
set 3,%r23
add %r23,2,%r9
mov %r22,%r10
call _s715f; nop
cmp %r8,5; be L2388; nop
ld [%r24+44],%r21
cmp %r21,%r0; be L2390; nop
set _2379,%r8
set 2,%r9
call _printf; nop
L2390:
add %r26,2,%r26
L2388:
mov %r26,%r24
L2378:
ret; restore
.global _s715f
.align 4
.proc 4
_s715f:
mov %r9,%r8
L2392:
retl; nop
.seg "data"
_2394:.ascii "s72,er%d\012\000"
_2395:.ascii "s72    \000"
.seg "text"
.global _s72
.align 4
.proc 4
_s72:save %sp,-144,%sp
set _2395,%r28
add %r24,60,%r27
mov %r0,%r25
L2396:
L2397:
mov %r27,%r19
add %r19,1,%r27
mov %r28,%r18
add %r18,1,%r28
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L2396; nop
set 2,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
cmp %r19,2; be L2399; nop
add %r25,1,%r25
set _2394,%r8
set 1,%r9
call _printf; nop
L2399:
ld [%fp+-4],%r19
sub %g0,%r19,%r18
add %r19,%r18,%r19
cmp %r19,%r0; be L2401; nop
add %r25,2,%r25
set _2394,%r8
set 2,%r9
call _printf; nop
L2401:
st %r0,[%fp+-4]
mov %r0,%r29
ba L2406; nop
L2403:
ld [%fp+-4],%r19
sll %r19,1,%r19
or %r19,1,%r19
st %r19,[%fp+-4]
L2404:
add %r29,1,%r29
L2406:
ld [%r24+4],%r19
cmp %r29,%r19; bl L2403; nop
ld [%fp+-4],%r19
xnor %r19,%g0,%r19
cmp %r19,%r0; be L2407; nop
add %r25,4,%r25
set _2394,%r8
set 4,%r9
call _printf; nop
L2407:
set 5,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
add %r19,1,%r19
st %r19,[%fp+-4]
cmp %r19,6; bne L2414; nop
ld [%fp+-4],%r19
sub %r19,1,%r19
st %r19,[%fp+-4]
cmp %r19,5; bne L2414; nop
ld [%fp+-4],%r19
add %r19,1,%r18
st %r18,[%fp+-4]
cmp %r19,5; bne L2414; nop
ld [%fp+-4],%r19
sub %r19,1,%r18
st %r18,[%fp+-4]
cmp %r19,6; bne L2414; nop
ld [%fp+-4],%r19
cmp %r19,5; be L2409; nop
L2414:
add %r25,8,%r25
set _2394,%r8
set 8,%r9
call _printf; nop
L2409:
set 26,%r22
set 26,%r20
set L2415,%r19
ld [%r19],%f30; ld [%r19+4],%f31
st %f30,[%fp+-16]; st %f31,[%fp+-16+4]
set 26,%r21
set 26,%r19
st %r19,[%fp+-8]
set 26,%r19
mov %r19,%r23
set L2416,%r19
ld [%r19],%f31
st %f31,[%fp+-20]
mov %r0,%r26
sll %r21,16,%g1; sra %g1,16,%r19
sll %r19,24,%g1; sra %g1,24,%r19
cmp %r19,26; bne L2423; nop
mov %r23,%r19
sll %r19,24,%g1; sra %g1,24,%r19
cmp %r19,26; bne L2423; nop
mov %r20,%r19
sll %r19,24,%g1; sra %g1,24,%r19
cmp %r19,26; bne L2423; nop
ld [%fp+-8],%r19
sll %r19,24,%g1; sra %g1,24,%r19
cmp %r19,26; bne L2423; nop
ld [%fp+-20],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
sll %r19,24,%g1; sra %g1,24,%r19
cmp %r19,26; bne L2423; nop
ld [%fp+-16],%f30; ld [%fp+-16+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
sll %r19,24,%g1; sra %g1,24,%r19
cmp %r19,26; be L2417; nop
L2423:
add %r26,1,%r26
L2417:
sll %r22,24,%g1; sra %g1,24,%r19
sll %r19,16,%g1; sra %g1,16,%r19
cmp %r19,26; bne L2430; nop
mov %r23,%r19
sll %r19,16,%g1; sra %g1,16,%r19
cmp %r19,26; bne L2430; nop
mov %r20,%r19
sll %r19,16,%g1; sra %g1,16,%r19
cmp %r19,26; bne L2430; nop
ld [%fp+-8],%r19
sll %r19,16,%g1; sra %g1,16,%r19
cmp %r19,26; bne L2430; nop
ld [%fp+-20],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
sll %r19,16,%g1; sra %g1,16,%r19
cmp %r19,26; bne L2430; nop
ld [%fp+-16],%f30; ld [%fp+-16+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
sll %r19,16,%g1; sra %g1,16,%r19
cmp %r19,26; be L2424; nop
L2430:
add %r26,2,%r26
L2424:
sll %r22,24,%g1; sra %g1,24,%r19
cmp %r19,26; bne L2437; nop
sll %r21,16,%g1; sra %g1,16,%r19
cmp %r19,26; bne L2437; nop
cmp %r20,26; bne L2437; nop
ld [%fp+-8],%r19
cmp %r19,26; bne L2437; nop
ld [%fp+-20],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
cmp %r19,26; bne L2437; nop
ld [%fp+-16],%f30; ld [%fp+-16+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
cmp %r19,26; be L2431; nop
L2437:
add %r26,4,%r26
L2431:
sll %r22,24,%g1; sra %g1,24,%r19
cmp %r19,26; bne L2444; nop
sll %r21,16,%g1; sra %g1,16,%r19
cmp %r19,26; bne L2444; nop
cmp %r23,26; bne L2444; nop
ld [%fp+-8],%r19
cmp %r19,26; bne L2444; nop
ld [%fp+-20],%f31
fstod %f31,%f30
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
cmp %r19,26; bne L2444; nop
ld [%fp+-16],%f30; ld [%fp+-16+4],%f31
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
cmp %r19,26; be L2438; nop
L2444:
add %r26,8,%r26
L2438:
sll %r22,24,%g1; sra %g1,24,%r19
cmp %r19,26; bne L2453; nop
sll %r21,16,%g1; sra %g1,16,%r19
cmp %r19,26; bne L2453; nop
cmp %r23,26; bne L2453; nop
cmp %r20,26; bne L2453; nop
ld [%fp+-20],%f31
fstod %f31,%f30
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L2454; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-24]
ba L2455; nop
L2454:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-24]
L2455:
ld [%fp+-24],%r19
cmp %r19,26; bne L2453; nop
ld [%fp+-16],%f30; ld [%fp+-16+4],%f31
set L835,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbul L2456; nop
fsubd %f30,%f28,%f28
fdtoi %f28,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
set 0x80000000,%r18
add %r19,%r18,%r19
st %r19,[%fp+-28]
ba L2457; nop
L2456:
fdtoi %f30,%f0; st %f0,[%sp+64]; ld [%sp+64],%r19
st %r19,[%fp+-28]
L2457:
ld [%fp+-28],%r19
cmp %r19,26; be L2445; nop
L2453:
add %r26,16,%r26
L2445:
sll %r22,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
fstod %f31,%f30
set L2415,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbne L2465; nop
sll %r21,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
fstod %f31,%f30
fcmped %f30,%f28; nop; fbne L2465; nop
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
fstod %f31,%f30
fcmped %f30,%f28; nop; fbne L2465; nop
st %r20,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
fstod %f31,%f30
fcmped %f30,%f28; nop; fbne L2465; nop
ld [%fp+-8],%r19
cmp %r19,%r0; bl L2466; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
ba L2467; nop
L2466:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-40]; st %f31,[%fp+-40+4]
L2467:
ld [%fp+-40],%f30; ld [%fp+-40+4],%f31
fdtos %f30,%f31
fstod %f31,%f30
set L2415,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbne L2465; nop
ld [%fp+-16],%f30; ld [%fp+-16+4],%f31
fdtos %f30,%f31
fstod %f31,%f30
fcmped %f30,%f28; nop; fbue L2458; nop
L2465:
add %r26,32,%r26
L2458:
sll %r22,24,%g1; sra %g1,24,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L2415,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbne L2475; nop
sll %r21,16,%g1; sra %g1,16,%r19
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fcmped %f30,%f28; nop; fbne L2475; nop
st %r23,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fcmped %f30,%f28; nop; fbne L2475; nop
st %r20,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fcmped %f30,%f28; nop; fbne L2475; nop
ld [%fp+-8],%r19
cmp %r19,%r0; bl L2476; nop
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
st %f30,[%fp+-48]; st %f31,[%fp+-48+4]
ba L2477; nop
L2476:
st %r19,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
set L600,%r19
ld [%r19],%f28; ld [%r19+4],%f29
faddd %f30,%f28,%f30
st %f30,[%fp+-48]; st %f31,[%fp+-48+4]
L2477:
ld [%fp+-48],%f30; ld [%fp+-48+4],%f31
set L2415,%r19
ld [%r19],%f28; ld [%r19+4],%f29
fcmped %f30,%f28; nop; fbne L2475; nop
ld [%fp+-20],%f31
fstod %f31,%f30
fcmped %f30,%f28; nop; fbue L2468; nop
L2475:
add %r26,64,%r26
L2468:
cmp %r26,%r0; be L2478; nop
add %r25,16,%r25
set _2394,%r8
set 16,%r9
call _printf; nop
L2478:
mov %r25,%r24
L2393:
ret; restore
.seg "data"
_2481:.ascii "s757,er%d\012\000"
_2482:.ascii "s757   \000"
.seg "text"
.global _s757
.align 4
.proc 4
_s757:save %sp,-208,%sp
set _2482,%r19
st %r19,[%fp+-76]
add %r24,60,%r19
st %r19,[%fp+-80]
st %r0,[%fp+-84]
L2483:
L2484:
ld [%fp+-80],%r19
add %r19,1,%r18
st %r18,[%fp+-80]
ld [%fp+-76],%r18
add %r18,1,%r17
st %r17,[%fp+-76]
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L2483; nop
set 40,%r19
st %r19,[%fp+-88]
ld [%fp+-88],%r19
sll %r19,3,%r18
sll %r18,2,%r18
cmp %r18,1280; bne L2488; nop
sra %r19,3,%r19
sra %r19,2,%r19
cmp %r19,1; be L2486; nop
L2488:
ld [%fp+-84],%r19
add %r19,1,%r19
st %r19,[%fp+-84]
ld [%r24+44],%r19
cmp %r19,%r0; be L2489; nop
set _2481,%r8
set 1,%r9
call _printf; nop
L2489:
L2486:
mov %r0,%r25
mov %r0,%r20
ba L2494; nop
L2491:
set 1,%r22
ld [%r24+16],%r19
sub %r19,1,%r19
sll %r22,%r19,%r21
st %r0,[%fp+-68]
ld [%fp+-68],%r19
xnor %r19,%g0,%r19
srl %r19,%r20,%r18
st %r18,[%fp+-72]
sll %r19,%r20,%r19
st %r19,[%fp+-68]
mov %r0,%r23
ba L2498; nop
L2495:
cmp %r23,%r20; bge L2506; nop
set 1,%r19
st %r19,[%fp+-96]
ba L2507; nop
L2506:
st %r0,[%fp+-96]
L2507:
ld [%fp+-68],%r19
and %r22,%r19,%r19
cmp %r19,%r0; bne L2508; nop
set 1,%r19
st %r19,[%fp+-100]
ba L2509; nop
L2508:
st %r0,[%fp+-100]
L2509:
ld [%fp+-96],%r19
ld [%fp+-100],%r18
cmp %r19,%r18; bne L2505; nop
cmp %r23,%r20; bge L2510; nop
set 1,%r19
st %r19,[%fp+-104]
ba L2511; nop
L2510:
st %r0,[%fp+-104]
L2511:
ld [%fp+-72],%r19
and %r21,%r19,%r19
cmp %r19,%r0; bne L2512; nop
set 1,%r19
st %r19,[%fp+-108]
ba L2513; nop
L2512:
st %r0,[%fp+-108]
L2513:
ld [%fp+-104],%r19
ld [%fp+-108],%r18
cmp %r19,%r18; be L2499; nop
L2505:
set 1,%r25
L2499:
sll %r22,1,%r22
srl %r21,1,%r21
L2496:
add %r23,1,%r23
L2498:
ld [%r24+16],%r19
cmp %r23,%r19; bl L2495; nop
L2492:
add %r20,1,%r20
L2494:
ld [%r24+16],%r19
cmp %r20,%r19; bl L2491; nop
cmp %r25,%r0; be L2514; nop
ld [%fp+-84],%r19
add %r19,2,%r19
st %r19,[%fp+-84]
ld [%r24+44],%r19
cmp %r19,%r0; be L2516; nop
set _2481,%r8
set 2,%r9
call _printf; nop
L2516:
L2514:
set 3,%r26
set 2,%r27
set 1,%r28
cmp %r26,%r27; bge L2524; nop
set 1,%r19
st %r19,[%fp+-100]
ba L2525; nop
L2524:
st %r0,[%fp+-100]
L2525:
ld [%fp+-100],%r19
cmp %r19,%r28; bge L2522; nop
set 1,%r19
st %r19,[%fp+-96]
ba L2523; nop
L2522:
st %r0,[%fp+-96]
L2523:
ld [%fp+-96],%r19
cmp %r19,1; be L2518; nop
ld [%fp+-84],%r19
add %r19,4,%r19
st %r19,[%fp+-84]
ld [%r24+44],%r19
cmp %r19,%r0; be L2526; nop
set _2481,%r8
set 4,%r9
call _printf; nop
L2526:
L2518:
add -60,%fp,%r19
add -64,%fp,%r18
cmp %r19,%r18; bne L2528; nop
ld [%fp+-84],%r19
add %r19,8,%r19
st %r19,[%fp+-84]
ld [%r24+44],%r19
cmp %r19,%r0; be L2531; nop
set _2481,%r8
set 8,%r9
call _printf; nop
L2531:
L2528:
add -60,%fp,%r19
add -64,%fp,%r18
cmp %r19,%r18; bgeu L2533; nop
ld [%r24+40],%r19
cmp %r19,%r0; be L2536; nop
set L2538,%r8
call _printf; nop
L2536:
L2533:
mov %r0,%r25
mov %r0,%r23
L2539:
set 1,%r19
sll %r23,2,%r18
add -64,%fp,%r17
st %r19,[%r18+%r17]
L2540:
add %r23,1,%r23
cmp %r23,16; bl L2539; nop
st %r0,[%fp+-60]
st %r0,[%fp+-48]
st %r0,[%fp+-40]
st %r0,[%fp+-36]
st %r0,[%fp+-28]
st %r0,[%fp+-12]
mov %r0,%r26
L2549:
mov %r0,%r27
L2553:
mov %r0,%r28
L2557:
mov %r0,%r29
L2561:
cmp %r26,%r27; bge L2572; nop
set 1,%r19
st %r19,[%fp+-108]
ba L2573; nop
L2572:
st %r0,[%fp+-108]
L2573:
cmp %r28,%r29; bge L2574; nop
set 1,%r19
st %r19,[%fp+-112]
ba L2575; nop
L2574:
st %r0,[%fp+-112]
L2575:
ld [%fp+-108],%r19
ld [%fp+-112],%r18
cmp %r19,%r18; bne L2570; nop
set 1,%r19
st %r19,[%fp+-104]
ba L2571; nop
L2570:
st %r0,[%fp+-104]
L2571:
ld [%fp+-104],%r19
sll %r26,3,%r18
sll %r27,2,%r17
add %r18,%r17,%r18
sll %r28,1,%r17
add %r18,%r17,%r18
add %r18,%r29,%r18
sll %r18,2,%r18
add -64,%fp,%r17
ld [%r18+%r17],%r18
cmp %r19,%r18; be L2565; nop
set 1,%r25
L2565:
L2562:
add %r29,1,%r29
cmp %r29,2; bl L2561; nop
L2558:
add %r28,1,%r28
cmp %r28,2; bl L2557; nop
L2554:
add %r27,1,%r27
cmp %r27,2; bl L2553; nop
L2550:
add %r26,1,%r26
cmp %r26,2; bl L2549; nop
cmp %r25,%r0; be L2576; nop
ld [%fp+-84],%r19
add %r19,16,%r19
st %r19,[%fp+-84]
ld [%r24+44],%r19
cmp %r19,%r0; be L2578; nop
set _2481,%r8
set 16,%r9
call _printf; nop
L2578:
L2576:
st %r0,[%fp+-92]
ld [%fp+-92],%r19
cmp %r19,%r0; be L2580; nop
ld [%fp+-84],%r19
add %r19,32,%r19
st %r19,[%fp+-84]
ld [%r24+44],%r19
cmp %r19,%r0; be L2582; nop
set _2481,%r8
set 32,%r9
call _printf; nop
L2582:
L2580:
ld [%fp+-84],%r24
L2480:
ret; restore
.seg "data"
_2585:.ascii "Local error %d.\012\000"
_2586:.ascii "s7813,er%d\012\000"
_2587:.ascii "s7813  \000"
.seg "text"
.global _s7813
.align 4
.proc 4
_s7813:save %sp,-144,%sp
set _2587,%r23
add %r24,60,%r22
mov %r0,%r28
mov %r0,%r21
ld [%r24+48],%r29
L2588:
L2589:
mov %r22,%r19
add %r19,1,%r22
mov %r23,%r18
add %r18,1,%r23
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L2588; nop
ba L2591; nop
set 1,%r28
cmp %r29,%r0; be L2593; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2593:
L2591:
ba L2595; nop
set 2,%r28
cmp %r29,%r0; be L2597; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2597:
L2595:
ba L2599; nop
set 3,%r28
cmp %r29,%r0; be L2601; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2601:
L2599:
ba L2603; nop
set 4,%r28
cmp %r29,%r0; be L2605; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2605:
L2603:
ba L2607; nop
set 5,%r28
cmp %r29,%r0; be L2609; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2609:
L2607:
ba L2611; nop
set 6,%r28
cmp %r29,%r0; be L2613; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2613:
L2611:
ba L2615; nop
set 7,%r28
cmp %r29,%r0; be L2617; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2617:
L2615:
ba L2619; nop
set 8,%r28
cmp %r29,%r0; be L2621; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2621:
L2619:
ba L2623; nop
set 9,%r28
cmp %r29,%r0; be L2625; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2625:
L2623:
ba L2627; nop
set 10,%r28
cmp %r29,%r0; be L2629; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2629:
L2627:
ba L2631; nop
set 11,%r28
cmp %r29,%r0; be L2633; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2633:
L2631:
ba L2635; nop
set 12,%r28
cmp %r29,%r0; be L2637; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2637:
L2635:
ba L2639; nop
set 13,%r28
cmp %r29,%r0; be L2641; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2641:
L2639:
ba L2643; nop
set 14,%r28
cmp %r29,%r0; be L2645; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2645:
L2643:
ba L2647; nop
set 15,%r28
cmp %r29,%r0; be L2649; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2649:
L2647:
ba L2651; nop
set 16,%r28
cmp %r29,%r0; be L2653; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2653:
L2651:
ba L2655; nop
set 17,%r28
cmp %r29,%r0; be L2657; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2657:
L2655:
ba L2659; nop
set 18,%r28
cmp %r29,%r0; be L2661; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2661:
L2659:
ba L2663; nop
set 16,%r28
cmp %r29,%r0; be L2665; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2665:
L2663:
ba L2667; nop
set 20,%r28
cmp %r29,%r0; be L2669; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2669:
L2667:
ba L2671; nop
set 21,%r28
cmp %r29,%r0; be L2673; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2673:
L2671:
ba L2675; nop
set 22,%r28
cmp %r29,%r0; be L2677; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2677:
L2675:
ba L2679; nop
set 23,%r28
cmp %r29,%r0; be L2681; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2681:
L2679:
ba L2683; nop
set 24,%r28
cmp %r29,%r0; be L2685; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2685:
L2683:
ba L2687; nop
set 25,%r28
cmp %r29,%r0; be L2689; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2689:
L2687:
ba L2691; nop
set 26,%r28
cmp %r29,%r0; be L2693; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2693:
L2691:
ba L2695; nop
set 27,%r28
cmp %r29,%r0; be L2697; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2697:
L2695:
ba L2699; nop
set 28,%r28
cmp %r29,%r0; be L2701; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2701:
L2699:
ba L2703; nop
set 26,%r28
cmp %r29,%r0; be L2705; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2705:
L2703:
ba L2707; nop
set 30,%r28
cmp %r29,%r0; be L2709; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2709:
L2707:
ba L2711; nop
set 31,%r28
cmp %r29,%r0; be L2713; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2713:
L2711:
ba L2715; nop
set 32,%r28
cmp %r29,%r0; be L2717; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2717:
L2715:
ba L2719; nop
set 33,%r28
cmp %r29,%r0; be L2721; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2721:
L2719:
ba L2723; nop
set 34,%r28
cmp %r29,%r0; be L2725; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2725:
L2723:
ba L2727; nop
set 35,%r28
cmp %r29,%r0; be L2729; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2729:
L2727:
ba L2731; nop
set 36,%r28
cmp %r29,%r0; be L2733; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2733:
L2731:
ba L2735; nop
set 37,%r28
cmp %r29,%r0; be L2737; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2737:
L2735:
ba L2739; nop
set 38,%r28
cmp %r29,%r0; be L2741; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2741:
L2739:
ba L2743; nop
set 39,%r28
cmp %r29,%r0; be L2745; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2745:
L2743:
ba L2747; nop
set 40,%r28
cmp %r29,%r0; be L2749; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2749:
L2747:
ba L2751; nop
set 41,%r28
cmp %r29,%r0; be L2753; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2753:
L2751:
ba L2755; nop
set 42,%r28
cmp %r29,%r0; be L2757; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2757:
L2755:
ba L2759; nop
set 43,%r28
cmp %r29,%r0; be L2761; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2761:
L2759:
ba L2763; nop
set 44,%r28
cmp %r29,%r0; be L2765; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2765:
L2763:
ba L2767; nop
set 45,%r28
cmp %r29,%r0; be L2769; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2769:
L2767:
ba L2771; nop
set 46,%r28
cmp %r29,%r0; be L2773; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2773:
L2771:
ba L2775; nop
set 47,%r28
cmp %r29,%r0; be L2777; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2777:
L2775:
ba L2779; nop
set 48,%r28
cmp %r29,%r0; be L2781; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2781:
L2779:
ba L2783; nop
set 49,%r28
cmp %r29,%r0; be L2785; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2785:
L2783:
ba L2787; nop
set 50,%r28
cmp %r29,%r0; be L2789; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2789:
L2787:
ba L2791; nop
set 51,%r28
cmp %r29,%r0; be L2793; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2793:
L2791:
ba L2795; nop
set 52,%r28
cmp %r29,%r0; be L2797; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2797:
L2795:
ba L2799; nop
set 53,%r28
cmp %r29,%r0; be L2801; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2801:
L2799:
ba L2803; nop
set 54,%r28
cmp %r29,%r0; be L2805; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2805:
L2803:
ba L2807; nop
set 55,%r28
cmp %r29,%r0; be L2809; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2809:
L2807:
ba L2811; nop
set 56,%r28
cmp %r29,%r0; be L2813; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2813:
L2811:
ba L2815; nop
set 57,%r28
cmp %r29,%r0; be L2817; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2817:
L2815:
ba L2819; nop
set 58,%r28
cmp %r29,%r0; be L2821; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2821:
L2819:
ba L2823; nop
set 56,%r28
cmp %r29,%r0; be L2825; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2825:
L2823:
ba L2827; nop
set 60,%r28
cmp %r29,%r0; be L2829; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2829:
L2827:
ba L2831; nop
set 61,%r28
cmp %r29,%r0; be L2833; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2833:
L2831:
ba L2835; nop
set 62,%r28
cmp %r29,%r0; be L2837; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2837:
L2835:
ba L2839; nop
set 63,%r28
cmp %r29,%r0; be L2841; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2841:
L2839:
ba L2843; nop
set 64,%r28
cmp %r29,%r0; be L2845; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2845:
L2843:
ba L2847; nop
set 65,%r28
cmp %r29,%r0; be L2849; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2849:
L2847:
ba L2851; nop
set 66,%r28
cmp %r29,%r0; be L2853; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2853:
L2851:
ba L2855; nop
set 67,%r28
cmp %r29,%r0; be L2857; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2857:
L2855:
ba L2859; nop
set 68,%r28
cmp %r29,%r0; be L2861; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2861:
L2859:
ba L2863; nop
set 69,%r28
cmp %r29,%r0; be L2865; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2865:
L2863:
ba L2867; nop
set 70,%r28
cmp %r29,%r0; be L2869; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2869:
L2867:
ba L2871; nop
set 71,%r28
cmp %r29,%r0; be L2873; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2873:
L2871:
ba L2875; nop
set 72,%r28
cmp %r29,%r0; be L2877; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2877:
L2875:
ba L2879; nop
set 73,%r28
cmp %r29,%r0; be L2881; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2881:
L2879:
ba L2883; nop
set 74,%r28
cmp %r29,%r0; be L2885; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2885:
L2883:
ba L2887; nop
set 75,%r28
cmp %r29,%r0; be L2889; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2889:
L2887:
cmp %r28,%r0; be L2891; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L2893; nop
set _2586,%r8
set 1,%r9
call _printf; nop
L2893:
add %r21,1,%r21
L2891:
mov %r0,%r28
mov %r0,%r26
mov %r0,%r27
mov %r27,%r19
add %r19,1,%r27
cmp %r19,%r0; be L2896; nop
mov %r26,%r19
add %r19,1,%r26
cmp %r19,%r0; be L2896; nop
set 1,%r19
st %r19,[%fp+-8]
ba L2897; nop
L2896:
st %r0,[%fp+-8]
L2897:
ld [%fp+-8],%r25
cmp %r27,1; be L2898; nop
set 1,%r28
cmp %r29,%r0; be L2900; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2900:
L2898:
cmp %r26,%r0; be L2902; nop
set 2,%r28
cmp %r29,%r0; be L2904; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2904:
L2902:
cmp %r25,%r0; be L2906; nop
set 3,%r28
cmp %r29,%r0; be L2908; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2908:
L2906:
cmp %r27,%r0; be L2911; nop
mov %r26,%r19
add %r19,1,%r26
cmp %r19,%r0; be L2911; nop
set 1,%r19
st %r19,[%fp+-12]
ba L2912; nop
L2911:
st %r0,[%fp+-12]
L2912:
ld [%fp+-12],%r25
cmp %r27,1; be L2913; nop
set 4,%r28
cmp %r29,%r0; be L2915; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2915:
L2913:
cmp %r26,1; be L2917; nop
set 5,%r28
cmp %r29,%r0; be L2919; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2919:
L2917:
cmp %r25,%r0; be L2921; nop
set 6,%r28
cmp %r29,%r0; be L2923; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2923:
L2921:
mov %r27,%r19
sub %r19,1,%r27
cmp %r19,%r0; be L2926; nop
cmp %r26,%r0; be L2926; nop
set 1,%r19
st %r19,[%fp+-16]
ba L2927; nop
L2926:
st %r0,[%fp+-16]
L2927:
ld [%fp+-16],%r25
cmp %r27,%r0; be L2928; nop
set 7,%r28
cmp %r29,%r0; be L2930; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2930:
L2928:
cmp %r26,1; be L2932; nop
set 8,%r28
cmp %r29,%r0; be L2934; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2934:
L2932:
cmp %r25,1; be L2936; nop
set 9,%r28
cmp %r29,%r0; be L2938; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2938:
L2936:
cmp %r27,%r0; be L2941; nop
mov %r26,%r19
sub %r19,1,%r26
cmp %r19,%r0; be L2941; nop
set 1,%r19
st %r19,[%fp+-20]
ba L2942; nop
L2941:
st %r0,[%fp+-20]
L2942:
ld [%fp+-20],%r25
cmp %r27,%r0; be L2943; nop
set 10,%r28
cmp %r29,%r0; be L2945; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2945:
L2943:
cmp %r26,1; be L2947; nop
set 11,%r28
cmp %r29,%r0; be L2949; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2949:
L2947:
cmp %r25,%r0; be L2951; nop
set 12,%r28
cmp %r29,%r0; be L2953; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2953:
L2951:
cmp %r28,%r0; be L2955; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L2957; nop
set _2586,%r8
set 2,%r9
call _printf; nop
L2957:
add %r21,2,%r21
L2955:
mov %r0,%r28
mov %r0,%r26
mov %r0,%r27
mov %r27,%r19
add %r19,1,%r27
cmp %r19,%r0; bne L2962; nop
cmp %r26,%r0; be L2960; nop
L2962:
set 1,%r19
st %r19,[%fp+-24]
ba L2961; nop
L2960:
st %r0,[%fp+-24]
L2961:
ld [%fp+-24],%r25
cmp %r27,1; be L2963; nop
set 1,%r28
cmp %r29,%r0; be L2965; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2965:
L2963:
cmp %r26,%r0; be L2967; nop
set 2,%r28
cmp %r29,%r0; be L2969; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2969:
L2967:
cmp %r25,%r0; be L2971; nop
set 3,%r28
cmp %r29,%r0; be L2973; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2973:
L2971:
mov %r26,%r19
add %r19,1,%r26
cmp %r19,%r0; bne L2978; nop
cmp %r27,%r0; be L2976; nop
L2978:
set 1,%r19
st %r19,[%fp+-28]
ba L2977; nop
L2976:
st %r0,[%fp+-28]
L2977:
ld [%fp+-28],%r25
cmp %r27,1; be L2979; nop
set 4,%r28
cmp %r29,%r0; be L2981; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2981:
L2979:
cmp %r26,1; be L2983; nop
set 5,%r28
cmp %r29,%r0; be L2985; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2985:
L2983:
cmp %r25,1; be L2987; nop
set 6,%r28
cmp %r29,%r0; be L2989; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2989:
L2987:
mov %r27,%r19
sub %r19,1,%r27
cmp %r19,%r0; bne L2994; nop
mov %r26,%r19
sub %r19,1,%r26
cmp %r19,%r0; be L2992; nop
L2994:
set 1,%r19
st %r19,[%fp+-32]
ba L2993; nop
L2992:
st %r0,[%fp+-32]
L2993:
ld [%fp+-32],%r25
cmp %r27,%r0; be L2995; nop
set 7,%r28
cmp %r29,%r0; be L2997; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L2997:
L2995:
cmp %r26,1; be L2999; nop
set 8,%r28
cmp %r29,%r0; be L3001; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L3001:
L2999:
cmp %r25,1; be L3003; nop
set 9,%r28
cmp %r29,%r0; be L3005; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L3005:
L3003:
cmp %r27,%r0; bne L3010; nop
mov %r26,%r19
sub %r19,1,%r26
cmp %r19,%r0; be L3008; nop
L3010:
set 1,%r19
st %r19,[%fp+-36]
ba L3009; nop
L3008:
st %r0,[%fp+-36]
L3009:
ld [%fp+-36],%r25
cmp %r27,%r0; be L3011; nop
set 10,%r28
cmp %r29,%r0; be L3013; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L3013:
L3011:
cmp %r26,%r0; be L3015; nop
set 11,%r28
cmp %r29,%r0; be L3017; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L3017:
L3015:
cmp %r25,1; be L3019; nop
set 12,%r28
cmp %r29,%r0; be L3021; nop
set _2585,%r8
mov %r28,%r9
call _printf; nop
L3021:
L3019:
cmp %r28,%r0; be L3023; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L3025; nop
set _2586,%r8
set 4,%r9
call _printf; nop
L3025:
add %r21,4,%r21
L3023:
mov %r0,%r26
mov %r0,%r27
mov %r0,%r20
set 1,%r19
st %r19,[%fp+-4]
ld [%fp+-4],%r19
cmp %r19,%r0; be L3029; nop
st %r20,[%fp+-40]
ba L3030; nop
L3029:
ld [%fp+-4],%r19
cmp %r19,%r0; be L3031; nop
mov %r27,%r19
add %r19,1,%r27
st %r19,[%fp+-44]
ba L3032; nop
L3031:
mov %r26,%r19
add %r19,1,%r26
st %r19,[%fp+-44]
L3032:
ld [%fp+-44],%r19
st %r19,[%fp+-40]
L3030:
ld [%fp+-40],%r25
cmp %r25,%r0; bne L3036; nop
cmp %r27,%r0; bne L3036; nop
cmp %r26,%r0; be L3033; nop
L3036:
ld [%r24+44],%r19
cmp %r19,%r0; be L3037; nop
set _2586,%r8
set 8,%r9
call _printf; nop
L3037:
add %r21,8,%r21
L3033:
ld [%fp+-4],%r19
cmp %r19,%r0; be L3044; nop
st %r20,[%fp+-48]
ba L3045; nop
L3044:
set 1,%r19
st %r19,[%fp+-48]
L3045:
ld [%fp+-48],%r19
cmp %r19,%r0; bne L3043; nop
cmp %r20,%r0; be L3046; nop
set 1,%r19
st %r19,[%fp+-52]
ba L3047; nop
L3046:
st %r20,[%fp+-52]
L3047:
ld [%fp+-52],%r19
cmp %r19,%r0; be L3039; nop
L3043:
ld [%r24+44],%r19
cmp %r19,%r0; be L3048; nop
set _2586,%r8
set 16,%r9
call _printf; nop
L3048:
add %r21,16,%r21
L3039:
mov %r21,%r24
L2584:
ret; restore
.seg "data"
_3051:.ascii "s81,er%d\012\000"
_3052:.ascii "s81    \000"
_3053:.ascii "Register count for %s is unreliable.\012\000"
_3054:.ascii "%d registers assigned to %s variables.\012\000"
.seg "text"
.global _s81
.align 4
.proc 4
_s81:save %sp,-112,%sp
st %r0,[%fp+-20]
st %r0,[%fp+-8]
st %r0,[%fp+-12]
st %r0,[%fp+-16]
set _3052,%r21
add %r24,60,%r20
L3055:
L3056:
mov %r20,%r19
add %r19,1,%r20
mov %r21,%r18
add %r18,1,%r21
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L3055; nop
set 1,%r19
st %r19,[%fp+-4]
mov %r0,%r26
L3058:
ld [%fp+-4],%r19
mov %r19,%r18
mov %r18,%r29
mov %r18,%r25
add -4,%fp,%r18
mov %r18,%r28
mov %r18,%r23
mov %r19,%r27
mov %r19,%r22
sll %r29,24,%g1; sra %g1,24,%r19
sll %r25,24,%g1; sra %g1,24,%r18
cmp %r19,%r18; be L3062; nop
set 1,%r19
st %r19,[%fp+-8]
L3062:
cmp %r28,%r23; be L3064; nop
set 1,%r19
st %r19,[%fp+-12]
L3064:
cmp %r27,%r22; be L3066; nop
set 1,%r19
st %r19,[%fp+-16]
L3066:
ld [%fp+-4],%r19
sll %r19,1,%r19
st %r19,[%fp+-4]
L3059:
add %r26,1,%r26
cmp %r26,50; bl L3058; nop
ld [%fp+-8],%r19
cmp %r19,%r0; be L3068; nop
ld [%fp+-20],%r19
add %r19,1,%r19
st %r19,[%fp+-20]
ld [%r24+44],%r19
cmp %r19,%r0; be L3070; nop
set _3051,%r8
set 1,%r9
call _printf; nop
L3070:
L3068:
ld [%fp+-12],%r19
cmp %r19,%r0; be L3072; nop
ld [%fp+-20],%r19
add %r19,2,%r19
st %r19,[%fp+-20]
ld [%r24+44],%r19
cmp %r19,%r0; be L3074; nop
set _3051,%r8
set 2,%r9
call _printf; nop
L3074:
L3072:
ld [%fp+-16],%r19
cmp %r19,%r0; be L3076; nop
ld [%fp+-20],%r19
add %r19,4,%r19
st %r19,[%fp+-20]
ld [%r24+44],%r19
cmp %r19,%r0; be L3078; nop
set _3051,%r8
set 4,%r9
call _printf; nop
L3078:
L3076:
call _regc; nop
st %r8,[%fp+-4]
ld [%r24+40],%r19
cmp %r19,%r0; be L3080; nop
ld [%fp+-4],%r19
cmp %r19,%r0; bge L3082; nop
set _3053,%r8
set L425,%r9
call _printf; nop
ba L3083; nop
L3082:
set _3054,%r8
ld [%fp+-4],%r9
set L425,%r10
call _printf; nop
L3083:
L3080:
call _regp; nop
st %r8,[%fp+-4]
ld [%r24+40],%r19
cmp %r19,%r0; be L3084; nop
ld [%fp+-4],%r19
cmp %r19,%r0; bge L3086; nop
set _3053,%r8
set L3088,%r9
call _printf; nop
ba L3087; nop
L3086:
set _3054,%r8
ld [%fp+-4],%r9
set L3088,%r10
call _printf; nop
L3087:
L3084:
call _regi; nop
st %r8,[%fp+-4]
ld [%r24+40],%r19
cmp %r19,%r0; be L3089; nop
ld [%fp+-4],%r19
cmp %r19,%r0; bge L3091; nop
set _3053,%r8
set L426,%r9
call _printf; nop
ba L3092; nop
L3091:
set _3054,%r8
ld [%fp+-4],%r9
set L426,%r10
call _printf; nop
L3092:
L3089:
ld [%fp+-20],%r24
L3050:
ret; restore
.global _regc
.align 4
.proc 4
_regc:save %sp,-232,%sp
stb %r0,[%fp+-130]
set 1,%r19
stb %r19,[%fp+-109]
set 2,%r19
stb %r19,[%fp+-110]
set 3,%r19
stb %r19,[%fp+-111]
set 4,%r29
set 5,%r19
stb %r19,[%fp+-112]
set 6,%r28
set 7,%r19
stb %r19,[%fp+-113]
set 8,%r27
set 9,%r19
stb %r19,[%fp+-114]
set 10,%r26
set 11,%r19
stb %r19,[%fp+-115]
set 12,%r25
set 13,%r19
stb %r19,[%fp+-116]
set 14,%r24
set 15,%r19
stb %r19,[%fp+-117]
set 16,%r23
set 17,%r19
stb %r19,[%fp+-118]
set 18,%r22
set 19,%r19
stb %r19,[%fp+-119]
set 20,%r21
set 21,%r19
stb %r19,[%fp+-120]
set 22,%r20
set 23,%r19
stb %r19,[%fp+-121]
set 24,%r19
stb %r19,[%fp+-1]
set 25,%r19
stb %r19,[%fp+-122]
set 26,%r19
stb %r19,[%fp+-2]
set 27,%r19
stb %r19,[%fp+-123]
set 28,%r19
stb %r19,[%fp+-3]
set 29,%r19
stb %r19,[%fp+-124]
set 30,%r19
stb %r19,[%fp+-4]
set 31,%r19
stb %r19,[%fp+-125]
set 32,%r19
stb %r19,[%fp+-5]
set 33,%r19
stb %r19,[%fp+-126]
set 34,%r19
stb %r19,[%fp+-6]
set 35,%r19
stb %r19,[%fp+-127]
set 36,%r19
stb %r19,[%fp+-128]
set 37,%r19
stb %r19,[%fp+-129]
set 38,%r19
stb %r19,[%fp+-131]
add -109,%fp,%r19
add -130,%fp,%r18
sub %r19,%r18,%r18
st %r18,[%fp+-96]
add -110,%fp,%r18
sub %r18,%r19,%r19
st %r19,[%fp+-92]
add -111,%fp,%r19
add -110,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-88]
add -112,%fp,%r19
add -111,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-84]
add -113,%fp,%r19
add -112,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-80]
add -114,%fp,%r19
add -113,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-76]
add -115,%fp,%r19
add -114,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-72]
add -116,%fp,%r19
add -115,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-68]
add -117,%fp,%r19
add -116,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-64]
add -118,%fp,%r19
add -117,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-60]
add -119,%fp,%r19
add -118,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-56]
add -120,%fp,%r19
add -119,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-52]
add -121,%fp,%r19
add -120,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-48]
add -122,%fp,%r19
add -121,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-44]
add -123,%fp,%r19
add -122,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-40]
add -124,%fp,%r19
add -123,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-36]
add -125,%fp,%r19
add -124,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-32]
add -126,%fp,%r19
add -125,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-28]
add -127,%fp,%r19
add -126,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-24]
add -128,%fp,%r19
add -127,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-20]
add -129,%fp,%r19
add -128,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-16]
add -131,%fp,%r19
add -129,%fp,%r18
sub %r19,%r18,%r19
st %r19,[%fp+-12]
ld [%fp+-96],%r19
st %r19,[%fp+-108]
set 1,%r19
st %r19,[%fp+-104]
st %r0,[%fp+-100]
L3115:
ld [%fp+-104],%r19
cmp %r19,1; be L3121; nop
cmp %r19,2; be L3124; nop
cmp %r19,3; be L3129; nop
ba L3119; nop
L3121:
ld [%fp+-100],%r19
sll %r19,2,%r19
add -96,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-108],%r18
cmp %r19,%r18; be L3120; nop
ld [%fp+-100],%r19
sll %r19,2,%r19
add -96,%fp,%r18
ld [%r19+%r18],%r19
st %r19,[%fp+-140]
set 2,%r19
st %r19,[%fp+-104]
set 1,%r19
st %r19,[%fp+-136]
ba L3120; nop
L3124:
ld [%fp+-100],%r19
sll %r19,2,%r19
add -96,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-108],%r18
cmp %r19,%r18; bne L3125; nop
set 3,%r19
st %r19,[%fp+-104]
ba L3120; nop
L3125:
ld [%fp+-100],%r19
sll %r19,2,%r19
add -96,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-140],%r18
cmp %r19,%r18; bne L3127; nop
ld [%fp+-136],%r19
add %r19,1,%r19
st %r19,[%fp+-136]
ba L3120; nop
L3127:
set 4,%r19
st %r19,[%fp+-104]
ba L3120; nop
L3129:
ld [%fp+-100],%r19
sll %r19,2,%r19
add -96,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-108],%r18
cmp %r19,%r18; be L3120; nop
set 4,%r19
st %r19,[%fp+-104]
L3119:
L3120:
L3116:
ld [%fp+-100],%r19
add %r19,1,%r19
st %r19,[%fp+-100]
ld [%fp+-100],%r19
cmp %r19,22; bl L3115; nop
ld [%fp+-104],%r19
cmp %r19,3; bne L3132; nop
set 16,%r19
ld [%fp+-136],%r18
sub %r19,%r18,%r24
ba L3093; nop
L3132:
set -1,%r24
L3093:
ret; restore
.global _regi
.align 4
.proc 4
_regi:save %sp,-320,%sp
st %r0,[%fp+-212]
set 1,%r19
st %r19,[%fp+-128]
set 2,%r19
st %r19,[%fp+-132]
set 3,%r19
st %r19,[%fp+-136]
set 4,%r19
mov %r19,%r29
set 5,%r18
st %r18,[%fp+-140]
set 6,%r28
set 7,%r18
st %r18,[%fp+-144]
set 8,%r27
set 9,%r18
st %r18,[%fp+-148]
set 10,%r26
set 11,%r18
st %r18,[%fp+-152]
set 12,%r25
set 13,%r18
st %r18,[%fp+-156]
set 14,%r24
set 15,%r18
st %r18,[%fp+-160]
set 16,%r23
set 17,%r18
st %r18,[%fp+-164]
set 18,%r22
set 19,%r18
st %r18,[%fp+-168]
set 20,%r21
set 21,%r18
st %r18,[%fp+-172]
set 22,%r20
set 23,%r18
st %r18,[%fp+-176]
set 24,%r18
st %r18,[%fp+-4]
set 25,%r18
st %r18,[%fp+-180]
set 26,%r18
st %r18,[%fp+-8]
set 27,%r18
st %r18,[%fp+-184]
set 28,%r18
st %r18,[%fp+-12]
set 29,%r18
st %r18,[%fp+-188]
set 30,%r18
st %r18,[%fp+-16]
set 31,%r18
st %r18,[%fp+-192]
set 32,%r18
st %r18,[%fp+-20]
set 33,%r18
st %r18,[%fp+-196]
set 34,%r18
st %r18,[%fp+-24]
set 35,%r18
st %r18,[%fp+-200]
set 36,%r18
st %r18,[%fp+-204]
set 37,%r18
st %r18,[%fp+-208]
set 38,%r18
st %r18,[%fp+-216]
add -128,%fp,%r18
add -212,%fp,%r17
sub %r18,%r17,%r17
mov %r19,%o1; call .div,2; mov %r17,%o0; mov %o0,%r17
st %r17,[%fp+-112]
add -132,%fp,%r17
sub %r17,%r18,%r18
mov %r19,%o1; call .div,2; mov %r18,%o0; mov %o0,%r19
st %r19,[%fp+-108]
add -136,%fp,%r19
add -132,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-104]
add -140,%fp,%r19
add -136,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-100]
add -144,%fp,%r19
add -140,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-96]
add -148,%fp,%r19
add -144,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-92]
add -152,%fp,%r19
add -148,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-88]
add -156,%fp,%r19
add -152,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-84]
add -160,%fp,%r19
add -156,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-80]
add -164,%fp,%r19
add -160,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-76]
add -168,%fp,%r19
add -164,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-72]
add -172,%fp,%r19
add -168,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-68]
add -176,%fp,%r19
add -172,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-64]
add -180,%fp,%r19
add -176,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-60]
add -184,%fp,%r19
add -180,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-56]
add -188,%fp,%r19
add -184,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-52]
add -192,%fp,%r19
add -188,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-48]
add -196,%fp,%r19
add -192,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-44]
add -200,%fp,%r19
add -196,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-40]
add -204,%fp,%r19
add -200,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-36]
add -208,%fp,%r19
add -204,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-32]
add -216,%fp,%r19
add -208,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-28]
ld [%fp+-112],%r19
st %r19,[%fp+-124]
set 1,%r19
st %r19,[%fp+-120]
st %r0,[%fp+-116]
L3156:
ld [%fp+-120],%r19
cmp %r19,1; be L3162; nop
cmp %r19,2; be L3165; nop
cmp %r19,3; be L3170; nop
ba L3160; nop
L3162:
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-124],%r18
cmp %r19,%r18; be L3161; nop
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
st %r19,[%fp+-224]
set 2,%r19
st %r19,[%fp+-120]
set 1,%r19
st %r19,[%fp+-220]
ba L3161; nop
L3165:
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-124],%r18
cmp %r19,%r18; bne L3166; nop
set 3,%r19
st %r19,[%fp+-120]
ba L3161; nop
L3166:
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-224],%r18
cmp %r19,%r18; bne L3168; nop
ld [%fp+-220],%r19
add %r19,1,%r19
st %r19,[%fp+-220]
ba L3161; nop
L3168:
set 4,%r19
st %r19,[%fp+-120]
ba L3161; nop
L3170:
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-124],%r18
cmp %r19,%r18; be L3161; nop
set 4,%r19
st %r19,[%fp+-120]
L3160:
L3161:
L3157:
ld [%fp+-116],%r19
add %r19,1,%r19
st %r19,[%fp+-116]
ld [%fp+-116],%r19
cmp %r19,22; bl L3156; nop
ld [%fp+-120],%r19
cmp %r19,3; bne L3173; nop
set 16,%r19
ld [%fp+-220],%r18
sub %r19,%r18,%r24
ba L3134; nop
L3173:
set -1,%r24
L3134:
ret; restore
.global _regp
.align 4
.proc 4
_regp:save %sp,-320,%sp
add -212,%fp,%r19
st %r19,[%fp+-212]
add -188,%fp,%r18
st %r18,[%fp+-188]
add -192,%fp,%r17
st %r17,[%fp+-192]
add -196,%fp,%r16
st %r16,[%fp+-196]
add -124,%fp,%r16
mov %r16,%r29
st %r16,[%fp+-124]
add -128,%fp,%r16
mov %r16,%r28
st %r16,[%fp+-128]
add -132,%fp,%r16
mov %r16,%r27
st %r16,[%fp+-132]
add -136,%fp,%r16
mov %r16,%r26
st %r16,[%fp+-136]
add -140,%fp,%r16
mov %r16,%r25
st %r16,[%fp+-140]
add -144,%fp,%r16
mov %r16,%r24
st %r16,[%fp+-144]
add -148,%fp,%r16
mov %r16,%r23
st %r16,[%fp+-148]
add -152,%fp,%r16
mov %r16,%r22
st %r16,[%fp+-152]
add -156,%fp,%r16
mov %r16,%r21
st %r16,[%fp+-156]
add -160,%fp,%r16
mov %r16,%r20
st %r16,[%fp+-160]
add -164,%fp,%r16
st %r16,[%fp+-4]
st %r16,[%fp+-164]
add -168,%fp,%r16
st %r16,[%fp+-8]
st %r16,[%fp+-168]
add -172,%fp,%r16
st %r16,[%fp+-12]
st %r16,[%fp+-172]
add -176,%fp,%r16
st %r16,[%fp+-16]
st %r16,[%fp+-176]
add -180,%fp,%r16
st %r16,[%fp+-20]
st %r16,[%fp+-180]
add -184,%fp,%r16
st %r16,[%fp+-24]
st %r16,[%fp+-184]
add -200,%fp,%r16
st %r16,[%fp+-200]
add -204,%fp,%r16
st %r16,[%fp+-204]
add -216,%fp,%r16
st %r16,[%fp+-216]
sub %r18,%r19,%r19
set 4,%r16
mov %r16,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-112]
sub %r17,%r18,%r19
mov %r16,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-108]
add -196,%fp,%r19
add -192,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-104]
add -124,%fp,%r19
add -196,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-100]
add -128,%fp,%r19
add -124,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-96]
add -132,%fp,%r19
add -128,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-92]
add -136,%fp,%r19
add -132,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-88]
add -140,%fp,%r19
add -136,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-84]
add -144,%fp,%r19
add -140,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-80]
add -148,%fp,%r19
add -144,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-76]
add -152,%fp,%r19
add -148,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-72]
add -156,%fp,%r19
add -152,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-68]
add -160,%fp,%r19
add -156,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-64]
add -164,%fp,%r19
add -160,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-60]
add -168,%fp,%r19
add -164,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-56]
add -172,%fp,%r19
add -168,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-52]
add -176,%fp,%r19
add -172,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-48]
add -180,%fp,%r19
add -176,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-44]
add -184,%fp,%r19
add -180,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-40]
add -200,%fp,%r19
add -184,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-36]
add -204,%fp,%r19
add -200,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-32]
add -216,%fp,%r19
add -204,%fp,%r18
sub %r19,%r18,%r19
set 4,%r18
mov %r18,%o1; call .div,2; mov %r19,%o0; mov %o0,%r19
st %r19,[%fp+-28]
ld [%fp+-112],%r19
st %r19,[%fp+-208]
set 1,%r19
st %r19,[%fp+-120]
st %r0,[%fp+-116]
L3197:
ld [%fp+-120],%r19
cmp %r19,1; be L3203; nop
cmp %r19,2; be L3206; nop
cmp %r19,3; be L3211; nop
ba L3201; nop
L3203:
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-208],%r18
cmp %r19,%r18; be L3202; nop
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
st %r19,[%fp+-224]
set 2,%r19
st %r19,[%fp+-120]
set 1,%r19
st %r19,[%fp+-220]
ba L3202; nop
L3206:
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-208],%r18
cmp %r19,%r18; bne L3207; nop
set 3,%r19
st %r19,[%fp+-120]
ba L3202; nop
L3207:
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-224],%r18
cmp %r19,%r18; bne L3209; nop
ld [%fp+-220],%r19
add %r19,1,%r19
st %r19,[%fp+-220]
ba L3202; nop
L3209:
set 4,%r19
st %r19,[%fp+-120]
ba L3202; nop
L3211:
ld [%fp+-116],%r19
sll %r19,2,%r19
add -112,%fp,%r18
ld [%r19+%r18],%r19
ld [%fp+-208],%r18
cmp %r19,%r18; be L3202; nop
set 4,%r19
st %r19,[%fp+-120]
L3201:
L3202:
L3198:
ld [%fp+-116],%r19
add %r19,1,%r19
st %r19,[%fp+-116]
ld [%fp+-116],%r19
cmp %r19,22; bl L3197; nop
ld [%fp+-120],%r19
cmp %r19,3; bne L3214; nop
set 16,%r19
ld [%fp+-220],%r18
sub %r19,%r18,%r24
ba L3175; nop
L3214:
set -1,%r24
L3175:
ret; restore
.seg "bss"
.align 4; .reserve _3217,420
.seg "data"
_3218:.ascii "s84,er%d\012\000"
_3219:.ascii "s84    \000"
.seg "text"
.global _s84
.align 4
.proc 4
_s84:save %sp,-248,%sp
set _3219,%r26
add %r24,60,%r25
mov %r0,%r23
L3220:
L3221:
mov %r25,%r20
add %r20,1,%r25
mov %r26,%r19
add %r19,1,%r26
ldsb [%r19],%r19
stb %r19,[%r20]
sll %r19,24,%g1; sra %g1,24,%r20
cmp %r20,%r0; bne L3220; nop
set 3,%r8
call _fip; nop
mov %r8,%r22
ld [%r22],%r20
cmp %r20,3; be L3223; nop
ld [%r24+44],%r20
cmp %r20,%r0; be L3225; nop
set _3218,%r8
set 1,%r9
call _printf; nop
L3225:
add %r23,1,%r23
L3223:
set _glork,%r21
set 4,%r8
call %r21; nop
cmp %r8,4; be L3227; nop
ld [%r24+44],%r20
cmp %r20,%r0; be L3229; nop
set _3218,%r8
set 2,%r9
call _printf; nop
L3229:
add %r23,2,%r23
L3227:
mov %r0,%r28
L3231:
st %r28,[%sp+64]; ld [%sp+64],%f30; fitod %f30,%f30
fdtos %f30,%f31
sll %r28,2,%r20
add -68,%fp,%r19
st %f31,[%r20+%r19]
sll %r28,2,%r20
add -68,%fp,%r19
add %r20,%r19,%r19
add -136,%fp,%r18
st %r19,[%r20+%r18]
L3232:
add %r28,1,%r28
cmp %r28,17; bl L3231; nop
set L412,%r20
ld [%r20],%f31
st %f31,[%fp+-140]
mov %r0,%r28
L3235:
ld [%fp+-140],%f31
sll %r28,2,%r20
add -136,%fp,%r19
ld [%r20+%r19],%r20
ld [%r20],%f30
fadds %f31,%f30,%f31
st %f31,[%fp+-140]
L3236:
add %r28,1,%r28
cmp %r28,17; bl L3235; nop
ld [%fp+-140],%f31
set L3241,%r20
ld [%r20],%f30
fcmpes %f31,%f30; nop; fbue L3239; nop
ld [%r24+44],%r20
cmp %r20,%r0; be L3242; nop
set _3218,%r8
set 4,%r9
call _printf; nop
L3242:
add %r23,4,%r23
L3239:
mov %r0,%r27
L3244:
mov %r0,%r28
L3248:
mov %r0,%r29
L3252:
set 28,%r20
mov %r28,%o1; call .mul,2; mov %r20,%o0; mov %o0,%r20
set 140,%r19
mov %r27,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
set 35,%r18
mov %r27,%o1; call .mul,2; mov %r18,%o0; mov %o0,%r18
set 7,%r17
mov %r28,%o1; call .mul,2; mov %r17,%o0; mov %o0,%r17
add %r18,%r17,%r18
add %r18,%r29,%r18
sll %r29,2,%r17
set _3217,%r16
add %r19,%r16,%r19
add %r20,%r19,%r20
st %r18,[%r17+%r20]
L3253:
add %r29,1,%r29
cmp %r29,7; bl L3252; nop
L3249:
add %r28,1,%r28
cmp %r28,5; bl L3248; nop
L3245:
add %r27,1,%r27
cmp %r27,3; bl L3244; nop
set 1,%r27
set 2,%r28
set 3,%r29
set _3217,%r8
set 105,%r9
mov %r0,%r10
call _array; nop
st %r8,[%fp+-144]
set 140,%r20
mov %r27,%o1; call .mul,2; mov %r20,%o0; mov %o0,%r20
set _3217,%r19
add %r20,%r19,%r8
set 35,%r20
mov %r20,%r9
mov %r20,%r10
call _array; nop
st %r8,[%fp+-148]
set 28,%r20
mov %r28,%o1; call .mul,2; mov %r20,%o0; mov %o0,%r20
set 140,%r19
mov %r27,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
set _3217,%r18
add %r19,%r18,%r19
add %r20,%r19,%r8
set 7,%r9
set 49,%r10
call _array; nop
st %r8,[%fp+-152]
set 28,%r20
mov %r28,%o1; call .mul,2; mov %r20,%o0; mov %o0,%r20
set 140,%r19
mov %r27,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
ld [%fp+-144],%r18
ld [%fp+-148],%r17
add %r18,%r17,%r18
ld [%fp+-152],%r17
add %r18,%r17,%r18
sll %r29,2,%r17
set _3217,%r16
add %r19,%r16,%r19
add %r20,%r19,%r20
ld [%r17+%r20],%r20
add %r18,%r20,%r20
sub %r20,52,%r20
cmp %r20,%r0; be L3256; nop
ld [%r24+44],%r20
cmp %r20,%r0; be L3258; nop
set _3218,%r8
set 8,%r9
call _printf; nop
L3258:
add %r23,8,%r23
L3256:
mov %r23,%r24
L3216:
ret; restore
.global _array
.align 4
.proc 4
_array:
mov %r0,%r13
ba L3267; nop
L3264:
sll %r13,2,%r12
ld [%r12+%r8],%r12
add %r13,%r10,%r11
cmp %r12,%r11; be L3268; nop
set 1,%r8
ba L3263; nop
L3268:
L3265:
add %r13,1,%r13
L3267:
cmp %r13,%r9; bl L3264; nop
mov %r0,%r8
L3263:
retl; nop
.seg "bss"
.align 4; .reserve _3271,4
.seg "text"
.global _fip
.align 4
.proc 4
_fip:
set _3271,%r13
st %r8,[%r13]
mov %r13,%r8
L3270:
retl; nop
.global _glork
.align 4
.proc 4
_glork:
L3272:
retl; nop
.seg "data"
_3274:.ascii "s85,er%d\012\000"
_3275:.ascii "s85    \000"
.align 4; _3283:.word L425
.word L427
.word L426
.word L428
.word L429
.word L430
.word L431
_3284:.ascii " alignment: \000"
.seg "text"
.global _s85
.align 4
.proc 4
_s85:save %sp,-488,%sp
set _3275,%r28
add %r24,60,%r27
mov %r0,%r26
L3288:
L3289:
mov %r27,%r25
add %r25,1,%r27
mov %r28,%r23
add %r23,1,%r28
ldsb [%r23],%r23
stb %r23,[%r25]
sll %r23,24,%g1; sra %g1,24,%r25
cmp %r25,%r0; bne L3288; nop
add -284,%fp,%r25
add -304,%fp,%r23
sub %r25,%r23,%r25
cmp %r25,%r0; ble L3299; nop
add -280,%fp,%r25
add -284,%fp,%r23
sub %r25,%r23,%r25
cmp %r25,%r0; ble L3299; nop
add -276,%fp,%r25
add -280,%fp,%r23
sub %r25,%r23,%r25
cmp %r25,%r0; bg L3291; nop
L3299:
ld [%r24+44],%r25
cmp %r25,%r0; be L3300; nop
set _3274,%r8
set 1,%r9
call _printf; nop
L3300:
add %r26,1,%r26
L3291:
add -337,%fp,%r25
add -338,%fp,%r23
sub %r25,%r23,%r25
st %r25,[%fp+-268]
add -340,%fp,%r25
add -342,%fp,%r23
sub %r25,%r23,%r25
st %r25,[%fp+-264]
add -348,%fp,%r25
add -352,%fp,%r23
sub %r25,%r23,%r25
st %r25,[%fp+-260]
add -356,%fp,%r25
add -360,%fp,%r23
sub %r25,%r23,%r25
st %r25,[%fp+-256]
add -364,%fp,%r25
add -368,%fp,%r23
sub %r25,%r23,%r25
st %r25,[%fp+-252]
add -372,%fp,%r25
add -376,%fp,%r23
sub %r25,%r23,%r25
st %r25,[%fp+-248]
add -384,%fp,%r25
add -392,%fp,%r23
sub %r25,%r23,%r25
st %r25,[%fp+-244]
ld [%r24+40],%r25
cmp %r25,%r0; be L3315; nop
mov %r0,%r29
L3317:
set L3321,%r8
sll %r29,2,%r25
set _3283,%r23
ld [%r25+%r23],%r9
set _3284,%r10
add -268,%fp,%r23
ld [%r25+%r23],%r11
call _printf; nop
L3318:
add %r29,1,%r29
cmp %r29,7; bl L3317; nop
L3315:
ld [%fp+-272],%r25
set 0x1c000000,%r23
or %r25,%r23,%r25
st %r25,[%fp+-272]
ld [%fp+-272],%r25
set 0x3fffffff,%r23
and %r25,%r23,%r23
sll %r25,3,%r25
sra %r25,29,%r25
sll %r25,30,%r25
sra %r25,30,%r25
sll %r25,30,%r25
set 0xc0000000,%r22
and %r25,%r22,%r25
or %r23,%r25,%r25
st %r25,[%fp+-272]
ld [%fp+-272],%r25
set 0xe3ffffff,%r23
and %r25,%r23,%r23
sra %r25,30,%r25
sll %r25,29,%r25
sra %r25,29,%r25
sll %r25,26,%r25
set 0x1c000000,%r22
and %r25,%r22,%r25
or %r23,%r25,%r25
st %r25,[%fp+-272]
ld [%fp+-272],%r25
sll %r25,3,%r25
sra %r25,29,%r25
cmp %r25,3; be L3322; nop
ld [%fp+-272],%r25
sll %r25,3,%r25
sra %r25,29,%r25
cmp %r25,-1; bne L3324; nop
ld [%r24+40],%r25
cmp %r25,%r0; be L3325; nop
set L3328,%r8
call _printf; nop
ba L3325; nop
L3324:
ld [%r24+44],%r25
cmp %r25,%r0; be L3329; nop
set _3274,%r8
set 2,%r9
call _printf; nop
L3329:
add %r26,2,%r26
L3325:
L3322:
ld [%fp+-272],%r25
set 0x2000000,%r23
or %r25,%r23,%r25
st %r25,[%fp+-272]
ld [%fp+-272],%r25
sll %r25,6,%r25
sra %r25,31,%r25
cmp %r25,1; be L3331; nop
ld [%r24+40],%r25
cmp %r25,%r0; be L3333; nop
set L3335,%r8
call _printf; nop
L3333:
L3331:
add -240,%fp,%r25
sub %r25,%r25,%r25
cmp %r25,%r0; bne L3343; nop
cmp %r25,%r0; bne L3343; nop
cmp %r25,%r0; bne L3343; nop
cmp %r25,%r0; bne L3343; nop
cmp %r25,%r0; bne L3343; nop
cmp %r25,%r0; bne L3343; nop
cmp %r25,%r0; be L3336; nop
L3343:
ld [%r24+44],%r25
cmp %r25,%r0; be L3344; nop
set _3274,%r8
set 4,%r9
call _printf; nop
L3344:
add %r26,4,%r26
L3336:
ba L3346; nop
ld [%r24+44],%r25
cmp %r25,%r0; be L3348; nop
set _3274,%r8
set 8,%r9
call _printf; nop
L3348:
add %r26,8,%r26
L3346:
add -336,%fp,%r25
st %r25,[%fp+-276]
set 2,%r25
stb %r25,[%fp+-336]
ld [%fp+-276],%r25
ldsb [%r25],%r23
add %r23,1,%r23
stb %r23,[%r25]
ldsb [%fp+-336],%r25
cmp %r25,3; be L3352; nop
ld [%r24+44],%r25
cmp %r25,%r0; be L3354; nop
set _3274,%r8
set 16,%r9
call _printf; nop
L3354:
add %r26,16,%r26
L3352:
mov %r26,%r24
L3273:
ret; restore
.seg "data"
_3357:.ascii "s86,er%d\012\000"
_3358:.ascii "s86    \000"
.align 4; _3359:.word 1
.word 3
.word 5
.align 4; _3360:.word _3359+8
.seg "bss"
.align 4; .reserve _3362,40
.seg "data"
.align 4; _3363:.word 0x3f800000
.word 0x40400000
.word 0x40a00000
.word 0x40000000
.word 0x40800000
.word 0x40c00000
.word 0x40400000
.word 0x40a00000
.word 0x40e00000
.word 0x0
.word 0x0
.word 0x0
.align 4; _3364:.word 0x3f800000
.word 0x40400000
.word 0x40a00000
.word 0x40000000
.word 0x40800000
.word 0x40c00000
.word 0x40400000
.word 0x40a00000
.word 0x40e00000
.skip 12
.align 4; _3365:.word 0x3f800000
.word 0x40400000
.word 0x40a00000
.word 0x40000000
.word 0x40800000
.word 0x40c00000
.word 0x40400000
.word 0x40a00000
.word 0x40e00000
.skip 12
.align 4; _3366:.word 0x3f800000
.skip 8
.word 0x40000000
.skip 8
.word 0x40400000
.skip 8
.word 0x40800000
.skip 8
.seg "text"
.global _s86
.align 4
.proc 4
_s86:save %sp,-96,%sp
set _3360,%r19
ld [%r19],%r19
add %r19,-4,%r20
call _one; nop
sll %r8,2,%r19
add %r19,%r20,%r29
set _3358,%r23
add %r24,60,%r22
mov %r0,%r21
L3367:
L3368:
mov %r22,%r19
add %r19,1,%r22
mov %r23,%r18
add %r18,1,%r23
ldsb [%r18],%r18
stb %r18,[%r19]
sll %r18,24,%g1; sra %g1,24,%r19
cmp %r19,%r0; bne L3367; nop
set _3360,%r19
ld [%r19],%r19
ld [%r19],%r19
cmp %r19,5; be L3370; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L3372; nop
set _3357,%r8
set 1,%r9
call _printf; nop
L3372:
add %r21,1,%r21
L3370:
ld [%r20],%r19
cmp %r19,3; be L3374; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L3376; nop
set _3357,%r8
set 2,%r9
call _printf; nop
L3376:
add %r21,2,%r21
L3374:
ld [%r29],%r19
cmp %r19,5; be L3378; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L3380; nop
set _3357,%r8
set 4,%r9
call _printf; nop
L3380:
add %r21,4,%r21
L3378:
mov %r0,%r25
mov %r0,%r28
L3382:
sll %r28,2,%r19
set _3362,%r18
ld [%r19+%r18],%r19
cmp %r19,%r0; be L3386; nop
set 1,%r25
L3386:
L3383:
add %r28,1,%r28
cmp %r28,10; bl L3382; nop
cmp %r25,%r0; be L3388; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L3390; nop
set _3357,%r8
set 8,%r9
call _printf; nop
L3390:
add %r21,8,%r21
L3388:
mov %r0,%r25
mov %r0,%r27
L3392:
mov %r0,%r28
L3396:
set 3,%r19
mov %r27,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
add %r19,%r28,%r26
set 12,%r19
mov %r27,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
sll %r28,2,%r18
set _3364,%r17
add %r19,%r17,%r17
ld [%r18+%r17],%f31
set _3365,%r17
add %r19,%r17,%r19
ld [%r18+%r19],%f30
fcmpes %f31,%f30; nop; fbne L3402; nop
sll %r26,2,%r19
set _3363,%r18
ld [%r19+%r18],%f30
fcmpes %f31,%f30; nop; fbue L3400; nop
L3402:
set 1,%r25
L3400:
L3397:
add %r28,1,%r28
cmp %r28,3; bl L3396; nop
L3393:
add %r27,1,%r27
cmp %r27,4; bl L3392; nop
cmp %r25,%r0; be L3403; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L3405; nop
set _3357,%r8
set 16,%r9
call _printf; nop
L3405:
add %r21,16,%r21
L3403:
mov %r0,%r25
mov %r0,%r28
L3407:
set 12,%r19
mov %r28,%o1; call .mul,2; mov %r19,%o0; mov %o0,%r19
set _3366,%r18
ld [%r19+%r18],%f31
add %r28,1,%r19
st %r19,[%sp+64]; ld [%sp+64],%f28; fitod %f28,%f28
fdtos %f28,%f30
fcmpes %f31,%f30; nop; fbue L3411; nop
set 1,%r25
L3411:
L3408:
add %r28,1,%r28
cmp %r28,4; bl L3407; nop
cmp %r25,%r0; be L3413; nop
ld [%r24+44],%r19
cmp %r19,%r0; be L3415; nop
set _3357,%r8
set 32,%r9
call _printf; nop
L3415:
add %r21,32,%r21
L3413:
mov %r21,%r24
L3356:
ret; restore
.global _one
.align 4
.proc 4
_one:
set 1,%r8
L3417:
retl; nop
.seg "data"
_3419:.ascii "s88,er%d\012\000"
_3420:.ascii "s88    \000"
.seg "text"
.global _s88
.align 4
.proc 4
_s88:save %sp,-112,%sp
set _3420,%r29
add %r24,60,%r28
mov %r0,%r27
L3422:
L3423:
mov %r28,%r25
add %r25,1,%r28
mov %r29,%r23
add %r23,1,%r29
ldsb [%r23],%r23
stb %r23,[%r25]
sll %r23,24,%g1; sra %g1,24,%r25
cmp %r25,%r0; bne L3422; nop
ba L3425; nop
ld [%r24+44],%r25
cmp %r25,%r0; be L3427; nop
set _3419,%r8
set 1,%r9
call _printf; nop
L3427:
add %r27,1,%r27
L3425:
add -20,%fp,%r25
set _metricp,%r23
st %r25,[%r23]
set 2,%r25
st %r25,[%fp+-20]
set 3,%r25
ld [%r23],%r23
st %r25,[%r23]
ld [%fp+-20],%r25
cmp %r25,3; be L3429; nop
ld [%r24+44],%r25
cmp %r25,%r0; be L3431; nop
set _3419,%r8
set 2,%r9
call _printf; nop
L3431:
add %r27,2,%r27
L3429:
set L419,%r25
ld [%r25],%f30; ld [%r25+4],%f31
st %f30,[%fp+-16]; st %f31,[%fp+-16+4]
st %f30,[%fp+-8]; st %f31,[%fp+-8+4]
add -16,%fp,%r26
set L418,%r25
ld [%r25],%f30; ld [%r25+4],%f31
st %f30,[%r26]; st %f31,[%r26+4]
set L418,%r25
ld [%r25],%f30; ld [%r25+4],%f31
st %f30,[%r26+8]; st %f31,[%r26+8+4]
ld [%fp+-16],%f30; ld [%fp+-16+4],%f31
ld [%fp+-8],%f28; ld [%fp+-8+4],%f29
faddd %f30,%f28,%f30
set L416,%r25
ld [%r25],%f28; ld [%r25+4],%f29
fcmped %f30,%f28; nop; fbue L3434; nop
ld [%r24+44],%r25
cmp %r25,%r0; be L3437; nop
set _3419,%r8
set 4,%r9
call _printf; nop
L3437:
add %r27,4,%r27
L3434:
mov %r27,%r24
L3418:
ret; restore
.seg "data"
_3440:.ascii "s9,er%d\012\000"
_3441:.ascii "s9     \000"
.seg "text"
.global _s9
.align 4
.proc 4
_s9:save %sp,-96,%sp
set _3441,%r27
add %r24,60,%r26
mov %r0,%r25
L3442:
L3443:
mov %r26,%r23
add %r23,1,%r26
mov %r27,%r22
add %r22,1,%r27
ldsb [%r22],%r22
stb %r22,[%r23]
sll %r22,24,%g1; sra %g1,24,%r23
cmp %r23,%r0; bne L3442; nop
mov %r0,%r28
mov %r0,%r29
L3445:
set 2,%r21
mov %r21,%r23
mov %r21,%r22
set 3,%r19
mov %r19,%r21
mov %r19,%r20
cmp %r20,3; bne L3451; nop
cmp %r21,3; be L3449; nop
L3451:
set 1,%r28
L3449:
cmp %r22,2; bne L3454; nop
cmp %r23,2; be L3452; nop
L3454:
set 1,%r28
L3452:
L3446:
add %r29,1,%r29
cmp %r29,2; bl L3445; nop
cmp %r28,%r0; be L3459; nop
ld [%r24+44],%r23
cmp %r23,%r0; be L3457; nop
set _3440,%r8
set 1,%r9
call _printf; nop
L3457:
add %r25,1,%r25
ba L3459; nop
ld [%r24+44],%r23
cmp %r23,%r0; be L3460; nop
set _3440,%r8
set 2,%r9
call _printf; nop
L3460:
add %r25,2,%r25
L3459:
mov %r25,%r24
L3439:
ret; restore
.global _setev
.align 4
.proc 4
_setev:
set 1066,%r13
set _extvar,%r12
st %r13,[%r12]
L3462:
retl; nop
.seg "bss"
.global _rfs
.common _rfs,8
.global _crc
.align 4; .common _crc,4
.global _rrc
.align 4; .common _rrc,4
.global _flgl
.align 4; .common _flgl,4
.global _flgd
.align 4; .common _flgd,4
.global _flgm
.align 4; .common _flgm,4
.global _flgs
.align 4; .common _flgs,4
.global _dprec
.align 4; .common _dprec,4
.global _fprec
.align 4; .common _fprec,4
.global _dbits
.align 4; .common _dbits,4
.global _fbits
.align 4; .common _fbits,4
.global _ubits
.align 4; .common _ubits,4
.global _lbits
.align 4; .common _lbits,4
.global _metricp
.align 4; .common _metricp,4
.global _extvar
.align 4; .common _extvar,4
.seg "text"
L3335:.ascii "Be especially careful with 1-bit fields!\012\000"
L3328:.ascii "Sign extension in fields\012\000"
L3321:.ascii "%s%s%d\012\000"
.align 4; L3241:.word 0x43080000
L3088:.ascii "pointer\000"
L2538:.ascii "Increasing array elements assigned to decreasing locations\012\000"
.align 4; L2416:.word 0x41d00000
.align 8; L2415:.word 0x403a0000,0x0
.align 8; L1713:.word 0x40040000,0x0
.align 8; L1529:.word 0x40240000,0x0
.align 4; L1497:.word 0x41200000
.align 8; L1313:.word 0x40080000,0x0
.align 4; L1281:.word 0x40400000
.align 8; L1097:.word 0x401c0000,0x0
.align 4; L1065:.word 0x40e00000
.align 8; L879:.word 0x40140000,0x0
.align 4; L847:.word 0x40a00000
.align 8; L835:.word 0x41e00000,0x0
L660:.ascii "queep\000"
L646:.byte 10,0
L645:.byte 37,100,0
L640:.ascii "   key=\000"
.align 8; L600:.word 0x41f00000,0x0
.align 8; L537:.word 0x40ce8480,0x0
.align 8; L536:.word 0x405f4000,0x0
.align 4; L535:.word 0x42fa0000
.align 4; L530:.word 0x40000000
L431:.ascii "double\000"
L430:.ascii "float\000"
L429:.ascii "unsigned\000"
L428:.ascii "long\000"
L427:.ascii "short\000"
L426:.ascii "int\000"
L425:.ascii "char\000"
.align 8; L419:.word 0x0,0x0
.align 8; L418:.word 0x3ff00000,0x0
.align 8; L417:.word 0x40100000,0x0
.align 8; L416:.word 0x40000000,0x0
.align 4; L412:.word 0x0
.align 4; L411:.word 0x3f800000
L390:.ascii "queep!\000"
L379:.byte 10,9,8,13,12,92,39,0
L375:.byte 46,34,46,0
L362:.ascii "...\000"
.align 8; L333:.word 0x40938800,0x0
L199:.ascii "   different results when assigned to longs.\012\000"
L198:.ascii "Decimal and octal/hex constants sometimes give\012\000"
L27:.ascii "\012Failed.\012\000"
L26:.ascii "\012No errors detected.\012\000"
L20:.ascii "Section %s returned %d.\012\000"
