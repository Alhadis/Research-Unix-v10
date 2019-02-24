
#include <jerq.h>

#undef	bitblt
/* #undef	texture */
#define bitblt(s, r, d, p, c)	(*((void(*)())0x7a24))(s,r,d,p,c)
/* #define texture(s, r, d, c)	(*((void(*)())0x4372c))(s, r, d, c) */

#define sgn(x) ((x)<0 ? -1 : (x)==0 ? 0 : 1)

Texture magtexture = {
	 0x0FC0, 0x0FE0, 0x0FF0, 0x0708,
	 0x0384, 0x01C2, 0x00E1, 0x7E71,
	 0x7F39, 0x7F19, 0x3899, 0x1C59,
	 0x0E39, 0x0732, 0x03F4, 0x01F8,
};

long lmin(x,y)
long x,y;
{
   if (x<y) return(x); return(y);
}

small(i)
register i;
{
	return((rand() % i) - (i >> 1));
}

PickVel(velocity)
Point *velocity;
{
  do {
    velocity->x = small(7);
    velocity->y = small(7);
  } while ((velocity->x == 0) && (velocity->y == 0));
}

ChangeVel(velocity)
Point *velocity;
{
   do {
     velocity->x += small(3);
     velocity->y += small(3);
   } while ((velocity->x < -7) && (velocity->x > 7) &&
            (velocity->y < -7) && (velocity->y > 7));
}

main()
{
   Point ms,diff,force,leftover,position,velocity;
   long distx,disty,sqdist;
   Bitmap *magnet, screen;
   screen.base = addr(&display,Pt(0,0));
   screen.width = display.width;
   screen.rect = Jrect;
   
   request(MOUSE|KBD);

   position = div(add(Drect.origin,Drect.corner),2);

   magnet = balloc(Rect(0,0,16,16));
   texture(magnet,magnet->rect,&magtexture,F_STORE);
   bitblt(magnet,magnet->rect,&screen,position,F_XOR);

   PickVel(&velocity);

   while (!(own()&KBD)) {
     sleep(2);
     wait(MOUSE);
     ms = mouse.xy;
     /* if (small(7)==0) ChangeVel(&velocity); */
     bitblt(magnet,magnet->rect,&screen,position,F_XOR);
     position = add(position,velocity);
     if (!ptinrect(position,inset(Jrect,20))) {
       position = sub(position,velocity);
       PickVel(&velocity);
     }
     bitblt(magnet,magnet->rect,&screen,position,F_XOR);
     diff = sub(position,ms);
     distx = abs(diff.x);
     disty = abs(diff.y);
     sqdist = (distx+disty)*(distx+disty);
     if (sqdist<100) force.x = 0;
     else force.x = lmin(distx,1+1000000/sqdist); 
     if (sqdist<100) force.y = 0;
     else force.y = lmin(disty,1+1000000/sqdist);
     ms.x += sgn(diff.x)*force.x;
     ms.y += sgn(diff.y)*force.y;
     cursset(ms);
   }
   bitblt(magnet,magnet->rect,&screen,position,F_XOR);
}


