#include <jerq.h>
#include <font.h>
#define PADS_TERM
#include "../pads.pri"

long assertf();
#define ASSERT
#define assert(e) ( assertf( (long) (e) ) )
#define salloc(s) ((struct s*) Alloc(sizeof(struct s)) )

typedef unsigned char uchar;
typedef int  (*PFI)();
typedef long (*PFL)();
typedef char (*PFC)();

typedef struct Line {
	long		object;
	Index		carte;
unsigned char		ptop;
unsigned char		phit;
	Attrib		attributes;
/* ***************************** */
	char		*text;
	long		key;
	Rectangle	rect;
 struct	Line		*down;
 struct	Line		*up;
} Line;

typedef struct Pad {
	long		object;
	Index		carte;
unsigned char		ptop;
unsigned char		phit;
	Attrib		attributes;
/* ***************************** */
 struct	Pad		*front;
 struct	Pad		*back;
	Rectangle	rect;
	Rectangle	srect;
	char		*name;
	Line		sentinel;
	long		selkey;
	short		lo;
	short		hi;
	short		ticks;
	short		tabs;
} Pad ;

typedef struct Selection {
	Pad	*pad;
	Line	*line;
} Selection;

typedef struct Entry {
	char	*text;
	int	(*action)();
	long	opand;
struct	Script	*script;
} Entry;

typedef struct Script{
	Entry	*(*generator)();
	int	(*limits)();
struct	Script	*more;
	Index	cindex;		/* bogus! bogus! bogus! */
	short	items;
	short	width;
	short	prevtop;
	short	prevhit;
} Script;

Entry *ScriptHit();

typedef enum Cover { CLEAR, PARTIAL, COMPLETE } Cover;

extern Selection Selected;	/* selected line 			*/
extern Pad *Current;		/* current pad				*/
extern long HostObject;		/* global arg to HostAction(Index)	*/
extern long HostParent;		/*  and its pad's object		*/
extern short Scrolly;		/* suggest middle for Paint()		*/

extern Point Zpoint;
extern Rectangle ZRectangle;
extern Pad *DirtyPad;

#define BIGMEMORY 1
#define NOVICEUSER 2
int Configuration;

Point dxordy();
Rectangle boundrect();
Rectangle moverect();
Rectangle scrollbar();
Rectangle canon();

char *strcpy();
char *strdupl();
char *strcat();
char *itoa();

char *Alloc();
char *GCAlloc();

char *IndexToStr();
Carte *IndexToCarte();

int DeletePick();
int DeletePad();
int Move();
int CutLine();
int Sever();
int Reshape();
int HostAction();
int HostNumeric();
int FoldToggle();

Pad *New();
Pad *PickPad();
Pad *PidToPad();

Entry *TitleEntry();
Entry *FoldEntry();

extern Texture Danger;
extern Texture Bullseye;
extern Texture NoMemory;
extern Texture NoGCMemory;
extern Texture Coffee;
extern Texture HostBusy;
extern Texture Arrow;
extern Texture *Cursor;

extern Index CIndex;

#define butts (mouse.buttons&07)
#define BUTT1 4
#define BUTT2 2
#define BUTT3 1

typedef struct RectList RectList ;
struct RectList {
	Rectangle	*rp;
	RectList	*more;
};
