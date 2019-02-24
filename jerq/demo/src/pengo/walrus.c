/* pengo battle computer */
/* the menu in mouse.c sets/unsets autoflag */
/* the keyboard loop in player.c may call on us */
#include "pengo.h"
#include "jerqio.h"

/* the scott memorial expert system: gets arrays of strings and funcs.  executes
	string after string until one is true or it runs out.  string operators
	are | & ! numbers r t.  t marks the last rule.  numbers name funtions
	in the function array.  r23 names rule 23.
*/

char *ru[] = {"0", "1", "&2 3", "&4 5", "6", "t"};
int to_wall(), kill_seed(), x_wall(), stun_x(), y_wall(), stun_y(), to_corner();
int (*fn[])() = {kill_seed, to_wall, x_wall, stun_x, y_wall, stun_y, to_corner};
scott()
{	int i;
	char *pc;
	move_mouse.x = move_mouse.y = 0;
	for(i = 0, pc = ru[0]; !dorule(&pc, fn); i++, pc = ru[i])
		;
}

dorule(s, f)
char **s;
int (*f[])();
{	char *p = *s;
	int l, r;
loop:
	switch(*p++) {
	case ' ':
		goto loop;
	case 't':
	default:	/* ? */
		*s = p;
		return(1);
	case '|':
		*s = p;
		l = dorule(s, f);
		if(l)
			return(1);
		r = dorule(s, f);
		return(r);
	case '&':
		*s = p;
		l = dorule(s, f);
		if(!l)
			return(0);
		r = dorule(s, f);
		return(r);
	case '!':
		*s = p;
		l = dorule(s, f);
		return(!l);
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		l = p[-1] - '0';
		while(*p <= '9' && *p >= '0') {
			l = 10 * l + *p++ - '0';
		}
		r = (*f[l])();
		*s = p;
		return(r);
	}
}

to_corner()
{
	togoal(0, 0);
	return(1);
}

x_wall()
{
	return(pengo.x == 0);
}

y_wall()
{
	return(pengo.y == 0);
}

stun_x()
{	int i;
	for(i = 0; i < 4; i++) {	/* 4 is in the header file */
		if(!sno_bee[i].exists)
			continue;
		if(sno_bee[i].x == 0) {
			if(sno_bee[i].other == CONFUSED)
				continue;
			move_mouse.x = 8;
			move_mouse.y = 0;
			return(1);
		}
	}
	for(i = 0; i < 4; i++) {
		if(!sno_bee[i].exists)
			continue;
		if(sno_bee[i].x == 0 && sno_bee[i].other == CONFUSED) {
			togoal(sno_bee[i].x, sno_bee[i].y);
			return(1);
		}
	}
	return(0);
}

stun_y()
{	int i;
	for(i = 0; i < 4; i++) {	/* 4 is in the header file */
		if(!sno_bee[i].exists)
			continue;
		if(sno_bee[i].y == 0) {
			if(sno_bee[i].other == CONFUSED)
				continue;
			move_mouse.y = 8;
			move_mouse.x = 0;
			return(1);
		}
	}
	for(i = 0; i < 4; i++) {
		if(!sno_bee[i].exists)
			continue;
		if(sno_bee[i].y == 0 && sno_bee[i].other == CONFUSED) {
			togoal(sno_bee[i].x, sno_bee[i].y);
			return(1);
		}
	}
	return(0);
}

to_wall()
{
	if(!pengo.x || !pengo.y)
		return(0);
	if(pengo.x < pengo.y)
		togoal(0, pengo.x);
	else
		togoal(pengo.x, 0);
	return(1);
}
		
kill_seed()
{	int i, dist = 32766, best = -1, dx, dy;
	if(pengo_left < 2)
		return(0);	/* safety first now */
	for(i = 0; i < MAX_BEES; i++) {
		if(eggs[i].exists == FALSE)
			continue;
		dx = pengo.x - eggs[i].x;
		dy = pengo.y - eggs[i].y;
		if(dx < 0)
			dx = -dx;
		if(dy < 0)
			dy = -dy;
		if(dx + dy < dist) {
			best = i;
			dist = dx + dy;
		}
	}
	if(best >= 0) {
		togoal(eggs[best].x, eggs[best].y);
		return(1);
	}
	return(0);
}

char *gs[] = {"0", "1", "2", "t"};
int avoid_bee(), avoid_block(), do_move();
int (*gf[])() = {avoid_bee, avoid_block, do_move};
static int gx, gy, pref;
togoal(goalx, goaly)
{	int i;
	char *pc;
	move_mouse.x = move_mouse.y = 0;
	gx = goalx;
	gy = goaly;
	for(i = 0, pc = gs[0]; !dorule(&pc, gf); i++, pc = gs[i])
		;
}

avoid_bee()
{	int i, dx, dy, bad = 0, ok = 0;
	static dir = 0;
	for(i = 0; i < 4; i++) {
		if(!sno_bee[i].exists || sno_bee[i].other == CONFUSED)
			continue;
		if((dx = sno_bee[i].x - pengo.x) > 62 || dx < -62)
			continue;
		if((dy = sno_bee[i].y - pengo.y) > 62 || dy < -62)
			continue;
		if(dx >= -64 && dx <= 0)
			bad |= 1;	/* it's left!!*/
		if(dx > 0 && dx < 64)
			bad |= 2;	/* it's right!!*/
		if(dy >= -64 && dy <= 0)
			bad |= 4;	/* it's above!!*/
		if(dy > 0 && dy < 64)
			bad |= 8;	/* it's below!!*/
	}
	if(!bad)
		return(0);
	dir++;
	bad = ~bad;
	dx = pengo.x >> 5;
	dy = pengo.y >> 5;
	if(screen_blank(dx-1, dy)) ok |= 1;
	if(screen_blank(dx+1, dy)) ok |= 2;
	if(screen_blank(dx, dy+1)) ok |= 8;
	if(screen_blank(dx, dy-1)) ok |= 4;
	switch((bad & ok)? (bad & ok): bad) {
	default:	return(0);
	case 1: go_left(); break;
	case 2:	go_right(); break;
	case 3: (dir & 1)? go_left(): go_right(); break;
	case 4: go_up(); break;
	case 5: (dir & 1)? go_left(): go_up(); break;
	case 6: (dir & 1)? go_up(): go_right(); break;
	case 7: (dir & 2)? go_left(): ((dir & 1)? go_up(): go_right()); break;
	case 8: go_down(); break;
	case 9: (dir & 1)? go_left(): go_down(); break;
	case 10: (dir & 1)? go_right(): go_down(); break;
	case 11: (dir & 2)? (dir & 1)? go_left(): go_right(): go_down(); break;
	case 12: (dir & 1)? go_down(): go_up(); break;
	case 13: (dir & 2)? (dir & 1)? go_down(): go_left(): go_up(); break;
	case 14: (dir & 2)? (dir & 1)? go_down(): go_right(): go_up(); break;
	case 15: (dir & 2)? ((dir & 1)? go_down(): go_right()):
			(dir & 1)? go_up(): go_left(); break;
	}
	return(1);
}

avoid_block()
{
	return(0);
}

do_move() {
	if(gx == pengo.x && gy == pengo.y) {
		hit = TRUE;
		return(1);
	}
	if(!pref) {
		if(gx > pengo.x)
			go_right();
		else if(gx < pengo.x)
			go_left();
		else if(gy > pengo.y)
			go_down();
		else if(gy < pengo.y)
			go_up();
	}
	else {
		if(gy > pengo.y)
			go_down();
		else if(gy < pengo.y)
			go_up();
		else if(gx > pengo.x)
			go_right();
		else if(gx < pengo.x)
			go_left();
	}
	pref = !pref;
	return(1);
}

go_right()
{
	move_mouse.x = -8;
}

go_left()
{
	move_mouse.x = 8;
}

go_up()
{
	move_mouse.y = 8;
}

go_down()
{
	move_mouse.y = -8;
}
