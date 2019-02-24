/*
 *	Defines for Tmenu and Titem menu structures.
 *	Old menu structures can be found in <dmd.h>.
 */

typedef struct Titem
{
    char	    *text;	/* string for menu */
    struct {
	unsigned short    uval;	/* user field */
    	unsigned short    grey;	/* flag indicates invalid selection */
    } ufield;
    struct Tmenu    *next;	/* pointer to sub-menu */
    Bitmap	    *icon;	/* pointer to the icons bitmap */
    struct Font	    *font;	/* font defined for this item */
    void    	    (*dfn)();	/* execute function before sub-menu */
    void    	    (*bfn)();	/* execute function after sub-menu */
    void    	    (*hfn)();	/* execute function on selection */
} Titem;

typedef struct Tmenu
{
    Titem    *item;		/* Titem array, ending with text=0 */
    short    prevhit;		/* private to menuhit() */
    short    prevtop;		/* private to menuhit() */
    Titem    *(*generator)();	/* used if item == 0 */
    short    menumap;		/* bit definition for user Titem structure */
} Tmenu;

/*
 *	The following bit definitions are for the user to define
 *	the structure of the user Titem structure.  These bits are
 *	stored in the menumap data member, which converts the user
 *	Titem structure into that of the Titem structure defined above.
 */
#define TM_TEXT		0x0001		/* defines the text field */
#define	TM_UFIELD	0x0002		/* defines the ufield field */
#define TM_NEXT		0x0004		/* defines the next field */
#define TM_ICON		0x0008		/* defines the icon field */
#define TM_FONT		0x0010		/* defines the font field */
#define TM_DFN		0x0020		/* defines the dfn field */
#define TM_BFN		0x0040		/* defines the bfn field */
#define TM_HFN		0x0080		/* defines the hfn field */

#define TM_EXPAND	1		/* expand submenus on invocation */
#define TM_NORET	2		/* don't return until non-selection */
#define TM_STATIC	4		/* select on button depression */
#define TM_POINT	8		/* specify a point for the root menu */

#define MAX_TMFIELDS	9		/* number of fields in Titem */
#define MAX_MASK	0x0100		/* upper limit bit definition */

extern void tm_ret();			/* return function from TM_NORET */
extern Titem *tmenuhit();


/* This is the button 3's global menu item */
typedef struct Titem1 {
	char *text;
	struct {
		unsigned short uval;
		unsigned short grey;
	} ufield;
	struct Tmenu *next;
        Bitmap *icon;
	void (*dfn)();
} Titem1;


extern Bitmap B_checkmark;
extern Bitmap B_rtarrow;
