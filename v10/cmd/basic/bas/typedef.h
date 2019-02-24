#include "defines.h"

/*
 * Type and structure definitions for basic
 */

typedef short unsigned	Lnr;
typedef struct textline *Linep;
struct textline {
	unsigned	l_len;
	Lnr		l_lnr;
	unsigned	: 0;
	char		l_line[MAXLINELEN];
	};
struct linehdr {
	unsigned		l_len;
	Lnr		l_lnr;
	unsigned	: 0;
	};

#define LINESIZE	(sizeof(struct linehdr))

typedef struct string	String;
struct string {
	char		*s_ptr;
	unsigned	s_len;
	};

typedef struct vector	Vector;
struct vector {
	int		v_subsc[MAXSUBS];
	unsigned	: 0;
	union {
		float		v_fltvec[1];
		double		v_dblvec[1];
		int		v_intvec[1];
		String		v_strvec[1];
		}	v_vecun;
	};
struct vechdr {
	int		v_subsc[MAXSUBS];
	unsigned	: 0;
	};

#define VECHDRSIZE		(sizeof(struct vechdr))

typedef struct func	Func;
struct func {
	char		*fn_inptr;
	Linep		fn_curline;
	};

typedef struct symbol	Symbol;
typedef Symbol		*Symptr;
struct symbol {
	Symptr		v_next;
	char		v_name[2];
	short unsigned	v_nsubs;
	unsigned	: 0;
	union {
		float		v_float;
		double		v_double;
		int		v_int;
		String		v_str;
		Vector		v_vec;
		Func		v_fn;
		}	v_un;
	};
struct symhdr {
	Symptr		v_next;
	char		v_name[2];
	short unsigned	v_nsubs;
	unsigned	: 0;
	};

#define SYMHDRLEN	(sizeof(struct symhdr))

typedef struct forstk	Forstk;
struct forstk {
	Linep		frk_curline;
	char		*frk_inptr;
	Symptr		frk_symp;
	double		frk_incr;
	double		frk_last;
	};

typedef struct fnstk	Fnstk;
struct fnstk {
	Linep		fnk_curline;
	char		*fnk_inptr;
	char		*fnk_line;
	char		*fnk_ptr;
	Symptr		fnk_symp;
	short		fnk_type;
	short		fnk_argcnt;
	};

typedef struct gosub	Gosub;
struct gosub {
	Linep		g_curline;
	char		*g_inptr;
	};

typedef struct stkfr	Stkfr;
typedef Stkfr		*Stkptr;
struct stkfr {
	char unsigned	k_type;
	short unsigned	k_len;
	unsigned	: 0;
	union {
		double		k_dbl;
		Symptr		k_symp;
		String		k_str;
		Forstk		k_frk;
		Fnstk		k_fnk;
		Gosub		k_gosub;
		}	k_un;
	};
struct stkhdr {
	char unsigned	k_type;
	short unsigned	k_len;
	unsigned	: 0;
	};

#define FRHDRLEN		(sizeof(struct stkhdr))
#define DBLFRLEN		(FRHDRLEN + sizeof(double))
#define VARFRLEN		(FRHDRLEN + sizeof(Symptr))
#define STRFRLEN		(FRHDRLEN + sizeof(String))
#define FORFRLEN		(FRHDRLEN + sizeof(Forstk))
#define FNFRLEN 		(FRHDRLEN + sizeof(Fnstk))
#define GOSUBFRLEN		(FRHDRLEN + sizeof(Gosub))
