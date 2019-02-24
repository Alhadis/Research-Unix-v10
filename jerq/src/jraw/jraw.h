#ifndef JRAW_H
#define JRAW_H
#include <CC/jerq.h>
#include <CC/jerqio.h>
#undef P

/* codes for text placement */
#define	BL	01	/* placement is bottom left corner of string */
#define BC	02	/* bottom center */
#define BR	04	/* bottom right */
#define RC	010	/* right center */
#define TR	020	/* top right */
#define TC	040	/* top center */
#define TL	0100	/* top left */
#define LC	0200	/* left center */

/* hit codes */
#define WIREHIT		1
#define BOXHIT		2
#define LABHIT		4
#define DOTHIT		010
#define WIREBOX		020
#define BOXBOX		040
#define LABBOX		0100
#define DOTBOX		0200
#define ALLHIT		(WIREHIT|BOXHIT|LABHIT|DOTHIT)
#define ALLBOX		(WIREBOX|BOXBOX|LABBOX|DOTBOX)
#define ALL	(ALLBOX|ALLHIT)

#define min(x,y)	((x < y) ? x : y)
#define max(x,y)	((x > y) ? x : y)

class Hit {
public:
	short type;		/* what kind of hit */
	char f,g;		/* styles due to hit */
	Hit(){}
	Hit(short t, char flag, char galf)	{type = t; f = flag; g = galf;}
};

class Dict;
class List;
class Wire;

extern char *instname;
extern Point offset,dp;
extern Rectangle canon(Rectangle);
extern Dict *ml;
extern List *dl;
extern void drawlist(List *, Bitmap *, int),removelist(List *),scroll();
extern void unselect(int),displacelist(List *, Point),movem(int),snapsel();
extern input(FILE *),output(FILE *),putlist(List *,FILE *),kbdstr(char *);
extern selbox(Rectangle),selborder(int,Rectangle),fixit(),drawicon();
extern edge(int,Point,Rectangle),selpt(Point),meanyhit(int),clearout();
extern strcmp(char *,char *),fclose(FILE *),exit();
extern char getc(FILE *),*strcpy(char *,char *);
extern Point collist(Point,Wire *);
extern Rectangle mbblist(List *);
extern List *editlist(List *),*cut(List **),*copy(List *);
extern Hit textcode(Point);
#endif
