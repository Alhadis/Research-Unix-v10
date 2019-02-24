# include	"curses.ext"
/*	@(#) addch.c: 1.1 10/15/83	(1.5	11/18/82)	*/

/*
 *	This routine prints the character in the current position.
 *	Think of it as putc.
 *
 * 3/5/81 (Berkeley) @(#)addch.c	1.3
 */
waddch(win, c)
register WINDOW	*win;
register chtype		c;
{
	register int		x, y;
	char *uctr;
	register char rawc = c & A_CHARTEXT;

	x = win->_curx;
	y = win->_cury;
# ifdef DEBUG
	if (outf)
		if (c == rawc)
			fprintf(outf, "'%c'", rawc);
		else
			fprintf(outf, "'%c' %o, raw %o", c, c, rawc);
# endif
	if (y >= win->_maxy || x >= win->_maxx || y < 0 || x < 0)
	{
# ifdef DEBUG
if(outf)
{
fprintf(outf,"off edge, (%d,%d) not in (%d,%d)\n",y,x,win->_maxy,win->_maxx);
}
# endif
		return ERR;
	}
	switch( rawc )
	{
	case '\t':
		{
			register int newx;

			for( newx = x + (8 - (x & 07)); x < newx; x++ )
			{
				if( waddch(win, ' ') == ERR )
				{
					return ERR;
				}
			}
			return OK;
		}
	  default:
		if( rawc < ' ' || rawc > '~' )
		{
			uctr = unctrl(rawc);
			waddch(win, (chtype)uctr[0]|(c&A_ATTRIBUTES));
			waddch(win, (chtype)uctr[1]|(c&A_ATTRIBUTES));
			return OK;
		}
		if( win->_attrs )
		{
#ifdef	DEBUG
if(outf) fprintf(outf, "(attrs %o, %o=>%o)", win->_attrs, c, c | win->_attrs);
#endif	DEBUG
			c |= win->_attrs;;
		}
		if( win->_y[y][x] != c )
		{
			if( win->_firstch[y] == _NOCHANGE )
			{
				win->_firstch[y] = win->_lastch[y] = x;
			}
			else
			{
				if( x < win->_firstch[y] )
				{
					win->_firstch[y] = x;
				}
				else
				{
					if( x > win->_lastch[y] )
					{
						win->_lastch[y] = x;
					}
				}
			}
		}
		win->_y[y][x++] = c;
		if (x >= win->_maxx)
		{
			x = 0;
new_line:
			if (++y > win->_bmarg)
			{
				if (win->_scroll && !(win->_flags&_ISPAD))
				{
#ifdef	DEBUG
	if(outf)
	{
		fprintf( outf, "Calling wrefresh( 0%o ) & _tscroll(  0%o  )\n",
				win, win );
	}
#endif	DEBUG
					wrefresh(win);
					_tscroll( win );
					--y;
				}
				else
				{
# ifdef DEBUG
					int i;
					if(outf)
					{
					    fprintf(outf,
					    "ERR because (%d,%d) > (%d,%d)\n",
					    x, y, win->_maxx, win->_maxy);
					    fprintf(outf, "line: '");
					    for (i=0; i<win->_maxy; i++)
						fprintf(outf, "%c",
							win->_y[y-1][i]);
					    fprintf(outf, "'\n");
					}
# endif	DEBUG
					return ERR;
				}
			}
		}
# ifdef DEBUG
		if(outf) fprintf(outf, "ADDCH: 2: y = %d, x = %d, firstch = %d, lastch = %d\n", y, x, win->_firstch[y], win->_lastch[y]);
# endif	DEBUG
		break;
	  case '\n':
		wclrtoeol(win);
		x = 0;
		goto new_line;
	  case '\r':
		x = 0;
		break;
	  case '\b':
		if (--x < 0)
			x = 0;
		break;
	}
	win->_curx = x;
	win->_cury = y;
	return OK;
}
