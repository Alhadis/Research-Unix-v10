#ifdef JOURNAL
#include "term.h"

#define JOURNALSIZE 25
char *J_name[JOURNALSIZE+1];
char J_state[JOURNALSIZE];
char *J_hist = "<none>";
char J_init = 0;

int J_search(t)
register char *t;
{
	register i;
	for( i = 0; i < JOURNALSIZE && J_name[i]; ++i )
		if( *t==*J_name[i] && !strcmp(t, J_name[i]) )
			return i;
	if( i >= JOURNALSIZE ) return -1;
	J_name[i] = t;
	J_state[i] = J_init;
	return i;
}

void journal( n, f, a1, a2, a3, a4, a5, a6 )
register char *n, *f;
{
	register i;
	J_hist = n;
	if( !J_init && !J_name[0] ) return;
	i = J_search(n);
	if( i == -1 || !J_state[i] ) return;
	PutTextf( "%s:\t", n);
	PutTextf( f, a1, a2, a3, a4, a5, a6 );
	PutText('\n');
	DIRTYServe();
}

char *J_Gen(i)
register i;
{
	static char m[32];
	register char *t;

	if( i == 0 ) return "\240* :=OFF";
	if( i == 1 ) return "\240* :=ON ";
	t = J_name[i-=2];
	if( !t ) return 0;
	strcpy( m, t);
	strcat( m, J_state[i] ? "\240:=OFF" : "\240:=ON " );
	return m;
}

Menu J_menu = { 0, J_Gen };

#endif JOURNAL

void MOUSEServe()
{
#ifdef JOURNAL
	register hit, i;
	if( own(MOUSE) ){
		if( butts == BUTT3 ){
			request(SEND|RCV);
			sleep(1);
			request(RCV|KBD|MOUSE);
		} else if( ptinrect(mouse.xy,Drect) && butts==(BUTT1|BUTT3) ){
			while( butts&BUTT1 ) {}
			cursswitch(0L);
			hit = menuhit(&J_menu,3);
			cursswitch(Cursor);
			switch( hit ){
			case -1: break;
			case 0:
			case 1:	J_init = hit;
				for( i = 0; i < JOURNALSIZE; ++i )
					J_state[i] = hit;
				break;
			default:
				J_state[hit-2] ^= 1;
			}
		}
	}
#endif JOURNAL
}
