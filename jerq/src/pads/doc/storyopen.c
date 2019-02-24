void Story.open()
{
	char buf[256];

	if( !pad ){
		Menu m( "middle", (Action) &middle );
		pad = new Pad( (PadRcv*) this );
		pad->banner( "%s:", file );
		pad->name( file );
		pad->menu( m );
		if( !(fp = fopen( file, "r" )) ){
			pad->insert( 1, "cannot open file" );
			return;
		}
		lines = 0;
		while( fgets( buf, 256, fp ) ) ++lines;
		pad->lines(lines);
	}
	pad->makecurrent();
}
