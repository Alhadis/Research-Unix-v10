.set reorder
.data; .align 2; L.2:.word s22
.word s241
.word s243
.word s244
.word s25
.word s26
.word s4
.word s61
.word s626
.word s71
.word s72
.word s757
.word s7813
.word s714
.word s715
.word s81
.word s84
.word s85
.word s86
.word s88
.word s9
.lcomm L.3,68
.lcomm L.4,4
.text
.globl main
.text
.align 2; .ent main
main:
$fp1=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0xc0000000,-4
sw $30,16($sp)
sw $31,20($sp)
sw $4,24($sp)
sw $5,28($sp)
li $25,1
sw $25,L.3+36
li $25,1
sw $25,L.3+40
li $25,1
sw $25,L.3+44
li $25,1
sw $25,L.3+48
la $25,L.3
sw $25,L.4
move $30,$0
b L.12
L.9:
lw $4,L.4
sll $25,$30,2
lw $25,L.2($25)
jal $25  # int
sw $2,L.3+52
lw $25,L.3+56
lw $24,L.3+52
addu $25,$25,$24
sw $25,L.3+56
lw $25,L.3+36
beq $25,$0,L.17
la $4,L.20
la $5,L.3+60
lw $6,L.3+52
jal printf  # int
L.17:
L.10:
addu $30,$30,1
L.12:
bltu $30,21,L.9
lw $25,L.3+56
bne $25,$0,L.23
la $4,L.26
jal printf  # int
b L.24
L.23:
la $4,L.27
jal printf  # int
L.24:
move $2,$0
L.1:
lw $30,16($sp)
lw $31,20($sp)
addu $sp,$sp,24
j $31
.end main
.data; L.29:.ascii "s22,er%d\012\000"
.data; L.30:.ascii "s22    \000"
.text
.globl s22
.text
.align 2; .ent s22
s22:
$fp2=56
.frame $sp,56,$31
addu $sp,$sp,-56
.mask 0xc0e00000,-24
sw $21,16($sp)
sw $22,20($sp)
sw $23,24($sp)
sw $30,28($sp)
sw $31,32($sp)
sw $4,56($sp)
move $22,$0
la $30,L.30
lw $25,$fp2+0($sp)
addu $23,$25,60
L.31:
L.32:
move $25,$23
addu $23,$25,1
move $24,$30
addu $30,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.31
li $21,1
li $25,2
sw $25,$fp2-8($sp)
li $25,3
sw $25,$fp2-12($sp)
li $25,4
sw $25,$fp2-4($sp)
lw $25,$fp2-8($sp)
addu $25,$21,$25
lw $24,$fp2-12($sp)
addu $25,$25,$24
lw $24,$fp2-4($sp)
addu $25,$25,$24
beq $25,10,L.34
addu $22,$22,1
lw $25,$fp2+0($sp)
lw $25,44($25)
beq $25,$0,L.36
la $4,L.29
li $5,1
jal printf  # int
L.36:
L.34:
li $25,2
sw $25,$fp2-16($sp)
lw $25,$fp2-16($sp)
bne $25,$21,L.38
addu $22,$22,4
lw $25,$fp2+0($sp)
lw $25,44($25)
beq $25,$0,L.40
la $4,L.29
li $5,4
jal printf  # int
L.40:
L.38:
move $2,$22
L.28:
lw $21,16($sp)
lw $22,20($sp)
lw $23,24($sp)
lw $30,28($sp)
lw $31,32($sp)
addu $sp,$sp,56
j $31
.end s22
.data; L.43:.ascii "s241,er%d\012\000"
.data; L.44:.ascii "s241   \000"
.data; .align 2; L.45:.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x0
.word 0x6
.word 0x0
.word 0x8
.word 0x0
.word 0xc
.word 0x0
.word 0x10
.word 0x0
.word 0x12
.word 0x0
.word 0x14
.word 0x0
.word 0x18
.word 0x0
.word 0x1c
.word 0x0
.word 0x1e
.word 0x0
.word 0x20
.word 0x0
.word 0x24
.text
.globl s241
.text
.align 2; .ent s241
s241:
$fp3=520
.frame $sp,520,$31
addu $sp,$sp,-520
.mask 0xc0f80000,-480
sw $19,16($sp)
sw $20,20($sp)
sw $21,24($sp)
sw $22,28($sp)
sw $23,32($sp)
sw $30,36($sp)
sw $31,40($sp)
move $30,$4
move $20,$0
sw $0,$fp3-472($sp)
la $22,L.44
addu $21,$30,60
L.46:
L.47:
move $25,$21
addu $21,$25,1
move $24,$22
addu $22,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.46
b L.49
addu $20,$20,1
lw $25,44($30)
beq $25,$0,L.51
la $4,L.43
li $5,1
jal printf  # int
L.51:
L.49:
b L.53
addu $20,$20,2
lw $25,44($30)
beq $25,$0,L.55
la $4,L.43
li $5,2
jal printf  # int
L.55:
L.53:
b L.57
addu $20,$20,4
lw $25,44($30)
beq $25,$0,L.59
la $4,L.43
li $5,4
jal printf  # int
L.59:
L.57:
b L.61
addu $20,$20,8
lw $25,44($30)
beq $25,$0,L.63
la $4,L.43
li $5,8
jal printf  # int
L.63:
L.61:
move $23,$0
L.65:
sll $25,$23,2
sw $23,L.45($25)
L.66:
addu $23,$23,1
blt $23,17,L.65
li $23,18
L.69:
sll $25,$23,2
move $19,$25
lw $4,L.45($25)
jal pow2  # int
sw $2,L.45($19)
sll $25,$23,2
lw $24,L.45($25)
subu $24,$24,1
sw $24,L.45-4($25)
addu $23,$23,2
L.70:
blt $23,39,L.69
sw $0,$fp3-156($sp)
sw $0,$fp3-312($sp)
sw $0,$fp3-468($sp)
li $25,1
sw $25,$fp3-152($sp)
li $25,1
sw $25,$fp3-308($sp)
li $25,1
sw $25,$fp3-464($sp)
li $25,2
sw $25,$fp3-148($sp)
li $25,2
sw $25,$fp3-304($sp)
li $25,2
sw $25,$fp3-460($sp)
li $25,3
sw $25,$fp3-144($sp)
li $25,3
sw $25,$fp3-300($sp)
li $25,3
sw $25,$fp3-456($sp)
li $25,4
sw $25,$fp3-140($sp)
li $25,4
sw $25,$fp3-296($sp)
li $25,4
sw $25,$fp3-452($sp)
li $25,5
sw $25,$fp3-136($sp)
li $25,5
sw $25,$fp3-292($sp)
li $25,5
sw $25,$fp3-448($sp)
li $25,6
sw $25,$fp3-132($sp)
li $25,6
sw $25,$fp3-288($sp)
li $25,6
sw $25,$fp3-444($sp)
li $25,7
sw $25,$fp3-128($sp)
li $25,7
sw $25,$fp3-284($sp)
li $25,7
sw $25,$fp3-440($sp)
li $25,8
sw $25,$fp3-124($sp)
li $25,8
sw $25,$fp3-280($sp)
li $25,8
sw $25,$fp3-436($sp)
li $25,9
sw $25,$fp3-120($sp)
li $25,9
sw $25,$fp3-276($sp)
li $25,9
sw $25,$fp3-432($sp)
li $25,10
sw $25,$fp3-116($sp)
li $25,10
sw $25,$fp3-272($sp)
li $25,10
sw $25,$fp3-428($sp)
li $25,11
sw $25,$fp3-112($sp)
li $25,11
sw $25,$fp3-268($sp)
li $25,11
sw $25,$fp3-424($sp)
li $25,12
sw $25,$fp3-108($sp)
li $25,12
sw $25,$fp3-264($sp)
li $25,12
sw $25,$fp3-420($sp)
li $25,13
sw $25,$fp3-104($sp)
li $25,13
sw $25,$fp3-260($sp)
li $25,13
sw $25,$fp3-416($sp)
li $25,14
sw $25,$fp3-100($sp)
li $25,14
sw $25,$fp3-256($sp)
li $25,14
sw $25,$fp3-412($sp)
li $25,15
sw $25,$fp3-96($sp)
li $25,15
sw $25,$fp3-252($sp)
li $25,15
sw $25,$fp3-408($sp)
li $25,16
sw $25,$fp3-92($sp)
li $25,16
sw $25,$fp3-248($sp)
li $25,16
sw $25,$fp3-404($sp)
li $25,63
sw $25,$fp3-88($sp)
li $25,63
sw $25,$fp3-244($sp)
li $25,63
sw $25,$fp3-400($sp)
li $25,64
sw $25,$fp3-84($sp)
li $25,64
sw $25,$fp3-240($sp)
li $25,64
sw $25,$fp3-396($sp)
li $25,255
sw $25,$fp3-80($sp)
li $25,255
sw $25,$fp3-236($sp)
li $25,255
sw $25,$fp3-392($sp)
li $25,256
sw $25,$fp3-76($sp)
li $25,256
sw $25,$fp3-232($sp)
li $25,256
sw $25,$fp3-388($sp)
li $25,4095
sw $25,$fp3-72($sp)
li $25,4095
sw $25,$fp3-228($sp)
li $25,4095
sw $25,$fp3-384($sp)
li $25,4096
sw $25,$fp3-68($sp)
li $25,4096
sw $25,$fp3-224($sp)
li $25,4096
sw $25,$fp3-380($sp)
li $25,65535
sw $25,$fp3-64($sp)
li $25,65535
sw $25,$fp3-220($sp)
li $25,65535
sw $25,$fp3-376($sp)
li $25,65536
sw $25,$fp3-60($sp)
li $25,65536
sw $25,$fp3-216($sp)
li $25,65536
sw $25,$fp3-372($sp)
li $25,262143
sw $25,$fp3-56($sp)
li $25,262143
sw $25,$fp3-212($sp)
li $25,262143
sw $25,$fp3-368($sp)
li $25,262144
sw $25,$fp3-52($sp)
li $25,262144
sw $25,$fp3-208($sp)
li $25,262144
sw $25,$fp3-364($sp)
li $25,1048575
sw $25,$fp3-48($sp)
li $25,1048575
sw $25,$fp3-204($sp)
li $25,1048575
sw $25,$fp3-360($sp)
li $25,1048576
sw $25,$fp3-44($sp)
li $25,1048576
sw $25,$fp3-200($sp)
li $25,1048576
sw $25,$fp3-356($sp)
li $25,16777215
sw $25,$fp3-40($sp)
li $25,16777215
sw $25,$fp3-196($sp)
li $25,16777215
sw $25,$fp3-352($sp)
li $25,16777216
sw $25,$fp3-36($sp)
li $25,16777216
sw $25,$fp3-192($sp)
li $25,16777216
sw $25,$fp3-348($sp)
li $25,268435455
sw $25,$fp3-32($sp)
li $25,268435455
sw $25,$fp3-188($sp)
li $25,268435455
sw $25,$fp3-344($sp)
li $25,268435456
sw $25,$fp3-28($sp)
li $25,268435456
sw $25,$fp3-184($sp)
li $25,268435456
sw $25,$fp3-340($sp)
li $25,1073741823
sw $25,$fp3-24($sp)
li $25,1073741823
sw $25,$fp3-180($sp)
li $25,1073741823
sw $25,$fp3-336($sp)
li $25,1073741824
sw $25,$fp3-20($sp)
li $25,1073741824
sw $25,$fp3-176($sp)
li $25,1073741824
sw $25,$fp3-332($sp)
li $25,0xffffffff
sw $25,$fp3-16($sp)
li $25,0xffffffff
sw $25,$fp3-172($sp)
li $25,0xffffffff
sw $25,$fp3-328($sp)
li $25,2147483647
sw $25,$fp3-12($sp)
li $25,2147483647
sw $25,$fp3-168($sp)
li $25,2147483647
sw $25,$fp3-324($sp)
li $25,2147483647
sw $25,$fp3-8($sp)
li $25,2147483647
sw $25,$fp3-164($sp)
li $25,2147483647
sw $25,$fp3-320($sp)
li $25,2147483647
sw $25,$fp3-4($sp)
li $25,2147483647
sw $25,$fp3-160($sp)
li $25,2147483647
sw $25,$fp3-316($sp)
move $23,$0
L.188:
sll $25,$23,2
lw $24,L.45($25)
la $15,$fp3-156($sp)
addu $15,$25,$15
lw $15,($15)
bne $24,$15,L.195
la $24,$fp3-312($sp)
addu $24,$25,$24
lw $24,($24)
bne $15,$24,L.195
la $15,$fp3-468($sp)
addu $25,$25,$15
lw $25,($25)
beq $24,$25,L.192
L.195:
lw $25,40($30)
beq $25,$0,L.196
la $4,L.198
jal printf  # int
la $4,L.199
jal printf  # int
L.196:
L.192:
L.189:
addu $23,$23,1
blt $23,39,L.188
lw $25,$fp3-472($sp)
beq $25,$0,L.200
li $20,16
L.200:
move $2,$20
L.42:
lw $19,16($sp)
lw $20,20($sp)
lw $21,24($sp)
lw $22,28($sp)
lw $23,32($sp)
lw $30,36($sp)
lw $31,40($sp)
addu $sp,$sp,520
j $31
.end s241
.globl pow2
.text
.align 2; .ent pow2
pow2:
$fp4=0
.frame $sp,0,$31
li $24,1
b L.205
L.204:
sll $24,$24,1
L.205:
move $25,$4
subu $4,$25,1
bne $25,$0,L.204
move $2,$24
L.203:
j $31
.end pow2
.data; L.208:.ascii "s243,er%d\012\000"
.data; L.209:.ascii "s243   \000"
.text
.globl s243
.text
.align 2; .ent s243
s243:
$fp5=288
.frame $sp,288,$31
addu $sp,$sp,-288
.mask 0xc0c00000,-260
sw $22,16($sp)
sw $23,20($sp)
sw $30,24($sp)
sw $31,28($sp)
sw $4,288($sp)
move $22,$0
la $30,L.209
lw $25,$fp5+0($sp)
addu $23,$25,60
L.210:
L.211:
move $25,$23
addu $23,$25,1
move $24,$30
addu $30,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.210
la $4,$fp5-256($sp)
jal zerofill  # int
li $25,1
sb $25,$fp5-159($sp)
li $25,1
sb $25,$fp5-191($sp)
li $25,1
sb $25,$fp5-130($sp)
li $25,1
sb $25,$fp5-208($sp)
li $25,1
sb $25,$fp5-158($sp)
li $25,1
sb $25,$fp5-190($sp)
li $25,1
sb $25,$fp5-223($sp)
li $25,1
sb $25,$fp5-207($sp)
li $25,1
sb $25,$fp5-157($sp)
li $25,1
sb $25,$fp5-189($sp)
li $25,1
sb $25,$fp5-222($sp)
li $25,1
sb $25,$fp5-206($sp)
li $25,1
sb $25,$fp5-156($sp)
li $25,1
sb $25,$fp5-188($sp)
li $25,1
sb $25,$fp5-221($sp)
li $25,1
sb $25,$fp5-205($sp)
li $25,1
sb $25,$fp5-155($sp)
li $25,1
sb $25,$fp5-187($sp)
li $25,1
sb $25,$fp5-219($sp)
li $25,1
sb $25,$fp5-204($sp)
li $25,1
sb $25,$fp5-154($sp)
li $25,1
sb $25,$fp5-186($sp)
li $25,1
sb $25,$fp5-218($sp)
li $25,1
sb $25,$fp5-203($sp)
li $25,1
sb $25,$fp5-153($sp)
li $25,1
sb $25,$fp5-185($sp)
li $25,1
sb $25,$fp5-216($sp)
li $25,1
sb $25,$fp5-202($sp)
li $25,1
sb $25,$fp5-152($sp)
li $25,1
sb $25,$fp5-184($sp)
li $25,1
sb $25,$fp5-215($sp)
li $25,1
sb $25,$fp5-201($sp)
li $25,1
sb $25,$fp5-151($sp)
li $25,1
sb $25,$fp5-183($sp)
li $25,1
sb $25,$fp5-161($sp)
li $25,1
sb $25,$fp5-200($sp)
li $25,1
sb $25,$fp5-150($sp)
li $25,1
sb $25,$fp5-182($sp)
li $25,1
sb $25,$fp5-195($sp)
li $25,1
sb $25,$fp5-199($sp)
li $25,1
sb $25,$fp5-149($sp)
li $25,1
sb $25,$fp5-181($sp)
li $25,1
sb $25,$fp5-211($sp)
li $25,1
sb $25,$fp5-148($sp)
li $25,1
sb $25,$fp5-180($sp)
li $25,1
sb $25,$fp5-162($sp)
li $25,1
sb $25,$fp5-147($sp)
li $25,1
sb $25,$fp5-179($sp)
li $25,1
sb $25,$fp5-132($sp)
li $25,1
sb $25,$fp5-246($sp)
li $25,1
sb $25,$fp5-146($sp)
li $25,1
sb $25,$fp5-178($sp)
li $25,1
sb $25,$fp5-247($sp)
li $25,1
sb $25,$fp5-145($sp)
li $25,1
sb $25,$fp5-177($sp)
li $25,1
sb $25,$fp5-133($sp)
li $25,1
sb $25,$fp5-248($sp)
li $25,1
sb $25,$fp5-144($sp)
li $25,1
sb $25,$fp5-176($sp)
li $25,1
sb $25,$fp5-131($sp)
li $25,1
sb $25,$fp5-243($sp)
li $25,1
sb $25,$fp5-143($sp)
li $25,1
sb $25,$fp5-175($sp)
li $25,1
sb $25,$fp5-165($sp)
li $25,1
sb $25,$fp5-244($sp)
li $25,1
sb $25,$fp5-142($sp)
li $25,1
sb $25,$fp5-174($sp)
li $25,1
sb $25,$fp5-163($sp)
li $25,1
sb $25,$fp5-141($sp)
li $25,1
sb $25,$fp5-173($sp)
li $25,1
sb $25,$fp5-213($sp)
li $25,1
sb $25,$fp5-164($sp)
li $25,1
sb $25,$fp5-140($sp)
li $25,1
sb $25,$fp5-172($sp)
li $25,1
sb $25,$fp5-197($sp)
li $25,1
sb $25,$fp5-217($sp)
li $25,1
sb $25,$fp5-139($sp)
li $25,1
sb $25,$fp5-171($sp)
li $25,1
sb $25,$fp5-214($sp)
li $25,1
sb $25,$fp5-138($sp)
li $25,1
sb $25,$fp5-170($sp)
li $25,1
sb $25,$fp5-198($sp)
li $25,1
sb $25,$fp5-256($sp)
sb $25,$fp5-137($sp)
li $25,1
sb $25,$fp5-169($sp)
li $25,1
sb $25,$fp5-196($sp)
li $25,1
sb $25,$fp5-224($sp)
li $25,1
sb $25,$fp5-136($sp)
li $25,1
sb $25,$fp5-168($sp)
li $25,1
sb $25,$fp5-194($sp)
li $25,1
sb $25,$fp5-135($sp)
li $25,1
sb $25,$fp5-167($sp)
li $25,1
sb $25,$fp5-212($sp)
li $25,1
sb $25,$fp5-134($sp)
li $25,1
sb $25,$fp5-166($sp)
li $25,1
sb $25,$fp5-210($sp)
li $25,1
sb $25,$fp5-193($sp)
li $25,1
sb $25,$fp5-209($sp)
la $4,$fp5-256($sp)
jal sumof  # int
beq $2,98,L.310
addu $22,$22,1
lw $25,$fp5+0($sp)
lw $25,44($25)
beq $25,$0,L.312
la $4,L.208
li $5,1
jal printf  # int
L.312:
L.310:
b L.314
addu $22,$22,8
lw $25,$fp5+0($sp)
lw $25,44($25)
beq $25,$0,L.316
la $4,L.208
li $5,8
jal printf  # int
L.316:
L.314:
move $2,$22
L.207:
lw $22,16($sp)
lw $23,20($sp)
lw $30,24($sp)
lw $31,28($sp)
addu $sp,$sp,288
j $31
.end s243
.globl zerofill
.text
.align 2; .ent zerofill
zerofill:
$fp6=0
.frame $sp,0,$31
move $24,$0
L.319:
move $25,$4
addu $4,$25,1
sb $0,($25)
L.320:
addu $24,$24,1
blt $24,256,L.319
L.318:
j $31
.end zerofill
.globl sumof
.text
.align 2; .ent sumof
sumof:
$fp7=0
.frame $sp,0,$31
move $24,$4
move $14,$0
move $15,$0
L.324:
move $25,$24
addu $24,$25,1
lb $25,($25)
addu $14,$14,$25
L.325:
addu $15,$15,1
blt $15,256,L.324
move $2,$14
L.323:
j $31
.end sumof
.data; L.329:.ascii "s244,er%d\012\000"
.data; L.330:.ascii "s244   \000"
.text
.globl s244
.text
.align 2; .ent s244
s244:
$fp8=104
.frame $sp,104,$31
addu $sp,$sp,-104
.mask 0xc0f00000,-68
sw $20,16($sp)
sw $21,20($sp)
sw $22,24($sp)
sw $23,28($sp)
sw $30,32($sp)
sw $31,36($sp)
sw $4,104($sp)
la $23,L.330
lw $25,$fp8+0($sp)
addu $22,$25,60
L.331:
L.332:
move $25,$22
addu $22,$25,1
move $24,$23
addu $23,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.331
move $20,$0
move $21,$0
l.d $f18,L.334
s.d $f18,$fp8-64($sp)
s.d $f18,$fp8-56($sp)
l.d $f18,L.334
s.d $f18,$fp8-48($sp)
l.d $f18,L.334
s.d $f18,$fp8-40($sp)
l.d $f18,L.334
s.d $f18,$fp8-32($sp)
l.d $f18,L.334
s.d $f18,$fp8-24($sp)
l.d $f18,L.334
s.d $f18,$fp8-16($sp)
l.d $f18,L.334
s.d $f18,$fp8-8($sp)
move $21,$0
move $30,$0
L.342:
sll $25,$30,3
la $24,$fp8-64($sp)
addu $24,$25,$24
l.d $f18,($24)
la $24,$fp8-56($sp)
addu $25,$25,$24
l.d $f16,($25)
c.eq.d $f18,$f16; bc1t L.346
li $21,1
L.346:
L.343:
addu $30,$30,1
blt $30,7,L.342
beq $21,$0,L.349
lw $25,$fp8+0($sp)
lw $25,44($25)
beq $25,$0,L.351
la $4,L.329
li $5,1
jal printf  # int
L.351:
addu $20,$20,1
L.349:
b L.353
lw $25,$fp8+0($sp)
lw $25,44($25)
beq $25,$0,L.355
la $4,L.329
li $5,2
jal printf  # int
L.355:
addu $20,$20,2
L.353:
move $2,$20
L.328:
lw $20,16($sp)
lw $21,20($sp)
lw $22,24($sp)
lw $23,28($sp)
lw $30,32($sp)
lw $31,36($sp)
addu $sp,$sp,104
j $31
.end s244
.data; L.358:.ascii "s25,er%d\012\000"
.data; L.359:.ascii "s25    \000"
.text
.globl s25
.text
.align 2; .ent s25
s25:
$fp9=56
.frame $sp,56,$31
addu $sp,$sp,-56
.mask 0xc0fe0000,-8
sw $17,16($sp)
sw $18,20($sp)
sw $19,24($sp)
sw $20,28($sp)
sw $21,32($sp)
sw $22,36($sp)
sw $23,40($sp)
sw $30,44($sp)
sw $31,48($sp)
move $30,$4
la $20,L.359
addu $19,$30,60
L.360:
L.361:
move $25,$19
addu $19,$25,1
move $24,$20
addu $20,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.360
move $18,$0
la $22,L.363
lb $25,($22)
lb $24,1($22)
bne $25,$24,L.367
lb $25,2($22)
bne $24,$25,L.367
beq $25,46,L.364
L.367:
addu $18,$18,1
lw $25,44($30)
beq $25,$0,L.368
la $4,L.358
li $5,1
jal printf  # int
L.368:
L.364:
lb $25,3($22)
beq $25,$0,L.370
addu $18,$18,4
lw $25,44($30)
beq $25,$0,L.372
la $4,L.358
li $5,4
jal printf  # int
L.372:
L.370:
lb $25,L.376+1
beq $25,34,L.374
addu $18,$18,8
lw $25,44($30)
beq $25,$0,L.378
la $4,L.358
li $5,8
jal printf  # int
L.378:
L.374:
la $22,L.380
lb $25,($22)
bne $25,10,L.388
lb $25,1($22)
bne $25,9,L.388
lb $25,2($22)
bne $25,8,L.388
lb $25,3($22)
bne $25,13,L.388
lb $25,4($22)
bne $25,12,L.388
lb $25,5($22)
bne $25,92,L.388
lb $25,6($22)
beq $25,39,L.381
L.388:
addu $18,$18,16
lw $25,44($30)
beq $25,$0,L.389
la $4,L.358
li $5,16
jal printf  # int
L.389:
L.381:
la $25,L.391
move $21,$25
move $22,$25
move $17,$0
move $23,$0
b L.395
L.392:
addu $25,$23,$22
lb $25,($25)
addu $24,$23,$21
lb $24,($24)
beq $25,$24,L.396
li $17,1
L.396:
L.393:
addu $23,$23,1
L.395:
bltu $23,7,L.392
beq $17,$0,L.398
addu $18,$18,32
lw $25,44($30)
beq $25,$0,L.400
la $4,L.358
li $5,32
jal printf  # int
L.400:
L.398:
move $2,$18
L.357:
lw $17,16($sp)
lw $18,20($sp)
lw $19,24($sp)
lw $20,28($sp)
lw $21,32($sp)
lw $22,36($sp)
lw $23,40($sp)
lw $30,44($sp)
lw $31,48($sp)
addu $sp,$sp,56
j $31
.end s25
.data; L.403:.ascii "s26    \000"
.data; L.404:.ascii "%3d bits in %ss.\012\000"
.data; L.405:.ascii "%e is the least number that can be added to 1. (%s).\012\000"
.text
.globl s26
.text
.align 2; .ent s26
s26:
$fp10=88
.frame $sp,88,$31
addu $sp,$sp,-88
.fmask 0xffc00000,-36
s.d $f22,20($sp)
s.d $f24,28($sp)
s.d $f26,36($sp)
s.d $f28,44($sp)
s.d $f30,52($sp)
.mask 0xc0f00000,-8
sw $20,60($sp)
sw $21,64($sp)
sw $22,68($sp)
sw $23,72($sp)
sw $30,76($sp)
sw $31,80($sp)
move $30,$4
la $22,L.403
addu $21,$30,60
L.406:
L.407:
move $25,$21
addu $21,$25,1
move $24,$22
addu $22,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.406
sw $0,($30)
move $20,$0
li $23,1
b L.410
L.409:
sll $25,$23,24; sra $25,$25,24
sll $23,$25,1
lw $25,($30)
addu $25,$25,1
sw $25,($30)
L.410:
sll $25,$20,24; sra $25,$25,24
sll $24,$23,24; sra $24,$24,24
bne $25,$24,L.409
lw $25,($30)
sll $25,$25,2
sw $25,4($30)
lw $25,($30)
sll $25,$25,1
sw $25,8($30)
lw $25,($30)
sll $25,$25,2
sw $25,12($30)
lw $25,($30)
sll $25,$25,2
sw $25,16($30)
lw $25,($30)
sll $25,$25,2
sw $25,20($30)
lw $25,($30)
sll $25,$25,3
sw $25,24($30)
l.s $f18,L.412
mov.s $f26,$f18
mov.s $f30,$f18
l.s $f28,L.413
b L.415
L.414:
add.s $f28,$f26,$f30
cvt.d.s $f18,$f30
l.d $f16,L.417
div.d $f18,$f18,$f16
cvt.s.d $f30,$f18
L.415:
c.eq.s $f28,$f26; bc1f L.414
l.d $f18,L.418
cvt.d.s $f16,$f30
mul.d $f18,$f18,$f16
cvt.s.d $f18,$f18
s.s $f18,28($30)
l.d $f22,L.419
l.s $f30,L.412
l.d $f24,L.420
b L.422
L.421:
cvt.d.s $f18,$f30
add.d $f24,$f22,$f18
l.d $f16,L.417
div.d $f18,$f18,$f16
cvt.s.d $f30,$f18
L.422:
c.eq.d $f24,$f22; bc1f L.421
l.d $f18,L.418
cvt.d.s $f16,$f30
mul.d $f18,$f18,$f16
cvt.s.d $f18,$f18
s.s $f18,32($30)
lw $25,40($30)
beq $25,$0,L.424
la $4,L.404
lw $5,($30)
la $6,L.426
jal printf  # int
la $4,L.404
lw $5,4($30)
la $6,L.427
jal printf  # int
la $4,L.404
lw $5,8($30)
la $6,L.428
jal printf  # int
la $4,L.404
lw $5,12($30)
la $6,L.429
jal printf  # int
la $4,L.404
lw $5,16($30)
la $6,L.430
jal printf  # int
la $4,L.404
lw $5,20($30)
la $6,L.431
jal printf  # int
la $4,L.404
lw $5,24($30)
la $6,L.432
jal printf  # int
la $4,L.405
l.s $f18,28($30)
cvt.d.s $f18,$f18
mfc1.d $6,$f18
la $25,L.431
sw $25,16($sp)
jal printf  # int
la $4,L.405
l.s $f18,32($30)
cvt.d.s $f18,$f18
mfc1.d $6,$f18
la $25,L.432
sw $25,16($sp)
jal printf  # int
L.424:
move $2,$0
L.402:
l.d $f22,20($sp)
l.d $f24,28($sp)
l.d $f26,36($sp)
l.d $f28,44($sp)
l.d $f30,52($sp)
lw $20,60($sp)
lw $21,64($sp)
lw $22,68($sp)
lw $23,72($sp)
lw $30,76($sp)
lw $31,80($sp)
addu $sp,$sp,88
j $31
.end s26
.data; L.434:.ascii "s4,er%d\012\000"
.data; L.435:.ascii "s4     \000"
.text
.globl s4
.text
.align 2; .ent s4
s4:
$fp11=64
.frame $sp,64,$31
addu $sp,$sp,-64
.mask 0xc0fe0000,-16
sw $17,16($sp)
sw $18,20($sp)
sw $19,24($sp)
sw $20,28($sp)
sw $21,32($sp)
sw $22,36($sp)
sw $23,40($sp)
sw $30,44($sp)
sw $31,48($sp)
move $30,$4
move $18,$0
la $20,L.435
addu $19,$30,60
L.436:
L.437:
move $25,$19
addu $19,$25,1
move $24,$20
addu $20,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.436
move $23,$0
L.439:
move $4,$23
jal svtest  # int
move $17,$2
jal zero  # int
beq $17,$2,L.443
li $18,1
lw $25,44($30)
beq $25,$0,L.445
la $4,L.434
li $5,1
jal printf  # int
L.445:
L.443:
L.440:
addu $23,$23,1
blt $23,3,L.439
jal setev  # int
jal testev  # int
beq $2,$0,L.447
addu $18,$18,2
lw $25,44($30)
beq $25,$0,L.449
la $4,L.434
li $5,2
jal printf  # int
L.449:
L.447:
b L.451
addu $18,$18,4
lw $25,44($30)
beq $25,$0,L.453
la $4,L.434
li $5,4
jal printf  # int
L.453:
L.451:
move $22,$0
subu $22,$22,1
li $21,1
move $23,$0
b L.458
L.455:
and $21,$21,$22
srl $22,$22,1
L.456:
addu $23,$23,1
L.458:
lw $25,($30)
sll $25,$25,2
bltu $23,$25,L.455
bne $21,1,L.461
beq $22,$0,L.459
L.461:
addu $18,$18,8
lw $25,44($30)
beq $25,$0,L.462
la $4,L.434
li $5,8
jal printf  # int
L.462:
L.459:
move $2,$18
L.433:
lw $17,16($sp)
lw $18,20($sp)
lw $19,24($sp)
lw $20,28($sp)
lw $21,32($sp)
lw $22,36($sp)
lw $23,40($sp)
lw $30,44($sp)
lw $31,48($sp)
addu $sp,$sp,64
j $31
.end s4
.lcomm L.466,4
.text
.globl svtest
.text
.align 2; .ent svtest
svtest:
$fp12=8
.frame $sp,8,$31
addu $sp,$sp,-8
beq $4,$0,L.469
beq $4,1,L.470
beq $4,2,L.473
b L.467
L.469:
li $25,1978
sw $25,L.466
sw $0,$fp12-4($sp)
b L.468
L.470:
lw $25,L.466
beq $25,1978,L.471
li $25,1
sw $25,$fp12-4($sp)
b L.468
L.471:
li $25,1929
sw $25,L.466
sw $0,$fp12-4($sp)
b L.468
L.473:
lw $25,L.466
beq $25,1929,L.474
li $25,1
sw $25,$fp12-4($sp)
b L.468
L.474:
sw $0,$fp12-4($sp)
L.467:
L.468:
lw $2,$fp12-4($sp)
L.465:
addu $sp,$sp,8
j $31
.end svtest
.lcomm L.477,4
.text
.globl zero
.text
.align 2; .ent zero
zero:
$fp13=8
.frame $sp,8,$31
addu $sp,$sp,-8
li $25,2
sw $25,L.477
sw $0,$fp13-4($sp)
lw $2,$fp13-4($sp)
L.476:
addu $sp,$sp,8
j $31
.end zero
.globl testev
.text
.align 2; .ent testev
testev:
$fp14=0
.frame $sp,0,$31
lw $25,extvar
beq $25,1066,L.479
li $2,1
b L.478
L.479:
move $2,$0
L.478:
j $31
.end testev
.data; L.482:.ascii "s61,er%d\012\000"
.data; L.483:.ascii "s61    \000"
.data; L.484:.ascii "ABCDEFGHIJKLMNOPQRSTUVWXYZ\000"
.data; L.485:.ascii "abcdefghijklmnopqrstuvwxyz\000"
.data; L.486:.ascii "0123456789\000"
.data; L.487:.ascii "~!\"#%&()_=-^|{}[]+;*:<>,.?/\000"
.data; L.488:.byte 10,9,8,13,12,92,39,0
.data; L.489:.byte 32,0,0
.text
.globl s61
.text
.align 2; .ent s61
s61:
$fp15=88
.frame $sp,88,$31
addu $sp,$sp,-88
.mask 0xc0fe0000,-40
sw $17,16($sp)
sw $18,20($sp)
sw $19,24($sp)
sw $20,28($sp)
sw $21,32($sp)
sw $22,36($sp)
sw $23,40($sp)
sw $30,44($sp)
sw $31,48($sp)
sw $4,88($sp)
la $22,L.483
lw $25,$fp15+0($sp)
addu $21,$25,60
move $19,$0
L.490:
L.491:
move $25,$21
addu $21,$25,1
move $24,$22
addu $22,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.490
li $25,-19
sh $25,$fp15-26($sp)
lh $25,$fp15-26($sp)
sw $25,$fp15-32($sp)
lw $25,$fp15-32($sp)
beq $25,-19,L.493
addu $19,$19,1
lw $25,$fp15+0($sp)
lw $25,44($25)
beq $25,$0,L.495
la $4,L.482
li $5,1
jal printf  # int
L.495:
L.493:
la $25,L.484
sw $25,$fp15-24($sp)
la $25,L.485
sw $25,$fp15-20($sp)
la $25,L.486
sw $25,$fp15-16($sp)
la $25,L.487
sw $25,$fp15-12($sp)
la $25,L.488
sw $25,$fp15-8($sp)
la $25,L.489
sw $25,$fp15-4($sp)
move $23,$0
move $30,$0
b L.507
L.506:
sll $25,$30,2
la $24,$fp15-24($sp)
addu $25,$25,$24
lw $24,($25)
addu $15,$24,1
sw $15,($25)
lb $25,($24)
bge $25,$0,L.509
li $23,1
L.509:
L.507:
sll $25,$30,2
la $24,$fp15-24($sp)
addu $25,$25,$24
lw $25,($25)
lb $25,($25)
bne $25,$0,L.506
L.503:
addu $30,$30,1
blt $30,6,L.507
beq $23,$0,L.511
addu $19,$19,2
lw $25,$fp15+0($sp)
lw $25,44($25)
beq $25,$0,L.513
la $4,L.482
li $5,2
jal printf  # int
L.513:
L.511:
li $20,1048579
move $18,$20
move $17,$20
sll $25,$18,16; sra $25,$25,16
beq $25,$20,L.518
bne $25,3,L.517
L.518:
sll $25,$17,24; sra $25,$25,24
beq $25,$20,L.515
beq $25,3,L.515
L.517:
addu $19,$19,8
lw $25,$fp15+0($sp)
lw $25,44($25)
beq $25,$0,L.519
la $4,L.482
li $5,8
jal printf  # int
L.519:
L.515:
move $2,$19
L.481:
lw $17,16($sp)
lw $18,20($sp)
lw $19,24($sp)
lw $20,28($sp)
lw $21,32($sp)
lw $22,36($sp)
lw $23,40($sp)
lw $30,44($sp)
lw $31,48($sp)
addu $sp,$sp,88
j $31
.end s61
.data; L.522:.ascii "s626,er%d\012\000"
.data; L.523:.ascii "s626   \000"
.text
.globl s626
.text
.align 2; .ent s626
s626:
$fp16=160
.frame $sp,160,$31
addu $sp,$sp,-160
.fmask 0xffc00000,-112
s.d $f22,16($sp)
s.d $f24,24($sp)
s.d $f26,32($sp)
s.d $f28,40($sp)
s.d $f30,48($sp)
.mask 0xc0ff0000,-68
sw $16,56($sp)
sw $17,60($sp)
sw $18,64($sp)
sw $19,68($sp)
sw $20,72($sp)
sw $21,76($sp)
sw $22,80($sp)
sw $23,84($sp)
sw $30,88($sp)
sw $31,92($sp)
move $30,$4
la $20,L.523
addu $19,$30,60
sw $0,$fp16-52($sp)
L.524:
L.525:
move $25,$19
addu $19,$25,1
move $24,$20
addu $20,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.524
l.s $f30,L.412
li $25,1
move $18,$25
move $22,$25
move $23,$0
b L.530
L.527:
l.s $f18,L.531
mul.s $f30,$f18,$f30
sll $25,$22,1
or $22,$25,$18
L.528:
addu $23,$23,1
L.530:
lw $25,12($30)
subu $25,$25,2
blt $23,$25,L.527
mtc1 $22,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
s.s $f18,$fp16-60($sp)
l.s $f18,$fp16-60($sp)
sub.s $f18,$f30,$f18
div.s $f30,$f18,$f30
cvt.d.s $f18,$f30
l.d $f16,L.417
l.s $f10,28($30)
cvt.d.s $f10,$f10
mul.d $f16,$f16,$f10
c.le.d $f18,$f16; bc1t L.532
lw $25,$fp16-52($sp)
addu $25,$25,2
sw $25,$fp16-52($sp)
lw $25,44($30)
beq $25,$0,L.534
la $4,L.522
li $5,2
jal printf  # int
L.534:
L.532:
li $16,125
li $25,125
sh $25,$fp16-30($sp)
li $25,125
sw $25,$fp16-36($sp)
li $25,15625
sw $25,$fp16-44($sp)
li $25,125
sw $25,$fp16-40($sp)
li $25,15625
sw $25,$fp16-48($sp)
li $17,125
li $25,15625
sw $25,$fp16-56($sp)
l.s $f26,L.536
l.d $f24,L.537
l.d $f28,L.538
move $23,$0
L.539:
la $25,$fp16-28($sp)
addu $25,$23,$25
sb $0,($25)
L.540:
addu $23,$23,1
blt $23,28,L.539
sll $25,$16,24; sra $25,$25,24
mul $25,$25,$25
lw $24,$fp16-44($sp)
beq $25,$24,L.543
li $25,1
sb $25,$fp16-28($sp)
L.543:
lh $25,$fp16-30($sp)
sll $24,$16,24; sra $24,$24,24
mul $25,$25,$24
lw $24,$fp16-44($sp)
beq $25,$24,L.545
li $25,1
sb $25,$fp16-27($sp)
L.545:
lh $25,$fp16-30($sp)
mul $25,$25,$25
lw $24,$fp16-44($sp)
beq $25,$24,L.548
li $25,1
sb $25,$fp16-26($sp)
L.548:
lw $25,$fp16-36($sp)
sll $24,$16,24; sra $24,$24,24
mul $25,$25,$24
lw $24,$fp16-44($sp)
beq $25,$24,L.551
li $25,1
sb $25,$fp16-25($sp)
L.551:
lw $25,$fp16-36($sp)
lh $24,$fp16-30($sp)
mul $25,$25,$24
lw $24,$fp16-44($sp)
beq $25,$24,L.554
li $25,1
sb $25,$fp16-24($sp)
L.554:
lw $25,$fp16-36($sp)
mul $25,$25,$25
lw $24,$fp16-44($sp)
beq $25,$24,L.557
li $25,1
sb $25,$fp16-23($sp)
L.557:
lw $25,$fp16-40($sp)
sll $24,$16,24; sra $24,$24,24
mul $25,$25,$24
lw $24,$fp16-48($sp)
beq $25,$24,L.560
li $25,1
sb $25,$fp16-22($sp)
L.560:
lw $25,$fp16-40($sp)
lh $24,$fp16-30($sp)
mul $25,$25,$24
lw $24,$fp16-48($sp)
beq $25,$24,L.563
li $25,1
sb $25,$fp16-21($sp)
L.563:
lw $25,$fp16-40($sp)
lw $24,$fp16-36($sp)
mul $25,$25,$24
lw $24,$fp16-48($sp)
beq $25,$24,L.566
li $25,1
sb $25,$fp16-20($sp)
L.566:
lw $25,$fp16-40($sp)
mul $25,$25,$25
lw $24,$fp16-48($sp)
beq $25,$24,L.569
li $25,1
sb $25,$fp16-19($sp)
L.569:
sll $25,$16,24; sra $25,$25,24
mul $25,$17,$25
lw $24,$fp16-56($sp)
beq $25,$24,L.572
li $25,1
sb $25,$fp16-18($sp)
L.572:
lh $25,$fp16-30($sp)
mul $25,$17,$25
lw $24,$fp16-56($sp)
beq $25,$24,L.575
li $25,1
sb $25,$fp16-17($sp)
L.575:
lw $25,$fp16-36($sp)
mul $25,$17,$25
lw $24,$fp16-56($sp)
beq $25,$24,L.578
li $25,1
sb $25,$fp16-16($sp)
L.578:
lw $25,$fp16-40($sp)
mul $25,$17,$25
lw $24,$fp16-48($sp)
beq $25,$24,L.581
li $25,1
sb $25,$fp16-15($sp)
L.581:
mul $25,$17,$17
lw $24,$fp16-56($sp)
beq $25,$24,L.584
li $25,1
sb $25,$fp16-14($sp)
L.584:
sll $25,$16,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f18,$f26,$f18
cvt.d.s $f18,$f18
c.eq.d $f18,$f28; bc1t L.587
li $25,1
sb $25,$fp16-13($sp)
L.587:
lh $25,$fp16-30($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f18,$f26,$f18
cvt.d.s $f18,$f18
c.eq.d $f18,$f28; bc1t L.590
li $25,1
sb $25,$fp16-12($sp)
L.590:
lw $25,$fp16-36($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f18,$f26,$f18
cvt.d.s $f18,$f18
c.eq.d $f18,$f28; bc1t L.593
li $25,1
sb $25,$fp16-11($sp)
L.593:
lw $25,$fp16-40($sp)
blt $25,$0,L.599
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.600
L.599:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.600:
cvt.s.d $f18,$f22
mul.s $f18,$f26,$f18
cvt.d.s $f18,$f18
c.eq.d $f18,$f28; bc1t L.596
li $25,1
sb $25,$fp16-10($sp)
L.596:
mtc1 $17,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f18,$f26,$f18
cvt.d.s $f18,$f18
c.eq.d $f18,$f28; bc1t L.603
li $25,1
sb $25,$fp16-9($sp)
L.603:
mul.s $f18,$f26,$f26
cvt.d.s $f18,$f18
c.eq.d $f18,$f28; bc1t L.606
li $25,1
sb $25,$fp16-8($sp)
L.606:
sll $25,$16,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f18,$f24,$f18
c.eq.d $f18,$f28; bc1t L.609
li $25,1
sb $25,$fp16-7($sp)
L.609:
lh $25,$fp16-30($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f18,$f24,$f18
c.eq.d $f18,$f28; bc1t L.612
li $25,1
sb $25,$fp16-6($sp)
L.612:
lw $25,$fp16-36($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f18,$f24,$f18
c.eq.d $f18,$f28; bc1t L.615
li $25,1
sb $25,$fp16-5($sp)
L.615:
lw $25,$fp16-40($sp)
blt $25,$0,L.621
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.622
L.621:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.622:
mul.d $f18,$f24,$f22
c.eq.d $f18,$f28; bc1t L.618
li $25,1
sb $25,$fp16-4($sp)
L.618:
mtc1 $17,$f18; cvt.d.w $f18,$f18
mul.d $f18,$f24,$f18
c.eq.d $f18,$f28; bc1t L.624
li $25,1
sb $25,$fp16-3($sp)
L.624:
cvt.d.s $f18,$f26
mul.d $f18,$f24,$f18
c.eq.d $f18,$f28; bc1t L.627
li $25,1
sb $25,$fp16-2($sp)
L.627:
mul.d $f18,$f24,$f24
c.eq.d $f18,$f28; bc1t L.630
li $25,1
sb $25,$fp16-1($sp)
L.630:
move $21,$0
move $23,$0
L.633:
sll $25,$21,24; sra $25,$25,24
la $24,$fp16-28($sp)
addu $24,$23,$24
lb $24,($24)
addu $21,$25,$24
L.634:
addu $23,$23,1
blt $23,28,L.633
sll $25,$21,24; sra $25,$25,24
beq $25,$0,L.637
lw $25,$fp16-52($sp)
addu $25,$25,4
sw $25,$fp16-52($sp)
lw $25,44($30)
beq $25,$0,L.639
la $4,L.522
li $5,4
jal printf  # int
la $4,L.641
jal printf  # int
move $23,$0
L.642:
la $4,L.646
la $25,$fp16-28($sp)
addu $25,$23,$25
lb $5,($25)
jal printf  # int
L.643:
addu $23,$23,1
blt $23,28,L.642
la $4,L.647
jal printf  # int
L.639:
L.637:
li $17,32768
bleu $17,0x8000,L.648
lw $25,$fp16-52($sp)
addu $25,$25,8
sw $25,$fp16-52($sp)
lw $25,44($30)
beq $25,$0,L.650
la $4,L.522
li $5,8
jal printf  # int
L.650:
L.648:
lw $2,$fp16-52($sp)
L.521:
l.d $f22,16($sp)
l.d $f24,24($sp)
l.d $f26,32($sp)
l.d $f28,40($sp)
l.d $f30,48($sp)
lw $16,56($sp)
lw $17,60($sp)
lw $18,64($sp)
lw $19,68($sp)
lw $20,72($sp)
lw $21,76($sp)
lw $22,80($sp)
lw $23,84($sp)
lw $30,88($sp)
lw $31,92($sp)
addu $sp,$sp,160
j $31
.end s626
.data; L.653:.ascii "s71,er%d\012\000"
.data; L.654:.ascii "s71    \000"
.sdata; L.655:.byte 113
.text
.globl s71
.text
.align 2; .ent s71
s71:
$fp17=88
.frame $sp,88,$31
addu $sp,$sp,-88
.mask 0xc0f00000,-52
sw $20,16($sp)
sw $21,20($sp)
sw $22,24($sp)
sw $23,28($sp)
sw $30,32($sp)
sw $31,36($sp)
move $30,$4
la $23,L.654
addu $22,$30,60
move $21,$0
L.656:
L.657:
move $25,$22
addu $22,$25,1
move $24,$23
addu $23,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.656
lb $25,L.661
lb $24,L.655
beq $25,$24,L.659
addu $21,$21,1
lw $25,44($30)
beq $25,$0,L.662
la $4,L.653
li $5,1
jal printf  # int
L.662:
L.659:
b L.664
addu $21,$21,2
lw $25,44($30)
beq $25,$0,L.666
la $4,L.653
li $5,2
jal printf  # int
L.666:
L.664:
li $25,1942
sw $25,$fp17-20($sp)
lw $25,$fp17-20($sp)
bne $25,1942,L.674
lw $25,$fp17-20($sp)
lw $24,$fp17-20($sp)
beq $25,$24,L.669
L.674:
addu $21,$21,4
lw $25,44($30)
beq $25,$0,L.675
la $4,L.653
li $5,4
jal printf  # int
L.675:
L.669:
li $4,-5
jal McCarthy  # int
beq $2,91,L.677
addu $21,$21,8
lw $25,44($30)
beq $25,$0,L.679
la $4,L.653
li $5,8
jal printf  # int
L.679:
L.677:
li $20,2
li $25,3
sw $25,$fp17-44($sp)
la $25,$fp17-44($sp)
sw $25,$fp17-48($sp)
move $4,$20
lw $5,$fp17-48($sp)
jal clobber  # int
bne $20,2,L.683
lw $25,$fp17-44($sp)
beq $25,2,L.681
L.683:
addu $21,$21,16
lw $25,44($30)
beq $25,$0,L.684
la $4,L.653
li $5,16
jal printf  # int
L.684:
L.681:
l.s $f18,32($30)
c.eq.s $f18,$f18; bc1t L.686
addu $21,$21,32
lw $25,44($30)
beq $25,$0,L.688
la $4,L.653
li $5,32
jal printf  # int
L.688:
L.686:
move $2,$21
L.652:
lw $20,16($sp)
lw $21,20($sp)
lw $22,24($sp)
lw $23,28($sp)
lw $30,32($sp)
lw $31,36($sp)
addu $sp,$sp,88
j $31
.end s71
.globl McCarthy
.text
.align 2; .ent McCarthy
McCarthy:
$fp18=24
.frame $sp,24,$31
addu $sp,$sp,-24
.mask 0x80000000,-8
sw $31,16($sp)
sw $4,24($sp)
lw $25,$fp18+0($sp)
ble $25,100,L.691
lw $25,$fp18+0($sp)
subu $2,$25,10
b L.690
L.691:
lw $25,$fp18+0($sp)
addu $4,$25,11
jal McCarthy  # int
move $4,$2
jal McCarthy  # int
L.690:
lw $31,16($sp)
addu $sp,$sp,24
j $31
.end McCarthy
.globl clobber
.text
.align 2; .ent clobber
clobber:
$fp19=0
.frame $sp,0,$31
li $4,3
li $25,2
sw $25,($5)
L.693:
j $31
.end clobber
.data; L.695:.ascii "Local error %d.\012\000"
.data; L.696:.ascii "s714,er%d\012\000"
.data; L.697:.ascii "s714   \000"
.text
.globl s714
.text
.align 2; .ent s714
s714:
$fp20=160
.frame $sp,160,$31
addu $sp,$sp,-160
.fmask 0xffc00000,-112
s.d $f22,16($sp)
s.d $f24,24($sp)
s.d $f26,32($sp)
s.d $f28,40($sp)
s.d $f30,48($sp)
.mask 0xc0ff0000,-68
sw $16,56($sp)
sw $17,60($sp)
sw $18,64($sp)
sw $19,68($sp)
sw $20,72($sp)
sw $21,76($sp)
sw $22,80($sp)
sw $23,84($sp)
sw $30,88($sp)
sw $31,92($sp)
sw $4,160($sp)
la $25,L.697
sw $25,$fp20-16($sp)
lw $25,$fp20+0($sp)
addu $24,$25,60
sw $24,$fp20-20($sp)
sw $0,$fp20-24($sp)
move $23,$0
lw $30,48($25)
L.698:
L.699:
lw $25,$fp20-20($sp)
addu $24,$25,1
sw $24,$fp20-20($sp)
lw $24,$fp20-16($sp)
addu $15,$24,1
sw $15,$fp20-16($sp)
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.698
li $22,5
li $17,2
move $22,$17
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.701
li $23,1
beq $30,$0,L.703
la $4,L.695
move $5,$23
jal printf  # int
L.703:
L.701:
li $22,5
li $16,2
sll $25,$16,16; sra $25,$25,16
move $22,$25
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.705
li $23,2
beq $30,$0,L.707
la $4,L.695
move $5,$23
jal printf  # int
L.707:
L.705:
li $22,5
li $25,2
sw $25,$fp20-4($sp)
lw $22,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.709
li $23,3
beq $30,$0,L.711
la $4,L.695
move $5,$23
jal printf  # int
L.711:
L.709:
li $22,5
li $25,2
sw $25,$fp20-8($sp)
lw $22,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.713
li $23,4
beq $30,$0,L.715
la $4,L.695
move $5,$23
jal printf  # int
L.715:
L.713:
li $22,5
li $25,2
sw $25,$fp20-12($sp)
lw $22,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.717
li $23,5
beq $30,$0,L.719
la $4,L.695
move $5,$23
jal printf  # int
L.719:
L.717:
li $22,5
l.s $f26,L.531
trunc.w.s $f2,$f26,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.721
li $23,6
beq $30,$0,L.723
la $4,L.695
move $5,$23
jal printf  # int
L.723:
L.721:
li $22,5
l.d $f24,L.417
trunc.w.d $f2,$f24,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.725
li $23,7
beq $30,$0,L.727
la $4,L.695
move $5,$23
jal printf  # int
L.727:
L.725:
li $21,5
li $17,2
sll $25,$17,24; sra $25,$25,24
move $21,$25
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.729
li $23,8
beq $30,$0,L.731
la $4,L.695
move $5,$23
jal printf  # int
L.731:
L.729:
li $21,5
li $16,2
move $21,$16
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.733
li $23,9
beq $30,$0,L.735
la $4,L.695
move $5,$23
jal printf  # int
L.735:
L.733:
li $21,5
li $25,2
sw $25,$fp20-4($sp)
lw $21,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.737
li $23,10
beq $30,$0,L.739
la $4,L.695
move $5,$23
jal printf  # int
L.739:
L.737:
li $21,5
li $25,2
sw $25,$fp20-8($sp)
lw $21,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.741
li $23,11
beq $30,$0,L.743
la $4,L.695
move $5,$23
jal printf  # int
L.743:
L.741:
li $21,5
li $25,2
sw $25,$fp20-12($sp)
lw $21,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.745
li $23,12
beq $30,$0,L.747
la $4,L.695
move $5,$23
jal printf  # int
L.747:
L.745:
li $21,5
l.s $f26,L.531
trunc.w.s $f2,$f26,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.749
li $23,13
beq $30,$0,L.751
la $4,L.695
move $5,$23
jal printf  # int
L.751:
L.749:
li $21,5
l.d $f24,L.417
trunc.w.d $f2,$f24,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.753
li $23,14
beq $30,$0,L.755
la $4,L.695
move $5,$23
jal printf  # int
L.755:
L.753:
li $20,5
li $17,2
sll $25,$17,24; sra $25,$25,24
move $20,$25
beq $20,2,L.757
li $23,15
beq $30,$0,L.759
la $4,L.695
move $5,$23
jal printf  # int
L.759:
L.757:
li $20,5
li $16,2
sll $25,$16,16; sra $25,$25,16
move $20,$25
beq $20,2,L.761
li $23,16
beq $30,$0,L.763
la $4,L.695
move $5,$23
jal printf  # int
L.763:
L.761:
li $20,5
li $25,2
sw $25,$fp20-4($sp)
lw $20,$fp20-4($sp)
beq $20,2,L.765
li $23,17
beq $30,$0,L.767
la $4,L.695
move $5,$23
jal printf  # int
L.767:
L.765:
li $20,5
li $25,2
sw $25,$fp20-8($sp)
lw $20,$fp20-8($sp)
beq $20,2,L.769
li $23,18
beq $30,$0,L.771
la $4,L.695
move $5,$23
jal printf  # int
L.771:
L.769:
li $20,5
li $25,2
sw $25,$fp20-12($sp)
lw $20,$fp20-12($sp)
beq $20,2,L.773
li $23,19
beq $30,$0,L.775
la $4,L.695
move $5,$23
jal printf  # int
L.775:
L.773:
li $20,5
l.s $f26,L.531
trunc.w.s $f2,$f26,$20; mfc1 $20,$f2
beq $20,2,L.777
li $23,20
beq $30,$0,L.779
la $4,L.695
move $5,$23
jal printf  # int
L.779:
L.777:
li $20,5
l.d $f24,L.417
trunc.w.d $f2,$f24,$20; mfc1 $20,$f2
beq $20,2,L.781
li $23,21
beq $30,$0,L.783
la $4,L.695
move $5,$23
jal printf  # int
L.783:
L.781:
li $19,5
li $17,2
sll $25,$17,24; sra $25,$25,24
move $19,$25
beq $19,2,L.785
li $23,22
beq $30,$0,L.787
la $4,L.695
move $5,$23
jal printf  # int
L.787:
L.785:
li $19,5
li $16,2
sll $25,$16,16; sra $25,$25,16
move $19,$25
beq $19,2,L.789
li $23,23
beq $30,$0,L.791
la $4,L.695
move $5,$23
jal printf  # int
L.791:
L.789:
li $19,5
li $25,2
sw $25,$fp20-4($sp)
lw $19,$fp20-4($sp)
beq $19,2,L.793
li $23,24
beq $30,$0,L.795
la $4,L.695
move $5,$23
jal printf  # int
L.795:
L.793:
li $19,5
li $25,2
sw $25,$fp20-8($sp)
lw $19,$fp20-8($sp)
beq $19,2,L.797
li $23,25
beq $30,$0,L.799
la $4,L.695
move $5,$23
jal printf  # int
L.799:
L.797:
li $19,5
li $25,2
sw $25,$fp20-12($sp)
lw $19,$fp20-12($sp)
beq $19,2,L.801
li $23,26
beq $30,$0,L.803
la $4,L.695
move $5,$23
jal printf  # int
L.803:
L.801:
li $19,5
l.s $f26,L.531
trunc.w.s $f2,$f26,$19; mfc1 $19,$f2
beq $19,2,L.805
li $23,27
beq $30,$0,L.807
la $4,L.695
move $5,$23
jal printf  # int
L.807:
L.805:
li $19,5
l.d $f24,L.417
trunc.w.d $f2,$f24,$19; mfc1 $19,$f2
beq $19,2,L.809
li $23,28
beq $30,$0,L.811
la $4,L.695
move $5,$23
jal printf  # int
L.811:
L.809:
li $18,5
li $17,2
sll $25,$17,24; sra $25,$25,24
move $18,$25
beq $18,2,L.813
li $23,29
beq $30,$0,L.815
la $4,L.695
move $5,$23
jal printf  # int
L.815:
L.813:
li $18,5
li $16,2
sll $25,$16,16; sra $25,$25,16
move $18,$25
beq $18,2,L.817
li $23,30
beq $30,$0,L.819
la $4,L.695
move $5,$23
jal printf  # int
L.819:
L.817:
li $18,5
li $25,2
sw $25,$fp20-4($sp)
lw $18,$fp20-4($sp)
beq $18,2,L.821
li $23,31
beq $30,$0,L.823
la $4,L.695
move $5,$23
jal printf  # int
L.823:
L.821:
li $18,5
li $25,2
sw $25,$fp20-8($sp)
lw $18,$fp20-8($sp)
beq $18,2,L.825
li $23,32
beq $30,$0,L.827
la $4,L.695
move $5,$23
jal printf  # int
L.827:
L.825:
li $18,5
li $25,2
sw $25,$fp20-12($sp)
lw $18,$fp20-12($sp)
beq $18,2,L.829
li $23,33
beq $30,$0,L.831
la $4,L.695
move $5,$23
jal printf  # int
L.831:
L.829:
li $18,5
l.s $f26,L.531
cvt.d.s $f18,$f26
l.d $f16,L.836
c.lt.d $f18,$f16; bc1t L.834
sub.d $f18,$f18,$f16
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-28($sp)
b L.835
L.834:
trunc.w.s $f2,$f26,$25; mfc1 $25,$f2
sw $25,$fp20-28($sp)
L.835:
lw $18,$fp20-28($sp)
beq $18,2,L.837
li $23,34
beq $30,$0,L.839
la $4,L.695
move $5,$23
jal printf  # int
L.839:
L.837:
li $18,5
l.d $f24,L.417
l.d $f18,L.836
c.lt.d $f24,$f18; bc1t L.842
sub.d $f18,$f24,$f18
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-32($sp)
b L.843
L.842:
trunc.w.d $f2,$f24,$25; mfc1 $25,$f2
sw $25,$fp20-32($sp)
L.843:
lw $18,$fp20-32($sp)
beq $18,2,L.844
li $23,35
beq $30,$0,L.846
la $4,L.695
move $5,$23
jal printf  # int
L.846:
L.844:
l.s $f30,L.848
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f30,$f18
l.s $f18,L.531
c.eq.s $f30,$f18; bc1t L.849
li $23,36
beq $30,$0,L.851
la $4,L.695
move $5,$23
jal printf  # int
L.851:
L.849:
l.s $f30,L.848
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f30,$f18
l.s $f18,L.531
c.eq.s $f30,$f18; bc1t L.853
li $23,37
beq $30,$0,L.855
la $4,L.695
move $5,$23
jal printf  # int
L.855:
L.853:
l.s $f30,L.848
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f30,$f18
l.s $f18,L.531
c.eq.s $f30,$f18; bc1t L.857
li $23,38
beq $30,$0,L.859
la $4,L.695
move $5,$23
jal printf  # int
L.859:
L.857:
l.s $f30,L.848
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f30,$f18
l.s $f18,L.531
c.eq.s $f30,$f18; bc1t L.861
li $23,39
beq $30,$0,L.863
la $4,L.695
move $5,$23
jal printf  # int
L.863:
L.861:
l.s $f30,L.848
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.866
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.867
L.866:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.867:
cvt.s.d $f30,$f22
l.s $f18,L.531
c.eq.s $f30,$f18; bc1t L.868
li $23,40
beq $30,$0,L.870
la $4,L.695
move $5,$23
jal printf  # int
L.870:
L.868:
l.s $f30,L.848
l.s $f18,L.531
mov.s $f26,$f18
mov.s $f30,$f26
c.eq.s $f30,$f18; bc1t L.872
li $23,41
beq $30,$0,L.874
la $4,L.695
move $5,$23
jal printf  # int
L.874:
L.872:
l.s $f30,L.848
l.d $f24,L.417
cvt.s.d $f30,$f24
l.s $f18,L.531
c.eq.s $f30,$f18; bc1t L.876
li $23,42
beq $30,$0,L.878
la $4,L.695
move $5,$23
jal printf  # int
L.878:
L.876:
l.d $f28,L.880
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f28; cvt.d.w $f28,$f28
l.d $f18,L.417
c.eq.d $f28,$f18; bc1t L.881
li $23,43
beq $30,$0,L.883
la $4,L.695
move $5,$23
jal printf  # int
L.883:
L.881:
l.d $f28,L.880
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f28; cvt.d.w $f28,$f28
l.d $f18,L.417
c.eq.d $f28,$f18; bc1t L.885
li $23,44
beq $30,$0,L.887
la $4,L.695
move $5,$23
jal printf  # int
L.887:
L.885:
l.d $f28,L.880
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f28; cvt.d.w $f28,$f28
l.d $f18,L.417
c.eq.d $f28,$f18; bc1t L.889
li $23,45
beq $30,$0,L.891
la $4,L.695
move $5,$23
jal printf  # int
L.891:
L.889:
l.d $f28,L.880
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f28; cvt.d.w $f28,$f28
l.d $f18,L.417
c.eq.d $f28,$f18; bc1t L.893
li $23,46
beq $30,$0,L.895
la $4,L.695
move $5,$23
jal printf  # int
L.895:
L.893:
l.d $f28,L.880
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.898
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.899
L.898:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.899:
mov.d $f28,$f22
l.d $f18,L.417
c.eq.d $f28,$f18; bc1t L.900
li $23,47
beq $30,$0,L.902
la $4,L.695
move $5,$23
jal printf  # int
L.902:
L.900:
l.d $f28,L.880
l.s $f26,L.531
cvt.d.s $f28,$f26
l.d $f18,L.417
c.eq.d $f28,$f18; bc1t L.904
li $23,48
beq $30,$0,L.906
la $4,L.695
move $5,$23
jal printf  # int
L.906:
L.904:
l.d $f28,L.880
l.d $f18,L.417
mov.d $f24,$f18
mov.d $f28,$f24
c.eq.d $f28,$f18; bc1t L.908
li $23,49
beq $30,$0,L.910
la $4,L.695
move $5,$23
jal printf  # int
L.910:
L.908:
li $22,5
li $17,2
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
addu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,7,L.912
li $23,50
beq $30,$0,L.914
la $4,L.695
move $5,$23
jal printf  # int
L.914:
L.912:
li $22,5
li $16,2
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
addu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,7,L.916
li $23,51
beq $30,$0,L.918
la $4,L.695
move $5,$23
jal printf  # int
L.918:
L.916:
li $22,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
addu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,7,L.920
li $23,52
beq $30,$0,L.922
la $4,L.695
move $5,$23
jal printf  # int
L.922:
L.920:
li $22,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
addu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,7,L.924
li $23,53
beq $30,$0,L.926
la $4,L.695
move $5,$23
jal printf  # int
L.926:
L.924:
li $22,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
addu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,7,L.928
li $23,54
beq $30,$0,L.930
la $4,L.695
move $5,$23
jal printf  # int
L.930:
L.928:
li $22,5
l.s $f26,L.531
sll $25,$22,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
add.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,7,L.932
li $23,55
beq $30,$0,L.934
la $4,L.695
move $5,$23
jal printf  # int
L.934:
L.932:
li $22,5
l.d $f24,L.417
sll $25,$22,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
add.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,7,L.936
li $23,56
beq $30,$0,L.938
la $4,L.695
move $5,$23
jal printf  # int
L.938:
L.936:
li $21,5
li $17,2
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
addu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,7,L.940
li $23,57
beq $30,$0,L.942
la $4,L.695
move $5,$23
jal printf  # int
L.942:
L.940:
li $21,5
li $16,2
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
addu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,7,L.944
li $23,58
beq $30,$0,L.946
la $4,L.695
move $5,$23
jal printf  # int
L.946:
L.944:
li $21,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
addu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,7,L.948
li $23,59
beq $30,$0,L.950
la $4,L.695
move $5,$23
jal printf  # int
L.950:
L.948:
li $21,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
addu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,7,L.952
li $23,60
beq $30,$0,L.954
la $4,L.695
move $5,$23
jal printf  # int
L.954:
L.952:
li $21,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
addu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,7,L.956
li $23,61
beq $30,$0,L.958
la $4,L.695
move $5,$23
jal printf  # int
L.958:
L.956:
li $21,5
l.s $f26,L.531
sll $25,$21,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
add.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,7,L.960
li $23,62
beq $30,$0,L.962
la $4,L.695
move $5,$23
jal printf  # int
L.962:
L.960:
li $21,5
l.d $f24,L.417
sll $25,$21,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
add.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,7,L.964
li $23,63
beq $30,$0,L.966
la $4,L.695
move $5,$23
jal printf  # int
L.966:
L.964:
li $20,5
li $17,2
sll $25,$17,24; sra $25,$25,24
addu $20,$20,$25
beq $20,7,L.968
li $23,64
beq $30,$0,L.970
la $4,L.695
move $5,$23
jal printf  # int
L.970:
L.968:
li $20,5
li $16,2
sll $25,$16,16; sra $25,$25,16
addu $20,$20,$25
beq $20,7,L.972
li $23,65
beq $30,$0,L.974
la $4,L.695
move $5,$23
jal printf  # int
L.974:
L.972:
li $20,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
addu $20,$20,$25
beq $20,7,L.976
li $23,66
beq $30,$0,L.978
la $4,L.695
move $5,$23
jal printf  # int
L.978:
L.976:
li $20,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
addu $20,$20,$25
beq $20,7,L.980
li $23,67
beq $30,$0,L.982
la $4,L.695
move $5,$23
jal printf  # int
L.982:
L.980:
li $20,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
addu $20,$20,$25
beq $20,7,L.984
li $23,68
beq $30,$0,L.986
la $4,L.695
move $5,$23
jal printf  # int
L.986:
L.984:
li $20,5
l.s $f26,L.531
mtc1 $20,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
add.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$20; mfc1 $20,$f2
beq $20,7,L.988
li $23,69
beq $30,$0,L.990
la $4,L.695
move $5,$23
jal printf  # int
L.990:
L.988:
li $20,5
l.d $f24,L.417
mtc1 $20,$f18; cvt.d.w $f18,$f18
add.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$20; mfc1 $20,$f2
beq $20,7,L.992
li $23,70
beq $30,$0,L.994
la $4,L.695
move $5,$23
jal printf  # int
L.994:
L.992:
li $19,5
li $17,2
sll $25,$17,24; sra $25,$25,24
addu $19,$19,$25
beq $19,7,L.996
li $23,71
beq $30,$0,L.998
la $4,L.695
move $5,$23
jal printf  # int
L.998:
L.996:
li $19,5
li $16,2
sll $25,$16,16; sra $25,$25,16
addu $19,$19,$25
beq $19,7,L.1000
li $23,72
beq $30,$0,L.1002
la $4,L.695
move $5,$23
jal printf  # int
L.1002:
L.1000:
li $19,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
addu $19,$19,$25
beq $19,7,L.1004
li $23,73
beq $30,$0,L.1006
la $4,L.695
move $5,$23
jal printf  # int
L.1006:
L.1004:
li $19,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
addu $19,$19,$25
beq $19,7,L.1008
li $23,74
beq $30,$0,L.1010
la $4,L.695
move $5,$23
jal printf  # int
L.1010:
L.1008:
li $19,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
addu $19,$19,$25
beq $19,7,L.1012
li $23,75
beq $30,$0,L.1014
la $4,L.695
move $5,$23
jal printf  # int
L.1014:
L.1012:
li $19,5
l.s $f26,L.531
mtc1 $19,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
add.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$19; mfc1 $19,$f2
beq $19,7,L.1016
li $23,76
beq $30,$0,L.1018
la $4,L.695
move $5,$23
jal printf  # int
L.1018:
L.1016:
li $19,5
l.d $f24,L.417
mtc1 $19,$f18; cvt.d.w $f18,$f18
add.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$19; mfc1 $19,$f2
beq $19,7,L.1020
li $23,77
beq $30,$0,L.1022
la $4,L.695
move $5,$23
jal printf  # int
L.1022:
L.1020:
li $18,5
li $17,2
sll $25,$17,24; sra $25,$25,24
addu $18,$18,$25
beq $18,7,L.1024
li $23,78
beq $30,$0,L.1026
la $4,L.695
move $5,$23
jal printf  # int
L.1026:
L.1024:
li $18,5
li $16,2
sll $25,$16,16; sra $25,$25,16
addu $18,$18,$25
beq $18,7,L.1028
li $23,79
beq $30,$0,L.1030
la $4,L.695
move $5,$23
jal printf  # int
L.1030:
L.1028:
li $18,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
addu $18,$18,$25
beq $18,7,L.1032
li $23,80
beq $30,$0,L.1034
la $4,L.695
move $5,$23
jal printf  # int
L.1034:
L.1032:
li $18,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
addu $18,$18,$25
beq $18,7,L.1036
li $23,81
beq $30,$0,L.1038
la $4,L.695
move $5,$23
jal printf  # int
L.1038:
L.1036:
li $18,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
addu $18,$18,$25
beq $18,7,L.1040
li $23,82
beq $30,$0,L.1042
la $4,L.695
move $5,$23
jal printf  # int
L.1042:
L.1040:
li $18,5
l.s $f26,L.531
blt $18,$0,L.1048
mtc1 $18,$f22; cvt.d.w $f22,$f22
b L.1049
L.1048:
mtc1 $18,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1049:
cvt.s.d $f18,$f22
add.s $f18,$f18,$f26
cvt.d.s $f16,$f18
l.d $f10,L.836
c.lt.d $f16,$f10; bc1t L.1046
sub.d $f16,$f16,$f10
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-36($sp)
b L.1047
L.1046:
trunc.w.s $f2,$f18,$25; mfc1 $25,$f2
sw $25,$fp20-36($sp)
L.1047:
lw $18,$fp20-36($sp)
beq $18,7,L.1050
li $23,83
beq $30,$0,L.1052
la $4,L.695
move $5,$23
jal printf  # int
L.1052:
L.1050:
li $18,5
l.d $f24,L.417
blt $18,$0,L.1058
mtc1 $18,$f22; cvt.d.w $f22,$f22
b L.1059
L.1058:
mtc1 $18,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1059:
add.d $f18,$f22,$f24
l.d $f16,L.836
c.lt.d $f18,$f16; bc1t L.1056
sub.d $f16,$f18,$f16
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-40($sp)
b L.1057
L.1056:
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
sw $25,$fp20-40($sp)
L.1057:
lw $18,$fp20-40($sp)
beq $18,7,L.1060
li $23,84
beq $30,$0,L.1062
la $4,L.695
move $5,$23
jal printf  # int
L.1062:
L.1060:
l.s $f30,L.848
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
add.s $f30,$f30,$f18
l.s $f18,L.1066
c.eq.s $f30,$f18; bc1t L.1064
li $23,85
beq $30,$0,L.1067
la $4,L.695
move $5,$23
jal printf  # int
L.1067:
L.1064:
l.s $f30,L.848
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
add.s $f30,$f30,$f18
l.s $f18,L.1066
c.eq.s $f30,$f18; bc1t L.1069
li $23,86
beq $30,$0,L.1071
la $4,L.695
move $5,$23
jal printf  # int
L.1071:
L.1069:
l.s $f30,L.848
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
add.s $f30,$f30,$f18
l.s $f18,L.1066
c.eq.s $f30,$f18; bc1t L.1073
li $23,87
beq $30,$0,L.1075
la $4,L.695
move $5,$23
jal printf  # int
L.1075:
L.1073:
l.s $f30,L.848
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
add.s $f30,$f30,$f18
l.s $f18,L.1066
c.eq.s $f30,$f18; bc1t L.1077
li $23,88
beq $30,$0,L.1079
la $4,L.695
move $5,$23
jal printf  # int
L.1079:
L.1077:
l.s $f30,L.848
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.1082
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.1083
L.1082:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1083:
cvt.s.d $f18,$f22
add.s $f30,$f30,$f18
l.s $f18,L.1066
c.eq.s $f30,$f18; bc1t L.1084
li $23,89
beq $30,$0,L.1086
la $4,L.695
move $5,$23
jal printf  # int
L.1086:
L.1084:
l.s $f30,L.848
l.s $f26,L.531
add.s $f30,$f30,$f26
l.s $f18,L.1066
c.eq.s $f30,$f18; bc1t L.1088
li $23,90
beq $30,$0,L.1090
la $4,L.695
move $5,$23
jal printf  # int
L.1090:
L.1088:
l.s $f30,L.848
l.d $f24,L.417
cvt.d.s $f18,$f30
add.d $f18,$f18,$f24
cvt.s.d $f30,$f18
l.s $f18,L.1066
c.eq.s $f30,$f18; bc1t L.1092
li $23,91
beq $30,$0,L.1094
la $4,L.695
move $5,$23
jal printf  # int
L.1094:
L.1092:
l.d $f28,L.880
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
add.d $f28,$f28,$f18
l.d $f18,L.1098
c.eq.d $f28,$f18; bc1t L.1096
li $23,92
beq $30,$0,L.1099
la $4,L.695
move $5,$23
jal printf  # int
L.1099:
L.1096:
l.d $f28,L.880
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
add.d $f28,$f28,$f18
l.d $f18,L.1098
c.eq.d $f28,$f18; bc1t L.1101
li $23,93
beq $30,$0,L.1103
la $4,L.695
move $5,$23
jal printf  # int
L.1103:
L.1101:
l.d $f28,L.880
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
add.d $f28,$f28,$f18
l.d $f18,L.1098
c.eq.d $f28,$f18; bc1t L.1105
li $23,94
beq $30,$0,L.1107
la $4,L.695
move $5,$23
jal printf  # int
L.1107:
L.1105:
l.d $f28,L.880
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
add.d $f28,$f28,$f18
l.d $f18,L.1098
c.eq.d $f28,$f18; bc1t L.1109
li $23,95
beq $30,$0,L.1111
la $4,L.695
move $5,$23
jal printf  # int
L.1111:
L.1109:
l.d $f28,L.880
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.1114
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.1115
L.1114:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1115:
add.d $f28,$f28,$f22
l.d $f18,L.1098
c.eq.d $f28,$f18; bc1t L.1116
li $23,96
beq $30,$0,L.1118
la $4,L.695
move $5,$23
jal printf  # int
L.1118:
L.1116:
l.d $f28,L.880
l.s $f26,L.531
cvt.d.s $f18,$f26
add.d $f28,$f28,$f18
l.d $f18,L.1098
c.eq.d $f28,$f18; bc1t L.1120
li $23,97
beq $30,$0,L.1122
la $4,L.695
move $5,$23
jal printf  # int
L.1122:
L.1120:
l.d $f28,L.880
l.d $f24,L.417
add.d $f28,$f28,$f24
l.d $f18,L.1098
c.eq.d $f28,$f18; bc1t L.1124
li $23,98
beq $30,$0,L.1126
la $4,L.695
move $5,$23
jal printf  # int
L.1126:
L.1124:
li $22,5
li $17,2
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
subu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,3,L.1128
li $23,99
beq $30,$0,L.1130
la $4,L.695
move $5,$23
jal printf  # int
L.1130:
L.1128:
li $22,5
li $16,2
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
subu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,3,L.1132
li $23,100
beq $30,$0,L.1134
la $4,L.695
move $5,$23
jal printf  # int
L.1134:
L.1132:
li $22,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
subu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,3,L.1136
li $23,101
beq $30,$0,L.1138
la $4,L.695
move $5,$23
jal printf  # int
L.1138:
L.1136:
li $22,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
subu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,3,L.1140
li $23,102
beq $30,$0,L.1142
la $4,L.695
move $5,$23
jal printf  # int
L.1142:
L.1140:
li $22,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
subu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,3,L.1144
li $23,103
beq $30,$0,L.1146
la $4,L.695
move $5,$23
jal printf  # int
L.1146:
L.1144:
li $22,5
l.s $f26,L.531
sll $25,$22,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
sub.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,3,L.1148
li $23,104
beq $30,$0,L.1150
la $4,L.695
move $5,$23
jal printf  # int
L.1150:
L.1148:
li $22,5
l.d $f24,L.417
sll $25,$22,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
sub.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,3,L.1152
li $23,105
beq $30,$0,L.1154
la $4,L.695
move $5,$23
jal printf  # int
L.1154:
L.1152:
li $21,5
li $17,2
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
subu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,3,L.1156
li $23,106
beq $30,$0,L.1158
la $4,L.695
move $5,$23
jal printf  # int
L.1158:
L.1156:
li $21,5
li $16,2
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
subu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,3,L.1160
li $23,107
beq $30,$0,L.1162
la $4,L.695
move $5,$23
jal printf  # int
L.1162:
L.1160:
li $21,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
subu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,3,L.1164
li $23,108
beq $30,$0,L.1166
la $4,L.695
move $5,$23
jal printf  # int
L.1166:
L.1164:
li $21,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
subu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,3,L.1168
li $23,109
beq $30,$0,L.1170
la $4,L.695
move $5,$23
jal printf  # int
L.1170:
L.1168:
li $21,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
subu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,3,L.1172
li $23,110
beq $30,$0,L.1174
la $4,L.695
move $5,$23
jal printf  # int
L.1174:
L.1172:
li $21,5
l.s $f26,L.531
sll $25,$21,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
sub.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,3,L.1176
li $23,111
beq $30,$0,L.1178
la $4,L.695
move $5,$23
jal printf  # int
L.1178:
L.1176:
li $21,5
l.d $f24,L.417
sll $25,$21,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
sub.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,3,L.1180
li $23,112
beq $30,$0,L.1182
la $4,L.695
move $5,$23
jal printf  # int
L.1182:
L.1180:
li $20,5
li $17,2
sll $25,$17,24; sra $25,$25,24
subu $20,$20,$25
beq $20,3,L.1184
li $23,113
beq $30,$0,L.1186
la $4,L.695
move $5,$23
jal printf  # int
L.1186:
L.1184:
li $20,5
li $16,2
sll $25,$16,16; sra $25,$25,16
subu $20,$20,$25
beq $20,3,L.1188
li $23,114
beq $30,$0,L.1190
la $4,L.695
move $5,$23
jal printf  # int
L.1190:
L.1188:
li $20,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
subu $20,$20,$25
beq $20,3,L.1192
li $23,115
beq $30,$0,L.1194
la $4,L.695
move $5,$23
jal printf  # int
L.1194:
L.1192:
li $20,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
subu $20,$20,$25
beq $20,3,L.1196
li $23,116
beq $30,$0,L.1198
la $4,L.695
move $5,$23
jal printf  # int
L.1198:
L.1196:
li $20,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
subu $20,$20,$25
beq $20,3,L.1200
li $23,117
beq $30,$0,L.1202
la $4,L.695
move $5,$23
jal printf  # int
L.1202:
L.1200:
li $20,5
l.s $f26,L.531
mtc1 $20,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
sub.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$20; mfc1 $20,$f2
beq $20,3,L.1204
li $23,118
beq $30,$0,L.1206
la $4,L.695
move $5,$23
jal printf  # int
L.1206:
L.1204:
li $20,5
l.d $f24,L.417
mtc1 $20,$f18; cvt.d.w $f18,$f18
sub.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$20; mfc1 $20,$f2
beq $20,3,L.1208
li $23,119
beq $30,$0,L.1210
la $4,L.695
move $5,$23
jal printf  # int
L.1210:
L.1208:
li $19,5
li $17,2
sll $25,$17,24; sra $25,$25,24
subu $19,$19,$25
beq $19,3,L.1212
li $23,120
beq $30,$0,L.1214
la $4,L.695
move $5,$23
jal printf  # int
L.1214:
L.1212:
li $19,5
li $16,2
sll $25,$16,16; sra $25,$25,16
subu $19,$19,$25
beq $19,3,L.1216
li $23,121
beq $30,$0,L.1218
la $4,L.695
move $5,$23
jal printf  # int
L.1218:
L.1216:
li $19,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
subu $19,$19,$25
beq $19,3,L.1220
li $23,122
beq $30,$0,L.1222
la $4,L.695
move $5,$23
jal printf  # int
L.1222:
L.1220:
li $19,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
subu $19,$19,$25
beq $19,3,L.1224
li $23,123
beq $30,$0,L.1226
la $4,L.695
move $5,$23
jal printf  # int
L.1226:
L.1224:
li $19,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
subu $19,$19,$25
beq $19,3,L.1228
li $23,124
beq $30,$0,L.1230
la $4,L.695
move $5,$23
jal printf  # int
L.1230:
L.1228:
li $19,5
l.s $f26,L.531
mtc1 $19,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
sub.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$19; mfc1 $19,$f2
beq $19,3,L.1232
li $23,125
beq $30,$0,L.1234
la $4,L.695
move $5,$23
jal printf  # int
L.1234:
L.1232:
li $19,5
l.d $f24,L.417
mtc1 $19,$f18; cvt.d.w $f18,$f18
sub.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$19; mfc1 $19,$f2
beq $19,3,L.1236
li $23,126
beq $30,$0,L.1238
la $4,L.695
move $5,$23
jal printf  # int
L.1238:
L.1236:
li $18,5
li $17,2
sll $25,$17,24; sra $25,$25,24
subu $18,$18,$25
beq $18,3,L.1240
li $23,127
beq $30,$0,L.1242
la $4,L.695
move $5,$23
jal printf  # int
L.1242:
L.1240:
li $18,5
li $16,2
sll $25,$16,16; sra $25,$25,16
subu $18,$18,$25
beq $18,3,L.1244
li $23,128
beq $30,$0,L.1246
la $4,L.695
move $5,$23
jal printf  # int
L.1246:
L.1244:
li $18,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
subu $18,$18,$25
beq $18,3,L.1248
li $23,129
beq $30,$0,L.1250
la $4,L.695
move $5,$23
jal printf  # int
L.1250:
L.1248:
li $18,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
subu $18,$18,$25
beq $18,3,L.1252
li $23,130
beq $30,$0,L.1254
la $4,L.695
move $5,$23
jal printf  # int
L.1254:
L.1252:
li $18,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
subu $18,$18,$25
beq $18,3,L.1256
li $23,131
beq $30,$0,L.1258
la $4,L.695
move $5,$23
jal printf  # int
L.1258:
L.1256:
li $18,5
l.s $f26,L.531
blt $18,$0,L.1264
mtc1 $18,$f22; cvt.d.w $f22,$f22
b L.1265
L.1264:
mtc1 $18,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1265:
cvt.s.d $f18,$f22
sub.s $f18,$f18,$f26
cvt.d.s $f16,$f18
l.d $f10,L.836
c.lt.d $f16,$f10; bc1t L.1262
sub.d $f16,$f16,$f10
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-44($sp)
b L.1263
L.1262:
trunc.w.s $f2,$f18,$25; mfc1 $25,$f2
sw $25,$fp20-44($sp)
L.1263:
lw $18,$fp20-44($sp)
beq $18,3,L.1266
li $23,132
beq $30,$0,L.1268
la $4,L.695
move $5,$23
jal printf  # int
L.1268:
L.1266:
li $18,5
l.d $f24,L.417
blt $18,$0,L.1274
mtc1 $18,$f22; cvt.d.w $f22,$f22
b L.1275
L.1274:
mtc1 $18,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1275:
sub.d $f18,$f22,$f24
l.d $f16,L.836
c.lt.d $f18,$f16; bc1t L.1272
sub.d $f16,$f18,$f16
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-48($sp)
b L.1273
L.1272:
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
sw $25,$fp20-48($sp)
L.1273:
lw $18,$fp20-48($sp)
beq $18,3,L.1276
li $23,133
beq $30,$0,L.1278
la $4,L.695
move $5,$23
jal printf  # int
L.1278:
L.1276:
l.s $f30,L.848
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
sub.s $f30,$f30,$f18
l.s $f18,L.1282
c.eq.s $f30,$f18; bc1t L.1280
li $23,134
beq $30,$0,L.1283
la $4,L.695
move $5,$23
jal printf  # int
L.1283:
L.1280:
l.s $f30,L.848
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
sub.s $f30,$f30,$f18
l.s $f18,L.1282
c.eq.s $f30,$f18; bc1t L.1285
li $23,135
beq $30,$0,L.1287
la $4,L.695
move $5,$23
jal printf  # int
L.1287:
L.1285:
l.s $f30,L.848
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
sub.s $f30,$f30,$f18
l.s $f18,L.1282
c.eq.s $f30,$f18; bc1t L.1289
li $23,136
beq $30,$0,L.1291
la $4,L.695
move $5,$23
jal printf  # int
L.1291:
L.1289:
l.s $f30,L.848
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
sub.s $f30,$f30,$f18
l.s $f18,L.1282
c.eq.s $f30,$f18; bc1t L.1293
li $23,137
beq $30,$0,L.1295
la $4,L.695
move $5,$23
jal printf  # int
L.1295:
L.1293:
l.s $f30,L.848
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.1298
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.1299
L.1298:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1299:
cvt.s.d $f18,$f22
sub.s $f30,$f30,$f18
l.s $f18,L.1282
c.eq.s $f30,$f18; bc1t L.1300
li $23,138
beq $30,$0,L.1302
la $4,L.695
move $5,$23
jal printf  # int
L.1302:
L.1300:
l.s $f30,L.848
l.s $f26,L.531
sub.s $f30,$f30,$f26
l.s $f18,L.1282
c.eq.s $f30,$f18; bc1t L.1304
li $23,139
beq $30,$0,L.1306
la $4,L.695
move $5,$23
jal printf  # int
L.1306:
L.1304:
l.s $f30,L.848
l.d $f24,L.417
cvt.d.s $f18,$f30
sub.d $f18,$f18,$f24
cvt.s.d $f30,$f18
l.s $f18,L.1282
c.eq.s $f30,$f18; bc1t L.1308
li $23,140
beq $30,$0,L.1310
la $4,L.695
move $5,$23
jal printf  # int
L.1310:
L.1308:
l.d $f28,L.880
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
sub.d $f28,$f28,$f18
l.d $f18,L.1314
c.eq.d $f28,$f18; bc1t L.1312
li $23,141
beq $30,$0,L.1315
la $4,L.695
move $5,$23
jal printf  # int
L.1315:
L.1312:
l.d $f28,L.880
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
sub.d $f28,$f28,$f18
l.d $f18,L.1314
c.eq.d $f28,$f18; bc1t L.1317
li $23,142
beq $30,$0,L.1319
la $4,L.695
move $5,$23
jal printf  # int
L.1319:
L.1317:
l.d $f28,L.880
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
sub.d $f28,$f28,$f18
l.d $f18,L.1314
c.eq.d $f28,$f18; bc1t L.1321
li $23,143
beq $30,$0,L.1323
la $4,L.695
move $5,$23
jal printf  # int
L.1323:
L.1321:
l.d $f28,L.880
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
sub.d $f28,$f28,$f18
l.d $f18,L.1314
c.eq.d $f28,$f18; bc1t L.1325
li $23,144
beq $30,$0,L.1327
la $4,L.695
move $5,$23
jal printf  # int
L.1327:
L.1325:
l.d $f28,L.880
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.1330
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.1331
L.1330:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1331:
sub.d $f28,$f28,$f22
l.d $f18,L.1314
c.eq.d $f28,$f18; bc1t L.1332
li $23,145
beq $30,$0,L.1334
la $4,L.695
move $5,$23
jal printf  # int
L.1334:
L.1332:
l.d $f28,L.880
l.s $f26,L.531
cvt.d.s $f18,$f26
sub.d $f28,$f28,$f18
l.d $f18,L.1314
c.eq.d $f28,$f18; bc1t L.1336
li $23,146
beq $30,$0,L.1338
la $4,L.695
move $5,$23
jal printf  # int
L.1338:
L.1336:
l.d $f28,L.880
l.d $f24,L.417
sub.d $f28,$f28,$f24
l.d $f18,L.1314
c.eq.d $f28,$f18; bc1t L.1340
li $23,147
beq $30,$0,L.1342
la $4,L.695
move $5,$23
jal printf  # int
L.1342:
L.1340:
li $22,5
li $17,2
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
mul $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,10,L.1344
li $23,148
beq $30,$0,L.1346
la $4,L.695
move $5,$23
jal printf  # int
L.1346:
L.1344:
li $22,5
li $16,2
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
mul $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,10,L.1348
li $23,149
beq $30,$0,L.1350
la $4,L.695
move $5,$23
jal printf  # int
L.1350:
L.1348:
li $22,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
mul $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,10,L.1352
li $23,150
beq $30,$0,L.1354
la $4,L.695
move $5,$23
jal printf  # int
L.1354:
L.1352:
li $22,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
mul $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,10,L.1356
li $23,151
beq $30,$0,L.1358
la $4,L.695
move $5,$23
jal printf  # int
L.1358:
L.1356:
li $22,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
mul $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,10,L.1360
li $23,152
beq $30,$0,L.1362
la $4,L.695
move $5,$23
jal printf  # int
L.1362:
L.1360:
li $22,5
l.s $f26,L.531
sll $25,$22,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,10,L.1364
li $23,153
beq $30,$0,L.1366
la $4,L.695
move $5,$23
jal printf  # int
L.1366:
L.1364:
li $22,5
l.d $f24,L.417
sll $25,$22,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,10,L.1368
li $23,154
beq $30,$0,L.1370
la $4,L.695
move $5,$23
jal printf  # int
L.1370:
L.1368:
li $21,5
li $17,2
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
mul $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,10,L.1372
li $23,155
beq $30,$0,L.1374
la $4,L.695
move $5,$23
jal printf  # int
L.1374:
L.1372:
li $21,5
li $16,2
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
mul $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,10,L.1376
li $23,156
beq $30,$0,L.1378
la $4,L.695
move $5,$23
jal printf  # int
L.1378:
L.1376:
li $21,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
mul $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,10,L.1380
li $23,157
beq $30,$0,L.1382
la $4,L.695
move $5,$23
jal printf  # int
L.1382:
L.1380:
li $21,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
mul $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,10,L.1384
li $23,158
beq $30,$0,L.1386
la $4,L.695
move $5,$23
jal printf  # int
L.1386:
L.1384:
li $21,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
mul $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,10,L.1388
li $23,159
beq $30,$0,L.1390
la $4,L.695
move $5,$23
jal printf  # int
L.1390:
L.1388:
li $21,5
l.s $f26,L.531
sll $25,$21,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,10,L.1392
li $23,160
beq $30,$0,L.1394
la $4,L.695
move $5,$23
jal printf  # int
L.1394:
L.1392:
li $21,5
l.d $f24,L.417
sll $25,$21,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,10,L.1396
li $23,161
beq $30,$0,L.1398
la $4,L.695
move $5,$23
jal printf  # int
L.1398:
L.1396:
li $20,5
li $17,2
sll $25,$17,24; sra $25,$25,24
mul $20,$20,$25
beq $20,10,L.1400
li $23,162
beq $30,$0,L.1402
la $4,L.695
move $5,$23
jal printf  # int
L.1402:
L.1400:
li $20,5
li $16,2
sll $25,$16,16; sra $25,$25,16
mul $20,$20,$25
beq $20,10,L.1404
li $23,163
beq $30,$0,L.1406
la $4,L.695
move $5,$23
jal printf  # int
L.1406:
L.1404:
li $20,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mul $20,$20,$25
beq $20,10,L.1408
li $23,164
beq $30,$0,L.1410
la $4,L.695
move $5,$23
jal printf  # int
L.1410:
L.1408:
li $20,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mul $20,$20,$25
beq $20,10,L.1412
li $23,165
beq $30,$0,L.1414
la $4,L.695
move $5,$23
jal printf  # int
L.1414:
L.1412:
li $20,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
mul $20,$20,$25
beq $20,10,L.1416
li $23,166
beq $30,$0,L.1418
la $4,L.695
move $5,$23
jal printf  # int
L.1418:
L.1416:
li $20,5
l.s $f26,L.531
mtc1 $20,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$20; mfc1 $20,$f2
beq $20,10,L.1420
li $23,167
beq $30,$0,L.1422
la $4,L.695
move $5,$23
jal printf  # int
L.1422:
L.1420:
li $20,5
l.d $f24,L.417
mtc1 $20,$f18; cvt.d.w $f18,$f18
mul.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$20; mfc1 $20,$f2
beq $20,10,L.1424
li $23,168
beq $30,$0,L.1426
la $4,L.695
move $5,$23
jal printf  # int
L.1426:
L.1424:
li $19,5
li $17,2
sll $25,$17,24; sra $25,$25,24
mul $19,$19,$25
beq $19,10,L.1428
li $23,169
beq $30,$0,L.1430
la $4,L.695
move $5,$23
jal printf  # int
L.1430:
L.1428:
li $19,5
li $16,2
sll $25,$16,16; sra $25,$25,16
mul $19,$19,$25
beq $19,10,L.1432
li $23,170
beq $30,$0,L.1434
la $4,L.695
move $5,$23
jal printf  # int
L.1434:
L.1432:
li $19,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mul $19,$19,$25
beq $19,10,L.1436
li $23,171
beq $30,$0,L.1438
la $4,L.695
move $5,$23
jal printf  # int
L.1438:
L.1436:
li $19,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mul $19,$19,$25
beq $19,10,L.1440
li $23,172
beq $30,$0,L.1442
la $4,L.695
move $5,$23
jal printf  # int
L.1442:
L.1440:
li $19,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
mul $19,$19,$25
beq $19,10,L.1444
li $23,173
beq $30,$0,L.1446
la $4,L.695
move $5,$23
jal printf  # int
L.1446:
L.1444:
li $19,5
l.s $f26,L.531
mtc1 $19,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$19; mfc1 $19,$f2
beq $19,10,L.1448
li $23,174
beq $30,$0,L.1450
la $4,L.695
move $5,$23
jal printf  # int
L.1450:
L.1448:
li $19,5
l.d $f24,L.417
mtc1 $19,$f18; cvt.d.w $f18,$f18
mul.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$19; mfc1 $19,$f2
beq $19,10,L.1452
li $23,175
beq $30,$0,L.1454
la $4,L.695
move $5,$23
jal printf  # int
L.1454:
L.1452:
li $18,5
li $17,2
sll $25,$17,24; sra $25,$25,24
mul $18,$18,$25
beq $18,10,L.1456
li $23,176
beq $30,$0,L.1458
la $4,L.695
move $5,$23
jal printf  # int
L.1458:
L.1456:
li $18,5
li $16,2
sll $25,$16,16; sra $25,$25,16
mul $18,$18,$25
beq $18,10,L.1460
li $23,177
beq $30,$0,L.1462
la $4,L.695
move $5,$23
jal printf  # int
L.1462:
L.1460:
li $18,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mul $18,$18,$25
beq $18,10,L.1464
li $23,178
beq $30,$0,L.1466
la $4,L.695
move $5,$23
jal printf  # int
L.1466:
L.1464:
li $18,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mul $18,$18,$25
beq $18,10,L.1468
li $23,179
beq $30,$0,L.1470
la $4,L.695
move $5,$23
jal printf  # int
L.1470:
L.1468:
li $18,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
mul $18,$18,$25
beq $18,10,L.1472
li $23,180
beq $30,$0,L.1474
la $4,L.695
move $5,$23
jal printf  # int
L.1474:
L.1472:
li $18,5
l.s $f26,L.531
blt $18,$0,L.1480
mtc1 $18,$f22; cvt.d.w $f22,$f22
b L.1481
L.1480:
mtc1 $18,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1481:
cvt.s.d $f18,$f22
mul.s $f18,$f18,$f26
cvt.d.s $f16,$f18
l.d $f10,L.836
c.lt.d $f16,$f10; bc1t L.1478
sub.d $f16,$f16,$f10
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-52($sp)
b L.1479
L.1478:
trunc.w.s $f2,$f18,$25; mfc1 $25,$f2
sw $25,$fp20-52($sp)
L.1479:
lw $18,$fp20-52($sp)
beq $18,10,L.1482
li $23,181
beq $30,$0,L.1484
la $4,L.695
move $5,$23
jal printf  # int
L.1484:
L.1482:
li $18,5
l.d $f24,L.417
blt $18,$0,L.1490
mtc1 $18,$f22; cvt.d.w $f22,$f22
b L.1491
L.1490:
mtc1 $18,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1491:
mul.d $f18,$f22,$f24
l.d $f16,L.836
c.lt.d $f18,$f16; bc1t L.1488
sub.d $f16,$f18,$f16
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-56($sp)
b L.1489
L.1488:
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
sw $25,$fp20-56($sp)
L.1489:
lw $18,$fp20-56($sp)
beq $18,10,L.1492
li $23,182
beq $30,$0,L.1494
la $4,L.695
move $5,$23
jal printf  # int
L.1494:
L.1492:
l.s $f30,L.848
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f30,$f30,$f18
l.s $f18,L.1498
c.eq.s $f30,$f18; bc1t L.1496
li $23,183
beq $30,$0,L.1499
la $4,L.695
move $5,$23
jal printf  # int
L.1499:
L.1496:
l.s $f30,L.848
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f30,$f30,$f18
l.s $f18,L.1498
c.eq.s $f30,$f18; bc1t L.1501
li $23,184
beq $30,$0,L.1503
la $4,L.695
move $5,$23
jal printf  # int
L.1503:
L.1501:
l.s $f30,L.848
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f30,$f30,$f18
l.s $f18,L.1498
c.eq.s $f30,$f18; bc1t L.1505
li $23,185
beq $30,$0,L.1507
la $4,L.695
move $5,$23
jal printf  # int
L.1507:
L.1505:
l.s $f30,L.848
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
mul.s $f30,$f30,$f18
l.s $f18,L.1498
c.eq.s $f30,$f18; bc1t L.1509
li $23,186
beq $30,$0,L.1511
la $4,L.695
move $5,$23
jal printf  # int
L.1511:
L.1509:
l.s $f30,L.848
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.1514
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.1515
L.1514:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1515:
cvt.s.d $f18,$f22
mul.s $f30,$f30,$f18
l.s $f18,L.1498
c.eq.s $f30,$f18; bc1t L.1516
li $23,187
beq $30,$0,L.1518
la $4,L.695
move $5,$23
jal printf  # int
L.1518:
L.1516:
l.s $f30,L.848
l.s $f26,L.531
mul.s $f30,$f30,$f26
l.s $f18,L.1498
c.eq.s $f30,$f18; bc1t L.1520
li $23,188
beq $30,$0,L.1522
la $4,L.695
move $5,$23
jal printf  # int
L.1522:
L.1520:
l.s $f30,L.848
l.d $f24,L.417
cvt.d.s $f18,$f30
mul.d $f18,$f18,$f24
cvt.s.d $f30,$f18
l.s $f18,L.1498
c.eq.s $f30,$f18; bc1t L.1524
li $23,189
beq $30,$0,L.1526
la $4,L.695
move $5,$23
jal printf  # int
L.1526:
L.1524:
l.d $f28,L.880
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f28,$f28,$f18
l.d $f18,L.1530
c.eq.d $f28,$f18; bc1t L.1528
li $23,190
beq $30,$0,L.1531
la $4,L.695
move $5,$23
jal printf  # int
L.1531:
L.1528:
l.d $f28,L.880
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f28,$f28,$f18
l.d $f18,L.1530
c.eq.d $f28,$f18; bc1t L.1533
li $23,191
beq $30,$0,L.1535
la $4,L.695
move $5,$23
jal printf  # int
L.1535:
L.1533:
l.d $f28,L.880
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f28,$f28,$f18
l.d $f18,L.1530
c.eq.d $f28,$f18; bc1t L.1537
li $23,192
beq $30,$0,L.1539
la $4,L.695
move $5,$23
jal printf  # int
L.1539:
L.1537:
l.d $f28,L.880
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
mul.d $f28,$f28,$f18
l.d $f18,L.1530
c.eq.d $f28,$f18; bc1t L.1541
li $23,193
beq $30,$0,L.1543
la $4,L.695
move $5,$23
jal printf  # int
L.1543:
L.1541:
l.d $f28,L.880
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.1546
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.1547
L.1546:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1547:
mul.d $f28,$f28,$f22
l.d $f18,L.1530
c.eq.d $f28,$f18; bc1t L.1548
li $23,194
beq $30,$0,L.1550
la $4,L.695
move $5,$23
jal printf  # int
L.1550:
L.1548:
l.d $f28,L.880
l.s $f26,L.531
cvt.d.s $f18,$f26
mul.d $f28,$f28,$f18
l.d $f18,L.1530
c.eq.d $f28,$f18; bc1t L.1552
li $23,195
beq $30,$0,L.1554
la $4,L.695
move $5,$23
jal printf  # int
L.1554:
L.1552:
l.d $f28,L.880
l.d $f24,L.417
mul.d $f28,$f28,$f24
l.d $f18,L.1530
c.eq.d $f28,$f18; bc1t L.1556
li $23,196
beq $30,$0,L.1558
la $4,L.695
move $5,$23
jal printf  # int
L.1558:
L.1556:
li $22,5
li $17,2
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
div $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.1560
li $23,197
beq $30,$0,L.1562
la $4,L.695
move $5,$23
jal printf  # int
L.1562:
L.1560:
li $22,5
li $16,2
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
div $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.1564
li $23,198
beq $30,$0,L.1566
la $4,L.695
move $5,$23
jal printf  # int
L.1566:
L.1564:
li $22,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
div $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.1568
li $23,199
beq $30,$0,L.1570
la $4,L.695
move $5,$23
jal printf  # int
L.1570:
L.1568:
li $22,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
div $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.1572
li $23,200
beq $30,$0,L.1574
la $4,L.695
move $5,$23
jal printf  # int
L.1574:
L.1572:
li $22,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
divu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.1576
li $23,201
beq $30,$0,L.1578
la $4,L.695
move $5,$23
jal printf  # int
L.1578:
L.1576:
li $22,5
l.s $f26,L.531
sll $25,$22,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
div.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.1580
li $23,202
beq $30,$0,L.1582
la $4,L.695
move $5,$23
jal printf  # int
L.1582:
L.1580:
li $22,5
l.d $f24,L.417
sll $25,$22,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
div.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$22; mfc1 $22,$f2
sll $25,$22,24; sra $25,$25,24
beq $25,2,L.1584
li $23,203
beq $30,$0,L.1586
la $4,L.695
move $5,$23
jal printf  # int
L.1586:
L.1584:
li $21,5
li $17,2
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
div $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.1588
li $23,204
beq $30,$0,L.1590
la $4,L.695
move $5,$23
jal printf  # int
L.1590:
L.1588:
li $21,5
li $16,2
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
div $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.1592
li $23,205
beq $30,$0,L.1594
la $4,L.695
move $5,$23
jal printf  # int
L.1594:
L.1592:
li $21,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
div $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.1596
li $23,206
beq $30,$0,L.1598
la $4,L.695
move $5,$23
jal printf  # int
L.1598:
L.1596:
li $21,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
div $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.1600
li $23,207
beq $30,$0,L.1602
la $4,L.695
move $5,$23
jal printf  # int
L.1602:
L.1600:
li $21,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
divu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.1604
li $23,208
beq $30,$0,L.1606
la $4,L.695
move $5,$23
jal printf  # int
L.1606:
L.1604:
li $21,5
l.s $f26,L.531
sll $25,$21,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
div.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.1608
li $23,209
beq $30,$0,L.1610
la $4,L.695
move $5,$23
jal printf  # int
L.1610:
L.1608:
li $21,5
l.d $f24,L.417
sll $25,$21,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
div.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$21; mfc1 $21,$f2
sll $25,$21,16; sra $25,$25,16
beq $25,2,L.1612
li $23,210
beq $30,$0,L.1614
la $4,L.695
move $5,$23
jal printf  # int
L.1614:
L.1612:
li $20,5
li $17,2
sll $25,$17,24; sra $25,$25,24
div $20,$20,$25
beq $20,2,L.1616
li $23,211
beq $30,$0,L.1618
la $4,L.695
move $5,$23
jal printf  # int
L.1618:
L.1616:
li $20,5
li $16,2
sll $25,$16,16; sra $25,$25,16
div $20,$20,$25
beq $20,2,L.1620
li $23,212
beq $30,$0,L.1622
la $4,L.695
move $5,$23
jal printf  # int
L.1622:
L.1620:
li $20,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
div $20,$20,$25
beq $20,2,L.1624
li $23,213
beq $30,$0,L.1626
la $4,L.695
move $5,$23
jal printf  # int
L.1626:
L.1624:
li $20,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
div $20,$20,$25
beq $20,2,L.1628
li $23,214
beq $30,$0,L.1630
la $4,L.695
move $5,$23
jal printf  # int
L.1630:
L.1628:
li $20,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
divu $20,$20,$25
beq $20,2,L.1632
li $23,215
beq $30,$0,L.1634
la $4,L.695
move $5,$23
jal printf  # int
L.1634:
L.1632:
li $20,5
l.s $f26,L.531
mtc1 $20,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
div.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$20; mfc1 $20,$f2
beq $20,2,L.1636
li $23,216
beq $30,$0,L.1638
la $4,L.695
move $5,$23
jal printf  # int
L.1638:
L.1636:
li $20,5
l.d $f24,L.417
mtc1 $20,$f18; cvt.d.w $f18,$f18
div.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$20; mfc1 $20,$f2
beq $20,2,L.1640
li $23,217
beq $30,$0,L.1642
la $4,L.695
move $5,$23
jal printf  # int
L.1642:
L.1640:
li $19,5
li $17,2
sll $25,$17,24; sra $25,$25,24
div $19,$19,$25
beq $19,2,L.1644
li $23,218
beq $30,$0,L.1646
la $4,L.695
move $5,$23
jal printf  # int
L.1646:
L.1644:
li $19,5
li $16,2
sll $25,$16,16; sra $25,$25,16
div $19,$19,$25
beq $19,2,L.1648
li $23,219
beq $30,$0,L.1650
la $4,L.695
move $5,$23
jal printf  # int
L.1650:
L.1648:
li $19,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
div $19,$19,$25
beq $19,2,L.1652
li $23,220
beq $30,$0,L.1654
la $4,L.695
move $5,$23
jal printf  # int
L.1654:
L.1652:
li $19,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
div $19,$19,$25
beq $19,2,L.1656
li $23,221
beq $30,$0,L.1658
la $4,L.695
move $5,$23
jal printf  # int
L.1658:
L.1656:
li $19,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
divu $19,$19,$25
beq $19,2,L.1660
li $23,222
beq $30,$0,L.1662
la $4,L.695
move $5,$23
jal printf  # int
L.1662:
L.1660:
li $19,5
l.s $f26,L.531
mtc1 $19,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
div.s $f18,$f18,$f26
trunc.w.s $f2,$f18,$19; mfc1 $19,$f2
beq $19,2,L.1664
li $23,223
beq $30,$0,L.1666
la $4,L.695
move $5,$23
jal printf  # int
L.1666:
L.1664:
li $19,5
l.d $f24,L.417
mtc1 $19,$f18; cvt.d.w $f18,$f18
div.d $f18,$f18,$f24
trunc.w.d $f2,$f18,$19; mfc1 $19,$f2
beq $19,2,L.1668
li $23,224
beq $30,$0,L.1670
la $4,L.695
move $5,$23
jal printf  # int
L.1670:
L.1668:
li $18,5
li $17,2
sll $25,$17,24; sra $25,$25,24
divu $18,$18,$25
beq $18,2,L.1672
li $23,225
beq $30,$0,L.1674
la $4,L.695
move $5,$23
jal printf  # int
L.1674:
L.1672:
li $18,5
li $16,2
sll $25,$16,16; sra $25,$25,16
divu $18,$18,$25
beq $18,2,L.1676
li $23,226
beq $30,$0,L.1678
la $4,L.695
move $5,$23
jal printf  # int
L.1678:
L.1676:
li $18,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
divu $18,$18,$25
beq $18,2,L.1680
li $23,227
beq $30,$0,L.1682
la $4,L.695
move $5,$23
jal printf  # int
L.1682:
L.1680:
li $18,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
divu $18,$18,$25
beq $18,2,L.1684
li $23,228
beq $30,$0,L.1686
la $4,L.695
move $5,$23
jal printf  # int
L.1686:
L.1684:
li $18,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
divu $18,$18,$25
beq $18,2,L.1688
li $23,229
beq $30,$0,L.1690
la $4,L.695
move $5,$23
jal printf  # int
L.1690:
L.1688:
li $18,5
l.s $f26,L.531
blt $18,$0,L.1696
mtc1 $18,$f22; cvt.d.w $f22,$f22
b L.1697
L.1696:
mtc1 $18,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1697:
cvt.s.d $f18,$f22
div.s $f18,$f18,$f26
cvt.d.s $f16,$f18
l.d $f10,L.836
c.lt.d $f16,$f10; bc1t L.1694
sub.d $f16,$f16,$f10
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-60($sp)
b L.1695
L.1694:
trunc.w.s $f2,$f18,$25; mfc1 $25,$f2
sw $25,$fp20-60($sp)
L.1695:
lw $18,$fp20-60($sp)
beq $18,2,L.1698
li $23,230
beq $30,$0,L.1700
la $4,L.695
move $5,$23
jal printf  # int
L.1700:
L.1698:
li $18,5
l.d $f24,L.417
blt $18,$0,L.1706
mtc1 $18,$f22; cvt.d.w $f22,$f22
b L.1707
L.1706:
mtc1 $18,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1707:
div.d $f18,$f22,$f24
l.d $f16,L.836
c.lt.d $f18,$f16; bc1t L.1704
sub.d $f16,$f18,$f16
trunc.w.d $f2,$f16,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp20-64($sp)
b L.1705
L.1704:
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
sw $25,$fp20-64($sp)
L.1705:
lw $18,$fp20-64($sp)
beq $18,2,L.1708
li $23,231
beq $30,$0,L.1710
la $4,L.695
move $5,$23
jal printf  # int
L.1710:
L.1708:
l.s $f30,L.848
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
div.s $f30,$f30,$f18
cvt.d.s $f18,$f30
l.d $f16,L.1714
c.eq.d $f18,$f16; bc1t L.1712
li $23,232
beq $30,$0,L.1715
la $4,L.695
move $5,$23
jal printf  # int
L.1715:
L.1712:
l.s $f30,L.848
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
div.s $f30,$f30,$f18
cvt.d.s $f18,$f30
l.d $f16,L.1714
c.eq.d $f18,$f16; bc1t L.1717
li $23,233
beq $30,$0,L.1719
la $4,L.695
move $5,$23
jal printf  # int
L.1719:
L.1717:
l.s $f30,L.848
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
div.s $f30,$f30,$f18
cvt.d.s $f18,$f30
l.d $f16,L.1714
c.eq.d $f18,$f16; bc1t L.1721
li $23,234
beq $30,$0,L.1723
la $4,L.695
move $5,$23
jal printf  # int
L.1723:
L.1721:
l.s $f30,L.848
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
div.s $f30,$f30,$f18
cvt.d.s $f18,$f30
l.d $f16,L.1714
c.eq.d $f18,$f16; bc1t L.1725
li $23,235
beq $30,$0,L.1727
la $4,L.695
move $5,$23
jal printf  # int
L.1727:
L.1725:
l.s $f30,L.848
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.1730
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.1731
L.1730:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1731:
cvt.s.d $f18,$f22
div.s $f30,$f30,$f18
cvt.d.s $f18,$f30
l.d $f16,L.1714
c.eq.d $f18,$f16; bc1t L.1732
li $23,236
beq $30,$0,L.1734
la $4,L.695
move $5,$23
jal printf  # int
L.1734:
L.1732:
l.s $f30,L.848
l.s $f26,L.531
div.s $f30,$f30,$f26
cvt.d.s $f18,$f30
l.d $f16,L.1714
c.eq.d $f18,$f16; bc1t L.1736
li $23,237
beq $30,$0,L.1738
la $4,L.695
move $5,$23
jal printf  # int
L.1738:
L.1736:
l.s $f30,L.848
l.d $f24,L.417
cvt.d.s $f18,$f30
div.d $f18,$f18,$f24
cvt.s.d $f30,$f18
cvt.d.s $f18,$f30
l.d $f16,L.1714
c.eq.d $f18,$f16; bc1t L.1740
li $23,238
beq $30,$0,L.1742
la $4,L.695
move $5,$23
jal printf  # int
L.1742:
L.1740:
l.d $f28,L.880
li $17,2
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
div.d $f28,$f28,$f18
l.d $f18,L.1714
c.eq.d $f28,$f18; bc1t L.1744
li $23,239
beq $30,$0,L.1746
la $4,L.695
move $5,$23
jal printf  # int
L.1746:
L.1744:
l.d $f28,L.880
li $16,2
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
div.d $f28,$f28,$f18
l.d $f18,L.1714
c.eq.d $f28,$f18; bc1t L.1748
li $23,240
beq $30,$0,L.1750
la $4,L.695
move $5,$23
jal printf  # int
L.1750:
L.1748:
l.d $f28,L.880
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
div.d $f28,$f28,$f18
l.d $f18,L.1714
c.eq.d $f28,$f18; bc1t L.1752
li $23,241
beq $30,$0,L.1754
la $4,L.695
move $5,$23
jal printf  # int
L.1754:
L.1752:
l.d $f28,L.880
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
div.d $f28,$f28,$f18
l.d $f18,L.1714
c.eq.d $f28,$f18; bc1t L.1756
li $23,242
beq $30,$0,L.1758
la $4,L.695
move $5,$23
jal printf  # int
L.1758:
L.1756:
l.d $f28,L.880
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
blt $25,$0,L.1761
mtc1 $25,$f22; cvt.d.w $f22,$f22
b L.1762
L.1761:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f22,$f18,$f16
L.1762:
div.d $f28,$f28,$f22
l.d $f18,L.1714
c.eq.d $f28,$f18; bc1t L.1763
li $23,243
beq $30,$0,L.1765
la $4,L.695
move $5,$23
jal printf  # int
L.1765:
L.1763:
l.d $f28,L.880
l.s $f26,L.531
cvt.d.s $f18,$f26
div.d $f28,$f28,$f18
l.d $f18,L.1714
c.eq.d $f28,$f18; bc1t L.1767
li $23,244
beq $30,$0,L.1769
la $4,L.695
move $5,$23
jal printf  # int
L.1769:
L.1767:
l.d $f28,L.880
l.d $f24,L.417
div.d $f28,$f28,$f24
l.d $f18,L.1714
c.eq.d $f28,$f18; bc1t L.1771
li $23,245
beq $30,$0,L.1773
la $4,L.695
move $5,$23
jal printf  # int
L.1773:
L.1771:
li $22,5
li $17,2
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
rem $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1775
li $23,246
beq $30,$0,L.1777
la $4,L.695
move $5,$23
jal printf  # int
L.1777:
L.1775:
li $22,5
li $16,2
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
rem $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1779
li $23,247
beq $30,$0,L.1781
la $4,L.695
move $5,$23
jal printf  # int
L.1781:
L.1779:
li $22,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
rem $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1783
li $23,248
beq $30,$0,L.1785
la $4,L.695
move $5,$23
jal printf  # int
L.1785:
L.1783:
li $22,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
rem $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1787
li $23,249
beq $30,$0,L.1789
la $4,L.695
move $5,$23
jal printf  # int
L.1789:
L.1787:
li $22,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
remu $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1791
li $23,250
beq $30,$0,L.1793
la $4,L.695
move $5,$23
jal printf  # int
L.1793:
L.1791:
li $21,5
li $17,2
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
rem $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1795
li $23,251
beq $30,$0,L.1797
la $4,L.695
move $5,$23
jal printf  # int
L.1797:
L.1795:
li $21,5
li $16,2
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
rem $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1799
li $23,252
beq $30,$0,L.1801
la $4,L.695
move $5,$23
jal printf  # int
L.1801:
L.1799:
li $21,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
rem $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1803
li $23,253
beq $30,$0,L.1805
la $4,L.695
move $5,$23
jal printf  # int
L.1805:
L.1803:
li $21,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
rem $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1807
li $23,254
beq $30,$0,L.1809
la $4,L.695
move $5,$23
jal printf  # int
L.1809:
L.1807:
li $21,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
remu $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1811
li $23,255
beq $30,$0,L.1813
la $4,L.695
move $5,$23
jal printf  # int
L.1813:
L.1811:
li $20,5
li $17,2
sll $25,$17,24; sra $25,$25,24
rem $20,$20,$25
beq $20,1,L.1815
li $23,256
beq $30,$0,L.1817
la $4,L.695
move $5,$23
jal printf  # int
L.1817:
L.1815:
li $20,5
li $16,2
sll $25,$16,16; sra $25,$25,16
rem $20,$20,$25
beq $20,1,L.1819
li $23,257
beq $30,$0,L.1821
la $4,L.695
move $5,$23
jal printf  # int
L.1821:
L.1819:
li $20,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
rem $20,$20,$25
beq $20,1,L.1823
li $23,258
beq $30,$0,L.1825
la $4,L.695
move $5,$23
jal printf  # int
L.1825:
L.1823:
li $20,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
rem $20,$20,$25
beq $20,1,L.1827
li $23,259
beq $30,$0,L.1829
la $4,L.695
move $5,$23
jal printf  # int
L.1829:
L.1827:
li $20,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
remu $20,$20,$25
beq $20,1,L.1831
li $23,260
beq $30,$0,L.1833
la $4,L.695
move $5,$23
jal printf  # int
L.1833:
L.1831:
li $19,5
li $17,2
sll $25,$17,24; sra $25,$25,24
rem $19,$19,$25
beq $19,1,L.1835
li $23,261
beq $30,$0,L.1837
la $4,L.695
move $5,$23
jal printf  # int
L.1837:
L.1835:
li $19,5
li $16,2
sll $25,$16,16; sra $25,$25,16
rem $19,$19,$25
beq $19,1,L.1839
li $23,262
beq $30,$0,L.1841
la $4,L.695
move $5,$23
jal printf  # int
L.1841:
L.1839:
li $19,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
rem $19,$19,$25
beq $19,1,L.1843
li $23,263
beq $30,$0,L.1845
la $4,L.695
move $5,$23
jal printf  # int
L.1845:
L.1843:
li $19,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
rem $19,$19,$25
beq $19,1,L.1847
li $23,264
beq $30,$0,L.1849
la $4,L.695
move $5,$23
jal printf  # int
L.1849:
L.1847:
li $19,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
remu $19,$19,$25
beq $19,1,L.1851
li $23,265
beq $30,$0,L.1853
la $4,L.695
move $5,$23
jal printf  # int
L.1853:
L.1851:
li $18,5
li $17,2
sll $25,$17,24; sra $25,$25,24
remu $18,$18,$25
beq $18,1,L.1855
li $23,266
beq $30,$0,L.1857
la $4,L.695
move $5,$23
jal printf  # int
L.1857:
L.1855:
li $18,5
li $16,2
sll $25,$16,16; sra $25,$25,16
remu $18,$18,$25
beq $18,1,L.1859
li $23,267
beq $30,$0,L.1861
la $4,L.695
move $5,$23
jal printf  # int
L.1861:
L.1859:
li $18,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
remu $18,$18,$25
beq $18,1,L.1863
li $23,268
beq $30,$0,L.1865
la $4,L.695
move $5,$23
jal printf  # int
L.1865:
L.1863:
li $18,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
remu $18,$18,$25
beq $18,1,L.1867
li $23,269
beq $30,$0,L.1869
la $4,L.695
move $5,$23
jal printf  # int
L.1869:
L.1867:
li $18,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
remu $18,$18,$25
beq $18,1,L.1871
li $23,270
beq $30,$0,L.1873
la $4,L.695
move $5,$23
jal printf  # int
L.1873:
L.1871:
li $22,5
li $17,2
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
sra $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1875
li $23,271
beq $30,$0,L.1877
la $4,L.695
move $5,$23
jal printf  # int
L.1877:
L.1875:
li $22,5
li $16,2
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
sra $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1879
li $23,272
beq $30,$0,L.1881
la $4,L.695
move $5,$23
jal printf  # int
L.1881:
L.1879:
li $22,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
sra $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1883
li $23,273
beq $30,$0,L.1885
la $4,L.695
move $5,$23
jal printf  # int
L.1885:
L.1883:
li $22,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
sra $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1887
li $23,274
beq $30,$0,L.1889
la $4,L.695
move $5,$23
jal printf  # int
L.1889:
L.1887:
li $22,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
sra $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,1,L.1891
li $23,275
beq $30,$0,L.1893
la $4,L.695
move $5,$23
jal printf  # int
L.1893:
L.1891:
li $21,5
li $17,2
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
sra $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1895
li $23,276
beq $30,$0,L.1897
la $4,L.695
move $5,$23
jal printf  # int
L.1897:
L.1895:
li $21,5
li $16,2
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
sra $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1899
li $23,277
beq $30,$0,L.1901
la $4,L.695
move $5,$23
jal printf  # int
L.1901:
L.1899:
li $21,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
sra $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1903
li $23,278
beq $30,$0,L.1905
la $4,L.695
move $5,$23
jal printf  # int
L.1905:
L.1903:
li $21,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
sra $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1907
li $23,279
beq $30,$0,L.1909
la $4,L.695
move $5,$23
jal printf  # int
L.1909:
L.1907:
li $21,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
sra $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,1,L.1911
li $23,280
beq $30,$0,L.1913
la $4,L.695
move $5,$23
jal printf  # int
L.1913:
L.1911:
li $20,5
li $17,2
sll $25,$17,24; sra $25,$25,24
sra $20,$20,$25
beq $20,1,L.1915
li $23,281
beq $30,$0,L.1917
la $4,L.695
move $5,$23
jal printf  # int
L.1917:
L.1915:
li $20,5
li $16,2
sll $25,$16,16; sra $25,$25,16
sra $20,$20,$25
beq $20,1,L.1919
li $23,282
beq $30,$0,L.1921
la $4,L.695
move $5,$23
jal printf  # int
L.1921:
L.1919:
li $20,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
sra $20,$20,$25
beq $20,1,L.1923
li $23,283
beq $30,$0,L.1925
la $4,L.695
move $5,$23
jal printf  # int
L.1925:
L.1923:
li $20,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
sra $20,$20,$25
beq $20,1,L.1927
li $23,284
beq $30,$0,L.1929
la $4,L.695
move $5,$23
jal printf  # int
L.1929:
L.1927:
li $20,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
sra $20,$20,$25
beq $20,1,L.1931
li $23,285
beq $30,$0,L.1933
la $4,L.695
move $5,$23
jal printf  # int
L.1933:
L.1931:
li $19,5
li $17,2
sll $25,$17,24; sra $25,$25,24
sra $19,$19,$25
beq $19,1,L.1935
li $23,286
beq $30,$0,L.1937
la $4,L.695
move $5,$23
jal printf  # int
L.1937:
L.1935:
li $19,5
li $16,2
sll $25,$16,16; sra $25,$25,16
sra $19,$19,$25
beq $19,1,L.1939
li $23,287
beq $30,$0,L.1941
la $4,L.695
move $5,$23
jal printf  # int
L.1941:
L.1939:
li $19,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
sra $19,$19,$25
beq $19,1,L.1943
li $23,288
beq $30,$0,L.1945
la $4,L.695
move $5,$23
jal printf  # int
L.1945:
L.1943:
li $19,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
sra $19,$19,$25
beq $19,1,L.1947
li $23,289
beq $30,$0,L.1949
la $4,L.695
move $5,$23
jal printf  # int
L.1949:
L.1947:
li $19,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
sra $19,$19,$25
beq $19,1,L.1951
li $23,290
beq $30,$0,L.1953
la $4,L.695
move $5,$23
jal printf  # int
L.1953:
L.1951:
li $18,5
li $17,2
sll $25,$17,24; sra $25,$25,24
srl $18,$18,$25
beq $18,1,L.1955
li $23,291
beq $30,$0,L.1957
la $4,L.695
move $5,$23
jal printf  # int
L.1957:
L.1955:
li $18,5
li $16,2
sll $25,$16,16; sra $25,$25,16
srl $18,$18,$25
beq $18,1,L.1959
li $23,292
beq $30,$0,L.1961
la $4,L.695
move $5,$23
jal printf  # int
L.1961:
L.1959:
li $18,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
srl $18,$18,$25
beq $18,1,L.1963
li $23,293
beq $30,$0,L.1965
la $4,L.695
move $5,$23
jal printf  # int
L.1965:
L.1963:
li $18,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
srl $18,$18,$25
beq $18,1,L.1967
li $23,294
beq $30,$0,L.1969
la $4,L.695
move $5,$23
jal printf  # int
L.1969:
L.1967:
li $18,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
srl $18,$18,$25
beq $18,1,L.1971
li $23,295
beq $30,$0,L.1973
la $4,L.695
move $5,$23
jal printf  # int
L.1973:
L.1971:
li $22,5
li $17,2
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
sll $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,20,L.1975
li $23,296
beq $30,$0,L.1977
la $4,L.695
move $5,$23
jal printf  # int
L.1977:
L.1975:
li $22,5
li $16,2
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
sll $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,20,L.1979
li $23,297
beq $30,$0,L.1981
la $4,L.695
move $5,$23
jal printf  # int
L.1981:
L.1979:
li $22,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
sll $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,20,L.1983
li $23,298
beq $30,$0,L.1985
la $4,L.695
move $5,$23
jal printf  # int
L.1985:
L.1983:
li $22,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
sll $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,20,L.1987
li $23,299
beq $30,$0,L.1989
la $4,L.695
move $5,$23
jal printf  # int
L.1989:
L.1987:
li $22,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
sll $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,20,L.1991
li $23,300
beq $30,$0,L.1993
la $4,L.695
move $5,$23
jal printf  # int
L.1993:
L.1991:
li $21,5
li $17,2
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
sll $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,20,L.1995
li $23,301
beq $30,$0,L.1997
la $4,L.695
move $5,$23
jal printf  # int
L.1997:
L.1995:
li $21,5
li $16,2
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
sll $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,20,L.1999
li $23,302
beq $30,$0,L.2001
la $4,L.695
move $5,$23
jal printf  # int
L.2001:
L.1999:
li $21,5
li $25,2
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
sll $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,20,L.2003
li $23,303
beq $30,$0,L.2005
la $4,L.695
move $5,$23
jal printf  # int
L.2005:
L.2003:
li $21,5
li $25,2
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
sll $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,20,L.2007
li $23,304
beq $30,$0,L.2009
la $4,L.695
move $5,$23
jal printf  # int
L.2009:
L.2007:
li $21,5
li $25,2
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
sll $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,20,L.2011
li $23,305
beq $30,$0,L.2013
la $4,L.695
move $5,$23
jal printf  # int
L.2013:
L.2011:
li $20,5
li $17,2
sll $25,$17,24; sra $25,$25,24
sll $20,$20,$25
beq $20,20,L.2015
li $23,306
beq $30,$0,L.2017
la $4,L.695
move $5,$23
jal printf  # int
L.2017:
L.2015:
li $20,5
li $16,2
sll $25,$16,16; sra $25,$25,16
sll $20,$20,$25
beq $20,20,L.2019
li $23,307
beq $30,$0,L.2021
la $4,L.695
move $5,$23
jal printf  # int
L.2021:
L.2019:
li $20,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
sll $20,$20,$25
beq $20,20,L.2023
li $23,308
beq $30,$0,L.2025
la $4,L.695
move $5,$23
jal printf  # int
L.2025:
L.2023:
li $20,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
sll $20,$20,$25
beq $20,20,L.2027
li $23,309
beq $30,$0,L.2029
la $4,L.695
move $5,$23
jal printf  # int
L.2029:
L.2027:
li $20,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
sll $20,$20,$25
beq $20,20,L.2031
li $23,310
beq $30,$0,L.2033
la $4,L.695
move $5,$23
jal printf  # int
L.2033:
L.2031:
li $19,5
li $17,2
sll $25,$17,24; sra $25,$25,24
sll $19,$19,$25
beq $19,20,L.2035
li $23,311
beq $30,$0,L.2037
la $4,L.695
move $5,$23
jal printf  # int
L.2037:
L.2035:
li $19,5
li $16,2
sll $25,$16,16; sra $25,$25,16
sll $19,$19,$25
beq $19,20,L.2039
li $23,312
beq $30,$0,L.2041
la $4,L.695
move $5,$23
jal printf  # int
L.2041:
L.2039:
li $19,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
sll $19,$19,$25
beq $19,20,L.2043
li $23,313
beq $30,$0,L.2045
la $4,L.695
move $5,$23
jal printf  # int
L.2045:
L.2043:
li $19,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
sll $19,$19,$25
beq $19,20,L.2047
li $23,314
beq $30,$0,L.2049
la $4,L.695
move $5,$23
jal printf  # int
L.2049:
L.2047:
li $19,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
sll $19,$19,$25
beq $19,20,L.2051
li $23,315
beq $30,$0,L.2053
la $4,L.695
move $5,$23
jal printf  # int
L.2053:
L.2051:
li $18,5
li $17,2
sll $25,$17,24; sra $25,$25,24
sll $18,$18,$25
beq $18,20,L.2055
li $23,316
beq $30,$0,L.2057
la $4,L.695
move $5,$23
jal printf  # int
L.2057:
L.2055:
li $18,5
li $16,2
sll $25,$16,16; sra $25,$25,16
sll $18,$18,$25
beq $18,20,L.2059
li $23,317
beq $30,$0,L.2061
la $4,L.695
move $5,$23
jal printf  # int
L.2061:
L.2059:
li $18,5
li $25,2
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
sll $18,$18,$25
beq $18,20,L.2063
li $23,318
beq $30,$0,L.2065
la $4,L.695
move $5,$23
jal printf  # int
L.2065:
L.2063:
li $18,5
li $25,2
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
sll $18,$18,$25
beq $18,20,L.2067
li $23,319
beq $30,$0,L.2069
la $4,L.695
move $5,$23
jal printf  # int
L.2069:
L.2067:
li $18,5
li $25,2
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
sll $18,$18,$25
beq $18,20,L.2071
li $23,320
beq $30,$0,L.2073
la $4,L.695
move $5,$23
jal printf  # int
L.2073:
L.2071:
li $22,12
li $17,10
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
and $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,8,L.2075
li $23,321
beq $30,$0,L.2077
la $4,L.695
move $5,$23
jal printf  # int
L.2077:
L.2075:
li $22,12
li $16,10
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
and $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,8,L.2079
li $23,322
beq $30,$0,L.2081
la $4,L.695
move $5,$23
jal printf  # int
L.2081:
L.2079:
li $22,12
li $25,10
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
and $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,8,L.2083
li $23,323
beq $30,$0,L.2085
la $4,L.695
move $5,$23
jal printf  # int
L.2085:
L.2083:
li $22,12
li $25,10
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
and $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,8,L.2087
li $23,324
beq $30,$0,L.2089
la $4,L.695
move $5,$23
jal printf  # int
L.2089:
L.2087:
li $22,12
li $25,10
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
and $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,8,L.2091
li $23,325
beq $30,$0,L.2093
la $4,L.695
move $5,$23
jal printf  # int
L.2093:
L.2091:
li $21,12
li $17,10
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
and $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,8,L.2095
li $23,326
beq $30,$0,L.2097
la $4,L.695
move $5,$23
jal printf  # int
L.2097:
L.2095:
li $21,12
li $16,10
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
and $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,8,L.2099
li $23,327
beq $30,$0,L.2101
la $4,L.695
move $5,$23
jal printf  # int
L.2101:
L.2099:
li $21,12
li $25,10
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
and $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,8,L.2103
li $23,328
beq $30,$0,L.2105
la $4,L.695
move $5,$23
jal printf  # int
L.2105:
L.2103:
li $21,12
li $25,10
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
and $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,8,L.2107
li $23,329
beq $30,$0,L.2109
la $4,L.695
move $5,$23
jal printf  # int
L.2109:
L.2107:
li $21,12
li $25,10
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
and $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,8,L.2111
li $23,330
beq $30,$0,L.2113
la $4,L.695
move $5,$23
jal printf  # int
L.2113:
L.2111:
li $20,12
li $17,10
sll $25,$17,24; sra $25,$25,24
and $20,$20,$25
beq $20,8,L.2115
li $23,331
beq $30,$0,L.2117
la $4,L.695
move $5,$23
jal printf  # int
L.2117:
L.2115:
li $20,12
li $16,10
sll $25,$16,16; sra $25,$25,16
and $20,$20,$25
beq $20,8,L.2119
li $23,332
beq $30,$0,L.2121
la $4,L.695
move $5,$23
jal printf  # int
L.2121:
L.2119:
li $20,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
and $20,$20,$25
beq $20,8,L.2123
li $23,333
beq $30,$0,L.2125
la $4,L.695
move $5,$23
jal printf  # int
L.2125:
L.2123:
li $20,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
and $20,$20,$25
beq $20,8,L.2127
li $23,334
beq $30,$0,L.2129
la $4,L.695
move $5,$23
jal printf  # int
L.2129:
L.2127:
li $20,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
and $20,$20,$25
beq $20,8,L.2131
li $23,335
beq $30,$0,L.2133
la $4,L.695
move $5,$23
jal printf  # int
L.2133:
L.2131:
li $19,12
li $17,10
sll $25,$17,24; sra $25,$25,24
and $19,$19,$25
beq $19,8,L.2135
li $23,336
beq $30,$0,L.2137
la $4,L.695
move $5,$23
jal printf  # int
L.2137:
L.2135:
li $19,12
li $16,10
sll $25,$16,16; sra $25,$25,16
and $19,$19,$25
beq $19,8,L.2139
li $23,337
beq $30,$0,L.2141
la $4,L.695
move $5,$23
jal printf  # int
L.2141:
L.2139:
li $19,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
and $19,$19,$25
beq $19,8,L.2143
li $23,338
beq $30,$0,L.2145
la $4,L.695
move $5,$23
jal printf  # int
L.2145:
L.2143:
li $19,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
and $19,$19,$25
beq $19,8,L.2147
li $23,339
beq $30,$0,L.2149
la $4,L.695
move $5,$23
jal printf  # int
L.2149:
L.2147:
li $19,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
and $19,$19,$25
beq $19,8,L.2151
li $23,340
beq $30,$0,L.2153
la $4,L.695
move $5,$23
jal printf  # int
L.2153:
L.2151:
li $18,12
li $17,10
sll $25,$17,24; sra $25,$25,24
and $18,$18,$25
beq $18,8,L.2155
li $23,341
beq $30,$0,L.2157
la $4,L.695
move $5,$23
jal printf  # int
L.2157:
L.2155:
li $18,12
li $16,10
sll $25,$16,16; sra $25,$25,16
and $18,$18,$25
beq $18,8,L.2159
li $23,342
beq $30,$0,L.2161
la $4,L.695
move $5,$23
jal printf  # int
L.2161:
L.2159:
li $18,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
and $18,$18,$25
beq $18,8,L.2163
li $23,343
beq $30,$0,L.2165
la $4,L.695
move $5,$23
jal printf  # int
L.2165:
L.2163:
li $18,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
and $18,$18,$25
beq $18,8,L.2167
li $23,344
beq $30,$0,L.2169
la $4,L.695
move $5,$23
jal printf  # int
L.2169:
L.2167:
li $18,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
and $18,$18,$25
beq $18,8,L.2171
li $23,345
beq $30,$0,L.2173
la $4,L.695
move $5,$23
jal printf  # int
L.2173:
L.2171:
li $22,12
li $17,10
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
xor $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,6,L.2175
li $23,346
beq $30,$0,L.2177
la $4,L.695
move $5,$23
jal printf  # int
L.2177:
L.2175:
li $22,12
li $16,10
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
xor $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,6,L.2179
li $23,347
beq $30,$0,L.2181
la $4,L.695
move $5,$23
jal printf  # int
L.2181:
L.2179:
li $22,12
li $25,10
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
xor $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,6,L.2183
li $23,348
beq $30,$0,L.2185
la $4,L.695
move $5,$23
jal printf  # int
L.2185:
L.2183:
li $22,12
li $25,10
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
xor $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,6,L.2187
li $23,349
beq $30,$0,L.2189
la $4,L.695
move $5,$23
jal printf  # int
L.2189:
L.2187:
li $22,12
li $25,10
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
xor $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,6,L.2191
li $23,350
beq $30,$0,L.2193
la $4,L.695
move $5,$23
jal printf  # int
L.2193:
L.2191:
li $21,12
li $17,10
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
xor $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,6,L.2195
li $23,351
beq $30,$0,L.2197
la $4,L.695
move $5,$23
jal printf  # int
L.2197:
L.2195:
li $21,12
li $16,10
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
xor $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,6,L.2199
li $23,352
beq $30,$0,L.2201
la $4,L.695
move $5,$23
jal printf  # int
L.2201:
L.2199:
li $21,12
li $25,10
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
xor $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,6,L.2203
li $23,353
beq $30,$0,L.2205
la $4,L.695
move $5,$23
jal printf  # int
L.2205:
L.2203:
li $21,12
li $25,10
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
xor $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,6,L.2207
li $23,354
beq $30,$0,L.2209
la $4,L.695
move $5,$23
jal printf  # int
L.2209:
L.2207:
li $21,12
li $25,10
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
xor $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,6,L.2211
li $23,355
beq $30,$0,L.2213
la $4,L.695
move $5,$23
jal printf  # int
L.2213:
L.2211:
li $20,12
li $17,10
sll $25,$17,24; sra $25,$25,24
xor $20,$20,$25
beq $20,6,L.2215
li $23,356
beq $30,$0,L.2217
la $4,L.695
move $5,$23
jal printf  # int
L.2217:
L.2215:
li $20,12
li $16,10
sll $25,$16,16; sra $25,$25,16
xor $20,$20,$25
beq $20,6,L.2219
li $23,357
beq $30,$0,L.2221
la $4,L.695
move $5,$23
jal printf  # int
L.2221:
L.2219:
li $20,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
xor $20,$20,$25
beq $20,6,L.2223
li $23,358
beq $30,$0,L.2225
la $4,L.695
move $5,$23
jal printf  # int
L.2225:
L.2223:
li $20,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
xor $20,$20,$25
beq $20,6,L.2227
li $23,359
beq $30,$0,L.2229
la $4,L.695
move $5,$23
jal printf  # int
L.2229:
L.2227:
li $20,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
xor $20,$20,$25
beq $20,6,L.2231
li $23,360
beq $30,$0,L.2233
la $4,L.695
move $5,$23
jal printf  # int
L.2233:
L.2231:
li $19,12
li $17,10
sll $25,$17,24; sra $25,$25,24
xor $19,$19,$25
beq $19,6,L.2235
li $23,361
beq $30,$0,L.2237
la $4,L.695
move $5,$23
jal printf  # int
L.2237:
L.2235:
li $19,12
li $16,10
sll $25,$16,16; sra $25,$25,16
xor $19,$19,$25
beq $19,6,L.2239
li $23,362
beq $30,$0,L.2241
la $4,L.695
move $5,$23
jal printf  # int
L.2241:
L.2239:
li $19,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
xor $19,$19,$25
beq $19,6,L.2243
li $23,363
beq $30,$0,L.2245
la $4,L.695
move $5,$23
jal printf  # int
L.2245:
L.2243:
li $19,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
xor $19,$19,$25
beq $19,6,L.2247
li $23,364
beq $30,$0,L.2249
la $4,L.695
move $5,$23
jal printf  # int
L.2249:
L.2247:
li $19,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
xor $19,$19,$25
beq $19,6,L.2251
li $23,365
beq $30,$0,L.2253
la $4,L.695
move $5,$23
jal printf  # int
L.2253:
L.2251:
li $18,12
li $17,10
sll $25,$17,24; sra $25,$25,24
xor $18,$18,$25
beq $18,6,L.2255
li $23,366
beq $30,$0,L.2257
la $4,L.695
move $5,$23
jal printf  # int
L.2257:
L.2255:
li $18,12
li $16,10
sll $25,$16,16; sra $25,$25,16
xor $18,$18,$25
beq $18,6,L.2259
li $23,367
beq $30,$0,L.2261
la $4,L.695
move $5,$23
jal printf  # int
L.2261:
L.2259:
li $18,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
xor $18,$18,$25
beq $18,6,L.2263
li $23,368
beq $30,$0,L.2265
la $4,L.695
move $5,$23
jal printf  # int
L.2265:
L.2263:
li $18,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
xor $18,$18,$25
beq $18,6,L.2267
li $23,369
beq $30,$0,L.2269
la $4,L.695
move $5,$23
jal printf  # int
L.2269:
L.2267:
li $18,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
xor $18,$18,$25
beq $18,6,L.2271
li $23,370
beq $30,$0,L.2273
la $4,L.695
move $5,$23
jal printf  # int
L.2273:
L.2271:
li $22,12
li $17,10
sll $25,$22,24; sra $25,$25,24
sll $24,$17,24; sra $24,$24,24
or $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,14,L.2275
li $23,371
beq $30,$0,L.2277
la $4,L.695
move $5,$23
jal printf  # int
L.2277:
L.2275:
li $22,12
li $16,10
sll $25,$22,24; sra $25,$25,24
sll $24,$16,16; sra $24,$24,16
or $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,14,L.2279
li $23,372
beq $30,$0,L.2281
la $4,L.695
move $5,$23
jal printf  # int
L.2281:
L.2279:
li $22,12
li $25,10
sw $25,$fp20-4($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-4($sp)
or $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,14,L.2283
li $23,373
beq $30,$0,L.2285
la $4,L.695
move $5,$23
jal printf  # int
L.2285:
L.2283:
li $22,12
li $25,10
sw $25,$fp20-8($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-8($sp)
or $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,14,L.2287
li $23,374
beq $30,$0,L.2289
la $4,L.695
move $5,$23
jal printf  # int
L.2289:
L.2287:
li $22,12
li $25,10
sw $25,$fp20-12($sp)
sll $25,$22,24; sra $25,$25,24
lw $24,$fp20-12($sp)
or $22,$25,$24
sll $25,$22,24; sra $25,$25,24
beq $25,14,L.2291
li $23,375
beq $30,$0,L.2293
la $4,L.695
move $5,$23
jal printf  # int
L.2293:
L.2291:
li $21,12
li $17,10
sll $25,$21,16; sra $25,$25,16
sll $24,$17,24; sra $24,$24,24
or $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,14,L.2295
li $23,376
beq $30,$0,L.2297
la $4,L.695
move $5,$23
jal printf  # int
L.2297:
L.2295:
li $21,12
li $16,10
sll $25,$21,16; sra $25,$25,16
sll $24,$16,16; sra $24,$24,16
or $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,14,L.2299
li $23,377
beq $30,$0,L.2301
la $4,L.695
move $5,$23
jal printf  # int
L.2301:
L.2299:
li $21,12
li $25,10
sw $25,$fp20-4($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-4($sp)
or $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,14,L.2303
li $23,378
beq $30,$0,L.2305
la $4,L.695
move $5,$23
jal printf  # int
L.2305:
L.2303:
li $21,12
li $25,10
sw $25,$fp20-8($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-8($sp)
or $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,14,L.2307
li $23,379
beq $30,$0,L.2309
la $4,L.695
move $5,$23
jal printf  # int
L.2309:
L.2307:
li $21,12
li $25,10
sw $25,$fp20-12($sp)
sll $25,$21,16; sra $25,$25,16
lw $24,$fp20-12($sp)
or $21,$25,$24
sll $25,$21,16; sra $25,$25,16
beq $25,14,L.2311
li $23,380
beq $30,$0,L.2313
la $4,L.695
move $5,$23
jal printf  # int
L.2313:
L.2311:
li $20,12
li $17,10
sll $25,$17,24; sra $25,$25,24
or $20,$20,$25
beq $20,14,L.2315
li $23,381
beq $30,$0,L.2317
la $4,L.695
move $5,$23
jal printf  # int
L.2317:
L.2315:
li $20,12
li $16,10
sll $25,$16,16; sra $25,$25,16
or $20,$20,$25
beq $20,14,L.2319
li $23,382
beq $30,$0,L.2321
la $4,L.695
move $5,$23
jal printf  # int
L.2321:
L.2319:
li $20,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
or $20,$20,$25
beq $20,14,L.2323
li $23,383
beq $30,$0,L.2325
la $4,L.695
move $5,$23
jal printf  # int
L.2325:
L.2323:
li $20,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
or $20,$20,$25
beq $20,14,L.2327
li $23,384
beq $30,$0,L.2329
la $4,L.695
move $5,$23
jal printf  # int
L.2329:
L.2327:
li $20,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
or $20,$20,$25
beq $20,14,L.2331
li $23,385
beq $30,$0,L.2333
la $4,L.695
move $5,$23
jal printf  # int
L.2333:
L.2331:
li $19,12
li $17,10
sll $25,$17,24; sra $25,$25,24
or $19,$19,$25
beq $19,14,L.2335
li $23,386
beq $30,$0,L.2337
la $4,L.695
move $5,$23
jal printf  # int
L.2337:
L.2335:
li $19,12
li $16,10
sll $25,$16,16; sra $25,$25,16
or $19,$19,$25
beq $19,14,L.2339
li $23,387
beq $30,$0,L.2341
la $4,L.695
move $5,$23
jal printf  # int
L.2341:
L.2339:
li $19,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
or $19,$19,$25
beq $19,14,L.2343
li $23,388
beq $30,$0,L.2345
la $4,L.695
move $5,$23
jal printf  # int
L.2345:
L.2343:
li $19,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
or $19,$19,$25
beq $19,14,L.2347
li $23,389
beq $30,$0,L.2349
la $4,L.695
move $5,$23
jal printf  # int
L.2349:
L.2347:
li $19,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
or $19,$19,$25
beq $19,14,L.2351
li $23,390
beq $30,$0,L.2353
la $4,L.695
move $5,$23
jal printf  # int
L.2353:
L.2351:
li $18,12
li $17,10
sll $25,$17,24; sra $25,$25,24
or $18,$18,$25
beq $18,14,L.2355
li $23,391
beq $30,$0,L.2357
la $4,L.695
move $5,$23
jal printf  # int
L.2357:
L.2355:
li $18,12
li $16,10
sll $25,$16,16; sra $25,$25,16
or $18,$18,$25
beq $18,14,L.2359
li $23,392
beq $30,$0,L.2361
la $4,L.695
move $5,$23
jal printf  # int
L.2361:
L.2359:
li $18,12
li $25,10
sw $25,$fp20-4($sp)
lw $25,$fp20-4($sp)
or $18,$18,$25
beq $18,14,L.2363
li $23,393
beq $30,$0,L.2365
la $4,L.695
move $5,$23
jal printf  # int
L.2365:
L.2363:
li $18,12
li $25,10
sw $25,$fp20-8($sp)
lw $25,$fp20-8($sp)
or $18,$18,$25
beq $18,14,L.2367
li $23,394
beq $30,$0,L.2369
la $4,L.695
move $5,$23
jal printf  # int
L.2369:
L.2367:
li $18,12
li $25,10
sw $25,$fp20-12($sp)
lw $25,$fp20-12($sp)
or $18,$18,$25
beq $18,14,L.2371
li $23,395
beq $30,$0,L.2373
la $4,L.695
move $5,$23
jal printf  # int
L.2373:
L.2371:
beq $23,$0,L.2375
li $25,1
sw $25,$fp20-24($sp)
lw $25,$fp20+0($sp)
lw $25,44($25)
beq $25,$0,L.2377
la $4,L.696
li $5,1
jal printf  # int
L.2377:
L.2375:
lw $2,$fp20-24($sp)
L.694:
l.d $f22,16($sp)
l.d $f24,24($sp)
l.d $f26,32($sp)
l.d $f28,40($sp)
l.d $f30,48($sp)
lw $16,56($sp)
lw $17,60($sp)
lw $18,64($sp)
lw $19,68($sp)
lw $20,72($sp)
lw $21,76($sp)
lw $22,80($sp)
lw $23,84($sp)
lw $30,88($sp)
lw $31,92($sp)
addu $sp,$sp,160
j $31
.end s714
.data; L.2380:.ascii "s715,er%d\012\000"
.data; L.2381:.ascii "s715   \000"
.text
.globl s715
.text
.align 2; .ent s715
s715:
$fp21=48
.frame $sp,48,$31
addu $sp,$sp,-48
.mask 0xc0e00000,-16
sw $21,16($sp)
sw $22,20($sp)
sw $23,24($sp)
sw $30,28($sp)
sw $31,32($sp)
sw $4,48($sp)
sw $0,$fp21-12($sp)
sw $0,$fp21-4($sp)
la $30,L.2381
lw $25,$fp21+0($sp)
addu $23,$25,60
move $21,$0
L.2382:
L.2383:
move $25,$23
addu $23,$25,1
move $24,$30
addu $30,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.2382
li $25,1
move $22,$25
addu $22,$22,1
addu $22,$22,1
addu $22,$22,1
addu $22,$22,1
addu $25,$22,1
move $22,$25
beq $25,6,L.2385
lw $25,$fp21+0($sp)
lw $25,44($25)
beq $25,$0,L.2387
la $4,L.2380
li $5,1
jal printf  # int
L.2387:
addu $21,$21,1
L.2385:
lw $4,$fp21-4($sp)
li $25,3
sw $25,$fp21-8($sp)
lw $25,$fp21-8($sp)
addu $5,$25,2
lw $6,$fp21-12($sp)
jal s715f  # int
beq $2,5,L.2389
lw $25,$fp21+0($sp)
lw $25,44($25)
beq $25,$0,L.2391
la $4,L.2380
li $5,2
jal printf  # int
L.2391:
addu $21,$21,2
L.2389:
move $2,$21
L.2379:
lw $21,16($sp)
lw $22,20($sp)
lw $23,24($sp)
lw $30,28($sp)
lw $31,32($sp)
addu $sp,$sp,48
j $31
.end s715
.globl s715f
.text
.align 2; .ent s715f
s715f:
$fp22=0
.frame $sp,0,$31
move $2,$5
L.2393:
j $31
.end s715f
.data; L.2395:.ascii "s72,er%d\012\000"
.data; L.2396:.ascii "s72    \000"
.text
.globl s72
.text
.align 2; .ent s72
s72:
$fp23=104
.frame $sp,104,$31
addu $sp,$sp,-104
.fmask 0xfc000000,-72
s.d $f26,16($sp)
s.d $f28,24($sp)
s.d $f30,32($sp)
.mask 0xc0ff0000,-28
sw $16,40($sp)
sw $17,44($sp)
sw $18,48($sp)
sw $19,52($sp)
sw $20,56($sp)
sw $21,60($sp)
sw $22,64($sp)
sw $23,68($sp)
sw $30,72($sp)
sw $31,76($sp)
move $30,$4
la $22,L.2396
addu $21,$30,60
move $19,$0
L.2397:
L.2398:
move $25,$21
addu $21,$25,1
move $24,$22
addu $22,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.2397
li $25,2
sw $25,$fp23-4($sp)
lw $25,$fp23-4($sp)
beq $25,2,L.2400
addu $19,$19,1
la $4,L.2395
li $5,1
jal printf  # int
L.2400:
lw $25,$fp23-4($sp)
negu $24,$25
addu $25,$25,$24
beq $25,$0,L.2402
addu $19,$19,2
la $4,L.2395
li $5,2
jal printf  # int
L.2402:
sw $0,$fp23-4($sp)
move $23,$0
b L.2407
L.2404:
lw $25,$fp23-4($sp)
sll $25,$25,1
or $25,$25,1
sw $25,$fp23-4($sp)
L.2405:
addu $23,$23,1
L.2407:
lw $25,4($30)
blt $23,$25,L.2404
lw $25,$fp23-4($sp)
not $25,$25
beq $25,$0,L.2408
addu $19,$19,4
la $4,L.2395
li $5,4
jal printf  # int
L.2408:
li $25,5
sw $25,$fp23-4($sp)
lw $25,$fp23-4($sp)
addu $25,$25,1
sw $25,$fp23-4($sp)
bne $25,6,L.2415
lw $25,$fp23-4($sp)
subu $25,$25,1
sw $25,$fp23-4($sp)
bne $25,5,L.2415
lw $25,$fp23-4($sp)
addu $24,$25,1
sw $24,$fp23-4($sp)
bne $25,5,L.2415
lw $25,$fp23-4($sp)
subu $24,$25,1
sw $24,$fp23-4($sp)
bne $25,6,L.2415
lw $25,$fp23-4($sp)
beq $25,5,L.2410
L.2415:
addu $19,$19,8
la $4,L.2395
li $5,8
jal printf  # int
L.2410:
li $17,26
li $25,26
sw $25,$fp23-8($sp)
l.d $f30,L.2416
li $16,26
li $25,26
sw $25,$fp23-12($sp)
li $25,26
move $18,$25
l.s $f28,L.2417
move $20,$0
sll $25,$16,16; sra $25,$25,16
sll $25,$25,24; sra $25,$25,24
bne $25,26,L.2424
sll $25,$18,24; sra $25,$25,24
bne $25,26,L.2424
lw $25,$fp23-8($sp)
sll $25,$25,24; sra $25,$25,24
bne $25,26,L.2424
lw $25,$fp23-12($sp)
sll $25,$25,24; sra $25,$25,24
bne $25,26,L.2424
trunc.w.s $f2,$f28,$25; mfc1 $25,$f2
sll $25,$25,24; sra $25,$25,24
bne $25,26,L.2424
trunc.w.d $f2,$f30,$25; mfc1 $25,$f2
sll $25,$25,24; sra $25,$25,24
beq $25,26,L.2418
L.2424:
addu $20,$20,1
L.2418:
sll $25,$17,24; sra $25,$25,24
sll $25,$25,16; sra $25,$25,16
bne $25,26,L.2431
sll $25,$18,16; sra $25,$25,16
bne $25,26,L.2431
lw $25,$fp23-8($sp)
sll $25,$25,16; sra $25,$25,16
bne $25,26,L.2431
lw $25,$fp23-12($sp)
sll $25,$25,16; sra $25,$25,16
bne $25,26,L.2431
trunc.w.s $f2,$f28,$25; mfc1 $25,$f2
sll $25,$25,16; sra $25,$25,16
bne $25,26,L.2431
trunc.w.d $f2,$f30,$25; mfc1 $25,$f2
sll $25,$25,16; sra $25,$25,16
beq $25,26,L.2425
L.2431:
addu $20,$20,2
L.2425:
sll $25,$17,24; sra $25,$25,24
bne $25,26,L.2438
sll $25,$16,16; sra $25,$25,16
bne $25,26,L.2438
lw $25,$fp23-8($sp)
bne $25,26,L.2438
lw $25,$fp23-12($sp)
bne $25,26,L.2438
trunc.w.s $f2,$f28,$25; mfc1 $25,$f2
bne $25,26,L.2438
trunc.w.d $f2,$f30,$25; mfc1 $25,$f2
beq $25,26,L.2432
L.2438:
addu $20,$20,4
L.2432:
sll $25,$17,24; sra $25,$25,24
bne $25,26,L.2445
sll $25,$16,16; sra $25,$25,16
bne $25,26,L.2445
bne $18,26,L.2445
lw $25,$fp23-12($sp)
bne $25,26,L.2445
trunc.w.s $f2,$f28,$25; mfc1 $25,$f2
bne $25,26,L.2445
trunc.w.d $f2,$f30,$25; mfc1 $25,$f2
beq $25,26,L.2439
L.2445:
addu $20,$20,8
L.2439:
sll $25,$17,24; sra $25,$25,24
bne $25,26,L.2454
sll $25,$16,16; sra $25,$25,16
bne $25,26,L.2454
bne $18,26,L.2454
lw $25,$fp23-8($sp)
bne $25,26,L.2454
cvt.d.s $f18,$f28
l.d $f16,L.836
c.lt.d $f18,$f16; bc1t L.2455
sub.d $f18,$f18,$f16
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp23-16($sp)
b L.2456
L.2455:
trunc.w.s $f2,$f28,$25; mfc1 $25,$f2
sw $25,$fp23-16($sp)
L.2456:
lw $25,$fp23-16($sp)
bne $25,26,L.2454
l.d $f18,L.836
c.lt.d $f30,$f18; bc1t L.2457
sub.d $f18,$f30,$f18
trunc.w.d $f2,$f18,$25; mfc1 $25,$f2
addu $25,$25,0x80000000
sw $25,$fp23-20($sp)
b L.2458
L.2457:
trunc.w.d $f2,$f30,$25; mfc1 $25,$f2
sw $25,$fp23-20($sp)
L.2458:
lw $25,$fp23-20($sp)
beq $25,26,L.2446
L.2454:
addu $20,$20,16
L.2446:
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
cvt.d.s $f18,$f18
l.d $f16,L.2416
c.eq.d $f18,$f16; bc1f L.2466
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
cvt.d.s $f18,$f18
c.eq.d $f18,$f16; bc1f L.2466
mtc1 $18,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
cvt.d.s $f18,$f18
c.eq.d $f18,$f16; bc1f L.2466
lw $25,$fp23-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
cvt.d.s $f18,$f18
c.eq.d $f18,$f16; bc1f L.2466
lw $25,$fp23-12($sp)
blt $25,$0,L.2467
mtc1 $25,$f26; cvt.d.w $f26,$f26
b L.2468
L.2467:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f26,$f18,$f16
L.2468:
cvt.s.d $f18,$f26
cvt.d.s $f18,$f18
l.d $f16,L.2416
c.eq.d $f18,$f16; bc1f L.2466
cvt.s.d $f18,$f30
cvt.d.s $f18,$f18
c.eq.d $f18,$f16; bc1t L.2459
L.2466:
addu $20,$20,32
L.2459:
sll $25,$17,24; sra $25,$25,24
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.2416
c.eq.d $f18,$f16; bc1f L.2476
sll $25,$16,16; sra $25,$25,16
mtc1 $25,$f18; cvt.d.w $f18,$f18
c.eq.d $f18,$f16; bc1f L.2476
mtc1 $18,$f18; cvt.d.w $f18,$f18
c.eq.d $f18,$f16; bc1f L.2476
lw $25,$fp23-8($sp)
mtc1 $25,$f18; cvt.d.w $f18,$f18
c.eq.d $f18,$f16; bc1f L.2476
lw $25,$fp23-12($sp)
blt $25,$0,L.2477
mtc1 $25,$f26; cvt.d.w $f26,$f26
b L.2478
L.2477:
mtc1 $25,$f18; cvt.d.w $f18,$f18
l.d $f16,L.601
add.d $f26,$f18,$f16
L.2478:
l.d $f18,L.2416
c.eq.d $f26,$f18; bc1f L.2476
cvt.d.s $f16,$f28
c.eq.d $f16,$f18; bc1t L.2469
L.2476:
addu $20,$20,64
L.2469:
beq $20,$0,L.2479
addu $19,$19,16
la $4,L.2395
li $5,16
jal printf  # int
L.2479:
move $2,$19
L.2394:
l.d $f26,16($sp)
l.d $f28,24($sp)
l.d $f30,32($sp)
lw $16,40($sp)
lw $17,44($sp)
lw $18,48($sp)
lw $19,52($sp)
lw $20,56($sp)
lw $21,60($sp)
lw $22,64($sp)
lw $23,68($sp)
lw $30,72($sp)
lw $31,76($sp)
addu $sp,$sp,104
j $31
.end s72
.data; L.2482:.ascii "s757,er%d\012\000"
.data; L.2483:.ascii "s757   \000"
.text
.globl s757
.text
.align 2; .ent s757
s757:
$fp24=176
.frame $sp,176,$31
addu $sp,$sp,-176
.mask 0xc0ff0000,-124
sw $16,16($sp)
sw $17,20($sp)
sw $18,24($sp)
sw $19,28($sp)
sw $20,32($sp)
sw $21,36($sp)
sw $22,40($sp)
sw $23,44($sp)
sw $30,48($sp)
sw $31,52($sp)
move $30,$4
la $25,L.2483
sw $25,$fp24-80($sp)
addu $25,$30,60
sw $25,$fp24-84($sp)
sw $0,$fp24-88($sp)
L.2484:
L.2485:
lw $25,$fp24-84($sp)
addu $24,$25,1
sw $24,$fp24-84($sp)
lw $24,$fp24-80($sp)
addu $15,$24,1
sw $15,$fp24-80($sp)
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.2484
li $25,40
sw $25,$fp24-92($sp)
lw $25,$fp24-92($sp)
sll $24,$25,3
sll $24,$24,2
bne $24,1280,L.2489
sra $25,$25,3
sra $25,$25,2
beq $25,1,L.2487
L.2489:
lw $25,$fp24-88($sp)
addu $25,$25,1
sw $25,$fp24-88($sp)
lw $25,44($30)
beq $25,$0,L.2490
la $4,L.2482
li $5,1
jal printf  # int
L.2490:
L.2487:
move $19,$0
sw $0,$fp24-68($sp)
b L.2495
L.2492:
li $17,1
lw $25,16($30)
subu $25,$25,1
sll $16,$17,$25
sw $0,$fp24-72($sp)
lw $25,$fp24-72($sp)
not $25,$25
lw $24,$fp24-68($sp)
srl $15,$25,$24
sw $15,$fp24-76($sp)
sll $25,$25,$24
sw $25,$fp24-72($sp)
move $18,$0
b L.2499
L.2496:
lw $25,$fp24-68($sp)
bge $18,$25,L.2507
li $25,1
sw $25,$fp24-100($sp)
b L.2508
L.2507:
sw $0,$fp24-100($sp)
L.2508:
lw $25,$fp24-72($sp)
and $25,$17,$25
bne $25,$0,L.2509
li $25,1
sw $25,$fp24-104($sp)
b L.2510
L.2509:
sw $0,$fp24-104($sp)
L.2510:
lw $25,$fp24-100($sp)
lw $24,$fp24-104($sp)
bne $25,$24,L.2506
lw $25,$fp24-68($sp)
bge $18,$25,L.2511
li $25,1
sw $25,$fp24-108($sp)
b L.2512
L.2511:
sw $0,$fp24-108($sp)
L.2512:
lw $25,$fp24-76($sp)
and $25,$16,$25
bne $25,$0,L.2513
li $25,1
sw $25,$fp24-112($sp)
b L.2514
L.2513:
sw $0,$fp24-112($sp)
L.2514:
lw $25,$fp24-108($sp)
lw $24,$fp24-112($sp)
beq $25,$24,L.2500
L.2506:
li $19,1
L.2500:
sll $17,$17,1
srl $16,$16,1
L.2497:
addu $18,$18,1
L.2499:
lw $25,16($30)
blt $18,$25,L.2496
L.2493:
lw $25,$fp24-68($sp)
addu $25,$25,1
sw $25,$fp24-68($sp)
L.2495:
lw $25,$fp24-68($sp)
lw $24,16($30)
blt $25,$24,L.2492
beq $19,$0,L.2515
lw $25,$fp24-88($sp)
addu $25,$25,2
sw $25,$fp24-88($sp)
lw $25,44($30)
beq $25,$0,L.2517
la $4,L.2482
li $5,2
jal printf  # int
L.2517:
L.2515:
li $20,3
li $21,2
li $22,1
bge $20,$21,L.2525
li $25,1
sw $25,$fp24-104($sp)
b L.2526
L.2525:
sw $0,$fp24-104($sp)
L.2526:
lw $25,$fp24-104($sp)
bge $25,$22,L.2523
li $25,1
sw $25,$fp24-100($sp)
b L.2524
L.2523:
sw $0,$fp24-100($sp)
L.2524:
lw $25,$fp24-100($sp)
beq $25,1,L.2519
lw $25,$fp24-88($sp)
addu $25,$25,4
sw $25,$fp24-88($sp)
lw $25,44($30)
beq $25,$0,L.2527
la $4,L.2482
li $5,4
jal printf  # int
L.2527:
L.2519:
la $25,$fp24-60($sp)
la $24,$fp24-64($sp)
bne $25,$24,L.2529
lw $25,$fp24-88($sp)
addu $25,$25,8
sw $25,$fp24-88($sp)
lw $25,44($30)
beq $25,$0,L.2532
la $4,L.2482
li $5,8
jal printf  # int
L.2532:
L.2529:
la $25,$fp24-60($sp)
la $24,$fp24-64($sp)
bgeu $25,$24,L.2534
lw $25,40($30)
beq $25,$0,L.2537
la $4,L.2539
jal printf  # int
L.2537:
L.2534:
move $19,$0
move $18,$0
L.2540:
sll $25,$18,2
la $24,$fp24-64($sp)
addu $25,$25,$24
li $24,1
sw $24,($25)
L.2541:
addu $18,$18,1
blt $18,16,L.2540
sw $0,$fp24-60($sp)
sw $0,$fp24-48($sp)
sw $0,$fp24-40($sp)
sw $0,$fp24-36($sp)
sw $0,$fp24-28($sp)
sw $0,$fp24-12($sp)
move $20,$0
L.2550:
move $21,$0
L.2554:
move $22,$0
L.2558:
move $23,$0
L.2562:
bge $20,$21,L.2573
li $25,1
sw $25,$fp24-112($sp)
b L.2574
L.2573:
sw $0,$fp24-112($sp)
L.2574:
bge $22,$23,L.2575
li $25,1
sw $25,$fp24-116($sp)
b L.2576
L.2575:
sw $0,$fp24-116($sp)
L.2576:
lw $25,$fp24-112($sp)
lw $24,$fp24-116($sp)
bne $25,$24,L.2571
li $25,1
sw $25,$fp24-108($sp)
b L.2572
L.2571:
sw $0,$fp24-108($sp)
L.2572:
lw $25,$fp24-108($sp)
sll $24,$20,3
sll $15,$21,2
addu $24,$24,$15
sll $15,$22,1
addu $24,$24,$15
addu $24,$24,$23
sll $24,$24,2
la $15,$fp24-64($sp)
addu $24,$24,$15
lw $24,($24)
beq $25,$24,L.2566
li $19,1
L.2566:
L.2563:
addu $23,$23,1
blt $23,2,L.2562
L.2559:
addu $22,$22,1
blt $22,2,L.2558
L.2555:
addu $21,$21,1
blt $21,2,L.2554
L.2551:
addu $20,$20,1
blt $20,2,L.2550
beq $19,$0,L.2577
lw $25,$fp24-88($sp)
addu $25,$25,16
sw $25,$fp24-88($sp)
lw $25,44($30)
beq $25,$0,L.2579
la $4,L.2482
li $5,16
jal printf  # int
L.2579:
L.2577:
sw $0,$fp24-96($sp)
lw $25,$fp24-96($sp)
beq $25,$0,L.2581
lw $25,$fp24-88($sp)
addu $25,$25,32
sw $25,$fp24-88($sp)
lw $25,44($30)
beq $25,$0,L.2583
la $4,L.2482
li $5,32
jal printf  # int
L.2583:
L.2581:
lw $2,$fp24-88($sp)
L.2481:
lw $16,16($sp)
lw $17,20($sp)
lw $18,24($sp)
lw $19,28($sp)
lw $20,32($sp)
lw $21,36($sp)
lw $22,40($sp)
lw $23,44($sp)
lw $30,48($sp)
lw $31,52($sp)
addu $sp,$sp,176
j $31
.end s757
.data; L.2586:.ascii "Local error %d.\012\000"
.data; L.2587:.ascii "s7813,er%d\012\000"
.data; L.2588:.ascii "s7813  \000"
.text
.globl s7813
.text
.align 2; .ent s7813
s7813:
$fp25=112
.frame $sp,112,$31
addu $sp,$sp,-112
.mask 0xc0ff0000,-60
sw $16,16($sp)
sw $17,20($sp)
sw $18,24($sp)
sw $19,28($sp)
sw $20,32($sp)
sw $21,36($sp)
sw $22,40($sp)
sw $23,44($sp)
sw $30,48($sp)
sw $31,52($sp)
sw $4,112($sp)
la $19,L.2588
lw $25,$fp25+0($sp)
addu $18,$25,60
move $23,$0
move $17,$0
lw $30,48($25)
L.2589:
L.2590:
move $25,$18
addu $18,$25,1
move $24,$19
addu $19,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.2589
b L.2592
li $23,1
beq $30,$0,L.2594
la $4,L.2586
move $5,$23
jal printf  # int
L.2594:
L.2592:
b L.2596
li $23,2
beq $30,$0,L.2598
la $4,L.2586
move $5,$23
jal printf  # int
L.2598:
L.2596:
b L.2600
li $23,3
beq $30,$0,L.2602
la $4,L.2586
move $5,$23
jal printf  # int
L.2602:
L.2600:
b L.2604
li $23,4
beq $30,$0,L.2606
la $4,L.2586
move $5,$23
jal printf  # int
L.2606:
L.2604:
b L.2608
li $23,5
beq $30,$0,L.2610
la $4,L.2586
move $5,$23
jal printf  # int
L.2610:
L.2608:
b L.2612
li $23,6
beq $30,$0,L.2614
la $4,L.2586
move $5,$23
jal printf  # int
L.2614:
L.2612:
b L.2616
li $23,7
beq $30,$0,L.2618
la $4,L.2586
move $5,$23
jal printf  # int
L.2618:
L.2616:
b L.2620
li $23,8
beq $30,$0,L.2622
la $4,L.2586
move $5,$23
jal printf  # int
L.2622:
L.2620:
b L.2624
li $23,9
beq $30,$0,L.2626
la $4,L.2586
move $5,$23
jal printf  # int
L.2626:
L.2624:
b L.2628
li $23,10
beq $30,$0,L.2630
la $4,L.2586
move $5,$23
jal printf  # int
L.2630:
L.2628:
b L.2632
li $23,11
beq $30,$0,L.2634
la $4,L.2586
move $5,$23
jal printf  # int
L.2634:
L.2632:
b L.2636
li $23,12
beq $30,$0,L.2638
la $4,L.2586
move $5,$23
jal printf  # int
L.2638:
L.2636:
b L.2640
li $23,13
beq $30,$0,L.2642
la $4,L.2586
move $5,$23
jal printf  # int
L.2642:
L.2640:
b L.2644
li $23,14
beq $30,$0,L.2646
la $4,L.2586
move $5,$23
jal printf  # int
L.2646:
L.2644:
b L.2648
li $23,15
beq $30,$0,L.2650
la $4,L.2586
move $5,$23
jal printf  # int
L.2650:
L.2648:
b L.2652
li $23,16
beq $30,$0,L.2654
la $4,L.2586
move $5,$23
jal printf  # int
L.2654:
L.2652:
b L.2656
li $23,17
beq $30,$0,L.2658
la $4,L.2586
move $5,$23
jal printf  # int
L.2658:
L.2656:
b L.2660
li $23,18
beq $30,$0,L.2662
la $4,L.2586
move $5,$23
jal printf  # int
L.2662:
L.2660:
b L.2664
li $23,16
beq $30,$0,L.2666
la $4,L.2586
move $5,$23
jal printf  # int
L.2666:
L.2664:
b L.2668
li $23,20
beq $30,$0,L.2670
la $4,L.2586
move $5,$23
jal printf  # int
L.2670:
L.2668:
b L.2672
li $23,21
beq $30,$0,L.2674
la $4,L.2586
move $5,$23
jal printf  # int
L.2674:
L.2672:
b L.2676
li $23,22
beq $30,$0,L.2678
la $4,L.2586
move $5,$23
jal printf  # int
L.2678:
L.2676:
b L.2680
li $23,23
beq $30,$0,L.2682
la $4,L.2586
move $5,$23
jal printf  # int
L.2682:
L.2680:
b L.2684
li $23,24
beq $30,$0,L.2686
la $4,L.2586
move $5,$23
jal printf  # int
L.2686:
L.2684:
b L.2688
li $23,25
beq $30,$0,L.2690
la $4,L.2586
move $5,$23
jal printf  # int
L.2690:
L.2688:
b L.2692
li $23,26
beq $30,$0,L.2694
la $4,L.2586
move $5,$23
jal printf  # int
L.2694:
L.2692:
b L.2696
li $23,27
beq $30,$0,L.2698
la $4,L.2586
move $5,$23
jal printf  # int
L.2698:
L.2696:
b L.2700
li $23,28
beq $30,$0,L.2702
la $4,L.2586
move $5,$23
jal printf  # int
L.2702:
L.2700:
b L.2704
li $23,26
beq $30,$0,L.2706
la $4,L.2586
move $5,$23
jal printf  # int
L.2706:
L.2704:
b L.2708
li $23,30
beq $30,$0,L.2710
la $4,L.2586
move $5,$23
jal printf  # int
L.2710:
L.2708:
b L.2712
li $23,31
beq $30,$0,L.2714
la $4,L.2586
move $5,$23
jal printf  # int
L.2714:
L.2712:
b L.2716
li $23,32
beq $30,$0,L.2718
la $4,L.2586
move $5,$23
jal printf  # int
L.2718:
L.2716:
b L.2720
li $23,33
beq $30,$0,L.2722
la $4,L.2586
move $5,$23
jal printf  # int
L.2722:
L.2720:
b L.2724
li $23,34
beq $30,$0,L.2726
la $4,L.2586
move $5,$23
jal printf  # int
L.2726:
L.2724:
b L.2728
li $23,35
beq $30,$0,L.2730
la $4,L.2586
move $5,$23
jal printf  # int
L.2730:
L.2728:
b L.2732
li $23,36
beq $30,$0,L.2734
la $4,L.2586
move $5,$23
jal printf  # int
L.2734:
L.2732:
b L.2736
li $23,37
beq $30,$0,L.2738
la $4,L.2586
move $5,$23
jal printf  # int
L.2738:
L.2736:
b L.2740
li $23,38
beq $30,$0,L.2742
la $4,L.2586
move $5,$23
jal printf  # int
L.2742:
L.2740:
b L.2744
li $23,39
beq $30,$0,L.2746
la $4,L.2586
move $5,$23
jal printf  # int
L.2746:
L.2744:
b L.2748
li $23,40
beq $30,$0,L.2750
la $4,L.2586
move $5,$23
jal printf  # int
L.2750:
L.2748:
b L.2752
li $23,41
beq $30,$0,L.2754
la $4,L.2586
move $5,$23
jal printf  # int
L.2754:
L.2752:
b L.2756
li $23,42
beq $30,$0,L.2758
la $4,L.2586
move $5,$23
jal printf  # int
L.2758:
L.2756:
b L.2760
li $23,43
beq $30,$0,L.2762
la $4,L.2586
move $5,$23
jal printf  # int
L.2762:
L.2760:
b L.2764
li $23,44
beq $30,$0,L.2766
la $4,L.2586
move $5,$23
jal printf  # int
L.2766:
L.2764:
b L.2768
li $23,45
beq $30,$0,L.2770
la $4,L.2586
move $5,$23
jal printf  # int
L.2770:
L.2768:
b L.2772
li $23,46
beq $30,$0,L.2774
la $4,L.2586
move $5,$23
jal printf  # int
L.2774:
L.2772:
b L.2776
li $23,47
beq $30,$0,L.2778
la $4,L.2586
move $5,$23
jal printf  # int
L.2778:
L.2776:
b L.2780
li $23,48
beq $30,$0,L.2782
la $4,L.2586
move $5,$23
jal printf  # int
L.2782:
L.2780:
b L.2784
li $23,49
beq $30,$0,L.2786
la $4,L.2586
move $5,$23
jal printf  # int
L.2786:
L.2784:
b L.2788
li $23,50
beq $30,$0,L.2790
la $4,L.2586
move $5,$23
jal printf  # int
L.2790:
L.2788:
b L.2792
li $23,51
beq $30,$0,L.2794
la $4,L.2586
move $5,$23
jal printf  # int
L.2794:
L.2792:
b L.2796
li $23,52
beq $30,$0,L.2798
la $4,L.2586
move $5,$23
jal printf  # int
L.2798:
L.2796:
b L.2800
li $23,53
beq $30,$0,L.2802
la $4,L.2586
move $5,$23
jal printf  # int
L.2802:
L.2800:
b L.2804
li $23,54
beq $30,$0,L.2806
la $4,L.2586
move $5,$23
jal printf  # int
L.2806:
L.2804:
b L.2808
li $23,55
beq $30,$0,L.2810
la $4,L.2586
move $5,$23
jal printf  # int
L.2810:
L.2808:
b L.2812
li $23,56
beq $30,$0,L.2814
la $4,L.2586
move $5,$23
jal printf  # int
L.2814:
L.2812:
b L.2816
li $23,57
beq $30,$0,L.2818
la $4,L.2586
move $5,$23
jal printf  # int
L.2818:
L.2816:
b L.2820
li $23,58
beq $30,$0,L.2822
la $4,L.2586
move $5,$23
jal printf  # int
L.2822:
L.2820:
b L.2824
li $23,56
beq $30,$0,L.2826
la $4,L.2586
move $5,$23
jal printf  # int
L.2826:
L.2824:
b L.2828
li $23,60
beq $30,$0,L.2830
la $4,L.2586
move $5,$23
jal printf  # int
L.2830:
L.2828:
b L.2832
li $23,61
beq $30,$0,L.2834
la $4,L.2586
move $5,$23
jal printf  # int
L.2834:
L.2832:
b L.2836
li $23,62
beq $30,$0,L.2838
la $4,L.2586
move $5,$23
jal printf  # int
L.2838:
L.2836:
b L.2840
li $23,63
beq $30,$0,L.2842
la $4,L.2586
move $5,$23
jal printf  # int
L.2842:
L.2840:
b L.2844
li $23,64
beq $30,$0,L.2846
la $4,L.2586
move $5,$23
jal printf  # int
L.2846:
L.2844:
b L.2848
li $23,65
beq $30,$0,L.2850
la $4,L.2586
move $5,$23
jal printf  # int
L.2850:
L.2848:
b L.2852
li $23,66
beq $30,$0,L.2854
la $4,L.2586
move $5,$23
jal printf  # int
L.2854:
L.2852:
b L.2856
li $23,67
beq $30,$0,L.2858
la $4,L.2586
move $5,$23
jal printf  # int
L.2858:
L.2856:
b L.2860
li $23,68
beq $30,$0,L.2862
la $4,L.2586
move $5,$23
jal printf  # int
L.2862:
L.2860:
b L.2864
li $23,69
beq $30,$0,L.2866
la $4,L.2586
move $5,$23
jal printf  # int
L.2866:
L.2864:
b L.2868
li $23,70
beq $30,$0,L.2870
la $4,L.2586
move $5,$23
jal printf  # int
L.2870:
L.2868:
b L.2872
li $23,71
beq $30,$0,L.2874
la $4,L.2586
move $5,$23
jal printf  # int
L.2874:
L.2872:
b L.2876
li $23,72
beq $30,$0,L.2878
la $4,L.2586
move $5,$23
jal printf  # int
L.2878:
L.2876:
b L.2880
li $23,73
beq $30,$0,L.2882
la $4,L.2586
move $5,$23
jal printf  # int
L.2882:
L.2880:
b L.2884
li $23,74
beq $30,$0,L.2886
la $4,L.2586
move $5,$23
jal printf  # int
L.2886:
L.2884:
b L.2888
li $23,75
beq $30,$0,L.2890
la $4,L.2586
move $5,$23
jal printf  # int
L.2890:
L.2888:
beq $23,$0,L.2892
lw $25,$fp25+0($sp)
lw $25,44($25)
beq $25,$0,L.2894
la $4,L.2587
li $5,1
jal printf  # int
L.2894:
addu $17,$17,1
L.2892:
move $23,$0
move $21,$0
move $22,$0
move $25,$22
addu $22,$25,1
beq $25,$0,L.2897
move $25,$21
addu $21,$25,1
beq $25,$0,L.2897
li $25,1
sw $25,$fp25-8($sp)
b L.2898
L.2897:
sw $0,$fp25-8($sp)
L.2898:
lw $20,$fp25-8($sp)
beq $22,1,L.2899
li $23,1
beq $30,$0,L.2901
la $4,L.2586
move $5,$23
jal printf  # int
L.2901:
L.2899:
beq $21,$0,L.2903
li $23,2
beq $30,$0,L.2905
la $4,L.2586
move $5,$23
jal printf  # int
L.2905:
L.2903:
beq $20,$0,L.2907
li $23,3
beq $30,$0,L.2909
la $4,L.2586
move $5,$23
jal printf  # int
L.2909:
L.2907:
beq $22,$0,L.2912
move $25,$21
addu $21,$25,1
beq $25,$0,L.2912
li $25,1
sw $25,$fp25-12($sp)
b L.2913
L.2912:
sw $0,$fp25-12($sp)
L.2913:
lw $20,$fp25-12($sp)
beq $22,1,L.2914
li $23,4
beq $30,$0,L.2916
la $4,L.2586
move $5,$23
jal printf  # int
L.2916:
L.2914:
beq $21,1,L.2918
li $23,5
beq $30,$0,L.2920
la $4,L.2586
move $5,$23
jal printf  # int
L.2920:
L.2918:
beq $20,$0,L.2922
li $23,6
beq $30,$0,L.2924
la $4,L.2586
move $5,$23
jal printf  # int
L.2924:
L.2922:
move $25,$22
subu $22,$25,1
beq $25,$0,L.2927
beq $21,$0,L.2927
li $25,1
sw $25,$fp25-16($sp)
b L.2928
L.2927:
sw $0,$fp25-16($sp)
L.2928:
lw $20,$fp25-16($sp)
beq $22,$0,L.2929
li $23,7
beq $30,$0,L.2931
la $4,L.2586
move $5,$23
jal printf  # int
L.2931:
L.2929:
beq $21,1,L.2933
li $23,8
beq $30,$0,L.2935
la $4,L.2586
move $5,$23
jal printf  # int
L.2935:
L.2933:
beq $20,1,L.2937
li $23,9
beq $30,$0,L.2939
la $4,L.2586
move $5,$23
jal printf  # int
L.2939:
L.2937:
beq $22,$0,L.2942
move $25,$21
subu $21,$25,1
beq $25,$0,L.2942
li $25,1
sw $25,$fp25-20($sp)
b L.2943
L.2942:
sw $0,$fp25-20($sp)
L.2943:
lw $20,$fp25-20($sp)
beq $22,$0,L.2944
li $23,10
beq $30,$0,L.2946
la $4,L.2586
move $5,$23
jal printf  # int
L.2946:
L.2944:
beq $21,1,L.2948
li $23,11
beq $30,$0,L.2950
la $4,L.2586
move $5,$23
jal printf  # int
L.2950:
L.2948:
beq $20,$0,L.2952
li $23,12
beq $30,$0,L.2954
la $4,L.2586
move $5,$23
jal printf  # int
L.2954:
L.2952:
beq $23,$0,L.2956
lw $25,$fp25+0($sp)
lw $25,44($25)
beq $25,$0,L.2958
la $4,L.2587
li $5,2
jal printf  # int
L.2958:
addu $17,$17,2
L.2956:
move $23,$0
move $21,$0
move $22,$0
move $25,$22
addu $22,$25,1
bne $25,$0,L.2963
beq $21,$0,L.2961
L.2963:
li $25,1
sw $25,$fp25-24($sp)
b L.2962
L.2961:
sw $0,$fp25-24($sp)
L.2962:
lw $20,$fp25-24($sp)
beq $22,1,L.2964
li $23,1
beq $30,$0,L.2966
la $4,L.2586
move $5,$23
jal printf  # int
L.2966:
L.2964:
beq $21,$0,L.2968
li $23,2
beq $30,$0,L.2970
la $4,L.2586
move $5,$23
jal printf  # int
L.2970:
L.2968:
beq $20,$0,L.2972
li $23,3
beq $30,$0,L.2974
la $4,L.2586
move $5,$23
jal printf  # int
L.2974:
L.2972:
move $25,$21
addu $21,$25,1
bne $25,$0,L.2979
beq $22,$0,L.2977
L.2979:
li $25,1
sw $25,$fp25-28($sp)
b L.2978
L.2977:
sw $0,$fp25-28($sp)
L.2978:
lw $20,$fp25-28($sp)
beq $22,1,L.2980
li $23,4
beq $30,$0,L.2982
la $4,L.2586
move $5,$23
jal printf  # int
L.2982:
L.2980:
beq $21,1,L.2984
li $23,5
beq $30,$0,L.2986
la $4,L.2586
move $5,$23
jal printf  # int
L.2986:
L.2984:
beq $20,1,L.2988
li $23,6
beq $30,$0,L.2990
la $4,L.2586
move $5,$23
jal printf  # int
L.2990:
L.2988:
move $25,$22
subu $22,$25,1
bne $25,$0,L.2995
move $25,$21
subu $21,$25,1
beq $25,$0,L.2993
L.2995:
li $25,1
sw $25,$fp25-32($sp)
b L.2994
L.2993:
sw $0,$fp25-32($sp)
L.2994:
lw $20,$fp25-32($sp)
beq $22,$0,L.2996
li $23,7
beq $30,$0,L.2998
la $4,L.2586
move $5,$23
jal printf  # int
L.2998:
L.2996:
beq $21,1,L.3000
li $23,8
beq $30,$0,L.3002
la $4,L.2586
move $5,$23
jal printf  # int
L.3002:
L.3000:
beq $20,1,L.3004
li $23,9
beq $30,$0,L.3006
la $4,L.2586
move $5,$23
jal printf  # int
L.3006:
L.3004:
bne $22,$0,L.3011
move $25,$21
subu $21,$25,1
beq $25,$0,L.3009
L.3011:
li $25,1
sw $25,$fp25-36($sp)
b L.3010
L.3009:
sw $0,$fp25-36($sp)
L.3010:
lw $20,$fp25-36($sp)
beq $22,$0,L.3012
li $23,10
beq $30,$0,L.3014
la $4,L.2586
move $5,$23
jal printf  # int
L.3014:
L.3012:
beq $21,$0,L.3016
li $23,11
beq $30,$0,L.3018
la $4,L.2586
move $5,$23
jal printf  # int
L.3018:
L.3016:
beq $20,1,L.3020
li $23,12
beq $30,$0,L.3022
la $4,L.2586
move $5,$23
jal printf  # int
L.3022:
L.3020:
beq $23,$0,L.3024
lw $25,$fp25+0($sp)
lw $25,44($25)
beq $25,$0,L.3026
la $4,L.2587
li $5,4
jal printf  # int
L.3026:
addu $17,$17,4
L.3024:
move $21,$0
move $22,$0
move $16,$0
li $25,1
sw $25,$fp25-4($sp)
lw $25,$fp25-4($sp)
beq $25,$0,L.3030
sw $16,$fp25-40($sp)
b L.3031
L.3030:
lw $25,$fp25-4($sp)
beq $25,$0,L.3032
move $25,$22
addu $22,$25,1
sw $25,$fp25-44($sp)
b L.3033
L.3032:
move $25,$21
addu $21,$25,1
sw $25,$fp25-44($sp)
L.3033:
lw $25,$fp25-44($sp)
sw $25,$fp25-40($sp)
L.3031:
lw $20,$fp25-40($sp)
bne $20,$0,L.3037
bne $22,$0,L.3037
beq $21,$0,L.3034
L.3037:
lw $25,$fp25+0($sp)
lw $25,44($25)
beq $25,$0,L.3038
la $4,L.2587
li $5,8
jal printf  # int
L.3038:
addu $17,$17,8
L.3034:
lw $25,$fp25-4($sp)
beq $25,$0,L.3045
sw $16,$fp25-48($sp)
b L.3046
L.3045:
li $25,1
sw $25,$fp25-48($sp)
L.3046:
lw $25,$fp25-48($sp)
bne $25,$0,L.3044
beq $16,$0,L.3047
li $25,1
sw $25,$fp25-52($sp)
b L.3048
L.3047:
sw $16,$fp25-52($sp)
L.3048:
lw $25,$fp25-52($sp)
beq $25,$0,L.3040
L.3044:
lw $25,$fp25+0($sp)
lw $25,44($25)
beq $25,$0,L.3049
la $4,L.2587
li $5,16
jal printf  # int
L.3049:
addu $17,$17,16
L.3040:
move $2,$17
L.2585:
lw $16,16($sp)
lw $17,20($sp)
lw $18,24($sp)
lw $19,28($sp)
lw $20,32($sp)
lw $21,36($sp)
lw $22,40($sp)
lw $23,44($sp)
lw $30,48($sp)
lw $31,52($sp)
addu $sp,$sp,112
j $31
.end s7813
.data; L.3052:.ascii "s81,er%d\012\000"
.data; L.3053:.ascii "s81    \000"
.data; L.3054:.ascii "Register count for %s is unreliable.\012\000"
.data; L.3055:.ascii "%d registers assigned to %s variables.\012\000"
.text
.globl s81
.text
.align 2; .ent s81
s81:
$fp26=80
.frame $sp,80,$31
addu $sp,$sp,-80
.mask 0xc0ff0000,-28
sw $16,16($sp)
sw $17,20($sp)
sw $18,24($sp)
sw $19,28($sp)
sw $20,32($sp)
sw $21,36($sp)
sw $22,40($sp)
sw $23,44($sp)
sw $30,48($sp)
sw $31,52($sp)
sw $4,80($sp)
sw $0,$fp26-20($sp)
sw $0,$fp26-8($sp)
sw $0,$fp26-12($sp)
sw $0,$fp26-16($sp)
la $17,L.3053
lw $25,$fp26+0($sp)
addu $16,$25,60
L.3056:
L.3057:
move $25,$16
addu $16,$25,1
move $24,$17
addu $17,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.3056
li $25,1
sw $25,$fp26-4($sp)
move $21,$0
L.3059:
lw $25,$fp26-4($sp)
move $24,$25
move $30,$24
move $20,$24
la $24,$fp26-4($sp)
move $23,$24
move $19,$24
move $22,$25
move $18,$25
sll $25,$30,24; sra $25,$25,24
sll $24,$20,24; sra $24,$24,24
beq $25,$24,L.3063
li $25,1
sw $25,$fp26-8($sp)
L.3063:
beq $23,$19,L.3065
li $25,1
sw $25,$fp26-12($sp)
L.3065:
beq $22,$18,L.3067
li $25,1
sw $25,$fp26-16($sp)
L.3067:
lw $25,$fp26-4($sp)
sll $25,$25,1
sw $25,$fp26-4($sp)
L.3060:
addu $21,$21,1
blt $21,50,L.3059
lw $25,$fp26-8($sp)
beq $25,$0,L.3069
lw $25,$fp26-20($sp)
addu $25,$25,1
sw $25,$fp26-20($sp)
lw $25,$fp26+0($sp)
lw $25,44($25)
beq $25,$0,L.3071
la $4,L.3052
li $5,1
jal printf  # int
L.3071:
L.3069:
lw $25,$fp26-12($sp)
beq $25,$0,L.3073
lw $25,$fp26-20($sp)
addu $25,$25,2
sw $25,$fp26-20($sp)
lw $25,$fp26+0($sp)
lw $25,44($25)
beq $25,$0,L.3075
la $4,L.3052
li $5,2
jal printf  # int
L.3075:
L.3073:
lw $25,$fp26-16($sp)
beq $25,$0,L.3077
lw $25,$fp26-20($sp)
addu $25,$25,4
sw $25,$fp26-20($sp)
lw $25,$fp26+0($sp)
lw $25,44($25)
beq $25,$0,L.3079
la $4,L.3052
li $5,4
jal printf  # int
L.3079:
L.3077:
jal regc  # int
sw $2,$fp26-4($sp)
lw $25,$fp26+0($sp)
lw $25,40($25)
beq $25,$0,L.3081
lw $25,$fp26-4($sp)
bge $25,$0,L.3083
la $4,L.3054
la $5,L.426
jal printf  # int
b L.3084
L.3083:
la $4,L.3055
lw $5,$fp26-4($sp)
la $6,L.426
jal printf  # int
L.3084:
L.3081:
jal regp  # int
sw $2,$fp26-4($sp)
lw $25,$fp26+0($sp)
lw $25,40($25)
beq $25,$0,L.3085
lw $25,$fp26-4($sp)
bge $25,$0,L.3087
la $4,L.3054
la $5,L.3089
jal printf  # int
b L.3088
L.3087:
la $4,L.3055
lw $5,$fp26-4($sp)
la $6,L.3089
jal printf  # int
L.3088:
L.3085:
jal regi  # int
sw $2,$fp26-4($sp)
lw $25,$fp26+0($sp)
lw $25,40($25)
beq $25,$0,L.3090
lw $25,$fp26-4($sp)
bge $25,$0,L.3092
la $4,L.3054
la $5,L.427
jal printf  # int
b L.3093
L.3092:
la $4,L.3055
lw $5,$fp26-4($sp)
la $6,L.427
jal printf  # int
L.3093:
L.3090:
lw $2,$fp26-20($sp)
L.3051:
lw $16,16($sp)
lw $17,20($sp)
lw $18,24($sp)
lw $19,28($sp)
lw $20,32($sp)
lw $21,36($sp)
lw $22,40($sp)
lw $23,44($sp)
lw $30,48($sp)
lw $31,52($sp)
addu $sp,$sp,80
j $31
.end s81
.globl regc
.text
.align 2; .ent regc
regc:
$fp27=144
.frame $sp,144,$31
addu $sp,$sp,-144
.mask 0x40000000,-144
sw $30,0($sp)
sb $0,$fp27-130($sp)
li $25,1
sb $25,$fp27-109($sp)
li $25,2
sb $25,$fp27-110($sp)
li $25,3
sb $25,$fp27-111($sp)
li $30,4
li $25,5
sb $25,$fp27-112($sp)
li $8,6
li $25,7
sb $25,$fp27-113($sp)
li $9,8
li $25,9
sb $25,$fp27-114($sp)
li $10,10
li $25,11
sb $25,$fp27-115($sp)
li $11,12
li $25,13
sb $25,$fp27-116($sp)
li $12,14
li $25,15
sb $25,$fp27-117($sp)
li $13,16
li $25,17
sb $25,$fp27-118($sp)
li $14,18
li $25,19
sb $25,$fp27-119($sp)
li $15,20
li $25,21
sb $25,$fp27-120($sp)
li $25,22
sb $25,$fp27-1($sp)
li $25,23
sb $25,$fp27-121($sp)
li $25,24
sb $25,$fp27-2($sp)
li $25,25
sb $25,$fp27-122($sp)
li $25,26
sb $25,$fp27-3($sp)
li $25,27
sb $25,$fp27-123($sp)
li $25,28
sb $25,$fp27-4($sp)
li $25,29
sb $25,$fp27-124($sp)
li $25,30
sb $25,$fp27-5($sp)
li $25,31
sb $25,$fp27-125($sp)
li $25,32
sb $25,$fp27-6($sp)
li $25,33
sb $25,$fp27-126($sp)
li $25,34
sb $25,$fp27-7($sp)
li $25,35
sb $25,$fp27-127($sp)
li $25,36
sb $25,$fp27-128($sp)
li $25,37
sb $25,$fp27-129($sp)
li $25,38
sb $25,$fp27-131($sp)
la $25,$fp27-109($sp)
la $24,$fp27-130($sp)
subu $24,$25,$24
sw $24,$fp27-96($sp)
la $24,$fp27-110($sp)
subu $25,$24,$25
sw $25,$fp27-92($sp)
la $25,$fp27-111($sp)
la $24,$fp27-110($sp)
subu $25,$25,$24
sw $25,$fp27-88($sp)
la $25,$fp27-112($sp)
la $24,$fp27-111($sp)
subu $25,$25,$24
sw $25,$fp27-84($sp)
la $25,$fp27-113($sp)
la $24,$fp27-112($sp)
subu $25,$25,$24
sw $25,$fp27-80($sp)
la $25,$fp27-114($sp)
la $24,$fp27-113($sp)
subu $25,$25,$24
sw $25,$fp27-76($sp)
la $25,$fp27-115($sp)
la $24,$fp27-114($sp)
subu $25,$25,$24
sw $25,$fp27-72($sp)
la $25,$fp27-116($sp)
la $24,$fp27-115($sp)
subu $25,$25,$24
sw $25,$fp27-68($sp)
la $25,$fp27-117($sp)
la $24,$fp27-116($sp)
subu $25,$25,$24
sw $25,$fp27-64($sp)
la $25,$fp27-118($sp)
la $24,$fp27-117($sp)
subu $25,$25,$24
sw $25,$fp27-60($sp)
la $25,$fp27-119($sp)
la $24,$fp27-118($sp)
subu $25,$25,$24
sw $25,$fp27-56($sp)
la $25,$fp27-120($sp)
la $24,$fp27-119($sp)
subu $25,$25,$24
sw $25,$fp27-52($sp)
la $25,$fp27-121($sp)
la $24,$fp27-120($sp)
subu $25,$25,$24
sw $25,$fp27-48($sp)
la $25,$fp27-122($sp)
la $24,$fp27-121($sp)
subu $25,$25,$24
sw $25,$fp27-44($sp)
la $25,$fp27-123($sp)
la $24,$fp27-122($sp)
subu $25,$25,$24
sw $25,$fp27-40($sp)
la $25,$fp27-124($sp)
la $24,$fp27-123($sp)
subu $25,$25,$24
sw $25,$fp27-36($sp)
la $25,$fp27-125($sp)
la $24,$fp27-124($sp)
subu $25,$25,$24
sw $25,$fp27-32($sp)
la $25,$fp27-126($sp)
la $24,$fp27-125($sp)
subu $25,$25,$24
sw $25,$fp27-28($sp)
la $25,$fp27-127($sp)
la $24,$fp27-126($sp)
subu $25,$25,$24
sw $25,$fp27-24($sp)
la $25,$fp27-128($sp)
la $24,$fp27-127($sp)
subu $25,$25,$24
sw $25,$fp27-20($sp)
la $25,$fp27-129($sp)
la $24,$fp27-128($sp)
subu $25,$25,$24
sw $25,$fp27-16($sp)
la $25,$fp27-131($sp)
la $24,$fp27-129($sp)
subu $25,$25,$24
sw $25,$fp27-12($sp)
lw $25,$fp27-96($sp)
sw $25,$fp27-108($sp)
li $25,1
sw $25,$fp27-104($sp)
sw $0,$fp27-100($sp)
L.3116:
lw $25,$fp27-104($sp)
beq $25,1,L.3122
beq $25,2,L.3125
beq $25,3,L.3130
b L.3120
L.3122:
lw $25,$fp27-100($sp)
sll $25,$25,2
la $24,$fp27-96($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp27-108($sp)
beq $25,$24,L.3121
lw $25,$fp27-100($sp)
sll $25,$25,2
la $24,$fp27-96($sp)
addu $25,$25,$24
lw $25,($25)
sw $25,$fp27-140($sp)
li $25,2
sw $25,$fp27-104($sp)
li $25,1
sw $25,$fp27-136($sp)
b L.3121
L.3125:
lw $25,$fp27-100($sp)
sll $25,$25,2
la $24,$fp27-96($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp27-108($sp)
bne $25,$24,L.3126
li $25,3
sw $25,$fp27-104($sp)
b L.3121
L.3126:
lw $25,$fp27-100($sp)
sll $25,$25,2
la $24,$fp27-96($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp27-140($sp)
bne $25,$24,L.3128
lw $25,$fp27-136($sp)
addu $25,$25,1
sw $25,$fp27-136($sp)
b L.3121
L.3128:
li $25,4
sw $25,$fp27-104($sp)
b L.3121
L.3130:
lw $25,$fp27-100($sp)
sll $25,$25,2
la $24,$fp27-96($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp27-108($sp)
beq $25,$24,L.3121
li $25,4
sw $25,$fp27-104($sp)
L.3120:
L.3121:
L.3117:
lw $25,$fp27-100($sp)
addu $25,$25,1
sw $25,$fp27-100($sp)
lw $25,$fp27-100($sp)
blt $25,22,L.3116
lw $25,$fp27-104($sp)
bne $25,3,L.3133
li $25,16
lw $24,$fp27-136($sp)
subu $2,$25,$24
b L.3094
L.3133:
li $2,-1
L.3094:
lw $30,0($sp)
addu $sp,$sp,144
j $31
.end regc
.globl regi
.text
.align 2; .ent regi
regi:
$fp28=232
.frame $sp,232,$31
addu $sp,$sp,-232
.mask 0x40000000,-232
sw $30,0($sp)
sw $0,$fp28-216($sp)
li $25,1
sw $25,$fp28-132($sp)
li $25,2
sw $25,$fp28-136($sp)
li $25,3
sw $25,$fp28-140($sp)
li $25,4
move $30,$25
li $25,5
sw $25,$fp28-144($sp)
li $8,6
li $25,7
sw $25,$fp28-148($sp)
li $9,8
li $25,9
sw $25,$fp28-152($sp)
li $10,10
li $25,11
sw $25,$fp28-156($sp)
li $11,12
li $25,13
sw $25,$fp28-160($sp)
li $12,14
li $25,15
sw $25,$fp28-164($sp)
li $13,16
li $25,17
sw $25,$fp28-168($sp)
li $14,18
li $25,19
sw $25,$fp28-172($sp)
li $15,20
li $25,21
sw $25,$fp28-176($sp)
li $25,22
sw $25,$fp28-4($sp)
li $25,23
sw $25,$fp28-180($sp)
li $25,24
sw $25,$fp28-8($sp)
li $25,25
sw $25,$fp28-184($sp)
li $25,26
sw $25,$fp28-12($sp)
li $25,27
sw $25,$fp28-188($sp)
li $25,28
sw $25,$fp28-16($sp)
li $25,29
sw $25,$fp28-192($sp)
li $25,30
sw $25,$fp28-20($sp)
li $25,31
sw $25,$fp28-196($sp)
li $25,32
sw $25,$fp28-24($sp)
li $25,33
sw $25,$fp28-200($sp)
li $25,34
sw $25,$fp28-28($sp)
li $25,35
sw $25,$fp28-204($sp)
li $25,36
sw $25,$fp28-208($sp)
li $25,37
sw $25,$fp28-212($sp)
li $25,38
sw $25,$fp28-220($sp)
la $25,$fp28-132($sp)
la $24,$fp28-216($sp)
subu $24,$25,$24
div $24,$24,4
sw $24,$fp28-116($sp)
la $24,$fp28-136($sp)
subu $25,$24,$25
div $25,$25,4
sw $25,$fp28-112($sp)
la $25,$fp28-140($sp)
la $24,$fp28-136($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-108($sp)
la $25,$fp28-144($sp)
la $24,$fp28-140($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-104($sp)
la $25,$fp28-148($sp)
la $24,$fp28-144($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-100($sp)
la $25,$fp28-152($sp)
la $24,$fp28-148($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-96($sp)
la $25,$fp28-156($sp)
la $24,$fp28-152($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-92($sp)
la $25,$fp28-160($sp)
la $24,$fp28-156($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-88($sp)
la $25,$fp28-164($sp)
la $24,$fp28-160($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-84($sp)
la $25,$fp28-168($sp)
la $24,$fp28-164($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-80($sp)
la $25,$fp28-172($sp)
la $24,$fp28-168($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-76($sp)
la $25,$fp28-176($sp)
la $24,$fp28-172($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-72($sp)
la $25,$fp28-180($sp)
la $24,$fp28-176($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-68($sp)
la $25,$fp28-184($sp)
la $24,$fp28-180($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-64($sp)
la $25,$fp28-188($sp)
la $24,$fp28-184($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-60($sp)
la $25,$fp28-192($sp)
la $24,$fp28-188($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-56($sp)
la $25,$fp28-196($sp)
la $24,$fp28-192($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-52($sp)
la $25,$fp28-200($sp)
la $24,$fp28-196($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-48($sp)
la $25,$fp28-204($sp)
la $24,$fp28-200($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-44($sp)
la $25,$fp28-208($sp)
la $24,$fp28-204($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-40($sp)
la $25,$fp28-212($sp)
la $24,$fp28-208($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-36($sp)
la $25,$fp28-220($sp)
la $24,$fp28-212($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp28-32($sp)
lw $25,$fp28-116($sp)
sw $25,$fp28-128($sp)
li $25,1
sw $25,$fp28-124($sp)
sw $0,$fp28-120($sp)
L.3157:
lw $25,$fp28-124($sp)
beq $25,1,L.3163
beq $25,2,L.3166
beq $25,3,L.3171
b L.3161
L.3163:
lw $25,$fp28-120($sp)
sll $25,$25,2
la $24,$fp28-116($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp28-128($sp)
beq $25,$24,L.3162
lw $25,$fp28-120($sp)
sll $25,$25,2
la $24,$fp28-116($sp)
addu $25,$25,$24
lw $25,($25)
sw $25,$fp28-228($sp)
li $25,2
sw $25,$fp28-124($sp)
li $25,1
sw $25,$fp28-224($sp)
b L.3162
L.3166:
lw $25,$fp28-120($sp)
sll $25,$25,2
la $24,$fp28-116($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp28-128($sp)
bne $25,$24,L.3167
li $25,3
sw $25,$fp28-124($sp)
b L.3162
L.3167:
lw $25,$fp28-120($sp)
sll $25,$25,2
la $24,$fp28-116($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp28-228($sp)
bne $25,$24,L.3169
lw $25,$fp28-224($sp)
addu $25,$25,1
sw $25,$fp28-224($sp)
b L.3162
L.3169:
li $25,4
sw $25,$fp28-124($sp)
b L.3162
L.3171:
lw $25,$fp28-120($sp)
sll $25,$25,2
la $24,$fp28-116($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp28-128($sp)
beq $25,$24,L.3162
li $25,4
sw $25,$fp28-124($sp)
L.3161:
L.3162:
L.3158:
lw $25,$fp28-120($sp)
addu $25,$25,1
sw $25,$fp28-120($sp)
lw $25,$fp28-120($sp)
blt $25,22,L.3157
lw $25,$fp28-124($sp)
bne $25,3,L.3174
li $25,16
lw $24,$fp28-224($sp)
subu $2,$25,$24
b L.3135
L.3174:
li $2,-1
L.3135:
lw $30,0($sp)
addu $sp,$sp,232
j $31
.end regi
.globl regp
.text
.align 2; .ent regp
regp:
$fp29=240
.frame $sp,240,$31
addu $sp,$sp,-240
.mask 0x40c00000,-232
sw $22,0($sp)
sw $23,4($sp)
sw $30,8($sp)
la $25,$fp29-216($sp)
sw $25,$fp29-216($sp)
la $24,$fp29-192($sp)
sw $24,$fp29-192($sp)
la $15,$fp29-196($sp)
sw $15,$fp29-196($sp)
la $14,$fp29-200($sp)
sw $14,$fp29-200($sp)
la $14,$fp29-128($sp)
move $30,$14
sw $14,$fp29-128($sp)
la $14,$fp29-132($sp)
move $23,$14
sw $14,$fp29-132($sp)
la $14,$fp29-136($sp)
move $22,$14
sw $14,$fp29-136($sp)
la $14,$fp29-140($sp)
move $8,$14
sw $14,$fp29-140($sp)
la $14,$fp29-144($sp)
move $9,$14
sw $14,$fp29-144($sp)
la $14,$fp29-148($sp)
move $10,$14
sw $14,$fp29-148($sp)
la $14,$fp29-152($sp)
move $11,$14
sw $14,$fp29-152($sp)
la $14,$fp29-156($sp)
move $12,$14
sw $14,$fp29-156($sp)
la $14,$fp29-160($sp)
move $13,$14
sw $14,$fp29-160($sp)
la $14,$fp29-164($sp)
sw $14,$fp29-4($sp)
sw $14,$fp29-164($sp)
la $14,$fp29-168($sp)
sw $14,$fp29-8($sp)
sw $14,$fp29-168($sp)
la $14,$fp29-172($sp)
sw $14,$fp29-12($sp)
sw $14,$fp29-172($sp)
la $14,$fp29-176($sp)
sw $14,$fp29-16($sp)
sw $14,$fp29-176($sp)
la $14,$fp29-180($sp)
sw $14,$fp29-20($sp)
sw $14,$fp29-180($sp)
la $14,$fp29-184($sp)
sw $14,$fp29-24($sp)
sw $14,$fp29-184($sp)
la $14,$fp29-188($sp)
sw $14,$fp29-28($sp)
sw $14,$fp29-188($sp)
la $14,$fp29-204($sp)
sw $14,$fp29-204($sp)
la $14,$fp29-208($sp)
sw $14,$fp29-208($sp)
la $14,$fp29-220($sp)
sw $14,$fp29-220($sp)
subu $25,$24,$25
div $25,$25,4
sw $25,$fp29-116($sp)
move $25,$15
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-112($sp)
la $25,$fp29-200($sp)
la $24,$fp29-196($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-108($sp)
la $25,$fp29-128($sp)
la $24,$fp29-200($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-104($sp)
la $25,$fp29-132($sp)
la $24,$fp29-128($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-100($sp)
la $25,$fp29-136($sp)
la $24,$fp29-132($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-96($sp)
la $25,$fp29-140($sp)
la $24,$fp29-136($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-92($sp)
la $25,$fp29-144($sp)
la $24,$fp29-140($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-88($sp)
la $25,$fp29-148($sp)
la $24,$fp29-144($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-84($sp)
la $25,$fp29-152($sp)
la $24,$fp29-148($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-80($sp)
la $25,$fp29-156($sp)
la $24,$fp29-152($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-76($sp)
la $25,$fp29-160($sp)
la $24,$fp29-156($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-72($sp)
la $25,$fp29-164($sp)
la $24,$fp29-160($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-68($sp)
la $25,$fp29-168($sp)
la $24,$fp29-164($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-64($sp)
la $25,$fp29-172($sp)
la $24,$fp29-168($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-60($sp)
la $25,$fp29-176($sp)
la $24,$fp29-172($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-56($sp)
la $25,$fp29-180($sp)
la $24,$fp29-176($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-52($sp)
la $25,$fp29-184($sp)
la $24,$fp29-180($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-48($sp)
la $25,$fp29-188($sp)
la $24,$fp29-184($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-44($sp)
la $25,$fp29-204($sp)
la $24,$fp29-188($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-40($sp)
la $25,$fp29-208($sp)
la $24,$fp29-204($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-36($sp)
la $25,$fp29-220($sp)
la $24,$fp29-208($sp)
subu $25,$25,$24
div $25,$25,4
sw $25,$fp29-32($sp)
lw $25,$fp29-116($sp)
sw $25,$fp29-212($sp)
li $25,1
sw $25,$fp29-124($sp)
sw $0,$fp29-120($sp)
L.3198:
lw $25,$fp29-124($sp)
beq $25,1,L.3204
beq $25,2,L.3207
beq $25,3,L.3212
b L.3202
L.3204:
lw $25,$fp29-120($sp)
sll $25,$25,2
la $24,$fp29-116($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp29-212($sp)
beq $25,$24,L.3203
lw $25,$fp29-120($sp)
sll $25,$25,2
la $24,$fp29-116($sp)
addu $25,$25,$24
lw $25,($25)
sw $25,$fp29-228($sp)
li $25,2
sw $25,$fp29-124($sp)
li $25,1
sw $25,$fp29-224($sp)
b L.3203
L.3207:
lw $25,$fp29-120($sp)
sll $25,$25,2
la $24,$fp29-116($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp29-212($sp)
bne $25,$24,L.3208
li $25,3
sw $25,$fp29-124($sp)
b L.3203
L.3208:
lw $25,$fp29-120($sp)
sll $25,$25,2
la $24,$fp29-116($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp29-228($sp)
bne $25,$24,L.3210
lw $25,$fp29-224($sp)
addu $25,$25,1
sw $25,$fp29-224($sp)
b L.3203
L.3210:
li $25,4
sw $25,$fp29-124($sp)
b L.3203
L.3212:
lw $25,$fp29-120($sp)
sll $25,$25,2
la $24,$fp29-116($sp)
addu $25,$25,$24
lw $25,($25)
lw $24,$fp29-212($sp)
beq $25,$24,L.3203
li $25,4
sw $25,$fp29-124($sp)
L.3202:
L.3203:
L.3199:
lw $25,$fp29-120($sp)
addu $25,$25,1
sw $25,$fp29-120($sp)
lw $25,$fp29-120($sp)
blt $25,22,L.3198
lw $25,$fp29-124($sp)
bne $25,3,L.3215
li $25,16
lw $24,$fp29-224($sp)
subu $2,$25,$24
b L.3176
L.3215:
li $2,-1
L.3176:
lw $22,0($sp)
lw $23,4($sp)
lw $30,8($sp)
addu $sp,$sp,240
j $31
.end regp
.lcomm L.3218,420
.data; L.3219:.ascii "s84,er%d\012\000"
.data; L.3220:.ascii "s84    \000"
.text
.globl s84
.text
.align 2; .ent s84
s84:
$fp30=208
.frame $sp,208,$31
addu $sp,$sp,-208
.fmask 0xc0000000,-192
s.d $f30,16($sp)
.mask 0xc0ff0000,-148
sw $16,24($sp)
sw $17,28($sp)
sw $18,32($sp)
sw $19,36($sp)
sw $20,40($sp)
sw $21,44($sp)
sw $22,48($sp)
sw $23,52($sp)
sw $30,56($sp)
sw $31,60($sp)
move $30,$4
la $20,L.3220
addu $19,$30,60
move $18,$0
L.3221:
L.3222:
move $25,$19
addu $19,$25,1
move $24,$20
addu $20,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.3221
li $4,3
jal fip  # int
move $25,$2
sw $25,$fp30-140($sp)
lw $25,$fp30-140($sp)
lw $25,($25)
beq $25,3,L.3224
lw $25,44($30)
beq $25,$0,L.3226
la $4,L.3219
li $5,1
jal printf  # int
L.3226:
addu $18,$18,1
L.3224:
la $25,glork
sw $25,$fp30-144($sp)
li $4,4
lw $25,$fp30-144($sp)
jal $25  # int
beq $2,4,L.3228
lw $25,44($30)
beq $25,$0,L.3230
la $4,L.3219
li $5,2
jal printf  # int
L.3230:
addu $18,$18,2
L.3228:
move $22,$0
L.3232:
sll $25,$22,2
la $24,$fp30-68($sp)
addu $25,$25,$24
mtc1 $22,$f18; cvt.d.w $f18,$f18
cvt.s.d $f18,$f18
s.s $f18,($25)
sll $25,$22,2
la $24,$fp30-136($sp)
addu $24,$25,$24
la $15,$fp30-68($sp)
addu $25,$25,$15
sw $25,($24)
L.3233:
addu $22,$22,1
blt $22,17,L.3232
l.s $f30,L.413
move $22,$0
L.3236:
sll $25,$22,2
la $24,$fp30-136($sp)
addu $25,$25,$24
lw $25,($25)
l.s $f18,($25)
add.s $f30,$f30,$f18
L.3237:
addu $22,$22,1
blt $22,17,L.3236
l.s $f18,L.3242
c.eq.s $f30,$f18; bc1t L.3240
lw $25,44($30)
beq $25,$0,L.3243
la $4,L.3219
li $5,4
jal printf  # int
L.3243:
addu $18,$18,4
L.3240:
move $21,$0
L.3245:
move $22,$0
L.3249:
move $23,$0
L.3253:
sll $25,$23,2
li $24,28
mul $24,$24,$22
li $15,140
mul $15,$15,$21
la $15,L.3218($15)
addu $24,$24,$15
addu $25,$25,$24
li $24,35
mul $24,$24,$21
li $15,7
mul $15,$15,$22
addu $24,$24,$15
addu $24,$24,$23
sw $24,($25)
L.3254:
addu $23,$23,1
blt $23,7,L.3253
L.3250:
addu $22,$22,1
blt $22,5,L.3249
L.3246:
addu $21,$21,1
blt $21,3,L.3245
li $21,1
li $22,2
li $23,3
la $4,L.3218
li $5,105
move $6,$0
jal array  # int
move $17,$2
li $25,140
mul $25,$25,$21
la $4,L.3218($25)
li $25,35
move $5,$25
move $6,$25
jal array  # int
move $16,$2
li $25,28
mul $25,$25,$22
li $24,140
mul $24,$24,$21
la $24,L.3218($24)
addu $4,$25,$24
li $5,7
li $6,49
jal array  # int
addu $25,$17,$16
addu $25,$25,$2
sll $24,$23,2
li $15,28
mul $15,$15,$22
li $14,140
mul $14,$14,$21
la $14,L.3218($14)
addu $15,$15,$14
addu $24,$24,$15
lw $24,($24)
addu $25,$25,$24
subu $25,$25,52
beq $25,$0,L.3257
lw $25,44($30)
beq $25,$0,L.3259
la $4,L.3219
li $5,8
jal printf  # int
L.3259:
addu $18,$18,8
L.3257:
move $2,$18
L.3217:
l.d $f30,16($sp)
lw $16,24($sp)
lw $17,28($sp)
lw $18,32($sp)
lw $19,36($sp)
lw $20,40($sp)
lw $21,44($sp)
lw $22,48($sp)
lw $23,52($sp)
lw $30,56($sp)
lw $31,60($sp)
addu $sp,$sp,208
j $31
.end s84
.globl array
.text
.align 2; .ent array
array:
$fp31=0
.frame $sp,0,$31
move $15,$0
b L.3267
L.3264:
sll $25,$15,2
addu $25,$25,$4
lw $25,($25)
addu $24,$15,$6
beq $25,$24,L.3268
li $2,1
b L.3263
L.3268:
L.3265:
addu $15,$15,1
L.3267:
blt $15,$5,L.3264
move $2,$0
L.3263:
j $31
.end array
.lcomm L.3271,4
.text
.globl fip
.text
.align 2; .ent fip
fip:
$fp32=0
.frame $sp,0,$31
sw $4,L.3271
la $2,L.3271
L.3270:
j $31
.end fip
.globl glork
.text
.align 2; .ent glork
glork:
$fp33=0
.frame $sp,0,$31
move $2,$4
L.3272:
j $31
.end glork
.data; L.3274:.ascii "s85,er%d\012\000"
.data; L.3275:.ascii "s85    \000"
.data; .align 2; L.3283:.word L.426
.word L.428
.word L.427
.word L.429
.word L.430
.word L.431
.word L.432
.data; L.3284:.ascii " alignment: \000"
.text
.globl s85
.text
.align 2; .ent s85
s85:
$fp34=432
.frame $sp,432,$31
addu $sp,$sp,-432
.mask 0xc0f00000,-396
sw $20,16($sp)
sw $21,20($sp)
sw $22,24($sp)
sw $23,28($sp)
sw $30,32($sp)
sw $31,36($sp)
move $30,$4
la $22,L.3275
addu $21,$30,60
move $20,$0
L.3288:
L.3289:
move $25,$21
addu $21,$25,1
move $24,$22
addu $22,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.3288
la $25,$fp34-284($sp)
la $24,$fp34-304($sp)
subu $25,$25,$24
ble $25,$0,L.3299
la $25,$fp34-280($sp)
la $24,$fp34-284($sp)
subu $25,$25,$24
ble $25,$0,L.3299
la $25,$fp34-276($sp)
la $24,$fp34-280($sp)
subu $25,$25,$24
bgt $25,$0,L.3291
L.3299:
lw $25,44($30)
beq $25,$0,L.3300
la $4,L.3274
li $5,1
jal printf  # int
L.3300:
addu $20,$20,1
L.3291:
la $25,$fp34-337($sp)
la $24,$fp34-338($sp)
subu $25,$25,$24
sw $25,$fp34-268($sp)
la $25,$fp34-340($sp)
la $24,$fp34-342($sp)
subu $25,$25,$24
sw $25,$fp34-264($sp)
la $25,$fp34-348($sp)
la $24,$fp34-352($sp)
subu $25,$25,$24
sw $25,$fp34-260($sp)
la $25,$fp34-356($sp)
la $24,$fp34-360($sp)
subu $25,$25,$24
sw $25,$fp34-256($sp)
la $25,$fp34-364($sp)
la $24,$fp34-368($sp)
subu $25,$25,$24
sw $25,$fp34-252($sp)
la $25,$fp34-372($sp)
la $24,$fp34-376($sp)
subu $25,$25,$24
sw $25,$fp34-248($sp)
la $25,$fp34-384($sp)
la $24,$fp34-392($sp)
subu $25,$25,$24
sw $25,$fp34-244($sp)
lw $25,40($30)
beq $25,$0,L.3315
move $23,$0
L.3317:
la $4,L.3321
sll $25,$23,2
lw $5,L.3283($25)
la $6,L.3284
la $24,$fp34-268($sp)
addu $25,$25,$24
lw $7,($25)
jal printf  # int
L.3318:
addu $23,$23,1
blt $23,7,L.3317
L.3315:
lw $25,$fp34-272($sp)
or $25,$25,0x1c000000
sw $25,$fp34-272($sp)
lw $25,$fp34-272($sp)
move $24,$25
and $24,$24,0x3fffffff
sll $25,$25,3
sra $25,$25,29
sll $25,$25,30
sra $25,$25,30
sll $25,$25,30
and $25,$25,0xc0000000
or $25,$24,$25
sw $25,$fp34-272($sp)
lw $25,$fp34-272($sp)
move $24,$25
and $24,$24,0xe3ffffff
sra $25,$25,30
sll $25,$25,29
sra $25,$25,29
sll $25,$25,26
and $25,$25,0x1c000000
or $25,$24,$25
sw $25,$fp34-272($sp)
lw $25,$fp34-272($sp)
sll $25,$25,3
sra $25,$25,29
beq $25,3,L.3322
lw $25,$fp34-272($sp)
sll $25,$25,3
sra $25,$25,29
bne $25,-1,L.3324
lw $25,40($30)
beq $25,$0,L.3325
la $4,L.3328
jal printf  # int
b L.3325
L.3324:
lw $25,44($30)
beq $25,$0,L.3329
la $4,L.3274
li $5,2
jal printf  # int
L.3329:
addu $20,$20,2
L.3325:
L.3322:
lw $25,$fp34-272($sp)
or $25,$25,0x2000000
sw $25,$fp34-272($sp)
lw $25,$fp34-272($sp)
sll $25,$25,6
sra $25,$25,31
beq $25,1,L.3331
lw $25,40($30)
beq $25,$0,L.3333
la $4,L.3335
jal printf  # int
L.3333:
L.3331:
la $25,$fp34-240($sp)
subu $25,$25,$25
bne $25,$0,L.3343
bne $25,$0,L.3343
bne $25,$0,L.3343
bne $25,$0,L.3343
bne $25,$0,L.3343
bne $25,$0,L.3343
beq $25,$0,L.3336
L.3343:
lw $25,44($30)
beq $25,$0,L.3344
la $4,L.3274
li $5,4
jal printf  # int
L.3344:
addu $20,$20,4
L.3336:
b L.3346
lw $25,44($30)
beq $25,$0,L.3348
la $4,L.3274
li $5,8
jal printf  # int
L.3348:
addu $20,$20,8
L.3346:
la $25,$fp34-336($sp)
sw $25,$fp34-276($sp)
li $25,2
sb $25,$fp34-336($sp)
lw $25,$fp34-276($sp)
lb $24,($25)
addu $24,$24,1
sb $24,($25)
lb $25,$fp34-336($sp)
beq $25,3,L.3352
lw $25,44($30)
beq $25,$0,L.3354
la $4,L.3274
li $5,16
jal printf  # int
L.3354:
addu $20,$20,16
L.3352:
move $2,$20
L.3273:
lw $20,16($sp)
lw $21,20($sp)
lw $22,24($sp)
lw $23,28($sp)
lw $30,32($sp)
lw $31,36($sp)
addu $sp,$sp,432
j $31
.end s85
.data; L.3357:.ascii "s86,er%d\012\000"
.data; L.3358:.ascii "s86    \000"
.data; .align 2; L.3359:.word 0x1
.word 0x3
.word 0x5
.sdata; .align 2; L.3360:.word L.3359+8
.lcomm L.3362,40
.data; .align 2; L.3363:.word 0x3f800000
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
.data; .align 2; L.3364:.word 0x3f800000
.word 0x40400000
.word 0x40a00000
.word 0x40000000
.word 0x40800000
.word 0x40c00000
.word 0x40400000
.word 0x40a00000
.word 0x40e00000
.space 12
.data; .align 2; L.3365:.word 0x3f800000
.word 0x40400000
.word 0x40a00000
.word 0x40000000
.word 0x40800000
.word 0x40c00000
.word 0x40400000
.word 0x40a00000
.word 0x40e00000
.space 12
.data; .align 2; L.3366:.word 0x3f800000
.space 8
.word 0x40000000
.space 8
.word 0x40400000
.space 8
.word 0x40800000
.space 8
.text
.globl s86
.text
.align 2; .ent s86
s86:
$fp35=56
.frame $sp,56,$31
addu $sp,$sp,-56
.mask 0xc0ff0000,-4
sw $16,16($sp)
sw $17,20($sp)
sw $18,24($sp)
sw $19,28($sp)
sw $20,32($sp)
sw $21,36($sp)
sw $22,40($sp)
sw $23,44($sp)
sw $30,48($sp)
sw $31,52($sp)
sw $4,56($sp)
lw $25,L.3360
addu $16,$25,-4
jal one  # int
sll $25,$2,2
addu $30,$25,$16
la $19,L.3358
lw $25,$fp35+0($sp)
addu $18,$25,60
move $17,$0
L.3367:
L.3368:
move $25,$18
addu $18,$25,1
move $24,$19
addu $19,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.3367
lw $25,L.3360
lw $25,($25)
beq $25,5,L.3370
lw $25,$fp35+0($sp)
lw $25,44($25)
beq $25,$0,L.3372
la $4,L.3357
li $5,1
jal printf  # int
L.3372:
addu $17,$17,1
L.3370:
lw $25,($16)
beq $25,3,L.3374
lw $25,$fp35+0($sp)
lw $25,44($25)
beq $25,$0,L.3376
la $4,L.3357
li $5,2
jal printf  # int
L.3376:
addu $17,$17,2
L.3374:
lw $25,($30)
beq $25,5,L.3378
lw $25,$fp35+0($sp)
lw $25,44($25)
beq $25,$0,L.3380
la $4,L.3357
li $5,4
jal printf  # int
L.3380:
addu $17,$17,4
L.3378:
move $20,$0
move $23,$0
L.3382:
sll $25,$23,2
lw $25,L.3362($25)
beq $25,$0,L.3386
li $20,1
L.3386:
L.3383:
addu $23,$23,1
blt $23,10,L.3382
beq $20,$0,L.3388
lw $25,$fp35+0($sp)
lw $25,44($25)
beq $25,$0,L.3390
la $4,L.3357
li $5,8
jal printf  # int
L.3390:
addu $17,$17,8
L.3388:
move $20,$0
move $22,$0
L.3392:
move $23,$0
L.3396:
li $25,3
mul $25,$25,$22
addu $21,$25,$23
sll $25,$23,2
li $24,12
mul $24,$24,$22
la $15,L.3364($24)
addu $15,$25,$15
l.s $f18,($15)
la $24,L.3365($24)
addu $25,$25,$24
l.s $f16,($25)
c.eq.s $f18,$f16; bc1f L.3402
sll $25,$21,2
l.s $f16,L.3363($25)
c.eq.s $f18,$f16; bc1t L.3400
L.3402:
li $20,1
L.3400:
L.3397:
addu $23,$23,1
blt $23,3,L.3396
L.3393:
addu $22,$22,1
blt $22,4,L.3392
beq $20,$0,L.3403
lw $25,$fp35+0($sp)
lw $25,44($25)
beq $25,$0,L.3405
la $4,L.3357
li $5,16
jal printf  # int
L.3405:
addu $17,$17,16
L.3403:
move $20,$0
move $23,$0
L.3407:
li $25,12
mul $25,$25,$23
l.s $f18,L.3366($25)
addu $25,$23,1
mtc1 $25,$f16; cvt.d.w $f16,$f16
cvt.s.d $f16,$f16
c.eq.s $f18,$f16; bc1t L.3411
li $20,1
L.3411:
L.3408:
addu $23,$23,1
blt $23,4,L.3407
beq $20,$0,L.3413
lw $25,$fp35+0($sp)
lw $25,44($25)
beq $25,$0,L.3415
la $4,L.3357
li $5,32
jal printf  # int
L.3415:
addu $17,$17,32
L.3413:
move $2,$17
L.3356:
lw $16,16($sp)
lw $17,20($sp)
lw $18,24($sp)
lw $19,28($sp)
lw $20,32($sp)
lw $21,36($sp)
lw $22,40($sp)
lw $23,44($sp)
lw $30,48($sp)
lw $31,52($sp)
addu $sp,$sp,56
j $31
.end s86
.globl one
.text
.align 2; .ent one
one:
$fp36=0
.frame $sp,0,$31
li $2,1
L.3417:
j $31
.end one
.data; L.3419:.ascii "s88,er%d\012\000"
.data; L.3420:.ascii "s88    \000"
.text
.globl s88
.text
.align 2; .ent s88
s88:
$fp37=56
.frame $sp,56,$31
addu $sp,$sp,-56
.mask 0xc0e00000,-24
sw $21,16($sp)
sw $22,20($sp)
sw $23,24($sp)
sw $30,28($sp)
sw $31,32($sp)
sw $4,56($sp)
la $30,L.3420
lw $25,$fp37+0($sp)
addu $23,$25,60
move $22,$0
L.3422:
L.3423:
move $25,$23
addu $23,$25,1
move $24,$30
addu $30,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.3422
b L.3425
lw $25,$fp37+0($sp)
lw $25,44($25)
beq $25,$0,L.3427
la $4,L.3419
li $5,1
jal printf  # int
L.3427:
addu $22,$22,1
L.3425:
la $25,$fp37-20($sp)
sw $25,metricp
li $25,2
sw $25,$fp37-20($sp)
lw $25,metricp
li $24,3
sw $24,($25)
lw $25,$fp37-20($sp)
beq $25,3,L.3429
lw $25,$fp37+0($sp)
lw $25,44($25)
beq $25,$0,L.3431
la $4,L.3419
li $5,2
jal printf  # int
L.3431:
addu $22,$22,2
L.3429:
l.d $f18,L.420
s.d $f18,$fp37-16($sp)
s.d $f18,$fp37-8($sp)
la $21,$fp37-16($sp)
l.d $f18,L.419
s.d $f18,($21)
l.d $f18,L.419
s.d $f18,8($21)
l.d $f18,$fp37-16($sp)
l.d $f16,$fp37-8($sp)
add.d $f18,$f18,$f16
l.d $f16,L.417
c.eq.d $f18,$f16; bc1t L.3434
lw $25,$fp37+0($sp)
lw $25,44($25)
beq $25,$0,L.3437
la $4,L.3419
li $5,4
jal printf  # int
L.3437:
addu $22,$22,4
L.3434:
move $2,$22
L.3418:
lw $21,16($sp)
lw $22,20($sp)
lw $23,24($sp)
lw $30,28($sp)
lw $31,32($sp)
addu $sp,$sp,56
j $31
.end s88
.data; L.3440:.ascii "s9,er%d\012\000"
.data; L.3441:.ascii "s9     \000"
.text
.globl s9
.text
.align 2; .ent s9
s9:
$fp38=56
.frame $sp,56,$31
addu $sp,$sp,-56
.mask 0xc0ff0000,-4
sw $16,16($sp)
sw $17,20($sp)
sw $18,24($sp)
sw $19,28($sp)
sw $20,32($sp)
sw $21,36($sp)
sw $22,40($sp)
sw $23,44($sp)
sw $30,48($sp)
sw $31,52($sp)
sw $4,56($sp)
la $22,L.3441
lw $25,$fp38+0($sp)
addu $21,$25,60
move $20,$0
L.3442:
L.3443:
move $25,$21
addu $21,$25,1
move $24,$22
addu $22,$24,1
lb $24,($24)
sb $24,($25)
sll $25,$24,24; sra $25,$25,24
bne $25,$0,L.3442
move $23,$0
move $30,$0
L.3445:
li $25,2
move $19,$25
move $18,$25
li $25,3
move $17,$25
move $16,$25
bne $16,3,L.3451
beq $17,3,L.3449
L.3451:
li $23,1
L.3449:
bne $18,2,L.3454
beq $19,2,L.3452
L.3454:
li $23,1
L.3452:
L.3446:
addu $30,$30,1
blt $30,2,L.3445
beq $23,$0,L.3459
lw $25,$fp38+0($sp)
lw $25,44($25)
beq $25,$0,L.3457
la $4,L.3440
li $5,1
jal printf  # int
L.3457:
addu $20,$20,1
b L.3459
lw $25,$fp38+0($sp)
lw $25,44($25)
beq $25,$0,L.3460
la $4,L.3440
li $5,2
jal printf  # int
L.3460:
addu $20,$20,2
L.3459:
move $2,$20
L.3439:
lw $16,16($sp)
lw $17,20($sp)
lw $18,24($sp)
lw $19,28($sp)
lw $20,32($sp)
lw $21,36($sp)
lw $22,40($sp)
lw $23,44($sp)
lw $30,48($sp)
lw $31,52($sp)
addu $sp,$sp,56
j $31
.end s9
.globl setev
.text
.align 2; .ent setev
setev:
$fp39=0
.frame $sp,0,$31
li $25,1066
sw $25,extvar
L.3462:
j $31
.end setev
.globl rfs
.comm rfs,8
.globl crc
.comm crc,4
.globl rrc
.comm rrc,4
.globl flgl
.comm flgl,4
.globl flgd
.comm flgd,4
.globl flgm
.comm flgm,4
.globl flgs
.comm flgs,4
.globl dprec
.comm dprec,4
.globl fprec
.comm fprec,4
.globl dbits
.comm dbits,4
.globl fbits
.comm fbits,4
.globl ubits
.comm ubits,4
.globl lbits
.comm lbits,4
.globl metricp
.comm metricp,4
.globl extvar
.comm extvar,4
.rdata
L.3335:.ascii "Be especially careful with 1-bit fields!\012\000"
L.3328:.ascii "Sign extension in fields\012\000"
L.3321:.ascii "%s%s%d\012\000"
.align 2; L.3242:.word 0x43080000
L.3089:.ascii "pointer\000"
L.2539:.ascii "Increasing array elements assigned to decreasing locations\012\000"
.align 2; L.2417:.word 0x41d00000
.align 3; L.2416:.word 0x403a0000
.word 0x0
.align 3; L.1714:.word 0x40040000
.word 0x0
.align 3; L.1530:.word 0x40240000
.word 0x0
.align 2; L.1498:.word 0x41200000
.align 3; L.1314:.word 0x40080000
.word 0x0
.align 2; L.1282:.word 0x40400000
.align 3; L.1098:.word 0x401c0000
.word 0x0
.align 2; L.1066:.word 0x40e00000
.align 3; L.880:.word 0x40140000
.word 0x0
.align 2; L.848:.word 0x40a00000
.align 3; L.836:.word 0x41e00000
.word 0x0
L.661:.ascii "queep\000"
L.647:.byte 10,0
L.646:.byte 37,100,0
L.641:.ascii "   key=\000"
.align 3; L.601:.word 0x41f00000
.word 0x0
.align 3; L.538:.word 0x40ce8480
.word 0x0
.align 3; L.537:.word 0x405f4000
.word 0x0
.align 2; L.536:.word 0x42fa0000
.align 2; L.531:.word 0x40000000
L.432:.ascii "double\000"
L.431:.ascii "float\000"
L.430:.ascii "unsigned\000"
L.429:.ascii "long\000"
L.428:.ascii "short\000"
L.427:.ascii "int\000"
L.426:.ascii "char\000"
.align 3; L.420:.word 0x0
.word 0x0
.align 3; L.419:.word 0x3ff00000
.word 0x0
.align 3; L.418:.word 0x40100000
.word 0x0
.align 3; L.417:.word 0x40000000
.word 0x0
.align 2; L.413:.word 0x0
.align 2; L.412:.word 0x3f800000
L.391:.ascii "queep!\000"
L.380:.byte 10,9,8,13,12,92,39,0
L.376:.byte 46,34,46,0
L.363:.ascii "...\000"
.align 3; L.334:.word 0x40938800
.word 0x0
L.199:.ascii "   different results when assigned to longs.\012\000"
L.198:.ascii "Decimal and octal/hex constants sometimes give\012\000"
L.27:.ascii "\012Failed.\012\000"
L.26:.ascii "\012No errors detected.\012\000"
L.20:.ascii "Section %s returned %d.\012\000"
