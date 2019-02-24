/*	@(#) vidputs.c: 1.1 10/15/83	(1.8	10/27/82)	*/
#include "curses.ext"

static int oldmode = 0;	/* This really should be in the struct term */
char *tparm();

/* nooff: modes that don't have an explicit "turn me off" capability */
#define nooff	(A_PROTECT|A_INVIS|A_BOLD|A_DIM|A_BLINK|A_REVERSE)
/* hilite: modes that could be faked with standout in a pinch. */
#define hilite	(A_UNDERLINE|A_BOLD|A_DIM|A_BLINK|A_REVERSE)

vidputs(newmode, outc)
int newmode;
int (*outc)();
{
	int curmode = oldmode;

#ifdef DEBUG
	if (outf) fprintf(outf, "vidputs oldmode=%o, newmode=%o\n", oldmode, newmode);
#endif
	if (newmode || !exit_attribute_mode) {
		if (set_attributes) {
			tputs(tparm(set_attributes,
					newmode & A_STANDOUT,
					newmode & A_UNDERLINE,
					newmode & A_REVERSE,
					newmode & A_BLINK,
					newmode & A_DIM,
					newmode & A_BOLD,
					newmode & A_INVIS,
					newmode & A_PROTECT,
					newmode & A_ALTCHARSET),
				1, outc);
			curmode = newmode;
		} else {
			if ((oldmode&nooff) > (newmode&nooff)) {
				if (exit_attribute_mode) {
					tputs(exit_attribute_mode, 1, outc);
				} else if (oldmode == A_UNDERLINE && exit_underline_mode) {
					tputs(exit_underline_mode, 1, outc);
				} else if (exit_standout_mode) {
					tputs(exit_standout_mode, 1, outc);
				}
				curmode = oldmode = 0;
			}
			if ((newmode&A_ALTCHARSET) && !(oldmode&A_ALTCHARSET)) {
				tputs(enter_alt_charset_mode, 1, outc);
				curmode |= A_ALTCHARSET;
			}
			if (!(newmode&A_ALTCHARSET) && (oldmode&A_ALTCHARSET)) {
				tputs(exit_alt_charset_mode, 1, outc);
				curmode &= ~A_ALTCHARSET;
			}
			if ((newmode&A_PROTECT) && !(oldmode&A_PROTECT)) {
				tputs(enter_protected_mode, 1, outc);
				curmode |= A_PROTECT;
			}
			if ((newmode&A_INVIS) && !(oldmode&A_INVIS)) {
				tputs(enter_secure_mode, 1, outc);
				curmode |= A_INVIS;
			}
			if ((newmode&A_BOLD) && !(oldmode&A_BOLD))
				if (enter_bold_mode) {
					curmode |= A_BOLD;
					tputs(enter_bold_mode, 1, outc);
				}
			if ((newmode&A_DIM) && !(oldmode&A_DIM)) 
				if (enter_dim_mode) {
					curmode |= A_DIM;
					tputs(enter_dim_mode, 1, outc);
				}
			if ((newmode&A_BLINK) && !(oldmode&A_BLINK)) 
				if (enter_blink_mode) {
					curmode |= A_BLINK;
					tputs(enter_blink_mode, 1, outc);
				}
			if ((newmode&A_REVERSE) && !(oldmode&A_REVERSE)) 
				if (enter_reverse_mode) {
					curmode |= A_REVERSE;
					tputs(enter_reverse_mode, 1, outc);
				}
			if ((newmode&A_UNDERLINE) && !(oldmode&A_UNDERLINE)) 
				if (enter_underline_mode) {
					curmode |= A_UNDERLINE;
					tputs(enter_underline_mode,1,outc);
				}
			if (!(newmode&A_UNDERLINE) && (oldmode&A_UNDERLINE)) {
				tputs(exit_underline_mode, 1, outc);
				curmode &= ~A_UNDERLINE;
			}
			if ((newmode&A_STANDOUT) && !(oldmode&A_STANDOUT)) 
				if (enter_standout_mode) {
					curmode |= A_STANDOUT;
					tputs(enter_standout_mode,1,outc);
				}
			if (!(newmode&A_STANDOUT) && (oldmode&A_STANDOUT)) {
				tputs(exit_standout_mode, 1, outc);
				curmode &= ~A_STANDOUT;
			}
		}
	} else {
		if (exit_attribute_mode)
			tputs(exit_attribute_mode, 1, outc);
		else if (oldmode == A_UNDERLINE && exit_underline_mode)
			tputs(exit_underline_mode, 1, outc);
		else if (exit_standout_mode)
			tputs(exit_standout_mode, 1, outc);
		curmode = 0;
	}
	/*
	 * If we asked for bold, say, on a terminal with only standout,
	 * and we aren't already in standout, we settle for standout.
	 */
	if ((newmode&hilite) && curmode!=newmode && (curmode&A_STANDOUT)==0) {
		tputs(enter_standout_mode, 1, outc);
		curmode |= A_STANDOUT;
	}
	oldmode = curmode;
}
