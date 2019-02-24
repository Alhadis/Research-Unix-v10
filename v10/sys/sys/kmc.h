struct kmc {
	char	k_stat;
	char	k_type;
	char	k_written;
	struct device *k_addr;
	int	(*k_rint)();
	int	(*k_xint)();
	int	(*k_reset)();
};

/*
 * kmc-related ioctls
 * should really be in kmcio.h
 */
#define	KSTEP	1
#define	KMS	2
#define	KCSR	3
#define	KSTOP	4
#define	KMCLR	5
#define	KRUN	6
#define	KLU	7
#define	KWRCR	8
#define	KRESET	9

#define	KCSETA	(('k'<<8)|1)

struct kmcntl {
	int	kmd;
	short	*kcsr;
	int	kval;
};
