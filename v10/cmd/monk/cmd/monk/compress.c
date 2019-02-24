#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"rd.h"
#include	"space.h"
#include	"dbcompress.h"

#define	USAGE	"compress [ -d db_dir ] doctype\n"
#define	OPTIONS	"d:"

/* Read ascii database files and write compressed database buffers:
	hash structures for attributes;
	hash structures for definitions;
	attrib_info structures;
	attrib_case structures;
	definition structures;
	def_el structures;
	value structures;
	text strings;
 */

extern struct db_buffer	db_struct[B_NUMBER];

main(argc, argv)
int	argc;
char	**argv;
{
	extern int	optind;
	extern char	*optarg;

	int	c;
	char	*doctype, *db_path;

	db_path = DB_PATH;
	while ((c = getopt(argc, argv, OPTIONS)) != EOF )
		switch (c) {
			case 'd':
				db_path = optarg;
				break;
			case '?':
				warn_user(0, "USAGE: %s; no option %c\n",
								USAGE, c);
		}
	if ((argc - optind) > 0)
		doctype = argv[optind];
	else
		doctype = (char *) 0;
	init_dbread();
#ifdef TIMING
	newtime("Starting to read databases\n");
#endif
	read_asciidb(DB_ATTRIBUTE, db_path, NULL);
	if (doctype == (char *) 0) {
		read_asciidb(DB_MACRO, db_path, NULL);
		read_asciidb(DB_GLOBAL, db_path, NULL);
		check_internal_defines(doctype);
		write_buffers(tmpstrcat(DB_GLOBAL, DB_COMP_EXT), db_path, 1);
	} else {
		read_asciidb(doctype, db_path, NULL);
		check_internal_defines(doctype);
		write_buffers(tmpstrcat(doctype, DB_COMP_EXT), db_path, 0);
	}
	exit(0);
}

write_buffers(filename, db_path, write_attrib)
char	*filename, *db_path;
short	write_attrib;
{
	FILE	*cdatabase;
	int	buf_size[B_NUMBER], buf_number, n;

	/* fix pointers in buffers - need to store offsets for easy read */
	for (buf_number = 0; buf_number < B_NUMBER; ++buf_number) {
		buf_size[buf_number] = 0;
		switch(buf_number) {
			case B_HASH_ATT:
				if (write_attrib)
			case B_HASH_ENV:
				subhash(buf_number, db_struct, buf_size);
				break;
			case B_ATT_INFO:
				if (write_attrib)
					subattinfo(buf_number, db_struct,
								buf_size);
				break;
			case B_ATT_CASE:
				if (write_attrib)
					subattcase(buf_number, db_struct,
								buf_size);
				break;
			case B_DEF:
				subdef(buf_number, db_struct, buf_size);
				break;
			case B_DEF_EL:
				subdef_el(buf_number, db_struct, buf_size);
				break;
			case B_COND_DEF_EL:
				subcond_def_el(buf_number, db_struct, buf_size);
				break;
			case B_VALUE:
				subvalue(buf_number, db_struct, buf_size);
				break;
			case B_MACRO:
			case B_TEXT:
				buf_size[buf_number] =
						db_struct[buf_number].current
						- db_struct[buf_number].top;
				break;
			default:
				fprintf(stderr,
				"Compressing db: buffer %d out of range\n",
								buf_number);
		}
	}
	cdatabase = fopendb(filename, db_path, "w", EXIT_ON_FAIL);
	fwrite(buf_size, sizeof(int), B_NUMBER, cdatabase);
	for (buf_number = 0; buf_number < B_NUMBER; ++buf_number)
		warn_db(0, "Buffer %d: size %d\n", buf_number,
							buf_size[buf_number]);
	for (buf_number = 0; buf_number < B_NUMBER; ++buf_number) {
		if ((n = buf_size[buf_number]) == 0)
			continue;
		if (fwrite(db_struct[buf_number].top, 1, n, cdatabase) == 0) {
			fprintf(stderr, "Compress: cannot write buffer %d\n",
							buf_number);
			return(0);
		}
	}
	return(1);
}

/* take out the bufmalloc in the read_def and read_att!!! */

subhash(buf_number, db_buffer, buf_size)
short	buf_number;
struct db_buffer	db_buffer[];
int	buf_size[];
{
	DBENTRY	*entry, *p, *pe;
	struct table_info	*gethashtbl(), *t;
	char	*top;

	if (buf_number == B_HASH_ATT) {
		t = gethashtbl(ATT_TABLE);
		top = db_buffer[B_ATT_INFO].top;
	} else
		if (buf_number == B_HASH_ENV) {
			t = gethashtbl(ENV_TABLE);
			top = db_buffer[B_DEF].top;
		} else
			fprintf(stderr,
				"Error in compressed hash %d\n", buf_number);
	/* redefinitions are wasting space in buffer - bufmalloc here */
	entry = (DBENTRY *) db_buffer[buf_number].top;
	for (p = (DBENTRY *) t->start, pe = p + t->length; p < pe; ++p)
		if (p->key.p != NULL) {
#ifdef NEWMALLOC
			entry = (DBENTRY *) bufmalloc(buf_number,
							sizeof(DBENTRY));
#endif
			entry->key.i = GET_OFFSET(p->key.p,
						db_buffer[B_TEXT].top);
			entry->data.i= GET_OFFSET(p->data.p, top);
			++entry;
		}
	buf_size[buf_number] = db_buffer[buf_number].current
				- db_buffer[buf_number].top;
}

subattinfo(buf_number, db_buffer, buf_size)
short	buf_number;
struct db_buffer	db_buffer[];
int	buf_size[];
{
	struct dbatt_info	*dbai;
	short	nitems;

	dbai = (struct dbatt_info *) db_buffer[buf_number].top;
	buf_size[buf_number] = db_buffer[buf_number].current
						- db_buffer[buf_number].top;
	nitems = buf_size[buf_number] / sizeof(struct dbatt_info);

	while (nitems--) {
		dbai->firstcase.icase = GET_OFFSET(dbai->firstcase.acase,
			(struct attrib_case *) db_buffer[B_ATT_CASE].top);
		++dbai;
	}
}

subattcase(buf_number, db_buffer, buf_size)
short	buf_number;
struct db_buffer	db_buffer[];
int	buf_size[];
{
	struct dbatt_case	*ac;
	short	nitems;

	ac = (struct dbatt_case *) db_buffer[buf_number].top;
	buf_size[buf_number] = db_buffer[buf_number].current
						- db_buffer[buf_number].top;
	nitems = buf_size[buf_number] / sizeof(struct dbatt_case);

	while (nitems--) {
		ac->troff.istring = GET_OFFSET(ac->troff.string,
						db_buffer[B_TEXT].top);
		ac->value.ivalue = GET_OFFSET(ac->value.value,
				(struct value *) db_buffer[B_VALUE].top);
		ac->next.icase = GET_OFFSET(ac->next.acase,
			(struct attrib_case *) db_buffer[B_ATT_CASE].top);
		++ac;
	}
}

subdef(buf_number, db_buffer, buf_size)
short	buf_number;
struct db_buffer	db_buffer[];
int	buf_size[];
{
	struct dbdefinition	*d;
	short	nitems;

	d = (struct dbdefinition *) db_buffer[buf_number].top;
	buf_size[buf_number] = db_buffer[buf_number].current
						- db_buffer[buf_number].top;
	nitems = buf_size[buf_number] / sizeof(struct dbdefinition);

	while (nitems--) {
		d->name.istring = GET_OFFSET(d->name.string,
						db_buffer[B_TEXT].top);
		d->values.ivalue = GET_OFFSET(d->values.value,
				(struct value *) db_buffer[B_VALUE].top);
		d->begin_def.idef_el = GET_OFFSET(d->begin_def.def_el,
			(struct def_element *) db_buffer[B_DEF_EL].top);
		d->end_def.idef_el = GET_OFFSET(d->end_def.def_el,
			(struct def_element *) db_buffer[B_DEF_EL].top);
		d->sub_def.idef = GET_OFFSET(d->sub_def.def,
			(struct definition *) db_buffer[B_DEF].top);
		++d;
	}
}

subdef_el(buf_number, db_buffer, buf_size)
short	buf_number;
struct db_buffer	db_buffer[];
int	buf_size[];
{
	struct dbdef_el	*d;
	short	nitems;

	d = (struct dbdef_el *) db_buffer[buf_number].top;
	buf_size[buf_number] = db_buffer[buf_number].current
						- db_buffer[buf_number].top;
	nitems = buf_size[buf_number] / sizeof(struct dbdef_el);

	while (nitems--) {
		d->attribute.istring = GET_OFFSET(d->attribute.string,
						db_buffer[B_TEXT].top);
		d->troff.istring = GET_OFFSET(d->troff.string,
						db_buffer[B_TEXT].top);
		d->value.ivalue = GET_OFFSET(d->value.value,
				(struct value *) db_buffer[B_VALUE].top);
		d->cdl.icond_def_el = GET_OFFSET(d->cdl.cond_def_el,
			(struct cond_def_el *) db_buffer[B_COND_DEF_EL].top);
		d->next.idef_el = GET_OFFSET(d->next.def_el,
			(struct def_element *) db_buffer[B_DEF_EL].top);
		++d;
	}
}

subcond_def_el(buf_number, db_buffer, buf_size)
short	buf_number;
struct db_buffer	db_buffer[];
int	buf_size[];
{
	struct dbcond_def_el	*d;
	short	nitems;

	d = (struct dbcond_def_el *) db_buffer[buf_number].top;
	buf_size[buf_number] = db_buffer[buf_number].current
						- db_buffer[buf_number].top;
	nitems = buf_size[buf_number] / sizeof(struct dbcond_def_el);

	while (nitems--) {
		d->attribute.istring = GET_OFFSET(d->attribute.string,
						db_buffer[B_TEXT].top);
		d->value.ivalue = GET_OFFSET(d->value.value,
				(struct value *) db_buffer[B_VALUE].top);
		d->next_on_fail.idef_el = GET_OFFSET(d->next_on_fail.def_el,
			(struct def_element *) db_buffer[B_DEF_EL].top);
		++d;
	}
}

subvalue(buf_number, db_buffer, buf_size)
short	buf_number;
struct db_buffer	db_buffer[];
int	buf_size[];
{
	struct dbvalue	*v;
	short	nitems;

	v = (struct dbvalue *) db_buffer[buf_number].top;
	buf_size[buf_number] = db_buffer[buf_number].current
						- db_buffer[buf_number].top;
	nitems = buf_size[buf_number] / sizeof(struct dbvalue);

	while (nitems--) {
		v->value.istring = GET_OFFSET(v->value.string,
					db_buffer[B_TEXT].top);
		v->next.ivalue = GET_OFFSET(v->next.value,
				(struct value *) db_buffer[B_VALUE].top);
		++v;
	}
}

/* check that internal definitions are all defined */
	/* monk envir at beginning of usertext*/
	/* monk envir at end of usertext*/
	/* envir at beg of usertext*/
	/* envir invoked at end of usertext */
	/* handles multiple user nls */
	/* handles whitespace following newline */

check_internal_defines(doctype)
char	*doctype;
{
	extern char	*global_internal_defs[], *doc_internal_defs[];
	char	**p;

	if (doctype == (char *) 0) {
		for (p = &global_internal_defs[0]; *p != NULL; ++p)
			if (hashfind(ENV_TABLE, *p) == (ENTRY *) NULL)
				warn_db(0, "Internal %s %s is not defined\n",
							ENVIRONMENT, *p);
	} else
		for (p = &doc_internal_defs[0]; *p != NULL; ++p)
			if (hashfind(ENV_TABLE, *p) == (ENTRY *) NULL)
				warn_db(0, "Internal %s %s is not defined\n",
							ENVIRONMENT, *p);
}
