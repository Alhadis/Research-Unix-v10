/* C compiler: MIPS configuration parameters */

#define	IREG	0	/* a general-purpose register */
#define	FREG	1	/* a floating-point register */

#include "gen.h"

dclproto(extern Symbol argreg,(int, int, int, int));
dclproto(extern void emitargb,(Node));
dclproto(extern void emitasgnb,(Node));
dclproto(extern void addnops,(void));
extern int framesize;
extern int gnum;
extern char *nrflag;

#define announce(p) (bflag ? put.stackvar(p) : (unsigned)0)
#define strtod(a,b) atof(a)
dclproto(extern double atof,(char *));

struct emit {
	dclproto(void (*seg),(int));
	dclproto(void (*spacen),(int, char *));
	dclproto(void (*string),(char*, char*, int, char*));
	dclproto(unsigned (*emit),(unsigned, int, int, int, int));
	dclproto(void (*addr),(Symbol, Symbol, int));
	dclproto(unsigned (*def),(Symbol));
	dclproto(void (*defaddr),(Symbol));
	dclproto(void (*end),(Symbol));
	dclproto(void (*entry),(Symbol, int, int));
	dclproto(int (*offset),(int));
	dclproto(void (*symbol),(int, Symbol));
	dclproto(void (*word),(unsigned, int));
};

extern struct emit put;

#define	r_at	1
#define	r_gp	28
#define r_sp	29

#define ea_base(wd) 	((wd >> 21) & 0x001f)
#define ea_offset(wd)	((wd) & 0xffff)
#define ea_addr(off,base) (assert(ea_base(off)==0), (((base&0x001f)<<21) | ea_offset(off)))

#define o_add_s		(((unsigned)021<<26)|(020<<21)|0)
#define o_add_d		(((unsigned)021<<26)|(021<<21)|0)
#define o_sub_s		(((unsigned)021<<26)|(020<<21)|1)
#define o_sub_d		(((unsigned)021<<26)|(021<<21)|1)
#define o_mul_s		(((unsigned)021<<26)|(020<<21)|2)
#define o_mul_d		(((unsigned)021<<26)|(021<<21)|2)
#define o_div_s		(((unsigned)021<<26)|(020<<21)|3)
#define o_div_d		(((unsigned)021<<26)|(021<<21)|3)
#define o_mov_s		(((unsigned)021<<26)|(020<<21)|6)
#define o_mov_d		(((unsigned)021<<26)|(021<<21)|6)
#define o_neg_s		(((unsigned)021<<26)|(020<<21)|7)
#define o_neg_d		(((unsigned)021<<26)|(021<<21)|7)
#define o_cvt_d_s	(((unsigned)021<<26)|(020<<21)|33)
#define o_cvt_d_w	(((unsigned)021<<26)|(024<<21)|33)
#define o_cvt_s_d	(((unsigned)021<<26)|(021<<21)|32)
#define o_cvt_w_s	(((unsigned)021<<26)|(020<<21)|36)
#define o_cvt_w_d	(((unsigned)021<<26)|(021<<21)|36)
#define o_c_eq_s	(((unsigned)021<<26)|(020<<21)|50)
#define o_c_eq_d	(((unsigned)021<<26)|(021<<21)|50)
#define o_c_le_s	(((unsigned)021<<26)|(020<<21)|62)
#define o_c_le_d	(((unsigned)021<<26)|(021<<21)|62)
#define o_c_lt_s	(((unsigned)021<<26)|(020<<21)|60)
#define o_c_lt_d	(((unsigned)021<<26)|(021<<21)|60)
#define o_mfc1_s	(((unsigned)021<<26)|(0<<21)|0)	/* mfc1 */
#define o_mtc1_s	(((unsigned)021<<26)|(4<<21)|0)	/* mtc1 */
#define o_mfc1_d	(((unsigned)021<<26)|(1<<21)|0)	/* fake mfc1 */
#define o_mtc1_d	(((unsigned)021<<26)|(5<<21)|0)	/* fake mtc1 */
#define o_cfc1		(((unsigned)021<<26)|(2<<21)|0)
#define o_ctc1		(((unsigned)021<<26)|(6<<21)|0)
#define o_l_s		((unsigned)061<<26)		/* lwc1 */
#define o_l_d		((unsigned)065<<26)		/* ldc1 */
#define o_s_s		((unsigned)071<<26)		/* swc1 */
#define o_s_d		((unsigned)075<<26)		/* sdc1 */
#define o_bc1f		(((unsigned)021<<26)|(0400<<16))
#define o_bc1t		(((unsigned)021<<26)|(0401<<16))
#define o_lui		((unsigned)017<<26)
#define o_lb		((unsigned)040<<26)
#define o_lbu		((unsigned)044<<26)
#define o_lh		((unsigned)041<<26)
#define o_lhu		((unsigned)045<<26)
#define o_lw		((unsigned)043<<26)
#define o_sb		((unsigned)050<<26)
#define o_sh		((unsigned)051<<26)
#define o_sw		((unsigned)053<<26)
#define o_lwl		((unsigned)042<<26)
#define o_lwr		((unsigned)046<<26)
#define o_swl		((unsigned)052<<26)
#define o_swr		((unsigned)056<<26)
#define o_add		(((unsigned)0<<26)|040)
#define o_addu		(((unsigned)0<<26)|041)
#define o_addi		((unsigned)010<<26)
#define o_addui		((unsigned)011<<26)		/* addiu */
#define o_and		(((unsigned)0<<26)|044)
#define o_andi		((unsigned)014<<26)
#define o_sub		(((unsigned)0<<26)|042)
#define o_subu		(((unsigned)0<<26)|043)
#define o_subui		((unsigned)011<<26)		/* addiu */
#define o_or		(((unsigned)0<<26)|045)
#define o_ori		((unsigned)015<<26)
#define o_xor		(((unsigned)0<<26)|046)
#define o_xori		((unsigned)016<<26)
#define o_not		(((unsigned)0<<26)|047)		/* nor */
#define o_mul		(((unsigned)0<<26)|030)
#define o_mulu		(((unsigned)0<<26)|031)
#define o_div		(((unsigned)0<<26)|032)
#define o_divu		(((unsigned)0<<26)|033)
#define o_rem		o_div
#define o_remu		o_divu
#define o_negu		o_subu
#define o_sll		(((unsigned)0<<26)|04)		/* sllv */
#define o_slli		(((unsigned)0<<26)|0)		/* sll */
#define o_srl		(((unsigned)0<<26)|06)		/* srlv */
#define o_srli		(((unsigned)0<<26)|02)		/* srl */
#define o_sra		(((unsigned)0<<26)|07)		/* srav */
#define o_srai		(((unsigned)0<<26)|03)		/* sra */
#define	o_slt		(((unsigned)0<<26)|052)
#define o_slti		((unsigned)012<<26)
#define	o_sltu		(((unsigned)0<<26)|053)
#define o_sltui		((unsigned)013<<26)
#define o_beq		((unsigned)04<<26)
#define o_bne		((unsigned)05<<26)
#define o_bge		(((unsigned)01<<26)|(1<<16))	/* bgez */
#define o_bgt		((unsigned)07<<26)		/* bgtz */
#define o_ble		((unsigned)06<<26)		/* blez */
#define o_blt		(((unsigned)01<<26)|(0<<16))	/* bltz */
#define o_b		((unsigned)02<<26)		/* j */
#define o_jal		((unsigned)03<<26)		/* jalr */
#define o_jalr		(((unsigned)0<<26)|011)		/* jalr */
#define o_jr		(((unsigned)0<<26)|010)		/* jr */
#define o_move		o_addu
#define o_mfhi		(((unsigned)0<<26)|020)
#define o_mflo		(((unsigned)0<<26)|022)
#define o_break		(((unsigned)0<<26)|015)
#define o_syscall	(((unsigned)0<<26)|014)
