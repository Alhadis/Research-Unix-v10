
#include <jerq.h>
#include <jerqio.h>
#include <font.h>

typedef struct Icon
{
   Point Where;
   char *Which;
   struct Icon *Next;
} Icon;

struct Icon *IconList = 0;

NewIcon(Where,Which)
Point Where;
char *Which;
{
   Icon *New;
   New = (Icon *)alloc(sizeof(Icon));
   if (New==0) return(0);
   New->Where = Where;
   New->Which = Which;
   New->Next = IconList;
   IconList = New;
}

char *IconAt(p)
Point p;
{
   Icon *Ic;
   for (Ic = IconList; Ic != 0; Ic = Ic->Next) {
     if ((p.x>=Ic->Where.x) && (p.y>=Ic->Where.y) &&
         (p.x<=Ic->Where.x+16) && (p.y<=Ic->Where.y+16))
       return(Ic->Which);
   }
   return("");
}

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

Point p,NamePoint;
Bitmap *b;

Setup()
{
   request(KBD|MOUSE);
   rectf(&display,Drect,F_CLR);
   p = add(Drect.origin,Pt(16,32));
   NamePoint = add(Drect.origin,Pt(16,12));
   b = balloc(Rect(0,0,16,16));
}

ShowName()
{
   char *str;
   if (button1()) {
     str = IconAt(mouse.xy);
     string(&defont,"point at an icon to see its name",&display,NamePoint,F_XOR);
     string(&defont,str,&display,NamePoint,F_XOR);
     while (button1());
     string(&defont,str,&display,NamePoint,F_XOR);
     string(&defont,"point at an icon to see its name",&display,NamePoint,F_XOR);
   }
}

NextPage()
{
   int ch;
   ch = kbdchar();
   rectf(&display,Drect,F_CLR);
   string(&defont,"point at an icon to see its name",&display,NamePoint,F_XOR);
   IconList = 0;
   p = add(Drect.origin,Pt(16,32));
}

PrintIcon(Name)
char *Name;
{
   Texture t;
   if (own()&MOUSE) ShowName();
   if ((Name[0]>='A') && (Name[0]<='Z')) return(0);
   OpLoad(&t,Name);
   texture(b,b->rect,&t,F_STORE);
   bitblt(b,b->rect,&display,p,F_XOR);
   NewIcon(p,Name);
   p.x += 32;
   if (p.x > (Drect.corner.x-32)) {
     p.x = Drect.origin.x+16;
     p.y += 32;
   }
   if (p.y > (Drect.corner.y-32)) {
     for (;;) {
       wait(KBD|MOUSE);
       if (own()&KBD) {NextPage(); break;}
       if (own()&MOUSE) ShowName();
     }
   }
}

main(argc, argv)
char *argv[];
{
   Setup();
   if (argc == 1) {
     string(&defont,"use: CATALOG <file list>",&display,NamePoint,F_XOR);
     wait(KBD);
     exit();
   }
   string(&defont,"point at an icon to see its name",&display,NamePoint,F_XOR);
   while(argc>1){
      wait(CPU);
      PrintIcon(argv[1]);
      --argc; argv++;
   }
   for (;;) {
     wait(KBD|MOUSE);
     if (own()&KBD) exit();
     if (own()&MOUSE) ShowName();
   }
}
