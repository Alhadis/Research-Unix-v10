#include "paint.h"
pickrect(rp)
register Rectangle *rp;
{
	*rp=getrect3();
	if(rp->corner.x && rp->corner.x-rp->origin.x<=1
	&& rp->corner.y-rp->origin.y<=1)
		*rp=canvas->rect;
	return(rectclip(rp, canvas->rect)
	   && rp->corner.x-rp->origin.x>0 && rp->corner.y-rp->origin.y>0);
}
Bitmap *msgsave;
Rectangle msgrect;
msg(s)
char *s;
{
	if(msgsave)
		bfree(msgsave);
	msgsave=balloc(msgrect);
	if(msgsave)
		bitblt(&display, msgrect, msgsave, msgrect.or, F_STORE);
	rectf(&display, msgrect, F_CLR);
	nmsg(s);
}
nmsg(s)
char *s;
{
	rectf(&display, Rpt(string(&defont, s, &display, add(msgrect.or, Pt(5, 3)),
					F_STORE), msgrect.co), F_CLR);
}
unmsg(){
	if(msgsave)
		bitblt(msgsave, msgrect, &display, msgrect.or, F_STORE);
}
pause(m)
char *m;
{
	msg(m);
	while(!button123())
		wait(CPU);
	while(button123())
		wait(CPU);
	unmsg(m);
}
idchar(c)
register c;
{
	return('a'<=c && c<='z' || 'A'<=c && c<='Z' || '0'<=c && c<='9' || c=='_');
}
getline(buf, prompt)
char buf[];
char *prompt;
{
	register int c;
	register char *s=buf+strlen(buf);
	char line[512];
	sprintf(line, "%s%s\1", prompt, buf);
	msg(line);
	wait(KBD);
	while((c=kbdchar())!='\n' && c!='\r'){
		switch(c){
		case -1:
			continue;
		case '\b':
			if(s!=buf)
				*--s='\0';
			break;
		case 027:	/* ctrl-w */
			while(s!=buf && !idchar(s[-1]))
				--s;
			while(s!=buf && idchar(s[-1]))
				--s;
			*s='\0';
			break;
		case '@':
			s=buf;
			*s='\0';
			break;
		default:
			*s++=c;
			*s='\0';
			break;
		}
		sprintf(line, "%s%s\1 ", prompt, buf);
		nmsg(line);
	}
	unmsg();
}
char *getfname(dir)
char *dir;
{
	static char buf[512];
	if(dir && dir[0])
		sprintf(buf, "%s/", dir);
	else
		buf[0]='\0';
	getline(buf, "file name? ");
	return(buf);
}
Texture dead={
	 0x0000, 0x0114, 0xA208, 0x4100,
	 0x0000, 0x0008, 0x0004, 0x0082,
	 0x0441, 0xFFE1, 0x5FF1, 0x3FFE,
	 0x17F0, 0x03E0, 0x0000, 0x0000,
};
/*
 * write the image in a file
 */
iwrite(im, name)
Image *im;
char *name;
{
	register FILE *f;
	Texture *t;
	if((f=fopen(name, "w"))==0){
		msg("Can't create file");
		sleep(60);
		unmsg();
		return;
	}
	t=cursswitch(&dead);
	putbitmap(im->grn, im->rect, f, &display);
	putbitmap(im->alf, im->rect, f, &display);
	cursswitch(t);
	fclose(f);
}
/*
 * Read an image from a file
 */
Image *iread(name)
char *name;
{
	register FILE *f;
	Texture *t;
	Image *im=(Image *)alloc(sizeof(Image));
	if(im==0)
		return(0);
	if((f=fopen(name, "r"))==0){
		msg("Can't open file");
		sleep(60);
		unmsg();
		return(0);
	}
	t=cursswitch(&dead);
	im->grn=getbitmap(f, &display);
	if(im->grn==0){
		free(im);
		return(0);
	}
	im->alf=getbitmap(f, &display);
	if(im->alf==0){
		bfree(im->grn);
		free(im);
		return(0);
	}
	im->rect=im->grn->rect;
	cursswitch(t);
	fclose(f);
	return(im);
}
Image *texmap=0;
inittex(){
/*	if(!texmap){
		texmap=ialloc(canvas->rect);
		if(!texmap){
			pause("Can't allocate texture");
			exit(1);
		}
	}
	texture(texmap->grn, texmap->rect, &tex, F_STORE);
	rectf(texmap->alf, texmap->rect, F_CLR);
*/
}
/*
 * texcomp is like texture, except that op is a compositing operator,
 * and alf is the alpha channel to mask the texture with.  The upper left corner of
 * alf is used as the mask.
 */
texcomp(b, r, t, alf, op)
Image *b;
Rectangle r;
Texture *t;
Bitmap *alf;
{
	Image *i=ialloc(r);
	if(i){
		texture(i->grn, r, t, F_STORE);
		rectf(alf, alf->rect, F_XOR);
		bitblt(alf, alf->rect, i->grn, r.or, F_CLR);
		rectf(alf, alf->rect, F_XOR);
		bitblt(alf, alf->rect, i->alf, r.or, F_STORE);
		composite(i, r, b, r.or, op);
		ifree(i);
	}
}
/*
 * Read a texture from a file
 */
rdtexture(tex, name)
Texture *tex;
char *name;
{
	register FILE *f;
	Texture *t;
	if((f=fopen(name, "r"))==0){
		msg("Can't open file");
		sleep(60);
		unmsg();
		return;
	}
	t=cursswitch(&dead);
	rdtex(f, tex);
	fclose(f);
	inittex();
	cursswitch(t);
}
atotex(t, s)
register Texture *t;
register char *s;
{
	register i;
	for(i=0;i!=16 && *s;i++){
		t->bits[i]=htob(*s++)<<12;
		if(*s=='\0')
			break;
		t->bits[i]+=htob(*s++)<<8;
		if(*s=='\0')
			break;
		t->bits[i]+=htob(*s++)<<4;
		if(*s=='\0')
			break;
		t->bits[i]+=htob(*s++);
	}
}
rdtex(f, t)
FILE *f;
Texture *t;
{
	char buf[512];
	if(fgets(buf, sizeof buf, f))
		atotex(t, buf);
}
/*
 * Write a texture to a file
 */
wrtexture(tex, name)
Texture *tex;
char *name;
{
	register FILE *f;
	Texture *t;
	if((f=fopen(name, "w"))==0){
		msg("Can't create file");
		sleep(60);
		unmsg();
		return;
	}
	t=cursswitch(&dead);
	wrtex(f, tex);
	fclose(f);
	cursswitch(t);
}
wrtex(f, t)
FILE *f;
register Texture *t;
{
	static char hexchars[]="0123456789ABCDEF";
	register i;
	for(i=0;i!=16;i++){
		putc(hexchars[(t->bits[i]>>12)&15], f);
		putc(hexchars[(t->bits[i]>>8)&15], f);
		putc(hexchars[(t->bits[i]>>4)&15], f);
		putc(hexchars[t->bits[i]&15], f);
	}
	putc('\n', f);
}
htob(c){
	if('0'<=c && c<='9') return(c-'0');
	if('a'<=c && c<='f') return(c-'a'+10);
	if('A'<=c && c<='F') return(c-'A'+10);
	return(0);
}
brushread(name)
char *name;
{
	Image *im=iread(name);
	if(im){
		if(brush)
			ifree(brush);
		brush=im;
		brushcen=div(sub(brush->rect.co, brush->rect.or), 2);
		strcpy(brushname, name);
	}
}
picread(name)
char *name;
{
	register Image *im=iread(name);
	if(im){
		composite(im, im->rect, canvas, canvas->rect.or, C_A);
		ifree(im);
	}
}
