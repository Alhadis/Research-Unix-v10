#include <jerq.h>
#include <jerqio.h>
#include <font.h>
/*
 * Geometrical shorthands
 */
#define D               (&display)
#define R               (Drect)
#define Ro              (Drect.origin)
#define Rc              (Drect.corner)
#define DY              1

#define NPOSTIMAGE      80
#define MAXDIST         1024
#define POSTSPACING     (MAXDIST/5)
#define SCALE           8
#define WANDERMASK      (NWANDER-1)
#define NWANDER         (MAXDIST/SCALE)
#define ABS(x)          ((x) < 0? -(x) : (x))

char    *fontdir        = "/usr/jerq/font";

Point   vp;     /* vanishing point */
Point   svp;    /* vanishing point, screen coords */
Point   posts[2 * NPOSTIMAGE];
short   *width;
short   *old;
short   wander[NWANDER];
static  vel = 8000;
struct  Font *fp;

struct  fntstr  {
        int     size;
        char    *name;
} fnts[]        = {
        350,    "EB.24",
        275,    "EB.18",
        200,    "EB.12",
        120,    "EB.10",
        0,      "EB.8",
        0,      0,
};

Texture darkgrey = {
        0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777,
        0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777, 0xDDDD, 0x7777,
};
Texture lightgrey = {
        0x2222, 0x8888, 0x2222, 0x8888, 0x2222, 0x8888, 0x2222, 0x8888,
        0x2222, 0x8888, 0x2222, 0x8888, 0x2222, 0x8888, 0x2222, 0x8888,
};
Texture grey = {
        0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555,
        0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555, 0xAAAA, 0x5555,
};
Texture black = {
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
	0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
};
Texture *greys[] = {
        &lightgrey, &grey, &darkgrey,
};

Bitmap  *postimage[NPOSTIMAGE];
Bitmap  *postims[2 * NPOSTIMAGE];

char    *adj[]  = {
        "historic", "putrid", "vermin-infested", "sunny", "stinky",
        "hazy", "polluted", "crime-ridden", "stormy", "cloudy",
        "downtown", "scenic", "metropolitan", "balmy",
};

char    *pre[]  = {
        "San", "Los", "New", "Mt.", "Lake", "Fort", "Point", "South",
        "North", "East", "West", "St.", "Old", "Port",
};

char    *stem[] = {
        "Loma", "Westbury", "Michel", "Haven", "Hampton", "Dakota",
        "Fork", "Reyes", "Whippany", "Hopatcong", "Shasta", "York",
        "Angeles", "Francisco",
};

char    *motto[]        = {
        "Bright spot in Earth's future",
        "One helluva burg",
        "Land of opportunity",
        "Artichoke capitol of the World",
        "Dirt-encrusted rathole",
        "Home of the Sin City Serenaders",
        "Industry's rising star",
        "Vacation spot of the Southland",
        "Paradise on Earth",
        "Where earth meets sky",
        "Where it all began",
        "In a galaxy far, far away ...",
        "Boring but safe",
        "The antidote for civilization",
};

Font    *loadfont();

main(argc, argv)
char *argv[];
{
        char buf[64];
        int location; /* position on track */
        int oloc;

        srand((short) realtime());
        request(KBD|MOUSE);
        vp=div(sub(Rc, Ro), 2);
        svp=add(vp, Ro);
        fp = loadfont();
        initposts();
        initroad();
        greet();
        while(kbdchar()!='q'){
                drawposts(location);
                sleep(2);
                location=forward(oloc = location);
                wiggle(oloc, location);
                drawroad(location);
                undrawposts();
        }
}

Font    *
loadfont()
{
        register int i;
        char buf[64];
        Font *fp;
	return(&defont);
	/*
        for (i = 0; fnts[i].name != 0; i++)
                if (fnts[i].size < vp.x && (vp.y > 150 || fnts[i].size < 100))
                        break;
        if (fnts[i].name == 0)
                exit(1);
        sprintf(buf, "%s/%s", fontdir, fnts[i].name); 
        fp = getfont(buf);
        if (fp == (Font *) 0)
                exit(1);
        return(fp);
	*/
}

greet()
{
        register int n, i;
        Point p;

        p = string(fp, "Greetings from ", &display, Ro, F_XOR);
        n = sizeof adj / sizeof adj[0];
        i = rand() % n;
        p = string(fp, adj[i], &display, p, F_XOR);     
        p = string(fp, " ", &display, p, F_XOR);        
        n = sizeof pre / sizeof pre[0];
        i = rand() % n;
        p = string(fp, pre[i], &display, p, F_XOR);     
        p = string(fp, " ", &display, p, F_XOR);        
        n = sizeof stem / sizeof stem[0];
        i = rand() % n;
        p = string(fp, stem[i], &display, p, F_XOR);    
        p = string(fp, " . . .", &display, p, F_XOR);   
        p = string(fp, "\"", &display, Pt(Ro.x + 0, Ro.y + 30), F_XOR);
        n = sizeof motto / sizeof motto[0];
        i = rand() % n;
        p = string(fp, motto[i], &display, p, F_XOR);
        p = string(fp, "!\"", &display, p, F_XOR);      
}
wiggle(oloc, nloc)
{
        register int n, o, p, q;
        static int dw;

        n = (nloc / SCALE) & WANDERMASK;
        o = (oloc / SCALE) & WANDERMASK;
        for (p = (o - 1 + NWANDER) & WANDERMASK; o != n; ) {
                dw += rand() % 3 - 1;
                q = wander[p];
                if (q > 100 || dw > 12)
                        --dw;
                else if (q < -100 || dw < -12)
                        dw++;
                wander[o] = q + dw;
                p = o;
                o = (o + 1) & WANDERMASK;
        }
}
forward(loc)
int loc;
{
        register m;
        static mm;

        if(button1() && mouse.xy.y < Rc.y && mouse.xy.y > Ro.y)
                vel=80*((Ro.y+vp.y)-mouse.xy.y);
        mm+=vel;
        m=mm/1000;
        mm%=1000;
        return (loc+m)%(8*MAXDIST);
}
initposts()
{
        register Bitmap *b;
        register int dist, i, maxh=vp.y/5, h;

        for(i=0; i<NPOSTIMAGE; i++){
                dist = muldiv(i+1, MAXDIST, NPOSTIMAGE);
                h = disttody(dist) / 2;
                b=postimage[i]=balloc(Rect(0, 0, 1+(h>>4), h));
                if(b==0)
                        exit();
                rectf(b, b->rect, F_OR);
                postims[i] = postims[i + NPOSTIMAGE] = 0;
        }
}

undrawposts()
{
        register Bitmap *b;
        register int i, sdx, dy, ph;

        for (i = 2 * NPOSTIMAGE; --i >= 0; ) {
                b = postims[i];
                if (b != 0)
                        bitblt(b, b->rect, D, posts[i], F_XOR);
        }
}

drawposts(location)
register int location;
{
        register Bitmap *b;
        register int i, sdx, dy, ph;
        int dist;

        dist = POSTSPACING - location % POSTSPACING;
        if (dist < 2)
                dist += POSTSPACING;
        for(i = 0; dist<MAXDIST; dist+=POSTSPACING){
                b=postimage[muldiv(dist, NPOSTIMAGE, MAXDIST)];
                sdx=svp.x + disttodx(dist, location);
                dy=disttody(dist) ;
                ph = b->rect.corner.y - b->rect.origin.y;
                bitblt(b, b->rect, D, Pt(sdx-width[dy], svp.y+dy-ph), F_XOR);
                bitblt(b, b->rect, D, Pt(sdx+width[dy], svp.y+dy-ph), F_XOR);
                postims[i] = b;
                posts[i].x = sdx-width[dy];
                posts[i++].y = svp.y + dy - ph;
                postims[i] = b;
                posts[i].x = sdx+width[dy];
                posts[i++].y = svp.y + dy - ph;
        }
}
int
disttodx(dist, location)
{
        register wy, dy;

        dy = disttody(dist);
        wy = wander[((dist+location)/SCALE)&WANDERMASK];
        return muldiv(wy, dy, vp.y);
}
int
disttody(dist)
{
        return 2 * SCALE * vp.y / dist;
}
int
dytodist(dy)
{
        return 2 * SCALE * vp.y / (dy + 1);
}
short *
myalloc(n)
{
        char *p=alloc(n);
        if(n==0){
                string(&defont,"alloc failed",&display,Pt(0, 0),F_XOR);
                sleep(180);
                exit();
        }
        return (short *)p;
}
initroad()
{
        register i, w, x, h, gi;

        width=myalloc(vp.y*sizeof(width[0]));
        old=myalloc(vp.y*sizeof(old[0]));
        for(i=0; i<vp.y; i++){
                old[i]=svp.x;
                width[i]=i;
        }
        for(i=0; i<vp.y; i++){
                rectf(D, Rect(svp.x-width[i], svp.y+i,
                        svp.x+width[i], svp.y+i+1), F_XOR);
        }
        /* horizon */
        rectf(D, raddp(Rect(0, vp.y, Rc.x-Ro.x, vp.y+1), Ro), F_XOR);
        /* cityscape */
        for (i = 20; --i >= 0; ) {
                x = (rand() % 100) + (rand() % 100) + (rand() % 100) - 150;
                w = rand() % 40;
                h = rand() % (160 - ABS(x));
                x += svp.x;
                gi = rand() % 3;
                texture(&display, Rect(x - w, svp.y - h, x + w, svp.y),
                 greys[gi], F_XOR);
        }  
}
drawroad(location)
{
        register dy;
        register x, y, n, o;
        int t;
        for(dy=0; dy<vp.y; dy++){
                n=svp.x+muldiv(wander[((dytodist(dy)+location)/SCALE)&WANDERMASK], dy, vp.y);
                o=old[dy];
                if(n == o)
                        continue;
                old[dy]=n;
                if(n<o)
                        t=n, n=o, o=t;
		if(o==n)
			continue;
		if(o==n-1){
			point(D, Pt(o-width[dy], svp.y+dy), F_XOR);
                	point(D, Pt(o+width[dy], svp.y+dy), F_XOR);
		}else{
                	texture(D, Rect(o-width[dy], svp.y+dy,
				n-width[dy], svp.y+dy+1), &black, F_XOR);
                	texture(D, Rect(o+width[dy], svp.y+dy,
				n+width[dy], svp.y+dy+1), &black, F_XOR);
		}
        }
}
