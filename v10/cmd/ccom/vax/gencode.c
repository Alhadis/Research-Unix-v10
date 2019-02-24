#include "gencode.h"
#define fieldbotch(p) if(p->left->op == Fld) {rewritefld(p); longjmp(back, 1);}
jmp_buf back;
int acnt, Pflag, bbcnt;
NODE *svq;		/* just for printing at FAIL */

gencode(p)
NODE *p;
{	mnod *q;
	ret s;
	int svtemp, svregvar, i, svbb;
	extern int bothdebug;
	svtemp = tmpoff;
	svregvar = regvar;
	svbb = ++bbcnt;
	svq = p;
	if(setjmp(back)) {
		pr("#\treg\t%d\n", ++acnt) /*,prtree(q), putchar('\n')*/;
		if(acnt > 20) {
			prtree(q);
			outpr();
			tmpoff = svtemp;
			bbcnt = svbb;
			uerror("expression too complicated");
			return;
		}
	}
	else
		q = copytree(p);
	buf = bufs[1];
	prptr = prbuf;
	if(Pflag && q->op != Init) {
		pr("#\tincl\tlocprof+%d\n", 4*(svbb+3));
	}
	s = doit(q, 0, 0, REGMASK);
	if(s.flag & FAIL)
		uerror("codegen failed at top level");
	acnt = ntree = 0;
	if(Pflag && q->op != Init)
		printx("#%d ", svbb), prtree(q), printx("\n");
	outpr();
	tmpoff = svtemp;
	regvar = svregvar;
}

ret
doit(p, flag, dest, regmask)
mnod *p;
ret dest;
{	ret s, t, x, y;
	char *pp;
	mnod snode, *q;
	int i, j, svmask = regmask;
	switch(p->op) {
	default:
		pr("#\tweird??? %d\n", p->op);
		return(dest);
	case Andeq:
		fieldbotch(p)
		if(dest.ans && p->left->op == Star)
			longjmp(back, mediumstar(p));
		flag |= DESTISLEFT;
		flag &= ~CC;
	case And:
		if((flag & CC)) {	/* short b; reg int *c; if(b & *c++); */
			if(p->left->op == Conv && incrsize(p->left->left) < 4)
				p->left = p->left->left;
			if(p->right->op == Conv
				&& incrsize(p->right->left) < 4)
				p->right = p->right->left;
		}
		t = doit(p->left, VALUE|USED, 0, regmask);
		if(t.flag & FAIL)
			return(t);
		regmask &= ~t.regmask;
		s = doit(p->right, VALUE|USED, 0, regmask);
		if(s.flag & FAIL)
			goto binfail;
		if(flag & CC) {
			if(incrsize(p->left) == incrsize(p->right))
				i = type(p->right);
			else if(incrsize(p->left) < incrsize(p->right)) {
				if(s.flag & INDEX) {
					totemp(p, RIGHT);
					longjmp(back, 1);
				}
				i = childtype(p);
			}
			else {
				if(t.flag & INDEX) {
					totemp(p, LEFT);
					longjmp(back, 1);
				}
				i = type(p->right);
			}
			pr("#\tbit%c\t%s,%s\n", i, str(s), str(t));
			dest.ans = 0;
			dest.flag = CC;
			dest.regmask = 0;
			return(dest);
		}
		regmask &= ~s.regmask;
		/* p->right->op == Compl is a useful special case */
		if((flag & DESTISLEFT) && dest.ans == 0)
			dest = x = t;
		if(dest.ans == 0)
			if(s.flag & SCRATCH)
				dest = x = s;
			else if(t.flag & SCRATCH) {
				dest = t;
				x = allocreg(p, regmask);
			}
			else
				dest = x = allocreg(p, regmask);
		else
			x = allocreg(p, regmask);
		if(p->right->op == Icon) {
			x = tostack();	/* to get a buf */
			sprintx(str(x), "$%d", -p->right->lval - 1);
		}
		else
			pr("#\tmcom%c\t%s,%s\n", childtype(p), str(s), str(x));
		if(strcmp(str(t), str(dest)) == 0)
			pr("#\tbic%c2\t%s,%s\n", childtype(p), str(x), str(dest));
		else if(flag & DESTISLEFT) {
			pr("#\tbic%c2\t%s,%s\n", childtype(p), str(x), str(t));
			if(strcmp(str(t), str(dest))) {
				x = t;
				goto movexdest;
			}
		}
		else
			pr("#\tbic%c3\t%s,%s,%s\n", childtype(p), str(x),
				str(t), str(dest));
		if(flag & ASADDR)
			goto toaddr;
		dest.flag |= CC;
		return(dest);
	case Call:
call:
		s.flag = funargs(p->right, regmask);
		if(s.flag & FAIL)
			return(s);
		i = p->argsize/32;
		if(i > 255)
			uerror("%d arguments is too many", i);
called:
		s = doit(p->left, VALUE|ASADDR|USED, 0, regmask);
		pp = str(s);
aftercall:
		if(svmask != REGMASK) {
			debugpr("#FAIL aftercall ");
			s.flag = FAIL;
			return(s);
		}
		x = specialreg(p, regmask);
		pr("#\tcalls\t$%d,%s\n", i, pp);
		if(flag & ASADDR) {
			strcat(str(x), ")");
			strshift(str(x), 1);
			str(x)[0] = '(';
		}
		if(dest.ans == 0)
			if(flag & DESTISLEFT)
				dest = doit(p->left->op == Conv?
					p->left->left:
					p->left, 0/* ? */, 0, regmask & ~x.regmask);
			else
				return(x);
movexdest:	/* type(p), not childtype, for a = a % b */
		if(strcmp(str(x), str(dest)) == 0)
			return(x);
		if((flag & TOSTACK) && incrsize(p) == 4)
			pr("#\tpush%c\t%s\n", type(p), str(x));
		else if(x.flag & ICON0)
			pr("#\tclr%c\t%s\n", type(p), str(dest));
		else if(isfloat(p) != isfloat(p->left)
			&& p->op != Call && p->op != Ucall)
			pr("#\tcvt%c%c\t%s,%s\n", childtype(p), type(p), str(x), str(dest));
		else if((flag & TOSTACK) && (incrsize(p) != 8))
			pr("#\tcvt%cl\t%s,-(sp)\n", type(p), str(x));
		else {
			pr("#\tmov%c\t%s,%s\n", type(p), str(x), str(dest));
			dest = simpler(x, dest);
		}
		dest.flag |= CC;
		return(dest);
	case Cmp:
		if(p->left->op == Conv && p->right->op == Conv
			&& childtype(p->left) == childtype(p->right)
			&& !isfloat(p->left->left)) {	/* bogus? */
			p->left = p->left->left;
			p->right = p->right->left;
		}
		else if(p->left->op == Conv && p->right->op == Icon
			&& p->right->lval >= 0
			&& ((incrsize(p->left->left) == 1
				&& p->right->lval < 128)
			|| (incrsize(p->left->left) == 2
				&& p->right->lval < 32768)))
			p->left = p->left->left;
		/* the above rewriting depends on childype being of p->left */
		s = doit(p->left, VALUE|USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		regmask &= ~s.regmask;
		t = doit(p->right, VALUE|USED, 0, regmask);
		if(t.flag & FAIL) {
			t = doit(p->right, VALUE|USED, 0, REGMASK);
			if(!(t.flag & FAIL) && svmask != REGMASK)
				return(t.flag = FAIL, t);
			totemp(p, LEFT);
			longjmp(back, 1);
		}
		pr("#\tcmp%c\t%s,%s\n", childtype(p), str(s), str(t));
		dest.ans = dest.regmask = 0;
		dest.flag = CC;
		return(dest);
	case Comop:	/* qnodes lurking underneath */
		if(p->left->op == Genlab && hasqnode(p->right)) {
			if(dest.ans == 0) {
				dest = allocreg(p, regmask);
			}
			t = doit(p->left, VALUE|(flag & CC), dest, regmask);
		}
		else
			t = doit(p->left, 0, 0, regmask);
		if(t.flag & FAIL)
			return(t);
		s = doit(p->right, VALUE|(flag & (USED|CC)), dest, regmask);
		if(s.flag & FAIL)
			return(s);
		if(flag & ASADDR) {
			dest.ans = 0;
			i = (s.flag & (ISREG|CANINDIR|SCRATCH));
			j = s.regmask;
			buf = str(s);
			goto convbuf;
		}
		return(s);
	case Compl:
		s = doit(p->left, VALUE|USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		if(dest.ans == 0)
			if(s.flag & SCRATCH)
				dest = s;
			else
				dest = allocreg(p, regmask & ~s.regmask);
		pr("#\tmcom%c\t%s,%s\n", childtype(p), str(s), str(dest));
		dest.flag |= CC;
		return(dest);
	case Conv:
		if(p->left->op == Asg && p->left->left->op == Star) {
			if(dest.ans == 0) {
				dest = allocreg(p, regmask);
				s = doit(p->left, VALUE|USED, dest, regmask);
			}
			else {
				x = allocreg(p, regmask);
				s = doit(p->left, VALUE|USED, x, regmask & ~x.regmask);
			}
			if(s.flag & FAIL) {
				asgwrite(p->left);
				longjmp(back, 1);
			}
		}
		else
			s = doit(p->left, VALUE|USED, 0, regmask);
		if(childtype(p) == type(p) && dest.ans == 0)
			return(s);
		if(s.flag & FAIL)
			return(s);
		if(dest.ans == 0)
			if(s.flag & SCRATCH)
				dest = checksize(p, s, regmask);
			else
				dest = allocreg(p, regmask);
		if(isunsigned(p->left) && incrsize(p) > incrsize(p->left)) {
			if(type(p) != 'f' && type(p) != 'd')
				pp = "movz";
			else {	/* uns to float or double */
				rewriteconv(p);
				longjmp(back, 1);
			}
		}
		else
			pp = "cvt";
		if(isfloat(p) != isfloat(p->left))
			goto cvtop;
		if(incrsize(p) < incrsize(p->left)) {
			if(isfloat(p))
				goto cvtop;
			pr("#\tmov%c\t%s,%s\n", type(p), str(s), str(dest));
			dest = simpler(s, dest);
		}
		else if(incrsize(p) > incrsize(p->left))
cvtop:
			pr("#\t%s%c%c\t%s,%s\n", pp, childtype(p), type(p), str(s), str(dest));
		else {	/* types were the same, but dest.ans != 0 */
			pr("#\tmov%c\t%s,%s\n", type(p), str(s), str(dest));
			dest = simpler(s, dest);
		}
		dest.flag |= CC;
		return(dest);
	case Decr:
		i = -1;
		pp = "sub";
incrop:
		if(p->type == Tfloat)
			uerror("no float ++/--");
		fieldbotch(p)
		/* if the dest uses regs, it may be from a qnode, so that reg
		 * shouldn't be used (i?*a->b++:x()) where b has offset > 0
		 */
		s = doit(p->left, USED, 0, dest.ans? regmask & ~dest.regmask: regmask);
		if(s.flag & FAIL)
			return(s);
		t = doit(p->right, 0, 0, 0);
		if(flag & VALUE) {
			if(dest.ans == 0)
				dest = allocreg(p, regmask & ~s.regmask);
			pr("#\tmov%c\t%s,%s\n", childtype(p), str(s), str(dest));
			if(t.flag & ICON1) {
				if(i == 1)
					pp = "inc";
				else
					pp = "dec";
				pr("#\t%s%c\t%s\n", pp, childtype(p), str(s));
			}
			else
				pr("#\t%s%c2\t%s,%s\n", pp, childtype(p),
					str(t), str(s));
toaddr:
			if(flag & ASADDR) {
				if(dest.flag & ISREG) {
					sprintx(buf, "(%s)", str(dest));
					done(dest, CANINDIR, dest.regmask);
				}
				if(dest.flag & CANINDIR) {
					sprintx(buf, "*%s", str(dest));
					done(dest, 0, dest.regmask);
				}
				uerror("weird asaddr in incrop");
			}
			dest.flag &= ~CC;
			return(dest);
		}
		if(t.flag & ICON1) {
			if(i == 1)
				pp = "inc";
			else
				pp = "dec";
			pr("#\t%s%c\t%s\n", pp, childtype(p), str(s));
		}
		else
			pr("#\t%s%c2\t%s,%s\n", pp, childtype(p), str(t), str(s));
		if(dest.ans) {
			x = s;
			goto movexdest;
		}
		s.flag &= ~CC;
		return(s);
	case Diveq:
		fieldbotch(p)
		if(rewriteasgop(p))
			goto assign;
		flag |= DESTISLEFT;
	case Div:
		/* trees wrong: uns/=int and int/=uns */
		if(!isunsigned(p->right) && !isunsigned(p->left)) {
			pp = "div";
			goto binop;
		}
		pp = "udiv";
unsdiv:
		/* *p++ /= uns is hard */
		if(p->left->op == Star && fixuns(p))
			longjmp(back, 1);	/* *A op= B => (T=A, *T op= B) */
		if(incrsize(p->right) != 4) {
			snode.op = Conv;
			snode.left = p->right;
			snode.type = Tulong;
			s = doit(&snode, VALUE|TOSTACK, tostack(), regmask);
		}
		else
			s = doit(p->right, TOSTACK, tostack(), regmask);
		if(s.flag & FAIL)
			return(s);
		regmask &= ~s.regmask;
		if(incrsize(p->left) != 4) {		
			snode.op = Conv;
			snode.left = p->left;
			snode.type = Tulong;
			t = doit(&snode, VALUE|TOSTACK, tostack(), regmask);
		}
		else
			t = doit(p->left, VALUE|TOSTACK, tostack(), regmask);
		if(t.flag & FAIL) {
			if(dest.ans || svmask != REGMASK) {
				t = doit(p->left, VALUE|TOSTACK, tostack(), svmask);
				if(t.flag & FAIL)
					return(t);
			}
			totemp(p, RIGHT);
			longjmp(back, 1);
		}
		i = 2;
		goto aftercall;
	case Xoreq:
		fieldbotch(p)
		flag |= DESTISLEFT;
	case Xor:
		pp = "xor";
binop:
		if((flag & DESTISLEFT) && dest.ans && p->left->op == Star)
			longjmp(back, mediumstar(p));
		t = doit(p->left, VALUE|USED, 0, regmask);
		if(t.flag & FAIL) {
			return(t);
		}
		regmask &= ~t.regmask;
		s = doit(p->right, VALUE|USED, 0, regmask);
		if(s.flag & FAIL) {
binfail:
			if(dest.ans || svmask != REGMASK) {
				s = doit(p->right, VALUE|USED, 0, svmask);
				if(s.flag & FAIL)
					return(s);
			}
			/* *foo op= expr  requires care */
			if((flag & DESTISLEFT) && p->left->op == Star)
				totemp(p->left, LEFT);
			else
				totemp(p, LEFT);
			longjmp(back, 1);
		}
		if(type(p) != childtype(p)) {
			x = allocreg(p, regmask);
			pr("#\t%s%c3\t%s,%s,%s\n", pp, childtype(p), str(s),
				str(t), str(x));
			if(dest.ans == 0)
				if(flag & DESTISLEFT)
					dest = t;
				else
					dest = x;
			if(!isfloat(p) && !isfloat(p->left)
				&& incrsize(p) < incrsize(p->left)) {
				pr("#\tmov%c\t%s,%s\n", type(p), str(x), str(dest));
				dest = simpler(x, dest);
			}
			else
				pr("#\tcvt%c%c\t%s,%s\n", childtype(p), type(p),
					str(x), str(dest));
			dest.flag |= CC;
			return(dest);
		}	
		if(dest.ans == 0)
			if((t.flag & SCRATCH) || (flag & DESTISLEFT)) {
twoop:
				dest = t;
				if(*pp == 'a' && (s.flag & ICON1))
					pr("#\tinc%c\t%s\n", childtype(p), str(t));
				else if(*pp == 's' && (s.flag & ICON1))
					pr("#\tdec%c\t%s\n", childtype(p), str(t));
				else
					pr("#\t%s%c2\t%s,%s\n", pp, childtype(p),
						str(s), str(t));
				if(flag & ASADDR)
					goto binopaddr;
				if(flag & TOSTACK)
					pr("#\tpush%c\t%s\n", type(p), str(t));
				dest.flag |= CC;
				return(dest);
			}
			else if(s.flag & SCRATCH)
				dest = s;
			else
				dest = allocreg(p, regmask);
		if(dest.ans && (flag & DESTISLEFT)) {
			pr("#\t%s%c2\t%s,%s\n", pp, childtype(p), str(s), str(t));
			x = t;
			goto movexdest;
		}
		if(commutes(p->op) && strcmp(str(s), str(dest)) == 0) {
			dest = s;
			s = t;
			t = dest;
			goto twoop;
		}
		if(strcmp(str(t), str(dest)) == 0)
			goto twoop;
			
		pr("#\t%s%c3\t%s,%s,%s\n", pp, childtype(p), str(s), str(t), str(dest));
binopaddr:
		if(flag & ASADDR) {
			if(dest.flag & ISREG) {
				strcat(str(dest), ")");
				strshift(str(dest), 1);
				str(dest)[0] = '(';
				return(dest);
			}
			if(dest.flag & CANINDIR) {
				strshift(str(dest), 1);
				str(dest)[0] = '*';
				dest.flag &= ~CANINDIR;
				return(dest);
			}
			debugpr("#FAIL doubtful ");
			dest.flag = FAIL;	/* doubtful */
			return(dest);
		}
		dest.flag |= CC;
		return(dest);	
	case Fld:
		/* this is rvalues */
		s = doit(p->left, USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		if(dest.ans == 0)
			dest = allocreg(p, regmask);
		if((s.flag & INDEX) && p->left->op == Star
			&& incrsize(p->left) != 1) {
		/* over-enthusiastic use of index mode */
			if(dest.flag & SCRATCH)
				x = dest;
			else
				x = allocreg(p, regmask);
			pr("#\tmov%c\t%s,%s\n", childtype(p), str(s), str(x));
			s = x;
		}
		pr("#\text%sv\t$%d,$%d,%s,%s\n", isunsigned(p->left)? "z": "",
			p->rval/64, p->rval%64, str(s), str(dest));
		dest.flag |= CC;
		return(dest);
	case Genbr:
		if(p->left->op == Conv)
			s = doit(p->left->left, CC|VALUE|USED, 0, regmask);
		/*else if(p->left->op == And && pow2(p->left->right->lval) > 0) {
			s = doit(p->left->left, CC|VALUE, 0, regmask);
			pp = genjbc(p->lop);
			pr("#\t%s\t$%d,%s,L%d\n", pp, pow2(p->left->right->lval),
				str(s), p->label);
			goto genbr0;
		} not a win on 8550 */
		else
			s = doit(p->left, CC|VALUE|USED, 0, regmask);
		pp = genjmp(p->lop);
		if(s.flag & CC)
			if(p->left->op != Conv || p->left->type >= Tint)
				pr("#\t%s\t", pp);
			else
				pr("#\ttst%c\t%s\n\t%s\t", childtype(p),
					str(s), pp);
		else if(p->left->op == Conv)
			pr("#\ttst%c\t%s\n#\t%s\t", childtype(p->left), str(s), pp);
		else
			pr("#\ttst%c\t%s\n#\t%s\t", childtype(p), str(s), pp);
		pr("L%d\n", p->label);
genbr0:
		s.flag |= CC;	/* ?? */
		if(Pflag) {
			++bbcnt;
			pr("#\tincl\tlocprof+%d\n", 4*(bbcnt+3));
		}
		return(s);
	case Genlab:
		s = doit(p->left, flag, dest, regmask);
		pr("#L%d:\n", p->label);
		if(Pflag) {
			++bbcnt;
			pr("#\tincl\tlocprof+%d\n", 4*(bbcnt+3));
		}
		return(s);
	case Genubr:
		s = doit(p->left, flag & CC, dest, regmask);
		if((flag & CC) && !(s.flag & CC))
			pr("#\ttst%c\t%s\n", childtype(p), str(s));
		pr("#\tjbr\tL%d\n", p->label);
		return(s);
	case Icon:
		if(p->name)
			if(p->lval)
				sprintx(buf, "%s+%d", p->name, p->lval);
			else
				sprintx(buf, "%s", p->name);
		else
			sprintx(buf, "%d", p->lval);
		if(!(flag & ASADDR)) {
			strshift(buf, 1);;
			buf[0] = '$';
		}
		i = 0;
		if(p->name == 0)
			if(p->lval == 0)
				i = ICON0;
			else if(p->lval == 1)
				i = ICON1;
		j = 0;
convbuf:	/* the cookie is in buf */
		if(dest.ans == 0 && !(flag & TOSTACK)) {
			if(p->op == Icon || !(flag & ASADDR)) {
				done(s, i, j);
			}
			if(i & CANINDIR) {
				strshift(buf, 1);
				buf[0] = '*';
				done(s, i & ~CANINDIR, j);
			}
			if(i & ISREG) {
				strcat(buf, ")");
				strshift(buf, 1);
				buf[0] = '(';
				done(s, i, j);
			}
			pp = buf;
			buf += BUF;
			s = allocreg(p, regmask);
			pr("#\tmov%c\t%s,%s\n", type(p), pp, str(s));
			flag &= ~ASADDR;
			goto inreg;
		}
		if(flag & TOSTACK)
			if(incrsize(p) == 4)
				pr("#\tpushl\t%s\n", buf);
			else if(incrsize(p) == 8)
				pr("#\tmovd\t%s,%s\n", buf, str(dest));
			else
				pr("#\tcvt%cl\t%s,%s\n", type(p), buf, str(dest));
		else if(i & ICON0)
			pr("#\tclr%c\t%s\n", type(p), str(dest));
		else {	
			pr("#\tmov%c\t%s,%s\n", type(p), buf, str(dest));
			if(!(dest.flag & INDEX) || !index(str(dest), '+')
				&& !index(str(dest), '-'))
				;
			else if(!(i & INDEX) || !index(buf, '+')
				&& !index(buf, '-')) {
				done(s, (i|CC), j);
			}
			else
				dest.flag |= USED;
		}
		dest.flag |= CC;
		return(dest);
	case Incr:
		i = 1;
		pp = "add";
		goto incrop;
	case Init:	/* knows it is ICON */
		s = doit(p->left, ASADDR, 0, 0);
		pr("#\t.long\t%s\n", str(s));
		return(dest);
	case Lseq:
		if(incrsize(p) != 4)
			lsconv(p);
		if(rewriteasgop(p))
			goto assign;
		fieldbotch(p)
		flag |= DESTISLEFT;
		if(dest.ans && p->left->op == Star)
			longjmp(back, mediumstar(p));
	case Ls:	/* stupid vax */
		if(incrsize(p) != 4)
			lsconv(p);
		s = doit(p->left, VALUE|USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		regmask &= ~s.regmask;
		t = doit(p->right, VALUE|USED, 0, regmask);
		if(t.flag & FAIL)
			goto binfail;
		if((t.flag & INDEX) && p->right->op == Star
			&& incrsize(p->right) != 1) {
		/* over-enthusiastic use of index mode */
			if(dest.flag & SCRATCH)
				x = dest;
			else
				x = allocreg(p, regmask);
			pr("#\tmov%c\t%s,%s\n", childtype(p), str(t), str(x));
			t = x;
		}
		if(dest.ans == 0)
			if((s.flag & SCRATCH) || (flag & DESTISLEFT))
				dest = s;
			else
				dest = allocreg(p, regmask);
		if(incrsize(p) != 4) {	/* stupid vax */
			x = allocreg(p, regmask);
			pr("#\tashl\t%s,%s,%s\n", str(t), str(s), str(x));
			pr("#\tmov%c\t%s,%s\n", type(p), str(x), str(dest));
			if(flag & DESTISLEFT)
				pr("#\tmovl\t%s,%s\n", str(x), str(s));
			dest.flag |= CC;
			return(dest);
		}
		if(p->right->op == Icon && (i = p->right->lval) <= 4
			&& (dest.flag & ISREG) && !(s.flag&AUTO)) {
			if(strcmp(str(s), str(dest))) {
				i--;
				pr("#\taddl3\t%s,%s,%s\n", str(s), str(s), str(dest));
			}
			while(i-- > 0)
				pr("#\taddl2\t%s,%s\n", str(dest), str(dest));
		}
		else if(flag & DESTISLEFT)
			pr("#\tashl\t%s,%s,%s\n", str(t), str(s), str(s));
		else
			pr("#\tashl\t%s,%s,%s\n", str(t), str(s), str(dest));
		if((flag & DESTISLEFT) && strcmp(str(dest), str(s)))
			pr("#\tmovl\t%s,%s\n", str(s), str(dest));
		dest.flag |= CC;
		return(dest);
	case Minuseq:
		fieldbotch(p)
		if(rewriteasgop(p))
			goto assign;
		flag |= DESTISLEFT;
	case Minus:
		pp = "sub";
		/* you won't believe this: type x[], *y where type is shorter than
		 * int, generates subx $_x,y which the assembler barfs on.
		 * instead, generate crummy code (stupid assembler)
		 */
		if(incrsize(p) < 4) {
			if(p->left->op == Icon && p->left->name) {
				p->left->type = Tint;
				totemp(p, LEFT);
			}
			if(p->right->op == Icon && p->right->name) {
				p->right->type = Tint;
				totemp(p, RIGHT);
			}
		}
		goto binop;
	case Modeq:
		fieldbotch(p)
		if(rewriteasgop(p))
			goto assign;
		if(dest.ans && p->left->op == Star)
			longjmp(back, mediumstar(p));
		flag |= DESTISLEFT;
	case Mod:
		if(isunsigned(p->right) || isunsigned(p->left)) {
			pp = "urem";
			goto unsdiv;
		}
		s = doit(p->left, VALUE|USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		regmask &= ~s.regmask;
		t = doit(p->right, VALUE|USED, 0, regmask);
		if(t.flag & FAIL)
			goto binfail;
		regmask &= ~t.regmask;
		x = allocreg(p, regmask);
		pr("#\tdiv%c3\t%s,%s,%s\n", childtype(p), str(t), str(s), str(x));
		pr("#\tmul%c2\t%s,%s\n", childtype(p), str(t), str(x));
		pr("#\tsub%c3\t%s,%s,%s\n", childtype(p), str(x), str(s), str(x));
		if(dest.ans)
			goto movexdest;
		if(flag & DESTISLEFT) {
			dest = s;
			goto movexdest;
		}
		x.flag |= CC;
		return(x);
	case Muleq:
		fieldbotch(p)
		if(rewriteasgop(p))
			goto assign;
		flag |= DESTISLEFT;
	case Mul:
		pp = "mul";
		goto binop;
	case Name:	/* ASADDR? */
		if(p->lval && p->name)
			sprintx(buf, "%s+%d", p->name, p->lval);
		else if(p->name)
			sprintx(buf, "%s", p->name);
		else if(p->lval)
			sprintf(buf, "%d", p->lval);
		else
			sprintx(buf, "0");
		j = 0;
		i = CANINDIR;
		goto convbuf;
	case Oreq:
		fieldbotch(p)
		flag |= DESTISLEFT;
	case Or:
		pp = "bis";
		goto binop;
	case Pluseq:
		fieldbotch(p)
		if(rewriteasgop(p))
			goto assign;
		flag |= DESTISLEFT;
	case Plus:
		pp = "add";
		goto binop;
	case Reg:
		sprintx(buf, "%s", regnames[p->rval]);
		j = 0;
		i = ISREG;
		if(p->lval == 1)
			i |= SCRATCH;
		goto convbuf;
	case Rnode: case Snode:
		x = specialreg(p, 3);
		x.regmask = 0;
		if(dest.ans)
			goto movexdest;
		return(x);
	case Qnode:
		return(specialreg(p, regmask));
	case Rseq:
		fieldbotch(p)
		if(incrsize(p) != 4)
			lsconv(p);
		if(rewriteasgop(p))
			goto assign;
		flag |= DESTISLEFT;
	case Rs:	/* all right shifts are unsigned */
		if(incrsize(p->left) != 4)	/* shifting funny objects */
			lsconv(p);
		s = doit(p->left, VALUE|USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		if(p->left->op == Icon ||
			(s.flag & INDEX) && p->left->op == Star
			&& incrsize(p->left) != 1 ||
			(s.flag & AUTO) && incrsize(p->left) != 1) {
		/* over-enthusiastic use of index mode */
		/* 12 >> i generates a byte immediate */
			if((s.flag & INDEX) && (flag & DESTISLEFT)) {/* *a++ >>= 1 */
				lsconv(p);	/* not best */
				longjmp(back, 1);
			}
			if((flag & DESTISLEFT) && dest.ans == 0) {
				dest = s;
				regmask &= ~s.regmask;
			}
			if(dest.flag & SCRATCH)
				x = dest;
			else
				x = allocreg(p, regmask);
			pr("#\tmovl\t%s,%s\n", str(s), str(x));
			s = x;
		}
		regmask &= ~s.regmask;
		if(dest.ans == 0)
			if((flag & DESTISLEFT) || (s.flag & SCRATCH))
				dest = s;
			else
				dest = allocreg(p, regmask);
		dest.flag |= CC;
		regmask &= ~dest.regmask;
		if(incrsize(p) != 4)
			x = allocreg(p, regmask);
		else if(flag & DESTISLEFT)
			x = s;
		else
			x = dest;
		if(p->right->op == Icon && (i = p->right->lval) >= 0) {
			pr("#\textzv\t$%d,$%d,%s,%s\n", i, 32 - i, str(s), str(x));
			goto movexdest;
		}
		t = doit(p->right, VALUE|USED, 0, regmask);
		if(t.flag & FAIL)
			goto binfail;
		y = allocreg(p, regmask & ~t.regmask);
		pr("#\tsubl3\t%s,$32,%s\n", str(t), str(y));
		pr("#\textzv\t%s,%s,%s,%s\n", str(t), str(y), str(s), str(x));
		goto movexdest;
	case Star:
		switch(p->left->op) {
		case Icon:	/* as in foo((a, a)), where a is a struct */
			s = doit(p->left, VALUE, 0, regmask);
			sprintx(buf, "%s", str(s)+1);
			i = j = 0;
			goto convbuf;
		case Auto:
		case Param:
		case Name:
			s = doit(p->left, VALUE, 0, regmask);
			sprintx(buf, "*%s", str(s));
			j = i = 0;
			goto convbuf;
		case Reg:
			s = doit(p->left, VALUE, 0, regmask);
inreg:
			if(s.flag & ISREG) {
				sprintx(buf, "(%s)", str(s));
				j = s.regmask;
				i = CANINDIR;
				goto convbuf;
			}
			else if(s.flag & CANINDIR) {
				sprintx(buf, "*%s", str(s));
				j = s.regmask;
				i = 0;
				goto convbuf;
			}
			else {
				debugpr("#FAIL STAR REG ");
				s.flag = FAIL;
				return(s);
			}
		}
		q = p->left;
		if(q->op == Incr && !(flag & INDEX)
			&& q->right->op == Icon && q->left->op == Reg
			&& incrsize(p) == (int) q->right->lval) {
			s = doit(q->left, VALUE, 0, regmask);
			sprintx(buf, "(%s)+", str(s));
			i = INDEX|AUTO;
			j = s.regmask;
			goto convbuf;
		}
		if(q->op == Plus && q->left->op == Ls
			&& !(flag & INDEX)
			&& q->left->right->op == Icon
			&& shiftsize(p) == (int) q->left->right->lval) {
			s = doit(q->left->left, VALUE|USED, 0, regmask);
			if(s.flag & FAIL)
				return(s);
			regmask &= ~s.regmask;
			if(!(s.flag & ISREG)) {
				x = allocreg(p, regmask);
				pr("#\tmov%c\t%s,%s\n", childtype(q), str(s), str(x));
				regmask |= s.regmask;
				regmask &= ~x.regmask;
				s = x;
				if(!(s.flag & ISREG)) {
					debugpr("#FAIL a[b] ");
					s.flag = FAIL;
					return(s);
				}
			}
			t = doit(q->right, ASADDR|USED|VALUE, 0, regmask);
			if(t.flag & FAIL) {
				totemp(q, LEFT);
				longjmp(back, 1);
			}
			sprintx(buf, "%s[%s]", str(t), str(s));
			i = INDEX;
			j = s.regmask | t.regmask;
			goto convbuf;
		}
		if(q->op == Plus && q->right->op == Icon) {
			s = doit(q->left, VALUE|USED, 0, regmask);
			if(s.flag & FAIL)
				return(s);
			if(!(s.flag & ISREG)) {
				x = allocreg(p, regmask);
				pr("#\tmov%c\t%s,%s\n", childtype(q), str(s), str(x));
				s = x;
				if(!(s.flag & ISREG)) {
					debugpr("#FAIL a(b) ");
					s.flag = FAIL;
					return(s);
				}
			}
			regmask &= ~s.regmask;
			t = doit(q->right, ASADDR|USED, 0, regmask);
			sprintx(buf, "%s(%s)", str(t), str(s));
			i = CANINDIR;
			j = s.regmask;
			goto convbuf;
		}
		if(q->op == Minus && q->right->op == Icon) {
			s = doit(q->left, VALUE|USED, 0, regmask);
			if(s.flag & FAIL)
				return(s);
			if(!(s.flag & ISREG)) {
				x = allocreg(p, regmask);
				pr("#\tmov%c\t%s,%s\n", childtype(q), str(s), str(x));
				s = x;
				if(!(s.flag & ISREG)) {
					debugpr("#FAIL %d(a) ");
					s.flag = FAIL;
					return(s);
				}
			}
			regmask &= ~s.regmask;
			sprintx(buf, "%d(%s)", -q->right->lval, str(s));
			i = CANINDIR;
			j = s.regmask;
			goto convbuf;
		}
		if(q->op == Minuseq && !(flag & (INDEX|USED))
			&& q->right->op == Icon && q->left->op == Reg
			&& incrsize(p) == (int) q->right->lval) {
			s = doit(q->left, VALUE, 0, regmask);
			sprintx(buf, "-(%s)", str(s));
			i = INDEX|AUTO;
			j = s.regmask;
			goto convbuf;
		}
		if(q->op == Plus && q->left->op == Reg
			&& !(flag & INDEX) && q->right->op == Reg
			&& incrsize(p) == 1) {
			sprintx(buf, "(r%d)[r%d]", q->right->rval,
				q->left->rval);
			i = INDEX;
			j = 0;
			goto convbuf;
		}
		s = doit(p->left, VALUE|USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		regmask &= ~s.regmask;
		if(s.flag & CANINDIR) {
			sprintx(buf, "*%s", str(s));
			i = 0;
			j = s.regmask;
			goto convbuf;
		}
		if(!(s.flag & ISREG)) {
			x = allocreg(p, regmask);
			pr("#\tmov%c\t%s,%s\n", type(q), str(s), str(x));
			regmask |= s.regmask;
			regmask &= ~x.regmask;
			s = x;
		}
		if(!(s.flag & ISREG)) {
			debugpr("#FAIL STAR ");
			s.flag = FAIL;
			return(s);
		}
		sprintx(buf, "(%s)", str(s));
		i = 0;
		j = s.regmask;
		goto convbuf;
	case Stasg:
		if(p->stsize/8 == 4) {
			i = 'l';
stasg:
			s = doit(p->left, VALUE|ASADDR|USED|FSTASG, 0, regmask);
			if(s.flag & FAIL)
				return(s);
			regmask &= ~s.regmask;
			t = doit(p->right, VALUE|ASADDR|USED|FSTASG, 0, regmask);
			if(t.flag & FAIL) {
				t = doit(p->right, VALUE|ASADDR|USED|FSTASG, 0, REGMASK);
				if(!(t.flag & FAIL) && svmask != REGMASK)
					return(t.flag = FAIL, t);
				totemp(p, LEFT);
				longjmp(back, 1);
			}
			pr("#\tmov%c\t%s,%s\n", i, str(t), str(s));
			if(!(flag & FSTASG))
				s = addrsimp(s, t);
			return(s);
		}
		else if(p->stsize/8 == 8) {
			i = 'q';
			goto stasg;
		}
		if(regmask != REGMASK) {
			debugpr("#FAIL stasg ");
			s.flag = FAIL;
			return(s);
		}
		s = doit(p->right, VALUE|USED|FSTASG, 0, regmask);
		if(s.flag & INDEX) {
		/* over-enthusiastic use of index mode */
			x = allocreg(p, regmask);
			pr("#\tmovl\t%s,%s\n", str(s), str(x));
			s = x;
		}
		regmask &= ~s.regmask;
		t = doit(p->left, VALUE|USED|FSTASG, 0, regmask);
		if(t.flag & FAIL) {
			totemp(p, RIGHT);
			longjmp(back, 1);
		}
		if(t.flag & INDEX) {
		/* over-enthusiastic use of index mode */
			x = allocreg(p, regmask);
			pr("#\tmovl\t%s,%s\n", str(t), str(x));
			t = x;
		}
		t = indirit(t);
		if(t.flag & FAIL) {
			if(p->left->op != Star)
				cerror("codegen failure in struct asg");
			totemp(p->left, LEFT);
			longjmp(back, 1);
		}
		s = indirit(s);
		if(s.flag & FAIL) {
			if(p->right->op != Star)
				cerror("codgen fail in stasg");
			totemp(p->right, LEFT);
			longjmp(back, 1);
		}
		pr("#\tmovc3\t$%d,%s,%s\n", p->stsize/8, str(s), str(t));
		if(flag & USED) {
			t = specialreg(p, REGMASK);
			pr("#\tsubl3\t$%d,r3,r0\n", p->stsize/8);
		}
		else
			t.flag = 0;
		return(t);
	case Stcall:
		goto call;
	case Auto:
		sprintx(buf, "%d(%s)", p->lval, frameptr);
		j = 0;
		i = CANINDIR;
		goto convbuf;
	case Param:
		sprintx(buf, "%d(%s)", p->lval, argptr);
		j = 0;
		i = CANINDIR;
		goto convbuf;
	case Addr:
		s = doit(p->left, USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		regmask &= ~s.regmask;
		if(dest.ans == 0) {
			dest = allocreg(p, regmask);
		}
		if(s.flag & ISREG) {
			x = alloctmp(p);
			pr("#\tmov%c\t%s,%s\n", childtype(p), str(s), str(x));
			s = x;
		}
		if(flag & TOSTACK)
			pr("#\tpushal\t%s\n", str(s));
		else
			pr("#\tmova%c\t%s,%s\n", type(p), str(s), str(dest));
		if(flag & ASADDR) {
			if(dest.flag & ISREG) {
				strcat(str(dest), ")");
				strshift(str(dest), 1);
				str(dest)[0] = '(';
			}
			else {
				strshift(str(dest), 1);
				str(dest)[0] = '*';
			}
		}
		return(dest);
	case Ucall:
		i = 0;
		goto called;
	case Uminus:
		s = doit(p->left, VALUE|USED, 0, regmask);
		if(s.flag & FAIL)
			return(s);
		if(dest.ans == 0)
			if(s.flag & SCRATCH)
				dest = s;
			else
				dest = allocreg(p, regmask & ~s.regmask);
		pr("#\tmneg%c\t%s,%s\n", childtype(p), str(s), str(dest));
		dest.flag |= CC;
		return(dest);
	case Ustcall:
		i = 0;
		goto called;
	case Asg:
assign:
		if(p->left->op == Qnode && dest.ans) {
			s = doit(p->right, VALUE|(flag&USED? USED: 0),
				dest, regmask);
			return(s);
		}
		if(p->left->op == Rnode || p->left->op == Snode) {
			s = doit(p->right, VALUE|(flag&USED? USED: 0),
				dest, regmask);
			if(s.flag & FAIL)
				return(s);
			x = s;
			dest = doit(p->left, USED, 0, 0);
			goto movexdest;
		}
		if(p->left->op == Fld) {
			s = doit(p->left->left, INDEX, 0, regmask);
			regmask &= ~s.regmask;
			if(p->right->op == Asg) {
				x = allocreg(p->right, regmask);
				t = doit(p->right, VALUE|USED, x, regmask & ~x.regmask);
			}
			else
				t = doit(p->right, VALUE|USED, 0, regmask);
			if(t.flag & FAIL) {	/* was fldstar */
				totemp(p->left->left, LEFT);
				totemp(p, RIGHT);
				longjmp(back, 1);
			}
			i = '$';
			pr("#\tinsv\t%s,%c%d,%c%d,%s\n", str(t), i,
				p->left->rval/64, i,
				p->left->rval % 64, str(s));
			if(dest.ans) {
				pr("#\tmovl\t%s,%s\n", str(t), str(dest));
				t.flag |= CC;
			}
			return(t);
		}
		if(dest.ans && p->left->op == Star) {
			if(dest.flag & SCRATCH)
				x = dest;
			else
				x = allocreg(p, regmask);
			s = doit(p->right, VALUE|USED, x, regmask);
			if(s.flag & FAIL)
				return(s);
			regmask &= ~s.regmask;
			t = doit(p->left, USED, 0, regmask);
			if(t.flag & FAIL) {
			/* did we fail because of a dest, or was it us? */
				if(dest.ans || svmask != REGMASK) {
					t = doit(p->left, USED, 0, svmask);
					if(t.flag & FAIL) {
						return(t);
					}
					starasg(p);
				}
				else
					asgwrite(p);
				longjmp(back, 1);
			}
			pr("#\tmov%c\t%s,%s\n", type(p), str(x), str(t));
			if(strcmp(str(x), str(dest)) == 0) {
				dest.flag |= CC;
				return(dest);
			}
			goto movexdest;
		}
		if(p->right->op == Fld && incrsize(p) != 4) {
			/* ext sets whole word */
			asgwrite(p);
			p->left->type = isunsigned(p)? Tuint: Tint;
			longjmp(back, 1);
		}
		t = doit(p->left, USED, 0, regmask);
		regmask &= ~t.regmask;
		if(p->right->op == Genlab || p->right->op == Comop)
			extracheck(p);	/* slightly worse code in usual case */
		s = doit(p->right, VALUE, t, regmask);	/* VALUE is't right:
			register char *a, *b; return((*a++ = *b++ | 40) != 3); */
		if(s.flag & NOGOOD) {
		/* did we fail because of a dest, or was it us? */
			if(s.flag & FAIL) {
				if(dest.ans || svmask != REGMASK) {
					s = doit(p->right, VALUE, t, svmask);
					if(s.flag & FAIL) {
						return(s);
					}
				}
				asgwrite(p);
				longjmp(back, 1);
			}
			else if((flag & USED) && !((flag & CC) && (s.flag & CC))) {
				/* presumably USED as in *p++=*q++ */
				totemp(p, RIGHT);
				longjmp(back, 1);
			}
		}
		if(dest.ans) {
			x = s;
			goto movexdest;
		}
		else if(flag & ASADDR) {
			i = (s.flag & (ISREG|CANINDIR|SCRATCH));
			j = s.regmask;
			buf = str(s);
			goto convbuf;
		}
		return(s);
	}
}
