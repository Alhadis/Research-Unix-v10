#include "curses.ext"
/*	@(#) mvcur.c: 1.1 10/15/83	(1.14	3/16/83)	*/


/*
 * Cursor motion optimization routine.  This routine takes as parameters
 * the screen positions that the cursor is currently at, and the position
 * you want it to be at, and it will move the cursor there very
 * efficiently.  It isn't really optimal, since several approximations
 * are taken in the interests of efficiency and simplicity.  The code
 * here considers directly addressing the cursor, and also considers
 * local motions using left, right, up, down, tabs, backtabs, vertical
 * and horizontal addressing, and parameterized motions.  It does not
 * consider using home down, or taking advantage of automatic margins on
 * any of the four directions.  (Two of these directions, left and right,
 * are well defined by the am and bw capabilities, but up and down are
 * not defined, nor are tab or backtab off the ends.)
 *
 * General strategies considered:
 *	CA	Direct Cursor Addressing
 *	LM	Local Motions from the old position
 *	HDR	Home + Local Motions from upper left corner
 *	CR	CR + Local Motions from left margin
 *
 * Local Motions can include
 *	Up	cuu, cuu1, vpa
 *	Down	cud, cud1, vpa
 *	Left	cul, cul1, hpa, bs, cbt
 *	Right	cuf, cuf1, hpa, tab, char moved over
 */

#ifdef DEBUG
  /* We don't want this debugging output now. */
# undef DEBUG
#endif

/* #define mvcurdebug */

#ifdef mvcurdebug
# define DEBUG
# define _outch oc
FILE *outf;
main()
{
	int oldrow, oldcol, newrow, newcol, n;

	newterm(getenv("TERM"), stdout, stdin);
	echo();
	nocrmode();
	nl();
	outf = stdout;
	setbuf(stdout, NULL);
	for (;;) {
		printf("oldrow, oldcol, newrow, newcol: ");
		n = scanf("%d %d %d %d", &oldrow, &oldcol, &newrow, &newcol);
		if (n < 0) {
			endwin();
			exit(1);
		}
		mvcur(oldrow, oldcol, newrow, newcol);
	}
}

_outch(ch)
{
	printf("_outch '%s'\n", unctrl(ch));
}
#else
int _outch();
#endif

static int bare_lf_ok;	/* bare_lf_ok is true if linefeed will work right */
char *tparm();

mvcur(oldrow, oldcol, newrow, newcol)
int oldrow, oldcol, newrow, newcol;
{
	/* temporary costs of various submotions */
	int	home_right;	/* cost to move from col 0 right to newcol */
	int	cur_right;	/* motion from current postition to right */
	int	cur_left;	/* motion from current postition to left */
	int	home_down;	/* motion from line 0 downward */
	int	cur_down;	/* motion from current position downward */
	int	cur_up;		/* motion from current position upward */
	/* costs of various full motions we consider */
	int	cost_ca;	/* cost of directly addressing the cursor */
	int	cost_hdr;	/* going home, local motions down & right */
	int	cost_cr;	/* return, local motions up/down & right */
	int	cost_lm;	/* local motions up/down & left/right */
	int	sgf;

#ifdef DEBUG
	if(outf) fprintf(outf, "mvcur(oldrow=%d, oldcol=%d, newrow=%d, newcol=%d)\n",
		oldrow, oldcol, newrow, newcol);
#endif
	/* Quick check for nothing to do - very common case. */
	if (oldrow == newrow && oldcol == newcol)
		return;
#ifndef 	NONSTANDARD
# ifdef USG
	bare_lf_ok = ((cur_term->Nttyb.c_oflag&ONLCR)==0);
# else
	sgf = cur_term->Nttyb.sg_flags;
	bare_lf_ok = (sgf&RAW) ? 1 : (sgf&CRMOD)==0;
# endif
#else		NONSTANDARD
	bare_lf_ok = 1;
#endif		NONSTANDARD
	/* Another common case: crlf */
	if (newcol==0 && newrow==oldrow+1 && carriage_return && cursor_down) {
		/*
		 * Just because bare_lf_ok is on doesn't mean cursor_down is
		 * crlf.  E.g. in the tty 40/2 it's escape B.  The test below
		 * will be wrong on a newline style terminal (!bare_lf_ok)
		 * with no carriage_return where cursor_down is not crlf.
		 * I have never seen such a terminal.
		 */
		if (oldcol > 0 && carriage_return)
			tputs(carriage_return, 1, _outch);
		tputs(cursor_down, 1, _outch);
		return;
	}

	/* Figure out costs of various component motions */
	home_right = _loc_right(0     , newcol, 0, newcol);
	cur_right  = _loc_right(oldcol, newcol, 0, newcol);
	cur_left   = _loc_left (oldcol, newcol, 0);
	home_down  = _loc_down (0     , newrow, 0, newcol);
	cur_down   = _loc_down (oldrow, newrow, 0, newcol);
	cur_up     = _loc_up   (oldrow, newrow, 0);
#ifdef DEBUG
	if(outf) fprintf(outf, "home_right %d, cur_right %d, cur_left %d, home_down %d, cur_down %d, cur_up %d\n",
		home_right, cur_right, cur_left, home_down, cur_down, cur_up);
#endif

	/* 4 possible strategies: get costs for each */
	cost_ca    = _cost(Cursor_address);
	cost_hdr   = _cost(Cursor_home) + home_down + home_right;
	/* 3rd and 4th strategies: local motions and with carriage return. */
	if (newrow < oldrow) {
		if (newcol < oldcol)
			cost_lm    = cur_left + cur_up;
		else
			cost_lm    = cur_right + cur_up;
		cost_cr = _cost(Carriage_return) + cur_up + home_right;
	} else {
		if (newcol < oldcol)
			cost_lm    = cur_left + cur_down;
		else
			cost_lm    = cur_right + cur_down;
		if (cur_down >= INFINITY)
			cur_down = _loc_down(oldrow, newrow, 0, 0);
		cost_cr = _cost(Carriage_return) + cur_down + home_right;
	}

#ifdef DEBUG
	if(outf) fprintf(outf, "cost_ca %d, cost_hdr %d, cost_cr %d, cost_lm %d\n",
		cost_ca, cost_hdr, cost_cr, cost_lm);
#endif
	/*
	 * Now we pick which one is cheapest and actually do it.
	 * Note the ordering if they come out equal - this was
	 * conciously chosen based on how visually distracting
	 * it is to see the cursor bounce all over the screen,
	 * I did not take into account approximation errors.
	 */
	if (cost_ca <= cost_hdr && cost_ca <= cost_cr && cost_ca <= cost_lm) {
		/* direct cursor addressing is cheapest */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose absolute cursor addressing\n");
#endif
		tputs(tparm(cursor_address, newrow, newcol), 1, _outch);
	} else if (cost_lm <= cost_cr && cost_lm <= cost_hdr) {
		/* local motions are cheapest */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose local motions\n");
#endif
		if (newcol > oldcol) {
			if (newrow > oldrow)
				(void) _loc_down(oldrow, newrow, 1, newcol);
			else
				(void) _loc_up(oldrow, newrow, 1);
			(void) _loc_right(oldcol, newcol, 1, newrow);
		} else {
			if (newrow > oldrow)
				(void) _loc_down(oldrow, newrow, 1, newcol);
			else
				(void) _loc_up(oldrow, newrow, 1);
			(void) _loc_left(oldcol, newcol, 1);
		}
	} else if (cost_cr <= cost_hdr) {
		/* carriage return + local motions are cheapest */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose carriage return + local motions\n");
#endif
		tputs(carriage_return, 1, _outch);
		if (newrow > oldrow)
			(void) _loc_down(oldrow, newrow, 1, 0);
		else
			(void) _loc_up(oldrow, newrow, 1);
		(void) _loc_right(0, newcol, 1, newrow);
	} else {
		/* home + local motions are cheapest */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose home + local motions\n");
#endif
		tputs(cursor_home, 1, _outch);
		(void) _loc_down(0, newrow, 1, newcol);
		(void) _loc_right(0, newcol, 1, newrow);
	}
#ifdef DEBUG
	if(outf) fprintf(outf, "end of mvcur\n");
#endif
}

/*
 * These four routines figure out what the cost of the most efficient
 * kind of local motion from the given row or column to the other given
 * row or column is.  They return the cost, in characters.  If the third
 * argument is 1, they actually do the motion.  The row number is useful
 * if we're going to actually do the motion - that way we can look in
 * the screen image (if we have it) and just output the characters that
 * are already on the screen - this usually saves 50% over cursor_right.
 */
static
_loc_right(oldcol, newcol, domotion, row)
{
	int c0, c1, c2, c3;
	int i, tabcol, ntabs, nright, nleft;
	/* notinsmode: we know for sure we aren't in insert char mode */
	int notinsmode = SP && SP->phys_irm!=1;
	register struct line *rp;

	if (newcol < oldcol)	/* can't go left with right motions */
		return INFINITY;
	if (newcol == oldcol)
		return 0;	/* already there - nothing to do */
#ifdef DEBUG
	fprintf(outf, "SP %x, phys_irm %d, notinsmode %d\n",
	SP, SP->phys_irm, notinsmode);
#endif

	/*
	 * Code here and further down attempts to output the character that
	 * is already on the screen to move right.
	 */
	if (notinsmode)
		_cost(Cursor_right) = 1;
	else
		_cost(Cursor_right) = _cost(Right_base);
	/* figure out various costs */

	tabcol = (newcol+4)/8 * 8;   /* round to nearest 8 */
	/* tab past right margin is undefined */
	if (tabcol >= columns)
		tabcol = (columns-1)/8 * 8;
	ntabs = (tabcol-oldcol+7)/8;
	if (ntabs <= 0)
		tabcol = oldcol;
	if (tabcol < newcol) {
		/* some tabs plus some rights */
		nright = newcol - tabcol;
		c1 = ntabs*_cost(Tab) + nright*_cost(Cursor_right);
	} else {
		/* some tabs plus some lefts */
		nleft = tabcol - newcol;
		c1 = ntabs*_cost(Tab) + nleft*_cost(Cursor_left);
	}

	c0 = (newcol - oldcol) * _cost(Cursor_right);

	if (parm_right_cursor)
		c2 = _cost(Parm_right_cursor);
	else
		c2 = INFINITY;

	if (column_address)
		c3 = _cost(Column_address);
	else
		c3 = INFINITY;
	
#ifdef DEBUG
	if(outf) fprintf(outf, "_loc_right(%d, %d, %d), chars %d, ri %d, RI %d, ch %d\n",
		oldcol, newcol, domotion, c0, c1, c2, c3);
#endif

	/* Decide and maybe do them */
	if (c3 <= c1 && c3 <= c2 && c3 <= c0) {
		/* cheapest to use column absolute cursor addressing */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose column absolute cursor addressing\n");
#endif
		if (domotion)
			tputs(tparm(column_address, newcol), 1, _outch);
		return c3;
	} else if (c2 <= c1 && c2 <= c0) {
		/* cheapest to use column relative motion */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose column relative motion\n");
#endif
		if (domotion)
			tputs(tparm(parm_right_cursor, newcol-oldcol), 1, _outch);
		return c2;
	} else {
		/* cheapest to use several right commands */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose rights: ntabs %d, tabcol %d, nleft %d, nright %d\n", ntabs, tabcol, nleft, nright);
#endif
		if (domotion)
			if (c1 < c0) {
				for (i=0; i<ntabs; i++)
					tputs(tab, 1, _outch);
				if (tabcol < newcol) {
					/* some tabs plus some rights */
					for (i=0; i<nright; i++) {
#ifdef DEBUG
	if (outf && SP->cur_body[row+1]) fprintf(outf, "nd1, row %d col %d+%d=%d, char '%c'\n",
	row, tabcol, i, tabcol+i, SP->cur_body[row+1]->body[tabcol+i]);
#endif
						rp = SP->cur_body[row+1];
						if (cursor_right && (!notinsmode || rp && SP->phys_gr != (rp->body[tabcol+i] & A_ATTRIBUTES))) /* dont know */
							tputs(cursor_right,1,_outch);
						else if (rp && rp->length > tabcol+i)
							/* Note we assume dumb terminals without cursor_right don't have
							 * standout either, otherwise we should go into right standout
							 * mode here and in the essentially similar code below.
							 */
							_outch(rp->body[tabcol+i]&A_CHARTEXT);
						else	/* off edge */
							_outch(' ');
					}
				} else {
					/* some tabs plus some lefts */
					for (i=0; i<nleft; i++)
						tputs(cursor_left,1,_outch);
				}
			} else {
				for (i=oldcol; i<newcol; i++) {
#ifdef DEBUG
	if (outf && SP->cur_body[row+1]) fprintf(outf, "nd2, row %d col %d, char '%c'\n",
	row, i, SP->cur_body[row+1]->body[i]);
#endif
					rp = SP->cur_body[row+1];
					if (cursor_right && (!notinsmode || rp && SP->phys_gr != (rp->body[i] & A_ATTRIBUTES))) /* dont know */
						tputs(cursor_right,1,_outch);
					else if (rp && rp->length > i)
						_outch(rp->body[i]&A_CHARTEXT);
					else	/* off edge */
						_outch(' ');
				}
			}
		return (c1 < c0) ? c1 : c0;
	}
}

static
_loc_left(oldcol, newcol, domotion)
{
	int c1, c2, c3;
	int i, tabcol, ntabs, nright, nleft;

	if (newcol > oldcol)	/* can't go right with left motions */
		return INFINITY;
	if (newcol == oldcol)
		return 0;	/* already there - nothing to do */

	/* figure out various costs */
	if (cursor_left) {
		if (back_tab) {
			tabcol = (newcol+4)/8 * 8;   /* round to nearest 8 */
			/* tab past left margin is undefined */
			if (tabcol < 8)
				tabcol = 8;
			ntabs = (oldcol-tabcol+7)/8;
			if (ntabs <= 0)
				tabcol = oldcol;
			if (tabcol < newcol) {
				/* some backtabs plus some rights */
				nright = newcol - tabcol;
				c1 = ntabs*_cost(Back_tab) + nright*_cost(Cursor_right);
			} else {
				/* some tabs plus some lefts */
				nleft = tabcol - newcol;
				c1 = ntabs*_cost(Back_tab) + nleft*_cost(Cursor_left);
			}
		} else {
			c1 = (oldcol - newcol) * _cost(Cursor_left);
		}
	} else
		c1 = INFINITY;
	if (parm_left_cursor)
		c2 = _cost(Parm_left_cursor);
	else
		c2 = INFINITY;
	if (column_address)
		c3 = _cost(Column_address);
	else
		c3 = INFINITY;
	
#ifdef DEBUG
	if(outf) fprintf(outf, "_loc_left(%d, %d, %d), le %d, LE %d, ch %d\n",
		oldcol, newcol, domotion, c1, c2, c3);
#endif

	/* Decide and maybe do them */
	if (c3 <= c1 && c3 <= c2) {
		/* cheapest to use column absolute cursor addressing */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose column absolute cursor addressing\n");
#endif
		if (domotion)
			tputs(tparm(column_address, newcol), 1, _outch);
		return c3;
	} else if (c2 <= c1) {
		/* cheapest to use column relative motion */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose column relative motion\n");
#endif
		if (domotion)
			tputs(tparm(parm_left_cursor, oldcol-newcol), 1, _outch);
		return c2;
	} else {
		/* cheapest to use several left commands */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose several left commands\n");
#endif
		if (domotion)
			if (back_tab) {
				for (i=0; i<ntabs; i++)
					tputs(back_tab, 1, _outch);
				if (tabcol < newcol) {
					/* some tabs plus some rights */
					for (i=0; i<nright; i++)
						tputs(cursor_right,1, _outch);
				} else {
					/* some tabs plus some lefts */
					for (i=0; i<nleft; i++)
						tputs(cursor_left,1,_outch);
				}
			} else {
				for (i=oldcol; i>newcol; i--)
					tputs(cursor_left, 1, _outch);
			}
		return c1;
	}
}

static
_loc_up(oldrow, newrow, domotion)
{
	int c1, c2, c3, i;

	if (newrow > oldrow)	/* can't go down with up motions */
		return INFINITY;
	if (newrow == oldrow)
		return 0;	/* already there - nothing to do */

	/* figure out various costs */
	if (cursor_up)
		c1 = (oldrow - newrow) * _cost(Cursor_up);
	else
		c1 = INFINITY;
	if (parm_up_cursor)
		c2 = _cost(Parm_up_cursor);
	else
		c2 = INFINITY;
	if (row_address)
		c3 = _cost(Row_address);
	else
		c3 = INFINITY;
	
#ifdef DEBUG
	if(outf) fprintf(outf, "_loc_up(%d, %d, %d), up %d, UP %d, cv %d\n",
		oldrow, newrow, domotion, c1, c2, c3);
#endif

	/* Decide and maybe do them */
	if (c3 <= c1 && c3 <= c2) {
		/* cheapest to use row absolute cursor addressing */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose row absolute cursor addressing\n");
#endif
		if (domotion)
			tputs(tparm(row_address, newrow), 1, _outch);
		return c3;
	} else if (c2 <= c1) {
		/* cheapest to use row relative motion */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose row relative motion\n");
#endif
		if (domotion)
			tputs(tparm(parm_up_cursor, oldrow-newrow), 1, _outch);
		return c2;
	} else {
		/* cheapest to use several up commands */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose several up commands\n");
#endif
		if (domotion)
			for (i=oldrow; i>newrow; i--)
				tputs(cursor_up, 1, _outch);
		return c1;
	}
}

static
_loc_down(oldrow, newrow, domotion, col)
{
	int c1, c2, c3, i;

	if (newrow < oldrow)	/* can't go up with down motions */
		return INFINITY;
	if (newrow == oldrow)
		return 0;	/* already there - nothing to do */

	/* figure out various costs */
	if (cursor_down && (col==0 || bare_lf_ok || *cursor_down!='\n'))
		c1 = (newrow - oldrow) * _cost(Cursor_down);
	else
		c1 = INFINITY;
	if (parm_down_cursor)
		c2 = _cost(Parm_down_cursor);
	else
		c2 = INFINITY;
	if (row_address)
		c3 = _cost(Row_address);
	else
		c3 = INFINITY;
	
#ifdef DEBUG
	if(outf) fprintf(outf, "_loc_down(%d, %d, %d, %d), do %d, DO %d, cv %d\n",
		oldrow, newrow, domotion, col,c1, c2, c3);
#endif

	/* Decide and maybe do them */
	if (c3 <= c1 && c3 <= c2) {
		/* cheapest to use row absolute cursor addressing */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose row absolute cursor addressing\n");
#endif
		if (domotion)
			tputs(tparm(row_address, newrow), 1, _outch);
		return c3;
	} else if (c2 <= c1) {
		/* cheapest to use row relative motion */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose row relative motion\n");
#endif
		if (domotion)
			tputs(tparm(parm_down_cursor, newrow-oldrow), 1, _outch);
		return c2;
	} else {
		/* cheapest to use several down commands */
#ifdef DEBUG
		if(outf) fprintf(outf, "chose several down commands\n");
#endif
		if (domotion)
			for (i=oldrow; i<newrow; i++)
				tputs(cursor_down, 1, _outch);
		return c1;
	}
}
