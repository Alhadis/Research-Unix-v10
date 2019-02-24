/* buffers for compressed data */
#define	B_SIZE	BUFSIZ*4
#define B_NUMBER	10	/* number of buffers */

#define	B_VALUE		0	/* order simplifies checking */
#define	B_ATT_CASE	1	/*	value structures used by others */
#define	B_ATT_INFO	2
#define	B_HASH_ATT	3
#define	B_COND_DEF_EL	4
#define	B_DEF_EL	5
#define	B_DEF		6
#define	B_HASH_ENV	7
#define	B_TEXT		8
#define	B_MACRO		9
#define	B_HASH_DEFINE	10	/* not yet completely implemented */

#define	NO_OFFSET	-1

#define	GET_OFFSET(p, top)	((p == NULL) ? NO_OFFSET : (p) - (top))
#define	ADD_OFFSET(p, top)	((p == NO_OFFSET) ? NULL : (p) + (top))

struct db_buffer {
	char	*top;
	char	*current;
	char	*bottom;
};

/* Special structures for quickly reading database files:
	compressed files store pointers as integer offsets from top of buffer
 */

union p_char {
	int	istring;
	char	*string;
};

union p_acase {
	int	icase;
	struct attrib_case	*acase;
};

union p_value {
	int	ivalue;
	struct value	*value;
};

union p_cond_def_el {
	int	icond_def_el;
	struct cond_def_el	*cond_def_el;
};

union p_def_el {
	int	idef_el;
	struct def_element	*def_el;
};

union p_def {
	int	idef;
	struct definition	*def;
};

/* hash data for attribute definition
 */

struct dbatt_info {
	short	stacking;
	union p_acase	firstcase;
};

struct dbatt_case {
	short	nvalues;	/* number of arguments - $* counts as one */
	short	expanding;	/* are there any cases with expanding values? */
	short	special;	/* are there any special cookies in troff,
						e.g. __LINE__, __FILE__ */
	short	allocated;
	union p_char	troff;
	union p_value	value;
	union p_acase	next;
};

/* hash data for each database definition:
	struct definition pointing to linked list of def_elements
 */
struct dbvalue {
	union p_char	value;		/* value identifier;BAD(struct value) */
	short	type;			/* fixed, variable, expanding value */
	union p_value	next;
};

struct dbdef_el {
	union p_char	attribute;
	union p_char	troff;
	union p_value	value;
	short	special;
	short	allocated;
	short	stacking;
	union p_cond_def_el	cdl;
	union p_def_el	next;
};

struct dbcond_def_el {
	short	type;
	union p_char	attribute;
	union p_value	value;
	union p_def_el	next_on_fail;
};

struct dbdefinition {
	union p_char	name;
	union p_value	values;
	union p_def_el	begin_def;
	union p_def_el	end_def;
	union p_def	sub_def;
	struct environment	*instance;
};

union p_c {
	char	*p;
	int	i;
};

union p_d {
	struct attrib_info	*a;
	struct definition	*d;
	char	*p;
	int	i;
};

typedef struct dbentry { union p_c key; union p_d data; } DBENTRY;
