.data
.align 2; L2:.long _s22
.long _s241
.long _s243
.long _s244
.long _s25
.long _s26
.long _s4
.long _s61
.long _s626
.long _s71
.long _s72
.long _s757
.long _s7813
.long _s714
.long _s715
.long _s81
.long _s84
.long _s85
.long _s86
.long _s88
.long _s9
.bss
.lcomm L3,68
.lcomm L4,4
.text
.globl _main
.align 2
_main:link a6,#-24
moveml #0x490,a6@(-12)
movl #1,L3+36:l
movl #1,L3+40:l
movl #1,L3+44:l
movl #1,L3+48:l
movl #L3,L4:l
clrl d7
jra L12
L9:
movl L4:l,sp@
movl @(L2:l,d7:l:4),a2
jbsr a2@
movl d0,L3+52:l
movl L3+56:l,d4
addl L3+52:l,d4
movl d4,L3+56:l
tstl L3+36:l; jeq L17
movl #L20,sp@
movl #L3+60,sp@(4)
movl L3+52:l,sp@(8)
jbsr _printf:l
L17:
L10:
addql #1,d7
L12:
cmpl #21,d7; jcs L9
tstl L3+56:l; jne L23
movl #L26,sp@
jbsr _printf:l
jra L24
L23:
movl #L27,sp@
jbsr _printf:l
L24:
clrl d0
L1:
moveml a6@(-12),#0x490
unlk a6
rts
.data
L29:.ascii "s22,er%d\012\000"
L30:.ascii "s22    \000"
.text
.globl _s22
.align 2
_s22:link a6,#-48
moveml #0x34d0,a6@(-40)
clrl d7
lea L30:l,a5
movl a6@(8),a2
lea a2@(60),a4
L31:
L32:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L31
movl #1,d6
movl #2,a6@(-8)
movl #3,a6@(-12)
movl #4,a6@(-4)
movl d6,d4; addl a6@(-8),d4
addl a6@(-12),d4
addl a6@(-4),d4
cmpl #10,d4; jeq L34
addql #1,d7
movl a6@(8),a2
tstl a2@(44); jeq L36
movl #L29,sp@
movl #1,sp@(4)
jbsr _printf:l
L36:
L34:
movl #2,a6@(-16)
movl a6@(-16),d4
cmpl d6,d4; jne L38
addql #4,d7
movl a6@(8),a2
tstl a2@(44); jeq L40
movl #L29,sp@
movl #4,sp@(4)
jbsr _printf:l
L40:
L38:
movl d7,d0
L28:
moveml a6@(-40),#0x34d0
unlk a6
rts
.data
L43:.ascii "s241,er%d\012\000"
L44:.ascii "s241   \000"
.align 2; L45:.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 6
.long 0
.long 8
.long 0
.long 12
.long 0
.long 16
.long 0
.long 18
.long 0
.long 20
.long 0
.long 24
.long 0
.long 28
.long 0
.long 30
.long 0
.long 32
.long 0
.long 36
.text
.globl _s241
.align 2
_s241:link a6,#-516
moveml #0x3cdc,a6@(-508)
movl a6@(8),a5
clrl d6
clrl a6@(-472)
lea L44:l,a4
lea a5@(60),a3
L46:
L47:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L46
jra L49
addql #1,d6
tstl a5@(44); jeq L51
movl #L43,sp@
movl #1,sp@(4)
jbsr _printf:l
L51:
L49:
jra L53
addql #2,d6
tstl a5@(44); jeq L55
movl #L43,sp@
movl #2,sp@(4)
jbsr _printf:l
L55:
L53:
jra L57
addql #4,d6
tstl a5@(44); jeq L59
movl #L43,sp@
movl #4,sp@(4)
jbsr _printf:l
L59:
L57:
jra L61
addql #8,d6
tstl a5@(44); jeq L63
movl #L43,sp@
movl #8,sp@(4)
jbsr _printf:l
L63:
L61:
clrl d7
L65:
movl d7,@(L45:l,d7:l:4)
L66:
addql #1,d7
cmpl #17,d7; jlt L65
movl #18,d7
L69:
lea @(L45:l,d7:l:4),a2
movl a2@,sp@
jbsr _pow2:l
movl d0,a2@
movl @(L45:l,d7:l:4),d4
subql #1,d4
movl d4,@(L45-4:l,d7:l:4)
addql #2,d7
L70:
cmpl #39,d7; jlt L69
clrl a6@(-156)
clrl a6@(-312)
clrl a6@(-468)
movl #1,a6@(-152)
movl #1,a6@(-308)
movl #1,a6@(-464)
movl #2,a6@(-148)
movl #2,a6@(-304)
movl #2,a6@(-460)
movl #3,a6@(-144)
movl #3,a6@(-300)
movl #3,a6@(-456)
movl #4,a6@(-140)
movl #4,a6@(-296)
movl #4,a6@(-452)
movl #5,a6@(-136)
movl #5,a6@(-292)
movl #5,a6@(-448)
movl #6,a6@(-132)
movl #6,a6@(-288)
movl #6,a6@(-444)
movl #7,a6@(-128)
movl #7,a6@(-284)
movl #7,a6@(-440)
movl #8,a6@(-124)
movl #8,a6@(-280)
movl #8,a6@(-436)
movl #9,a6@(-120)
movl #9,a6@(-276)
movl #9,a6@(-432)
movl #10,a6@(-116)
movl #10,a6@(-272)
movl #10,a6@(-428)
movl #11,a6@(-112)
movl #11,a6@(-268)
movl #11,a6@(-424)
movl #12,a6@(-108)
movl #12,a6@(-264)
movl #12,a6@(-420)
movl #13,a6@(-104)
movl #13,a6@(-260)
movl #13,a6@(-416)
movl #14,a6@(-100)
movl #14,a6@(-256)
movl #14,a6@(-412)
movl #15,a6@(-96)
movl #15,a6@(-252)
movl #15,a6@(-408)
movl #16,a6@(-92)
movl #16,a6@(-248)
movl #16,a6@(-404)
movl #63,a6@(-88)
movl #63,a6@(-244)
movl #63,a6@(-400)
movl #64,a6@(-84)
movl #64,a6@(-240)
movl #64,a6@(-396)
movl #255,a6@(-80)
movl #255,a6@(-236)
movl #255,a6@(-392)
movl #256,a6@(-76)
movl #256,a6@(-232)
movl #256,a6@(-388)
movl #4095,a6@(-72)
movl #4095,a6@(-228)
movl #4095,a6@(-384)
movl #4096,a6@(-68)
movl #4096,a6@(-224)
movl #4096,a6@(-380)
movl #65535,a6@(-64)
movl #65535,a6@(-220)
movl #65535,a6@(-376)
movl #65536,a6@(-60)
movl #65536,a6@(-216)
movl #65536,a6@(-372)
movl #262143,a6@(-56)
movl #262143,a6@(-212)
movl #262143,a6@(-368)
movl #262144,a6@(-52)
movl #262144,a6@(-208)
movl #262144,a6@(-364)
movl #1048575,a6@(-48)
movl #1048575,a6@(-204)
movl #1048575,a6@(-360)
movl #1048576,a6@(-44)
movl #1048576,a6@(-200)
movl #1048576,a6@(-356)
movl #16777215,a6@(-40)
movl #16777215,a6@(-196)
movl #16777215,a6@(-352)
movl #16777216,a6@(-36)
movl #16777216,a6@(-192)
movl #16777216,a6@(-348)
movl #268435455,a6@(-32)
movl #268435455,a6@(-188)
movl #268435455,a6@(-344)
movl #268435456,a6@(-28)
movl #268435456,a6@(-184)
movl #268435456,a6@(-340)
movl #1073741823,a6@(-24)
movl #1073741823,a6@(-180)
movl #1073741823,a6@(-336)
movl #1073741824,a6@(-20)
movl #1073741824,a6@(-176)
movl #1073741824,a6@(-332)
movl #0xffffffff,a6@(-16)
movl #0xffffffff,a6@(-172)
movl #0xffffffff,a6@(-328)
movl #2147483647,a6@(-12)
movl #2147483647,a6@(-168)
movl #2147483647,a6@(-324)
movl #2147483647,a6@(-8)
movl #2147483647,a6@(-164)
movl #2147483647,a6@(-320)
movl #2147483647,a6@(-4)
movl #2147483647,a6@(-160)
movl #2147483647,a6@(-316)
clrl d7
L188:
movl @(L45:l,d7:l:4),d4
movl d7,d3; asll #2,d3
movl a6@(-156,d3:l:1),d2
cmpl d2,d4; jne L195
movl a6@(-312,d3:l:1),d4
cmpl d4,d2; jne L195
cmpl a6@(-468,d3:l:1),d4; jeq L192
L195:
tstl a5@(40); jeq L196
movl #L198,sp@
jbsr _printf:l
movl #L199,sp@
jbsr _printf:l
L196:
L192:
L189:
addql #1,d7
cmpl #39,d7; jlt L188
tstl a6@(-472); jeq L200
movl #16,d6
L200:
movl d6,d0
L42:
moveml a6@(-508),#0x3cdc
unlk a6
rts
.globl _pow2
.align 2
_pow2:link a6,#-12
moveml #0x98,a6@(-12)
movl a6@(8),d7
movl #1,d1
jra L204
L203:
movl d1,d4; asll #1,d4
movl d4,d1
L204:
movl d7,d4
movl d4,d3; subql #1,d3
movl d3,d7
tstl d4; jne L203
movl d1,d0
L202:
moveml a6@(-12),#0x98
unlk a6
rts
.data
L207:.ascii "s243,er%d\012\000"
L208:.ascii "s243   \000"
.text
.globl _s243
.align 2
_s243:link a6,#-284
moveml #0x3490,a6@(-276)
clrl d7
lea L208:l,a5
movl a6@(8),a2
lea a2@(60),a4
L209:
L210:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L209
lea a6@(-256),a2
movl a2,sp@
jbsr _zerofill:l
movb #1,a6@(-159)
movb #1,a6@(-191)
movb #1,a6@(-130)
movb #1,a6@(-208)
movb #1,a6@(-158)
movb #1,a6@(-190)
movb #1,a6@(-223)
movb #1,a6@(-207)
movb #1,a6@(-157)
movb #1,a6@(-189)
movb #1,a6@(-222)
movb #1,a6@(-206)
movb #1,a6@(-156)
movb #1,a6@(-188)
movb #1,a6@(-221)
movb #1,a6@(-205)
movb #1,a6@(-155)
movb #1,a6@(-187)
movb #1,a6@(-219)
movb #1,a6@(-204)
movb #1,a6@(-154)
movb #1,a6@(-186)
movb #1,a6@(-218)
movb #1,a6@(-203)
movb #1,a6@(-153)
movb #1,a6@(-185)
movb #1,a6@(-216)
movb #1,a6@(-202)
movb #1,a6@(-152)
movb #1,a6@(-184)
movb #1,a6@(-215)
movb #1,a6@(-201)
movb #1,a6@(-151)
movb #1,a6@(-183)
movb #1,a6@(-161)
movb #1,a6@(-200)
movb #1,a6@(-150)
movb #1,a6@(-182)
movb #1,a6@(-195)
movb #1,a6@(-199)
movb #1,a6@(-149)
movb #1,a6@(-181)
movb #1,a6@(-211)
movb #1,a6@(-148)
movb #1,a6@(-180)
movb #1,a6@(-162)
movb #1,a6@(-147)
movb #1,a6@(-179)
movb #1,a6@(-132)
movb #1,a6@(-246)
movb #1,a6@(-146)
movb #1,a6@(-178)
movb #1,a6@(-247)
movb #1,a6@(-145)
movb #1,a6@(-177)
movb #1,a6@(-133)
movb #1,a6@(-248)
movb #1,a6@(-144)
movb #1,a6@(-176)
movb #1,a6@(-131)
movb #1,a6@(-243)
movb #1,a6@(-143)
movb #1,a6@(-175)
movb #1,a6@(-165)
movb #1,a6@(-244)
movb #1,a6@(-142)
movb #1,a6@(-174)
movb #1,a6@(-163)
movb #1,a6@(-141)
movb #1,a6@(-173)
movb #1,a6@(-213)
movb #1,a6@(-164)
movb #1,a6@(-140)
movb #1,a6@(-172)
movb #1,a6@(-197)
movb #1,a6@(-217)
movb #1,a6@(-139)
movb #1,a6@(-171)
movb #1,a6@(-214)
movb #1,a6@(-138)
movb #1,a6@(-170)
movb #1,a6@(-198)
movb #1,a6@(-256)
movb #1,a6@(-137)
movb #1,a6@(-169)
movb #1,a6@(-196)
movb #1,a6@(-224)
movb #1,a6@(-136)
movb #1,a6@(-168)
movb #1,a6@(-194)
movb #1,a6@(-135)
movb #1,a6@(-167)
movb #1,a6@(-212)
movb #1,a6@(-134)
movb #1,a6@(-166)
movb #1,a6@(-210)
movb #1,a6@(-193)
movb #1,a6@(-209)
lea a6@(-256),a2
movl a2,sp@
jbsr _sumof:l
cmpl #98,d0; jeq L309
addql #1,d7
movl a6@(8),a2
tstl a2@(44); jeq L311
movl #L207,sp@
movl #1,sp@(4)
jbsr _printf:l
L311:
L309:
jra L313
addql #8,d7
movl a6@(8),a2
tstl a2@(44); jeq L315
movl #L207,sp@
movl #8,sp@(4)
jbsr _printf:l
L315:
L313:
movl d7,d0
L206:
moveml a6@(-276),#0x3490
unlk a6
rts
.globl _zerofill
.align 2
_zerofill:link a6,#-4
moveml #0x400,a6@(-4)
movl a6@(8),a1
clrl d1
L318:
movl a1,a2
lea a2@(1),a1
clrb a2@
L319:
addql #1,d1
cmpl #256,d1; jlt L318
L317:
moveml a6@(-4),#0x400
unlk a6
rts
.globl _sumof
.align 2
_sumof:link a6,#-12
moveml #0x490,a6@(-12)
movl a6@(8),a1
clrl d7
clrl d1
L323:
movl a1,a2
lea a2@(1),a1
movb a2@,d4
extbl d4
addl d4,d7
L324:
addql #1,d1
cmpl #256,d1; jlt L323
movl d7,d0
L322:
moveml a6@(-12),#0x490
unlk a6
rts
.data
L328:.ascii "s244,er%d\012\000"
L329:.ascii "s244   \000"
.text
.globl _s244
.align 2
_s244:link a6,#-112
fmovemx #0x10,a6@(-76)
moveml #0x34f0,a6@(-104)
lea L329:l,a5
movl a6@(8),a2
lea a2@(60),a4
L330:
L331:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L330
clrl d5
clrl d6
fmoved L333:l,fp3
fmoved fp3,a6@(-64)
fmoved fp3,a6@(-56)
fmoved L333:l,fp3
fmoved fp3,a6@(-48)
fmoved L333:l,fp3
fmoved fp3,a6@(-40)
fmoved L333:l,fp3
fmoved fp3,a6@(-32)
fmoved L333:l,fp3
fmoved fp3,a6@(-24)
fmoved L333:l,fp3
fmoved fp3,a6@(-16)
fmoved L333:l,fp3
fmoved fp3,a6@(-8)
clrl d6
clrl d7
L341:
movl d7,d4; asll #3,d4
fmoved a6@(-64,d4:l:1),fp3
fcmpd a6@(-56,d4:l:1),fp3; fjeq L345
movl #1,d6
L345:
L342:
addql #1,d7
cmpl #7,d7; jlt L341
tstl d6; jeq L348
movl a6@(8),a2
tstl a2@(44); jeq L350
movl #L328,sp@
movl #1,sp@(4)
jbsr _printf:l
L350:
addql #1,d5
L348:
jra L352
movl a6@(8),a2
tstl a2@(44); jeq L354
movl #L328,sp@
movl #2,sp@(4)
jbsr _printf:l
L354:
addql #2,d5
L352:
movl d5,d0
L327:
moveml a6@(-104),#0x34f0
fmovemx a6@(-76),#0x10
unlk a6
rts
.data
L357:.ascii "s25,er%d\012\000"
L358:.ascii "s25    \000"
.text
.globl _s25
.align 2
_s25:link a6,#-52
moveml #0x3cf8,a6@(-44)
movl a6@(8),a5
movl #L358,a6@(-4)
lea a5@(60),a2
movl a2,a6@(-8)
L359:
L360:
movl a6@(-8),a2
lea a2@(1),a1
movl a1,a6@(-8)
movl a6@(-4),a1
lea a1@(1),a0
movl a0,a6@(-4)
movb a1@,d4
movb d4,a2@
tstb d4; jne L359
clrl d6
lea L362:l,a4
movb a4@,d4
movb a4@(1),d3
cmpb d3,d4; jne L366
movb a4@(2),d4
cmpb d4,d3; jne L366
cmpb #46,d4; jeq L363
L366:
addql #1,d6
tstl a5@(44); jeq L367
movl #L357,sp@
movl #1,sp@(4)
jbsr _printf:l
L367:
L363:
tstb a4@(3); jeq L369
addql #4,d6
tstl a5@(44); jeq L371
movl #L357,sp@
movl #4,sp@(4)
jbsr _printf:l
L371:
L369:
cmpb #34,L375+1:l; jeq L373
addql #8,d6
tstl a5@(44); jeq L377
movl #L357,sp@
movl #8,sp@(4)
jbsr _printf:l
L377:
L373:
lea L379:l,a4
cmpb #10,a4@; jne L387
cmpb #9,a4@(1); jne L387
cmpb #8,a4@(2); jne L387
cmpb #13,a4@(3); jne L387
cmpb #12,a4@(4); jne L387
cmpb #92,a4@(5); jne L387
cmpb #39,a4@(6); jeq L380
L387:
addl #16,d6
tstl a5@(44); jeq L388
movl #L357,sp@
movl #16,sp@(4)
jbsr _printf:l
L388:
L380:
movl #L390,a3
lea L390:l,a4
clrl d5
clrl d7
jra L394
L391:
movl d7,a2; addl a4,a2
movb a2@,d4
movl d7,a2; addl a3,a2
cmpb a2@,d4; jeq L395
movl #1,d5
L395:
L392:
addql #1,d7
L394:
cmpl #7,d7; jcs L391
tstl d5; jeq L397
addl #32,d6
tstl a5@(44); jeq L399
movl #L357,sp@
movl #32,sp@(4)
jbsr _printf:l
L399:
L397:
movl d6,d0
L356:
moveml a6@(-44),#0x3cf8
unlk a6
rts
.data
L402:.ascii "s26    \000"
L403:.ascii "%3d bits in %ss.\012\000"
L404:.ascii "%e is the least number that can be added to 1. (%s).\012\000"
.text
.globl _s26
.align 2
_s26:link a6,#-112
fmovemx #0x1f,a6@(-68)
moveml #0x3cd0,a6@(-96)
movl a6@(8),a5
lea L402:l,a4
lea a5@(60),a3
L405:
L406:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L405
clrl a5@
clrb d6
movb #1,d7
jra L409
L408:
movl d7,d4; extbl d4
asll #1,d4
movb d4,d7
addql #1,a5@
L409:
cmpb d7,d6; jne L408
movl a5@,d4
lsll #2,d4
movl d4,a5@(4)
movl a5@,d4
lsll #1,d4
movl d4,a5@(8)
movl a5@,d4
lsll #2,d4
movl d4,a5@(12)
movl a5@,d4
lsll #2,d4
movl d4,a5@(16)
movl a5@,d4
lsll #2,d4
movl d4,a5@(20)
movl a5@,d4
lsll #3,d4
movl d4,a5@(24)
fmoves L411:l,fp3
fmovex fp3,fp5
fmovex fp3,fp7
fmoves L412:l,fp6
jra L414
L413:
fmovex fp5,fp3; faddx fp7,fp3
fmovex fp3,fp6
fmovex fp7,fp3
fdivd L416:l,fp3
fmoves fp3,d0; fmoves d0,fp7
L414:
fcmpx fp5,fp6; fjne L413
fmoved L417:l,fp3
fmulx fp7,fp3
fmoves fp3,a5@(28)
fmoved L418:l,fp3
fmoved fp3,a6@(-8)
fmoves L411:l,fp7
fmoved L419:l,fp4
jra L421
L420:
fmoved a6@(-8),fp3
faddx fp7,fp3
fmovex fp3,fp4
fmovex fp7,fp3; fdivd L416:l,fp3
fmoves fp3,d0; fmoves d0,fp7
L421:
fcmpd a6@(-8),fp4; fjne L420
fmoved L417:l,fp3
fmulx fp7,fp3
fmoves fp3,a5@(32)
tstl a5@(40); jeq L423
movl #L403,sp@
movl a5@,sp@(4)
movl #L425,sp@(8)
jbsr _printf:l
movl #L403,sp@
movl a5@(4),sp@(4)
movl #L426,sp@(8)
jbsr _printf:l
movl #L403,sp@
movl a5@(8),sp@(4)
movl #L427,sp@(8)
jbsr _printf:l
movl #L403,sp@
movl a5@(12),sp@(4)
movl #L428,sp@(8)
jbsr _printf:l
movl #L403,sp@
movl a5@(16),sp@(4)
movl #L429,sp@(8)
jbsr _printf:l
movl #L403,sp@
movl a5@(20),sp@(4)
movl #L430,sp@(8)
jbsr _printf:l
movl #L403,sp@
movl a5@(24),sp@(4)
movl #L431,sp@(8)
jbsr _printf:l
movl #L404,sp@
fmoves a5@(28),fp3
fmoved fp3,sp@(4)
movl #L430,sp@(12)
jbsr _printf:l
movl #L404,sp@
fmoves a5@(32),fp3
fmoved fp3,sp@(4)
movl #L431,sp@(12)
jbsr _printf:l
L423:
clrl d0
L401:
moveml a6@(-96),#0x3cd0
fmovemx a6@(-68),#0x1f
unlk a6
rts
.data
L433:.ascii "s4,er%d\012\000"
L434:.ascii "s4     \000"
.text
.globl _s4
.align 2
_s4:link a6,#-60
moveml #0x3cf0,a6@(-52)
movl a6@(8),a5
clrl a6@(-4)
lea L434:l,a4
lea a5@(60),a3
L435:
L436:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L435
clrl d7
L438:
movl d7,sp@
jbsr _svtest:l
movl d0,a6@(-20)
jbsr _zero:l
movl a6@(-20),d4
cmpl d0,d4; jeq L442
movl #1,a6@(-4)
tstl a5@(44); jeq L444
movl #L433,sp@
movl #1,sp@(4)
jbsr _printf:l
L444:
L442:
L439:
addql #1,d7
cmpl #3,d7; jlt L438
jbsr _setev:l
jbsr _testev:l
tstl d0; jeq L446
addql #2,a6@(-4)
tstl a5@(44); jeq L448
movl #L433,sp@
movl #2,sp@(4)
jbsr _printf:l
L448:
L446:
jra L450
addql #4,a6@(-4)
tstl a5@(44); jeq L452
movl #L433,sp@
movl #4,sp@(4)
jbsr _printf:l
L452:
L450:
clrl d6
subql #1,d6
movl #1,d5
clrl d7
jra L457
L454:
movl d5,d4; andl d6,d4
movl d4,d5
movl d6,d4; lsrl #1,d4
movl d4,d6
L455:
addql #1,d7
L457:
movl a5@,d4
lsll #2,d4
cmpl d4,d7; jcs L454
cmpl #1,d5; jne L460
tstl d6; jeq L458
L460:
addql #8,a6@(-4)
tstl a5@(44); jeq L461
movl #L433,sp@
movl #8,sp@(4)
jbsr _printf:l
L461:
L458:
movl a6@(-4),d0
L432:
moveml a6@(-52),#0x3cf0
unlk a6
rts
.bss
.lcomm L465,4
.text
.globl _svtest
.align 2
_svtest:link a6,#-8
moveml #0x10,a6@(-8)
movl a6@(8),d4
tstl d4; jeq L468
cmpl #1,d4; jeq L469
cmpl #2,d4; jeq L472
jra L466
L468:
movl #1978,L465:l
clrl a6@(-4)
jra L467
L469:
cmpl #1978,L465:l; jeq L470
movl #1,a6@(-4)
jra L467
L470:
movl #1929,L465:l
clrl a6@(-4)
jra L467
L472:
cmpl #1929,L465:l; jeq L473
movl #1,a6@(-4)
jra L467
L473:
clrl a6@(-4)
L466:
L467:
movl a6@(-4),d0
L464:
moveml a6@(-8),#0x10
unlk a6
rts
.bss
.lcomm L476,4
.text
.globl _zero
.align 2
_zero:link a6,#-4
movl #2,L476:l
clrl a6@(-4)
movl a6@(-4),d0
L475:
unlk a6
rts
.globl _testev
.align 2
_testev:link a6,#-0
cmpl #1066,_extvar:l; jeq L478
movl #1,d0
jra L477
L478:
clrl d0
L477:
unlk a6
rts
.data
L481:.ascii "s61,er%d\012\000"
L482:.ascii "s61    \000"
L483:.ascii "ABCDEFGHIJKLMNOPQRSTUVWXYZ\000"
L484:.ascii "abcdefghijklmnopqrstuvwxyz\000"
L485:.ascii "0123456789\000"
L486:.ascii "~!\"#%&()_=-^|{}[]+;*:<>,.?/\000"
L487:.byte 10,9,8,13,12,92,39,0
L488:.byte 32,0,0
.text
.globl _s61
.align 2
_s61:link a6,#-80
moveml #0x34f0,a6@(-72)
lea L482:l,a5
movl a6@(8),a2
lea a2@(60),a4
clrl a6@(-28)
L489:
L490:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L489
movw #-19,a6@(-40)
movw a6@(-40),d4
extl d4
movl d4,a6@(-44)
cmpl #-19,a6@(-44); jeq L492
addql #1,a6@(-28)
movl a6@(8),a2
tstl a2@(44); jeq L494
movl #L481,sp@
movl #1,sp@(4)
jbsr _printf:l
L494:
L492:
movl #L483,a6@(-24)
movl #L484,a6@(-20)
movl #L485,a6@(-16)
movl #L486,a6@(-12)
movl #L487,a6@(-8)
movl #L488,a6@(-4)
clrl d6
clrl d7
jra L506
L505:
movl d7,d4; asll #2,d4
lea a6@(-24,d4:l:1),a2
movl a2@,a1
lea a1@(1),a0
movl a0,a2@
tstb a1@; jge L508
movl #1,d6
L508:
L506:
movl d7,d4; asll #2,d4
movl a6@(-24,d4:l:1),a2
tstb a2@; jne L505
L502:
addql #1,d7
cmpl #6,d7; jlt L506
tstl d6; jeq L510
addql #2,a6@(-28)
movl a6@(8),a2
tstl a2@(44); jeq L512
movl #L481,sp@
movl #2,sp@(4)
jbsr _printf:l
L512:
L510:
movl #1048579,d5
movw d5,a6@(-32)
movb d5,a6@(-36)
movw a6@(-32),d4
extl d4
cmpl d5,d4; jeq L517
cmpl #3,d4; jne L516
L517:
movb a6@(-36),d4
extbl d4
cmpl d5,d4; jeq L514
cmpl #3,d4; jeq L514
L516:
addql #8,a6@(-28)
movl a6@(8),a2
tstl a2@(44); jeq L518
movl #L481,sp@
movl #8,sp@(4)
jbsr _printf:l
L518:
L514:
movl a6@(-28),d0
L480:
moveml a6@(-72),#0x34f0
unlk a6
rts
.data
L521:.ascii "s626,er%d\012\000"
L522:.ascii "s626   \000"
.text
.globl _s626
.align 2
_s626:link a6,#-204
fmovemx #0x3f,a6@(-160)
moveml #0x3cf8,a6@(-196)
movl a6@(8),a5
lea L522:l,a4
lea a5@(60),a3
clrl a6@(-64)
L523:
L524:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L523
fmoves L411:l,fp7
movl #1,a6@(-32)
movl #1,d6
clrl d7
jra L529
L526:
fmoves L530:l,fp3
fmulx fp7,fp3
fmovex fp3,fp7
movl d6,d4; asll #1,d4
movl d4,d4
orl a6@(-32),d4
movl d4,d6
L527:
addql #1,d7
L529:
movl a5@(12),d4
subql #2,d4
cmpl d4,d7; jlt L526
fmovel d6,fp3
fmoves fp3,a6@(-72)
fmovex fp7,fp3; fsubs a6@(-72),fp3
fdivx fp7,fp3
fmovex fp3,fp7
fmoved L416:l,fp3
fmuls a5@(28),fp3
fcmpx fp3,fp7; fjle L531
addql #2,a6@(-64)
tstl a5@(44); jeq L533
movl #L521,sp@
movl #2,sp@(4)
jbsr _printf:l
L533:
L531:
movb #125,a6@(-40)
movw #125,a6@(-44)
movl #125,a6@(-48)
movl #15625,a6@(-56)
movl #125,a6@(-52)
movl #15625,a6@(-60)
movl #125,a6@(-36)
movl #15625,a6@(-68)
fmoves L535:l,fp5
fmoved L536:l,fp4
fmoved L537:l,fp6
clrl d7
L538:
clrb a6@(-28,d7:l:1)
L539:
addql #1,d7
cmpl #28,d7; jlt L538
movb a6@(-40),d4
extbl d4
movl d4,d3; mulsl d4,d3
cmpl a6@(-56),d3; jeq L542
movb #1,a6@(-28)
L542:
movw a6@(-44),d4
extl d4
movb a6@(-40),d3
extbl d3
mulsl d3,d4
cmpl a6@(-56),d4; jeq L544
movb #1,a6@(-27)
L544:
movw a6@(-44),d4
extl d4
movl d4,d3; mulsl d4,d3
cmpl a6@(-56),d3; jeq L547
movb #1,a6@(-26)
L547:
movl a6@(-48),d4
movb a6@(-40),d3
extbl d3
mulsl d3,d4
cmpl a6@(-56),d4; jeq L550
movb #1,a6@(-25)
L550:
movl a6@(-48),d4
movw a6@(-44),d3
extl d3
mulsl d3,d4
cmpl a6@(-56),d4; jeq L553
movb #1,a6@(-24)
L553:
movl a6@(-48),d4
movl d4,d3; mulsl d4,d3
cmpl a6@(-56),d3; jeq L556
movb #1,a6@(-23)
L556:
movl a6@(-52),d4
movb a6@(-40),d3
extbl d3
movl d3,d3
mulul d3,d4
cmpl a6@(-60),d4; jeq L559
movb #1,a6@(-22)
L559:
movl a6@(-52),d4
movw a6@(-44),d3
extl d3
movl d3,d3
mulul d3,d4
cmpl a6@(-60),d4; jeq L562
movb #1,a6@(-21)
L562:
movl a6@(-52),d4
movl a6@(-48),d3
mulul d3,d4
cmpl a6@(-60),d4; jeq L565
movb #1,a6@(-20)
L565:
movl a6@(-52),d4
movl d4,d3; mulul d4,d3
cmpl a6@(-60),d3; jeq L568
movb #1,a6@(-19)
L568:
movl a6@(-36),d4
movb a6@(-40),d3
extbl d3
mulsl d3,d4
cmpl a6@(-68),d4; jeq L571
movb #1,a6@(-18)
L571:
movl a6@(-36),d4
movw a6@(-44),d3
extl d3
mulsl d3,d4
cmpl a6@(-68),d4; jeq L574
movb #1,a6@(-17)
L574:
movl a6@(-36),d4
movl a6@(-48),d3
mulsl d3,d4
cmpl a6@(-68),d4; jeq L577
movb #1,a6@(-16)
L577:
movl a6@(-36),d4
movl a6@(-52),d3
mulul d3,d4
cmpl a6@(-60),d4; jeq L580
movb #1,a6@(-15)
L580:
movl a6@(-36),d4
movl d4,d3; mulsl d4,d3
cmpl a6@(-68),d3; jeq L583
movb #1,a6@(-14)
L583:
movb a6@(-40),d4
extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp5,fp2; fmulx fp3,fp2
fmovex fp2,fp3
fcmpx fp6,fp3; fjeq L586
movb #1,a6@(-13)
L586:
movw a6@(-44),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp5,fp2; fmulx fp3,fp2
fmovex fp2,fp3
fcmpx fp6,fp3; fjeq L589
movb #1,a6@(-12)
L589:
movl a6@(-48),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp5,fp2; fmulx fp3,fp2
fmovex fp2,fp3
fcmpx fp6,fp3; fjeq L592
movb #1,a6@(-11)
L592:
movl a6@(-52),d4
tstl d4; jlt L598
fmovel d4,fp3
fmoved fp3,a6@(-80)
jra L599
L598:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-80)
L599:
fmoved a6@(-80),fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp5,fp2; fmulx fp3,fp2
fmovex fp2,fp3
fcmpx fp6,fp3; fjeq L595
movb #1,a6@(-10)
L595:
movl a6@(-36),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp5,fp2; fmulx fp3,fp2
fmovex fp2,fp3
fcmpx fp6,fp3; fjeq L602
movb #1,a6@(-9)
L602:
fmovex fp5,fp3; fmulx fp5,fp3
fmovex fp3,fp3
fcmpx fp6,fp3; fjeq L605
movb #1,a6@(-8)
L605:
movb a6@(-40),d4
extbl d4
fmovex fp4,fp3; fmull d4,fp3
fcmpx fp6,fp3; fjeq L608
movb #1,a6@(-7)
L608:
movw a6@(-44),d4
extl d4
fmovex fp4,fp3; fmull d4,fp3
fcmpx fp6,fp3; fjeq L611
movb #1,a6@(-6)
L611:
movl a6@(-48),d4
fmovex fp4,fp3; fmull d4,fp3
fcmpx fp6,fp3; fjeq L614
movb #1,a6@(-5)
L614:
movl a6@(-52),d4
tstl d4; jlt L620
fmovel d4,fp3
fmoved fp3,a6@(-88)
jra L621
L620:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-88)
L621:
fmovex fp4,fp3; fmuld a6@(-88),fp3
fcmpx fp6,fp3; fjeq L617
movb #1,a6@(-4)
L617:
movl a6@(-36),d4
fmovex fp4,fp3; fmull d4,fp3
fcmpx fp6,fp3; fjeq L623
movb #1,a6@(-3)
L623:
fmovex fp4,fp3; fmulx fp5,fp3
fcmpx fp6,fp3; fjeq L626
movb #1,a6@(-2)
L626:
fmovex fp4,fp3; fmulx fp4,fp3
fcmpx fp6,fp3; fjeq L629
movb #1,a6@(-1)
L629:
clrb d5
clrl d7
L632:
movl d5,d4; extbl d4
movb a6@(-28,d7:l:1),d3
extbl d3
addl d3,d4
movb d4,d5
L633:
addql #1,d7
cmpl #28,d7; jlt L632
tstb d5; jeq L636
addql #4,a6@(-64)
tstl a5@(44); jeq L638
movl #L521,sp@
movl #4,sp@(4)
jbsr _printf:l
movl #L640,sp@
jbsr _printf:l
clrl d7
L641:
movl #L645,sp@
movb a6@(-28,d7:l:1),d4
extbl d4
movl d4,sp@(4)
jbsr _printf:l
L642:
addql #1,d7
cmpl #28,d7; jlt L641
movl #L646,sp@
jbsr _printf:l
L638:
L636:
movl #32768,a6@(-36)
cmpl #0x8000,a6@(-36); jls L647
addql #8,a6@(-64)
tstl a5@(44); jeq L649
movl #L521,sp@
movl #8,sp@(4)
jbsr _printf:l
L649:
L647:
movl a6@(-64),d0
L520:
moveml a6@(-196),#0x3cf8
fmovemx a6@(-160),#0x3f
unlk a6
rts
.data
L652:.ascii "s71,er%d\012\000"
L653:.ascii "s71    \000"
L654:.byte 113
.text
.globl _s71
.align 2
_s71:link a6,#-96
fmovemx #0x10,a6@(-60)
moveml #0x3cd0,a6@(-88)
movl a6@(8),a5
lea L653:l,a4
lea a5@(60),a3
clrl d7
L655:
L656:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L655
movb L660:l,d4
cmpb L654:l,d4; jeq L658
addql #1,d7
tstl a5@(44); jeq L661
movl #L652,sp@
movl #1,sp@(4)
jbsr _printf:l
L661:
L658:
jra L663
addql #2,d7
tstl a5@(44); jeq L665
movl #L652,sp@
movl #2,sp@(4)
jbsr _printf:l
L665:
L663:
movl #1942,a6@(-20)
cmpl #1942,a6@(-20); jne L673
movl a6@(-20),d4
cmpl a6@(-20),d4; jeq L668
L673:
addql #4,d7
tstl a5@(44); jeq L674
movl #L652,sp@
movl #4,sp@(4)
jbsr _printf:l
L674:
L668:
movl #-5,sp@
jbsr _McCarthy:l
cmpl #91,d0; jeq L676
addql #8,d7
tstl a5@(44); jeq L678
movl #L652,sp@
movl #8,sp@(4)
jbsr _printf:l
L678:
L676:
movl #2,d6
movl #3,a6@(-44)
lea a6@(-44),a2
movl a2,a6@(-48)
movl d6,sp@
movl a6@(-48),sp@(4)
jbsr _clobber:l
cmpl #2,d6; jne L682
cmpl #2,a6@(-44); jeq L680
L682:
addl #16,d7
tstl a5@(44); jeq L683
movl #L652,sp@
movl #16,sp@(4)
jbsr _printf:l
L683:
L680:
fmoves a5@(32),fp3
fcmpx fp3,fp3; fjeq L685
addl #32,d7
tstl a5@(44); jeq L687
movl #L652,sp@
movl #32,sp@(4)
jbsr _printf:l
L687:
L685:
movl d7,d0
L651:
moveml a6@(-88),#0x3cd0
fmovemx a6@(-60),#0x10
unlk a6
rts
.globl _McCarthy
.align 2
_McCarthy:link a6,#-8
moveml #0x10,a6@(-4)
cmpl #100,a6@(8); jle L690
movl a6@(8),d4
movl d4,d0; subl #10,d0
jra L689
L690:
movl a6@(8),d4
addl #11,d4
movl d4,sp@
jbsr _McCarthy:l
movl d0,sp@
jbsr _McCarthy:l
L689:
moveml a6@(-4),#0x10
unlk a6
rts
.globl _clobber
.align 2
_clobber:link a6,#-4
moveml #0x400,a6@(-4)
movl #3,a6@(8)
movl a6@(12),a2
movl #2,a2@
L692:
moveml a6@(-4),#0x400
unlk a6
rts
.data
L694:.ascii "Local error %d.\012\000"
L695:.ascii "s714,er%d\012\000"
L696:.ascii "s714   \000"
.text
.globl _s714
.align 2
_s714:link a6,#-360
fmovemx #0xff,a6@(-320)
moveml #0x34f8,a6@(-352)
lea L696:l,a5
movl a6@(8),a2
lea a2@(60),a4
clrl a6@(-40)
clrl d6
movl a2@(48),d7
L697:
L698:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L697
movb #5,d5
movb #2,a6@(-20)
movb a6@(-20),d5
cmpb #2,d5; jeq L700
movl #1,d6
tstl d7; jeq L702
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L702:
L700:
movb #5,d5
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
movb d4,d5
cmpb #2,d5; jeq L704
movl #2,d6
tstl d7; jeq L706
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L706:
L704:
movb #5,d5
movl #2,a6@(-28)
movl a6@(-28),d4
movb d4,d5
cmpb #2,d5; jeq L708
movl #3,d6
tstl d7; jeq L710
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L710:
L708:
movb #5,d5
movl #2,a6@(-32)
movl a6@(-32),d4
movb d4,d5
cmpb #2,d5; jeq L712
movl #4,d6
tstl d7; jeq L714
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L714:
L712:
movb #5,d5
movl #2,a6@(-36)
movl a6@(-36),d4
movb d4,d5
cmpb #2,d5; jeq L716
movl #5,d6
tstl d7; jeq L718
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L718:
L716:
movb #5,d5
fmoves L530:l,fp5
fintrzx fp5,fp0; fmovel fp0,d4
movb d4,d5
cmpb #2,d5; jeq L720
movl #6,d6
tstl d7; jeq L722
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L722:
L720:
movb #5,d5
fmoved L416:l,fp4
fintrzx fp4,fp0; fmovel fp0,d4
movb d4,d5
cmpb #2,d5; jeq L724
movl #7,d6
tstl d7; jeq L726
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L726:
L724:
movw #5,a6@(-4)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L728
movl #8,d6
tstl d7; jeq L730
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L730:
L728:
movw #5,a6@(-4)
movw #2,a6@(-24)
movw a6@(-24),a6@(-4)
cmpw #2,a6@(-4); jeq L732
movl #9,d6
tstl d7; jeq L734
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L734:
L732:
movw #5,a6@(-4)
movl #2,a6@(-28)
movl a6@(-28),d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L736
movl #10,d6
tstl d7; jeq L738
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L738:
L736:
movw #5,a6@(-4)
movl #2,a6@(-32)
movl a6@(-32),d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L740
movl #11,d6
tstl d7; jeq L742
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L742:
L740:
movw #5,a6@(-4)
movl #2,a6@(-36)
movl a6@(-36),d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L744
movl #12,d6
tstl d7; jeq L746
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L746:
L744:
movw #5,a6@(-4)
fmoves L530:l,fp5
fintrzx fp5,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L748
movl #13,d6
tstl d7; jeq L750
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L750:
L748:
movw #5,a6@(-4)
fmoved L416:l,fp4
fintrzx fp4,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L752
movl #14,d6
tstl d7; jeq L754
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L754:
L752:
movl #5,a6@(-8)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L756
movl #15,d6
tstl d7; jeq L758
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L758:
L756:
movl #5,a6@(-8)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L760
movl #16,d6
tstl d7; jeq L762
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L762:
L760:
movl #5,a6@(-8)
movl #2,a6@(-28)
movl a6@(-28),a6@(-8)
cmpl #2,a6@(-8); jeq L764
movl #17,d6
tstl d7; jeq L766
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L766:
L764:
movl #5,a6@(-8)
movl #2,a6@(-32)
movl a6@(-32),a6@(-8)
cmpl #2,a6@(-8); jeq L768
movl #18,d6
tstl d7; jeq L770
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L770:
L768:
movl #5,a6@(-8)
movl #2,a6@(-36)
movl a6@(-36),a6@(-8)
cmpl #2,a6@(-8); jeq L772
movl #19,d6
tstl d7; jeq L774
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L774:
L772:
movl #5,a6@(-8)
fmoves L530:l,fp5
fintrzx fp5,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L776
movl #20,d6
tstl d7; jeq L778
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L778:
L776:
movl #5,a6@(-8)
fmoved L416:l,fp4
fintrzx fp4,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L780
movl #21,d6
tstl d7; jeq L782
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L782:
L780:
movl #5,a6@(-12)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L784
movl #22,d6
tstl d7; jeq L786
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L786:
L784:
movl #5,a6@(-12)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L788
movl #23,d6
tstl d7; jeq L790
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L790:
L788:
movl #5,a6@(-12)
movl #2,a6@(-28)
movl a6@(-28),a6@(-12)
cmpl #2,a6@(-12); jeq L792
movl #24,d6
tstl d7; jeq L794
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L794:
L792:
movl #5,a6@(-12)
movl #2,a6@(-32)
movl a6@(-32),a6@(-12)
cmpl #2,a6@(-12); jeq L796
movl #25,d6
tstl d7; jeq L798
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L798:
L796:
movl #5,a6@(-12)
movl #2,a6@(-36)
movl a6@(-36),a6@(-12)
cmpl #2,a6@(-12); jeq L800
movl #26,d6
tstl d7; jeq L802
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L802:
L800:
movl #5,a6@(-12)
fmoves L530:l,fp5
fintrzx fp5,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L804
movl #27,d6
tstl d7; jeq L806
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L806:
L804:
movl #5,a6@(-12)
fmoved L416:l,fp4
fintrzx fp4,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L808
movl #28,d6
tstl d7; jeq L810
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L810:
L808:
movl #5,a6@(-16)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
movl d4,a6@(-16)
cmpl #2,a6@(-16); jeq L812
movl #29,d6
tstl d7; jeq L814
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L814:
L812:
movl #5,a6@(-16)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
movl d4,a6@(-16)
cmpl #2,a6@(-16); jeq L816
movl #30,d6
tstl d7; jeq L818
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L818:
L816:
movl #5,a6@(-16)
movl #2,a6@(-28)
movl a6@(-28),a6@(-16)
cmpl #2,a6@(-16); jeq L820
movl #31,d6
tstl d7; jeq L822
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L822:
L820:
movl #5,a6@(-16)
movl #2,a6@(-32)
movl a6@(-32),a6@(-16)
cmpl #2,a6@(-16); jeq L824
movl #32,d6
tstl d7; jeq L826
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L826:
L824:
movl #5,a6@(-16)
movl #2,a6@(-36)
movl a6@(-36),a6@(-16)
cmpl #2,a6@(-16); jeq L828
movl #33,d6
tstl d7; jeq L830
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L830:
L828:
movl #5,a6@(-16)
fmoves L530:l,fp5
fmoved L835:l,fp3
fcmpx fp3,fp5; fjlt L833
fmovex fp5,fp2; fsubx fp3,fp2
fintrzx fp2,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-44)
jra L834
L833:
fintrzx fp5,fp0; fmovel fp0,d4
movl d4,a6@(-44)
L834:
movl a6@(-44),a6@(-16)
cmpl #2,a6@(-16); jeq L836
movl #34,d6
tstl d7; jeq L838
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L838:
L836:
movl #5,a6@(-16)
fmoved L416:l,fp4
fmoved L835:l,fp3
fcmpx fp3,fp4; fjlt L841
fmovex fp4,fp2; fsubx fp3,fp2
fintrzx fp2,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-48)
jra L842
L841:
fintrzx fp4,fp0; fmovel fp0,d4
movl d4,a6@(-48)
L842:
movl a6@(-48),a6@(-16)
cmpl #2,a6@(-16); jeq L843
movl #35,d6
tstl d7; jeq L845
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L845:
L843:
fmoves L847:l,fp7
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp7
fcmps L530:l,fp7; fjeq L848
movl #36,d6
tstl d7; jeq L850
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L850:
L848:
fmoves L847:l,fp7
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp7
fcmps L530:l,fp7; fjeq L852
movl #37,d6
tstl d7; jeq L854
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L854:
L852:
fmoves L847:l,fp7
movl #2,a6@(-28)
movl a6@(-28),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp7
fcmps L530:l,fp7; fjeq L856
movl #38,d6
tstl d7; jeq L858
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L858:
L856:
fmoves L847:l,fp7
movl #2,a6@(-32)
movl a6@(-32),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp7
fcmps L530:l,fp7; fjeq L860
movl #39,d6
tstl d7; jeq L862
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L862:
L860:
fmoves L847:l,fp7
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L865
fmovel d4,fp3
fmoved fp3,a6@(-56)
jra L866
L865:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-56)
L866:
fmoved a6@(-56),fp3
fmoves fp3,d0; fmoves d0,fp7
fcmps L530:l,fp7; fjeq L867
movl #40,d6
tstl d7; jeq L869
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L869:
L867:
fmoves L847:l,fp7
fmoves L530:l,fp3
fmovex fp3,fp5
fmovex fp5,fp7
fcmpx fp3,fp7; fjeq L871
movl #41,d6
tstl d7; jeq L873
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L873:
L871:
fmoves L847:l,fp7
fmoved L416:l,fp4
fmoves fp4,d0; fmoves d0,fp7
fcmps L530:l,fp7; fjeq L875
movl #42,d6
tstl d7; jeq L877
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L877:
L875:
fmoved L879:l,fp6
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovel d4,fp6
fcmpd L416:l,fp6; fjeq L880
movl #43,d6
tstl d7; jeq L882
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L882:
L880:
fmoved L879:l,fp6
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovel d4,fp6
fcmpd L416:l,fp6; fjeq L884
movl #44,d6
tstl d7; jeq L886
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L886:
L884:
fmoved L879:l,fp6
movl #2,a6@(-28)
movl a6@(-28),d4
fmovel d4,fp6
fcmpd L416:l,fp6; fjeq L888
movl #45,d6
tstl d7; jeq L890
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L890:
L888:
fmoved L879:l,fp6
movl #2,a6@(-32)
movl a6@(-32),d4
fmovel d4,fp6
fcmpd L416:l,fp6; fjeq L892
movl #46,d6
tstl d7; jeq L894
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L894:
L892:
fmoved L879:l,fp6
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L897
fmovel d4,fp3
fmoved fp3,a6@(-64)
jra L898
L897:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-64)
L898:
fmoved a6@(-64),fp6
fcmpd L416:l,fp6; fjeq L899
movl #47,d6
tstl d7; jeq L901
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L901:
L899:
fmoved L879:l,fp6
fmoves L530:l,fp5
fmovex fp5,fp6
fcmpd L416:l,fp6; fjeq L903
movl #48,d6
tstl d7; jeq L905
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L905:
L903:
fmoved L879:l,fp6
fmoved L416:l,fp3
fmovex fp3,fp4
fmovex fp4,fp6
fcmpx fp3,fp6; fjeq L907
movl #49,d6
tstl d7; jeq L909
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L909:
L907:
movb #5,d5
movb #2,a6@(-20)
movl d5,d4; extbl d4
movb a6@(-20),d3
extbl d3
addl d3,d4
movb d4,d5
cmpb #7,d5; jeq L911
movl #50,d6
tstl d7; jeq L913
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L913:
L911:
movb #5,d5
movw #2,a6@(-24)
movl d5,d4; extbl d4
movw a6@(-24),d3
extl d3
addl d3,d4
movb d4,d5
cmpb #7,d5; jeq L915
movl #51,d6
tstl d7; jeq L917
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L917:
L915:
movb #5,d5
movl #2,a6@(-28)
movl d5,d4; extbl d4
addl a6@(-28),d4
movb d4,d5
cmpb #7,d5; jeq L919
movl #52,d6
tstl d7; jeq L921
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L921:
L919:
movb #5,d5
movl #2,a6@(-32)
movl d5,d4; extbl d4
addl a6@(-32),d4
movb d4,d5
cmpb #7,d5; jeq L923
movl #53,d6
tstl d7; jeq L925
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L925:
L923:
movb #5,d5
movl #2,a6@(-36)
movl d5,d4; extbl d4
movl d4,d4
addl a6@(-36),d4
movb d4,d5
cmpb #7,d5; jeq L927
movl #54,d6
tstl d7; jeq L929
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L929:
L927:
movb #5,d5
fmoves L530:l,fp5
movl d5,d4; extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
faddx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,d5
cmpb #7,d5; jeq L931
movl #55,d6
tstl d7; jeq L933
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L933:
L931:
movb #5,d5
fmoved L416:l,fp4
movl d5,d4; extbl d4
fmovel d4,fp3
faddx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,d5
cmpb #7,d5; jeq L935
movl #56,d6
tstl d7; jeq L937
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L937:
L935:
movw #5,a6@(-4)
movb #2,a6@(-20)
movw a6@(-4),d4
extl d4
movb a6@(-20),d3
extbl d3
addl d3,d4
movw d4,a6@(-4)
cmpw #7,a6@(-4); jeq L939
movl #57,d6
tstl d7; jeq L941
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L941:
L939:
movw #5,a6@(-4)
movw #2,a6@(-24)
movw a6@(-4),d4
extl d4
movw a6@(-24),d3
extl d3
addl d3,d4
movw d4,a6@(-4)
cmpw #7,a6@(-4); jeq L943
movl #58,d6
tstl d7; jeq L945
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L945:
L943:
movw #5,a6@(-4)
movl #2,a6@(-28)
movw a6@(-4),d4
extl d4
addl a6@(-28),d4
movw d4,a6@(-4)
cmpw #7,a6@(-4); jeq L947
movl #59,d6
tstl d7; jeq L949
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L949:
L947:
movw #5,a6@(-4)
movl #2,a6@(-32)
movw a6@(-4),d4
extl d4
addl a6@(-32),d4
movw d4,a6@(-4)
cmpw #7,a6@(-4); jeq L951
movl #60,d6
tstl d7; jeq L953
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L953:
L951:
movw #5,a6@(-4)
movl #2,a6@(-36)
movw a6@(-4),d4
extl d4
movl d4,d4
addl a6@(-36),d4
movw d4,a6@(-4)
cmpw #7,a6@(-4); jeq L955
movl #61,d6
tstl d7; jeq L957
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L957:
L955:
movw #5,a6@(-4)
fmoves L530:l,fp5
movw a6@(-4),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
faddx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #7,a6@(-4); jeq L959
movl #62,d6
tstl d7; jeq L961
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L961:
L959:
movw #5,a6@(-4)
fmoved L416:l,fp4
movw a6@(-4),d4
extl d4
fmovel d4,fp3
faddx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #7,a6@(-4); jeq L963
movl #63,d6
tstl d7; jeq L965
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L965:
L963:
movl #5,a6@(-8)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
addl d4,a6@(-8)
cmpl #7,a6@(-8); jeq L967
movl #64,d6
tstl d7; jeq L969
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L969:
L967:
movl #5,a6@(-8)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
addl d4,a6@(-8)
cmpl #7,a6@(-8); jeq L971
movl #65,d6
tstl d7; jeq L973
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L973:
L971:
movl #5,a6@(-8)
movl #2,a6@(-28)
movl a6@(-8),d4
addl a6@(-28),d4
movl d4,a6@(-8)
cmpl #7,a6@(-8); jeq L975
movl #66,d6
tstl d7; jeq L977
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L977:
L975:
movl #5,a6@(-8)
movl #2,a6@(-32)
movl a6@(-8),d4
addl a6@(-32),d4
movl d4,a6@(-8)
cmpl #7,a6@(-8); jeq L979
movl #67,d6
tstl d7; jeq L981
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L981:
L979:
movl #5,a6@(-8)
movl #2,a6@(-36)
movl a6@(-8),d4
addl a6@(-36),d4
movl d4,a6@(-8)
cmpl #7,a6@(-8); jeq L983
movl #68,d6
tstl d7; jeq L985
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L985:
L983:
movl #5,a6@(-8)
fmoves L530:l,fp5
movl a6@(-8),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
faddx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #7,a6@(-8); jeq L987
movl #69,d6
tstl d7; jeq L989
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L989:
L987:
movl #5,a6@(-8)
fmoved L416:l,fp4
movl a6@(-8),d4
fmovel d4,fp3
faddx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #7,a6@(-8); jeq L991
movl #70,d6
tstl d7; jeq L993
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L993:
L991:
movl #5,a6@(-12)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
addl d4,a6@(-12)
cmpl #7,a6@(-12); jeq L995
movl #71,d6
tstl d7; jeq L997
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L997:
L995:
movl #5,a6@(-12)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
addl d4,a6@(-12)
cmpl #7,a6@(-12); jeq L999
movl #72,d6
tstl d7; jeq L1001
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1001:
L999:
movl #5,a6@(-12)
movl #2,a6@(-28)
movl a6@(-12),d4
addl a6@(-28),d4
movl d4,a6@(-12)
cmpl #7,a6@(-12); jeq L1003
movl #73,d6
tstl d7; jeq L1005
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1005:
L1003:
movl #5,a6@(-12)
movl #2,a6@(-32)
movl a6@(-12),d4
addl a6@(-32),d4
movl d4,a6@(-12)
cmpl #7,a6@(-12); jeq L1007
movl #74,d6
tstl d7; jeq L1009
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1009:
L1007:
movl #5,a6@(-12)
movl #2,a6@(-36)
movl a6@(-12),d4
addl a6@(-36),d4
movl d4,a6@(-12)
cmpl #7,a6@(-12); jeq L1011
movl #75,d6
tstl d7; jeq L1013
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1013:
L1011:
movl #5,a6@(-12)
fmoves L530:l,fp5
movl a6@(-12),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
faddx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #7,a6@(-12); jeq L1015
movl #76,d6
tstl d7; jeq L1017
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1017:
L1015:
movl #5,a6@(-12)
fmoved L416:l,fp4
movl a6@(-12),d4
fmovel d4,fp3
faddx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #7,a6@(-12); jeq L1019
movl #77,d6
tstl d7; jeq L1021
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1021:
L1019:
movl #5,a6@(-16)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
addl d4,a6@(-16)
cmpl #7,a6@(-16); jeq L1023
movl #78,d6
tstl d7; jeq L1025
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1025:
L1023:
movl #5,a6@(-16)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
addl d4,a6@(-16)
cmpl #7,a6@(-16); jeq L1027
movl #79,d6
tstl d7; jeq L1029
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1029:
L1027:
movl #5,a6@(-16)
movl #2,a6@(-28)
movl a6@(-16),d4
addl a6@(-28),d4
movl d4,a6@(-16)
cmpl #7,a6@(-16); jeq L1031
movl #80,d6
tstl d7; jeq L1033
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1033:
L1031:
movl #5,a6@(-16)
movl #2,a6@(-32)
movl a6@(-16),d4
addl a6@(-32),d4
movl d4,a6@(-16)
cmpl #7,a6@(-16); jeq L1035
movl #81,d6
tstl d7; jeq L1037
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1037:
L1035:
movl #5,a6@(-16)
movl #2,a6@(-36)
movl a6@(-16),d4
addl a6@(-36),d4
movl d4,a6@(-16)
cmpl #7,a6@(-16); jeq L1039
movl #82,d6
tstl d7; jeq L1041
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1041:
L1039:
movl #5,a6@(-16)
fmoves L530:l,fp5
movl a6@(-16),d4
tstl d4; jlt L1047
fmovel d4,fp3
fmoved fp3,a6@(-76)
jra L1048
L1047:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-76)
L1048:
fmoved a6@(-76),fp3
fmoves fp3,d0; fmoves d0,fp3
faddx fp5,fp3
fmovex fp3,fp3
fmoved L835:l,fp2
fcmpx fp2,fp3; fjlt L1045
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-68)
jra L1046
L1045:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-68)
L1046:
movl a6@(-68),a6@(-16)
cmpl #7,a6@(-16); jeq L1049
movl #83,d6
tstl d7; jeq L1051
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1051:
L1049:
movl #5,a6@(-16)
fmoved L416:l,fp4
movl a6@(-16),d4
tstl d4; jlt L1057
fmovel d4,fp3
fmoved fp3,a6@(-88)
jra L1058
L1057:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-88)
L1058:
fmoved a6@(-88),fp3
faddx fp4,fp3
fmoved L835:l,fp2
fcmpx fp2,fp3; fjlt L1055
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-80)
jra L1056
L1055:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-80)
L1056:
movl a6@(-80),a6@(-16)
cmpl #7,a6@(-16); jeq L1059
movl #84,d6
tstl d7; jeq L1061
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1061:
L1059:
fmoves L847:l,fp7
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; faddx fp3,fp2
fmovex fp2,fp7
fcmps L1065:l,fp7; fjeq L1063
movl #85,d6
tstl d7; jeq L1066
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1066:
L1063:
fmoves L847:l,fp7
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; faddx fp3,fp2
fmovex fp2,fp7
fcmps L1065:l,fp7; fjeq L1068
movl #86,d6
tstl d7; jeq L1070
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1070:
L1068:
fmoves L847:l,fp7
movl #2,a6@(-28)
movl a6@(-28),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; faddx fp3,fp2
fmovex fp2,fp7
fcmps L1065:l,fp7; fjeq L1072
movl #87,d6
tstl d7; jeq L1074
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1074:
L1072:
fmoves L847:l,fp7
movl #2,a6@(-32)
movl a6@(-32),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; faddx fp3,fp2
fmovex fp2,fp7
fcmps L1065:l,fp7; fjeq L1076
movl #88,d6
tstl d7; jeq L1078
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1078:
L1076:
fmoves L847:l,fp7
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L1081
fmovel d4,fp3
fmoved fp3,a6@(-96)
jra L1082
L1081:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-96)
L1082:
fmoved a6@(-96),fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; faddx fp3,fp2
fmovex fp2,fp7
fcmps L1065:l,fp7; fjeq L1083
movl #89,d6
tstl d7; jeq L1085
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1085:
L1083:
fmoves L847:l,fp7
fmoves L530:l,fp5
fmovex fp7,fp3; faddx fp5,fp3
fmovex fp3,fp7
fcmps L1065:l,fp7; fjeq L1087
movl #90,d6
tstl d7; jeq L1089
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1089:
L1087:
fmoves L847:l,fp7
fmoved L416:l,fp4
fmovex fp7,fp3; faddx fp4,fp3
fmoves fp3,d0; fmoves d0,fp7
fcmps L1065:l,fp7; fjeq L1091
movl #91,d6
tstl d7; jeq L1093
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1093:
L1091:
fmoved L879:l,fp6
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovex fp6,fp3; faddl d4,fp3
fmovex fp3,fp6
fcmpd L1097:l,fp6; fjeq L1095
movl #92,d6
tstl d7; jeq L1098
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1098:
L1095:
fmoved L879:l,fp6
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovex fp6,fp3; faddl d4,fp3
fmovex fp3,fp6
fcmpd L1097:l,fp6; fjeq L1100
movl #93,d6
tstl d7; jeq L1102
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1102:
L1100:
fmoved L879:l,fp6
movl #2,a6@(-28)
movl a6@(-28),d4
fmovex fp6,fp3; faddl d4,fp3
fmovex fp3,fp6
fcmpd L1097:l,fp6; fjeq L1104
movl #94,d6
tstl d7; jeq L1106
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1106:
L1104:
fmoved L879:l,fp6
movl #2,a6@(-32)
movl a6@(-32),d4
fmovex fp6,fp3; faddl d4,fp3
fmovex fp3,fp6
fcmpd L1097:l,fp6; fjeq L1108
movl #95,d6
tstl d7; jeq L1110
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1110:
L1108:
fmoved L879:l,fp6
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L1113
fmovel d4,fp3
fmoved fp3,a6@(-104)
jra L1114
L1113:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-104)
L1114:
fmovex fp6,fp3; faddd a6@(-104),fp3
fmovex fp3,fp6
fcmpd L1097:l,fp6; fjeq L1115
movl #96,d6
tstl d7; jeq L1117
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1117:
L1115:
fmoved L879:l,fp6
fmoves L530:l,fp5
fmovex fp6,fp3; faddx fp5,fp3
fmovex fp3,fp6
fcmpd L1097:l,fp6; fjeq L1119
movl #97,d6
tstl d7; jeq L1121
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1121:
L1119:
fmoved L879:l,fp6
fmoved L416:l,fp4
fmovex fp6,fp3; faddx fp4,fp3
fmovex fp3,fp6
fcmpd L1097:l,fp6; fjeq L1123
movl #98,d6
tstl d7; jeq L1125
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1125:
L1123:
movb #5,d5
movb #2,a6@(-20)
movl d5,d4; extbl d4
movb a6@(-20),d3
extbl d3
subl d3,d4
movb d4,d5
cmpb #3,d5; jeq L1127
movl #99,d6
tstl d7; jeq L1129
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1129:
L1127:
movb #5,d5
movw #2,a6@(-24)
movl d5,d4; extbl d4
movw a6@(-24),d3
extl d3
subl d3,d4
movb d4,d5
cmpb #3,d5; jeq L1131
movl #100,d6
tstl d7; jeq L1133
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1133:
L1131:
movb #5,d5
movl #2,a6@(-28)
movl d5,d4; extbl d4
subl a6@(-28),d4
movb d4,d5
cmpb #3,d5; jeq L1135
movl #101,d6
tstl d7; jeq L1137
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1137:
L1135:
movb #5,d5
movl #2,a6@(-32)
movl d5,d4; extbl d4
subl a6@(-32),d4
movb d4,d5
cmpb #3,d5; jeq L1139
movl #102,d6
tstl d7; jeq L1141
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1141:
L1139:
movb #5,d5
movl #2,a6@(-36)
movl d5,d4; extbl d4
movl d4,d4
subl a6@(-36),d4
movb d4,d5
cmpb #3,d5; jeq L1143
movl #103,d6
tstl d7; jeq L1145
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1145:
L1143:
movb #5,d5
fmoves L530:l,fp5
movl d5,d4; extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fsubx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,d5
cmpb #3,d5; jeq L1147
movl #104,d6
tstl d7; jeq L1149
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1149:
L1147:
movb #5,d5
fmoved L416:l,fp4
movl d5,d4; extbl d4
fmovel d4,fp3
fsubx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,d5
cmpb #3,d5; jeq L1151
movl #105,d6
tstl d7; jeq L1153
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1153:
L1151:
movw #5,a6@(-4)
movb #2,a6@(-20)
movw a6@(-4),d4
extl d4
movb a6@(-20),d3
extbl d3
subl d3,d4
movw d4,a6@(-4)
cmpw #3,a6@(-4); jeq L1155
movl #106,d6
tstl d7; jeq L1157
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1157:
L1155:
movw #5,a6@(-4)
movw #2,a6@(-24)
movw a6@(-4),d4
extl d4
movw a6@(-24),d3
extl d3
subl d3,d4
movw d4,a6@(-4)
cmpw #3,a6@(-4); jeq L1159
movl #107,d6
tstl d7; jeq L1161
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1161:
L1159:
movw #5,a6@(-4)
movl #2,a6@(-28)
movw a6@(-4),d4
extl d4
subl a6@(-28),d4
movw d4,a6@(-4)
cmpw #3,a6@(-4); jeq L1163
movl #108,d6
tstl d7; jeq L1165
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1165:
L1163:
movw #5,a6@(-4)
movl #2,a6@(-32)
movw a6@(-4),d4
extl d4
subl a6@(-32),d4
movw d4,a6@(-4)
cmpw #3,a6@(-4); jeq L1167
movl #109,d6
tstl d7; jeq L1169
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1169:
L1167:
movw #5,a6@(-4)
movl #2,a6@(-36)
movw a6@(-4),d4
extl d4
movl d4,d4
subl a6@(-36),d4
movw d4,a6@(-4)
cmpw #3,a6@(-4); jeq L1171
movl #110,d6
tstl d7; jeq L1173
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1173:
L1171:
movw #5,a6@(-4)
fmoves L530:l,fp5
movw a6@(-4),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fsubx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #3,a6@(-4); jeq L1175
movl #111,d6
tstl d7; jeq L1177
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1177:
L1175:
movw #5,a6@(-4)
fmoved L416:l,fp4
movw a6@(-4),d4
extl d4
fmovel d4,fp3
fsubx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #3,a6@(-4); jeq L1179
movl #112,d6
tstl d7; jeq L1181
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1181:
L1179:
movl #5,a6@(-8)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
subl d4,a6@(-8)
cmpl #3,a6@(-8); jeq L1183
movl #113,d6
tstl d7; jeq L1185
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1185:
L1183:
movl #5,a6@(-8)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
subl d4,a6@(-8)
cmpl #3,a6@(-8); jeq L1187
movl #114,d6
tstl d7; jeq L1189
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1189:
L1187:
movl #5,a6@(-8)
movl #2,a6@(-28)
movl a6@(-8),d4
subl a6@(-28),d4
movl d4,a6@(-8)
cmpl #3,a6@(-8); jeq L1191
movl #115,d6
tstl d7; jeq L1193
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1193:
L1191:
movl #5,a6@(-8)
movl #2,a6@(-32)
movl a6@(-8),d4
subl a6@(-32),d4
movl d4,a6@(-8)
cmpl #3,a6@(-8); jeq L1195
movl #116,d6
tstl d7; jeq L1197
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1197:
L1195:
movl #5,a6@(-8)
movl #2,a6@(-36)
movl a6@(-8),d4
subl a6@(-36),d4
movl d4,a6@(-8)
cmpl #3,a6@(-8); jeq L1199
movl #117,d6
tstl d7; jeq L1201
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1201:
L1199:
movl #5,a6@(-8)
fmoves L530:l,fp5
movl a6@(-8),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fsubx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #3,a6@(-8); jeq L1203
movl #118,d6
tstl d7; jeq L1205
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1205:
L1203:
movl #5,a6@(-8)
fmoved L416:l,fp4
movl a6@(-8),d4
fmovel d4,fp3
fsubx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #3,a6@(-8); jeq L1207
movl #119,d6
tstl d7; jeq L1209
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1209:
L1207:
movl #5,a6@(-12)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
subl d4,a6@(-12)
cmpl #3,a6@(-12); jeq L1211
movl #120,d6
tstl d7; jeq L1213
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1213:
L1211:
movl #5,a6@(-12)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
subl d4,a6@(-12)
cmpl #3,a6@(-12); jeq L1215
movl #121,d6
tstl d7; jeq L1217
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1217:
L1215:
movl #5,a6@(-12)
movl #2,a6@(-28)
movl a6@(-12),d4
subl a6@(-28),d4
movl d4,a6@(-12)
cmpl #3,a6@(-12); jeq L1219
movl #122,d6
tstl d7; jeq L1221
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1221:
L1219:
movl #5,a6@(-12)
movl #2,a6@(-32)
movl a6@(-12),d4
subl a6@(-32),d4
movl d4,a6@(-12)
cmpl #3,a6@(-12); jeq L1223
movl #123,d6
tstl d7; jeq L1225
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1225:
L1223:
movl #5,a6@(-12)
movl #2,a6@(-36)
movl a6@(-12),d4
subl a6@(-36),d4
movl d4,a6@(-12)
cmpl #3,a6@(-12); jeq L1227
movl #124,d6
tstl d7; jeq L1229
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1229:
L1227:
movl #5,a6@(-12)
fmoves L530:l,fp5
movl a6@(-12),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fsubx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #3,a6@(-12); jeq L1231
movl #125,d6
tstl d7; jeq L1233
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1233:
L1231:
movl #5,a6@(-12)
fmoved L416:l,fp4
movl a6@(-12),d4
fmovel d4,fp3
fsubx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #3,a6@(-12); jeq L1235
movl #126,d6
tstl d7; jeq L1237
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1237:
L1235:
movl #5,a6@(-16)
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
subl d4,a6@(-16)
cmpl #3,a6@(-16); jeq L1239
movl #127,d6
tstl d7; jeq L1241
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1241:
L1239:
movl #5,a6@(-16)
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
subl d4,a6@(-16)
cmpl #3,a6@(-16); jeq L1243
movl #128,d6
tstl d7; jeq L1245
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1245:
L1243:
movl #5,a6@(-16)
movl #2,a6@(-28)
movl a6@(-16),d4
subl a6@(-28),d4
movl d4,a6@(-16)
cmpl #3,a6@(-16); jeq L1247
movl #129,d6
tstl d7; jeq L1249
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1249:
L1247:
movl #5,a6@(-16)
movl #2,a6@(-32)
movl a6@(-16),d4
subl a6@(-32),d4
movl d4,a6@(-16)
cmpl #3,a6@(-16); jeq L1251
movl #130,d6
tstl d7; jeq L1253
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1253:
L1251:
movl #5,a6@(-16)
movl #2,a6@(-36)
movl a6@(-16),d4
subl a6@(-36),d4
movl d4,a6@(-16)
cmpl #3,a6@(-16); jeq L1255
movl #131,d6
tstl d7; jeq L1257
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1257:
L1255:
movl #5,a6@(-16)
fmoves L530:l,fp5
movl a6@(-16),d4
tstl d4; jlt L1263
fmovel d4,fp3
fmoved fp3,a6@(-116)
jra L1264
L1263:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-116)
L1264:
fmoved a6@(-116),fp3
fmoves fp3,d0; fmoves d0,fp3
fsubx fp5,fp3
fmovex fp3,fp3
fmoved L835:l,fp2
fcmpx fp2,fp3; fjlt L1261
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-108)
jra L1262
L1261:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-108)
L1262:
movl a6@(-108),a6@(-16)
cmpl #3,a6@(-16); jeq L1265
movl #132,d6
tstl d7; jeq L1267
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1267:
L1265:
movl #5,a6@(-16)
fmoved L416:l,fp4
movl a6@(-16),d4
tstl d4; jlt L1273
fmovel d4,fp3
fmoved fp3,a6@(-128)
jra L1274
L1273:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-128)
L1274:
fmoved a6@(-128),fp3
fsubx fp4,fp3
fmoved L835:l,fp2
fcmpx fp2,fp3; fjlt L1271
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-120)
jra L1272
L1271:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-120)
L1272:
movl a6@(-120),a6@(-16)
cmpl #3,a6@(-16); jeq L1275
movl #133,d6
tstl d7; jeq L1277
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1277:
L1275:
fmoves L847:l,fp7
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fsubx fp3,fp2
fmovex fp2,fp7
fcmps L1281:l,fp7; fjeq L1279
movl #134,d6
tstl d7; jeq L1282
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1282:
L1279:
fmoves L847:l,fp7
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fsubx fp3,fp2
fmovex fp2,fp7
fcmps L1281:l,fp7; fjeq L1284
movl #135,d6
tstl d7; jeq L1286
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1286:
L1284:
fmoves L847:l,fp7
movl #2,a6@(-28)
movl a6@(-28),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fsubx fp3,fp2
fmovex fp2,fp7
fcmps L1281:l,fp7; fjeq L1288
movl #136,d6
tstl d7; jeq L1290
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1290:
L1288:
fmoves L847:l,fp7
movl #2,a6@(-32)
movl a6@(-32),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fsubx fp3,fp2
fmovex fp2,fp7
fcmps L1281:l,fp7; fjeq L1292
movl #137,d6
tstl d7; jeq L1294
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1294:
L1292:
fmoves L847:l,fp7
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L1297
fmovel d4,fp3
fmoved fp3,a6@(-136)
jra L1298
L1297:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-136)
L1298:
fmoved a6@(-136),fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fsubx fp3,fp2
fmovex fp2,fp7
fcmps L1281:l,fp7; fjeq L1299
movl #138,d6
tstl d7; jeq L1301
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1301:
L1299:
fmoves L847:l,fp7
fmoves L530:l,fp5
fmovex fp7,fp3; fsubx fp5,fp3
fmovex fp3,fp7
fcmps L1281:l,fp7; fjeq L1303
movl #139,d6
tstl d7; jeq L1305
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1305:
L1303:
fmoves L847:l,fp7
fmoved L416:l,fp4
fmovex fp7,fp3; fsubx fp4,fp3
fmoves fp3,d0; fmoves d0,fp7
fcmps L1281:l,fp7; fjeq L1307
movl #140,d6
tstl d7; jeq L1309
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1309:
L1307:
fmoved L879:l,fp6
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovex fp6,fp3; fsubl d4,fp3
fmovex fp3,fp6
fcmpd L1313:l,fp6; fjeq L1311
movl #141,d6
tstl d7; jeq L1314
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1314:
L1311:
fmoved L879:l,fp6
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovex fp6,fp3; fsubl d4,fp3
fmovex fp3,fp6
fcmpd L1313:l,fp6; fjeq L1316
movl #142,d6
tstl d7; jeq L1318
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1318:
L1316:
fmoved L879:l,fp6
movl #2,a6@(-28)
movl a6@(-28),d4
fmovex fp6,fp3; fsubl d4,fp3
fmovex fp3,fp6
fcmpd L1313:l,fp6; fjeq L1320
movl #143,d6
tstl d7; jeq L1322
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1322:
L1320:
fmoved L879:l,fp6
movl #2,a6@(-32)
movl a6@(-32),d4
fmovex fp6,fp3; fsubl d4,fp3
fmovex fp3,fp6
fcmpd L1313:l,fp6; fjeq L1324
movl #144,d6
tstl d7; jeq L1326
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1326:
L1324:
fmoved L879:l,fp6
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L1329
fmovel d4,fp3
fmoved fp3,a6@(-144)
jra L1330
L1329:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-144)
L1330:
fmovex fp6,fp3; fsubd a6@(-144),fp3
fmovex fp3,fp6
fcmpd L1313:l,fp6; fjeq L1331
movl #145,d6
tstl d7; jeq L1333
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1333:
L1331:
fmoved L879:l,fp6
fmoves L530:l,fp5
fmovex fp6,fp3; fsubx fp5,fp3
fmovex fp3,fp6
fcmpd L1313:l,fp6; fjeq L1335
movl #146,d6
tstl d7; jeq L1337
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1337:
L1335:
fmoved L879:l,fp6
fmoved L416:l,fp4
fmovex fp6,fp3; fsubx fp4,fp3
fmovex fp3,fp6
fcmpd L1313:l,fp6; fjeq L1339
movl #147,d6
tstl d7; jeq L1341
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1341:
L1339:
movb #5,d5
movb #2,a6@(-20)
movl d5,d4; extbl d4
movb a6@(-20),d3
extbl d3
mulsl d3,d4
movb d4,d5
cmpb #10,d5; jeq L1343
movl #148,d6
tstl d7; jeq L1345
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1345:
L1343:
movb #5,d5
movw #2,a6@(-24)
movl d5,d4; extbl d4
movw a6@(-24),d3
extl d3
mulsl d3,d4
movb d4,d5
cmpb #10,d5; jeq L1347
movl #149,d6
tstl d7; jeq L1349
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1349:
L1347:
movb #5,d5
movl #2,a6@(-28)
movl d5,d4; extbl d4
movl a6@(-28),d3
mulsl d3,d4
movb d4,d5
cmpb #10,d5; jeq L1351
movl #150,d6
tstl d7; jeq L1353
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1353:
L1351:
movb #5,d5
movl #2,a6@(-32)
movl d5,d4; extbl d4
movl a6@(-32),d3
mulsl d3,d4
movb d4,d5
cmpb #10,d5; jeq L1355
movl #151,d6
tstl d7; jeq L1357
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1357:
L1355:
movb #5,d5
movl #2,a6@(-36)
movl d5,d4; extbl d4
movl d4,d4
movl a6@(-36),d3
mulul d3,d4
movb d4,d5
cmpb #10,d5; jeq L1359
movl #152,d6
tstl d7; jeq L1361
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1361:
L1359:
movb #5,d5
fmoves L530:l,fp5
movl d5,d4; extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmulx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,d5
cmpb #10,d5; jeq L1363
movl #153,d6
tstl d7; jeq L1365
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1365:
L1363:
movb #5,d5
fmoved L416:l,fp4
movl d5,d4; extbl d4
fmovel d4,fp3
fmulx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,d5
cmpb #10,d5; jeq L1367
movl #154,d6
tstl d7; jeq L1369
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1369:
L1367:
movw #5,a6@(-4)
movb #2,a6@(-20)
movw a6@(-4),d4
extl d4
movb a6@(-20),d3
extbl d3
mulsl d3,d4
movw d4,a6@(-4)
cmpw #10,a6@(-4); jeq L1371
movl #155,d6
tstl d7; jeq L1373
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1373:
L1371:
movw #5,a6@(-4)
movw #2,a6@(-24)
movw a6@(-4),d4
extl d4
movw a6@(-24),d3
extl d3
mulsl d3,d4
movw d4,a6@(-4)
cmpw #10,a6@(-4); jeq L1375
movl #156,d6
tstl d7; jeq L1377
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1377:
L1375:
movw #5,a6@(-4)
movl #2,a6@(-28)
movw a6@(-4),d4
extl d4
movl a6@(-28),d3
mulsl d3,d4
movw d4,a6@(-4)
cmpw #10,a6@(-4); jeq L1379
movl #157,d6
tstl d7; jeq L1381
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1381:
L1379:
movw #5,a6@(-4)
movl #2,a6@(-32)
movw a6@(-4),d4
extl d4
movl a6@(-32),d3
mulsl d3,d4
movw d4,a6@(-4)
cmpw #10,a6@(-4); jeq L1383
movl #158,d6
tstl d7; jeq L1385
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1385:
L1383:
movw #5,a6@(-4)
movl #2,a6@(-36)
movw a6@(-4),d4
extl d4
movl d4,d4
movl a6@(-36),d3
mulul d3,d4
movw d4,a6@(-4)
cmpw #10,a6@(-4); jeq L1387
movl #159,d6
tstl d7; jeq L1389
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1389:
L1387:
movw #5,a6@(-4)
fmoves L530:l,fp5
movw a6@(-4),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmulx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #10,a6@(-4); jeq L1391
movl #160,d6
tstl d7; jeq L1393
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1393:
L1391:
movw #5,a6@(-4)
fmoved L416:l,fp4
movw a6@(-4),d4
extl d4
fmovel d4,fp3
fmulx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #10,a6@(-4); jeq L1395
movl #161,d6
tstl d7; jeq L1397
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1397:
L1395:
movl #5,a6@(-8)
movb #2,a6@(-20)
movl a6@(-8),d4
movb a6@(-20),d3
extbl d3
mulsl d3,d4
movl d4,a6@(-8)
cmpl #10,a6@(-8); jeq L1399
movl #162,d6
tstl d7; jeq L1401
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1401:
L1399:
movl #5,a6@(-8)
movw #2,a6@(-24)
movl a6@(-8),d4
movw a6@(-24),d3
extl d3
mulsl d3,d4
movl d4,a6@(-8)
cmpl #10,a6@(-8); jeq L1403
movl #163,d6
tstl d7; jeq L1405
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1405:
L1403:
movl #5,a6@(-8)
movl #2,a6@(-28)
movl a6@(-8),d4
movl a6@(-28),d3
mulsl d3,d4
movl d4,a6@(-8)
cmpl #10,a6@(-8); jeq L1407
movl #164,d6
tstl d7; jeq L1409
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1409:
L1407:
movl #5,a6@(-8)
movl #2,a6@(-32)
movl a6@(-8),d4
movl a6@(-32),d3
mulsl d3,d4
movl d4,a6@(-8)
cmpl #10,a6@(-8); jeq L1411
movl #165,d6
tstl d7; jeq L1413
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1413:
L1411:
movl #5,a6@(-8)
movl #2,a6@(-36)
movl a6@(-8),d4
movl a6@(-36),d3
mulul d3,d4
movl d4,a6@(-8)
cmpl #10,a6@(-8); jeq L1415
movl #166,d6
tstl d7; jeq L1417
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1417:
L1415:
movl #5,a6@(-8)
fmoves L530:l,fp5
movl a6@(-8),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmulx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #10,a6@(-8); jeq L1419
movl #167,d6
tstl d7; jeq L1421
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1421:
L1419:
movl #5,a6@(-8)
fmoved L416:l,fp4
movl a6@(-8),d4
fmovel d4,fp3
fmulx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #10,a6@(-8); jeq L1423
movl #168,d6
tstl d7; jeq L1425
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1425:
L1423:
movl #5,a6@(-12)
movb #2,a6@(-20)
movl a6@(-12),d4
movb a6@(-20),d3
extbl d3
mulsl d3,d4
movl d4,a6@(-12)
cmpl #10,a6@(-12); jeq L1427
movl #169,d6
tstl d7; jeq L1429
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1429:
L1427:
movl #5,a6@(-12)
movw #2,a6@(-24)
movl a6@(-12),d4
movw a6@(-24),d3
extl d3
mulsl d3,d4
movl d4,a6@(-12)
cmpl #10,a6@(-12); jeq L1431
movl #170,d6
tstl d7; jeq L1433
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1433:
L1431:
movl #5,a6@(-12)
movl #2,a6@(-28)
movl a6@(-12),d4
movl a6@(-28),d3
mulsl d3,d4
movl d4,a6@(-12)
cmpl #10,a6@(-12); jeq L1435
movl #171,d6
tstl d7; jeq L1437
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1437:
L1435:
movl #5,a6@(-12)
movl #2,a6@(-32)
movl a6@(-12),d4
movl a6@(-32),d3
mulsl d3,d4
movl d4,a6@(-12)
cmpl #10,a6@(-12); jeq L1439
movl #172,d6
tstl d7; jeq L1441
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1441:
L1439:
movl #5,a6@(-12)
movl #2,a6@(-36)
movl a6@(-12),d4
movl a6@(-36),d3
mulul d3,d4
movl d4,a6@(-12)
cmpl #10,a6@(-12); jeq L1443
movl #173,d6
tstl d7; jeq L1445
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1445:
L1443:
movl #5,a6@(-12)
fmoves L530:l,fp5
movl a6@(-12),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmulx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #10,a6@(-12); jeq L1447
movl #174,d6
tstl d7; jeq L1449
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1449:
L1447:
movl #5,a6@(-12)
fmoved L416:l,fp4
movl a6@(-12),d4
fmovel d4,fp3
fmulx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #10,a6@(-12); jeq L1451
movl #175,d6
tstl d7; jeq L1453
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1453:
L1451:
movl #5,a6@(-16)
movb #2,a6@(-20)
movl a6@(-16),d4
movb a6@(-20),d3
extbl d3
movl d3,d3
mulul d3,d4
movl d4,a6@(-16)
cmpl #10,a6@(-16); jeq L1455
movl #176,d6
tstl d7; jeq L1457
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1457:
L1455:
movl #5,a6@(-16)
movw #2,a6@(-24)
movl a6@(-16),d4
movw a6@(-24),d3
extl d3
movl d3,d3
mulul d3,d4
movl d4,a6@(-16)
cmpl #10,a6@(-16); jeq L1459
movl #177,d6
tstl d7; jeq L1461
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1461:
L1459:
movl #5,a6@(-16)
movl #2,a6@(-28)
movl a6@(-16),d4
movl a6@(-28),d3
mulul d3,d4
movl d4,a6@(-16)
cmpl #10,a6@(-16); jeq L1463
movl #178,d6
tstl d7; jeq L1465
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1465:
L1463:
movl #5,a6@(-16)
movl #2,a6@(-32)
movl a6@(-16),d4
movl a6@(-32),d3
mulul d3,d4
movl d4,a6@(-16)
cmpl #10,a6@(-16); jeq L1467
movl #179,d6
tstl d7; jeq L1469
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1469:
L1467:
movl #5,a6@(-16)
movl #2,a6@(-36)
movl a6@(-16),d4
movl a6@(-36),d3
mulul d3,d4
movl d4,a6@(-16)
cmpl #10,a6@(-16); jeq L1471
movl #180,d6
tstl d7; jeq L1473
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1473:
L1471:
movl #5,a6@(-16)
fmoves L530:l,fp5
movl a6@(-16),d4
tstl d4; jlt L1479
fmovel d4,fp3
fmoved fp3,a6@(-156)
jra L1480
L1479:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-156)
L1480:
fmoved a6@(-156),fp3
fmoves fp3,d0; fmoves d0,fp3
fmulx fp5,fp3
fmovex fp3,fp3
fmoved L835:l,fp2
fcmpx fp2,fp3; fjlt L1477
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-148)
jra L1478
L1477:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-148)
L1478:
movl a6@(-148),a6@(-16)
cmpl #10,a6@(-16); jeq L1481
movl #181,d6
tstl d7; jeq L1483
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1483:
L1481:
movl #5,a6@(-16)
fmoved L416:l,fp4
movl a6@(-16),d4
tstl d4; jlt L1489
fmovel d4,fp3
fmoved fp3,a6@(-168)
jra L1490
L1489:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-168)
L1490:
fmoved a6@(-168),fp3
fmulx fp4,fp3
fmoved L835:l,fp2
fcmpx fp2,fp3; fjlt L1487
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-160)
jra L1488
L1487:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-160)
L1488:
movl a6@(-160),a6@(-16)
cmpl #10,a6@(-16); jeq L1491
movl #182,d6
tstl d7; jeq L1493
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1493:
L1491:
fmoves L847:l,fp7
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fmulx fp3,fp2
fmovex fp2,fp7
fcmps L1497:l,fp7; fjeq L1495
movl #183,d6
tstl d7; jeq L1498
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1498:
L1495:
fmoves L847:l,fp7
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fmulx fp3,fp2
fmovex fp2,fp7
fcmps L1497:l,fp7; fjeq L1500
movl #184,d6
tstl d7; jeq L1502
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1502:
L1500:
fmoves L847:l,fp7
movl #2,a6@(-28)
movl a6@(-28),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fmulx fp3,fp2
fmovex fp2,fp7
fcmps L1497:l,fp7; fjeq L1504
movl #185,d6
tstl d7; jeq L1506
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1506:
L1504:
fmoves L847:l,fp7
movl #2,a6@(-32)
movl a6@(-32),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fmulx fp3,fp2
fmovex fp2,fp7
fcmps L1497:l,fp7; fjeq L1508
movl #186,d6
tstl d7; jeq L1510
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1510:
L1508:
fmoves L847:l,fp7
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L1513
fmovel d4,fp3
fmoved fp3,a6@(-176)
jra L1514
L1513:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-176)
L1514:
fmoved a6@(-176),fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fmulx fp3,fp2
fmovex fp2,fp7
fcmps L1497:l,fp7; fjeq L1515
movl #187,d6
tstl d7; jeq L1517
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1517:
L1515:
fmoves L847:l,fp7
fmoves L530:l,fp5
fmovex fp7,fp3; fmulx fp5,fp3
fmovex fp3,fp7
fcmps L1497:l,fp7; fjeq L1519
movl #188,d6
tstl d7; jeq L1521
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1521:
L1519:
fmoves L847:l,fp7
fmoved L416:l,fp4
fmovex fp7,fp3; fmulx fp4,fp3
fmoves fp3,d0; fmoves d0,fp7
fcmps L1497:l,fp7; fjeq L1523
movl #189,d6
tstl d7; jeq L1525
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1525:
L1523:
fmoved L879:l,fp6
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovex fp6,fp3; fmull d4,fp3
fmovex fp3,fp6
fcmpd L1529:l,fp6; fjeq L1527
movl #190,d6
tstl d7; jeq L1530
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1530:
L1527:
fmoved L879:l,fp6
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovex fp6,fp3; fmull d4,fp3
fmovex fp3,fp6
fcmpd L1529:l,fp6; fjeq L1532
movl #191,d6
tstl d7; jeq L1534
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1534:
L1532:
fmoved L879:l,fp6
movl #2,a6@(-28)
movl a6@(-28),d4
fmovex fp6,fp3; fmull d4,fp3
fmovex fp3,fp6
fcmpd L1529:l,fp6; fjeq L1536
movl #192,d6
tstl d7; jeq L1538
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1538:
L1536:
fmoved L879:l,fp6
movl #2,a6@(-32)
movl a6@(-32),d4
fmovex fp6,fp3; fmull d4,fp3
fmovex fp3,fp6
fcmpd L1529:l,fp6; fjeq L1540
movl #193,d6
tstl d7; jeq L1542
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1542:
L1540:
fmoved L879:l,fp6
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L1545
fmovel d4,fp3
fmoved fp3,a6@(-184)
jra L1546
L1545:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-184)
L1546:
fmovex fp6,fp3; fmuld a6@(-184),fp3
fmovex fp3,fp6
fcmpd L1529:l,fp6; fjeq L1547
movl #194,d6
tstl d7; jeq L1549
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1549:
L1547:
fmoved L879:l,fp6
fmoves L530:l,fp5
fmovex fp6,fp3; fmulx fp5,fp3
fmovex fp3,fp6
fcmpd L1529:l,fp6; fjeq L1551
movl #195,d6
tstl d7; jeq L1553
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1553:
L1551:
fmoved L879:l,fp6
fmoved L416:l,fp4
fmovex fp6,fp3; fmulx fp4,fp3
fmovex fp3,fp6
fcmpd L1529:l,fp6; fjeq L1555
movl #196,d6
tstl d7; jeq L1557
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1557:
L1555:
movb #5,d5
movb #2,a6@(-20)
movl d5,d4; extbl d4
movb a6@(-20),d3
extbl d3
divsl d3,d4
movb d4,d5
cmpb #2,d5; jeq L1559
movl #197,d6
tstl d7; jeq L1561
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1561:
L1559:
movb #5,d5
movw #2,a6@(-24)
movl d5,d4; extbl d4
movw a6@(-24),d3
extl d3
divsl d3,d4
movb d4,d5
cmpb #2,d5; jeq L1563
movl #198,d6
tstl d7; jeq L1565
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1565:
L1563:
movb #5,d5
movl #2,a6@(-28)
movl d5,d4; extbl d4
movl a6@(-28),d3
divsl d3,d4
movb d4,d5
cmpb #2,d5; jeq L1567
movl #199,d6
tstl d7; jeq L1569
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1569:
L1567:
movb #5,d5
movl #2,a6@(-32)
movl d5,d4; extbl d4
movl a6@(-32),d3
divsl d3,d4
movb d4,d5
cmpb #2,d5; jeq L1571
movl #200,d6
tstl d7; jeq L1573
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1573:
L1571:
movb #5,d5
movl #2,a6@(-36)
movl d5,d4; extbl d4
movl d4,d4
movl a6@(-36),d3
divul d3,d4
movb d4,d5
cmpb #2,d5; jeq L1575
movl #201,d6
tstl d7; jeq L1577
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1577:
L1575:
movb #5,d5
fmoves L530:l,fp5
movl d5,d4; extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fdivx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,d5
cmpb #2,d5; jeq L1579
movl #202,d6
tstl d7; jeq L1581
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1581:
L1579:
movb #5,d5
fmoved L416:l,fp4
movl d5,d4; extbl d4
fmovel d4,fp3
fdivx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movb d4,d5
cmpb #2,d5; jeq L1583
movl #203,d6
tstl d7; jeq L1585
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1585:
L1583:
movw #5,a6@(-4)
movb #2,a6@(-20)
movw a6@(-4),d4
extl d4
movb a6@(-20),d3
extbl d3
divsl d3,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L1587
movl #204,d6
tstl d7; jeq L1589
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1589:
L1587:
movw #5,a6@(-4)
movw #2,a6@(-24)
movw a6@(-4),d4
extl d4
movw a6@(-24),d3
extl d3
divsl d3,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L1591
movl #205,d6
tstl d7; jeq L1593
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1593:
L1591:
movw #5,a6@(-4)
movl #2,a6@(-28)
movw a6@(-4),d4
extl d4
movl a6@(-28),d3
divsl d3,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L1595
movl #206,d6
tstl d7; jeq L1597
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1597:
L1595:
movw #5,a6@(-4)
movl #2,a6@(-32)
movw a6@(-4),d4
extl d4
movl a6@(-32),d3
divsl d3,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L1599
movl #207,d6
tstl d7; jeq L1601
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1601:
L1599:
movw #5,a6@(-4)
movl #2,a6@(-36)
movw a6@(-4),d4
extl d4
movl d4,d4
movl a6@(-36),d3
divul d3,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L1603
movl #208,d6
tstl d7; jeq L1605
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1605:
L1603:
movw #5,a6@(-4)
fmoves L530:l,fp5
movw a6@(-4),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fdivx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L1607
movl #209,d6
tstl d7; jeq L1609
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1609:
L1607:
movw #5,a6@(-4)
fmoved L416:l,fp4
movw a6@(-4),d4
extl d4
fmovel d4,fp3
fdivx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movw d4,a6@(-4)
cmpw #2,a6@(-4); jeq L1611
movl #210,d6
tstl d7; jeq L1613
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1613:
L1611:
movl #5,a6@(-8)
movb #2,a6@(-20)
movl a6@(-8),d4
movb a6@(-20),d3
extbl d3
divsl d3,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L1615
movl #211,d6
tstl d7; jeq L1617
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1617:
L1615:
movl #5,a6@(-8)
movw #2,a6@(-24)
movl a6@(-8),d4
movw a6@(-24),d3
extl d3
divsl d3,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L1619
movl #212,d6
tstl d7; jeq L1621
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1621:
L1619:
movl #5,a6@(-8)
movl #2,a6@(-28)
movl a6@(-8),d4
movl a6@(-28),d3
divsl d3,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L1623
movl #213,d6
tstl d7; jeq L1625
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1625:
L1623:
movl #5,a6@(-8)
movl #2,a6@(-32)
movl a6@(-8),d4
movl a6@(-32),d3
divsl d3,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L1627
movl #214,d6
tstl d7; jeq L1629
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1629:
L1627:
movl #5,a6@(-8)
movl #2,a6@(-36)
movl a6@(-8),d4
movl a6@(-36),d3
divul d3,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L1631
movl #215,d6
tstl d7; jeq L1633
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1633:
L1631:
movl #5,a6@(-8)
fmoves L530:l,fp5
movl a6@(-8),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fdivx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L1635
movl #216,d6
tstl d7; jeq L1637
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1637:
L1635:
movl #5,a6@(-8)
fmoved L416:l,fp4
movl a6@(-8),d4
fmovel d4,fp3
fdivx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-8)
cmpl #2,a6@(-8); jeq L1639
movl #217,d6
tstl d7; jeq L1641
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1641:
L1639:
movl #5,a6@(-12)
movb #2,a6@(-20)
movl a6@(-12),d4
movb a6@(-20),d3
extbl d3
divsl d3,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L1643
movl #218,d6
tstl d7; jeq L1645
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1645:
L1643:
movl #5,a6@(-12)
movw #2,a6@(-24)
movl a6@(-12),d4
movw a6@(-24),d3
extl d3
divsl d3,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L1647
movl #219,d6
tstl d7; jeq L1649
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1649:
L1647:
movl #5,a6@(-12)
movl #2,a6@(-28)
movl a6@(-12),d4
movl a6@(-28),d3
divsl d3,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L1651
movl #220,d6
tstl d7; jeq L1653
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1653:
L1651:
movl #5,a6@(-12)
movl #2,a6@(-32)
movl a6@(-12),d4
movl a6@(-32),d3
divsl d3,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L1655
movl #221,d6
tstl d7; jeq L1657
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1657:
L1655:
movl #5,a6@(-12)
movl #2,a6@(-36)
movl a6@(-12),d4
movl a6@(-36),d3
divul d3,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L1659
movl #222,d6
tstl d7; jeq L1661
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1661:
L1659:
movl #5,a6@(-12)
fmoves L530:l,fp5
movl a6@(-12),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fdivx fp5,fp3
fmovex fp3,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L1663
movl #223,d6
tstl d7; jeq L1665
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1665:
L1663:
movl #5,a6@(-12)
fmoved L416:l,fp4
movl a6@(-12),d4
fmovel d4,fp3
fdivx fp4,fp3
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-12)
cmpl #2,a6@(-12); jeq L1667
movl #224,d6
tstl d7; jeq L1669
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1669:
L1667:
movl #5,a6@(-16)
movb #2,a6@(-20)
movl a6@(-16),d4
movb a6@(-20),d3
extbl d3
movl d3,d3
divul d3,d4
movl d4,a6@(-16)
cmpl #2,a6@(-16); jeq L1671
movl #225,d6
tstl d7; jeq L1673
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1673:
L1671:
movl #5,a6@(-16)
movw #2,a6@(-24)
movl a6@(-16),d4
movw a6@(-24),d3
extl d3
movl d3,d3
divul d3,d4
movl d4,a6@(-16)
cmpl #2,a6@(-16); jeq L1675
movl #226,d6
tstl d7; jeq L1677
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1677:
L1675:
movl #5,a6@(-16)
movl #2,a6@(-28)
movl a6@(-16),d4
movl a6@(-28),d3
divul d3,d4
movl d4,a6@(-16)
cmpl #2,a6@(-16); jeq L1679
movl #227,d6
tstl d7; jeq L1681
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1681:
L1679:
movl #5,a6@(-16)
movl #2,a6@(-32)
movl a6@(-16),d4
movl a6@(-32),d3
divul d3,d4
movl d4,a6@(-16)
cmpl #2,a6@(-16); jeq L1683
movl #228,d6
tstl d7; jeq L1685
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1685:
L1683:
movl #5,a6@(-16)
movl #2,a6@(-36)
movl a6@(-16),d4
movl a6@(-36),d3
divul d3,d4
movl d4,a6@(-16)
cmpl #2,a6@(-16); jeq L1687
movl #229,d6
tstl d7; jeq L1689
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1689:
L1687:
movl #5,a6@(-16)
fmoves L530:l,fp5
movl a6@(-16),d4
tstl d4; jlt L1695
fmovel d4,fp3
fmoved fp3,a6@(-196)
jra L1696
L1695:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-196)
L1696:
fmoved a6@(-196),fp3
fmoves fp3,d0; fmoves d0,fp3
fdivx fp5,fp3
fmovex fp3,fp3
fmoved L835:l,fp2
fcmpx fp2,fp3; fjlt L1693
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-188)
jra L1694
L1693:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-188)
L1694:
movl a6@(-188),a6@(-16)
cmpl #2,a6@(-16); jeq L1697
movl #230,d6
tstl d7; jeq L1699
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1699:
L1697:
movl #5,a6@(-16)
fmoved L416:l,fp4
movl a6@(-16),d4
tstl d4; jlt L1705
fmovel d4,fp3
fmoved fp3,a6@(-208)
jra L1706
L1705:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-208)
L1706:
fmoved a6@(-208),fp3
fdivx fp4,fp3
fmoved L835:l,fp2
fcmpx fp2,fp3; fjlt L1703
fmovex fp3,fp1; fsubx fp2,fp1
fintrzx fp1,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-200)
jra L1704
L1703:
fintrzx fp3,fp0; fmovel fp0,d4
movl d4,a6@(-200)
L1704:
movl a6@(-200),a6@(-16)
cmpl #2,a6@(-16); jeq L1707
movl #231,d6
tstl d7; jeq L1709
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1709:
L1707:
fmoves L847:l,fp7
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fdivx fp3,fp2
fmovex fp2,fp7
fcmpd L1713:l,fp7; fjeq L1711
movl #232,d6
tstl d7; jeq L1714
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1714:
L1711:
fmoves L847:l,fp7
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fdivx fp3,fp2
fmovex fp2,fp7
fcmpd L1713:l,fp7; fjeq L1716
movl #233,d6
tstl d7; jeq L1718
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1718:
L1716:
fmoves L847:l,fp7
movl #2,a6@(-28)
movl a6@(-28),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fdivx fp3,fp2
fmovex fp2,fp7
fcmpd L1713:l,fp7; fjeq L1720
movl #234,d6
tstl d7; jeq L1722
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1722:
L1720:
fmoves L847:l,fp7
movl #2,a6@(-32)
movl a6@(-32),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fdivx fp3,fp2
fmovex fp2,fp7
fcmpd L1713:l,fp7; fjeq L1724
movl #235,d6
tstl d7; jeq L1726
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1726:
L1724:
fmoves L847:l,fp7
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L1729
fmovel d4,fp3
fmoved fp3,a6@(-216)
jra L1730
L1729:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-216)
L1730:
fmoved a6@(-216),fp3
fmoves fp3,d0; fmoves d0,fp3
fmovex fp7,fp2; fdivx fp3,fp2
fmovex fp2,fp7
fcmpd L1713:l,fp7; fjeq L1731
movl #236,d6
tstl d7; jeq L1733
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1733:
L1731:
fmoves L847:l,fp7
fmoves L530:l,fp5
fmovex fp7,fp3; fdivx fp5,fp3
fmovex fp3,fp7
fcmpd L1713:l,fp7; fjeq L1735
movl #237,d6
tstl d7; jeq L1737
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1737:
L1735:
fmoves L847:l,fp7
fmoved L416:l,fp4
fmovex fp7,fp3; fdivx fp4,fp3
fmoves fp3,d0; fmoves d0,fp7
fcmpd L1713:l,fp7; fjeq L1739
movl #238,d6
tstl d7; jeq L1741
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1741:
L1739:
fmoved L879:l,fp6
movb #2,a6@(-20)
movb a6@(-20),d4
extbl d4
fmovex fp6,fp3; fdivl d4,fp3
fmovex fp3,fp6
fcmpd L1713:l,fp6; fjeq L1743
movl #239,d6
tstl d7; jeq L1745
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1745:
L1743:
fmoved L879:l,fp6
movw #2,a6@(-24)
movw a6@(-24),d4
extl d4
fmovex fp6,fp3; fdivl d4,fp3
fmovex fp3,fp6
fcmpd L1713:l,fp6; fjeq L1747
movl #240,d6
tstl d7; jeq L1749
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1749:
L1747:
fmoved L879:l,fp6
movl #2,a6@(-28)
movl a6@(-28),d4
fmovex fp6,fp3; fdivl d4,fp3
fmovex fp3,fp6
fcmpd L1713:l,fp6; fjeq L1751
movl #241,d6
tstl d7; jeq L1753
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1753:
L1751:
fmoved L879:l,fp6
movl #2,a6@(-32)
movl a6@(-32),d4
fmovex fp6,fp3; fdivl d4,fp3
fmovex fp3,fp6
fcmpd L1713:l,fp6; fjeq L1755
movl #242,d6
tstl d7; jeq L1757
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1757:
L1755:
fmoved L879:l,fp6
movl #2,a6@(-36)
movl a6@(-36),d4
tstl d4; jlt L1760
fmovel d4,fp3
fmoved fp3,a6@(-224)
jra L1761
L1760:
fmovel d4,fp3
faddd L600:l,fp3
fmoved fp3,a6@(-224)
L1761:
fmovex fp6,fp3; fdivd a6@(-224),fp3
fmovex fp3,fp6
fcmpd L1713:l,fp6; fjeq L1762
movl #243,d6
tstl d7; jeq L1764
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1764:
L1762:
fmoved L879:l,fp6
fmoves L530:l,fp5
fmovex fp6,fp3; fdivx fp5,fp3
fmovex fp3,fp6
fcmpd L1713:l,fp6; fjeq L1766
movl #244,d6
tstl d7; jeq L1768
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1768:
L1766:
fmoved L879:l,fp6
fmoved L416:l,fp4
fmovex fp6,fp3; fdivx fp4,fp3
fmovex fp3,fp6
fcmpd L1713:l,fp6; fjeq L1770
movl #245,d6
tstl d7; jeq L1772
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1772:
L1770:
movb #5,d5
movb #2,a6@(-20)
movl d5,d0; extbl d0
movb a6@(-20),d4
extbl d4
divsll d4,d4:d0 |mod
movb d4,d5
cmpb #1,d5; jeq L1774
movl #246,d6
tstl d7; jeq L1776
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1776:
L1774:
movb #5,d5
movw #2,a6@(-24)
movl d5,d0; extbl d0
movw a6@(-24),d4
extl d4
divsll d4,d4:d0 |mod
movb d4,d5
cmpb #1,d5; jeq L1778
movl #247,d6
tstl d7; jeq L1780
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1780:
L1778:
movb #5,d5
movl #2,a6@(-28)
movl d5,d0; extbl d0
movl a6@(-28),d4
divsll d4,d4:d0 |mod
movb d4,d5
cmpb #1,d5; jeq L1782
movl #248,d6
tstl d7; jeq L1784
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1784:
L1782:
movb #5,d5
movl #2,a6@(-32)
movl d5,d0; extbl d0
movl a6@(-32),d4
divsll d4,d4:d0 |mod
movb d4,d5
cmpb #1,d5; jeq L1786
movl #249,d6
tstl d7; jeq L1788
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1788:
L1786:
movb #5,d5
movl #2,a6@(-36)
movl d5,d4; extbl d4
movl d4,d0
movl a6@(-36),d4
divull d4,d4:d0 |mod
movb d4,d5
cmpb #1,d5; jeq L1790
movl #250,d6
tstl d7; jeq L1792
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1792:
L1790:
movw #5,a6@(-4)
movb #2,a6@(-20)
movw a6@(-4),d4
movl d4,d0; extl d0
movb a6@(-20),d4
extbl d4
divsll d4,d4:d0 |mod
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1794
movl #251,d6
tstl d7; jeq L1796
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1796:
L1794:
movw #5,a6@(-4)
movw #2,a6@(-24)
movw a6@(-4),d4
movl d4,d0; extl d0
movw a6@(-24),d4
extl d4
divsll d4,d4:d0 |mod
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1798
movl #252,d6
tstl d7; jeq L1800
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1800:
L1798:
movw #5,a6@(-4)
movl #2,a6@(-28)
movw a6@(-4),d4
movl d4,d0; extl d0
movl a6@(-28),d4
divsll d4,d4:d0 |mod
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1802
movl #253,d6
tstl d7; jeq L1804
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1804:
L1802:
movw #5,a6@(-4)
movl #2,a6@(-32)
movw a6@(-4),d4
movl d4,d0; extl d0
movl a6@(-32),d4
divsll d4,d4:d0 |mod
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1806
movl #254,d6
tstl d7; jeq L1808
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1808:
L1806:
movw #5,a6@(-4)
movl #2,a6@(-36)
movw a6@(-4),d4
extl d4
movl d4,d0
movl a6@(-36),d4
divull d4,d4:d0 |mod
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1810
movl #255,d6
tstl d7; jeq L1812
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1812:
L1810:
movl #5,a6@(-8)
movb #2,a6@(-20)
movl a6@(-8),d0
movb a6@(-20),d4
extbl d4
divsll d4,d4:d0 |mod
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1814
movl #256,d6
tstl d7; jeq L1816
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1816:
L1814:
movl #5,a6@(-8)
movw #2,a6@(-24)
movl a6@(-8),d0
movw a6@(-24),d4
extl d4
divsll d4,d4:d0 |mod
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1818
movl #257,d6
tstl d7; jeq L1820
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1820:
L1818:
movl #5,a6@(-8)
movl #2,a6@(-28)
movl a6@(-8),d0
movl a6@(-28),d4
divsll d4,d4:d0 |mod
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1822
movl #258,d6
tstl d7; jeq L1824
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1824:
L1822:
movl #5,a6@(-8)
movl #2,a6@(-32)
movl a6@(-8),d0
movl a6@(-32),d4
divsll d4,d4:d0 |mod
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1826
movl #259,d6
tstl d7; jeq L1828
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1828:
L1826:
movl #5,a6@(-8)
movl #2,a6@(-36)
movl a6@(-8),d0
movl a6@(-36),d4
divull d4,d4:d0 |mod
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1830
movl #260,d6
tstl d7; jeq L1832
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1832:
L1830:
movl #5,a6@(-12)
movb #2,a6@(-20)
movl a6@(-12),d0
movb a6@(-20),d4
extbl d4
divsll d4,d4:d0 |mod
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1834
movl #261,d6
tstl d7; jeq L1836
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1836:
L1834:
movl #5,a6@(-12)
movw #2,a6@(-24)
movl a6@(-12),d0
movw a6@(-24),d4
extl d4
divsll d4,d4:d0 |mod
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1838
movl #262,d6
tstl d7; jeq L1840
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1840:
L1838:
movl #5,a6@(-12)
movl #2,a6@(-28)
movl a6@(-12),d0
movl a6@(-28),d4
divsll d4,d4:d0 |mod
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1842
movl #263,d6
tstl d7; jeq L1844
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1844:
L1842:
movl #5,a6@(-12)
movl #2,a6@(-32)
movl a6@(-12),d0
movl a6@(-32),d4
divsll d4,d4:d0 |mod
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1846
movl #264,d6
tstl d7; jeq L1848
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1848:
L1846:
movl #5,a6@(-12)
movl #2,a6@(-36)
movl a6@(-12),d0
movl a6@(-36),d4
divull d4,d4:d0 |mod
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1850
movl #265,d6
tstl d7; jeq L1852
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1852:
L1850:
movl #5,a6@(-16)
movb #2,a6@(-20)
movl a6@(-16),d0
movb a6@(-20),d4
extbl d4
movl d4,d4
divull d4,d4:d0 |mod
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1854
movl #266,d6
tstl d7; jeq L1856
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1856:
L1854:
movl #5,a6@(-16)
movw #2,a6@(-24)
movl a6@(-16),d0
movw a6@(-24),d4
extl d4
movl d4,d4
divull d4,d4:d0 |mod
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1858
movl #267,d6
tstl d7; jeq L1860
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1860:
L1858:
movl #5,a6@(-16)
movl #2,a6@(-28)
movl a6@(-16),d0
movl a6@(-28),d4
divull d4,d4:d0 |mod
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1862
movl #268,d6
tstl d7; jeq L1864
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1864:
L1862:
movl #5,a6@(-16)
movl #2,a6@(-32)
movl a6@(-16),d0
movl a6@(-32),d4
divull d4,d4:d0 |mod
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1866
movl #269,d6
tstl d7; jeq L1868
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1868:
L1866:
movl #5,a6@(-16)
movl #2,a6@(-36)
movl a6@(-16),d0
movl a6@(-36),d4
divull d4,d4:d0 |mod
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1870
movl #270,d6
tstl d7; jeq L1872
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1872:
L1870:
movb #5,d5
movb #2,a6@(-20)
movl d5,d4; extbl d4
movb a6@(-20),d3
extbl d3
asrl d3,d4
movb d4,d5
cmpb #1,d5; jeq L1874
movl #271,d6
tstl d7; jeq L1876
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1876:
L1874:
movb #5,d5
movw #2,a6@(-24)
movl d5,d4; extbl d4
movw a6@(-24),d3
extl d3
asrl d3,d4
movb d4,d5
cmpb #1,d5; jeq L1878
movl #272,d6
tstl d7; jeq L1880
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1880:
L1878:
movb #5,d5
movl #2,a6@(-28)
movl d5,d4; extbl d4
movl a6@(-28),d3
asrl d3,d4
movb d4,d5
cmpb #1,d5; jeq L1882
movl #273,d6
tstl d7; jeq L1884
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1884:
L1882:
movb #5,d5
movl #2,a6@(-32)
movl d5,d4; extbl d4
movl a6@(-32),d3
asrl d3,d4
movb d4,d5
cmpb #1,d5; jeq L1886
movl #274,d6
tstl d7; jeq L1888
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1888:
L1886:
movb #5,d5
movl #2,a6@(-36)
movl d5,d4; extbl d4
movl a6@(-36),d3
asrl d3,d4
movb d4,d5
cmpb #1,d5; jeq L1890
movl #275,d6
tstl d7; jeq L1892
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1892:
L1890:
movw #5,a6@(-4)
movb #2,a6@(-20)
movw a6@(-4),d4
extl d4
movb a6@(-20),d3
extbl d3
asrl d3,d4
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1894
movl #276,d6
tstl d7; jeq L1896
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1896:
L1894:
movw #5,a6@(-4)
movw #2,a6@(-24)
movw a6@(-4),d4
extl d4
movw a6@(-24),d3
extl d3
asrl d3,d4
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1898
movl #277,d6
tstl d7; jeq L1900
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1900:
L1898:
movw #5,a6@(-4)
movl #2,a6@(-28)
movw a6@(-4),d4
extl d4
movl a6@(-28),d3
asrl d3,d4
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1902
movl #278,d6
tstl d7; jeq L1904
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1904:
L1902:
movw #5,a6@(-4)
movl #2,a6@(-32)
movw a6@(-4),d4
extl d4
movl a6@(-32),d3
asrl d3,d4
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1906
movl #279,d6
tstl d7; jeq L1908
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1908:
L1906:
movw #5,a6@(-4)
movl #2,a6@(-36)
movw a6@(-4),d4
extl d4
movl a6@(-36),d3
asrl d3,d4
movw d4,a6@(-4)
cmpw #1,a6@(-4); jeq L1910
movl #280,d6
tstl d7; jeq L1912
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1912:
L1910:
movl #5,a6@(-8)
movb #2,a6@(-20)
movl a6@(-8),d4
movb a6@(-20),d3
extbl d3
asrl d3,d4
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1914
movl #281,d6
tstl d7; jeq L1916
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1916:
L1914:
movl #5,a6@(-8)
movw #2,a6@(-24)
movl a6@(-8),d4
movw a6@(-24),d3
extl d3
asrl d3,d4
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1918
movl #282,d6
tstl d7; jeq L1920
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1920:
L1918:
movl #5,a6@(-8)
movl #2,a6@(-28)
movl a6@(-8),d4
movl a6@(-28),d3
asrl d3,d4
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1922
movl #283,d6
tstl d7; jeq L1924
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1924:
L1922:
movl #5,a6@(-8)
movl #2,a6@(-32)
movl a6@(-8),d4
movl a6@(-32),d3
asrl d3,d4
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1926
movl #284,d6
tstl d7; jeq L1928
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1928:
L1926:
movl #5,a6@(-8)
movl #2,a6@(-36)
movl a6@(-8),d4
movl a6@(-36),d3
asrl d3,d4
movl d4,a6@(-8)
cmpl #1,a6@(-8); jeq L1930
movl #285,d6
tstl d7; jeq L1932
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1932:
L1930:
movl #5,a6@(-12)
movb #2,a6@(-20)
movl a6@(-12),d4
movb a6@(-20),d3
extbl d3
asrl d3,d4
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1934
movl #286,d6
tstl d7; jeq L1936
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1936:
L1934:
movl #5,a6@(-12)
movw #2,a6@(-24)
movl a6@(-12),d4
movw a6@(-24),d3
extl d3
asrl d3,d4
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1938
movl #287,d6
tstl d7; jeq L1940
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1940:
L1938:
movl #5,a6@(-12)
movl #2,a6@(-28)
movl a6@(-12),d4
movl a6@(-28),d3
asrl d3,d4
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1942
movl #288,d6
tstl d7; jeq L1944
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1944:
L1942:
movl #5,a6@(-12)
movl #2,a6@(-32)
movl a6@(-12),d4
movl a6@(-32),d3
asrl d3,d4
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1946
movl #289,d6
tstl d7; jeq L1948
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1948:
L1946:
movl #5,a6@(-12)
movl #2,a6@(-36)
movl a6@(-12),d4
movl a6@(-36),d3
asrl d3,d4
movl d4,a6@(-12)
cmpl #1,a6@(-12); jeq L1950
movl #290,d6
tstl d7; jeq L1952
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1952:
L1950:
movl #5,a6@(-16)
movb #2,a6@(-20)
movl a6@(-16),d4
movb a6@(-20),d3
extbl d3
lsrl d3,d4
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1954
movl #291,d6
tstl d7; jeq L1956
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1956:
L1954:
movl #5,a6@(-16)
movw #2,a6@(-24)
movl a6@(-16),d4
movw a6@(-24),d3
extl d3
lsrl d3,d4
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1958
movl #292,d6
tstl d7; jeq L1960
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1960:
L1958:
movl #5,a6@(-16)
movl #2,a6@(-28)
movl a6@(-16),d4
movl a6@(-28),d3
lsrl d3,d4
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1962
movl #293,d6
tstl d7; jeq L1964
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1964:
L1962:
movl #5,a6@(-16)
movl #2,a6@(-32)
movl a6@(-16),d4
movl a6@(-32),d3
lsrl d3,d4
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1966
movl #294,d6
tstl d7; jeq L1968
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1968:
L1966:
movl #5,a6@(-16)
movl #2,a6@(-36)
movl a6@(-16),d4
movl a6@(-36),d3
lsrl d3,d4
movl d4,a6@(-16)
cmpl #1,a6@(-16); jeq L1970
movl #295,d6
tstl d7; jeq L1972
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1972:
L1970:
movb #5,d5
movb #2,a6@(-20)
movl d5,d4; extbl d4
movb a6@(-20),d3
extbl d3
asll d3,d4
movb d4,d5
cmpb #20,d5; jeq L1974
movl #296,d6
tstl d7; jeq L1976
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1976:
L1974:
movb #5,d5
movw #2,a6@(-24)
movl d5,d4; extbl d4
movw a6@(-24),d3
extl d3
asll d3,d4
movb d4,d5
cmpb #20,d5; jeq L1978
movl #297,d6
tstl d7; jeq L1980
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1980:
L1978:
movb #5,d5
movl #2,a6@(-28)
movl d5,d4; extbl d4
movl a6@(-28),d3
asll d3,d4
movb d4,d5
cmpb #20,d5; jeq L1982
movl #298,d6
tstl d7; jeq L1984
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1984:
L1982:
movb #5,d5
movl #2,a6@(-32)
movl d5,d4; extbl d4
movl a6@(-32),d3
asll d3,d4
movb d4,d5
cmpb #20,d5; jeq L1986
movl #299,d6
tstl d7; jeq L1988
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1988:
L1986:
movb #5,d5
movl #2,a6@(-36)
movl d5,d4; extbl d4
movl a6@(-36),d3
asll d3,d4
movb d4,d5
cmpb #20,d5; jeq L1990
movl #300,d6
tstl d7; jeq L1992
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1992:
L1990:
movw #5,a6@(-4)
movb #2,a6@(-20)
movw a6@(-4),d4
extl d4
movb a6@(-20),d3
extbl d3
asll d3,d4
movw d4,a6@(-4)
cmpw #20,a6@(-4); jeq L1994
movl #301,d6
tstl d7; jeq L1996
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L1996:
L1994:
movw #5,a6@(-4)
movw #2,a6@(-24)
movw a6@(-4),d4
extl d4
movw a6@(-24),d3
extl d3
asll d3,d4
movw d4,a6@(-4)
cmpw #20,a6@(-4); jeq L1998
movl #302,d6
tstl d7; jeq L2000
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2000:
L1998:
movw #5,a6@(-4)
movl #2,a6@(-28)
movw a6@(-4),d4
extl d4
movl a6@(-28),d3
asll d3,d4
movw d4,a6@(-4)
cmpw #20,a6@(-4); jeq L2002
movl #303,d6
tstl d7; jeq L2004
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2004:
L2002:
movw #5,a6@(-4)
movl #2,a6@(-32)
movw a6@(-4),d4
extl d4
movl a6@(-32),d3
asll d3,d4
movw d4,a6@(-4)
cmpw #20,a6@(-4); jeq L2006
movl #304,d6
tstl d7; jeq L2008
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2008:
L2006:
movw #5,a6@(-4)
movl #2,a6@(-36)
movw a6@(-4),d4
extl d4
movl a6@(-36),d3
asll d3,d4
movw d4,a6@(-4)
cmpw #20,a6@(-4); jeq L2010
movl #305,d6
tstl d7; jeq L2012
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2012:
L2010:
movl #5,a6@(-8)
movb #2,a6@(-20)
movl a6@(-8),d4
movb a6@(-20),d3
extbl d3
asll d3,d4
movl d4,a6@(-8)
cmpl #20,a6@(-8); jeq L2014
movl #306,d6
tstl d7; jeq L2016
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2016:
L2014:
movl #5,a6@(-8)
movw #2,a6@(-24)
movl a6@(-8),d4
movw a6@(-24),d3
extl d3
asll d3,d4
movl d4,a6@(-8)
cmpl #20,a6@(-8); jeq L2018
movl #307,d6
tstl d7; jeq L2020
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2020:
L2018:
movl #5,a6@(-8)
movl #2,a6@(-28)
movl a6@(-8),d4
movl a6@(-28),d3
asll d3,d4
movl d4,a6@(-8)
cmpl #20,a6@(-8); jeq L2022
movl #308,d6
tstl d7; jeq L2024
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2024:
L2022:
movl #5,a6@(-8)
movl #2,a6@(-32)
movl a6@(-8),d4
movl a6@(-32),d3
asll d3,d4
movl d4,a6@(-8)
cmpl #20,a6@(-8); jeq L2026
movl #309,d6
tstl d7; jeq L2028
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2028:
L2026:
movl #5,a6@(-8)
movl #2,a6@(-36)
movl a6@(-8),d4
movl a6@(-36),d3
asll d3,d4
movl d4,a6@(-8)
cmpl #20,a6@(-8); jeq L2030
movl #310,d6
tstl d7; jeq L2032
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2032:
L2030:
movl #5,a6@(-12)
movb #2,a6@(-20)
movl a6@(-12),d4
movb a6@(-20),d3
extbl d3
asll d3,d4
movl d4,a6@(-12)
cmpl #20,a6@(-12); jeq L2034
movl #311,d6
tstl d7; jeq L2036
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2036:
L2034:
movl #5,a6@(-12)
movw #2,a6@(-24)
movl a6@(-12),d4
movw a6@(-24),d3
extl d3
asll d3,d4
movl d4,a6@(-12)
cmpl #20,a6@(-12); jeq L2038
movl #312,d6
tstl d7; jeq L2040
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2040:
L2038:
movl #5,a6@(-12)
movl #2,a6@(-28)
movl a6@(-12),d4
movl a6@(-28),d3
asll d3,d4
movl d4,a6@(-12)
cmpl #20,a6@(-12); jeq L2042
movl #313,d6
tstl d7; jeq L2044
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2044:
L2042:
movl #5,a6@(-12)
movl #2,a6@(-32)
movl a6@(-12),d4
movl a6@(-32),d3
asll d3,d4
movl d4,a6@(-12)
cmpl #20,a6@(-12); jeq L2046
movl #314,d6
tstl d7; jeq L2048
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2048:
L2046:
movl #5,a6@(-12)
movl #2,a6@(-36)
movl a6@(-12),d4
movl a6@(-36),d3
asll d3,d4
movl d4,a6@(-12)
cmpl #20,a6@(-12); jeq L2050
movl #315,d6
tstl d7; jeq L2052
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2052:
L2050:
movl #5,a6@(-16)
movb #2,a6@(-20)
movl a6@(-16),d4
movb a6@(-20),d3
extbl d3
lsll d3,d4
movl d4,a6@(-16)
cmpl #20,a6@(-16); jeq L2054
movl #316,d6
tstl d7; jeq L2056
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2056:
L2054:
movl #5,a6@(-16)
movw #2,a6@(-24)
movl a6@(-16),d4
movw a6@(-24),d3
extl d3
lsll d3,d4
movl d4,a6@(-16)
cmpl #20,a6@(-16); jeq L2058
movl #317,d6
tstl d7; jeq L2060
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2060:
L2058:
movl #5,a6@(-16)
movl #2,a6@(-28)
movl a6@(-16),d4
movl a6@(-28),d3
lsll d3,d4
movl d4,a6@(-16)
cmpl #20,a6@(-16); jeq L2062
movl #318,d6
tstl d7; jeq L2064
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2064:
L2062:
movl #5,a6@(-16)
movl #2,a6@(-32)
movl a6@(-16),d4
movl a6@(-32),d3
lsll d3,d4
movl d4,a6@(-16)
cmpl #20,a6@(-16); jeq L2066
movl #319,d6
tstl d7; jeq L2068
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2068:
L2066:
movl #5,a6@(-16)
movl #2,a6@(-36)
movl a6@(-16),d4
movl a6@(-36),d3
lsll d3,d4
movl d4,a6@(-16)
cmpl #20,a6@(-16); jeq L2070
movl #320,d6
tstl d7; jeq L2072
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2072:
L2070:
movb #12,d5
movb #10,a6@(-20)
movl d5,d4; extbl d4
movl d4,d4
movb a6@(-20),d3
extbl d3
andl d3,d4
movb d4,d5
cmpb #8,d5; jeq L2074
movl #321,d6
tstl d7; jeq L2076
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2076:
L2074:
movb #12,d5
movw #10,a6@(-24)
movl d5,d4; extbl d4
movl d4,d4
movw a6@(-24),d3
extl d3
andl d3,d4
movb d4,d5
cmpb #8,d5; jeq L2078
movl #322,d6
tstl d7; jeq L2080
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2080:
L2078:
movb #12,d5
movl #10,a6@(-28)
movl d5,d4; extbl d4
movl d4,d4
andl a6@(-28),d4
movb d4,d5
cmpb #8,d5; jeq L2082
movl #323,d6
tstl d7; jeq L2084
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2084:
L2082:
movb #12,d5
movl #10,a6@(-32)
movl d5,d4; extbl d4
movl d4,d4
andl a6@(-32),d4
movb d4,d5
cmpb #8,d5; jeq L2086
movl #324,d6
tstl d7; jeq L2088
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2088:
L2086:
movb #12,d5
movl #10,a6@(-36)
movl d5,d4; extbl d4
movl d4,d4
andl a6@(-36),d4
movb d4,d5
cmpb #8,d5; jeq L2090
movl #325,d6
tstl d7; jeq L2092
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2092:
L2090:
movw #12,a6@(-4)
movb #10,a6@(-20)
movw a6@(-4),d4
extl d4
movl d4,d4
movb a6@(-20),d3
extbl d3
andl d3,d4
movw d4,a6@(-4)
cmpw #8,a6@(-4); jeq L2094
movl #326,d6
tstl d7; jeq L2096
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2096:
L2094:
movw #12,a6@(-4)
movw #10,a6@(-24)
movw a6@(-4),d4
extl d4
movl d4,d4
movw a6@(-24),d3
extl d3
andl d3,d4
movw d4,a6@(-4)
cmpw #8,a6@(-4); jeq L2098
movl #327,d6
tstl d7; jeq L2100
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2100:
L2098:
movw #12,a6@(-4)
movl #10,a6@(-28)
movw a6@(-4),d4
extl d4
movl d4,d4
andl a6@(-28),d4
movw d4,a6@(-4)
cmpw #8,a6@(-4); jeq L2102
movl #328,d6
tstl d7; jeq L2104
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2104:
L2102:
movw #12,a6@(-4)
movl #10,a6@(-32)
movw a6@(-4),d4
extl d4
movl d4,d4
andl a6@(-32),d4
movw d4,a6@(-4)
cmpw #8,a6@(-4); jeq L2106
movl #329,d6
tstl d7; jeq L2108
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2108:
L2106:
movw #12,a6@(-4)
movl #10,a6@(-36)
movw a6@(-4),d4
extl d4
movl d4,d4
andl a6@(-36),d4
movw d4,a6@(-4)
cmpw #8,a6@(-4); jeq L2110
movl #330,d6
tstl d7; jeq L2112
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2112:
L2110:
movl #12,a6@(-8)
movb #10,a6@(-20)
movl a6@(-8),d4
movb a6@(-20),d3
extbl d3
andl d3,d4
movl d4,a6@(-8)
cmpl #8,a6@(-8); jeq L2114
movl #331,d6
tstl d7; jeq L2116
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2116:
L2114:
movl #12,a6@(-8)
movw #10,a6@(-24)
movl a6@(-8),d4
movw a6@(-24),d3
extl d3
andl d3,d4
movl d4,a6@(-8)
cmpl #8,a6@(-8); jeq L2118
movl #332,d6
tstl d7; jeq L2120
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2120:
L2118:
movl #12,a6@(-8)
movl #10,a6@(-28)
movl a6@(-8),d4
andl a6@(-28),d4
movl d4,a6@(-8)
cmpl #8,a6@(-8); jeq L2122
movl #333,d6
tstl d7; jeq L2124
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2124:
L2122:
movl #12,a6@(-8)
movl #10,a6@(-32)
movl a6@(-8),d4
andl a6@(-32),d4
movl d4,a6@(-8)
cmpl #8,a6@(-8); jeq L2126
movl #334,d6
tstl d7; jeq L2128
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2128:
L2126:
movl #12,a6@(-8)
movl #10,a6@(-36)
movl a6@(-8),d4
andl a6@(-36),d4
movl d4,a6@(-8)
cmpl #8,a6@(-8); jeq L2130
movl #335,d6
tstl d7; jeq L2132
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2132:
L2130:
movl #12,a6@(-12)
movb #10,a6@(-20)
movl a6@(-12),d4
movb a6@(-20),d3
extbl d3
andl d3,d4
movl d4,a6@(-12)
cmpl #8,a6@(-12); jeq L2134
movl #336,d6
tstl d7; jeq L2136
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2136:
L2134:
movl #12,a6@(-12)
movw #10,a6@(-24)
movl a6@(-12),d4
movw a6@(-24),d3
extl d3
andl d3,d4
movl d4,a6@(-12)
cmpl #8,a6@(-12); jeq L2138
movl #337,d6
tstl d7; jeq L2140
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2140:
L2138:
movl #12,a6@(-12)
movl #10,a6@(-28)
movl a6@(-12),d4
andl a6@(-28),d4
movl d4,a6@(-12)
cmpl #8,a6@(-12); jeq L2142
movl #338,d6
tstl d7; jeq L2144
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2144:
L2142:
movl #12,a6@(-12)
movl #10,a6@(-32)
movl a6@(-12),d4
andl a6@(-32),d4
movl d4,a6@(-12)
cmpl #8,a6@(-12); jeq L2146
movl #339,d6
tstl d7; jeq L2148
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2148:
L2146:
movl #12,a6@(-12)
movl #10,a6@(-36)
movl a6@(-12),d4
andl a6@(-36),d4
movl d4,a6@(-12)
cmpl #8,a6@(-12); jeq L2150
movl #340,d6
tstl d7; jeq L2152
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2152:
L2150:
movl #12,a6@(-16)
movb #10,a6@(-20)
movl a6@(-16),d4
movb a6@(-20),d3
extbl d3
andl d3,d4
movl d4,a6@(-16)
cmpl #8,a6@(-16); jeq L2154
movl #341,d6
tstl d7; jeq L2156
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2156:
L2154:
movl #12,a6@(-16)
movw #10,a6@(-24)
movl a6@(-16),d4
movw a6@(-24),d3
extl d3
andl d3,d4
movl d4,a6@(-16)
cmpl #8,a6@(-16); jeq L2158
movl #342,d6
tstl d7; jeq L2160
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2160:
L2158:
movl #12,a6@(-16)
movl #10,a6@(-28)
movl a6@(-16),d4
andl a6@(-28),d4
movl d4,a6@(-16)
cmpl #8,a6@(-16); jeq L2162
movl #343,d6
tstl d7; jeq L2164
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2164:
L2162:
movl #12,a6@(-16)
movl #10,a6@(-32)
movl a6@(-16),d4
andl a6@(-32),d4
movl d4,a6@(-16)
cmpl #8,a6@(-16); jeq L2166
movl #344,d6
tstl d7; jeq L2168
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2168:
L2166:
movl #12,a6@(-16)
movl #10,a6@(-36)
movl a6@(-16),d4
andl a6@(-36),d4
movl d4,a6@(-16)
cmpl #8,a6@(-16); jeq L2170
movl #345,d6
tstl d7; jeq L2172
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2172:
L2170:
movb #12,d5
movb #10,a6@(-20)
movl d5,d4; extbl d4
movl d4,d4
movb a6@(-20),d3
extbl d3
movl d3,d3
eorl d3,d4
movb d4,d5
cmpb #6,d5; jeq L2174
movl #346,d6
tstl d7; jeq L2176
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2176:
L2174:
movb #12,d5
movw #10,a6@(-24)
movl d5,d4; extbl d4
movl d4,d4
movw a6@(-24),d3
extl d3
movl d3,d3
eorl d3,d4
movb d4,d5
cmpb #6,d5; jeq L2178
movl #347,d6
tstl d7; jeq L2180
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2180:
L2178:
movb #12,d5
movl #10,a6@(-28)
movl d5,d4; extbl d4
movl d4,d4
movl a6@(-28),d3
eorl d3,d4
movb d4,d5
cmpb #6,d5; jeq L2182
movl #348,d6
tstl d7; jeq L2184
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2184:
L2182:
movb #12,d5
movl #10,a6@(-32)
movl d5,d4; extbl d4
movl d4,d4
movl a6@(-32),d3
eorl d3,d4
movb d4,d5
cmpb #6,d5; jeq L2186
movl #349,d6
tstl d7; jeq L2188
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2188:
L2186:
movb #12,d5
movl #10,a6@(-36)
movl d5,d4; extbl d4
movl d4,d4
movl a6@(-36),d3
eorl d3,d4
movb d4,d5
cmpb #6,d5; jeq L2190
movl #350,d6
tstl d7; jeq L2192
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2192:
L2190:
movw #12,a6@(-4)
movb #10,a6@(-20)
movw a6@(-4),d4
extl d4
movl d4,d4
movb a6@(-20),d3
extbl d3
movl d3,d3
eorl d3,d4
movw d4,a6@(-4)
cmpw #6,a6@(-4); jeq L2194
movl #351,d6
tstl d7; jeq L2196
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2196:
L2194:
movw #12,a6@(-4)
movw #10,a6@(-24)
movw a6@(-4),d4
extl d4
movl d4,d4
movw a6@(-24),d3
extl d3
movl d3,d3
eorl d3,d4
movw d4,a6@(-4)
cmpw #6,a6@(-4); jeq L2198
movl #352,d6
tstl d7; jeq L2200
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2200:
L2198:
movw #12,a6@(-4)
movl #10,a6@(-28)
movw a6@(-4),d4
extl d4
movl d4,d4
movl a6@(-28),d3
eorl d3,d4
movw d4,a6@(-4)
cmpw #6,a6@(-4); jeq L2202
movl #353,d6
tstl d7; jeq L2204
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2204:
L2202:
movw #12,a6@(-4)
movl #10,a6@(-32)
movw a6@(-4),d4
extl d4
movl d4,d4
movl a6@(-32),d3
eorl d3,d4
movw d4,a6@(-4)
cmpw #6,a6@(-4); jeq L2206
movl #354,d6
tstl d7; jeq L2208
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2208:
L2206:
movw #12,a6@(-4)
movl #10,a6@(-36)
movw a6@(-4),d4
extl d4
movl d4,d4
movl a6@(-36),d3
eorl d3,d4
movw d4,a6@(-4)
cmpw #6,a6@(-4); jeq L2210
movl #355,d6
tstl d7; jeq L2212
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2212:
L2210:
movl #12,a6@(-8)
movb #10,a6@(-20)
movl a6@(-8),d4
movb a6@(-20),d3
extbl d3
movl d3,d3
eorl d3,d4
movl d4,a6@(-8)
cmpl #6,a6@(-8); jeq L2214
movl #356,d6
tstl d7; jeq L2216
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2216:
L2214:
movl #12,a6@(-8)
movw #10,a6@(-24)
movl a6@(-8),d4
movw a6@(-24),d3
extl d3
movl d3,d3
eorl d3,d4
movl d4,a6@(-8)
cmpl #6,a6@(-8); jeq L2218
movl #357,d6
tstl d7; jeq L2220
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2220:
L2218:
movl #12,a6@(-8)
movl #10,a6@(-28)
movl a6@(-8),d4
movl a6@(-28),d3
eorl d3,d4
movl d4,a6@(-8)
cmpl #6,a6@(-8); jeq L2222
movl #358,d6
tstl d7; jeq L2224
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2224:
L2222:
movl #12,a6@(-8)
movl #10,a6@(-32)
movl a6@(-8),d4
movl a6@(-32),d3
eorl d3,d4
movl d4,a6@(-8)
cmpl #6,a6@(-8); jeq L2226
movl #359,d6
tstl d7; jeq L2228
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2228:
L2226:
movl #12,a6@(-8)
movl #10,a6@(-36)
movl a6@(-8),d4
movl a6@(-36),d3
eorl d3,d4
movl d4,a6@(-8)
cmpl #6,a6@(-8); jeq L2230
movl #360,d6
tstl d7; jeq L2232
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2232:
L2230:
movl #12,a6@(-12)
movb #10,a6@(-20)
movl a6@(-12),d4
movb a6@(-20),d3
extbl d3
movl d3,d3
eorl d3,d4
movl d4,a6@(-12)
cmpl #6,a6@(-12); jeq L2234
movl #361,d6
tstl d7; jeq L2236
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2236:
L2234:
movl #12,a6@(-12)
movw #10,a6@(-24)
movl a6@(-12),d4
movw a6@(-24),d3
extl d3
movl d3,d3
eorl d3,d4
movl d4,a6@(-12)
cmpl #6,a6@(-12); jeq L2238
movl #362,d6
tstl d7; jeq L2240
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2240:
L2238:
movl #12,a6@(-12)
movl #10,a6@(-28)
movl a6@(-12),d4
movl a6@(-28),d3
eorl d3,d4
movl d4,a6@(-12)
cmpl #6,a6@(-12); jeq L2242
movl #363,d6
tstl d7; jeq L2244
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2244:
L2242:
movl #12,a6@(-12)
movl #10,a6@(-32)
movl a6@(-12),d4
movl a6@(-32),d3
eorl d3,d4
movl d4,a6@(-12)
cmpl #6,a6@(-12); jeq L2246
movl #364,d6
tstl d7; jeq L2248
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2248:
L2246:
movl #12,a6@(-12)
movl #10,a6@(-36)
movl a6@(-12),d4
movl a6@(-36),d3
eorl d3,d4
movl d4,a6@(-12)
cmpl #6,a6@(-12); jeq L2250
movl #365,d6
tstl d7; jeq L2252
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2252:
L2250:
movl #12,a6@(-16)
movb #10,a6@(-20)
movl a6@(-16),d4
movb a6@(-20),d3
extbl d3
movl d3,d3
eorl d3,d4
movl d4,a6@(-16)
cmpl #6,a6@(-16); jeq L2254
movl #366,d6
tstl d7; jeq L2256
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2256:
L2254:
movl #12,a6@(-16)
movw #10,a6@(-24)
movl a6@(-16),d4
movw a6@(-24),d3
extl d3
movl d3,d3
eorl d3,d4
movl d4,a6@(-16)
cmpl #6,a6@(-16); jeq L2258
movl #367,d6
tstl d7; jeq L2260
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2260:
L2258:
movl #12,a6@(-16)
movl #10,a6@(-28)
movl a6@(-16),d4
movl a6@(-28),d3
eorl d3,d4
movl d4,a6@(-16)
cmpl #6,a6@(-16); jeq L2262
movl #368,d6
tstl d7; jeq L2264
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2264:
L2262:
movl #12,a6@(-16)
movl #10,a6@(-32)
movl a6@(-16),d4
movl a6@(-32),d3
eorl d3,d4
movl d4,a6@(-16)
cmpl #6,a6@(-16); jeq L2266
movl #369,d6
tstl d7; jeq L2268
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2268:
L2266:
movl #12,a6@(-16)
movl #10,a6@(-36)
movl a6@(-16),d4
movl a6@(-36),d3
eorl d3,d4
movl d4,a6@(-16)
cmpl #6,a6@(-16); jeq L2270
movl #370,d6
tstl d7; jeq L2272
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2272:
L2270:
movb #12,d5
movb #10,a6@(-20)
movl d5,d4; extbl d4
movl d4,d4
movb a6@(-20),d3
extbl d3
orl d3,d4
movb d4,d5
cmpb #14,d5; jeq L2274
movl #371,d6
tstl d7; jeq L2276
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2276:
L2274:
movb #12,d5
movw #10,a6@(-24)
movl d5,d4; extbl d4
movl d4,d4
movw a6@(-24),d3
extl d3
orl d3,d4
movb d4,d5
cmpb #14,d5; jeq L2278
movl #372,d6
tstl d7; jeq L2280
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2280:
L2278:
movb #12,d5
movl #10,a6@(-28)
movl d5,d4; extbl d4
movl d4,d4
orl a6@(-28),d4
movb d4,d5
cmpb #14,d5; jeq L2282
movl #373,d6
tstl d7; jeq L2284
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2284:
L2282:
movb #12,d5
movl #10,a6@(-32)
movl d5,d4; extbl d4
movl d4,d4
orl a6@(-32),d4
movb d4,d5
cmpb #14,d5; jeq L2286
movl #374,d6
tstl d7; jeq L2288
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2288:
L2286:
movb #12,d5
movl #10,a6@(-36)
movl d5,d4; extbl d4
movl d4,d4
orl a6@(-36),d4
movb d4,d5
cmpb #14,d5; jeq L2290
movl #375,d6
tstl d7; jeq L2292
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2292:
L2290:
movw #12,a6@(-4)
movb #10,a6@(-20)
movw a6@(-4),d4
extl d4
movl d4,d4
movb a6@(-20),d3
extbl d3
orl d3,d4
movw d4,a6@(-4)
cmpw #14,a6@(-4); jeq L2294
movl #376,d6
tstl d7; jeq L2296
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2296:
L2294:
movw #12,a6@(-4)
movw #10,a6@(-24)
movw a6@(-4),d4
extl d4
movl d4,d4
movw a6@(-24),d3
extl d3
orl d3,d4
movw d4,a6@(-4)
cmpw #14,a6@(-4); jeq L2298
movl #377,d6
tstl d7; jeq L2300
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2300:
L2298:
movw #12,a6@(-4)
movl #10,a6@(-28)
movw a6@(-4),d4
extl d4
movl d4,d4
orl a6@(-28),d4
movw d4,a6@(-4)
cmpw #14,a6@(-4); jeq L2302
movl #378,d6
tstl d7; jeq L2304
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2304:
L2302:
movw #12,a6@(-4)
movl #10,a6@(-32)
movw a6@(-4),d4
extl d4
movl d4,d4
orl a6@(-32),d4
movw d4,a6@(-4)
cmpw #14,a6@(-4); jeq L2306
movl #379,d6
tstl d7; jeq L2308
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2308:
L2306:
movw #12,a6@(-4)
movl #10,a6@(-36)
movw a6@(-4),d4
extl d4
movl d4,d4
orl a6@(-36),d4
movw d4,a6@(-4)
cmpw #14,a6@(-4); jeq L2310
movl #380,d6
tstl d7; jeq L2312
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2312:
L2310:
movl #12,a6@(-8)
movb #10,a6@(-20)
movl a6@(-8),d4
movb a6@(-20),d3
extbl d3
orl d3,d4
movl d4,a6@(-8)
cmpl #14,a6@(-8); jeq L2314
movl #381,d6
tstl d7; jeq L2316
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2316:
L2314:
movl #12,a6@(-8)
movw #10,a6@(-24)
movl a6@(-8),d4
movw a6@(-24),d3
extl d3
orl d3,d4
movl d4,a6@(-8)
cmpl #14,a6@(-8); jeq L2318
movl #382,d6
tstl d7; jeq L2320
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2320:
L2318:
movl #12,a6@(-8)
movl #10,a6@(-28)
movl a6@(-8),d4
orl a6@(-28),d4
movl d4,a6@(-8)
cmpl #14,a6@(-8); jeq L2322
movl #383,d6
tstl d7; jeq L2324
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2324:
L2322:
movl #12,a6@(-8)
movl #10,a6@(-32)
movl a6@(-8),d4
orl a6@(-32),d4
movl d4,a6@(-8)
cmpl #14,a6@(-8); jeq L2326
movl #384,d6
tstl d7; jeq L2328
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2328:
L2326:
movl #12,a6@(-8)
movl #10,a6@(-36)
movl a6@(-8),d4
orl a6@(-36),d4
movl d4,a6@(-8)
cmpl #14,a6@(-8); jeq L2330
movl #385,d6
tstl d7; jeq L2332
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2332:
L2330:
movl #12,a6@(-12)
movb #10,a6@(-20)
movl a6@(-12),d4
movb a6@(-20),d3
extbl d3
orl d3,d4
movl d4,a6@(-12)
cmpl #14,a6@(-12); jeq L2334
movl #386,d6
tstl d7; jeq L2336
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2336:
L2334:
movl #12,a6@(-12)
movw #10,a6@(-24)
movl a6@(-12),d4
movw a6@(-24),d3
extl d3
orl d3,d4
movl d4,a6@(-12)
cmpl #14,a6@(-12); jeq L2338
movl #387,d6
tstl d7; jeq L2340
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2340:
L2338:
movl #12,a6@(-12)
movl #10,a6@(-28)
movl a6@(-12),d4
orl a6@(-28),d4
movl d4,a6@(-12)
cmpl #14,a6@(-12); jeq L2342
movl #388,d6
tstl d7; jeq L2344
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2344:
L2342:
movl #12,a6@(-12)
movl #10,a6@(-32)
movl a6@(-12),d4
orl a6@(-32),d4
movl d4,a6@(-12)
cmpl #14,a6@(-12); jeq L2346
movl #389,d6
tstl d7; jeq L2348
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2348:
L2346:
movl #12,a6@(-12)
movl #10,a6@(-36)
movl a6@(-12),d4
orl a6@(-36),d4
movl d4,a6@(-12)
cmpl #14,a6@(-12); jeq L2350
movl #390,d6
tstl d7; jeq L2352
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2352:
L2350:
movl #12,a6@(-16)
movb #10,a6@(-20)
movl a6@(-16),d4
movb a6@(-20),d3
extbl d3
orl d3,d4
movl d4,a6@(-16)
cmpl #14,a6@(-16); jeq L2354
movl #391,d6
tstl d7; jeq L2356
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2356:
L2354:
movl #12,a6@(-16)
movw #10,a6@(-24)
movl a6@(-16),d4
movw a6@(-24),d3
extl d3
orl d3,d4
movl d4,a6@(-16)
cmpl #14,a6@(-16); jeq L2358
movl #392,d6
tstl d7; jeq L2360
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2360:
L2358:
movl #12,a6@(-16)
movl #10,a6@(-28)
movl a6@(-16),d4
orl a6@(-28),d4
movl d4,a6@(-16)
cmpl #14,a6@(-16); jeq L2362
movl #393,d6
tstl d7; jeq L2364
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2364:
L2362:
movl #12,a6@(-16)
movl #10,a6@(-32)
movl a6@(-16),d4
orl a6@(-32),d4
movl d4,a6@(-16)
cmpl #14,a6@(-16); jeq L2366
movl #394,d6
tstl d7; jeq L2368
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2368:
L2366:
movl #12,a6@(-16)
movl #10,a6@(-36)
movl a6@(-16),d4
orl a6@(-36),d4
movl d4,a6@(-16)
cmpl #14,a6@(-16); jeq L2370
movl #395,d6
tstl d7; jeq L2372
movl #L694,sp@
movl d6,sp@(4)
jbsr _printf:l
L2372:
L2370:
tstl d6; jeq L2374
movl #1,a6@(-40)
movl a6@(8),a2
tstl a2@(44); jeq L2376
movl #L695,sp@
movl #1,sp@(4)
jbsr _printf:l
L2376:
L2374:
movl a6@(-40),d0
L693:
moveml a6@(-352),#0x34f8
fmovemx a6@(-320),#0xff
unlk a6
rts
.data
L2379:.ascii "s715,er%d\012\000"
L2380:.ascii "s715   \000"
.text
.globl _s715
.align 2
_s715:link a6,#-48
moveml #0x34d0,a6@(-36)
clrl a6@(-12)
clrl a6@(-4)
lea L2380:l,a5
movl a6@(8),a2
lea a2@(60),a4
clrl d6
L2381:
L2382:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L2381
movl #1,d7
addql #1,d7
addql #1,d7
addql #1,d7
addql #1,d7
movl d7,d4; addql #1,d4
movl d4,d7
cmpl #6,d4; jeq L2384
movl a6@(8),a2
tstl a2@(44); jeq L2386
movl #L2379,sp@
movl #1,sp@(4)
jbsr _printf:l
L2386:
addql #1,d6
L2384:
movl a6@(-4),sp@
movl #3,a6@(-8)
movl a6@(-8),d4
addql #2,d4
movl d4,sp@(4)
movl a6@(-12),sp@(8)
jbsr _s715f:l
cmpl #5,d0; jeq L2388
movl a6@(8),a2
tstl a2@(44); jeq L2390
movl #L2379,sp@
movl #2,sp@(4)
jbsr _printf:l
L2390:
addql #2,d6
L2388:
movl d6,d0
L2378:
moveml a6@(-36),#0x34d0
unlk a6
rts
.globl _s715f
.align 2
_s715f:link a6,#-0
movl a6@(12),d0
L2392:
unlk a6
rts
.data
L2394:.ascii "s72,er%d\012\000"
L2395:.ascii "s72    \000"
.text
.globl _s72
.align 2
_s72:link a6,#-148
fmovemx #0xb7,a6@(-104)
moveml #0x3cf8,a6@(-140)
movl a6@(8),a5
lea L2395:l,a4
lea a5@(60),a3
clrl d5
L2396:
L2397:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L2396
movl #2,a6@(-4)
cmpl #2,a6@(-4); jeq L2399
addql #1,d5
movl #L2394,sp@
movl #1,sp@(4)
jbsr _printf:l
L2399:
movl a6@(-4),d4
movl d4,d3; negl d3
addl d3,d4
tstl d4; jeq L2401
addql #2,d5
movl #L2394,sp@
movl #2,sp@(4)
jbsr _printf:l
L2401:
clrl a6@(-4)
clrl d7
jra L2406
L2403:
movl a6@(-4),d4
asll #1,d4
movl d4,d4
orl #1,d4
movl d4,a6@(-4)
L2404:
addql #1,d7
L2406:
cmpl a5@(4),d7; jlt L2403
movl a6@(-4),d4
notl d4
tstl d4; jeq L2407
addql #4,d5
movl #L2394,sp@
movl #4,sp@(4)
jbsr _printf:l
L2407:
movl #5,a6@(-4)
movl a6@(-4),d4
addql #1,d4
movl d4,a6@(-4)
cmpl #6,d4; jne L2414
movl a6@(-4),d4
subql #1,d4
movl d4,a6@(-4)
cmpl #5,d4; jne L2414
movl a6@(-4),d4
movl d4,d3; addql #1,d3
movl d3,a6@(-4)
cmpl #5,d4; jne L2414
movl a6@(-4),d4
movl d4,d3; subql #1,d3
movl d3,a6@(-4)
cmpl #6,d4; jne L2414
cmpl #5,a6@(-4); jeq L2409
L2414:
addql #8,d5
movl #L2394,sp@
movl #8,sp@(4)
jbsr _printf:l
L2409:
movb #26,a6@(-12)
movl #26,a6@(-20)
fmoved L2415:l,fp7
movw #26,a6@(-16)
movl #26,a6@(-24)
movl #26,a6@(-8)
fmoves L2416:l,fp6
clrl d6
movw a6@(-16),d4
extl d4
cmpb #26,d4; jne L2423
movl a6@(-8),d4
cmpb #26,d4; jne L2423
movl a6@(-20),d4
cmpb #26,d4; jne L2423
movl a6@(-24),d4
cmpb #26,d4; jne L2423
fintrzx fp6,fp0; fmovel fp0,d4
cmpb #26,d4; jne L2423
fintrzx fp7,fp0; fmovel fp0,d4
cmpb #26,d4; jeq L2417
L2423:
addql #1,d6
L2417:
movb a6@(-12),d4
extbl d4
cmpw #26,d4; jne L2430
movl a6@(-8),d4
cmpw #26,d4; jne L2430
movl a6@(-20),d4
cmpw #26,d4; jne L2430
movl a6@(-24),d4
cmpw #26,d4; jne L2430
fintrzx fp6,fp0; fmovel fp0,d4
cmpw #26,d4; jne L2430
fintrzx fp7,fp0; fmovel fp0,d4
cmpw #26,d4; jeq L2424
L2430:
addql #2,d6
L2424:
cmpb #26,a6@(-12); jne L2437
cmpw #26,a6@(-16); jne L2437
cmpl #26,a6@(-20); jne L2437
cmpl #26,a6@(-24); jne L2437
fintrzx fp6,fp0; fmovel fp0,d4
cmpl #26,d4; jne L2437
fintrzx fp7,fp0; fmovel fp0,d4
cmpl #26,d4; jeq L2431
L2437:
addql #4,d6
L2431:
cmpb #26,a6@(-12); jne L2444
cmpw #26,a6@(-16); jne L2444
cmpl #26,a6@(-8); jne L2444
cmpl #26,a6@(-24); jne L2444
fintrzx fp6,fp0; fmovel fp0,d4
cmpl #26,d4; jne L2444
fintrzx fp7,fp0; fmovel fp0,d4
cmpl #26,d4; jeq L2438
L2444:
addql #8,d6
L2438:
cmpb #26,a6@(-12); jne L2453
cmpw #26,a6@(-16); jne L2453
cmpl #26,a6@(-8); jne L2453
cmpl #26,a6@(-20); jne L2453
fmoved L835:l,fp3
fcmpx fp3,fp6; fjlt L2454
fmovex fp6,fp2; fsubx fp3,fp2
fintrzx fp2,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-28)
jra L2455
L2454:
fintrzx fp6,fp0; fmovel fp0,d4
movl d4,a6@(-28)
L2455:
cmpl #26,a6@(-28); jne L2453
fmoved L835:l,fp3
fcmpx fp3,fp7; fjlt L2456
fmovex fp7,fp2; fsubx fp3,fp2
fintrzx fp2,fp0; fmovel fp0,d4
movl d4,d4
addl #0x80000000,d4
movl d4,a6@(-32)
jra L2457
L2456:
fintrzx fp7,fp0; fmovel fp0,d4
movl d4,a6@(-32)
L2457:
cmpl #26,a6@(-32); jeq L2445
L2453:
addl #16,d6
L2445:
movb a6@(-12),d4
extbl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fmoved L2415:l,fp2
fcmpx fp2,fp3; fjne L2465
movw a6@(-16),d4
extl d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fcmpx fp2,fp3; fjne L2465
movl a6@(-8),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fcmpx fp2,fp3; fjne L2465
movl a6@(-20),d4
fmovel d4,fp3
fmoves fp3,d0; fmoves d0,fp3
fcmpx fp2,fp3; fjne L2465
movl a6@(-24),d4
tstl d4; jlt L2466
fmovel d4,fp5
jra L2467
L2466:
fmovel d4,fp3
faddd L600:l,fp3
fmovex fp3,fp5
L2467:
fmoves fp5,d0; fmoves d0,fp3
fmoved L2415:l,fp2
fcmpx fp2,fp3; fjne L2465
fmoves fp7,d0; fmoves d0,fp3
fcmpx fp2,fp3; fjeq L2458
L2465:
addl #32,d6
L2458:
movb a6@(-12),d4
extbl d4
fmovel d4,fp3
fmoved L2415:l,fp2
fcmpx fp2,fp3; fjne L2475
movw a6@(-16),d4
extl d4
fmovel d4,fp3
fcmpx fp2,fp3; fjne L2475
movl a6@(-8),d4
fmovel d4,fp3
fcmpx fp2,fp3; fjne L2475
movl a6@(-20),d4
fmovel d4,fp3
fcmpx fp2,fp3; fjne L2475
movl a6@(-24),d4
tstl d4; jlt L2476
fmovel d4,fp5
jra L2477
L2476:
fmovel d4,fp3
faddd L600:l,fp3
fmovex fp3,fp5
L2477:
fmoved L2415:l,fp3
fcmpx fp3,fp5; fjne L2475
fcmpx fp3,fp6; fjeq L2468
L2475:
addl #64,d6
L2468:
tstl d6; jeq L2478
addl #16,d5
movl #L2394,sp@
movl #16,sp@(4)
jbsr _printf:l
L2478:
movl d5,d0
L2393:
moveml a6@(-140),#0x3cf8
fmovemx a6@(-104),#0xb7
unlk a6
rts
.data
L2481:.ascii "s757,er%d\012\000"
L2482:.ascii "s757   \000"
.text
.globl _s757
.align 2
_s757:link a6,#-176
moveml #0x3cfc,a6@(-168)
movl a6@(8),a5
lea L2482:l,a4
lea a5@(60),a3
clrl a6@(-100)
L2483:
L2484:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L2483
movl #40,a6@(-104)
movl a6@(-104),d4
movl d4,d3; asll #3,d3
asll #2,d3
cmpl #1280,d3; jne L2488
asrl #3,d4
asrl #2,d4
cmpl #1,d4; jeq L2486
L2488:
addql #1,a6@(-100)
tstl a5@(44); jeq L2489
movl #L2481,sp@
movl #1,sp@(4)
jbsr _printf:l
L2489:
L2486:
clrl a6@(-72)
clrl a6@(-88)
jra L2494
L2491:
movl #1,a6@(-80)
movl a6@(-80),d4
movl a5@(16),d3
subql #1,d3
lsll d3,d4
movl d4,a6@(-84)
clrl a6@(-92)
movl a6@(-92),d4
notl d4
movl a6@(-88),d3
movl d4,d2; lsrl d3,d2
movl d2,a6@(-96)
lsll d3,d4
movl d4,a6@(-92)
clrl a6@(-76)
jra L2498
L2495:
movl a6@(-76),d4
cmpl a6@(-88),d4; jge L2506
movl #1,a6@(-112)
jra L2507
L2506:
clrl a6@(-112)
L2507:
movl a6@(-80),d4
andl a6@(-92),d4
tstl d4; jne L2508
movl #1,a6@(-116)
jra L2509
L2508:
clrl a6@(-116)
L2509:
movl a6@(-112),d4
cmpl a6@(-116),d4; jne L2505
movl a6@(-76),d4
cmpl a6@(-88),d4; jge L2510
movl #1,a6@(-120)
jra L2511
L2510:
clrl a6@(-120)
L2511:
movl a6@(-84),d4
andl a6@(-96),d4
tstl d4; jne L2512
movl #1,a6@(-124)
jra L2513
L2512:
clrl a6@(-124)
L2513:
movl a6@(-120),d4
cmpl a6@(-124),d4; jeq L2499
L2505:
movl #1,a6@(-72)
L2499:
movl a6@(-80),d4
lsll #1,d4
movl d4,a6@(-80)
movl a6@(-84),d4
lsrl #1,d4
movl d4,a6@(-84)
L2496:
addql #1,a6@(-76)
L2498:
movl a6@(-76),d4
cmpl a5@(16),d4; jlt L2495
L2492:
addql #1,a6@(-88)
L2494:
movl a6@(-88),d4
cmpl a5@(16),d4; jlt L2491
tstl a6@(-72); jeq L2514
addql #2,a6@(-100)
tstl a5@(44); jeq L2516
movl #L2481,sp@
movl #2,sp@(4)
jbsr _printf:l
L2516:
L2514:
movl #3,a6@(-4)
movl #2,d5
movl #1,d6
movl a6@(-4),d4
cmpl d5,d4; jge L2524
movl #1,a6@(-116)
jra L2525
L2524:
clrl a6@(-116)
L2525:
movl a6@(-116),d4
cmpl d6,d4; jge L2522
movl #1,a6@(-112)
jra L2523
L2522:
clrl a6@(-112)
L2523:
cmpl #1,a6@(-112); jeq L2518
addql #4,a6@(-100)
tstl a5@(44); jeq L2526
movl #L2481,sp@
movl #4,sp@(4)
jbsr _printf:l
L2526:
L2518:
lea a6@(-64),a2
movl a2,d4
lea a6@(-68),a2
cmpl a2,d4; jne L2528
addql #8,a6@(-100)
tstl a5@(44); jeq L2531
movl #L2481,sp@
movl #8,sp@(4)
jbsr _printf:l
L2531:
L2528:
lea a6@(-64),a2
movl a2,d4
lea a6@(-68),a2
cmpl a2,d4; jcc L2533
tstl a5@(40); jeq L2536
movl #L2538,sp@
jbsr _printf:l
L2536:
L2533:
clrl a6@(-72)
clrl a6@(-76)
L2539:
movl a6@(-76),d4
asll #2,d4
movl #1,a6@(-68,d4:l:1)
L2540:
addql #1,a6@(-76)
cmpl #16,a6@(-76); jlt L2539
clrl a6@(-64)
clrl a6@(-52)
clrl a6@(-44)
clrl a6@(-40)
clrl a6@(-32)
clrl a6@(-16)
clrl a6@(-4)
L2549:
clrl d5
L2553:
clrl d6
L2557:
clrl d7
L2561:
movl a6@(-4),d4
cmpl d5,d4; jge L2572
movl #1,a6@(-124)
jra L2573
L2572:
clrl a6@(-124)
L2573:
cmpl d7,d6; jge L2574
movl #1,a6@(-128)
jra L2575
L2574:
clrl a6@(-128)
L2575:
movl a6@(-124),d4
cmpl a6@(-128),d4; jne L2570
movl #1,a6@(-120)
jra L2571
L2570:
clrl a6@(-120)
L2571:
movl a6@(-120),d4
movl a6@(-4),d3
asll #3,d3
movl d5,d2; asll #2,d2
addl d2,d3
movl d6,d2; asll #1,d2
addl d2,d3
addl d7,d3
asll #2,d3
cmpl a6@(-68,d3:l:1),d4; jeq L2565
movl #1,a6@(-72)
L2565:
L2562:
addql #1,d7
cmpl #2,d7; jlt L2561
L2558:
addql #1,d6
cmpl #2,d6; jlt L2557
L2554:
addql #1,d5
cmpl #2,d5; jlt L2553
L2550:
addql #1,a6@(-4)
cmpl #2,a6@(-4); jlt L2549
tstl a6@(-72); jeq L2576
addl #16,a6@(-100)
tstl a5@(44); jeq L2578
movl #L2481,sp@
movl #16,sp@(4)
jbsr _printf:l
L2578:
L2576:
clrl a6@(-108)
tstl a6@(-108); jeq L2580
addl #32,a6@(-100)
tstl a5@(44); jeq L2582
movl #L2481,sp@
movl #32,sp@(4)
jbsr _printf:l
L2582:
L2580:
movl a6@(-100),d0
L2480:
moveml a6@(-168),#0x3cfc
unlk a6
rts
.data
L2585:.ascii "Local error %d.\012\000"
L2586:.ascii "s7813,er%d\012\000"
L2587:.ascii "s7813  \000"
.text
.globl _s7813
.align 2
_s7813:link a6,#-108
moveml #0x34f8,a6@(-100)
lea L2587:l,a5
movl a6@(8),a2
lea a2@(60),a4
clrl d6
clrl a6@(-12)
movl a2@(48),d7
L2588:
L2589:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L2588
jra L2591
movl #1,d6
tstl d7; jeq L2593
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2593:
L2591:
jra L2595
movl #2,d6
tstl d7; jeq L2597
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2597:
L2595:
jra L2599
movl #3,d6
tstl d7; jeq L2601
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2601:
L2599:
jra L2603
movl #4,d6
tstl d7; jeq L2605
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2605:
L2603:
jra L2607
movl #5,d6
tstl d7; jeq L2609
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2609:
L2607:
jra L2611
movl #6,d6
tstl d7; jeq L2613
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2613:
L2611:
jra L2615
movl #7,d6
tstl d7; jeq L2617
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2617:
L2615:
jra L2619
movl #8,d6
tstl d7; jeq L2621
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2621:
L2619:
jra L2623
movl #9,d6
tstl d7; jeq L2625
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2625:
L2623:
jra L2627
movl #10,d6
tstl d7; jeq L2629
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2629:
L2627:
jra L2631
movl #11,d6
tstl d7; jeq L2633
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2633:
L2631:
jra L2635
movl #12,d6
tstl d7; jeq L2637
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2637:
L2635:
jra L2639
movl #13,d6
tstl d7; jeq L2641
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2641:
L2639:
jra L2643
movl #14,d6
tstl d7; jeq L2645
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2645:
L2643:
jra L2647
movl #15,d6
tstl d7; jeq L2649
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2649:
L2647:
jra L2651
movl #16,d6
tstl d7; jeq L2653
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2653:
L2651:
jra L2655
movl #17,d6
tstl d7; jeq L2657
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2657:
L2655:
jra L2659
movl #18,d6
tstl d7; jeq L2661
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2661:
L2659:
jra L2663
movl #16,d6
tstl d7; jeq L2665
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2665:
L2663:
jra L2667
movl #20,d6
tstl d7; jeq L2669
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2669:
L2667:
jra L2671
movl #21,d6
tstl d7; jeq L2673
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2673:
L2671:
jra L2675
movl #22,d6
tstl d7; jeq L2677
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2677:
L2675:
jra L2679
movl #23,d6
tstl d7; jeq L2681
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2681:
L2679:
jra L2683
movl #24,d6
tstl d7; jeq L2685
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2685:
L2683:
jra L2687
movl #25,d6
tstl d7; jeq L2689
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2689:
L2687:
jra L2691
movl #26,d6
tstl d7; jeq L2693
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2693:
L2691:
jra L2695
movl #27,d6
tstl d7; jeq L2697
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2697:
L2695:
jra L2699
movl #28,d6
tstl d7; jeq L2701
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2701:
L2699:
jra L2703
movl #26,d6
tstl d7; jeq L2705
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2705:
L2703:
jra L2707
movl #30,d6
tstl d7; jeq L2709
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2709:
L2707:
jra L2711
movl #31,d6
tstl d7; jeq L2713
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2713:
L2711:
jra L2715
movl #32,d6
tstl d7; jeq L2717
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2717:
L2715:
jra L2719
movl #33,d6
tstl d7; jeq L2721
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2721:
L2719:
jra L2723
movl #34,d6
tstl d7; jeq L2725
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2725:
L2723:
jra L2727
movl #35,d6
tstl d7; jeq L2729
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2729:
L2727:
jra L2731
movl #36,d6
tstl d7; jeq L2733
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2733:
L2731:
jra L2735
movl #37,d6
tstl d7; jeq L2737
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2737:
L2735:
jra L2739
movl #38,d6
tstl d7; jeq L2741
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2741:
L2739:
jra L2743
movl #39,d6
tstl d7; jeq L2745
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2745:
L2743:
jra L2747
movl #40,d6
tstl d7; jeq L2749
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2749:
L2747:
jra L2751
movl #41,d6
tstl d7; jeq L2753
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2753:
L2751:
jra L2755
movl #42,d6
tstl d7; jeq L2757
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2757:
L2755:
jra L2759
movl #43,d6
tstl d7; jeq L2761
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2761:
L2759:
jra L2763
movl #44,d6
tstl d7; jeq L2765
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2765:
L2763:
jra L2767
movl #45,d6
tstl d7; jeq L2769
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2769:
L2767:
jra L2771
movl #46,d6
tstl d7; jeq L2773
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2773:
L2771:
jra L2775
movl #47,d6
tstl d7; jeq L2777
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2777:
L2775:
jra L2779
movl #48,d6
tstl d7; jeq L2781
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2781:
L2779:
jra L2783
movl #49,d6
tstl d7; jeq L2785
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2785:
L2783:
jra L2787
movl #50,d6
tstl d7; jeq L2789
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2789:
L2787:
jra L2791
movl #51,d6
tstl d7; jeq L2793
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2793:
L2791:
jra L2795
movl #52,d6
tstl d7; jeq L2797
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2797:
L2795:
jra L2799
movl #53,d6
tstl d7; jeq L2801
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2801:
L2799:
jra L2803
movl #54,d6
tstl d7; jeq L2805
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2805:
L2803:
jra L2807
movl #55,d6
tstl d7; jeq L2809
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2809:
L2807:
jra L2811
movl #56,d6
tstl d7; jeq L2813
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2813:
L2811:
jra L2815
movl #57,d6
tstl d7; jeq L2817
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2817:
L2815:
jra L2819
movl #58,d6
tstl d7; jeq L2821
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2821:
L2819:
jra L2823
movl #56,d6
tstl d7; jeq L2825
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2825:
L2823:
jra L2827
movl #60,d6
tstl d7; jeq L2829
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2829:
L2827:
jra L2831
movl #61,d6
tstl d7; jeq L2833
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2833:
L2831:
jra L2835
movl #62,d6
tstl d7; jeq L2837
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2837:
L2835:
jra L2839
movl #63,d6
tstl d7; jeq L2841
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2841:
L2839:
jra L2843
movl #64,d6
tstl d7; jeq L2845
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2845:
L2843:
jra L2847
movl #65,d6
tstl d7; jeq L2849
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2849:
L2847:
jra L2851
movl #66,d6
tstl d7; jeq L2853
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2853:
L2851:
jra L2855
movl #67,d6
tstl d7; jeq L2857
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2857:
L2855:
jra L2859
movl #68,d6
tstl d7; jeq L2861
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2861:
L2859:
jra L2863
movl #69,d6
tstl d7; jeq L2865
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2865:
L2863:
jra L2867
movl #70,d6
tstl d7; jeq L2869
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2869:
L2867:
jra L2871
movl #71,d6
tstl d7; jeq L2873
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2873:
L2871:
jra L2875
movl #72,d6
tstl d7; jeq L2877
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2877:
L2875:
jra L2879
movl #73,d6
tstl d7; jeq L2881
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2881:
L2879:
jra L2883
movl #74,d6
tstl d7; jeq L2885
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2885:
L2883:
jra L2887
movl #75,d6
tstl d7; jeq L2889
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2889:
L2887:
tstl d6; jeq L2891
movl a6@(8),a2
tstl a2@(44); jeq L2893
movl #L2586,sp@
movl #1,sp@(4)
jbsr _printf:l
L2893:
addql #1,a6@(-12)
L2891:
clrl d6
clrl a6@(-4)
clrl d5
movl d5,d4
movl d4,d3; addql #1,d3
movl d3,d5
tstl d4; jeq L2896
movl a6@(-4),d4
movl d4,d3; addql #1,d3
movl d3,a6@(-4)
tstl d4; jeq L2896
movl #1,a6@(-24)
jra L2897
L2896:
clrl a6@(-24)
L2897:
movl a6@(-24),a6@(-8)
cmpl #1,d5; jeq L2898
movl #1,d6
tstl d7; jeq L2900
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2900:
L2898:
tstl a6@(-4); jeq L2902
movl #2,d6
tstl d7; jeq L2904
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2904:
L2902:
tstl a6@(-8); jeq L2906
movl #3,d6
tstl d7; jeq L2908
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2908:
L2906:
tstl d5; jeq L2911
movl a6@(-4),d4
movl d4,d3; addql #1,d3
movl d3,a6@(-4)
tstl d4; jeq L2911
movl #1,a6@(-28)
jra L2912
L2911:
clrl a6@(-28)
L2912:
movl a6@(-28),a6@(-8)
cmpl #1,d5; jeq L2913
movl #4,d6
tstl d7; jeq L2915
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2915:
L2913:
cmpl #1,a6@(-4); jeq L2917
movl #5,d6
tstl d7; jeq L2919
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2919:
L2917:
tstl a6@(-8); jeq L2921
movl #6,d6
tstl d7; jeq L2923
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2923:
L2921:
movl d5,d4
movl d4,d3; subql #1,d3
movl d3,d5
tstl d4; jeq L2926
tstl a6@(-4); jeq L2926
movl #1,a6@(-32)
jra L2927
L2926:
clrl a6@(-32)
L2927:
movl a6@(-32),a6@(-8)
tstl d5; jeq L2928
movl #7,d6
tstl d7; jeq L2930
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2930:
L2928:
cmpl #1,a6@(-4); jeq L2932
movl #8,d6
tstl d7; jeq L2934
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2934:
L2932:
cmpl #1,a6@(-8); jeq L2936
movl #9,d6
tstl d7; jeq L2938
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2938:
L2936:
tstl d5; jeq L2941
movl a6@(-4),d4
movl d4,d3; subql #1,d3
movl d3,a6@(-4)
tstl d4; jeq L2941
movl #1,a6@(-36)
jra L2942
L2941:
clrl a6@(-36)
L2942:
movl a6@(-36),a6@(-8)
tstl d5; jeq L2943
movl #10,d6
tstl d7; jeq L2945
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2945:
L2943:
cmpl #1,a6@(-4); jeq L2947
movl #11,d6
tstl d7; jeq L2949
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2949:
L2947:
tstl a6@(-8); jeq L2951
movl #12,d6
tstl d7; jeq L2953
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2953:
L2951:
tstl d6; jeq L2955
movl a6@(8),a2
tstl a2@(44); jeq L2957
movl #L2586,sp@
movl #2,sp@(4)
jbsr _printf:l
L2957:
addql #2,a6@(-12)
L2955:
clrl d6
clrl a6@(-4)
clrl d5
movl d5,d4
movl d4,d3; addql #1,d3
movl d3,d5
tstl d4; jne L2962
tstl a6@(-4); jeq L2960
L2962:
movl #1,a6@(-40)
jra L2961
L2960:
clrl a6@(-40)
L2961:
movl a6@(-40),a6@(-8)
cmpl #1,d5; jeq L2963
movl #1,d6
tstl d7; jeq L2965
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2965:
L2963:
tstl a6@(-4); jeq L2967
movl #2,d6
tstl d7; jeq L2969
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2969:
L2967:
tstl a6@(-8); jeq L2971
movl #3,d6
tstl d7; jeq L2973
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2973:
L2971:
movl a6@(-4),d4
movl d4,d3; addql #1,d3
movl d3,a6@(-4)
tstl d4; jne L2978
tstl d5; jeq L2976
L2978:
movl #1,a6@(-44)
jra L2977
L2976:
clrl a6@(-44)
L2977:
movl a6@(-44),a6@(-8)
cmpl #1,d5; jeq L2979
movl #4,d6
tstl d7; jeq L2981
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2981:
L2979:
cmpl #1,a6@(-4); jeq L2983
movl #5,d6
tstl d7; jeq L2985
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2985:
L2983:
cmpl #1,a6@(-8); jeq L2987
movl #6,d6
tstl d7; jeq L2989
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2989:
L2987:
movl d5,d4
movl d4,d3; subql #1,d3
movl d3,d5
tstl d4; jne L2994
movl a6@(-4),d4
movl d4,d3; subql #1,d3
movl d3,a6@(-4)
tstl d4; jeq L2992
L2994:
movl #1,a6@(-48)
jra L2993
L2992:
clrl a6@(-48)
L2993:
movl a6@(-48),a6@(-8)
tstl d5; jeq L2995
movl #7,d6
tstl d7; jeq L2997
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L2997:
L2995:
cmpl #1,a6@(-4); jeq L2999
movl #8,d6
tstl d7; jeq L3001
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L3001:
L2999:
cmpl #1,a6@(-8); jeq L3003
movl #9,d6
tstl d7; jeq L3005
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L3005:
L3003:
tstl d5; jne L3010
movl a6@(-4),d4
movl d4,d3; subql #1,d3
movl d3,a6@(-4)
tstl d4; jeq L3008
L3010:
movl #1,a6@(-52)
jra L3009
L3008:
clrl a6@(-52)
L3009:
movl a6@(-52),a6@(-8)
tstl d5; jeq L3011
movl #10,d6
tstl d7; jeq L3013
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L3013:
L3011:
tstl a6@(-4); jeq L3015
movl #11,d6
tstl d7; jeq L3017
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L3017:
L3015:
cmpl #1,a6@(-8); jeq L3019
movl #12,d6
tstl d7; jeq L3021
movl #L2585,sp@
movl d6,sp@(4)
jbsr _printf:l
L3021:
L3019:
tstl d6; jeq L3023
movl a6@(8),a2
tstl a2@(44); jeq L3025
movl #L2586,sp@
movl #4,sp@(4)
jbsr _printf:l
L3025:
addql #4,a6@(-12)
L3023:
clrl a6@(-4)
clrl d5
clrl a6@(-16)
movl #1,a6@(-20)
tstl a6@(-20); jeq L3029
movl a6@(-16),a6@(-56)
jra L3030
L3029:
tstl a6@(-20); jeq L3031
movl d5,d4
movl d4,d3; addql #1,d3
movl d3,d5
movl d4,a6@(-60)
jra L3032
L3031:
movl a6@(-4),d4
movl d4,d3; addql #1,d3
movl d3,a6@(-4)
movl d4,a6@(-60)
L3032:
movl a6@(-60),a6@(-56)
L3030:
movl a6@(-56),a6@(-8)
tstl a6@(-8); jne L3036
tstl d5; jne L3036
tstl a6@(-4); jeq L3033
L3036:
movl a6@(8),a2
tstl a2@(44); jeq L3037
movl #L2586,sp@
movl #8,sp@(4)
jbsr _printf:l
L3037:
addql #8,a6@(-12)
L3033:
tstl a6@(-20); jeq L3044
movl a6@(-16),a6@(-64)
jra L3045
L3044:
movl #1,a6@(-64)
L3045:
tstl a6@(-64); jne L3043
tstl a6@(-16); jeq L3046
movl #1,a6@(-68)
jra L3047
L3046:
movl a6@(-16),a6@(-68)
L3047:
tstl a6@(-68); jeq L3039
L3043:
movl a6@(8),a2
tstl a2@(44); jeq L3048
movl #L2586,sp@
movl #16,sp@(4)
jbsr _printf:l
L3048:
addl #16,a6@(-12)
L3039:
movl a6@(-12),d0
L2584:
moveml a6@(-100),#0x34f8
unlk a6
rts
.data
L3051:.ascii "s81,er%d\012\000"
L3052:.ascii "s81    \000"
L3053:.ascii "Register count for %s is unreliable.\012\000"
L3054:.ascii "%d registers assigned to %s variables.\012\000"
.text
.globl _s81
.align 2
_s81:link a6,#-76
moveml #0x3cf0,a6@(-64)
clrl a6@(-32)
clrl a6@(-20)
clrl a6@(-24)
clrl a6@(-28)
lea L3052:l,a3
movl a6@(8),a2
lea a2@(60),a2
movl a2,a6@(-16)
L3055:
L3056:
movl a6@(-16),a2
lea a2@(1),a1
movl a1,a6@(-16)
movl a3,a1
lea a1@(1),a3
movb a1@,d4
movb d4,a2@
tstb d4; jne L3055
movl #1,a6@(-4)
clrl d5
L3058:
movl a6@(-4),d4
movb d4,d7
movb d4,a6@(-8)
lea a6@(-4),a2
movl a2,a5
movl a2,a4
movl d4,d6
movl d4,a6@(-12)
cmpb a6@(-8),d7; jeq L3062
movl #1,a6@(-20)
L3062:
movl a5,d4
cmpl a4,d4; jeq L3064
movl #1,a6@(-24)
L3064:
cmpl a6@(-12),d6; jeq L3066
movl #1,a6@(-28)
L3066:
movl a6@(-4),d4
asll #1,d4
movl d4,a6@(-4)
L3059:
addql #1,d5
cmpl #50,d5; jlt L3058
tstl a6@(-20); jeq L3068
addql #1,a6@(-32)
movl a6@(8),a2
tstl a2@(44); jeq L3070
movl #L3051,sp@
movl #1,sp@(4)
jbsr _printf:l
L3070:
L3068:
tstl a6@(-24); jeq L3072
addql #2,a6@(-32)
movl a6@(8),a2
tstl a2@(44); jeq L3074
movl #L3051,sp@
movl #2,sp@(4)
jbsr _printf:l
L3074:
L3072:
tstl a6@(-28); jeq L3076
addql #4,a6@(-32)
movl a6@(8),a2
tstl a2@(44); jeq L3078
movl #L3051,sp@
movl #4,sp@(4)
jbsr _printf:l
L3078:
L3076:
jbsr _regc:l
movl d0,a6@(-4)
movl a6@(8),a2
tstl a2@(40); jeq L3080
tstl a6@(-4); jge L3082
movl #L3053,sp@
movl #L425,sp@(4)
jbsr _printf:l
jra L3083
L3082:
movl #L3054,sp@
movl a6@(-4),sp@(4)
movl #L425,sp@(8)
jbsr _printf:l
L3083:
L3080:
jbsr _regp:l
movl d0,a6@(-4)
movl a6@(8),a2
tstl a2@(40); jeq L3084
tstl a6@(-4); jge L3086
movl #L3053,sp@
movl #L3088,sp@(4)
jbsr _printf:l
jra L3087
L3086:
movl #L3054,sp@
movl a6@(-4),sp@(4)
movl #L3088,sp@(8)
jbsr _printf:l
L3087:
L3084:
jbsr _regi:l
movl d0,a6@(-4)
movl a6@(8),a2
tstl a2@(40); jeq L3089
tstl a6@(-4); jge L3091
movl #L3053,sp@
movl #L426,sp@(4)
jbsr _printf:l
jra L3092
L3091:
movl #L3054,sp@
movl a6@(-4),sp@(4)
movl #L426,sp@(8)
jbsr _printf:l
L3092:
L3089:
movl a6@(-32),d0
L3050:
moveml a6@(-64),#0x3cf0
unlk a6
rts
.globl _regc
.align 2
_regc:link a6,#-268
moveml #0x4d0,a6@(-268)
clrb a6@(-240)
movb #1,a6@(-156)
movb #2,a6@(-160)
movb #3,a6@(-164)
movb #4,d7
movb #5,a6@(-168)
movb #6,d6
movb #7,a6@(-172)
movb #8,d1
movb #9,a6@(-176)
movb #10,a6@(-4)
movb #11,a6@(-180)
movb #12,a6@(-8)
movb #13,a6@(-184)
movb #14,a6@(-12)
movb #15,a6@(-188)
movb #16,a6@(-16)
movb #17,a6@(-192)
movb #18,a6@(-20)
movb #19,a6@(-196)
movb #20,a6@(-24)
movb #21,a6@(-200)
movb #22,a6@(-28)
movb #23,a6@(-204)
movb #24,a6@(-32)
movb #25,a6@(-208)
movb #26,a6@(-36)
movb #27,a6@(-212)
movb #28,a6@(-40)
movb #29,a6@(-216)
movb #30,a6@(-44)
movb #31,a6@(-220)
movb #32,a6@(-48)
movb #33,a6@(-224)
movb #34,a6@(-52)
movb #35,a6@(-228)
movb #36,a6@(-232)
movb #37,a6@(-236)
movb #38,a6@(-244)
lea a6@(-156),a2
movl a2,d4
lea a6@(-240),a1
subl a1,d4
movl d4,a6@(-140)
lea a6@(-160),a1
movl a1,d4
subl a2,d4
movl d4,a6@(-136)
lea a6@(-164),a2
movl a2,d4
lea a6@(-160),a2
subl a2,d4
movl d4,a6@(-132)
lea a6@(-168),a2
movl a2,d4
lea a6@(-164),a2
subl a2,d4
movl d4,a6@(-128)
lea a6@(-172),a2
movl a2,d4
lea a6@(-168),a2
subl a2,d4
movl d4,a6@(-124)
lea a6@(-176),a2
movl a2,d4
lea a6@(-172),a2
subl a2,d4
movl d4,a6@(-120)
lea a6@(-180),a2
movl a2,d4
lea a6@(-176),a2
subl a2,d4
movl d4,a6@(-116)
lea a6@(-184),a2
movl a2,d4
lea a6@(-180),a2
subl a2,d4
movl d4,a6@(-112)
lea a6@(-188),a2
movl a2,d4
lea a6@(-184),a2
subl a2,d4
movl d4,a6@(-108)
lea a6@(-192),a2
movl a2,d4
lea a6@(-188),a2
subl a2,d4
movl d4,a6@(-104)
lea a6@(-196),a2
movl a2,d4
lea a6@(-192),a2
subl a2,d4
movl d4,a6@(-100)
lea a6@(-200),a2
movl a2,d4
lea a6@(-196),a2
subl a2,d4
movl d4,a6@(-96)
lea a6@(-204),a2
movl a2,d4
lea a6@(-200),a2
subl a2,d4
movl d4,a6@(-92)
lea a6@(-208),a2
movl a2,d4
lea a6@(-204),a2
subl a2,d4
movl d4,a6@(-88)
lea a6@(-212),a2
movl a2,d4
lea a6@(-208),a2
subl a2,d4
movl d4,a6@(-84)
lea a6@(-216),a2
movl a2,d4
lea a6@(-212),a2
subl a2,d4
movl d4,a6@(-80)
lea a6@(-220),a2
movl a2,d4
lea a6@(-216),a2
subl a2,d4
movl d4,a6@(-76)
lea a6@(-224),a2
movl a2,d4
lea a6@(-220),a2
subl a2,d4
movl d4,a6@(-72)
lea a6@(-228),a2
movl a2,d4
lea a6@(-224),a2
subl a2,d4
movl d4,a6@(-68)
lea a6@(-232),a2
movl a2,d4
lea a6@(-228),a2
subl a2,d4
movl d4,a6@(-64)
lea a6@(-236),a2
movl a2,d4
lea a6@(-232),a2
subl a2,d4
movl d4,a6@(-60)
lea a6@(-244),a2
movl a2,d4
lea a6@(-236),a2
subl a2,d4
movl d4,a6@(-56)
movl a6@(-140),a6@(-152)
movl #1,a6@(-148)
clrl a6@(-144)
L3115:
movl a6@(-148),d4
cmpl #1,d4; jeq L3121
cmpl #2,d4; jeq L3124
cmpl #3,d4; jeq L3129
jra L3119
L3121:
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-152),d4; jeq L3120
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),a6@(-252)
movl #2,a6@(-148)
movl #1,a6@(-248)
jra L3120
L3124:
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-152),d4; jne L3125
movl #3,a6@(-148)
jra L3120
L3125:
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-252),d4; jne L3127
addql #1,a6@(-248)
jra L3120
L3127:
movl #4,a6@(-148)
jra L3120
L3129:
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-152),d4; jeq L3120
movl #4,a6@(-148)
L3119:
L3120:
L3116:
addql #1,a6@(-144)
cmpl #22,a6@(-144); jlt L3115
cmpl #3,a6@(-148); jne L3132
movl #16,d4
movl d4,d0; subl a6@(-248),d0
jra L3093
L3132:
movl #-1,d0
L3093:
moveml a6@(-268),#0x4d0
unlk a6
rts
.globl _regi
.align 2
_regi:link a6,#-272
moveml #0x4d8,a6@(-272)
clrl a6@(-240)
movl #1,a6@(-156)
movl #2,a6@(-160)
movl #3,a6@(-164)
movl #4,d7
movl #5,a6@(-168)
movl #6,d6
movl #7,a6@(-172)
movl #8,d1
movl #9,a6@(-176)
movl #10,a6@(-4)
movl #11,a6@(-180)
movl #12,a6@(-8)
movl #13,a6@(-184)
movl #14,a6@(-12)
movl #15,a6@(-188)
movl #16,a6@(-16)
movl #17,a6@(-192)
movl #18,a6@(-20)
movl #19,a6@(-196)
movl #20,a6@(-24)
movl #21,a6@(-200)
movl #22,a6@(-28)
movl #23,a6@(-204)
movl #24,a6@(-32)
movl #25,a6@(-208)
movl #26,a6@(-36)
movl #27,a6@(-212)
movl #28,a6@(-40)
movl #29,a6@(-216)
movl #30,a6@(-44)
movl #31,a6@(-220)
movl #32,a6@(-48)
movl #33,a6@(-224)
movl #34,a6@(-52)
movl #35,a6@(-228)
movl #36,a6@(-232)
movl #37,a6@(-236)
movl #38,a6@(-244)
lea a6@(-156),a2
movl a2,d4
lea a6@(-240),a1
subl a1,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-140)
lea a6@(-160),a1
movl a1,d4
subl a2,d4
movl d4,d4
divsl d3,d4
movl d4,a6@(-136)
lea a6@(-164),a2
movl a2,d4
lea a6@(-160),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-132)
lea a6@(-168),a2
movl a2,d4
lea a6@(-164),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-128)
lea a6@(-172),a2
movl a2,d4
lea a6@(-168),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-124)
lea a6@(-176),a2
movl a2,d4
lea a6@(-172),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-120)
lea a6@(-180),a2
movl a2,d4
lea a6@(-176),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-116)
lea a6@(-184),a2
movl a2,d4
lea a6@(-180),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-112)
lea a6@(-188),a2
movl a2,d4
lea a6@(-184),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-108)
lea a6@(-192),a2
movl a2,d4
lea a6@(-188),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-104)
lea a6@(-196),a2
movl a2,d4
lea a6@(-192),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-100)
lea a6@(-200),a2
movl a2,d4
lea a6@(-196),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-96)
lea a6@(-204),a2
movl a2,d4
lea a6@(-200),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-92)
lea a6@(-208),a2
movl a2,d4
lea a6@(-204),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-88)
lea a6@(-212),a2
movl a2,d4
lea a6@(-208),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-84)
lea a6@(-216),a2
movl a2,d4
lea a6@(-212),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-80)
lea a6@(-220),a2
movl a2,d4
lea a6@(-216),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-76)
lea a6@(-224),a2
movl a2,d4
lea a6@(-220),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-72)
lea a6@(-228),a2
movl a2,d4
lea a6@(-224),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-68)
lea a6@(-232),a2
movl a2,d4
lea a6@(-228),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-64)
lea a6@(-236),a2
movl a2,d4
lea a6@(-232),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-60)
lea a6@(-244),a2
movl a2,d4
lea a6@(-236),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-56)
movl a6@(-140),a6@(-152)
movl #1,a6@(-148)
clrl a6@(-144)
L3156:
movl a6@(-148),d4
cmpl #1,d4; jeq L3162
cmpl #2,d4; jeq L3165
cmpl #3,d4; jeq L3170
jra L3160
L3162:
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-152),d4; jeq L3161
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),a6@(-252)
movl #2,a6@(-148)
movl #1,a6@(-248)
jra L3161
L3165:
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-152),d4; jne L3166
movl #3,a6@(-148)
jra L3161
L3166:
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-252),d4; jne L3168
addql #1,a6@(-248)
jra L3161
L3168:
movl #4,a6@(-148)
jra L3161
L3170:
movl a6@(-144),d4
asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-152),d4; jeq L3161
movl #4,a6@(-148)
L3160:
L3161:
L3157:
addql #1,a6@(-144)
cmpl #22,a6@(-144); jlt L3156
cmpl #3,a6@(-148); jne L3173
movl #16,d4
movl d4,d0; subl a6@(-248),d0
jra L3134
L3173:
movl #-1,d0
L3134:
moveml a6@(-272),#0x4d8
unlk a6
rts
.globl _regp
.align 2
_regp:link a6,#-276
moveml #0x3cd8,a6@(-276)
lea a6@(-228),a2
movl a2,a6@(-228)
lea a6@(-208),a1
movl a1,a6@(-208)
lea a6@(-212),a0
movl a0,a6@(-244)
movl a0,a6@(-212)
lea a6@(-216),a0
movl a0,a6@(-216)
lea a6@(-144),a0
movl a0,a5
movl a0,a6@(-144)
lea a6@(-148),a0
movl a0,a4
movl a0,a6@(-148)
lea a6@(-152),a0
movl a0,a3
movl a0,a6@(-152)
lea a6@(-156),a0
movl a0,a6@(-4)
movl a0,a6@(-156)
lea a6@(-160),a0
movl a0,a6@(-8)
movl a0,a6@(-160)
lea a6@(-164),a0
movl a0,a6@(-12)
movl a0,a6@(-164)
lea a6@(-168),a0
movl a0,a6@(-16)
movl a0,a6@(-168)
lea a6@(-172),a0
movl a0,a6@(-20)
movl a0,a6@(-172)
lea a6@(-176),a0
movl a0,a6@(-24)
movl a0,a6@(-176)
lea a6@(-180),a0
movl a0,a6@(-28)
movl a0,a6@(-180)
lea a6@(-184),a0
movl a0,a6@(-32)
movl a0,a6@(-184)
lea a6@(-188),a0
movl a0,a6@(-36)
movl a0,a6@(-188)
lea a6@(-192),a0
movl a0,a6@(-40)
movl a0,a6@(-192)
lea a6@(-196),a0
movl a0,a6@(-44)
movl a0,a6@(-196)
lea a6@(-200),a0
movl a0,a6@(-48)
movl a0,a6@(-200)
lea a6@(-204),a0
movl a0,a6@(-52)
movl a0,a6@(-204)
lea a6@(-220),a0
movl a0,a6@(-220)
lea a6@(-224),a0
movl a0,a6@(-224)
lea a6@(-232),a0
movl a0,a6@(-232)
movl a1,d4
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-140)
movl a6@(-244),d4
subl a1,d4
movl d4,d4
divsl d3,d4
movl d4,a6@(-136)
lea a6@(-216),a2
movl a2,d4
lea a6@(-212),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-132)
lea a6@(-144),a2
movl a2,d4
lea a6@(-216),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-128)
lea a6@(-148),a2
movl a2,d4
lea a6@(-144),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-124)
lea a6@(-152),a2
movl a2,d4
lea a6@(-148),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-120)
lea a6@(-156),a2
movl a2,d4
lea a6@(-152),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-116)
lea a6@(-160),a2
movl a2,d4
lea a6@(-156),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-112)
lea a6@(-164),a2
movl a2,d4
lea a6@(-160),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-108)
lea a6@(-168),a2
movl a2,d4
lea a6@(-164),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-104)
lea a6@(-172),a2
movl a2,d4
lea a6@(-168),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-100)
lea a6@(-176),a2
movl a2,d4
lea a6@(-172),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-96)
lea a6@(-180),a2
movl a2,d4
lea a6@(-176),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-92)
lea a6@(-184),a2
movl a2,d4
lea a6@(-180),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-88)
lea a6@(-188),a2
movl a2,d4
lea a6@(-184),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-84)
lea a6@(-192),a2
movl a2,d4
lea a6@(-188),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-80)
lea a6@(-196),a2
movl a2,d4
lea a6@(-192),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-76)
lea a6@(-200),a2
movl a2,d4
lea a6@(-196),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-72)
lea a6@(-204),a2
movl a2,d4
lea a6@(-200),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-68)
lea a6@(-220),a2
movl a2,d4
lea a6@(-204),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-64)
lea a6@(-224),a2
movl a2,d4
lea a6@(-220),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-60)
lea a6@(-232),a2
movl a2,d4
lea a6@(-224),a2
subl a2,d4
movl d4,d4
movl #4,d3
divsl d3,d4
movl d4,a6@(-56)
movl a6@(-140),d1
movl #1,d6
clrl d7
L3197:
cmpl #1,d6; jeq L3203
cmpl #2,d6; jeq L3206
cmpl #3,d6; jeq L3211
jra L3201
L3203:
movl d7,d4; asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl d1,d4; jeq L3202
movl d7,d4; asll #2,d4
movl a6@(-140,d4:l:1),a6@(-240)
movl #2,d6
movl #1,a6@(-236)
jra L3202
L3206:
movl d7,d4; asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl d1,d4; jne L3207
movl #3,d6
jra L3202
L3207:
movl d7,d4; asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl a6@(-240),d4; jne L3209
addql #1,a6@(-236)
jra L3202
L3209:
movl #4,d6
jra L3202
L3211:
movl d7,d4; asll #2,d4
movl a6@(-140,d4:l:1),d4
cmpl d1,d4; jeq L3202
movl #4,d6
L3201:
L3202:
L3198:
addql #1,d7
cmpl #22,d7; jlt L3197
cmpl #3,d6; jne L3214
movl #16,d4
movl d4,d0; subl a6@(-236),d0
jra L3175
L3214:
movl #-1,d0
L3175:
moveml a6@(-276),#0x3cd8
unlk a6
rts
.bss
.lcomm L3218,420
.data
L3219:.ascii "s84,er%d\012\000"
L3220:.ascii "s84    \000"
.text
.globl _s84
.align 2
_s84:link a6,#-232
fmovemx #0x11,a6@(-180)
moveml #0x3cfc,a6@(-220)
movl a6@(8),a5
lea L3220:l,a4
lea a5@(60),a3
clrl a6@(-140)
L3221:
L3222:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L3221
movl #3,sp@
jbsr _fip:l
movl d0,a6@(-144)
movl a6@(-144),a2
cmpl #3,a2@; jeq L3224
tstl a5@(44); jeq L3226
movl #L3219,sp@
movl #1,sp@(4)
jbsr _printf:l
L3226:
addql #1,a6@(-140)
L3224:
movl #_glork,a6@(-148)
movl #4,sp@
movl a6@(-148),a2
jbsr a2@
cmpl #4,d0; jeq L3228
tstl a5@(44); jeq L3230
movl #L3219,sp@
movl #2,sp@(4)
jbsr _printf:l
L3230:
addql #2,a6@(-140)
L3228:
clrl d6
L3232:
fmovel d6,fp3
movl d6,d4; asll #2,d4
fmoves fp3,a6@(-68,d4:l:1)
movl d6,d4; asll #2,d4
lea a6@(-68,d4:l:1),a2
movl a2,a6@(-136,d4:l:1)
L3233:
addql #1,d6
cmpl #17,d6; jlt L3232
fmoves L412:l,fp7
clrl d6
L3236:
movl d6,d4; asll #2,d4
movl a6@(-136,d4:l:1),a2
fmovex fp7,fp3; fadds a2@,fp3
fmovex fp3,fp7
L3237:
addql #1,d6
cmpl #17,d6; jlt L3236
fcmps L3242:l,fp7; fjeq L3240
tstl a5@(44); jeq L3243
movl #L3219,sp@
movl #4,sp@(4)
jbsr _printf:l
L3243:
addql #4,a6@(-140)
L3240:
clrl d5
L3245:
clrl d6
L3249:
clrl d7
L3253:
movl #35,d4
mulsl d5,d4
movl #7,d3
mulsl d6,d3
addl d3,d4
addl d7,d4
movl #28,d3
mulsl d6,d3
movl #140,d2
mulsl d5,d2
lea @(L3218:l,d2:l:1),a2
movl d3,a1; addl a2,a1
movl d4,a1@(0,d7:l:4)
L3254:
addql #1,d7
cmpl #7,d7; jlt L3253
L3250:
addql #1,d6
cmpl #5,d6; jlt L3249
L3246:
addql #1,d5
cmpl #3,d5; jlt L3245
movl #1,d5
movl #2,d6
movl #3,d7
movl #L3218,sp@
movl #105,sp@(4)
clrl sp@(8)
jbsr _array:l
movl d0,a6@(-152)
movl #140,d4
mulsl d5,d4
lea @(L3218:l,d4:l:1),a2
movl a2,sp@
movl #35,sp@(4)
movl #35,sp@(8)
jbsr _array:l
movl d0,a6@(-156)
movl #28,d4
mulsl d6,d4
movl #140,d3
mulsl d5,d3
lea @(L3218:l,d3:l:1),a2
movl d4,a1; addl a2,a1
movl a1,sp@
movl #7,sp@(4)
movl #49,sp@(8)
jbsr _array:l
movl a6@(-152),d4
addl a6@(-156),d4
addl d0,d4
movl #28,d3
mulsl d6,d3
movl #140,d2
mulsl d5,d2
lea @(L3218:l,d2:l:1),a2
movl d3,a1; addl a2,a1
addl a1@(0,d7:l:4),d4
subl #52,d4
tstl d4; jeq L3257
tstl a5@(44); jeq L3259
movl #L3219,sp@
movl #8,sp@(4)
jbsr _printf:l
L3259:
addql #8,a6@(-140)
L3257:
movl a6@(-140),d0
L3217:
moveml a6@(-220),#0x3cfc
fmovemx a6@(-180),#0x11
unlk a6
rts
.globl _array
.align 2
_array:link a6,#-16
moveml #0xd8,a6@(-16)
movl a6@(8),a1
movl a6@(12),d7
movl a6@(16),d6
clrl d1
jra L3267
L3264:
movl a1@(0,d1:l:4),d4
movl d1,d3; addl d6,d3
cmpl d3,d4; jeq L3268
movl #1,d0
jra L3263
L3268:
L3265:
addql #1,d1
L3267:
cmpl d7,d1; jlt L3264
clrl d0
L3263:
moveml a6@(-16),#0xd8
unlk a6
rts
.bss
.lcomm L3271,4
.text
.globl _fip
.align 2
_fip:link a6,#-0
movl a6@(8),L3271:l
movl #L3271,d0
L3270:
unlk a6
rts
.globl _glork
.align 2
_glork:link a6,#-0
movl a6@(8),d0
L3272:
unlk a6
rts
.data
L3274:.ascii "s85,er%d\012\000"
L3275:.ascii "s85    \000"
.align 2; L3283:.long L425
.long L427
.long L426
.long L428
.long L429
.long L430
.long L431
L3284:.ascii " alignment: \000"
.text
.globl _s85
.align 2
_s85:link a6,#-440
moveml #0x3cdc,a6@(-424)
movl a6@(8),a5
lea L3275:l,a4
lea a5@(60),a3
clrl d6
L3288:
L3289:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L3288
lea a6@(-284),a2
movl a2,d4
lea a6@(-304),a2
subl a2,d4
tstl d4; jle L3299
lea a6@(-280),a2
movl a2,d4
lea a6@(-284),a2
subl a2,d4
tstl d4; jle L3299
lea a6@(-276),a2
movl a2,d4
lea a6@(-280),a2
subl a2,d4
tstl d4; jgt L3291
L3299:
tstl a5@(44); jeq L3300
movl #L3274,sp@
movl #1,sp@(4)
jbsr _printf:l
L3300:
addql #1,d6
L3291:
lea a6@(-339),a2
movl a2,d4
lea a6@(-340),a2
subl a2,d4
movl d4,a6@(-268)
lea a6@(-342),a2
movl a2,d4
lea a6@(-344),a2
subl a2,d4
movl d4,a6@(-264)
lea a6@(-350),a2
movl a2,d4
lea a6@(-352),a2
subl a2,d4
movl d4,a6@(-260)
lea a6@(-358),a2
movl a2,d4
lea a6@(-360),a2
subl a2,d4
movl d4,a6@(-256)
lea a6@(-366),a2
movl a2,d4
lea a6@(-368),a2
subl a2,d4
movl d4,a6@(-252)
lea a6@(-374),a2
movl a2,d4
lea a6@(-376),a2
subl a2,d4
movl d4,a6@(-248)
lea a6@(-386),a2
movl a2,d4
lea a6@(-388),a2
subl a2,d4
movl d4,a6@(-244)
tstl a5@(40); jeq L3315
clrl d7
L3317:
movl #L3321,sp@
movl @(L3283:l,d7:l:4),sp@(4)
movl #L3284,sp@(8)
movl d7,d4; asll #2,d4
movl a6@(-268,d4:l:1),sp@(12)
jbsr _printf:l
L3318:
addql #1,d7
cmpl #7,d7; jlt L3317
L3315:
movl a6@(-272),d4
orl #0x1c000000,d4
movl d4,a6@(-272)
movl a6@(-272),d4
movl d4,d3
andl #0x3fffffff,d3
asll #3,d4
movl #29,d2
asrl d2,d4
movl #30,d2
asll d2,d4
asrl d2,d4
movl d4,d4
lsll d2,d4
andl #0xc0000000,d4
orl d4,d3
movl d3,a6@(-272)
movl a6@(-272),d4
movl d4,d3
andl #0xe3ffffff,d3
movl #30,d2
asrl d2,d4
movl #29,d2
asll d2,d4
asrl d2,d4
movl d4,d4
movl #26,d2
lsll d2,d4
andl #0x1c000000,d4
orl d4,d3
movl d3,a6@(-272)
movl a6@(-272),d4
asll #3,d4
movl #29,d3
asrl d3,d4
cmpl #3,d4; jeq L3322
movl a6@(-272),d4
asll #3,d4
movl #29,d3
asrl d3,d4
cmpl #-1,d4; jne L3324
tstl a5@(40); jeq L3325
movl #L3328,sp@
jbsr _printf:l
jra L3325
L3324:
tstl a5@(44); jeq L3329
movl #L3274,sp@
movl #2,sp@(4)
jbsr _printf:l
L3329:
addql #2,d6
L3325:
L3322:
movl a6@(-272),d4
orl #0x2000000,d4
movl d4,a6@(-272)
movl a6@(-272),d4
asll #6,d4
movl #31,d3
asrl d3,d4
cmpl #1,d4; jeq L3331
tstl a5@(40); jeq L3333
movl #L3335,sp@
jbsr _printf:l
L3333:
L3331:
lea a6@(-240),a2
movl a2,d4
subl a2,d4
tstl d4; jne L3343
tstl d4; jne L3343
tstl d4; jne L3343
tstl d4; jne L3343
tstl d4; jne L3343
tstl d4; jne L3343
tstl d4; jeq L3336
L3343:
tstl a5@(44); jeq L3344
movl #L3274,sp@
movl #4,sp@(4)
jbsr _printf:l
L3344:
addql #4,d6
L3336:
jra L3346
tstl a5@(44); jeq L3348
movl #L3274,sp@
movl #8,sp@(4)
jbsr _printf:l
L3348:
addql #8,d6
L3346:
lea a6@(-336),a2
movl a2,a6@(-276)
movb #2,a6@(-336)
movl a6@(-276),a2
movb a2@,d4
extbl d4
addql #1,d4
movb d4,a2@
cmpb #3,a6@(-336); jeq L3352
tstl a5@(44); jeq L3354
movl #L3274,sp@
movl #16,sp@(4)
jbsr _printf:l
L3354:
addl #16,d6
L3352:
movl d6,d0
L3273:
moveml a6@(-424),#0x3cdc
unlk a6
rts
.data
L3357:.ascii "s86,er%d\012\000"
L3358:.ascii "s86    \000"
.align 2; L3359:.long 1
.long 3
.long 5
.align 2; L3360:.long L3359+8
.bss
.lcomm L3362,40
.data
.align 2; L3363:.long 0x3f800000
.long 0x40400000
.long 0x40a00000
.long 0x40000000
.long 0x40800000
.long 0x40c00000
.long 0x40400000
.long 0x40a00000
.long 0x40e00000
.long 0x0
.long 0x0
.long 0x0
.align 2; L3364:.long 0x3f800000
.long 0x40400000
.long 0x40a00000
.long 0x40000000
.long 0x40800000
.long 0x40c00000
.long 0x40400000
.long 0x40a00000
.long 0x40e00000
.skip 12
.align 2; L3365:.long 0x3f800000
.long 0x40400000
.long 0x40a00000
.long 0x40000000
.long 0x40800000
.long 0x40c00000
.long 0x40400000
.long 0x40a00000
.long 0x40e00000
.skip 12
.align 2; L3366:.long 0x3f800000
.skip 8
.long 0x40000000
.skip 8
.long 0x40400000
.skip 8
.long 0x40800000
.skip 8
.text
.globl _s86
.align 2
_s86:link a6,#-76
fmovemx #0x30,a6@(-36)
moveml #0x3cf0,a6@(-68)
movl L3360:l,a2
lea a2@(-4),a2
movl a2,a6@(-12)
jbsr _one:l
movl a6@(-12),a2
lea a2@(0,d0:l:4),a5
lea L3358:l,a4
movl a6@(8),a2
lea a2@(60),a3
clrl a6@(-8)
L3367:
L3368:
movl a3,a2
lea a2@(1),a3
movl a4,a1
lea a1@(1),a4
movb a1@,d4
movb d4,a2@
tstb d4; jne L3367
movl L3360:l,a2
cmpl #5,a2@; jeq L3370
movl a6@(8),a2
tstl a2@(44); jeq L3372
movl #L3357,sp@
movl #1,sp@(4)
jbsr _printf:l
L3372:
addql #1,a6@(-8)
L3370:
movl a6@(-12),a2
cmpl #3,a2@; jeq L3374
movl a6@(8),a2
tstl a2@(44); jeq L3376
movl #L3357,sp@
movl #2,sp@(4)
jbsr _printf:l
L3376:
addql #2,a6@(-8)
L3374:
cmpl #5,a5@; jeq L3378
movl a6@(8),a2
tstl a2@(44); jeq L3380
movl #L3357,sp@
movl #4,sp@(4)
jbsr _printf:l
L3380:
addql #4,a6@(-8)
L3378:
clrl a6@(-4)
clrl d7
L3382:
tstl @(L3362:l,d7:l:4); jeq L3386
movl #1,a6@(-4)
L3386:
L3383:
addql #1,d7
cmpl #10,d7; jlt L3382
tstl a6@(-4); jeq L3388
movl a6@(8),a2
tstl a2@(44); jeq L3390
movl #L3357,sp@
movl #8,sp@(4)
jbsr _printf:l
L3390:
addql #8,a6@(-8)
L3388:
clrl a6@(-4)
clrl d6
L3392:
clrl d7
L3396:
movl #3,d4
mulsl d6,d4
addl d7,d4
movl d4,d5
movl #12,d4
mulsl d6,d4
lea @(L3364:l,d4:l:1),a2
fmoves a2@(0,d7:l:4),fp3
lea @(L3365:l,d4:l:1),a2
fcmps a2@(0,d7:l:4),fp3; fjne L3402
fcmps @(L3363:l,d5:l:4),fp3; fjeq L3400
L3402:
movl #1,a6@(-4)
L3400:
L3397:
addql #1,d7
cmpl #3,d7; jlt L3396
L3393:
addql #1,d6
cmpl #4,d6; jlt L3392
tstl a6@(-4); jeq L3403
movl a6@(8),a2
tstl a2@(44); jeq L3405
movl #L3357,sp@
movl #16,sp@(4)
jbsr _printf:l
L3405:
addl #16,a6@(-8)
L3403:
clrl a6@(-4)
clrl d7
L3407:
movl #12,d4
mulsl d7,d4
fmoves @(L3366:l,d4:l:1),fp3
movl d7,d4; addql #1,d4
fmovel d4,fp2
fmoves fp2,d0; fmoves d0,fp2
fcmpx fp2,fp3; fjeq L3411
movl #1,a6@(-4)
L3411:
L3408:
addql #1,d7
cmpl #4,d7; jlt L3407
tstl a6@(-4); jeq L3413
movl a6@(8),a2
tstl a2@(44); jeq L3415
movl #L3357,sp@
movl #32,sp@(4)
jbsr _printf:l
L3415:
addl #32,a6@(-8)
L3413:
movl a6@(-8),d0
L3356:
moveml a6@(-68),#0x3cf0
fmovemx a6@(-36),#0x30
unlk a6
rts
.globl _one
.align 2
_one:link a6,#-0
movl #1,d0
L3417:
unlk a6
rts
.data
L3419:.ascii "s88,er%d\012\000"
L3420:.ascii "s88    \000"
.text
.globl _s88
.align 2
_s88:link a6,#-64
fmovemx #0x10,a6@(-32)
moveml #0x3c90,a6@(-56)
lea L3420:l,a5
movl a6@(8),a2
lea a2@(60),a4
clrl d7
L3422:
L3423:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L3422
jra L3425
movl a6@(8),a2
tstl a2@(44); jeq L3427
movl #L3419,sp@
movl #1,sp@(4)
jbsr _printf:l
L3427:
addql #1,d7
L3425:
lea a6@(-20),a2
movl a2,_metricp:l
movl #2,a6@(-20)
movl _metricp:l,a2
movl #3,a2@
cmpl #3,a6@(-20); jeq L3429
movl a6@(8),a2
tstl a2@(44); jeq L3431
movl #L3419,sp@
movl #2,sp@(4)
jbsr _printf:l
L3431:
addql #2,d7
L3429:
fmoved L419:l,fp3
fmoved fp3,a6@(-16)
fmoved fp3,a6@(-8)
lea a6@(-16),a3
fmoved L418:l,fp3
fmoved fp3,a3@
fmoved L418:l,fp3
fmoved fp3,a3@(8)
fmoved a6@(-16),fp3
faddd a6@(-8),fp3
fcmpd L416:l,fp3; fjeq L3434
movl a6@(8),a2
tstl a2@(44); jeq L3437
movl #L3419,sp@
movl #4,sp@(4)
jbsr _printf:l
L3437:
addql #4,d7
L3434:
movl d7,d0
L3418:
moveml a6@(-56),#0x3c90
fmovemx a6@(-32),#0x10
unlk a6
rts
.data
L3440:.ascii "s9,er%d\012\000"
L3441:.ascii "s9     \000"
.text
.globl _s9
.align 2
_s9:link a6,#-52
moveml #0x34f0,a6@(-44)
lea L3441:l,a5
movl a6@(8),a2
lea a2@(60),a4
clrl d5
L3442:
L3443:
movl a4,a2
lea a2@(1),a4
movl a5,a1
lea a1@(1),a5
movb a1@,d4
movb d4,a2@
tstb d4; jne L3442
clrl d6
clrl d7
L3445:
movl #2,a6@(-4)
movl #2,a6@(-8)
movl #3,a6@(-12)
movl #3,a6@(-16)
cmpl #3,a6@(-16); jne L3451
cmpl #3,a6@(-12); jeq L3449
L3451:
movl #1,d6
L3449:
cmpl #2,a6@(-8); jne L3454
cmpl #2,a6@(-4); jeq L3452
L3454:
movl #1,d6
L3452:
L3446:
addql #1,d7
cmpl #2,d7; jlt L3445
tstl d6; jeq L3459
movl a6@(8),a2
tstl a2@(44); jeq L3457
movl #L3440,sp@
movl #1,sp@(4)
jbsr _printf:l
L3457:
addql #1,d5
jra L3459
movl a6@(8),a2
tstl a2@(44); jeq L3460
movl #L3440,sp@
movl #2,sp@(4)
jbsr _printf:l
L3460:
addql #2,d5
L3459:
movl d5,d0
L3439:
moveml a6@(-44),#0x34f0
unlk a6
rts
.globl _setev
.align 2
_setev:link a6,#-0
movl #1066,_extvar:l
L3462:
unlk a6
rts
.bss
.globl _rfs
.comm _rfs,8
.globl _crc
.comm _crc,4
.globl _rrc
.comm _rrc,4
.globl _flgl
.comm _flgl,4
.globl _flgd
.comm _flgd,4
.globl _flgm
.comm _flgm,4
.globl _flgs
.comm _flgs,4
.globl _dprec
.comm _dprec,4
.globl _fprec
.comm _fprec,4
.globl _dbits
.comm _dbits,4
.globl _fbits
.comm _fbits,4
.globl _ubits
.comm _ubits,4
.globl _lbits
.comm _lbits,4
.globl _metricp
.comm _metricp,4
.globl _extvar
.comm _extvar,4
.text
.align 2; L3335:.ascii "Be especially careful with 1-bit fields!\012\000"
.align 2; L3328:.ascii "Sign extension in fields\012\000"
.align 2; L3321:.ascii "%s%s%d\012\000"
.align 2; L3242:.long 0x43080000
.align 2; L3088:.ascii "pointer\000"
.align 2; L2538:.ascii "Increasing array elements assigned to decreasing locations\012\000"
.align 2; L2416:.long 0x41d00000
.align 2; L2415:.long 0x403a0000,0x0
.align 2; L1713:.long 0x40040000,0x0
.align 2; L1529:.long 0x40240000,0x0
.align 2; L1497:.long 0x41200000
.align 2; L1313:.long 0x40080000,0x0
.align 2; L1281:.long 0x40400000
.align 2; L1097:.long 0x401c0000,0x0
.align 2; L1065:.long 0x40e00000
.align 2; L879:.long 0x40140000,0x0
.align 2; L847:.long 0x40a00000
.align 2; L835:.long 0x41e00000,0x0
.align 2; L660:.ascii "queep\000"
.align 2; L646:.byte 10,0
.align 2; L645:.byte 37,100,0
.align 2; L640:.ascii "   key=\000"
.align 2; L600:.long 0x41f00000,0x0
.align 2; L537:.long 0x40ce8480,0x0
.align 2; L536:.long 0x405f4000,0x0
.align 2; L535:.long 0x42fa0000
.align 2; L530:.long 0x40000000
.align 2; L431:.ascii "double\000"
.align 2; L430:.ascii "float\000"
.align 2; L429:.ascii "unsigned\000"
.align 2; L428:.ascii "long\000"
.align 2; L427:.ascii "short\000"
.align 2; L426:.ascii "int\000"
.align 2; L425:.ascii "char\000"
.align 2; L419:.long 0x0,0x0
.align 2; L418:.long 0x3ff00000,0x0
.align 2; L417:.long 0x40100000,0x0
.align 2; L416:.long 0x40000000,0x0
.align 2; L412:.long 0x0
.align 2; L411:.long 0x3f800000
.align 2; L390:.ascii "queep!\000"
.align 2; L379:.byte 10,9,8,13,12,92,39,0
.align 2; L375:.byte 46,34,46,0
.align 2; L362:.ascii "...\000"
.align 2; L333:.long 0x40938800,0x0
.align 2; L199:.ascii "   different results when assigned to longs.\012\000"
.align 2; L198:.ascii "Decimal and octal/hex constants sometimes give\012\000"
.align 2; L27:.ascii "\012Failed.\012\000"
.align 2; L26:.ascii "\012No errors detected.\012\000"
.align 2; L20:.ascii "Section %s returned %d.\012\000"
