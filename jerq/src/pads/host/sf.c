#define SF_HASH 823
struct SF_CELL {
	SF_CELL *link;
	char     buf[1];
};
void sprintf(char*, char* ...);

char *sf(char *f ... )
{
	int *a = (int*)&f;
	static SF_CELL *Table[SF_HASH];
	static Calls, Strings, Worst, Bytes;
	char x[1024], *p;
	unsigned long len, h, i;
	struct SF_CELL *s;

	if( !f ){
		static char report[128];
		sprintf( report, "strings=%d calls=%d worst=%d bytes=%d",
				 Strings,   Calls,   Worst,    Bytes );
		return report;
	}
	++Calls;		
	sprintf( x, f, a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8], a[9] );
	h = 0;
	for( len = 0, p = x; *p; )
		h += (*p++) << (++len%4);
	h %= SF_HASH;
	for( s=Table[h],i=1; s; s=s->link,++i )
		if(!strcmp(x,s->buf)) return s->buf;
	++Strings;
	if( i>Worst ) Worst = i;
	len = (len+4+sizeof(SF_CELL*)) / 4 * 4;		/* vax */
	s = (SF_CELL*) new char [len];
	if( !s ) return "sf(): out of memory";
	Bytes += len;
	s->link = Table[h];
	Table[h] = s;
	strcpy( s->buf, x );
	return s->buf;
}
