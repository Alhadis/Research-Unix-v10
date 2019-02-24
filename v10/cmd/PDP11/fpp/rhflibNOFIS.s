.text
.globl fad,fsb,fdv,fmp,int,float /
	/each of these expects its args on the stack, and returns its
	/answer on the stack. eg.,
	/	mov alow,-(sp)
	/	mov ahi,-(sp)
	/	jsr pc,int
	/	mov (sp)+,iahi
	/	mov (sp)+,ialow
	/ &&
	/	mov alow,-(sp)
	/	mov ahi,-(sp)
	/	mov blow,-(sp)
	/	mov bhi,-(sp)
	/	jsr pc,fdv
	/	mov (sp)+,a/b.hi
	/	mov (sp)+,a/b.low
	/
	/	"hi" means "most significant bits"
	/
	/ MJM backwards compiled RH octal code
	/ using original NORGEN listing from RH
	/
saver:
	mov r1,-(sp)
	mov r2,-(sp)
	mov r3,-(sp)
	mov r4,-(sp)
	mov r5,-(sp)
	mov 10.(sp),-(sp)
	rts r0
rest:
	mov (sp)+,r0
	mov (sp)+,r5
	mov (sp)+,r4
	mov (sp)+,r3
	mov (sp)+,r2
	mov (sp)+,r1
	rts r0
/// ***
///
/ ddiv,ddivl;	ddivl has no fancy checks
/ double (actualy any size) divide
/ world's slowest method (shift and subtract)
/ calling seq:
/	mov numerator low part,-(sp)
/	...
/	mov numerator high part,-(sp)
/	mov number of numerator words,-(sp)
/	mov denom low part,-(sp)
/	...
/	mov denom high part,-(sp)
/	mov number of denom words,-(sp)
/	jsr pc,ddiv
/
/ on return the quotient has overwritten the
/ numerator (and is same size)
/ and the remainder has overwritten the denom
/ (and is same size).  remainder sign is same as
/ numerator.  so that n-r=q*d.
/ to bump remainder from stack user can
/   asl (sp)
/   add (sp)+,sp
/ or similarly then for quotient
/
ddiv:
	cmp 2(sp),$2	/ can we do quickie divide?
	bhi ugh
	blt ugh3	/ maybe
	tst 4(sp)
	bne ugh
	tst 6(sp)
	bmi ugh
	cmp 8.(sp),$2
	bne ugh
	mov r0,-(sp)
	mov r1,-(sp)
	mov 14.(sp),r0
	mov 16.(sp),r1
	div 10.(sp),r0
	bvs ugh2
	blo ugh2
	mov r0,16.(sp)
	clr 14.(sp)
	mov r1,10.(sp)
	mov (sp)+,r1
	mov (sp)+,r0
	rts pc
/
ugh3:
	cmp 6(sp),$2
	bne ugh
	tst 4(sp)
	bmi ugh
	mov r0,-(sp)
	mov r1,-(sp)
	mov 12.(sp),r0
	mov 14.(sp),r1
	div 8.(sp),r0
	bvs ugh2
	blo ugh2
	mov r0,14.(sp)
	clr 12.(sp)
	mov r1,8.(sp)
	mov (sp)+,r1
	mov (sp)+,r0
	rts pc
/
ugh2:
	mov (sp)+,r1
	mov (sp)+,r0
/
ddivl:
ugh:
	jsr r0,saver
	mov sp,r5
	add $14.,r5	/ point to wd (# words in denom)
	mov (r5)+,r4	/ wd to r4
	add r4,r5
	add r4,r5	/ bump r5 to wn (# wrds in num)
	clr -(sp)	/ copy denom and zero to stack
	mov r4,r0	/ set to copy d
1:
	mov -(r5),-(sp)
	sob r0,1b
	bpl 2f	/ if denom was neg we must negate it
	mov r4,r0	/ wd to r0
	mov sp,r2	/ addr of first wd to r2
	jsr pc,ns	/ negate subroutine
2:
	mov r4,-(sp)	/ store wd
	add r4,r5
	add r4,r5	/ point r5 to callers wn
	mov sp,r4	/ r4 will stay pointed to my wd
	mov (r5)+,r3	/ wn to r3
	add r3,r5
	add r3,r5	/ r5 just past numerator
	mov r3,r0	/ wn to r0
6:
	mov -(r5),-(sp)	/ copy num to my stack
	sob r0,6b
	bpl 7f	/ if negative we must negate it
	mov r3,r0	/ wn
	mov sp,r2	/ addr of first word of num
	jsr pc,ns	/ negate subroutine
7:
	add (r4),r3	/ wn+wd to r3
	mov (r4),r0	/ wd to r0
8:
	clr -(sp)	/ prefix wd zeroes on num
	sob r0,8b
	mov r3,-(sp)	/ store wd+wn ahead of all this
	mov sp,r3	/ denom fits in at r3+2
	mov -2(r5),-(sp)	/ wn is iteration counter
	mov $100000,-(sp)	/ quotient bit finder
	mov (r4),r0	/ shift denom right one bit
	mov r4,r1	/ code same as below
	tst (r1)+
t20:
	ror (r1)+
	sob r0,t20
	ror (r1)
/
/ master shift and subtract loop
/
9:
	clc	/ shift quotient bit right one
	ror (sp)	/ bit in question this iteration
	bhis t11	/ bcc	/ its still in word; no extra work.
		/ bit was shifted out, we need to bump everything
	ror (sp)	/ get our bit back
	dec 2(sp)	/ iteration counter
	ble t12	/ done if zero
	mov (r4),r0	/ wd
	mov r4,r1	/ addr of denom minus 2
	tst (r1)+
t13:
	mov 2(r1),(r1)+	/ copy denom back one word
	sob r0,t13
	clr (r1)	/ clear extra word
	cmp (r3)+,(r5)+	/ bump r3 and r5 up
t11:
	bic (sp),(r5)	/ assume we won't subtract
	mov (r4),r0	/ shift d one bit right
	mov r4,r1
	tst (r1)+	/ to first word of d
t10:
	ror (r1)+	/ shift.  tst clears carry
	sob r0,t10
	ror (r1)	/ extra word at end gets bits
	mov (r4),r0	/ wd
	inc r0	/ plus one
	mov r3,r1	/ addr in numerator
	mov r4,r2	/ addr in denom
	cmp (r2)+,(r1)+	/ bump both
t14:
	cmp (r1)+,(r2)+	/ cmp num to denom
	bne t15
	sob r0,t14
	br t21	/ equal, do subtract
t15:
	blo 9b	/ no subtract if num less
	dec r0	/ subtract off residual word count
	asl r0	/ to wrds
	add r0,r1
	add r0,r2
t21:
	mov (r4),r0
	bis (sp),(r5)	/ we were wrong.. set the bit
	mov r4,-(sp)	/ save r4
t16:
	sub -(r2),-(r1)	/ sub denom from num
	mov r1,r4
t22:
	sbc -(r4)
	blo t22	/ bcs
	sob r0,t16
	sub -(r2),-(r1)
	mov (sp)+,r4	/ restore r4
	br 9b
t12:
	cmp (sp)+,(sp)+	/ bump sp to wd+wn
	mov (sp),r0	/ wd+wn to r0
	sub (r4),r0	/ wn to r0
	sub r0,r5
	sub r0,r5	/ r5 now points to callers wn
	mov r5,r3
	tst (r5)+	/ now to numerator
	sub (r4),r3
	sub (r4),r3	/ r3 points to first wd of callers denom
	mov (r5),r2	/ sign of numerator to r2
	bpl t18	/ remainder if ok (positive)
	mov r4,r2	/ change sign of remainder via ns
	mov (r4),r0	/ wd to r0
	sub r0,r2
	sub r0,r2	/ r2 points at remainder
	jsr pc,ns	/ negate it
	mov (r5),r2	/ sign of numerator
	bic $100000,(r5)
t18:
	xor r2,(r3)	/ are signs of n and d same
	bpl t17	/ yes quotient is ok (positive)
	mov -2(r5),r0	/ wn to r0
	mov r5,r2
	jsr pc,ns	/ negate quotient
t17:
	mov r4,r2	/ now copy remainder to callers denom
	mov (r4),r0	/ wd to r0
	tst -(r5)	/ r5 pts to callers wn
t19:
	mov -(r2),-(r5)	/ copy remainder to denom
	sob r0,t19
	sub $16.,r5	/ r5 pts to save area
	mov r5,sp
	jsr pc,rest
	rts pc
ns:			/ negate subroutine
	mov r0,r1
4:
	com (r2)+
	sob r0,4b
5:
	adc -(r2)
	sob r1,5b
	rts pc
/// ***
///
/
/ convert number on stack from flt to int
/
int:
	mov r0,-(sp)
	mov r1,-(sp)
	mov r2,-(sp)
	clr -(sp)	/ sign of input number
	mov 12.(sp),r1	/ low part
	mov 10.(sp),r0	/ hi part
	bpl 1f	/ ok
	mov $100000,(sp)	/ set sign -
	bic (sp),r0	/ set number plus
1:
	mov r0,r2	/ extract exponent
	bic $100177,r2
	bic r2,r0
	ash $-7,r2	/ make it an integer
	bis $200,r0	/ set hidden bit
	sub $200,r2	/ subtr off excess 200
	ble 2f	/ zero, whole thing zero.
	sub $24.,r2	/ set for shift
	ashc r2,r0	/ left or right
4:
	tst (sp)+	/ sign
	bpl 3f
	neg r0
	neg r1
	sbc r0
3:
	mov r0,8.(sp)
	mov r1,10.(sp)
	mov (sp)+,r2
	mov (sp)+,r1
	mov (sp)+,r0
	rts pc
2:
	clr r0
	clr r1
	br 4b
/
/ float an integer on stack
/
float:
	mov 4(sp),-(sp)
	mov 4(sp),-(sp)
	bmi negflt
	clr -(sp)
	mov $[200+24.]*200,-(sp)	/ mov $46000,-(sp)
1:
	bit $377*200,4(sp)	/ bit $77600,4(sp)
	beq 2f
	asr 4(sp)
	ror 6(sp)
	add $200,(sp)
	br 1b
2:
	bis (sp),4(sp)
	jsr pc,fsb		// fsbx. (assuming hdwf=0; else 075016)
	mov (sp)+,4(sp)
	mov (sp)+,4(sp)
	rts pc
negflt:
	neg (sp)
	neg 2(sp)
	sbc (sp)
	bmi 3f	/ overflow. avoid recursion to 0
	jsr pc,float
	bis $100000,(sp)
	mov (sp)+,4(sp)
	mov (sp)+,4(sp)
	rts pc
3:
	cmp (sp)+,(sp)+	/ toss
	mov $[200+32.]*200+100000,2(sp)	/ mov $150000,2(sp) / -2**-31
	clr 4(sp)
	rts pc
/// ***
///
/ 32 bit and sub mul neg
/
/ jsr pd,dmul
/ input is 2 32-bit no's on stack
/ output is 64-bit product on stack
/	mov n low,-(sp)
/	mov n high,-(sp)
/	mov m low,-(sp)
/	mov m hi,-(sp)
/	jsr pc,dmul
/
dmul:
	jsr r0,saver
	tst 14.(sp)	/ see if we can simply multiply
	bne ughx
	tst 18.(sp)
	bne ughx
	mov 16.(sp),r0
	bmi ughx
	mov 20.(sp),r2
	bmi ughx
	mul r2,r0
	mov r1,20.(sp)
	mov r0,18.(sp)
	sxt 16.(sp)
	sxt 14.(sp)
	jsr pc,rest
	rts pc
/
ughx:
	clr -(sp)	/ signs differ flag
	mov sp,r5
	add $22.,r5	/ r5 -> l1
	mov (r5),r0	/ l1
	mov -(r5),r2	/ h1
	bpl 1f	/ make positive if not
	inc (sp)	/ signs differ flag
	neg r2
	neg r0
	sbc r2
1:
	mov -(r5),r1	/ l2
	mov -(r5),r4	/ h2
	bpl 2f	/ make pos if neg
	neg r4
	neg r1
	sbc r4
	dec (sp)	/ (sp) is zero of both signs same
2:
	mov r1,-(sp)	/ l2
	mov r0,-(sp)	/ l1
	mov r2,-(sp)	/ h1
	bic $100000,r0
	bic $100000,r1	/ clear l1 l2 sign bits so mul works
///
/ stack now looks like this: low core up
/	h1<-sp
/	l1
/	l2
/	sign flag
/	r5
/	r4
/	r3
/	r2
/	r1
/	r0
/	return
/	h2  (p4=hi part of product)
/	l2  (p3)
/	h1  (p2)
/	l1  (p1)
/
	mul r4,r2	/  h1 * h2
	mov r2,(r5)+	/ to p4, r5 to p3
	mov r3,(r5)+	/ to p3, r5 to p2
	mov r1,r2	/ l2
	mul r0,r2	/ l1 * l2
	mov r2,(r5)+	/ to p2, r5 to p1
	mov r3,(r5)	/ to p1, r5 to p1
	mov r4,r2	/ h2
	mul r0,r2	/ l1 * h2
	add r3,-(r5)	/ to p2, r5 to p2
	adc -(r5)	/ carry to p3, r5 to p3
	adc -2(r5)	/ carry to p4, r5 to p3
	add r2,(r5)	/ to p3, r5 to p3
	adc -(r5)	/ to p4, r5 to p4
	mov (sp),r2	/ h1
	mul r1,r2	/ l2 * h1
	cmp (r5)+,(r5)+	/ r5 to p2
	add r3,(r5)	/ to p2, r5 to p2
	adc -(r5)	/ to p3, r5 to p3
	adc -2(r5)	/ to p4, r5 to p3
	add r2,(r5)	/ to p3
	adc -(r5)	/ to p4
	tst 2(sp)	/ was l1 sign bit set
	bpl 3f	/ no
	cmp (r5)+,(r5)+	/ r5 to p2
	mov r4,r2	/ h2
	mov r1,r3	/ l2
	ashc $-1,r2
	bhis 4f		/ bcc
	add $100000,2(r5)	/ add to p1, r5 to p2
	adc (r5)
	adc -(r5)	/ r5 to p3
	adc -(r5)	/ r5 to p4
	cmp (r5)+,(r5)+	/  r5 to p2
4:
	add r3,(r5)	/  add to p2
	adc -(r5)	/ to p3
	adc -(r5)	/ to p4
	add r2,2(r5)	/  add to p3
	adc (r5)	/  to p4
3:
	tst 4(sp)	/  is l2 sign bit set
	bpl 5f	/ no
	mov (sp),r2	/  h1
	mov r0,r3	/  l1
	cmp (r5)+,(r5)+	/  r5 to p2
	ashc $-1,r2
	bhis 6f		/ bcc
	add $100000,2(r5)	/ add to p1
	adc (r5)	/ to p2
	adc -(r5)	/ to p3
	adc -(r5)	/ to p4
	cmp (r5)+,(r5)+	/ r5 to p2
6:
	add r3,(r5)	/ to p2
	adc -(r5)	/ to p3
	adc -(r5)	/ to p4
	add r2,2(r5)	/ to p3
	adc (r5)	/ to p4
	tst 2(sp)	/  is l1 sign bit set as well
	bpl 5f	/  no
	cmp (r5)+,(r5)+	/ r5 to p2
	add $40000,(r5)	/ add to p2
	adc -(r5)	/ to p3
	adc -(r5)	/ to p4
5:
	add $6,sp	/ get to sign flag
	tst (sp)+
	beq 7f	/ signs were same
	com (r5)+	/ set product negative
	com (r5)+
	com (r5)+
	com (r5)
	adc (r5)
	adc -(r5)
	adc -(r5)
	adc -(r5)
7:
	jsr pc,rest
	rts pc
/// ***
///
/
/ floating point operations
/	mov (arg1)+,-(sp)
/	mov (arg1)+,-(sp)
/	mov (arg2)+,-(sp)
/	mov (arg2)+,-(sp)
/	jsr pc,fad/fsb/fmp/fdv
/ ans returned over arg1 and stack popped
/
fsb:
	tst 2(sp)	/ jsr pc,*$tryflt, when no fltg hdware.
	bgt 1f
	bic $100000,2(sp)
	br fad
1:
	bis $100000,2(sp)
/
fad:
	jsr r0,saver	/ was: jsr pc,*$tryflt
	mov sp,r5
	add $18.,r5	/ point to arg 1 (high order part)
	jsr pc,fxt	/ extract exp to r0, leave dint on stack
	mov r5,r4	/ point to h1 with r4
	mov r0,r1	/ e1 to r1
	cmp -(r5),-(r5)	/ point to h2 with r5
	jsr pc,fxt	/ decipher it too
	cmp r1,r0	/ which has bigger (or eqal) exponent?
	bge 1f
	mov r4,r3	/ swap the args so that (r4) does
	mov r5,r4
	mov r3,r5
	mov r0,r3
	mov r1,r0
	mov r3,r1
1:
	sub r1,r0	/ get negative shift count
	ash $-7,r0	/ make exponent integer
	cmp r0,$-24.	/ if ge 24 get a zero instead
	ble 2f
	mov (r5)+,r2	/ get number in r2-r3
	mov (r5),r3
	ashc r0,r2
	adc r3
	adc r2	/ round the result
	br 3f
2:
	clr r2
	clr r3	/ shifted off... get zero
3:
	mov r1,r0	/ get correct exponent to r1
	add 2(r4),r3	/ add the numbers
	adc r2
	add (r4),r2
	jsr pc,dfxt	/ reassemble the flt point format
	mov sp,r4
	add $18.,r4	/ set to overwrite arg1
	mov r2,(r4)+
	mov r3,(r4)
exit1:	/ was exit; no good on unix
	jsr pc,rest
	mov (sp),4(sp)
	cmp (sp)+,(sp)+
	jmp *(sp)+	/ return
/
/ floating point utilities
/
fxt:
/
/ in:  r5 points to high part (low addr) of flt pt num
/ out: ro=exponent (bits 14-7, excess 200)
/ (r5),(r5)+2=32 bit 2's compl integer part
/
	mov (r5),r0	/ extract flt number componets
3:
	bic $100177,r0	/ clear fraction
	beq 4f	/ if exp zero, whole number is zero
	bic r0,(r5)	/ clear exp part in (r5)
	bpl 1f	/ number is postive
	bis $200,(r5)	/ num is negative, set hidden bit
	bic $100000,(r5)	/ reset sign bit
	com (r5)+
	com (r5)
	adc (r5)
	adc -(r5)
	br 2f
1:
	bis $200,(r5)	/ set hidden bit
2:
	rts pc
4:
	clr 2(r5)	/ make sure number is zero
	clr (r5)
	rts pc
/
/ routine to reverse the procedure
/	in:  r0 = exponent
/	r2-r3 = 2's compl integer part
/	out: r2-r3 = flt pt number
/
dfxt:
	mov r2,-(sp)	/ save sign
	ashc $0,r2	/ check whole number for zero and sign
	beq 3f	/ done if zero
	bpl 1f	/ complement it if it's negative
	com r2
	com r3
	adc r3
	adc r2
1:
	bit $077400,r2	/ shift data bits out of exp
	beq 2f
	add $200,r0	/ incr exponent part
	ashc $-1,r2	/ divide by two
	bne 1b	/ always branches
	br 6f	/ you cant get here, treat as oveflow
2:
	bit $200,r2
	bne 4f	 / make sure there's a data bit for hidden bit
	sub $200,r0	 / decr exponent
	ashc $1,r2	 / mul by two
	bne 2b	 / always branches
	br 6f	 /  you cant get here
4:
	bit $100177,r0
	bne 6f	/ exponent under or over flow; don't know which
	bic $200,r2	/ hide hidden bit
	bis r0,r2	/ move exp into number
	tst (sp)+	/ okay, what sign was it
	bpl 5f
	bis $100000,r2	/ set minus
5:
	rts pc
/
6:
3:
	clr r2
	clr r3
	tst (sp)+
	rts pc
//
fmp:
	jsr r0,saver	/ was: jsr pc,*$tryflt
	mov sp,r5
	add $18.,r5	/ r5 points to arg1 hi part (h1)
	jsr pc,fxt	/ busy it out
	cmp -(r5),-(r5)	/ goto arg2
	mov r0,r1	/ save exp
	jsr pc,fxt
	sub $[200]*200,r0	/ sub $40000,r0	/ subtr excess 200
	add r1,r0
	mov $2,r4
1:
	mov (r5)+,r2	/ prenormalize a little
	mov (r5)+,r3
	ashc $4,r2
	mov r3,-(sp)
	mov r2,-(sp)
	sob r4,1b	/ two ints copied to stack
	jsr pc,dmul	/ multiply and be fruitful
	mov (sp)+,r2
	mov (sp)+,r3	/ copy high two wrds of answer
	cmp (sp)+,(sp)+	/ bump past other two
	cmp -(r5),-(r5)	/ r5 to hi
mexit:
	jsr pc,dfxt	/ cnvt to flt pt
	mov r2,(r5)+
	mov r3,(r5)
	jmp exit1	/ done!
/
/ divide
/
fdv:
	jsr r0,saver	/ was: jsr pc,*$tryflt
	mov sp,r5	/ this code same as mul and fad
	add $14.,r5	/ except we pick up arg2 first
	jsr pc,fxt
	mov r0,r1
	cmp (r5)+,(r5)+	/ back to arg 1
	jsr pc,fxt
	sub r1,r0	/ exps subtract
	add $[200+1]*200,r0	/ add $40200,r0	/ add exc 200 and shift
	mov (r5)+,r2
	mov (r5),r3
	ashc $7,r2	/ get full bits from 3-word numerator
	clr -(sp)	/ trailing zeroes
	mov r3,-(sp)
	mov r2,-(sp)
	mov $3,-(sp)	/ 3-words
	cmp -(r5),-(r5)	/ to arg2 l2
	mov (r5),-(sp)
	mov -(r5),-(sp)	/ denom
	mov $2,-(sp)	/ 2-words
	jsr pc,ddiv	/ divide and conquer
	add $10.,sp	/ past remainder and counts
	mov (sp)+,r2	/ middle word of quotient
	mov (sp)+,r3	/ low word
	cmp (r5)+,(r5)+	/ p5 to h1
	jmp mexit	/ use fmp exit code. r5 in right place.
///
/ ***	The following from NORGEN code not being used  ***
/ try hardware floating point operation to see if option installed
/
/	tryflt:
/		mov $nohdw,*$10
/		clr *$12
/		clr -(sp)
/		clr -(sp)
/		clr -(sp)
/		clr -(sp)
/		075006	 / fadd sp
/		cmp (sp)+,(sp)+	/ is installed. set up calls to hdw.
/		mov jump,fad
/		mov jump,fsb
/		mov jump,fmp
/		mov jump,fdv
/		mov $hfad,fad+2
/		mov $hfsb,fsb+2
/		mov $hfmp,fmp+2
/		mov $hfdv,fdv+2
/		sub $4,(sp)	/ bump ret addr back 4.
/		rts pc
/	/
/	nohdw:
/		mov $saver,fad+2
/		mov $saver,fmp+2
/		mov $saver,fdv+2
/		mov test,fsb
/		mov test+2,fsb+2
/		mov jsr0,fad
/		mov jsr0,fmp
/		mov jsr0,fdv
/		add $12.,sp	/ bump past 8 flt pt, 4 interrupt
/		sub $4,(sp)	/ bump ret addr back to jsr
/		rts pc
/	/
/	test:
/		tst 2(sp)
/	jsr0:
/		jsr r0,*$saver
/	/
/	hfad:	sub $2,(sp)
/		mov $240,*(sp)
/		sub $2,(sp)
/		mov $75006,*(sp)
/		rts pc
/	hfsb:	sub $2,(sp)
/		mov $240,*(sp)
/		sub $2,(sp)
/		mov $75016,*(sp)
/		rts pc
/	hfmp:	sub $2,(sp)
/		mov $240,*(sp)
/		sub $2,(sp)
/		mov $75026,*(sp)
/		rts pc
/	hfdv:	sub $2,(sp)
/		mov $240,*(sp)
/		sub $2,(sp)
/		mov $75036,*(sp)
/		rts pc
/	/
/	jump:	jmp *$0
