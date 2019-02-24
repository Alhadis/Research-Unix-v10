/* gencode.c */
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[20];
struct _iobuf	*fopen();
struct _iobuf	*fdopen();
struct _iobuf	*freopen();
long	ftell();
char	*fgets();
	char		*hash();
	char		*savestr();
	char		*tstr();
	extern int	tstrused;
	extern char	*tstrbuf[];
	extern char	**curtstr;
extern int nerrors;  
typedef union ndu NODE;
typedef unsigned int TWORD;
typedef long CONSZ;  
extern int dope[];  
extern char *opst[];  
union ndu {
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		char * name;
		char pad[8  -sizeof(char *)];	
		NODE *left;
		NODE *right;
	}in;	
	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		char * name;
		char pad[8  -sizeof(char *)];	
		CONSZ lval;
		int rval;
	}tn;	
	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		int label;  
		int lop;  
	}bn;	
	struct {
		int op;
		int goal;
		TWORD type;
		int cst[(6+4)];
		int stsize;  
		short stalign;  
		short argsize;  
	}stn;	
	struct {
		int op;
		int goal;
		TWORD type;
		int cdim;
		int csiz;
	}fn;	
	
	struct {
		
		int op;
		int goal;
		TWORD type;
		int cdim;
		int csiz;
		double dval;
	}fpn;	
};
extern int busy[];
extern NODE node[];
typedef struct shape SHAPE;
	
extern struct shape {
	int	op;	
	SHAPE	*sl;	
	SHAPE	*sr;
	int	sh;	
	int	sc;	
} shapes[];
extern SHAPE	*pshape[];
typedef struct optab OPTAB;
struct optab {
	int	op;	
	int	tyop;	
	OPTAB	*nextop;
	SHAPE	**lshape;	
	int	ltype;		
	SHAPE	**rshape;	
	int	rtype;
	int	needs;
	int	rewrite;
	char	*cstring;
	int	cost;
	int	lcount;		
	int	rcount;		
	int	stinline;	
};
extern OPTAB
	*match(),
	*ophead[],
	table[];
extern NODE resc[];
extern int tmpoff;
extern int maxboff;
extern int maxtemp;
extern int maxarg;
extern int ftnno;
extern int sideff;
extern NODE
	*talloc(),
	*ind2type(),
	*tcopy(),
	*getadr(),
	*getlr();
extern CONSZ rdin();
extern char *rnames[];
extern int lineno;
extern char ftitle[];
extern int fldshf, fldsz;
extern int lflag, udebug, e2debug, odebug, rdebug, radebug, sdebug;
extern int fast;  
typedef struct inst INST;
struct inst {
	NODE	*p;
	OPTAB	*q;
	int	goal;
};
extern INST inst[300];
extern nins;
typedef SHAPE	*SHTABLE[2][20];
extern SHTABLE	sha;
typedef struct {
	unsigned char ans;
	unsigned char regmask;
	unsigned short flag;
} ret;
ret doit(), allocreg(), reg0(), indir(), tostack();
char *genjmp();
char bufs[128][64];
char *buf;
char prbuf[10240];
char *prptr;
NODE *copytree(), *gimmenode();
extern int ntree;
typedef int jmp_buf[10];
jmp_buf back;
int acnt;
gencode(p)
NODE *p;
{	NODE *q;
	if(setjmp(back)) {
		pr("#\treg\t%d\n", ++acnt);
		if(acnt > 20) {
			prtree(q);
			outpr();
			return;
		}
	}
	else
		q = copytree(p);
	buf = bufs[1];
	prptr = prbuf;
	(void) doit(q, 0, 0, 0x3f);
	acnt = ntree = 0;
	outpr();
}
ret
doit(p, flag, dest, regmask)
NODE *p;
ret dest;
{	ret s, t, x;
	char *pp;
	NODE snode, *q;
	int i, j;
	switch(p->in.op) {
	default:
		pr("#\tweird??? %d\n", p->in.op);
		return(dest);
	case 1+ 14:
		flag |= 256;
		flag &= ~1;
	case 14:
		
		t = doit(p->in.left, 2, 0, regmask);
		regmask &= ~t.regmask;
		s = doit(p->in.right, 2, 0, regmask);
		if(s.flag & 1024) {
			totemp(p, 0);
			longjmp(back, 1);
		}
		regmask &= ~s.regmask;
		if(flag & 1) {
			pr("#\tbit%c\t%s,%s\n", childtype(p), 	bufs[s.ans], 	bufs[t.ans]);
			dest.ans = 0;
			dest.flag = 1;
			dest.regmask = 0;
			return(dest);
		}
		if(flag & 256)
			dest = x = t;
		if(dest.ans == 0)
			if(s.flag & 16)
				dest = x = s;
			else if(t.flag & 16)
				dest = x = t;
			else
				dest = x = allocreg(p, regmask);
		else
			x = allocreg(p, regmask);
		if(p->in.right->in.op == 4) {
			x = tostack();	
			sprintf(	bufs[x.ans], "$%d", -p->in.right->tn.lval - 1);
		}
		else
			pr("#\tmcom%c\t%s,%s\n", childtype(p), 	bufs[s.ans], 	bufs[x.ans]);
		if(strcmp(	bufs[t.ans], 	bufs[dest.ans]) == 0)
			pr("#\tbic%c2\t%s,%s\n", childtype(p), 	bufs[x.ans], 	bufs[dest.ans]);
		else
			pr("#\tbic%c3\t%s,%s,%s\n", childtype(p), 	bufs[x.ans],
					bufs[t.ans], 	bufs[dest.ans]);
		dest.flag |= 1;
		return(dest);
	case 70:
call:
		funargs(p->in.right, regmask);
		i = p->stn.argsize/32;
called:
		s = doit(p->in.left, 2|8, 0, regmask & ~3);
		pp = 	bufs[s.ans];
aftercall:
		if(regmask != 0x3f) {
			s.flag = 1024;
			return(s);
		}
		x = reg0(p, regmask);
		pr("#\tcalls\t$%d,%s\n", i, pp);
		if(p->in.op == 100 || p->in.op == 2+ 100 || (flag & 8)) {
			strcat(	bufs[x.ans], ")");
			strshift(	bufs[x.ans], 1);
				bufs[x.ans][0] = '(';
		}
		if(dest.ans == 0)
			if(flag & 256)
				dest = doit(p->in.left->in.op == 104?
					p->in.left->in.left:
					p->in.left, 0, 0, regmask & ~x.regmask);
			else
				return(x);
movexdest:	
		if(strcmp(	bufs[x.ans], 	bufs[dest.ans]) == 0)
			return(x);
		if(flag & 4)
			pr("#\tpush%c\t%s\n", type(p), 	bufs[x.ans]);
		else
			pr("#\tmov%c\t%s,%s\n", type(p), 	bufs[x.ans], 	bufs[dest.ans]);
		dest.flag |= 1;
		return(dest);
	case 111:
		s = doit(p->in.left, 2, 0, regmask);
		regmask &= ~s.regmask;
		t = doit(p->in.right, 2, 0, regmask);
		if(t.flag & 1024) {
			totemp(p, 0);
			longjmp(back, 1);
		}
		pr("#\tcmp%c\t%s,%s\n", childtype(p), 	bufs[s.ans], 	bufs[t.ans]);
		dest.ans = dest.regmask = 0;
		dest.flag = 1;
		return(dest);
	case 59:	
		if(p->in.left->in.op == 108) {
			if(dest.ans == 0) {
				dest = allocreg(p, regmask);
			}
			(void) doit(p->in.left, 2, dest, regmask);
		}
		else
			(void) doit(p->in.left, 0, 0, regmask);
		s = doit(p->in.right, 2, dest, regmask);
		if(flag & 8) {
			dest.ans = 0;
			i = (s.flag & (128|512|16));
			j = s.regmask;
			buf = 	bufs[s.ans];
			goto convbuf;
		}
		return(s);
	case 77:
		s = doit(p->in.left, 2, 0, regmask);
		if(dest.ans == 0)
			if(s.flag & 16)
				dest = s;
			else
				dest = allocreg(p, regmask & ~s.regmask);
		pr("#\tmcom%c\t%s,%s\n", childtype(p), 	bufs[s.ans], 	bufs[dest.ans]);
		dest.flag |= 1;
		return(dest);
	case 104:
		if(p->in.left->in.op == 58 && p->in.left->in.left->in.op == (2+ 11)) {
			if(dest.ans == 0) {
				dest = allocreg(p, regmask);
				s = doit(p->in.left, 2, dest, regmask);
			}
			else {
				x = allocreg(p, regmask);
				s = doit(p->in.left, 2, x, regmask & ~x.regmask);
			}
		}
		else
			s = doit(p->in.left, 2, 0, regmask);
		regmask &= ~s.regmask;
		if(childtype(p) == type(p))
			return(s);
		if(dest.ans == 0)
			if(s.flag & 16)
				dest = s;
			else
				dest = allocreg(p, regmask);
		if(isunsigned(p->in.left) && incrsize(p) > incrsize(p->in.left)
			&& type(p) != 'f' && type(p) != 'd')
			pp = "movz";	
		else
			pp = "cvt";
		pr("#\t%s%c%c\t%s,%s\n", pp, childtype(p), type(p), 	bufs[s.ans], 	bufs[dest.ans]);
		dest.flag |= 1;
		return(dest);
	case 79:
		i = -1;
		pp = "sub";
incrop:
		if(p->in.right->in.op != 4)
			fflush((&_iob[1])), cerror("codegen! incrop not with ICON");
		s = doit(p->in.left, 0, 0, regmask);
		t = doit(p->in.right, 0, 0, 0);
		if(flag & 2) {
			if(dest.ans == 0)
				dest = allocreg(p, regmask & ~s.regmask);
			pr("#\tmov%c\t%s,%s\n", childtype(p), 	bufs[s.ans], 	bufs[dest.ans]);
			if(t.flag & 64) {
				if(i == 1)
					pp = "inc";
				else
					pp = "dec";
				pr("#\t%s%c\t%s\n", pp, childtype(p), 	bufs[s.ans]);
			}
			else
				pr("#\t%s%c2\t%s,%s\n", pp, childtype(p),
						bufs[t.ans], 	bufs[s.ans]);
			if(flag & 8) {
				s = dest;
				goto inreg;
			}
			dest.flag &= ~1;
			return(dest);
		}
		if(t.flag & 64) {
			if(i == 1)
				pp = "inc";
			else
				pp = "dec";
			pr("#\t%s%c\t%s\n", pp, childtype(p), 	bufs[s.ans]);
		}
		else
			pr("#\t%s%c2\t%s,%s\n", pp, childtype(p), 	bufs[t.ans], 	bufs[s.ans]);
		if(dest.ans) {
			x = s;
			goto movexdest;
		}
		s.flag |= 1;
		return(s);
	case 1+ 60:
		flag |= 256;
	case 60:
		
		if(!isunsigned(p->in.left)) {
			if(p->in.left->in.op == 104 && type(p) == childtype(p->in.left)) {
				p->in.left = p->in.left->in.left;
			}
			if(p->in.right->in.op == 104 && type(p) == childtype(p->in.right)) {
				p->in.right = p->in.right->in.left;
			}
			pp = "div";
			goto binop;
		}
		pp = "udiv";
unsdiv:
		if(incrsize(p->in.right) != 4) {
			snode.in.op = 104;
			snode.in.left = p->in.right;
			snode.in.type = 	01000;
			s = doit(&snode, 2|4, tostack(), regmask);
		}
		else
			s = doit(p->in.right, 4, tostack(), regmask);
		regmask &= ~s.regmask;
		if(incrsize(p->in.left) != 4) {		
			snode.in.op = 104;
			snode.in.left = p->in.left;
			snode.in.type = 	01000;
			t = doit(&snode, 2|4, tostack(), regmask);
		}
		else
			t = doit(p->in.left, 4, tostack(), regmask);
		if(t.flag & 1024) {
			totemp(p, 1);
			longjmp(back, 1);
		}
		i = 2;
		goto aftercall;
	case 1+ 19:
		flag |= 256;
	case 19:
		pp = "xor";
binop:
		t = doit(p->in.left, 2, 0, regmask);
		regmask &= ~t.regmask;
		s = doit(p->in.right, 2, 0, regmask);
		if(s.flag & 1024) {
			totemp(p, 0);
			longjmp(back, 1);
		}
		regmask &= ~s.regmask;
		if(type(p) != childtype(p)) {
			x = allocreg(p, regmask);
			pr("#\t%s%c3\t%s,%s,%s\n", pp, childtype(p), 	bufs[s.ans],
					bufs[t.ans], 	bufs[x.ans]);
			if(dest.ans == 0)
				dest = x;
			pr("#\tcvt%c%c\t%s,%s\n", childtype(p), type(p),
					bufs[x.ans], 	bufs[dest.ans]);
			dest.flag |= 1;
			return(dest);
		}	
		if(dest.ans == 0)	
			if((t.flag & 16) || (flag & 256)) {
twoop:
				dest = t;
				if(*pp == 'a' && (s.flag & 64))
					pr("#\tinc%c\t%s\n", childtype(p), 	bufs[t.ans]);
				else if(*pp == 's' && (s.flag & 64))
					pr("#\tdec%c\t%s\n", childtype(p), 	bufs[t.ans]);
				else
					pr("#\t%s%c2\t%s,%s\n", pp, childtype(p),
							bufs[s.ans], 	bufs[t.ans]);
				if(flag & 8)
					goto binopaddr;
				dest.flag |= 1;
				return(dest);
			}
			else if(s.flag & 16)
				dest = s;
			else
				dest = allocreg(p, regmask);
		if((*pp == 'a' || *pp == 'm' || *pp == 'x' || *pp == 'b')
			&& strcmp(	bufs[s.ans], 	bufs[dest.ans]) == 0) {
			dest = s;
			s = t;
			t = dest;
			goto twoop;
		}
		pr("#\t%s%c3\t%s,%s,%s\n", pp, childtype(p), 	bufs[s.ans], 	bufs[t.ans], 	bufs[dest.ans]);
binopaddr:
		if(flag & 8) {
			if(dest.flag & 128) {
				strcat(	bufs[dest.ans], ")");
				strshift(	bufs[dest.ans], 1);
					bufs[dest.ans][0] = '(';
				return(dest);
			}
			if(dest.flag & 512) {
				strshift(	bufs[dest.ans], 1);
					bufs[dest.ans][0] = '*';
				dest.flag &= ~512;
				return(dest);
			}
			dest.flag = 1024;	
			return(dest);
		}
		dest.flag |= 1;
		return(dest);	
	case 103:
		
		s = doit(p->in.left, 0, 0, regmask);
		if(dest.ans == 0)
			dest = allocreg(p, regmask & ~s.regmask);
		pr("#\text%sv\t$%d,$%d,%s,%s\n", isunsigned(p->in.left)? "z": "",
			p->tn.rval/64, p->tn.rval%64, 	bufs[s.ans], 	bufs[dest.ans]);
		dest.flag |= 1;
		return(dest);
	case 110:
		s = doit(p->in.left, 1|2, 0, regmask);
		pp = genjmp(p->bn.lop);
		if(s.flag & 1)
			pr("#\t%s\tL%d\n", pp, p->bn.label);
		else
			pr("#\ttst%c\t%s\n#\t%s\tL%d\n", childtype(p), 	bufs[s.ans],
				pp, p->bn.label);
		s.flag |= 1;
		return(s);
	case 108:
		s = doit(p->in.left, flag, dest, regmask);
		pr("#L%d:\n", p->bn.label);
		return(s);
	case 112:
		(void) doit(p->in.left, 0, dest, regmask);
		pr("#\tjbr\tL%d\n", p->bn.label);
		s.ans = s.regmask = s.flag = 0;
		return(s);
	case 4:
		if(p->tn.name)
			if(p->tn.lval)
				sprintf(buf, "%s+%d", p->tn.name, p->tn.lval);
			else
				sprintf(buf, "%s", p->tn.name);
		else
			sprintf(buf, "%d", p->tn.lval);
		if(!(flag & 8)) {
			strshift(buf, 1);;
			buf[0] = '$';
		}
		i = 0;
		if(p->tn.name == 0)
			if(p->tn.lval == 0)
				i = 32;
			else if(p->tn.lval == 1)
				i = 64;
		j = 0;
convbuf:	
		if(dest.ans == 0) {
			if(p->in.op == 4 || !(flag & 8)) {
				 s.ans = (buf - bufs[0])/64; buf += 64; s.flag =  i; s.regmask =  j; return(s);
			}
			if(i & 512) {
				strshift(buf, 1);
				buf[0] = '*';
				 s.ans = (buf - bufs[0])/64; buf += 64; s.flag =  i & ~512; s.regmask =  j; return(s);
			}
			if(i & 128) {
				strcat(buf, ")");
				strshift(buf, 1);
				buf[0] = '(';
				 s.ans = (buf - bufs[0])/64; buf += 64; s.flag =  i; s.regmask =  j; return(s);
			}
			pp = buf;
			buf += 64;
			s = allocreg(p, regmask);
			pr("#\tmov%c\t%s,%s\n", type(p), pp, 	bufs[s.ans]);
			flag &= ~8;
			goto inreg;
		}
		if(flag & 4)
			if(p->in.type != 040)
				pr("#\tpushl\t%s\n", buf);
			else
				pr("#\tmovd\t%s,%s\n", buf, 	bufs[dest.ans]);
		else if(i & 32)
			pr("#\tclr%c\t%s\n", type(p), 	bufs[dest.ans]);
		else	
			pr("#\tmov%c\t%s,%s\n", type(p), buf, 	bufs[dest.ans]);
		dest.flag |= 1;
		return(dest);
	case 78:
		i = 1;
		pp = "add";
		goto incrop;
	case 113:	
		s = doit(p->in.left, 8, 0, 0);
		pr("#\t.long\t%s\n", 	bufs[s.ans]);
		return(dest);
	case 1+ 64:
		flag |= 256;
	case 64:
		s = doit(p->in.left, 2, 0, regmask);
		regmask &= ~s.flag;
		t = doit(p->in.right, 2, 0, regmask);
		if(t.flag & 1024) {
			totemp(p, 0);
			longjmp(back, 1);
		}
		regmask &= ~t.regmask;
		if(dest.ans == 0)
			if((s.flag & 16) || (flag & 256))
				dest = s;
			else
				dest = allocreg(p, regmask);
		pr("#\tashl\t%s,%s,%s\n", 	bufs[t.ans], 	bufs[s.ans], 	bufs[dest.ans]);
		dest.flag |= 1;
		return(dest);
	case 1+ 8:
		if(p->in.right->in.op == 4) {
			i = -1;
			pp = "sub";
			flag &= ~2;
			goto incrop;
		}
		flag |= 256;
	case 8:
		pp = "sub";
		goto binop;
	case 1+ 62:
		flag |= 256;
		if(p->in.left->in.op == 104 && !isunsigned(p)) {
			snode = *p;
			snode.in.left = p->in.left->in.left;
			p = &snode;
		}
	case 62:
		if(isunsigned(p)) {
			pp = "urem";
			goto unsdiv;
		}
		s = doit(p->in.left, 2, 0, regmask);
		regmask &= ~s.regmask;
		t = doit(p->in.right, 2, 0, regmask);
		regmask &= ~t.regmask;
		x = allocreg(p, regmask);
		pr("#\tdiv%c3\t%s,%s,%s\n", childtype(p), 	bufs[t.ans], 	bufs[s.ans], 	bufs[x.ans]);
		pr("#\tmul%c2\t%s,%s\n", childtype(p), 	bufs[t.ans], 	bufs[x.ans]);
		pr("#\tsub%c3\t%s,%s,%s\n", childtype(p), 	bufs[x.ans], 	bufs[s.ans], 	bufs[x.ans]);
		if(dest.ans)
			goto movexdest;
		if(flag & 256) {
			dest = s;
			goto movexdest;
		}
		x.flag |= 1;
		return(x);
	case 1+ 11:
		flag |= 256;
	case 11:
		pp = "mul";
		goto binop;
	case 2:	
		if(p->tn.lval)
			sprintf(buf, "%s+%d", p->tn.name, p->tn.lval);
		else if(p->tn.name)
			sprintf(buf, "%s", p->tn.name);
		else
			sprintf(buf, "0");
		j = 0;
		i = 512;
		goto convbuf;
	case 1+ 17:
		flag |= 256;
	case 17:
		pp = "bis";
		goto binop;
	case 1+ 6:
		if(p->in.right->in.op == 4) {
			i = 1;
			pp = "add";
			flag &= ~2;
			goto incrop;
		}
		flag |= 256;
	case 6:
		pp = "add";
		goto binop;
	case 94:
		sprintf(buf, "r%d", p->tn.rval);
		j = 0;
		i = 128;
		goto convbuf;
	case 118: case 119:
		x = reg0(p, 3);
		x.regmask = 0;
		if(dest.ans)
			goto movexdest;
		return(x);
	case 120:
		return(reg0(p, regmask));
	case 1+ 66:
		flag |= 256;
	case 66:
		s = doit(p->in.left, 2, 0, regmask);
		regmask &= ~s.regmask;
		if(dest.ans == 0)
			if((flag & 256) || (s.flag & 16))
				dest = s;
			else
				dest = allocreg(p, regmask);
		dest.flag |= 1;
		regmask &= ~dest.regmask;
		if(p->in.right->in.op == 4 && (i = p->in.right->tn.lval) >= 0) {
			pr("#\textv\t$%d,$%d,%s,%s\n", i, 32 - i, 	bufs[s.ans], 	bufs[dest.ans]);
			return(dest);
		}
		t = doit(p->in.right, 2, 0, regmask);
		regmask &= ~t.regmask;
		x = allocreg(p, regmask);
		pr("#\tmnegl\t%s,%s\n", 	bufs[t.ans], 	bufs[x.ans]);
		pr("#\tashl\t%s,%s,%s\n", 	bufs[x.ans], 	bufs[s.ans], 	bufs[dest.ans]);
		return(dest);
	case (2+ 11):
		switch(p->in.left->in.op) {
		case 116:
		case 117:
		case 2:
			s = doit(p->in.left, 2, 0, regmask);
			sprintf(buf, "*%s", 	bufs[s.ans]);
			j = i = 0;
			goto convbuf;
		case 94:
			s = doit(p->in.left, 2, 0, regmask);
inreg:
			if(s.flag & 128) {
				sprintf(buf, "(%s)", 	bufs[s.ans]);
				j = 0;
				i = 512;
				goto convbuf;
			}
			else if(s.flag & 512) {
				sprintf(buf, "*%s", 	bufs[s.ans]);
				j = 0;
				i = 0;
				goto convbuf;
			}
			else {
				s.flag = 1024;
				return(s);
			}
		}
		q = p->in.left;
		if(q->in.op == 78
			&& q->in.right->in.op == 4 && q->in.left->in.op == 94
			&& incrsize(p) == (int) q->in.right->tn.lval) {
			s = doit(q->in.left, 2, 0, regmask);
			sprintf(buf, "(%s)+", 	bufs[s.ans]);
			i = 0;
			j = s.regmask;
			goto convbuf;
		}
		if(q->in.op == 6 && q->in.left->in.op == 64
			&& q->in.left->in.right->in.op == 4
			&& shiftsize(p) == (int) q->in.left->in.right->tn.lval) {
			s = doit(q->in.left->in.left, 2, 0, regmask);
			regmask &= ~s.regmask;
			if(!(s.flag & 128)) {
				x = allocreg(p, regmask);
				pr("#\tmov%c\t%s,%s\n", childtype(q), 	bufs[s.ans], 	bufs[x.ans]);
				regmask |= s.regmask;
				regmask &= ~x.regmask;
				s = x;
			}
			t = doit(q->in.right, 8, 0, regmask);
			sprintf(buf, "%s[%s]", 	bufs[t.ans], 	bufs[s.ans]);
			i = 0;
			j = s.regmask;
			goto convbuf;
		}
		if(q->in.op == 6
			&& (q->in.right->in.op == 4 || q->in.right->in.op == 2)) {
			s = doit(q->in.left, 2, 0, regmask);
			regmask &= ~s.regmask;
			if(!(s.flag & 128)) {
				x = allocreg(p, regmask);
				pr("#\tmov%c\t%s,%s\n", childtype(q), 	bufs[s.ans], 	bufs[x.ans]);
				regmask |= s.regmask;
				regmask &= ~x.regmask;
				s = x;
			}
			t = doit(q->in.right, 8, 0, regmask);
			sprintf(buf, "%s(%s)", 	bufs[t.ans], 	bufs[s.ans]);
			i = 512;
			j = s.regmask;
			goto convbuf;
		}
		if(q->in.op == 8 && q->in.right->in.op == 4) {
			s = doit(q->in.left, 2, 0, regmask);
			regmask &= ~s.regmask;
			if(!(s.flag & 128)) {
				x = allocreg(p, regmask);
				pr("#\tmov%c\t%s,%s\n", childtype(q), 	bufs[s.ans], 	bufs[x.ans]);
				regmask |= s.regmask;
				regmask &= ~x.regmask;
				s = x;
				if(!(s.flag & 128)) {
					s.flag = 1024;
					return(s);
				}
			}
			sprintf(buf, "%d(%s)", -q->in.right->tn.lval, 	bufs[s.ans]);
			i = 512;
			j = s.regmask;
			goto convbuf;
		}
		if(q->in.op == 1+ 8
			&& q->in.right->in.op == 4 && q->in.left->in.op == 94
			&& incrsize(p) == (int) q->in.right->tn.lval) {
			s = doit(q->in.left, 2, 0, regmask);
			sprintf(buf, "-(%s)", 	bufs[s.ans]);
			i = 0;
			j = s.regmask;
			goto convbuf;
		}
		if(q->in.op == 6 && q->in.left->in.op == 94
			&& q->in.right->in.op == 94) {
			sprintf(buf, "(r%d)[r%d]", q->in.right->tn.rval,
				q->in.left->tn.rval);
			i = 0;
			j = 0;
			goto convbuf;
		}
		s = doit(p->in.left, 2, 0, regmask);
		if(s.flag & 1024)
			return(s);
		regmask &= ~s.regmask;
		if(s.flag & 512) {
			sprintf(buf, "*%s", 	bufs[s.ans]);
			i = 0;
			j = s.regmask;
			goto convbuf;
		}
		if(!(s.flag & 128)) {
			x = allocreg(p, regmask);
			pr("#\tmov%c\t%s,%s\n", childtype(q), 	bufs[s.ans], 	bufs[x.ans]);
			regmask |= s.regmask;
			regmask &= ~x.regmask;
			s = x;
		}
		if(!(s.flag & 128)) {
			s.flag = 1024;
			return(s);
		}
		sprintf(buf, "(%s)", 	bufs[s.ans]);
		i = 0;
		j = s.regmask;
		goto convbuf;
	case 98:
		if(p->stn.stsize/8 == 4) {
			i = 'l';
stasg:
			s = doit(p->in.right, 2|8, 0, regmask);
			regmask &= ~s.regmask;
			t = doit(p->in.left, 2|8, 0, regmask);
			pr("#\tmov%c\t%s,%s\n", i, 	bufs[s.ans], 	bufs[t.ans]);
			s.regmask = s.flag = 0;
			return(s);
		}
		else if(p->stn.stsize/8 == 8) {
			i = 'q';
			goto stasg;
		}
		if(regmask != 0x3f) {
			s.flag = 1024;
			return(s);
		}
		s = doit(p->in.right, 2|8, 0, regmask);
		regmask &= ~s.regmask;
		t = doit(p->in.left, 2|8, 0, regmask);
		pr("#\tmovc3\t$%d,%s,%s\n", p->stn.stsize/8, 	bufs[s.ans], 	bufs[t.ans]);
		s.regmask = s.flag = 0;
		return(s);
	case 100:
		goto call;
	case 116:
		sprintf(buf, "%d(fp)", p->tn.lval);
		j = 0;
		i = 512;
		goto convbuf;
	case 117:
		sprintf(buf, "%d(ap)", p->tn.lval);
		j = 0;
		i = 512;
		goto convbuf;
	case 2+ 14:
		s = doit(p->in.left, 0, 0, regmask);
		regmask &= ~s.regmask;
		if(dest.ans == 0) {
			dest = allocreg(p, regmask);
		}
		if(flag & 4)
			pr("#\tpushal\t%s\n", 	bufs[s.ans]);
		else {
			pr("#\tmova%c\t%s,%s\n", childtype(p), 	bufs[s.ans], 	bufs[dest.ans]);
		}
		if(flag & 8) {
			if(dest.flag & 128) {
				strcat(	bufs[dest.ans], ")");
				strshift(	bufs[dest.ans], 1);
					bufs[dest.ans][0] = '(';
			}
			else {
				strshift(	bufs[dest.ans], 1);
					bufs[dest.ans][0] = '*';
			}
		}
		return(dest);
	case 2+ 70:
		i = 0;
		goto called;
	case 2+ 8:
		s = doit(p->in.left, 2, 0, regmask);
		if(dest.ans == 0)
			if(s.flag & 16)
				dest = s;
			else
				dest = allocreg(p, regmask & ~s.regmask);
		pr("#\tmneg%c\t%s,%s\n", childtype(p), 	bufs[s.ans], 	bufs[dest.ans]);
		dest.flag |= 1;
		return(dest);
	case 2+ 100:
		i = 0;
		goto called;
	case 58:
		if(p->in.left->in.op == 120 && dest.ans) {
			s = doit(p->in.right, 2, dest, regmask);
			return(s);
		}
		if(p->in.left->in.op == 103) {
			s = doit(p->in.left->in.left, 0, 0, regmask);
			regmask &= ~s.regmask;
			if(p->in.right->in.op == 58) {
				x = allocreg(p->in.right, regmask);
				t = doit(p->in.right, 2, x, regmask & ~x.regmask);
			}
			else
				t = doit(p->in.right, 2, 0, regmask);
			if(t.flag & 1024) {
				totemp(p->in.left, 0);
				longjmp(back, 1);
			}
			pr("#\tinsv\t%s,$%d,$%d,%s\n", 	bufs[t.ans], p->in.left->tn.rval/64,
				p->in.left->tn.rval % 64, 	bufs[s.ans]);
			t.flag |= 1;
			return(t);
		}
		if(dest.ans && p->in.left->in.op == (2+ 11)) {
			if(dest.flag & 16)
				x = dest;
			else
				x = allocreg(p, regmask);
			s = doit(p->in.right, 2, x, regmask);
			regmask &= ~s.regmask;
assignit:
			t = doit(p->in.left, 0, 0, regmask);
			pr("#\tmov%c\t%s,%s\n", type(p), 	bufs[x.ans], 	bufs[t.ans]);
			if(strcmp(	bufs[x.ans], 	bufs[dest.ans]) == 0) {
				dest.flag |= 1;
				return(dest);
			}
			goto movexdest;
		}
		if(p->in.right->in.op == 119)
			t = doit(p->in.left, 0, 0, regmask & ~3);
		else
			t = doit(p->in.left, 0, 0, regmask);
		regmask &= ~t.regmask;
		s = doit(p->in.right, 2, t, regmask);
		if(s.flag & 1024) {
		
			if(dest.ans) {
				regmask |= t.regmask;
				s = doit(p->in.right, 2, t, regmask);
				if(s.flag & 1024)
					return(s);
			}
			asgwrite(p);
			longjmp(back, 1);
		}
		if(dest.ans) {
			x = s;
			goto movexdest;
		}
		else if(flag & 8) {
			i = (s.flag & (128|512|16));
			j = s.regmask;
			buf = 	bufs[s.ans];
			goto convbuf;
		}
		s.flag |= 1;
		return(s);
	}
}
char *
genjmp(n)
{
	switch(n) {
	default:
		return("jweird");
	case 80:
		return("jeql");
	case 81:
		return("jneq");
	case 85:
		return("jgtr");
	case 84:
		return("jgeq");
	case 83:
		return("jlss");
	case 82:
		return("jleq");
	case 89:
		return("jgtru");
	case 88:
		return("jgequ");
	case 87:
		return("jlssu");
	case 86:
		return("jlequ");
	}
}
