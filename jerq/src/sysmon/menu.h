typedef struct NMitem
{
	char	*text;
	char	*help;
	struct	NMenu *next;
	void	(*dfn)(), (*bfn)(), (*hfn)();
	long	data;		/* user only */
} NMitem;

typedef struct NMenu
{
	NMitem	*item;			/* NMitem array, ending with text=0 */
	NMitem	*(*generator)();	/* used if item == 0 */
	short	prevhit;		/* private to menuhit() */
	short	prevtop;		/* private to menuhit() */
} NMenu;

extern NMitem *mhit();
