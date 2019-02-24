
#include <jerq.h>
#include <jerqio.h>
#include <font.h>

Rectangle nullrect = {0,0,0,0};

int hex(ch)
{
   if ((ch>='0') && (ch<='9')) return(ch-'0');
   if ((ch>='A') && (ch<='F')) return(10+ch-'A');
   if ((ch>='a') && (ch<='f')) return(10+ch-'a');
   return(0);
}

int scanhex(fp)
FILE *fp;
{
   int ch;
   int res;
   while((ch=getc(fp)) != '0');
   getc(fp); /* 'x' */
   res = 4096*hex(getc(fp));
   res += 256*hex(getc(fp));
   res += 16*hex(getc(fp));
   res += hex(getc(fp));
   return(res);
}

putnibble(ch,b,clipr,p)
int ch;
Bitmap *b;
Rectangle clipr;
Point p;
{
   int nibble;
   if (ch<='9') nibble = ch - '0';
   else if (ch<='F') nibble = 10 + (ch - 'A');
   else nibble = 10 + (ch - 'a');
   if (nibble/8 == 1) {
     if (ptinrect(p,clipr)) point(b,p,F_OR);
     nibble = nibble%8;
   }
   p.x += 1;
   if (nibble/4 == 1) {
     if (ptinrect(p,clipr)) point(b,p,F_OR);
     nibble = nibble%4;
   }
   p.x += 1;
   if (nibble/2 == 1) {
     if (ptinrect(p,clipr)) point(b,p,F_OR);
     nibble = nibble%2;
   }
   p.x += 1;
   if (nibble == 1) 
     if (ptinrect(p,clipr)) point(b,p,F_OR);
}

Rectangle OpLoad(bitmap,filename)
Bitmap *bitmap;
char *filename;
{
   FILE *fp;
   Rectangle rect;
   int ch,i,j;
   int xsize,ysize;
   rect = bitmap->rect;
   fp = fopen(filename,"r");
   if (fp == ((FILE *) 0)) return(nullrect);
   ch = getc(fp);
   if (((ch>='0')&&(ch<='9')) || ((ch>='A')&&(ch<='F')) || ((ch>='a')&&(ch<='f'))) {
     if (((ch=getc(fp))!='x') && (ch!='X')) {
       /* old format */
       fclose(fp); fp = fopen(filename,"r");
       if (fp == ((FILE *) 0)) return(nullrect);
       i = rect.origin.x; j = rect.origin.y;
       xsize = 0;
       while ((ch=getc(fp))!=EOF) {
         if (((ch>='0')&&(ch<='9')) || ((ch>='A')&&(ch<='F')) || ((ch>='a')&&(ch<='f'))) {
           putnibble(ch,bitmap,rect,Pt(i,j));
           i = i+4;
         } else if (ch=='\n') {
           xsize = max(xsize,i-rect.origin.x);
           i = rect.origin.x;
           j++;
         } else break;
       }
       ysize = j-rect.origin.y;
     } else {
       i = rect.origin.x; j = rect.origin.y;
       xsize = 0;
       for (;;) {
         putnibble(getc(fp),bitmap,rect,Pt(i,j)); i+=4;
         putnibble(getc(fp),bitmap,rect,Pt(i,j)); i+=4;
         putnibble(getc(fp),bitmap,rect,Pt(i,j)); i+=4;
         putnibble(getc(fp),bitmap,rect,Pt(i,j)); i+=4;
         getc(fp); /* ',' */
         ch = getc(fp);
         if (ch=='0') {
           getc(fp); /* 'x' */
         } else if (ch=='\n') {
           xsize = max(xsize,i-rect.origin.x);
           i = rect.origin.x;
           j++;
           getc(fp); /* '0' */
           getc(fp); /* 'x' */
         } else break;
       }
       ysize = j-rect.origin.y;
     }
   }
   fclose(fp);
   rect.origin.x = 0; rect.origin.y = 0; 
   rect.corner.x = xsize; rect.corner.y = ysize;
   return(rect);
}

Point p;
int maxy;
Bitmap *b;

Setup()
{
   request(KBD|MOUSE);
   rectf(&display,Drect,F_CLR);
   p = add(Drect.origin,Pt(16,16));
   b = balloc(Rect(0,0,200,200));
   maxy = 0;
}

PrintIcon(Name)
char *Name;
{
   int ch;
   Point size;
   Rectangle r;
   if ((Name[0]>='A') && (Name[0]<='Z')) return(0);
   rectf(b,b->rect,F_CLR);
   r = OpLoad(b,Name);
   if (eqrect(r,nullrect)) return(0);
   size = sub(r.corner,r.origin);
   if ((p.x+size.x+16 > Drect.corner.x) && (p.x != Drect.origin.x+16)) {
     p.x = Drect.origin.x+16;
     p.y += maxy+16;
     maxy = 0;
   }
   if ((p.y+size.y+16 > Drect.corner.y) && (p.y != Drect.origin.y+16)) {
     wait(KBD); ch = kbdchar();
     rectf(&display,Drect,F_CLR);
     p = add(Drect.origin,Pt(16,16));
   }
   bitblt(b,r,&display,p,F_XOR);
   p.x += 16+size.x;
   if (size.y > maxy) maxy = size.y;
}

main(argc, argv)
char *argv[];
{
   Setup();
   while(argc>1){
      PrintIcon(argv[1]);
      --argc; argv++;
   }
   wait(KBD);
   exit();
}
