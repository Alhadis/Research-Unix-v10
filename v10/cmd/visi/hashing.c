/*
 *      hashing.c 1.3
 *
 *	Hashing Functions for Spreadsheet Program `vis'
 *
 *      A. F. Gettier
 *      Bell Laboratories
 *      Update made 11/1/82 11:11:37
 *      Retrieved 11/15/82 13:22:21
 */
#include	<stdio.h>
#include	"vis.h"
#include	"y.tab.h"

struct hashhdr	htbl;

static struct reswd reswd[]={
	"PI", PI,
	"ABS", ABS,
	"ACOS", ACOS,
	"ASIN", ASIN,
	"AT", AT,
	"ATAN", ATAN,
	"ATAN2", ATAN2,
	"COS", COS,
	"EXP", EXP,
	"GAMMA", GAMMA,
	"HYPOT", HYPOT,
	"INT", INT,
	"LOG", LOG,
	"POW", POW,
	"SIN", SIN,
	"SQRT", SQRT,
	"COL", COL,
	"COPY", COPY,
	"DEBUG", DEBUG,
	"DOWN", DOWN,
	"DUP", DUP,
	"DUPLICATE", DUPLICATE,
	"EDIT", EDIT,
	"HELP", HELP,
	"LEFT", LEFT,
	"LIST", LIST,
	"POSITION", POSITION,
	"READ", READ,
	"REDRAW", REDRAW,
	"REFRESH", REFRESH,
	"REP", REP,
	"REPLICATE", REPLICATE,
	"RIGHT", RIGHT,
	"ROW", ROW,
	"SCALE", SCALE,
	"SHIFT", SHIFT,
	"SLIDE", SLIDE,
	"SHELL", SHELL,
	"SH", SH,
	"THRU", THRU,
	"UP", UP,
	"VER", VER,
	"WIDTH", WIDTH,
	"WRITE", WRITE,
	"ZERO", ZERO,
	"QUIT", QUIT,
	0, 0 };

/*
 *	Init a hash table
 */
void hashinit()
{
	int	i;
	int	size;
	struct hashentry	*tbl;
	size = 63; /* default hash table size */
	htbl.size = size;
	tbl = htbl.table = multnode( hashentry, size );
	for ( i=0; i<size; i++ ) (tbl++)->symbol = 0;
	/*
	 *	enter all the keywords
	 */
	for( i=0; reswd[i].name!=0; i++ )
		hashenter( reswd[i].name, reswd[i].value );
}

/*
 *	Search a hash table
 */
hashsearch( symbol )
char	*symbol;
{
	int	key, mkey;
	struct hashentry	*tbl, *start;
	key = makekey( symbol );
	mkey = key%(htbl.size - 1);
	start = tbl = htbl.table + mkey;
	do {
		if ( tbl->symbol == 0 ) return( 0 );
		if ( tbl->symbol != (char *)(-1) && tbl->key == key
		    && strcmp( symbol, tbl->symbol ) == 0 )
			return(tbl->value);
		tbl++;
		if ( tbl >= (htbl.table + htbl.size ) )  tbl = htbl.table;
	} while ( tbl != start );
	return( 0 );
}

/*
 *	Enter into a hash table
 */
void hashenter( symbol, value )
char	*symbol;
int	value;
{
	int	key, mkey;
	float	a, b;
	struct hashentry	*tbl;
	a = htbl.size;
	b = htbl.entry;
	/*
	 *	First try to clean up the table
	 */
	if ( b/a > .65 )	hashclean( );
	/*
	 *	Now expand
	 */
	b = htbl.entry;
	if ( b/a > .65 )	hashexpand( );
	key = makekey( symbol );
	mkey = key%(htbl.size - 1);
	tbl = htbl.table;
	tbl += mkey;
	loop {
		if ( tbl->symbol == 0 || tbl->symbol == (char *)(-1)) {
			tbl->key = key;
			tbl->symbol = symbol;
			(htbl.entry)++;
			tbl->value = value;
			return;
		}
		if ( tbl->key == key && strcmp( symbol, tbl->symbol ) == 0 ) {
			tbl->value = value;
			return;
		}
		tbl++;
		if ( tbl >= (htbl.table + htbl.size ) )  tbl = htbl.table;
	}
}

/*
 *	Make the key
 */
static makekey( symbol )
char	*symbol;
{
	int	key, i;
	key = 0;
	i = 8;
	while (*symbol != 0) {
		key += *symbol + ((*symbol)<<8) + ((*symbol)<<(i++));
		symbol++;
	}
	return( ((key<0)?(-key):(key)) );
}

/*
 *	Expand the Hash Table
 */
static hashexpand( )
 {
	int	i, mkey, size;
	struct hashentry	*tbl, *temp, *tbl2;
	size = htbl.size * 2 + 1;
	tbl = temp = multnode( hashentry, size );
	for ( i=0; i<size; i++ ) (temp++)->symbol = 0;
	tbl2 = htbl.table;
	for ( i=0; i<htbl.size; i++ ) {
		if ( tbl2->symbol == 0 || tbl2->symbol == (char *)(-1)) {
			tbl2++;
			continue;
		}
		mkey = (tbl2->key)%(size - 1);
		temp = tbl;
		temp += mkey;
		loop {
			if ( temp->symbol == 0 ) {
				temp->key = tbl2->key;
				temp->symbol = tbl2->symbol;
				temp->value = tbl2->value;
				tbl2++;
				break;
			}
			temp++;
			if ( temp >= ( tbl+ size ) )  temp = tbl;
		}
	}
	free( (char *)htbl.table );
	htbl.size = size;
	htbl.table = tbl;
}

/*
 *	Clean up the Hash Table
 */
static hashclean( )
{
	int	i, mkey, size, entry;
	struct hashentry	*tbl, *temp, *tbl2;
	size = htbl.size;
	tbl = temp = multnode( hashentry, size );
	for ( i=0; i<size; i++ ) (temp++)->symbol = 0;
	entry = 0;
	tbl2 = htbl.table;
	for ( i=0; i<htbl.size; i++ ) {
		if ( tbl2->symbol == 0 || tbl2->symbol == (char *)(-1)) {
			tbl2++;
			continue;
		}
		mkey = (tbl2->key)%(size - 1);
		temp = tbl;
		temp += mkey;
		loop {
			if ( temp->symbol == 0 ) {
				temp->key = tbl2->key;
				temp->symbol = tbl2->symbol;
				temp->value = tbl2->value;
				tbl2++;
				entry++;
				break;
			}
			temp++;
			if ( temp >= ( tbl+ size ) )  temp = tbl;
		}
	}
	free( (char *)htbl.table );
	htbl.entry = entry;
	htbl.table = tbl;
}
