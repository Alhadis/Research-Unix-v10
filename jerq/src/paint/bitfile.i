# 1 "bitfile.c"

# 1 "./paint.h"

# 1 "/usr/jerq/include/jerq.h"







# 1 "/usr/jerq/include/sys/2681.h"










































































struct duart
{
	int : 16; int :8; unsigned char mr1_2a;	  
	int : 16; int :8; unsigned char a_sr_csr; 
	int : 16; int :8; unsigned char a_cmnd;	  
	int : 16; int :8; unsigned char a_data;   

	int : 16; int :8; unsigned char ipc_acr;  
	int : 16; int :8; unsigned char is_imr;	  
	int : 16; int :8; unsigned char ctur;	  
	int : 16; int :8; unsigned char ctlr;	  

	int : 16; int :8; unsigned char mr1_2b;	  
	int : 16; int :8; unsigned char b_sr_csr; 
	int : 16; int :8; unsigned char b_cmnd;	  
	int : 16; int :8; unsigned char b_data;   

	int : 16; int :8; int :8;		  

	int : 16; int :8; unsigned char ip_opcr;  
	int : 16; int :8; unsigned char scc_sopbc;
	int : 16; int :8; unsigned char scc_ropbc;
};

























 


























































































# 8 "/usr/jerq/include/jerq.h"






















typedef int	Word;		

typedef unsigned int	UWord;	

typedef struct Point {
	short	x;
	short	y;
} Point;

typedef struct Rectangle {
	Point origin;
	Point corner;
} Rectangle;

typedef struct Bitmap {
	Word	*base;		
	unsigned width;		
	Rectangle rect;		
	char	*_null;		
} Bitmap;

typedef struct Menu{
	char	**item;			
	char	*(*generator)();	
	short	prevhit;		
	short	prevtop;		
} Menu;

typedef struct Texture32 {
	Word bits[32];
} Texture32;

typedef struct Texture {
	short	bits[16];
} Texture;

extern struct Mouse {
	Point	xy;
	short	buttons;
} mouse;

# 75 "/usr/jerq/include/jerq.h"















Rectangle getrectb(), getrect();










extern Word topbits[], botbits[];	
extern Rectangle Jrect;
extern Bitmap display;




typedef int	Code;




Point add(), sub(), mul(), div(), jstring(), string();
Rectangle rsubp(), raddp(), inset();
Word *addr();
char	*alloc(), *gcalloc();
Bitmap *balloc();
Texture *cursswitch();












# 1 "/usr/jerq/include/mux.h"




# 1 "/usr/jerq/include/layer.h"


typedef struct Layer{
	
	Word *base;
	unsigned width;
	Rectangle rect;	







	short someobs, nonevis;
	struct Layer *front;	
	struct Layer *back;	
	Bitmap *obs;		
}Layer;

typedef Layer Obscured;

Rectangle rsubp();
Layer *newlayer();
# 27 "/usr/jerq/include/layer.h"


# 5 "/usr/jerq/include/mux.h"



# 1 "/usr/jerq/include/jerqproc.h"

# 1 "/usr/jerq/include/tty.h"





struct ttychars {
	char	flags0;		
	char	flags1;		
	char	erase;		
	char	kill;		
	char	intrc;		
	char	quitc;		
	char	startc;		
	char	stopc;		
	char	eofc;		
	char	brkc;		
};



struct ttycmesg {
	char		cmd;		
	char		chan;		
	struct ttychars	ttychars;
};







# 2 "/usr/jerq/include/jerqproc.h"



extern	int	NPROC;




struct cbuf {	
	struct cbuf *next;
	short	word;
};
struct clist {
	struct cbuf *c_tail;
	struct cbuf *c_head;
	short	c_cc;
	short	state;
};

struct pcb {
	int	psw;
	char	(*pc)();
	int	sp;
	int	lower;
	int	upper;
	int	ap;
	int	fp;
	int	reg[9];		
	int	blkmv;
};
struct Proc {
	struct pcb pcb;
	char	*text;
	char	*data;
	char	*bss;
	int	state;
	Layer	 * layer;
	Rectangle rect;	
	struct clist kbdqueue;
	char	**traploc;
	int	*traptype;
	int	nticks;
	Texture * cursor;
	short	inhibited;
	short	nchars;
	unsigned char	cbuf[64	*3];		
	unsigned char	*cbufpin, *cbufpout;
	struct ttychars ttychars;		
	int	stack[(4096-sizeof(struct _*))/sizeof(int)];	
	struct Font *defaultfont;
};

struct Nqueue {			
	struct Nqueue *next;
	struct Proc *proc;
	long data;
	char name[4];
};

# 69 "/usr/jerq/include/jerqproc.h"




# 101 "/usr/jerq/include/jerqproc.h"





# 8 "/usr/jerq/include/mux.h"






extern Bitmap *Jdisplayp;

typedef	int (*ptr_fint)();






























































































































# 131 "/usr/jerq/include/jerq.h"

# 1 "/usr/jerq/include/setup.h"























































struct bram_mem {
	int:16;		unsigned char byte;
	int:8;
	};
struct blop {
	union blip{
		struct vals {
			int:16;		unsigned char baud;
			int:8;
			int:16;		unsigned char retkey;
			int:8;
			int:16;		unsigned char newline;
			int:8;
			int:16;		unsigned char duplex;
			int:8;
			int:16;		unsigned char keytone;
			int:8;
			int:16;		unsigned char screencolor;
			int:8;
			} individ;
		struct bram_mem opt_array[6];
		} opts;
	struct bram_mem pfkeys[8][50+1];	
	struct bram_mem numlock;		
	struct bram_mem capslock;		
	struct bram_mem dwnldflag;		
	struct bram_mem bmaxaddr;		
	} ;


	
# 107 "/usr/jerq/include/setup.h"

extern char baud_speeds[];
extern char *val_baud[];
extern char *val_retkey[];
extern char *val_newline[];
extern char *val_duplex[];
extern char *val_keytone[];
extern char *val_screencolor[];
extern char *val_pfkeys[];
extern char **val_indx[];
extern char val_maxes[];

extern long maxaddr[];












# 132 "/usr/jerq/include/jerq.h"
extern Rectangle Drect;
# 143 "/usr/jerq/include/jerq.h"


# 2 "./paint.h"

# 1 "/usr/jerq/include/jerqio.h"



extern	struct	_iobuf {
	int	count;
	char	*cp;
	char	*base;
	char	flag;
	char	file;
} _iob[15];










# 23 "/usr/jerq/include/jerqio.h"












struct _iobuf	*fopen();
struct _iobuf	*popen();
# 3 "./paint.h"

# 1 "/usr/jerq/include/font.h"















typedef struct Fontchar
{
	short x;		
	unsigned char top;	
	unsigned char bottom;	
	char left;		
	unsigned char width;	
} Fontchar;

typedef struct Font
{
	short n;		
	char height;		
	char ascent;		
	long unused;		
	Bitmap *bits;		
	Fontchar info[1];		
} Font;

extern Font *infont();		
extern Font *getfont();		



# 42 "/usr/jerq/include/font.h"



# 4 "./paint.h"

# 1 "/usr/jerq/include/menu.h"
typedef struct NMitem
{
	char	*text;
	char	*help;
	struct NMenu *next;
	void	(*dfn)(), (*bfn)(), (*hfn)();
	long	data;		
} NMitem;
typedef struct NMenu
{
	NMitem	*item;			
	NMitem	*(*generator)();	
	short	prevhit;		
	short	prevtop;		
} NMenu;
extern NMitem *hmenuhit();
# 5 "./paint.h"




typedef struct{
	Rectangle rect;		
	Bitmap *grn;		
	Bitmap *alf;		
}Image;


















extern Image *brush, Canvas;
extern Point brushcen;
Image *ialloc();
Image *iread();
extern int curstyle;
extern int (*styles[])();
extern int curlinestyle;
extern char *linestring;
extern int curop;
extern int ops[][2];
extern int textured;
extern Rectangle msgrect;
Bitmap *getbitmap();
extern NMenu Mmain;
extern Texture tex;
extern Image *texmap;
char *		(*	((struct Nqueue * (*)())((ptr_fint *)0x0071e700)[ 89]))()	;
extern char brushname[], brushdir[], picdir[], texdir[];
# 2 "bitfile.c"





static Word buffer[	800/32], raster[	800/32];
static Bitmap bbuffer={ buffer, (sizeof buffer)/(sizeof(Word)), 0, 0, 	800, 1};
static int ctype, count; static short *p1, *endraster;



static putrast(), putbits(), getrast();
putbitmap(bp, r, f, compl)
Bitmap *bp, *compl;
Rectangle r;
struct _iobuf *f;
{
	register i, nrasters, rastwid, nrword;
	Rectangle rrast;
	nrasters=r.corner.y-r.origin.y;
	i=r.corner.x-r.origin.x;
	rastwid=(i+16-1)/16;
	nrword=(i+32-1)/32;
	endraster=(short *)raster+rastwid-1;
		(putc(nrasters&255,  f), putc((nrasters>>8)&255,  f));
	i|=0x8000;
		(putc(i&255,  f), putc((i>>8)&255,  f));
			(*	((void (*)())((ptr_fint *)0x0071e700)[ 45]))(&bbuffer,  bbuffer.rect,	 ((Code) 2)	)	;
	for(i=0;i<nrword;i++)
		raster[i]=0;
	rrast=r;
	if(compl)
				(*	((void (*)())((ptr_fint *)0x0071e700)[ 45]))(compl,  r,	 ((Code) 3)	)	;
	for(;rrast.origin.y<r.corner.y;rrast.origin.y++){
		rrast.corner.y=rrast.origin.y+1;
		if(compl)
					(*	((void (*)())((ptr_fint *)0x0071e700)[ 45]))(compl,  rrast,	 ((Code) 3)	)	;
				(*	((void (*)())((ptr_fint *)0x0071e700)[ 8]))(bp,  rrast,	 &bbuffer,  	((((short)(0))<<16)|((unsigned short)( 0))),  ((Code) 0)	)	;
		for(i=0;i<nrword;i++)
			raster[i]^=buffer[i];
		putrast(f);
		for(i=0;i<nrword;i++)
			raster[i]=buffer[i];
	}
}
static putrast(f)
struct _iobuf *f;
{
	register short *p2;
	p1=p2=(short *)raster;
	do{
		if(p1>=p2){
			p2=p1+1;
			count=2;
			ctype = *p1==*p2;

		}
		else if((*p2==*(p2+1))==ctype){
			if(++count>=127){
				putbits(f);
				p1=p2+2;
			}
			else
				p2++;

		}
		else if(ctype){
			putbits(f);
			p1=p2+1;
			ctype=0;

		}
		else{
			count--;
			putbits(f);
			p1=p2;
			ctype=1;
		}
	}while(p2<endraster);
	if(p1<=endraster){
		if(p2>endraster)
			count--;
		putbits(f);
	}
}
static putbits(f)
struct _iobuf *f;
{
	int c;
	c=count;
	if(ctype){
		c+=128;
		count=1;
	}
	putc(c, f);
	fwrite((char *)p1, sizeof *p1, count, f);
}



Bitmap *getbitmap(f, compl)
struct _iobuf *f;
Bitmap *compl;
{
	register Bitmap *bp;
	register h, w, i, j;
	if((h=getc(f))<0)
		return 0;
	h|=getc(f)<<8;
	w=getc(f);
	w|=getc(f)<<8;
	if(w&0x8000){
		w&=~0x8000;
		w=(w+16-1)/16;
	}
	bp=		(*	((Bitmap * (*)())((ptr_fint *)0x0071e700)[ 6]))(		((((short)(0))<<16)|((unsigned short)(  0))), 	((((short)( 16*w))<<16)|((unsigned short)(  h))))	;
	if(bp==0)
		return(0);
			(*	((void (*)())((ptr_fint *)0x0071e700)[ 45]))(&bbuffer,  bbuffer.rect,	 ((Code) 2)	)	;
	if(compl)
				(*	((void (*)())((ptr_fint *)0x0071e700)[ 45]))(compl,  		(*	((Rectangle (*)())((ptr_fint *)0x0071e700)[ 40]))(bp->rect,  (&Canvas)->rect.origin)	,	 ((Code) 3)	)	;
	for(i=0;i!=h;i++){
		if(getrast(f, (short *)raster, w)){
					(*	((void (*)())((ptr_fint *)0x0071e700)[ 7]))(bp)	;
			return(0);
		}
		for(j=0;j!=bp->width;j++)
			buffer[j]^=raster[j];
				(*	((void (*)())((ptr_fint *)0x0071e700)[ 8]))(&bbuffer,  bbuffer.rect,	 bp,  	((((short)(0))<<16)|((unsigned short)( i))),  ((Code) 0)	)	;
		if(compl)
					(*	((void (*)())((ptr_fint *)0x0071e700)[ 45]))(compl,  		(*	((Rectangle (*)())((ptr_fint *)0x0071e700)[ 40]))(		((((short)(0))<<16)|((unsigned short)(  i))), 	((((short)( 16*w))<<16)|((unsigned short)(  i+1))), 
						(&Canvas)->rect.origin)	,	 ((Code) 3)	)	;
	}
	return(bp);
}
static getrast(f, p, nwords)	
struct _iobuf *f;
register short *p;
int nwords;
{
	register i, count;
	for(;nwords>0;nwords-=count){
		if((count=getc(f))<=0)
			return 1;
		if(count&128){
			count&=127;	
			if(fread(p, sizeof(short), 1, f)!=1)
				return 1;
			for(i=1;i!=count;i++){
				*(p+1) = *p;
				p++;
			}
			p++;
		}
		else{
			if(fread(p, sizeof(short), count, f)!=count)
				return 1;
			p+=count;
		}
	}
	return(nwords!=0);
}
