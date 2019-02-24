#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"rd.h"
#include	"dbcompress.h"

/* Read compressed database:
	buffers:
		hash structures for attributes;
		hash structures for definitions;
		attrib_info structures;
		attrib_case structures;
		definition structures;
		def_el structures;
		value structures;
		text strings;
 */

r_buffers(database, db_path)
FILE	*database;
char	*db_path;
{
	int	buf_size[B_NUMBER], buf_number, n, nread;
	char	*db_buf[B_NUMBER];

	fread(&buf_size[0], sizeof(int), B_NUMBER, database);
	for (buf_number = 0; buf_number < B_NUMBER; ++buf_number) {
		db_buf[buf_number] = 0;
		if ((n = buf_size[buf_number]) == 0)
			continue;
		if (buf_number == B_MACRO) {
			fixmacros(database, n);
			continue;
		}
		db_buf[buf_number] = mymalloc((unsigned) buf_size[buf_number]);
		if ((nread = fread(db_buf[buf_number], 1, n, database))
						!= buf_size[buf_number]) {
			fprintf(stderr,
			"Cannot read compressed database %d: read %d of %d\n",
						buf_number, nread, n);
			exit(0);
		}
	}
	for (buf_number = 0; buf_number < B_NUMBER; ++buf_number) {
	/* fix pointers in buffers */
		switch(buf_number) {
			case B_HASH_ATT:
			case B_HASH_ENV:
				fixhash(buf_number, db_buf, buf_size);
				break;
			case B_ATT_INFO:
				fixattinfo(buf_number, db_buf, buf_size);
				break;
			case B_ATT_CASE:
				fixattcase(buf_number, db_buf, buf_size);
				break;
			case B_DEF:
				fixdef(buf_number, db_buf, buf_size);
				break;
			case B_DEF_EL:
				fixdef_el(buf_number, db_buf, buf_size);
				break;
			case B_COND_DEF_EL:
				fixcond_def_el(buf_number, db_buf, buf_size);
				break;
			case B_VALUE:
				fixvalue(buf_number, db_buf, buf_size);
				break;
			case B_MACRO:
			case B_TEXT:
				break;
			default:
				fprintf(stderr,
				"Reading compressed buffer: %d out of range\n",
							buf_number);
		}
	}
}

fixhash(buf_number, db_buf, buf_size)
short	buf_number;
char	*db_buf[B_NUMBER];
int	buf_size[B_NUMBER];
{
	DBENTRY	*entry;
	int	hashtable;
	short	nitems;
	char	*data, *text, *type;

	nitems = buf_size[buf_number] / sizeof(ENTRY);

	if (buf_number == B_HASH_ATT) {
		data = db_buf[B_ATT_INFO];
		type = ATTRIBUTE;
		hashtable = ATT_TABLE;
	} else
		if (buf_number == B_HASH_ENV) {
			data = db_buf[B_DEF];
			type = ENVIRONMENT;
			hashtable = ENV_TABLE;
		} else
			fprintf(stderr,
				"Error in reading database %d\n", buf_number);
	text = db_buf[B_TEXT];
	for (entry = (DBENTRY *) db_buf[buf_number]; nitems--; ++entry) {
		entry->key.p = ADD_OFFSET(entry->key.i, text);
		entry->data.p = ADD_OFFSET(entry->data.i, data);
		hashncheck(hashtable, (ENTRY *) entry, type, 0);
	}
}

fixattinfo(buf_number, db_buf, buf_size)
short	buf_number;
char	*db_buf[B_NUMBER];
int	buf_size[B_NUMBER];
{
	struct dbatt_info	*dbai;
	short	nitems;

	dbai = (struct dbatt_info *) db_buf[buf_number];
	nitems = buf_size[buf_number] / sizeof(struct dbatt_info);

	while (nitems--) {
		dbai->firstcase.acase = ADD_OFFSET(dbai->firstcase.icase,
			(struct attrib_case *) db_buf[B_ATT_CASE]);
		dbai++;
	}
}

fixattcase(buf_number, db_buf, buf_size)
short	buf_number;
char	*db_buf[B_NUMBER];
int	buf_size[B_NUMBER];
{
	struct dbatt_case	*ac;
	short	nitems;

	ac = (struct dbatt_case *) db_buf[buf_number];
	nitems = buf_size[buf_number] / sizeof(struct dbatt_case);

	while (nitems--) {
		ac->troff.string = ADD_OFFSET(ac->troff.istring,
							db_buf[B_TEXT]);
		ac->value.value = ADD_OFFSET(ac->value.ivalue,
				(struct value *) db_buf[B_VALUE]);
		ac->next.acase = ADD_OFFSET(ac->next.icase,
			(struct attrib_case *) db_buf[B_ATT_CASE]);
		++ac;
	}
}

fixdef(buf_number, db_buf, buf_size)
short	buf_number;
char	*db_buf[B_NUMBER];
int	buf_size[B_NUMBER];
{
	struct dbdefinition	*d;
	short	nitems;

	d = (struct dbdefinition *) db_buf[buf_number];
	nitems = buf_size[buf_number] / sizeof(struct dbdefinition);

	while (nitems--) {
		d->name.string = ADD_OFFSET(d->name.istring, db_buf[B_TEXT]);
		d->values.value = ADD_OFFSET(d->values.ivalue,
				(struct value *) db_buf[B_VALUE]);
		d->begin_def.def_el = ADD_OFFSET(d->begin_def.idef_el,
				(struct def_element *) db_buf[B_DEF_EL]);
		d->end_def.def_el = ADD_OFFSET(d->end_def.idef_el,
				(struct def_element *) db_buf[B_DEF_EL]);
		d->sub_def.def = ADD_OFFSET(d->sub_def.idef,
				(struct definition *) db_buf[B_DEF]);
		++d;
	}
}

fixdef_el(buf_number, db_buf, buf_size)
short	buf_number;
char	*db_buf[B_NUMBER];
int	buf_size[B_NUMBER];
{
	struct dbdef_el	*d;
	short	nitems;

	d = (struct dbdef_el *) db_buf[buf_number];
	nitems = buf_size[buf_number] / sizeof(struct dbdef_el);

	while (nitems--) {
		d->attribute.string = ADD_OFFSET(d->attribute.istring,
						db_buf[B_TEXT]);
		d->troff.string = ADD_OFFSET(d->troff.istring,
						db_buf[B_TEXT]);
		d->value.value = ADD_OFFSET(d->value.ivalue,
				(struct value *) db_buf[B_VALUE]);
		d->cdl.cond_def_el = ADD_OFFSET(d->cdl.icond_def_el,
			(struct cond_def_el *) db_buf[B_COND_DEF_EL]);
		d->next.def_el = ADD_OFFSET(d->next.idef_el,
				(struct def_element *) db_buf[B_DEF_EL]);
		++d;
	}
}

fixcond_def_el(buf_number, db_buf, buf_size)
short	buf_number;
char	*db_buf[B_NUMBER];
int	buf_size[B_NUMBER];
{
	struct dbcond_def_el	*d;
	short	nitems;

	d = (struct dbcond_def_el *) db_buf[buf_number];
	nitems = buf_size[buf_number] / sizeof(struct dbcond_def_el);

	while (nitems--) {
		d->attribute.string = ADD_OFFSET(d->attribute.istring,
						db_buf[B_TEXT]);
		d->value.value = ADD_OFFSET(d->value.ivalue,
				(struct value *) db_buf[B_VALUE]);
		d->next_on_fail.def_el = ADD_OFFSET(d->next_on_fail.idef_el,
				(struct def_element *) db_buf[B_DEF_EL]);
		++d;
	}
}

fixvalue(buf_number, db_buf, buf_size)
short	buf_number;
char	*db_buf[B_NUMBER];
int	buf_size[B_NUMBER];
{
	struct dbvalue	*v;
	short	nitems;

	v = (struct dbvalue *) db_buf[buf_number];
	nitems = buf_size[buf_number] / sizeof(struct dbvalue);

	while (nitems--) {
		v->value.string = ADD_OFFSET(v->value.istring,
							db_buf[B_TEXT]);
		v->next.value = ADD_OFFSET(v->next.ivalue,
				(struct value *) db_buf[B_VALUE]);
		++v;
	}
}

fixmacros(macrofile, count)
FILE	*macrofile;
int	count;
{
	int	c;

	while (count-- > 0 && (c = mygetc(macrofile)) != EOF)
		textputc(c, MONK_DB);
}
