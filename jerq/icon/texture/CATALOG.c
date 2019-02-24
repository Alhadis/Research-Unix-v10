
#include <jerq.h>
#include <jerqio.h>
#include <font.h>

int hex(ch)
{
   if ((ch>='0') && (ch<='9')) return(ch-'0');
   return(10+ch-'A');
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
   if (ch==EOF) for (i=0; i<16; i++) tex->bits[i] = 0;
   else for (i=0; i<16; i++) {tex->bits[i] = scanhex(fp); getc(fp);}
   fclose(fp);
}

Point p;
Bitmap *b;

Setup()
{
   request(KBD|MOUSE);
   rectf(&display,Drect,F_CLR);
   p = add(Drect.origin,Pt(16,16));
   b = balloc(Rect(0,0,48,48));
}

PrintIcon(Name)
char *Name;
{
   Texture t;
   int ch;
   if ((Name[0]>='A') && (Name[0]<='Z')) return(0);
   OpLoad(&t,Name);
   texture(b,b->rect,&t,F_STORE);
   bitblt(b,b->rect,&display,p,F_XOR);
   p.x += 64;
   if (p.x > (Drect.corner.x-64)) {
     p.x = Drect.origin.x+16;
     p.y += 64;
   }
   if (p.y > (Drect.corner.y-64)) {
     wait(KBD); ch = kbdchar();
     rectf(&display,Drect,F_CLR);
     p = add(Drect.origin,Pt(16,16));
   }
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
