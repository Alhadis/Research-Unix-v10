#include "jf.h"

char *file_menu[]={
	"make new font",
	"read font",
	"squeeze font",
	"write font",
	"delete font",
	"write icon",
	NULL
};
Menu filemenu={ file_menu };

filefunc()		/* menu for file operations */
{
	switch (menuhit3(&filemenu)) {
		case 0:
			newfont(); break;
		case 1:
			readfile(CNULL); break;
		case 2:
			sqdfont(0); break;
		case 3:
			writefile(); break;
		case 4:
			sqdfont(1); break;
		case 5:
			writeicon(); break;
	}
}

#define NEWSIZE	16	/* size of empty font */
#define NEWNCH	16	/* number of characters in empty font */

newfont()	/* create empty font */
{
	register Font *fp; register Fontchar *i; Fontdisp *fdp, *newfdp();

	if ((fdp=newfdp()) == FDNULL) return 0;

	fp=(Font *)alloc(sizeof(Font) + NEWNCH*sizeof(Fontchar));
	if (fp == FNULL) return 0;

	fp->bits=balloc(Rect(0,0,1,NEWSIZE));
	if (fp->bits == (Bitmap *)0) { free((char *)fp); return 0; }

	rectf(fp->bits,fp->bits->rect,F_CLR);
	fp->n=NEWNCH; fp->height=NEWSIZE; fp->ascent=NEWSIZE;
	for (i=fp->info; i<=&fp->info[fp->n]; i++) {
		i->x = 1;
		i->top = 0;
		i->bottom = 0;
		i->left = 0;
		i->width = 0;
	}
	(--i)->width = NEWSIZE;
	fp->info[0].x = 0;

	fdp->fp=fp; fdp->filnam=CNULL;
	spaceman(); return 1;
}

#define NSTR	50	/* maximum length of font file name */

readfile(astr)	/* load font from file */
char *astr;
{
	static char fpath[]="/usr/jerq/font/"; char str[NSTR], *strcat2();
	int nchars; Point p; Font *fp; Fontdisp *fdp, *newfdp();

	if ((fdp=newfdp()) == FDNULL) return 0;

	if (astr == CNULL ) {
		drstore(rkbd);
		p=drstring("File: ",pkbd);
		nchars=kbdstring(str,NSTR-sizeof(fpath),p);
	} else {
		strncpy(str,astr,NSTR-sizeof(fpath));
		nchars=strlen(str);
	}
	if (nchars <= 0) { drclr(rkbd); return 0; }

	cursswitch(&deadmouse);
	if ((fp=getfont(str))==FNULL && (str[0]!='/')
	 && (fp=getfont(strcat2(fpath,str)))==FNULL) {
		drstore(rkbd);
		drstring("Font load failed.",pkbd);
		return 0;
	}

	fdp->fp=fp;
	if ((fdp->filnam=alloc(strlen(str))) != CNULL) strcpy(fdp->filnam,str);

	spaceman();
	drstore(rkbd);
	p=drstring("Loaded ",pkbd); drstring(str,p);

	return 1;
}

writefile()		/* write font to file */
{
	Fontdisp *fdp=FDNULL; Editdisp *edp; Font *fp; Fontchar *i;
	Point p; Rectangle r, charect(), rsupport();
	int c, nchars, fouch(), samename=0; char str[NSTR];

	cursswitch(&target);
	if (buttons(DOWN) == 3) { mousetrack(); fdp=mtk.fdp; }
	buttons(UP);
	if (fdp == FDNULL) { return 0; }

	fp=fdp->fp;
	for (edp=fdp->edp; edp!=EDNULL; edp=edp->edp) {
		c=edp->c; i=fp->info+c;
		r=rsupport(fp->bits,charect(fp,c),(Bitmap *)0);
		i->top=r.origin.y; i->bottom=r.corner.y;
	}

	drstore(rkbd);
	if (fdp->filnam != CNULL) {
		p=drstring("File (",pkbd);
		p=drstring(fdp->filnam,p);
		p=drstring(") : ",p);
	} else {
		p=drstring("File: ",pkbd);
	}

	nchars=kbdstring(str,NSTR,p);
	if (nchars <= 0) {
		if (fdp->filnam == CNULL) { drclr(rkbd); return 0; }
		samename=1;
		strcpy(str,fdp->filnam);
	}

	cursswitch(&deadmouse);
	filep=fopen(str,"w"); c=-1;
	if (filep != (FILE *)0) {
		c=outfont(fdp->fp,fouch);
		fclose(filep);
	}

	drstore(rkbd);
	if (c<0) {
		drstring("Font write failed.",pkbd);
		return 0;
	} else {
		p=drstring("Wrote ",pkbd); drstring(str,p);
		if (!samename) {
			if (fdp->filnam != CNULL) free(fdp->filnam);
			fdp->filnam=alloc(strlen(str));
			if (fdp->filnam != CNULL) strcpy(fdp->filnam,str);
		}
		return 1;
	}
}

fouch(c)		/* write single character to font file */
int c;
{
	return(putc(c,filep));
}

sqdfont(icode)	/* squeeze or delete font */
int icode;
{
	Fontdisp *fdp=FDNULL; Editdisp *edp;
	cursswitch(&target);
	if (buttons(DOWN) == 3) { mousetrack(); fdp=mtk.fdp; }
	buttons(UP);
	if (fdp == FDNULL) return 0;

	for (edp=fdp->edp; edp != EDNULL; edp=edp->edp) edp->fdp=FDNULL;
	fdp->edp=EDNULL;

	if (icode) {
		ffree(fdp->fp);
		fdp->fp=FNULL;
		if (fdp->filnam != CNULL) free(fdp->filnam);
	} else {
		icode = fontprune(fdp->fp);
	}

	spaceman();
	return icode;
}

writeicon()		/* write Texture declaration */
{
	Editdisp *edp; Font *fp; Fontchar *ich; char str[NSTR];
	register Bitmap *tb; register i;

	if ((tb = balloc(Rect(0,0,16,16))) == 0) return 0;

	cursswitch(&target);
	if (buttons(DOWN) == 3) { mousetrack(); edp=mtk.edp; }
	buttons(UP);
	if (edp == EDNULL) { bfree(tb); return 0; }

	fp = edp->fdp->fp; ich = fp->info + edp->c;

	drstore(rkbd);
	if (kbdstring(str,NSTR,drstring("Texture: ",pkbd)) <= 0) {
		bfree(tb); return 0;
	}

	cursswitch(&deadmouse);
	filep=fopen(str,"w");
	if (filep != (FILE *)0) {
		bitblt(fp->bits, Rect(ich->x, 0, (ich+1)->x, fp->height),
		    tb, Pt(0,0), F_STORE);
		fprintf(filep,"Texture %s = {\n", str);
		for (i=0; i<16; i++) {
			fprintf(filep, "%c0x%x,",
			    " \t"[(i&7) == 0], tb->base[i]>>(WORDSIZE-16));
			if (i == 7) fprintf(filep, "\n");
		}
		fprintf(filep, "\n};\n");
		fclose(filep);
		drstore(rkbd);
		drstring(str,drstring("Wrote ",pkbd));
		bfree(tb); return 1;
	} else {
		drstore(rkbd);
		drstring("Texture write failed.",pkbd);
		bfree(tb); return 0;
	}
}
