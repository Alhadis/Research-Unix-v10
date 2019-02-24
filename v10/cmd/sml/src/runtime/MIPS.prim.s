/* Copyright 1989 by AT&T Bell Laboratories */
#include "tags.h"
#include "prof.h"
#include "prim.h"
#define stdarg 2
#define stdcont 3
#define stdclos 4
#define storeptr 22
#define dataptr 23
#define exnptr 30
#define artemp1 24
#define artemp2 25
#define artemp3 20
#define ptrtemp 21
#define String(handle,len,str) .align 2;\
			       .set noreorder;\
			       .word len*power_tags+tag_string;\
			       handle: .ascii str;\
			       .set reorder
#define Closure(name) .align	2;\
		      .set noreorder;\
		      .word	mak_desc(1,tag_record);\
		      name:	.word 9f; /* address of routine */ \
		      .word	1; /* here for historical reasons */\
		      .word	tag_backptr;\
		      .set reorder;\
		      9:
	.data
bottom: .word 0			/* C's saved stack pointer */
saved_pc_diff:	.word 0


	.text

	.globl	runvec
	.align	2
        .word	mak_desc(8,tag_record)
runvec:
	.word	array_v
        .word	callc_v
	.word	create_b_v
	.word	create_s_v
	.word	floor_v
	.word	logb_v
	.word	scalb_v
	.word	syscall_v

Closure(array_v)
	lw $artemp1,0($stdarg)		/* tagged length in $artemp1 */
	lw $10,4($stdarg)		/* get initial value in $10 */
	sra $artemp1,1			/* drop the tag bit */
	sll $artemp2,$artemp1,width_tags /* length for descr into $artemp2 */
	ori $artemp2,tag_array		/* complete descriptor into $artemp2 */
	sll $artemp1,2			/* get length in bytes into $artemp1 */
.set noreorder	/* can't reorder because collection might occur */
	add $artemp3,$artemp1,$dataptr	/* $artemp3 points to last word 
							of new array*/
badgc1:	sw $0,($artemp3)			/* clear; causes allocation */
.set reorder  /* can rearrange instructions again */
	sw $artemp2,0($dataptr)		/* store the descriptor */
	add $dataptr,4			/* points to new object */
	add $artemp3,$artemp1,$dataptr	/* beyond last word of new array*/
	add $stdarg,$dataptr,$0		/* put ptr in return register
					(return val = arg of continuation) */
		b 2f
        1:	sw $10,0($dataptr)		/* store the value */
        	addi $dataptr,4			/* on to the next word */
        2:	bne $dataptr,$artemp3,1b	/* if not off the end, repeat */
        

	lw $10,0($stdcont)		/* grab continuation */
	j $10				/* return */

Closure(create_b_v)
	addi $artemp3,$0,tag_bytearray	/* tag into $artemp3 */
	b	2f
Closure(create_s_v)
	addi $artemp3,$0,tag_string	/* tag into $artemp3 */
2:	
	addi	$artemp1,$stdarg,13	/* $2n+14$ */
	sra	$artemp1,3		/* number of words in string+tag */
	sll	$artemp1,2 		/* # of bytes allocated for str+tag */
.set noreorder /* don't cross gc boundary */
	add 	$artemp2,$artemp1,$dataptr /* beyond last word of string */
badgc2:	sw $0,-4($artemp2)		/* clear last; causes allocation */
.set reorder
	sra	$artemp2,$stdarg,1	/* untagged length in bytes */
	sll	$artemp2,width_tags	/* room for descriptor */
	or	$artemp2,$artemp3 	/* descriptor */
	sw	$artemp2,0($dataptr)	/* store descriptor */
	addi	$stdarg,$dataptr,4	/* pointer to new string */
	add	$dataptr,$artemp1	/* advance; save 1 instruction */
	lw $10,0($stdcont)		/* grab continuation */
	j $10				/* return */

	.globl saveregs
	.globl handle_c
	.globl return_c
	.globl restoreregs
.ent restoreregs
restoreregs:
#define regspace 44
#define localspace 4
#define argbuild 16
#define framesize (regspace+localspace+argbuild) /* must be multiple of 8 */
#define frameoffset (0-localspace)
	subu $sp,framesize
.mask 0xd0ff0000,0-localspace
	sw	$31,argbuild+40($sp)
	sw	$30,argbuild+36($sp)
	sw	$gp,argbuild+32($sp)
        sw      $23,argbuild+28($sp)
        sw      $22,argbuild+24($sp)
        sw      $21,argbuild+20($sp)
        sw      $20,argbuild+16($sp)
        sw      $19,argbuild+12($sp)
        sw      $18,argbuild+8($sp)
        sw      $17,argbuild+4($sp)
        sw      $16,argbuild($sp)

.set noat
	cfc1 $at,$31		/* grab fpa control register */
	ori  $at,$at,0x600	/* set O and Z bits */
	ctc1 $at,$31		/* return fpa control register */
.set at

	sw	$sp,bottom	/* save C's stack pointer */
    lw $artemp1,saved_pc
    	la $artemp2,badgc1
    	beq $artemp1,$artemp2,badpc
    	la $artemp2,badgc2
    	beq $artemp1,$artemp2,badpc

    b 1f
badpc:
    subu $artemp1,4		/* adjust */
    sw $artemp1,saved_pc	/* save */
1:


	/* the big three: argument, continuation, closure */
	lw	$stdarg,saved_ptrs
	lw	$stdcont,saved_ptrs+4
	lw	$stdclos,saved_ptrs+8
	
	/* All the miscellaneous guys */
        lw      $5,saved_ptrs+12
        lw      $6,saved_ptrs+16
        lw      $7,saved_ptrs+20
        lw      $8,saved_ptrs+24
        lw      $9,saved_ptrs+28
        lw      $10,saved_ptrs+32
        lw      $11,saved_ptrs+36
        lw      $12,saved_ptrs+40
        lw      $13,saved_ptrs+44
        lw      $14,saved_ptrs+48
        lw      $15,saved_ptrs+52
        lw      $16,saved_ptrs+56
        lw      $17,saved_ptrs+60
        lw      $18,saved_ptrs+64
        lw      $19,saved_ptrs+68

	lw 	$21, saved_ptrs+72

	lw $artemp1,saved_pc
	lw $31,saved_pc_diff
	addu $31,$artemp1		/* mustn't overflow */
	lw	$artemp1,saved_nonptrs
	lw	$artemp2,saved_nonptrs+4
	lw	$artemp3,saved_nonptrs+8

	/* don't touch registers $26 and $27 */

	lw	$storeptr,saved_storeptr
	lw	$dataptr,saved_dataptr
	lw	$exnptr,saved_exnptr

.set noat /* This trick will cause a warning, but the code is OK */
	lw	$at,saved_pc	/* grab the saved program counter */
	j	$at		/* and continue executing at that spot */
.set at

Closure(handle_c) /* exception handler for ML functions called from C */
	li	$artemp1,CAUSE_EXN
	sw	$artemp1,cause
	b	saveregs
Closure(return_c) /* continuation for ML functions called from C */
	li	$artemp1,CAUSE_RET
	sw	$artemp1,cause
saveregs: 
					/* needn't save $1 */
	/* the big three: argument, continuation, closure */
	sw	$stdarg,saved_ptrs
	sw	$stdcont,saved_ptrs+4
	sw	$stdclos,saved_ptrs+8
	
	/* All the miscellaneous guys */
        sw      $5,saved_ptrs+12
        sw      $6,saved_ptrs+16
        sw      $7,saved_ptrs+20
        sw      $8,saved_ptrs+24
        sw      $9,saved_ptrs+28
        sw      $10,saved_ptrs+32
        sw      $11,saved_ptrs+36
        sw      $12,saved_ptrs+40
        sw      $13,saved_ptrs+44
        sw      $14,saved_ptrs+48
        sw      $15,saved_ptrs+52
        sw      $16,saved_ptrs+56
        sw      $17,saved_ptrs+60
        sw      $18,saved_ptrs+64
        sw      $19,saved_ptrs+68

	sw 	$21, saved_ptrs+72

	sw	$artemp1,saved_nonptrs
	sw	$artemp2,saved_nonptrs+4
	sw	$artemp3,saved_nonptrs+8

	/* don't touch registers $26 and $27 */

	sw	$storeptr,saved_storeptr
	sw	$dataptr,saved_dataptr
	sw	$exnptr,saved_exnptr

	lw $artemp1,saved_pc
	subu $artemp1,$31,$artemp1	/* mustn't overflow */
	sw $artemp1,saved_pc_diff


	lw	$sp,bottom	/* recover C's stack pointer */
	lw	$31,argbuild+40($sp)
	lw	$30,argbuild+36($sp)
	lw	$gp,argbuild+32($sp)
        lw      $23,argbuild+28($sp)
        lw      $22,argbuild+24($sp)
        lw      $21,argbuild+20($sp)
        lw      $20,argbuild+16($sp)
        lw      $19,argbuild+12($sp)
        lw      $18,argbuild+8($sp)
        lw      $17,argbuild+4($sp)
        lw      $16,argbuild($sp)


	addu $sp,framesize
	j	$31	/* return to C program */
.end restoreregs

Closure(callc_v)
	sw $stdcont,argbuild+regspace($sp) /* save continuation on stack */
	lw $4,4($stdarg)	/* get value a into arg register */
	lw $10,0($stdarg)	/* get address of f into misc reg */
	jal $10			/* call f ($31 can be trashed) */
	move $stdarg,$2		/* return val is argument to continuation */
	lw $stdcont,argbuild+regspace($sp) /* recover continuation */
	move $stdclos,$0
	move $5,$0
	move $6,$0
	move $7,$0
	move $8,$0
	move $9,$0
	move $10,$0
	move $11,$0
	move $12,$0
	move $13,$0
	move $14,$0
	move $15,$0
	/* $16--$23 and $30 are saved by the callee */

	lw $artemp3,cause	/* get cause */
        bne $artemp3,$0,saveregs /* if cause != 0, save ML & return to C */
	lw $10,0($stdcont)	/* grab continuation */
	j $10			/* return */
Closure(syscall_v)
	sw $stdcont,argbuild+regspace($sp) /* save continuation on stack */
	lw $artemp1,8($stdarg)	/* 2*argc+1 in $artemp1 */
	sra $artemp1,1 		/* argc in $artemp1 */
	move $16,$sp		/* save our $sp */
	ble $artemp1,4,1f		/* big enough */
	sub $artemp2,$artemp1,3		/* (temp2 = argc - 4 + 1) > 1 */
	sra $artemp2,1			
	sll $artemp2,3			/* temp2 = 4 * roundup (argc-4,2) */
	subu $sp,$artemp2		/* increase stack */
1:

	lw $ptrtemp,4($stdarg)	/* argv in $ptrtemp */
	move $artemp2,$sp		/* destination in $artemp2 */
	b 1f				/* branch forward to test */
2:	/* argc > 0 */
	lw $artemp3,0($ptrtemp)		/* get list element */
	andi $10,$artemp3,1		/* tagged? */
	beqz $10,3f
	sra $artemp3,1			/* drop tag bit */
3:	sw $artemp3,0($artemp2) 	/* save the argument */
	lw $ptrtemp,4($ptrtemp)		/* next element */
	add $artemp2,4			/* next arg build area */
	sub $artemp1,1			/* --argc */
1:	bgtz $artemp1,2b		/* if argc>0, store another */

	lw $4,0($sp)
	lw $5,4($sp)
	lw $6,8($sp)
	lw $7,12($sp)

9:	lw $2,0($stdarg)	/* get syscall # in $2; trash $stdarg */
	sra $2,1		/* throw out the tag bit */
	syscall
	move $sp,$16		/* recover the good stack pointer */
	lw $stdcont,argbuild+regspace($sp) /* recover continuation */
	bnez $7,1f		/* if error, return ~1 */
	move $stdarg,$2		/* return val is argument to continuation */
	add $stdarg,$stdarg	/* double return value */
	addi $stdarg,1		/* and add tag bit */
	b 2f
1:	sw  $stdarg,errno
	li $stdarg,-1
2:
	move $stdclos,$0
	move $5,$0
	move $6,$0
	move $7,$0
	move $8,$0
	move $9,$0
	move $10,$0
	move $11,$0
	move $12,$0
	move $13,$0
	move $14,$0
	move $15,$0
	/* $16--$23 and $30 are saved by the callee */

	lw $10,0($stdcont)	/* grab continuation */
	j $10			/* return */

/* Floating exceptions raised (assuming ROP's are never passed to functions):
 *	DIVIDE BY ZERO - (div)
 *	OVERFLOW/UNDERFLOW - (add,div,sub,mul) as appropriate
 *
 * floor raises integer overflow if the float is out of 32-bit range,
 * so the float is tested before conversion, to make sure it is in (31-bit)
 * range */

Closure(floor_v)
	lwc1 $f4,0($stdarg)		/* get least significant word */
	lwc1 $f5,4($stdarg)		/* get most significant word */
.set noat
	cfc1 $at,$31		/* grab fpa control register */
	ori  $at,0x03		/* set rounding bits to 11 */
	ctc1 $at,$31		/* return fpa control register */
.set at
	cvt.w.d $f6,$f4			/* convert to integer */
.set noat
	cfc1 $at,$31		/* grab fpa control register */
	ori  $at,0x03		/* set rounding bits to 11 */
	xori $at,0x03		/* set rounding bits to 00
	ctc1 $at,$31		/* return fpa control register */
.set at
	mfc1 $stdarg,$f6		/* get in std argument register */
	sll $stdarg,1		/* make room for tag bit */
	add $stdarg,1		/* add the tag bit */
	lw $10,0($stdcont)	/* grab continuation */
	j $10			/* return */


Closure(logb_v)
	lw 	$stdarg,4($stdarg)	/* most significant part */
	srl 	$stdarg,20		/* throw out 20 low bits */
	andi	$stdarg,0x07ff		/* clear all but 11 low bits */
	sub 	$stdarg,1023		/* subtract 1023 */
	sll 	$stdarg,1		/* make room for tag bit */
	add	$stdarg,1		/* add the tag bit */
	lw 	$10,0($stdcont)		/* grab continuation */
	j 	$10			/* return */

Closure(scalb_v)
	lw 	$artemp1,4($stdarg)	/* get tagged n */
	sra	$artemp1,1		/* get real n */
	beqz	$artemp1,9f		/* if zero, return the old float */
	lw	$ptrtemp,0($stdarg)	/* get pointer to float */
	lw 	$artemp2,4($ptrtemp)	/* most significant part */
	srl 	$artemp2,20		/* throw out 20 low bits */
	andi	$artemp2,0x07ff  	/* clear all but 11 low bits */
	add	$artemp3,$artemp2,$artemp1	/* new := old + n */
	blt	$artemp3,1,under	/* punt if underflow */
	bgt	$artemp3,2046,over	/* or overflow */
	xor	$artemp3,$artemp2	/* at3 = new xor old */
	sll	$artemp3,20		/* put exponent in right position */
	lw	$artemp2,4($ptrtemp)	/* most significant word */
	xor	$artemp2,$artemp3	/* change to new exponent */
.set noreorder
	sw	$artemp2,8($dataptr)	/* allocate; may cause gc */
.set reorder
	lw 	$artemp2,0($ptrtemp)	/* get least significant word */
	li	$10,mak_desc(8,tag_string) /* make descriptor */
	sw	$artemp2,4($dataptr)	/* save lsw */
	sw	$10,0($dataptr)		/* save descriptor */
	add	$stdarg,$dataptr,4	/* get pointer to new float */
	add	$dataptr,12		/* point to new free word */
	lw 	$10,0($stdcont)		/* grab continuation */
	j 	$10			/* return */

9:	lw	$stdarg,0($stdarg)	/* get old float */
	lw 	$10,0($stdcont)		/* grab continuation */
	j 	$10			/* return */

over:	la	$stdarg,1f		/* exception name in $stdarg */
	b 	raise_real
String(1,8,"overflow")
under:	la	$stdarg,1f		/* exception name in $stdarg */
	b 	raise_real
String(1,9,"underflow\0\0\0")

raise_real:
 /* build new record to pass to exception handler */
 /*    [descriptor]
 /*    [exception (string)]
 /*    [real_e (more exception info)]
  */
	la	$10,real_e		/* get address of real_e */
.set noreorder
	sw	$10,8($dataptr)		/* allocate; may cause gc */
.set reorder
	sw	$stdarg,4($dataptr)
	li	$10,mak_desc(2,tag_record)
	sw	$10,0($dataptr)
	add	$stdarg,$dataptr,4	/* new record is argument */
	addi	$dataptr,12		/* $dataptr restored */
	move	$stdclos,$exnptr	/* make sure closure is right */
	lw 	$10,0($exnptr)		/* grab handler */
	j 	$10			/* raise the exception */

/* this bogosity is for export.c */
	.globl	startptr
startptr: .word    __start	/* just a guess... */


