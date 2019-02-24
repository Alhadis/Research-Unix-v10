/ NORGEN-format floating point (most signif. part in hi address)
.text
.globl __ffrl,__ftol,__fadd,__fsub,__fmul,__fdiv
.globl __lton,__ntol,__fton,__ffrn,__ntoi,__iton,__uton,__ntou
.globl __ftoi,__ffri,__ffru,__ftou
.globl __fsign,__fsign2
.globl __one
.globl ret / fltg-pt interrupt level code should save/restore ret

.globl fad,fsb,fmp,fdv,int,float
__ffrl:
	mov (sp)+,ret
	jsr pc,float
	mov (sp)+,r1 /hi float in low pseodo-long
	mov (sp),r0  /low float in hi pseudo-long
	jsr pc,*ret

__ffri:	mov (sp)+,ret
	tst (sp)
	sxt -(sp)
	jsr pc,float
	mov (sp)+,r1
	mov (sp),r0
	jmp *ret

__ffru:	mov (sp)+,ret
	clr -(sp)
	jsr pc,float
	mov (sp)+,r1
	mov (sp),r0
	jmp *ret

__ftol:	mov 2(sp),-(sp)
	mov 6(sp),-(sp)
	jsr pc,int
	mov (sp)+,r0
	mov (sp)+,r1
	rts pc

__ftou:
__ftoi:	mov 2(sp),-(sp)
	mov 6(sp),-(sp)
	jsr pc,int
	mov (sp)+,r0
	mov (sp)+,r0
	rts pc

__fadd:	mov $fad,r0

binop:	mov 6(sp),-(sp)
	mov 10.(sp),-(sp)
	mov 6(sp),-(sp)
	mov 10.(sp),-(sp)
	jsr pc,(r0)
	mov (sp)+,r1
	mov (sp)+,r0
	rts pc

__fsub:	mov $fsb,r0
	jbr binop

__fmul:	mov $fmp,r0
	jbr binop

__fdiv:	mov $fdv,r0
	jbr binop

__lton:
__ntol:	mov 2(sp),r1
	mov 4(sp),r0
	rts pc

__ntou:
__ntoi:	mov 2(sp),r0
	rts pc

__iton:	mov 2(sp),r0
	sxt r1
	rts pc

__uton:	mov 2(sp),r0
	clr r1
	rts pc

__fton:	mov 2(sp),-(sp)
	mov 6(sp),-(sp)
	jsr pc,int
	mov (sp)+,r1
	mov (sp)+,r0
	rts pc

__ffrn:	mov 2(sp),-(sp)
	mov 6(sp),-(sp)
	jsr pc,float
	mov (sp)+,r1
	mov (sp)+,r0
	rts pc

.data
ret:0
__fsign:__fsign2: 0
100000
__one: 0
40200
