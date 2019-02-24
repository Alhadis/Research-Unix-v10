#include <stdio.h>
#include "ctype.h"
#include "typedef.h"
#include "basic.h"
#include "tokens.h"

static char	*types[] = {
	"",	"%",	"",	"$"
	};

int	typelens[] = {
	0,	  INTSIZE,	  FLOATSIZE,	  STRINGSIZE
	};

double	popfloat();


/*
 * clrsym --- clear symbol table
 */

clrsym()
{

   strptr = strspace;
   symlast = symspace;
   clear(chains, sizeof chains);
}


/*
 * getvar --- parse a variable name and return a pointer to its
 *	      symbol table entry.  if dimflag is non-zero this is
 *	      in the context of a DIM statement and allocation is
 *	      exact; otherwise a default sized array is allocated.
 */

Symptr getvar(type, dimflag)
int	*type;
{
	register char	*p;
	register Symptr v;
	register int	len;
	Symptr		*chain;
	char		name[2];
	int		i, size, offset, _nsubs, _subsc[MAXSUBS];
	
	p = inptr;
	offset = 0;
	if (*p == FN) {
		if (dimflag)
			badsyn();
		++p;
		++offset;
		}
	if (!isalpha(*p))
		err("name required");
	name[0] = *p++;
	name[1] = 0;
	while (isalnum(*p)) {
		if (!name[1])
			name[1] = *p;
		++p;
		}
	if (*p == '%') {
		++p;
		*type = INT;
		}
	else if (*p == '$') {
		++p;
		*type = STRING;
		}
	else
		*type = FLOAT;
	
	_nsubs = 0;	/* assume no subscripts for now */
	inptr = p;
	if (*p == LPAR) {
		if (++offset < 2) {	/* not a function */
			++inptr;
			for (;;) {
				_subsc[_nsubs++] = fexpr();
				if (*inptr == RPAR) {
					++inptr;
					break;
					}
				expectc(COMMA);
				if (_nsubs >= MAXSUBS)
					err("more than %d subscripts",
							MAXSUBS);
				}
			}
		}
	else
		if (dimflag)
			err("expected (");
	/*
	 * scan through the appropriate chain looking for the symbol.
	 */
	chain = &chains[offset][*type];
	for (v = *chain; v; v = v->v_next)
		if (v->v_name[0] == name[0] && v->v_name[1] == name[1]) {
			if (dimflag)
				err("attempt to re-dimension");
			goto done;
			}
	/*
	 * symbol was not found ... enter it.
	 */
	v = (Symptr)symlast;
	len = SYMHDRLEN;
	/*
	 * if subscripted variable then calculate the array size
	 * if dimensioned and in a DIM stmt then use the dimensions
	 * provided; otherwise use the default values.
	 */
	switch (offset) {
	case 0:		/* a scalar variable */
		len += typelens[*type];
		break;
	case 1:		/* a vector variable */
		for (i = 0, size = 1; i < _nsubs; ++i) {
			size *= (dimflag? _subsc[i] : DEFSIZE);
			if (size <= 0)
				err("zero or negative array size");
			}
		len += VECHDRSIZE + size * typelens[*type];
		break;
	case 2:		/* a function variable */
		len += sizeof(Func);
		break;
		}
	if (symlast + len >= symend)
		if (!moresym(symlast + len))
			err("symbol table overflow");
	symlast += len;
	/*
	 * clear and copy information into symbol table entry.
	 */
	clear((char *)v, len);
	v->v_name[0] = name[0];
	v->v_name[1] = name[1];
	if (_nsubs) {
		for (i = 0; i < _nsubs; ++i)
			v->v_un.v_vec.v_subsc[i] = (dimflag? _subsc[i] : DEFSIZE);
		v->v_nsubs = _nsubs;
		}
	v->v_next = *chain;
	*chain = v;
done:
	/*
	 * make global copy of subscript information
	 */
	if ((nsubs = _nsubs))
		for (i = 0; i < _nsubs; ++i)
			subsc[i] = _subsc[i];
	return(v);
}


/*
 * dim --- interpret a DIM statement
 */

dim()
{
	int	type;

	do {
		getvar(&type, YES);
		} while (*inptr == COMMA && ++inptr);
}


/*
 * dumpsym --- dump the symbol table
 */

dumpsym()
{
	register int	i;

	for (i = 1; i < MAXTYPES; ++i) {
		dumpchain(0, i);
		dumpchain(1, i);
		dumpchain(2, i);
		}
}


/*
 * dumpchain --- dump one chain of the symbol table
 */

dumpchain(offset, type)
{
	register Symptr s;
	register int	i, n;

	for (s = chains[offset][type]; s; s = s->v_next) {
		fprintf(stderr, "%8X %.2s%s", s, s->v_name, types[type]);
		switch (offset) {
		case 0:		/* this is a chain of scalars */
			switch (type) {
			case INT:
				fprintf(stderr, "\t= %d", s->v_un.v_int);
				break;
			case FLOAT:
				fprintf(stderr, "\t= %g",
					(SINGLE ? s->v_un.v_float
						: s->v_un.v_double));
				break;
			case STRING:
				fprintf(stderr, "\t(%d chars at %X) = ",
					s->v_un.v_str.s_len,
					s->v_un.v_str.s_ptr);
				fprintf(stderr, "\"%.*s\"",
					s->v_un.v_str.s_len,
					s->v_un.v_str.s_ptr);
				break;
				}
			break;
		case 1:
			fputs(" (", stderr);
			for (i = 0, n = s->v_nsubs; i < n; ) {
				fprintf(stderr, "%d",
						s->v_un.v_vec.v_subsc[i]);
				if (++i != n)
					putc(',', stderr);
				}
			putc(')', stderr);
			break;
		case 2:		/* this is a chain of functions */
			fprintf(stderr, "\tfunction defined at line %u",
				s->v_un.v_fn.fn_curline->l_lnr);
			break;
			}
		putc('\n', stderr);
		}
}
