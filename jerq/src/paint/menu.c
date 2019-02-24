#include "paint.h"
/*
 * Line style sub-submenu
 */
void Olinestyle();
NMitem Ilinestyle[]={
"*A",		"",	0,	Olinestyle,
" A.",		"",	0,	Olinestyle,
" A..",		"",	0,	Olinestyle,
" A...",	"",	0,	Olinestyle,
" AA.",		"",	0,	Olinestyle,
" AA..",	"",	0,	Olinestyle,
" AA...",	"",	0,	Olinestyle,
" AAA...",	"",	0,	Olinestyle,
" AAAA....",	"",	0,	Olinestyle,
" AB",		"",	0,	Olinestyle,
" A.B.",	"",	0,	Olinestyle,
" A..B..",	"",	0,	Olinestyle,
" A...B...",	"",	0,	Olinestyle,
0
};
NMenu Mlinestyle={Ilinestyle};
int curlinestyle=0;
char *linestring="A";
/*
 * Style submenu
 */
void Ostyle();
NMitem Istyle[]={
"*Paint\240",	"Multiple brushspots while you hold button 1",	0,	Ostyle,
" Circles\240",	"Rubberband circles",				0,	Ostyle,
" Lines\240",	"Rubberband brush lines",			0,	Ostyle,
" Curves\240",	"Continuous strokes while you hold button 1",	0,	Ostyle,
" Line style\240", "Solid, dotted, dashed, etc. lines",		&Mlinestyle, 0,
0
};
int Fpaint(), Fcircles(), Flines(), Fcurves();
int curstyle=0;
int (*styles[])()={
	Fpaint,
	Fcircles,
	Flines,
	Fcurves
};
NMenu Mstyle={Istyle};
/*
 * Operation submenu
 */
void Ooperation();
NMitem Ioperation[]={
"*Above/Erase\240", "Button 1 paints on top, Button 2 erases",	0,	Ooperation,
" Below/Erase\240", "Button 1 paints behind, Button 2 erases",	0,	Ooperation,
" Above/Below\240", "Button 1 paints on top, Button 2 behind",	0,	Ooperation,
" Inside/Erase\240","Button 1 paints inside, Button 2 erases",	0,	Ooperation,
" Brush/Clear\240", "Special effects A, CLEAR",			0,	Ooperation,
" AoutB/AinB\240",  "Special effects",				0,	Ooperation,
" BinA/BatopA\240", "Special effects",				0,	Ooperation,
" Xor/Above\240",   "Special effects",				0,	Ooperation,
0
};
int curop=0;
int ops[][2]={
	C_AoverB, C_BoutA,
	C_BoverA, C_BoutA,
	C_AoverB, C_BoverA,
	C_AatopB, C_BoutA,
	C_A,      C_CLEAR,
	C_AoutB,  C_AinB,
	C_BinA,   C_BatopA,
	C_AxorB,  C_AoverB,
};
NMenu Moperation={Ioperation};
/*
 * Texture library submenu
 */
NMitem *Gtlib();
NMenu Mtlib={0, Gtlib};
/*
 * Texture submenu
 */
void Otread(), Otexture(), Onegtexture(), Otlib(), Omktex(), Otwrite();
NMitem Itexture[]={
" Texture\240",	"Turn texturing on or off",			0,	Otexture,
" Make\240",	"Pick a texture from the picture",		0,	Omktex,
" Negate\240",	"Reverse the texture's green and black",	0,	Onegtexture,
" Save\240",	"Copy a texture into a file",			0,	Otwrite,
" Library\240",	"Get a texture from the texture library",	&Mtlib,	Otlib,
" Get\240",	"Type a name and get a texture from a file",	0,	Otread,
0
};
NMenu Mtexture={Itexture};
/*
 * Picture library submenu
 */
NMitem *Glib();
NMenu Mlib={0, Glib};
/*
 * Image submenu
 */
void Onegimage(), Oread(), Owrite(), Olib();
NMitem Icanvas[]={
"Negate\240",	"Reverse the picture's green and black",	0,	Onegimage,
"Save\240",	"Copy the picture into a file",			0,	Owrite,
"Library\240",	"Get a picture from the gallery",		&Mlib,	Olib,
"Get\240",	"Type a name and get a picture from a file",	0,	Oread,
0
};
NMenu Mcanvas={Icanvas};
/*
 * Brush library submenu
 */
NMitem *Gblib();
NMenu Mblib={0, Gblib};
/*
 * Brush submenu
 */
void Omkbrush(), Onegbrush(), Obread(), Obwrite(), Oblib();
NMitem Ibrush[]={
"Make\240",	"Turn part of the picture into a new brush",	0,	Omkbrush,
"Negate\240",	"Reverse the brush's green and black",		0,	Onegbrush,
"Save\240",	"Copy the brush into a file",			0,	Obwrite,
"Library\240",	"Get a brush from the brush library",		&Mblib,	Oblib,
"Get\240",	"Type a name and get a brush from a file",	0,	Obread,
0
};
NMenu Mbrush={Ibrush};
/*
 * state submenu
 */
void Obrushdir(), Opicdir(), Otexdir();
NMitem Istate[]={
"Brushes\240",	"Name the brush directory",			0,	Obrushdir,
"Pictures\240",	"Name the picture directory",			0,	Opicdir,
"Textures\240",	"Name the texture directory",			0,	Otexdir,
0
};
NMenu Mstate={Istate};
/*
 * Main menu
 */
void Otexture(), Ogreen(), Oalphavis(), Oexit(), Ofill();
NMitem Imain[]={
" Style\240",	"Different kinds of brushstrokes",	&Mstyle,	0,
" Operation\240","Ways of putting paint on canvas",	&Moperation,	0,
" Texture\240",	"Things to do to the texture pattern",	&Mtexture,	0,
" Brush\240",	"Things to do to the paintbrush",	&Mbrush,	0,
" Canvas\240",	"Things to do to the whole picture",	&Mcanvas,	0,
" State\240",	"Change things saved in .paintstate",	&Mstate,	0,
" Fill\240",	"Fill an area of the picture",		0,		Ofill,
" Green\240",	"Erase the entire picture",		0,		Ogreen,
" Mask\240",	"Look at the picture mask",		0,		Oalphavis,
" Exit\240",	"Leave the paint program",		0,		Oexit,
	0
};
NMenu Mmain={Imain};
void Ooperation(i) NMitem *i;{setop(i-Ioperation);}
setop(i){
	Ioperation[curop].text[0]=' ';
	curop=i;
	Ioperation[curop].text[0]='*';
}
void Olinestyle(i) NMitem *i;{setlinestyle(i-Ilinestyle);}
setlinestyle(i){
	Ilinestyle[curlinestyle].text[0]=' ';
	curlinestyle=i;
	linestring=Ilinestyle[curlinestyle].text+1;
	Ilinestyle[curlinestyle].text[0]='*';
}
void Ostyle(i) NMitem *i;{setstyle(i-Istyle);}
setstyle(i){
	Istyle[curstyle].text[0]=' ';
	curstyle=i;
	Istyle[curstyle].text[0]='*';
}
void Otexture(i)
NMitem *i;
{
	settex(!textured);
}
settex(i){
	textured=i;
	Itexture[0].text[0]=textured?'*':' ';
}
Texture sunset={
	0x0140,0x0948,0x2548,0x1010,
	0x07C0,0x4824,0x1010,0x1010,
	0x7FFE,0x783E,0xFCFC,0xF078,
	0x0002,0xA504,0x4228,0xA510,
};
void Oexit(){
	Texture *t=cursswitch(&sunset);
	while(!button123())
		wait(CPU);
	if(button3()){
		writestate();
		exit(0);
	}
	while(button123())
		wait(CPU);
	cursswitch(t);
}
Texture delta={
0x0000,0x0000,0x0000,0x0000,
0x0000,0x0000,0x0000,0x0000,
0x0080,0x01C0,0x03E0,0x07F0,
0x0FF8,0x1FFC,0x3FFE,0x7FFF,
};
void Ofill(){
	Point p;
	Texture *t=cursswitch(&delta);
	while(!button123())
		wait(CPU);
	if(button3()){
		p=mouse.xy;
		cursswitch(t);
		while(button123())
			wait(CPU);
		cursswitch(t);
		texfill(canvas, p);
	}
	else{
		while(button123())
			wait(CPU);
		cursswitch(t);
	}
}
Texture confirm = {
	 0x000E, 0x071F, 0x0317, 0x736F,
	 0xFBCE, 0xDB8C, 0xDBC0, 0xFB6C,
	 0x77FC, 0x0000, 0x0001, 0x0003,
	 0x94A6, 0x633C, 0x6318, 0x9490,
};
void Ogreen(){
	Texture *t=cursswitch(&confirm);
	while(!button123())
		wait(CPU);
	if(button3()){
		rectf(canvas->grn, canvas->rect, F_CLR);
		rectf(canvas->alf, canvas->rect, F_CLR);
	}
	while(button123())
		wait(CPU);
	cursswitch(t);
}
Point brushcen;
void Omkbrush(){
	Rectangle r;
	if(pickrect(&r)){
		ifree(brush);
		brushcen=div(sub(r.co, r.or), 2);
		brush=ialloc(r);		/* What if failure? */
		composite(canvas, r, brush, r.or, C_A);
	}
}
void Onegbrush(){
	bitblt(brush->alf, brush->rect, brush->grn, brush->rect.or, F_XOR);
}
void Oalphavis(i)
NMitem *i;
{
	Bitmap *t;
	i->text[0]=i->text[0]==' '?'*':' ';
	bitblt(canvas->grn, canvas->rect, canvas->alf, canvas->rect.or, F_XOR);
	bitblt(canvas->alf, canvas->rect, canvas->grn, canvas->rect.or, F_XOR);
	bitblt(canvas->grn, canvas->rect, canvas->alf, canvas->rect.or, F_XOR);
	t=canvas->grn;
	canvas->grn=canvas->alf;
	canvas->alf=t;
}
void Oread(){
	picread(getfname(picdir));
}
void Owrite(){
	iwrite(canvas, getfname(picdir));
}
void Onegimage(){
	bitblt(canvas->alf, canvas->rect, canvas->grn, canvas->rect.or, F_XOR);
}
void Obread(){
	brushread(getfname(brushdir));
}
int getbrname=1;
void Obwrite(){
	strcpy(brushname, getfname(brushdir));
	iwrite(brush, brushname);
	getbrname=1;
}
void Otread(){
	rdtexture(&tex, getfname(texdir));
}
int gettexname=1;
void Otwrite(){
	wrtexture(&tex, getfname(texdir));
	gettexname=1;
}
void Onegtexture(){
	register i;
	for(i=0;i!=16;i++)
		tex.bits[i]^=-1;
}
Texture box={
	0xFFFF, 0x8001, 0x8001, 0x8001,
	0x8001, 0x8001, 0x8001, 0x8001,
	0x8001, 0x8001, 0x8001, 0x8001,
	0x8001, 0x8001, 0x8001, 0xFFFF,
};
/*
 * Not very portable
 */
void Omktex(){
	Texture *t=cursswitch(&box);
	Bitmap *b=balloc(Rect(0, 0, 16, 16));
	register i;
	Point p;
	if(b){
		while(!button123())
			wait(CPU);
		if(button3()){
			p=sub(mouse.xy, Pt(8, 8));
			bitblt(canvas->grn, Rpt(p, add(p, Pt(16, 16))),
				b, Pt(0, 0), F_STORE);
			for(i=0;i!=16;i++)
				tex.bits[(i+p.y)&15]=rot((b->base[i]>>16)&0xFFFF,
					p.x&15);
			inittex();
		}
		bfree(b);
		while(button123())
			wait(CPU);
	}
	cursswitch(t);
}
rot(v, n){
	return(((v>>n)|(v<<(16-n)))&0xFFFF);
}
void Opicdir(){
	getline(picdir, "Picture directory: ");
}
void Obrushdir(){
	getline(brushdir, "Brush directory: ");
	getbrname=1;
}
void Otexdir(){
	getline(texdir, "Texture directory: ");
	gettexname=1;
}
/*
 * Picture library routines
 */
#define	NFILENAME	100
char picname[NFILENAME][15];
int npicname;
NMitem libitem;
void Olib(){
	npicname=getdir(picdir, picname);
}
void Olibpick(){
	char buf[512];
	sprintf(buf, "%s/%s", picdir, libitem.text);
	picread(buf);
}
NMitem *Glib(n, t)
NMitem t[];
{
	if(n<0 || npicname<=n)
		libitem.text=0;
	else
		libitem.text=picname[n];
	libitem.help="";
	libitem.next=0;
	libitem.fn=Olibpick;
	return(&libitem);
}
/*
 * Brush library routines
 */
char brushes[NFILENAME][15];
int nbrushes;
void Oblib(){
	if(getbrname){
		nbrushes=getdir(brushdir, brushes);
		getbrname=0;
	}
}
void Oblibpick(){
	char buf[512];
	sprintf(buf, "%s/%s", brushdir, libitem.text);
	brushread(buf);
}
NMitem *Gblib(n, t)
NMitem t[];
{
	if(n<0 || nbrushes<=n)
		libitem.text=0;
	else
		libitem.text=brushes[n];
	libitem.help="";
	libitem.next=0;
	libitem.fn=Oblibpick;
	return(&libitem);
}
/*
 * Texture library routines
 */
char texname[NFILENAME][15];
int ntexname;
void Otlib(){
	if(gettexname){
		ntexname=getdir(texdir, texname);
		gettexname=0;
	}
}
void Otlibpick(){
	char buf[512];
	sprintf(buf, "%s/%s", texdir, libitem.text);
	rdtexture(&tex, buf);
}
NMitem *Gtlib(n, t)
NMitem t[];
{
	if(n<0 || ntexname<=n)
		libitem.text=0;
	else
		libitem.text=texname[n];
	libitem.help="";
	libitem.next=0;
	libitem.fn=Otlibpick;
	return(&libitem);
}
getdir(d, filename)
char *d;
char filename[][15];
{
	extern strcmp();
	struct{
		short inum;
		char name[14];
	}ent;
	register FILE *f;
	register nfilename;
	if((f=fopen(d, "r"))==0)
		return 0;
	for(nfilename=0;nfilename!=NFILENAME && fread(&ent, sizeof ent, 1, f)==1;){
		if(ent.inum
		&& ent.name[0]!='.'
		&& (ent.name[0]<'A' || 'Z'<ent.name[0])){
			strncpy(filename[nfilename], ent.name, sizeof ent.name);
			filename[nfilename][sizeof ent.name]='\0';
			nfilename++;
		}
	}
	fclose(f);
	qsort(filename, nfilename, sizeof(filename[0]), strcmp);
	return nfilename;
}
