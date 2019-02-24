#ifndef MENU_H
#define MENU_H
class NMenu;
class NMitem {
public:
	char	*text;
	char	*help;
	NMenu	*next;
	int	result;
};
class NMenu {
public:
	NMitem	*item;			/* string array, ending with 0 */
	NMitem	*(*generator)(int);	/* used if item == 0 */
	short	prevhit;		/* private to menuhit() */
	short	prevtop;		/* private to menuhit() */
};
NMitem *mhit(NMenu *, int, int);

#define WIRE	10
#define BOX	11
#define DOTS	12
#define MEASURE	13
#define ORTHO	14
#define MACRO	15
#define MASTER	20
#define ENTER	21
#define EXIT	22
#define INST	30
#define SWEEP	40
#define CUT	41
#define PASTE	42
#define SNARF	43
#define SCROLL	44
#endif
