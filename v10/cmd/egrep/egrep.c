#include	"hdr.h"
#include	"y.tab.h"

cgotofn() {
	register i;
	count = cntpos = 0;
	nxtpos = 0;
	nxtfoll = MAXPOS-1;
	begout = 0;
	for (i=1; i<=line; i++) colpos[i] = 0;
	if (first(line-1)==0) {
		colpos[line] = 1;
		cntpos++;
		begout = 1;
	}
	for (i=1; i<=line; i++) tmpstat[i] = begstat[i] = colpos[i];
	count = begcnt = cntpos-1;	/* leave out position 1 */
	tmpstat[1] = begstat[1] = 0;
	addstate(1);
	inxtpos = nxtpos;
	istat = nxtst(states+1,LEFT);
}

State *
nxtst(ss,c)
State *ss;
{
	register i, num, k;
	int pos, curpos, number, newpos;
	int s = ss-states;
#ifdef	DOSTATS
	ntrans++;
#endif
	num = positions[state[s]];
	count = begcnt;
	for (i=3; i<=line; i++) tmpstat[i] = begstat[i];
	pos = state[s] + 1;
	for (i=0; i<num; i++) {
		curpos = positions[pos];
		if ((k = name[curpos]) >= 0) {
			if (
				(k == c)
				| (k == DOT && c != LEFT && c != RIGHT)
				| (k == CCL && member(c, right[curpos], 1))
				| (k == NCCL && member(c, right[curpos], 0) && c != LEFT && c != RIGHT)
			) {
				if (foll[curpos] == 0) {
					cntpos = 0;
					for (k=1; k<=line; k++) colpos[k] = 0;
#ifdef	DOSTATS
					nfollow++;
#endif
					follow(curpos);
					addfoll(curpos);
				}
				number = positions[foll[curpos]];
				newpos = foll[curpos] - 1;
				for (k=0; k<number; k++) {
					if (tmpstat[positions[newpos]] != 1) {
						tmpstat[positions[newpos]] = 1;
						count++;
					}
					newpos--;
				}
			}
		}
		pos++;
	}
	if (notin(nstate)) {
		if (++nstate >= NSTATES) {
			nxtpos = inxtpos;
			reinit = 1;
			nstate = 1;
			addstate(1);
			return states+1;
		}
		addstate(nstate);
		return(states[s].gotofn[c] = states+nstate);
	}
	else {
		return(states[s].gotofn[c] = states+xstate);
	}
}

first(v) {
	register b;
	if (left[v] == 0) {
		if (colpos[v] != 1) {
			colpos[v] = 1;
			cntpos++;
		}
		return(1);
	}
	else if (right[v] == 0) {
		if (first(left[v]) == 0) return (0);
		else if (name[v] == PLUS) return (1);
		else return (0);
	}
	else if (name[v] == CAT) {
		if (first(left[v]) == 0 && first(right[v]) == 0) return (0);
		else return (1);
	}
	else { /* name[v] == OR */
		b = first(right[v]);
		if (first(left[v]) == 0 || b == 0) return (0);
		else return (1);
	}
}

member(symb, set, torf) {
	register i, num, pos;
	num = chars[set];
	pos = set + 1;
	for (i=0; i<num; i++)
		if (symb == chars[pos++]) return (torf);
	return (!torf);
}

notin(n) {
	register i, j, pos;
	for (i=1; i<=n; i++) {
		if (positions[state[i]] == count) {
			pos = state[i] + 1;
			for (j=0; j < count; j++) {
				if (tmpstat[positions[pos++]] != 1) goto nxt; }
			xstate = i;
			return (0);
		}
		nxt: ;
	}
	return (1);
}

addstate(n) {
	register i;
	if (nxtpos + count >= nxtfoll) {
		overflo(); }
	for (i=0; i<NCHARS; i++)
		states[n].gotofn[i] = 0;
	state[n] = nxtpos;
	positions[nxtpos++] = count;
	for (i=3; i <= line; i++) {
		if (tmpstat[i] == 1) {
			positions[nxtpos++] = i;
		}
	}
	if (tmpstat[line] == 1)
		states[n].out = 1;
	else
		states[n].out = 0;
}

addfoll(n) {
	register i;
	if (nxtfoll - cntpos <= nxtpos) {
		overflo(); }
	foll[n] = nxtfoll;
#ifdef	DOSTATS
	if(nxtfoll > nmaxfoll) nmaxfoll = nxtfoll;
#endif
	positions[nxtfoll--] = cntpos;
	for (i=3; i <= line; i++) {
		if (colpos[i] == 1) {
			positions[nxtfoll--] = i;
		}
	}
}

follow(v) int v; {
	int p;
	if (v == line) return;
	p = parent[v];
	switch(name[p]) {
		case STAR:
		case PLUS:	first(v);
				follow(p);
				return;

		case OR:
		case QUEST:	follow(p);
				return;

		case CAT:	if (v == left[p]) {
					if (first(right[p]) == 0) {
						follow(p);
						return;
					}
				}
				else follow(p);
				return;
		case FINAL:	if (colpos[line] != 1) {
					colpos[line] = 1;
					cntpos++;
				}
				return;
	}
}

clearg() {
	register i;
	reinit = 0;
	states[1].out = begout;
	for (i=0; i<NCHARS; i++)
		states[1].gotofn[i] = 0;
	nstate = 1;
	state[1] = 0;
	istat = nxtst(states+1,LEFT);
}

execute(file)
char *file;
{
	register char *p;
	register State *cstat, *t;
	int len;
	char *nlp;
	int f;

#define	READLINE	if((p = nlp = Frdline(f)) == 0)\
				goto done;\
			else	/* Frdline nulls the \n, put it back */\
				len = FIOLINELEN(f), nlp[len++] = RIGHT

	if (file) {
		if ((f = open(file, 0)) < 0) {
			fprint(2, "%s: can't open %s\n", progname, file);
			badbotch=1;
			return;
		}
	}
	else f = 0;
	Finit(f, (char *)0);
	Ftie(f, 1);		/* link input f with stdout */
	lnum = 1;
	tln = 0;
	READLINE;
	cstat = istat;
	if (cstat->out) goto found;
	for (;;) {
		if ((t = cstat->gotofn[*(unsigned char *)p]) == 0)
			cstat = nxtst(cstat, *(unsigned char *)p);
		else
			cstat = t;
		if (cstat->out) {
		found:
			if (vflag == 0) {
		succeed:	nsucc = 1;
				if (cflag) tln++;
				else if (sflag){
					if(scanexit) exit(0);
				} else if (lflag) {
					Fprint(1, "%s\n", file);
					close(f);
					return;
				}
				else {
					if (nfile > 1 && hflag)
						Fprint(1, "%s:", file);
					if (bflag)
						Fprint(1, "%ld:", (FIOSEEK(f)-len)/BLKSIZE);
					if (nflag)
						Fprint(1, "%ld:", lnum);
					Fwrite(1, nlp, len);
				}
			}
			lnum++;
			READLINE;
			if (reinit == 1) clearg();
			if ((cstat = istat)->out)
				goto found;	/* we are a match already */
			else
				continue;	/* normal pattern matching loop */
		}
		if (*p++ == RIGHT) {
			if (vflag) goto succeed;
			else {
				lnum++;
				READLINE;
				if (reinit == 1) clearg();
				if ((cstat = istat)->out)
					goto found;	/* we are a match already */
			}
		}
	}
done:
#ifdef	DOSTATS
	nbytes += FIOSEEK(f);
	nlines += lnum-1;
#endif
	close(f);
	if (cflag) {
		if (nfile > 1)
			Fprint(1, "%s:", file);
		Fprint(1, "%ld\n", tln);
	}
}
