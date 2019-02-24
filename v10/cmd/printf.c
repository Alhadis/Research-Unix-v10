#define NEXT(p)	(*(p) ? *(p)++ : 0)

char fmtbuf[128];

double fnum; long lnum;

main(argc, argv)
char **argv;
{
	char *format = *++argv, **Argv;
	register char *fmt, *p;
	register c, i;
	++argv;
	if (argc >= 2) do {
		Argv = argv;
		fmt = format;
		while (c = NEXT(fmt)) switch (c) {
		case '\\':
			switch (c = NEXT(fmt)) {
			case 'n':
				putchar('\n'); break;
			case 'r':
				putchar('\r'); break;
			case 't':
				putchar('\t'); break;
			case 'b':
				putchar('\b'); break;
			case '0': case '1': case '2': case '3':
			case '4': case '5': case '6': case '7':
				c -= '0';
				for (i=0; i<2; i++)
					if (*fmt >= '0' && *fmt <= '7')
						c = (c << 3) + *fmt++ - '0';
			default:
				putchar(c); break;
			}
			break;
		case '%':
			p = fmtbuf;
			*p++ = c;
			while (c && (c = NEXT(fmt))) switch (c) {
			case '*':
				atonum(NEXT(argv));
				p += sprintf(p, "%d", lnum);
				break;
			case 'h': case 'l':
				break;
			case 'd': case 'o': case 'x':
			case 'D': case 'O': case 'X':
			case 'f': case 'e': case 'g':
			case 'c': case 'u': case 'U':
				*p++ = c; *p = 0;
				if (atonum(NEXT(argv)))
					printf(fmtbuf, fnum);
				else
					printf(fmtbuf, lnum);
				c = 0; break;
			case 's':
				*p++ = c; *p = 0;
				printf(fmtbuf, NEXT(argv));
				c = 0; break;
			case '%':
				*p++ = c; *p = 0;
				printf(fmtbuf);
				c = 0; break;
			default:
				*p++ = c;
			}
			break;
		default:
			putchar(c);
			break;
		}
	} while (*argv && argv > Argv);
	exit(0);
}

double
pwrten(exp)
register exp;
{
	register double d;
	if (exp < 0) return 1.0/pwrten(-exp);
	if (exp == 0) return 1.0;
	if (exp == 1) return 10.0;
	d = pwrten(exp/2), d *= d;
	if (exp & 1) d *= 10.0;
	return d;
}

lexconst(n, ftype, exp)
register n, ftype, exp;
{
	if (ftype)
		fnum = (double)n * pwrten(exp);
	else
		lnum = n;
	return ftype;
}

enum States { start, intscan, hexscan, fracscan, expstart, expscan};

atonum(p)
register char *p;
{
	register state = (int)start, c;
	unsigned int i = 0, ioct = 0;
	unsigned int sign = 1, octal = 0, fexp = 0, sexp = 1, exp = 0;
	switch (c = *p++) {
	case '+': 
		c = *p++; break;
	case '-': 
		sign = -1, c = *p++; break;
	case '0': 
		++octal, c = *p++; break;
	case '1': case '2': case '3': case '4': case '5': case '6':
	case '7': case '8': case '9': case '.': case 'e': case 'E':
		break;
	case '\'': case '"':
		return lexconst(*p++, 0, 0);
	default:
		return lexconst(c, 0, 0);
	}
	for (;; c = *p++) switch (state) {
	case start:
		if (octal && (c == 'x' || c == 'X')) {
			state = (int)hexscan;
			break;
		}
		state = (int)intscan;	/* fall through */
	case intscan:
		if (c >= '0' && c <= '9') {
			i = 10*i + (c -= '0');
			if (octal) ioct = 8*ioct + c;
		} else if (c == 'e' || c == 'E') {
			state = (int)expstart;
		} else if (c == '.') {
			state = (int)fracscan;
		} else {
			return lexconst(sign*(octal ? ioct : i), 0, 0);
		}
		break;
	case hexscan:
		if (c >= '0' && c <= '9') {
			i = 16*i + (c -= '0');
		} else if (c >= 'a' && c <= 'f') {
			i = 16*i + (c -= 'a' - 10);
		} else if (c >= 'A' && c <= 'F') {
			i = 16*i + (c -= 'A' - 10);
		} else {
			return lexconst(sign*i, 0, 0);
		}
		break;
	case fracscan:
		if (c >= '0' && c <= '9') {
			i = 10*i + (c -= '0');
			--fexp;
		} else if (c == 'e' || c == 'E') {
			state = (int)expstart;
		} else {
			return lexconst(sign*i, 1, fexp);
		}
		break;
	case expstart:
		state = (int)expscan;
		if (c == '+') {
			break;
		} else if (c == '-') {
			sexp = -1;
			break;
		}
	case expscan:
		if (c >= '0' && c <= '9') {
			exp = 10*exp + (c -= '0');
		} else {
			return lexconst(sign*i, 1, fexp + sexp*exp);
		}
		break;
	}
}
