#include <pads.pub>
#include <CC/stdio.h>

class Date : public PadRcv {
	class Pad	*pad;
	void		date();
			Date();
};

Date.Date()
{
	Menu m;
	pad = new Pad( (PadRcv*) this );
	pad->banner( "Current Date" );
	pad->name( "date" );
	m.first( "date", (Action) &date );
	pad->menu( m );
	pad->makecurrent();
}

void Date.date()
{
	char *ctime(long*);
	long time(long*), t;
	time(&t);
	pad->insert( 1, "%s", ctime(&t) );
}

void main()
{
	char *error = PadsInit();
	if( error ){
		fprintf( stderr, "%s", error );
		exit(1);
	}
	new Date;
	PadsServe();
}
