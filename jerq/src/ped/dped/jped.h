/*	Definition for terminal part of ped		*/
#include "device.h"

#ifdef D5620
#include "/usr/jerq/include/jerq.h"
#include "/usr/jerq/include/font.h"
#define MXPTS 512
#define MXOBJ 256
#endif
#ifdef BLIT
#include "/usr/blit/include/jerq.h"
#include "/usr/blit/include/font.h"
#define MXPTS 256
#define MXOBJ 96
#endif

#define MX (mouse.xy.x)
#define MY (mouse.xy.y)
#define GMX (!grsize? MX: gri(MX,xshift))
#define GMY (!grsize? MY: gri(MY,yshift))
#define MXMAC 4
#define NR_TAR 10
#define NR_PRV 5
#define MXFONT 5
#define ABSURD -8192
#define SILLY -4096

#define FAR (abs((pbfp-1)->x-q.x)>NR_PRV || abs((pbfp-1)->y-q.y)>NR_PRV)

#define MINUS 0377

#define bmark(A,B) bitblt(B,B->rect,&display,sub(A,Pt(8,8)),dr_mode);
/*
 #define pline(A,B) segment(&display,A,B,dr_mode);
*/
#define pstring(A,B) string(&defont,B,&display,A,dr_mode)
#define pcirc(C,R) circle(&display,C,R,dr_mode);
#define press1()	for(;;wait(MOUSE)) if( button1()) break
#define release1()	for(;;wait(MOUSE)) if(!button1()) break


typedef Point pointS, * pointP;

typedef struct obj {
	char status;
	char type;	/* pol(p|P), spl(s|S), circ(c), box(b|B), txt(t) */
	short n;
	short size;	/* PS for txt, radius for circ. */
	short ps;	/* PS for lines */
	char color;
	char pen;
	union pp {
		struct obj  ** meml;
		char *txtl;
	} pp;
	pointP p0;	/* where points go */
} objS, * objP;

#define txt pp.txtl
#define mem pp.meml

extern objP obj[MXOBJ];	/* Object pointer array */
extern objP tmpobf[];
extern objP macro[MXMAC];
extern objP salvage;

extern objP dupl_obj();
extern objP rec_one();
extern objP mk_obj();
extern short no_obj;	/* Number of obj pointers used */
extern short no_macro;
extern short D_ON; 	/* 1 if display, 0 if choose */

extern Point pbuf[];
extern char story[];

extern pointP lockpnt;	/* point found by locate routine */
extern objP locate();
extern objP s_locate();

extern short grsize;
extern short grihalf;
extern short xshift, yshift;
extern short grsize;
extern short no_font;
extern Font *kf;
extern short flen;

#define SOLID 's'
#define DASH 'd'
#define DOUBLE 't'
#define HEAVY 'h'
#define ROMAN 'R'
#define SEPAR 012

extern short drawpar;	/*draw heads and links*/
extern short nosk;	/*no sketch. draw roughly*/
extern Code dr_mode;	/* F_XOR or F_STORE */

extern Texture brush;
extern Texture clock;
extern Texture crcross;
extern Texture deadmouse;
extern Texture grey;
extern Texture livemouse;
extern Texture nsquare;
extern Texture pencil;
extern Texture pcross;
extern Texture pound;
extern Texture skull;
extern Texture target;
extern Texture attsh, blksh, hrzsh, ldish, rdish, vrtsh;
extern Texture wave1, wave2;

extern Rectangle board;
extern Rectangle bulletin;
extern Rectangle lfside[];


extern Bitmap *negsq, *poscross, *ball, *vball[];

extern Point placelab;

extern char * Alloc();
extern char * Free();

extern char scbf[];

extern char garbage[];

struct c_unit {
	char * menu_item;
	char **wlist;
	short (*fun)();
	short how_many;
	char imd;
	char txr;
	};

extern short osubsel;
extern short subsel;
extern struct c_unit vault[];
extern short c_menu;

extern char **auxmen;
extern char **oauxmen;
extern short m_size;
extern short om_size;
extern Texture * T_tab[];

extern short g_short();

extern short def_font;
extern Font *fontrcv[];
extern char textmode;
extern short def_size;
extern short lin_size;

extern struct disper {
	objP a;
	short x, y;
	};
extern struct disper dstack[], *stp;


extern objP old_fam;
extern short room;
extern short s_char();
extern short s_short();
extern short s_string();
extern short sendstring ();
extern short s_rect();
extern short s_flush();
extern short direct();
extern short transmit();
extern short trans_one();
extern short receive();
extern short zoom();
extern short no_kill();
extern short get_out();
extern short bailout();
extern short affine();
extern short getex();
extern short use_pen();
extern short wipe();
extern short draw_box();
extern short shades();
extern short place();
extern short ppfill();
extern short pfill();
extern short sides();
extern short mk_sides();
extern short ed_sides();
extern short sgswitch();
extern short realfill();
extern short draw();
extern short xcut();
extern short ninch();
extern short ffree();
extern short loadfont();
extern short idle();
extern short adiplay();
extern short get_help();
extern short setup_b();
extern short draw_menu();
extern short wheremouse();
extern short scroll();
extern short spilld();
extern short checkwin();
extern short color();
extern short mk_assemb();
extern short mk_fam();
extern short dis_fam();
extern short pack();
extern short arrange();
extern short mvcp();
extern short tr_obj();
extern short zap_obj();
extern short join();
extern short merge();
extern short rev_obj();
extern short chk_end();
extern short oddf();
extern short brk_obj();
extern short mice_pts();
extern short draw_spec();
extern short tog_grid();
extern short gri();
extern short plot_all();
extern short plot_grid();
extern short dr_link();
extern short mapfont();
extern short plot_one();
extern short pl_poly();
extern short ssline();
extern short spline();
extern short spl_arc();
extern short dline();
extern short dspline();
extern short pline();
extern short plot_arrow();
extern short no_line();
extern short klosed();
extern short fullob();
extern short pdline();
extern short rtext();
extern short rdup();
extern short obseat();
extern short reshape();
extern short poly_to_spl();
extern short repeat_pt();
extern short clop_obj();
extern short put_arrow();
extern short zap_pt();
extern short mv_pt();
extern short insrt_pt();
extern short obj_restore();
extern short fine_view();
extern short e_text();
extern short adj_text();
extern short ch_color();
extern short transf_obj();
extern short gcenter();
extern short sum_pts();
extern short gcenter();
extern short adj_box();
extern short flip();
extern short f1();
extern short irotate();
extern short host_one();
extern short remote();
extern short prec();
extern short ownrec();
extern short bconstruct();
extern short kbdstr();
extern short jtag();
extern short blitblt();
extern short adisplay();
