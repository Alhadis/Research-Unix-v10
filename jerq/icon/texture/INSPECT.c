
#include <jerq.h>
#include <jerqio.h>
#include <font.h>


char FNAME[50];

getstr(s,p)
char *s;
Point p;
{
   char c,*t;
   static char str[]="x";
   t = s;
   for (;;) {
     wait(KBD);
     if (((c=kbdchar()) == '\r') || (c == '\n')) {
       *s = '\0';
       return;
     }
     if (c == '\b') {
       if (s>t) {
         str[0] = *(--s);
         string(&defont,str,&display,(p = sub(p,Pt(9,0))),F_XOR);
       }
     } else {
       if (s-t<50) {
         *s++ = (str[0] = c);
         p = string(&defont,str,&display,p,F_XOR);
       }
     }
   }
}

GetFNAME()
{
   Point p;
   p = string(&defont,"Texture: ",&display,add(Drect.origin,Pt(15,5)),F_XOR);
   getstr(FNAME,p);
   p = string(&defont,"Texture: ",&display,add(Drect.origin,Pt(15,5)),F_XOR);
   string(&defont,FNAME,&display,p,F_XOR);
}

int hex(ch)
{
   if ((ch>='0') && (ch<='9')) return(ch-'0');
   return(10+ch-'A');
}

int scanhex(fp)
FILE *fp;
{
   int ch;
   while((ch=getc(fp)) != '0');
   getc(fp); /* 'x' */
   return(4096*hex(getc(fp))+256*hex(getc(fp))+16*hex(getc(fp))+hex(getc(fp)));
}

OpLoad(tex,filename)
Texture *tex;
char *filename;
{
   FILE *fp;
   int ch,i,j;
   fp = fopen(filename,"r");
   if (fp == ((FILE *) 0)) return(0);
   ch = getc(fp);
   while ((ch!='{')&&(ch!=EOF)) ch=getc(fp);
   for (i=0; i<16; i++) {tex->bits[i] = scanhex(fp); getc(fp);}
   fclose(fp);
}

OpRead(tex)
Texture *tex;
{
   GetFNAME();
   if (!FNAME[0]) return(0);
   OpLoad(tex,FNAME);
}

main()
{
   Texture t;
   request(KBD|MOUSE);
   while (!button123()) {
     OpRead(&t);
     rectf(&display,Drect,F_CLR);
     texture(&display,Rpt(Pt(Drect.origin.x,Drect.origin.y+32),Drect.corner),
             &t,F_XOR);
   }
   exit();
}
