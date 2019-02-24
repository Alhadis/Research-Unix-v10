typedef enum
{
	ld_lowreg,
	ld_hireg,
	ld_stack,

	ld_last,
}
	ldatas;

#define	LD_NUM	((int)ld_last)

#define	LD_VOL	0x0001
