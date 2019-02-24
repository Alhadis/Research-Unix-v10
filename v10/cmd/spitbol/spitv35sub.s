#
#	copyright (c) 1981 under berne and universal copyright
#	law by dewar information systems corporation.
#
#	the software described in this document is proprietary
#	to dewar information systems corporation and furnished
#	to the purchaser under a license  for use on a  single
#	computer  system and can be copied (with the inclusion
#	of dewar information systems  corporations's copyright
#	notice) only for use in such system.
#
#
#	cmc	compare character strings
#
	.globl	sbcmc
sbcmc:	pushr	$mr2+mr3
	ashl	$-15,r6,r11
	beqlu	2f
1:	cmpc3	$32768,(r10),(r9)
	blssu	3f
	bgtru	4f
	addl2	$32768,r10
	addl2	$32768,r9
	sobgtr	r11,1b
	bicl2	$0xffff8000,r6
	beqlu	5f
2:	cmpc3	r6,(r10),(r9)
	bgtru	4f
	beqlu	5f
3:	popr	$mr2+mr3
	clrl	r10
	clrl	r9
	movl	(sp)+,r11
	jmp	*(r11)+
4:	popr	$mr2+mr3
	clrl	r10
	clrl	r9
	addl3	(sp)+,$4,r11
	jmp	*(r11)+
5:	popr	$mr2+mr3
	clrl	r10
	clrl	r9
	addl2	$8,(sp)
	rsb
#
#	mcb	move characters backward (for buffer datatype)
#
	.globl	sbmcb
sbmcb:	pushr	$mr2+mr3+mr4+mr5
	subl2	r6,r9
	subl2	r6,r10
	ashl	$-15,r6,r11
	beqlu	2f
1:	movc3	$32768,(r10),(r9)
	movl	r1,r10
	movl	r3,r9
	sobgtr	r11,1b
	bicl2	$0xffff8000,r6
2:	movc3	r6,(r10),(r9)
	movl	r1,r10
	movl	r3,r9
3:	popr	$mr2+mr3+mr4+mr5
	rsb
#
#	mvc	move characters
#
	.globl	sbmvc
	.globl	sbmvw
sbmvc:
sbmvw:
	pushr	$mr2+mr3+mr4+mr5
	ashl	$-15,r6,r11
	beqlu	2f
1:	movc3	$32768,(r10),(r9)
	addl2	$32768,r10
	addl2	$32768,r9
	sobgtr	r11,1b
	bicl2	$0xffff8000,r6
	beqlu	3f
2:	movc3	r6,(r10),(r9)
	addl2	r6,r10
	addl2	r6,r9
3:	popr	$mr2+mr3+mr4+mr5
	rsb
#
#	mwb	move words backward
#
	.globl	sbmwb
sbmwb:	pushr	$mr2+mr3+mr4+mr5
	ashl	$-8,r6,r11
	beqlu	2f
1:	subl2	$256,r10
	subl2	$256,r9
	movc3	$256,(r10),(r9)
	sobgtr	r11,1b
	bicl2	$0xffffff00,r6
	beqlu	3f
2:	subl2	r6,r10
	subl2	r6,r9
	movc3	r6,(r10),(r9)
3:	popr	$mr2+mr3+mr4+mr5
	rsb
#
#	sbtrc	translate characters
#
	.globl	sbtrc
sbtrc:	pushr	$mr2+mr3+mr4+mr5
	ashl	$-15,r6,r11
	beqlu	2f
1:	movtc	$32768,(r10),$0,(r9),$32768,(r10)
	addl2	$32768,r10
	sobgtr	r11,1b
	bicl2	$0xffff8000,r6
	beqlu	3f
2:	movtc	r6,(r10),$0,(r9),r6,(r10)
	clrl	r10
	clrl	r9
3:	popr	$mr2+mr3+mr4+mr5
	rsb
