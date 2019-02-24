/*	@(#)ll_refresh.c	1.1	(1.22)	*/
#include "curses.ext"

extern int InputPending;
extern int outchcount;

extern int *context;

int didntdobotright;	/* writechars didn't output char in bot right corner */

/*
 * Optimally make the screen (which currently looks like SP->cur_body)
 * look like SP->std_body.  If use_idl is 1, this routine will call
 * out all its horses, including some code to figure out
 * how to use insert/delete line.  If use_idl is 0, or if the terminal
 * does not have insert/delete line, a simpler scheme will
 * be used, and the insert/delete line features of the terminal will not
 * be used.
 *
 * While the original intent of this parameter was speed (insert/delete
 * line was slow) the parameter currently is there to avoid annoying
 * the user with unnecessary insert/delete lines.
 */
int
_ll_refresh (use_idl)
int	use_idl;
{
	register int n, i, j;
	register struct line *dln, *pln;
	struct line *_line_alloc();

#ifdef DEBUG
	if(outf) fprintf(outf, "_ll_refresh(%d)\n", use_idl);
	if(outf) fprintf(outf, "phys cursor at (%d,%d), want cursor at (%d,%d)\n",
		SP->phys_y, SP->phys_x, SP->virt_y, SP->virt_x);
#endif	DEBUG
	outchcount = 0;
#ifdef FIONREAD
	if( SP->check_fd >= 0 )
	{
		ioctl (SP->check_fd, FIONREAD, &InputPending);
	}
	else
	{
		InputPending = 0;
	}
	if( InputPending )
	{
#ifdef DEBUG
		if (outf) fprintf(outf, "InputPending %d, aborted ll_refresh at start\n", InputPending);
#endif	DEBUG
		return 0;
	}
#endif FIONREAD
#ifdef		NONSTANDARD
	input_wait();
#endif		NONSTANDARD
#ifdef DEBUG
	if (outf) fprintf(outf, "virt cursor at y=%d, x=%d, length %d, length-1 %d\n",
		SP->virt_y, SP->virt_x, SP->std_body[SP->virt_y+1]->length,
		SP->std_body[SP->virt_y]->length);
#endif	DEBUG
	if( SP->virt_y >= 0 && SP->std_body[SP->virt_y+1]->length < SP->virt_x )
	{
		SP->std_body[SP->virt_y+1]->length =
		    SP->virt_x >= columns ? columns : SP->virt_x;
	}
	/* Get rid of trailing blanks on all lines */
	for( n = 1; n <= lines; n++ )
	{
		dln = SP->std_body[n];
		pln = SP->cur_body[n];
		if( dln && dln != pln )
		{
			register chtype *p;

			if( i = dln->length )
			{
				p = dln->body + i;
				while( *--p == ' ' && --i > 0 )
					;
				dln->length = i;
			}
		}

		if( pln )
		{
			register chtype *p;

			if( i = pln->length )
			{
				p = pln->body + i;
				while( *--p == ' ' && --i > 0 )
					;
				pln->length = i;
			}
		}
	}
	if( magic_cookie_glitch > 0 )
		_toss_cookies();
#ifdef DEBUG
	if(outf) fprintf(outf, "what we have:\n");
	for( i=1; i<=lines; i++ )
	{
		if(outf) fprintf(outf, "%8x: ", SP->cur_body[i]);
		if( SP->cur_body[i] == NULL )
		{
			if(outf) fprintf(outf, "()\n");
		}
		else
		{
			if(outf) fprintf(outf, "%4d ", SP->cur_body[i]->length);
			for( j=0; j<SP->cur_body[i]->length; j++ )
			{
				n = SP->cur_body[i]->body[j];
				if( n & A_ATTRIBUTES )
				{
					putc('\'', outf);
				}
				n &= 0177;
				if(outf) fprintf(outf, "%c", n>=' ' ? n : '.');
			}
			if(outf) fprintf(outf, "\n");
		}
	}
	if(outf) fprintf(outf, "what we want:\n");
	for (i=1; i<=lines; i++)
	{
		if(outf) fprintf(outf, "%8x: ", SP->std_body[i]);
		if (SP->std_body[i] == NULL)
		{
			if(outf) fprintf(outf, "()\n");
		}
		else
		{
			if(outf) fprintf(outf, "%4d ", SP->std_body[i]->length);
			for (j=0; j<SP->std_body[i]->length; j++)
			{
				n = SP->std_body[i]->body[j];
				if (n & A_ATTRIBUTES)
				{
					putc('\'', outf);
				}
				n &= 0177;
				if(outf) fprintf(outf, "%c", n>=' ' ? n : '.');
			}
			if(outf) fprintf(outf, "\n");
		}
	}
	if(outf) fprintf(outf, "----\n");
	if(outf) fflush(outf);
#endif	DEBUG
	SP->check_input = SP->baud / 2400;
	if (SP->doclear)
	{
#ifdef DEBUG
		if(outf) fprintf(outf, "SP->doclear, clearing screen\n");
#endif
		_reset();
		SP->doclear = 0;
		for( n = 0; n <= lines; n++ )
		{
			if( SP->cur_body[n] != SP->std_body[n] )
			{
				_line_free( SP->cur_body[n] );
			}
			SP->cur_body[n] = 0;
		}
	}

	/* Choose between two updating algorithms. */
	if( use_idl && _cost(ilfixed) < INFINITY )
	{
#ifdef DEBUG
		if(outf) fprintf(outf, "use_idl\n");
#endif
		for( n = 1; n <= lines; n++ )
		{
			if( SP->cur_body[n] == 0 )
			{
				SP->cur_body[n] = _line_alloc();
			}
			if( SP->std_body[n] == 0 )
			{
				SP->std_body[n] = SP->cur_body[n];
			}
			else
			{
				_comphash( SP->std_body[n] );
			}
			_comphash( SP->cur_body[n] );
		}
		/*
		 * Count number of matches if we scroll 1 line and if we
		 * don't scroll at all.  This is primarily useful for the
		 * case where we scroll the whole screen.  Scrolling a portion
		 * of the screen will be handled by the ins/del line routines,
		 * although a special case here might buy some CPU speed.
		 */
		for( i=1,n=0,j=0; i<lines; i++ )
		{
			if( SP->cur_body[i+1]->hash == SP->std_body[i]->hash )
				n++;
			if( SP->cur_body[i]->hash == SP->std_body[i]->hash )
				j++;
		}
		if( n > lines-3 && n > j )
		{
			_window(0, lines-1, 0, columns-1);
			_scrollf(1);
			_line_free(SP->cur_body[1]);
			for( i=1; i<lines; i++ )
			{
				/* Copy line with two references since they
				 * are no longer the same row on screen. */
				if( SP->cur_body[i+1] == SP->std_body[i+1] )
				{
					struct line *p;
					int l;
					chtype *b1, *b2;
					p = _line_alloc ();
					p->length=l=SP->cur_body[i+1]->length;
					p->hash = SP->cur_body[i+1]->hash;
					b1 = &(p->body[0]);
					b2 = &(SP->cur_body[i+1]->body[0]);
					for(  ; l>0; l-- )
					{
						*b1++ = *b2++;
					}
					SP->std_body[i+1] = p;
				}
				SP->cur_body[i] = SP->cur_body[i+1];
			}
			SP->cur_body[lines] = _line_alloc();
		}
		i = 1;
		/*
		 * Break the screen (from 1 to lines) into clumps of
		 * lines that are different.  Thus we ignore the ones that
		 * are identical.
		 */
		for( ;; )
		{
			while( i<=lines && SP->cur_body[i]==SP->std_body[i] )
			{
				i++;
			}
			if( i > lines )
			{
				break;
			}
			for( j=i; j <= lines &&
				SP->cur_body[j] != SP->std_body[j]; j++)
					;
			j--;
#ifdef DEBUG
			if(outf) fprintf(outf, "window from %d to %d\n", i, j);
#endif
			/* i thru j is a window of different lines. */
			if( i == j )
			{
				_id_char(SP->cur_body[i], SP->std_body[i], i-1);
				if( SP->cur_body[i] != SP->std_body[i] )
				{
					_line_free (SP->cur_body[i]);
				}
				SP->cur_body[i] = SP->std_body[i];
			}
			else
			{
				_window(i-1, j-1, 0, columns-1);
				_setwind();   /* Force action for moves, etc */
				_id_line(i, j);
			}
			i = j+1;
		}
	}
	else			/* fast update */
	{
#ifdef DEBUG
		if(outf) fprintf(outf, "Fast Update, lines %d\n", lines);
#endif
		_window(0, lines-1, 0, columns-1);
		_setwind();   /* Force action for moves, etc */
		for( n = 1; n <= lines; n++ )
			if( SP->std_body[n] != SP->cur_body[n] )
			{
				_id_char( SP->cur_body[n],SP->std_body[n],n-1 );
				if( SP->cur_body[n] != SP->std_body[n] )
				{
					_line_free (SP->cur_body[n]);
				}
				SP->cur_body[n] = SP->std_body[n];
			}
	}

	/*
	 * Didn't output char in bottom right corner of screen.
	 * Remember this fact so that next time when it's higher
	 * on the screen, we'll fix it up.
	 */
	if( didntdobotright )
	{
		int holdvx, holdvy;
#ifdef DEBUG
		if (outf) fprintf(outf, "didntdobotright so setting SP->cur_body[%d]->body[%d] from '%c' to space.\n", lines, columns-1, SP->cur_body[lines]->body[columns-1]);
#endif
		holdvx = SP->virt_x;
		holdvy = SP->virt_y;
		/*
		 * This code in effect marks the last line dirty
		 * so that the next time it will get fixed.  It also
		 * splits the line back into virt/phys so we don't
		 * clobber the virtual part too.
		 */
		_ll_move(lines-1, columns-1);
		SP->cur_body[lines]->body[columns-1] = ' ';
		didntdobotright = 0;
		/* Now restore the cursor we clobbered. */
		_ll_move(holdvy, holdvx);
	}

	_hlmode(0); _sethl();
#ifdef DEBUG
	if(outf) fprintf(outf, "at end, phys SP->curptr at (%d,%d), want SP->curptr at (%d,%d)\n",
		SP->phys_y, SP->phys_x, SP->virt_y, SP->virt_x);
#endif	DEBUG
#ifdef notdef
	if(magic_cookie_glitch > 0)
		_fixcursor();
#endif	notdef
	if( !InputPending && SP->virt_x >= 0 && SP->virt_y >= 0 )
	{
		_pos (SP->virt_y, SP->virt_x);
	}
	__cflush();
#ifdef DEBUG
	if(outf) fprintf(outf, "end of _ll_refresh, InputPending %d\n", InputPending);
	if(outf) fflush(outf);
#endif	DEBUG
	return outchcount;
}

/*
 * This routine is only needed on terminals with the "magic cookie"
 * effect.  The problem is that the designers of
 * these terminals didn't allocate 16 bits for
 * each character (7 for the character and 9 for attributes) but instead
 * created some reserved "magic cookie" characters to tell the scan
 * routine "you should change attributes now".  This would be fine except
 * that these cookies take up a space in memory, and usually display as a
 * blank.  This makes it impossible to display what the user really
 * wanted, if he is using attributes for underlining, bold, etc.  Such
 * terminals include the Teleray 1061, TVI 912 and 950, Teletype
 * DataSpeed 40, and Adds Regent 40.
 *
 * One approach to this problem is to make everybody pay the price of
 * this effect, forcing the programmer to allocate a blank space
 * when attributes are changed.  This works cleanly but I consider it
 * unacceptable.
 *
 * My approach is to simulate what the programmer (who wasn't thinking
 * about these terminals) wanted as closely as possible.  If there
 * is a desired blank in there, we use that slot.  If not, we shove the
 * rest of the line to the right one space.  (When several attributes
 * are changed on one line, this can result in losing several characters
 * from the right of the line.)
 *
 * This routine looks for places in SP->std_body where shoving to the
 * right is needed, and does the required shoving.
 */
static
_toss_cookies()
{
	register int i, j, len;
	register struct line *dsi;
	register chtype *b;

#ifdef DEBUG
	if(outf) fprintf(outf, "_toss_cookies\n");
#endif
	for( i=1; i<=lines; i++ )
	{
		dsi = SP->std_body[i];
		if( dsi && dsi != SP->cur_body[i] )
		{
			len = dsi->length;
			b = dsi->body;
			for( j=0; j<len; j++ )
			{
				if( b[j]&A_ATTRIBUTES )
				{
#ifdef DEBUG
					if(outf) fprintf(outf, "_shove, line %d, char %d, val %o\n", i, j, b[j]);
#endif
					dsi->length = _shove(b, len, i);
					break;
				}
			}
		}
	}
}
