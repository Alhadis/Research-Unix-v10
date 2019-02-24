/*
 * mkconf.c	1.10	85/01/03
 *	Functions in this file build conf.c from the devices list
 */

#include <stdio.h>
#include <ctype.h>
#include "y.tab.h"
#include "config.h"

#define next_word(fp, wd)\
    { register char *word = get_word(fp);\
	if (word == WEOF) return EOF; \
	else wd = word; }

#define max(a,b)    ((a) > (b) ? (a) : (b))
#define bit(n)	    (1L << (n))

static struct dev_list *dcur;

struct routine {
    char    *rt_name;
    char    *rt_type;
    int	    rt_class;
    char    *rt_default;
    char    *rt_empty;
};

#define FUNC	1
#define POINTER	2
#define LITERAL 3

struct routine block_routines[] = {
    {"open", "int", FUNC, "nulldev", "nodev"},
    {"close", "int", FUNC, "nulldev", "nodev"},
    {"strategy", "int", FUNC, "nodev"},
    {"dump", "int", FUNC, "nodev"},
    {"B_TAPE", NULL, LITERAL, "0"},
    {NULL}
};
struct routine char_routines[] = {
    {"open", "int", FUNC, "nulldev", "nodev"},
    {"close", "int", FUNC, "nulldev", "nodev"},
    {"read", "int", FUNC, "nodev"},
    {"write", "int", FUNC, "nodev"},
    {"ioctl", "int", FUNC, "nodev"},
    {"reset", "int", FUNC, "nulldev"},
    {"", NULL, NULL, "NULL"},
    {NULL}
};
struct routine stream_routines[] = {
    {"", NULL, NULL, "nodev"},
    {"", NULL, NULL, "nodev"},
    {"", NULL, NULL, "nodev"},
    {"", NULL, NULL, "nodev"},
    {"", NULL, NULL, "nodev"},
    {"reset", "int", FUNC, "nulldev"},
    {"info", "struct streamtab", POINTER, "NULL"},
    {NULL}
};
struct routine fs_routines[] = {
    {"put", "int", FUNC, "0"},
    {"get", "struct inode *", FUNC, "0"},
    {"free", "int", FUNC, "0"},
    {"updat", "int", FUNC, "0"},
    {"read", "int", FUNC, "0"},
    {"write", "int", FUNC, "0"},
    {"trunc", "int", FUNC, "0"},
    {"stat", "int", FUNC, "0"},
    {"nami", "int", FUNC, "0"},
    {"mount", "int", FUNC, "0"},
    {"ioctl", "int", FUNC, "0"},
    {NULL}
};
struct routine ld_routines[] = {
    {"info", "struct streamtab", POINTER, "NULL"},
    {NULL}
};

/*
 * dev_lookup
 *	look up a device name
 */

struct dev_list *dev_lookup(type, num)
register int type, num;
{
    register struct dev_list *devp;

    for (devp = devtab ; devp != NULL; devp = devp->dev_next)
    {
	if (devp->dev_type == type && devp->dev_num == num)
	    return devp;
    }
    return NULL;
}

/*
 * new_dent
 *	Make a new device list entry
 */

struct dev_list *new_dent()
{
    register struct dev_list *devp;

    devp = (struct dev_list *) malloc(sizeof *devp);
    devp->dev_next = NULL;
    if (dcur == NULL)
	dcur = devtab = devp;
    else
	dcur->dev_next = devp;
    dcur = devp;
    return devp;
}


/*
 * devices:
 *	Read in the "devices" file.
 *	Store it in the devtab linked list
 */

read_devices()
{

    devtab = NULL;
    read_devices_file(GLOBAL("devices"), TRUE);
    read_devices_file(LOCAL("devices"), FALSE);
}

static struct litlst *litlast;

read_devices_file(filename, must_exist)
    char *filename;
{
    FILE *fp;
    register struct dev_list *tp;
    register struct litlst *litp;
    register char *wd, *cond;
    int standard, type, num;
    char *index();

    fp = fopen(filename, "r");
    if (fp == NULL) {
	if (must_exist) {
	    perror(filename);
	    exit(1);
	} else
	    return;
    }
    while((wd = get_word(fp)) != WEOF)
    {
	if (wd == NULL)
	    continue;
	if (eq(wd, ":")) {
	    char line[100];
	    fgets(line, 100, fp);
	    if (!isspace(line[0]))
		continue;
	    if (wd = index(line, '\n'))
		*wd = '\0';
	    litp = (struct litlst *)malloc(sizeof(*litp) + strlen(line));
	    if (litp == NULL) {
		fprintf(stderr, "Out of memory\n");
		exit(10);
	    }
	    strcpy(litp->line = (char *)(litp + 1), line + 1);
	    litp->lit_next = NULL;
	    if (littab == NULL)
		littab = litlast = litp;
	    else
		litlast = litlast->lit_next = litp;
	    continue;
	}
	standard = FALSE;
	cond = NULL;
	if (eq(wd, "standard")) {
	    next_word(fp, wd);
	    standard = TRUE;
	} else if (eq(wd, "if")) {
	    next_word(fp, wd);
	    cond = ns(wd);
	    next_word(fp, wd);
	}
	if (eq(wd, "stream-device"))
	    type = STREAM_DEVICE;
	else if (eq(wd, "block-device"))
	    type = BLOCK_DEVICE;
	else if (eq(wd, "device"))
	    type = CHAR_DEVICE;
	else if (eq(wd, "line-discipline"))
	    type = LINE_DISC;
	else if (eq(wd, "file-system"))
	    type = FILE_SYS;
	else {
	    fprintf(stderr, "Unrecognized device type %d.\n", type);
	    exit(10);
	}

	next_word(fp, wd);
	if (wd == NULL) {
	    fprintf(stderr, "No device number.\n");
	    exit(10);
	}
	num = atoi(wd);

	next_word(fp, wd);
	if (wd == NULL) {
	    fprintf(stderr, "Huh, no name for device.\n");
	    exit(10);
	}
	if ((tp = dev_lookup(type, num)) == NULL)
	    tp = new_dent();
	tp->dev_name = ns(wd);
	tp->dev_type = type;
	tp->dev_num = num;
	tp->dev_standard = standard;
	tp->dev_if = cond;
	if (num > max_num[type])
	    max_num[type] = num;

	next_word(fp, wd);
	if (wd == NULL) {
	    tp->dev_mask = 0L;
	    continue;
	}
	tp->dev_prefix = ns(wd);

	switch (type) {
	    case STREAM_DEVICE:
		device_params(fp, tp, stream_routines);
		break;
	    case BLOCK_DEVICE:
		device_params(fp, tp, block_routines);
		break;
	    case CHAR_DEVICE:
		device_params(fp, tp, char_routines);
		break;
	    case LINE_DISC:
		device_params(fp, tp, ld_routines);
		break;
	    case FILE_SYS:
		device_params(fp, tp, fs_routines);
		break;
	}
    }
    fclose(fp);
}


/*
 *  device_params:
 *	read in a list of device parameters
 *
 */

device_params(fp, dp, routines)
FILE *fp;
struct dev_list *dp;
struct routine *routines;
{
    register struct routine *rp;
    char *wd;

    dp->dev_mask = 0L;
    for (;;) {
	next_word(fp, wd);
	if (wd == NULL)
	    break;
	for (rp = routines; rp->rt_name != NULL; ++rp)
	    if (eq(wd, rp->rt_name))
		break;
	if (rp->rt_name == NULL) {
	    fprintf(stderr, "Unknown entry point name '%s' for device %s.\n",
			    wd, dp->dev_name);
	    exit(10);
	}
	dp->dev_mask |= bit(rp - routines);
    }
}


conf()
{
    FILE *fp;
    register struct dev_list	*dp;

    read_devices();

    fp = fopen(LOCAL("conf.c"), "w");
    if (fp == NULL) {
	perror(LOCAL("conf.c"));
	exit(1);
    }
    fprintf(fp, "\t/* conf.c built automatically by config */\n\n");
    fprintf(fp, "#include \"../h/param.h\"\n");
    fprintf(fp, "#include \"../h/systm.h\"\n");
    fprintf(fp, "#include \"../h/conf.h\"\n");
    fprintf(fp, "#include \"../h/inode.h\"\n");
    fprintf(fp, "#include \"../h/stream.h\"\n");
    fprintf(fp, "#include \"../h/buf.h\"\n");
    fprintf(fp, "int nulldev(), nodev();\n\n");

    for (dp = devtab; dp; dp = dp->dev_next) {
	if (dp->dev_mask == 0L)
	    continue;
	switch (dp->dev_type) {
	    case CHAR_DEVICE:
		do_declare(fp, dp, char_routines);
		break;
	    case BLOCK_DEVICE:
		do_declare(fp, dp, block_routines);
		break;
	    case STREAM_DEVICE:
		do_declare(fp, dp, stream_routines);
		break;
	    case LINE_DISC:
		do_declare(fp, dp, ld_routines);
		break;
	    case FILE_SYS:
		decl_list(fp, dp, fs_routines);
		break;
	}
	putc('\n', fp);
    }

    fprintf(fp, "struct bdevsw bdevsw[] = {\n");
    do_table(fp, block_routines, BLOCK_DEVICE, NULL, NULL);
    fprintf(fp, "    0\n};\n\n");

    fprintf(fp, "struct cdevsw cdevsw[] = {\n");
    do_table(fp, char_routines, CHAR_DEVICE, stream_routines, STREAM_DEVICE);
    fprintf(fp, "    0\n};\n");
    fprintf(fp, "int nchrdev = sizeof(cdevsw) / sizeof(cdevsw[0]) - 1;\n\n");

    fprintf(fp, "struct fstypsw fstypsw[] = {\n");
    do_table(fp, fs_routines, FILE_SYS, NULL, NULL);
    fprintf(fp, "};\n");
    fprintf(fp, "int nfstyp = sizeof(fstypsw) / sizeof(fstypsw[0]);\n\n");

    fprintf(fp, "struct streamtab *streamtab[] = {\n");
    do_table(fp, ld_routines, LINE_DISC, NULL, NULL);
    fprintf(fp, "};\n");
    fprintf(fp, "int nstream = sizeof(streamtab) / sizeof(streamtab[0]);\n\n");

    while (littab != NULL) {
	fprintf(fp, "%s\n", littab->line);
	littab = littab->lit_next;
    }

    fclose(fp);
}


char *toentry(dp, routine)
    struct dev_list *dp;
    char *routine;
{
    static char entry[40];

    sprintf(entry, "%s%s", dp->dev_prefix, routine);
    return entry;
}

do_declare(fp, dp, routines)
    FILE *fp;
    struct dev_list *dp;
    struct routine *routines;
{
    char *tomacro();

    if (dp->dev_if)
	fprintf(fp, "#if %s\n", dp->dev_if);
    else if (!dp->dev_standard) {
	include(fp, dp->dev_name);
	fprintf(fp, "#if %s > 0\n", tomacro(dp->dev_name));
    }

    decl_list(fp, dp, routines);

    if (!dp->dev_standard) {
	fprintf(fp, "#else\n");
	define_list(fp, dp, routines);
	fprintf(fp, "#endif\n");
    }
}

decl_list(fp, dp, routines)
    FILE    *fp;
    struct dev_list *dp;
    struct routine *routines;
{
    register struct routine *rp, *rp2;
    register long   mask;
    register int    ndone;
    char    *entry;

    for (rp = routines, mask = dp->dev_mask; rp->rt_name != NULL; ++rp) {
	for (rp2 = rp, ndone = 0; rp2->rt_name != NULL; ++rp2) {
	    if (rp2->rt_type == NULL || (mask & bit(rp2 - routines)) == 0)
		continue;
	    entry = toentry(dp, rp2->rt_name);
	    if (eq(rp2->rt_type, rp->rt_type) && !declared(entry, FALSE)) {
		mask &= ~bit(rp2 - routines);
		if (++ndone == 1)
		    fprintf(fp, "    extern %s ", rp->rt_type);
		else
		    putc(',', fp);
		fprintf(fp, "%s%s", entry, rp2->rt_class == FUNC ? "()" : "");
	    }
	}
	if (ndone > 0)
	    fprintf(fp, ";\n");
    }
}


define_list(fp, dp, routines)
    FILE *fp;
    struct dev_list *dp;
    struct routine *routines;
{
    register struct routine *rp;
    char    *entry;

    for (rp = routines; rp->rt_name != NULL; ++rp) {
	if (rp->rt_type != NULL && (dp->dev_mask & bit(rp - routines))) {
	    entry = toentry(dp, rp->rt_name);
	    if (!declared(entry, FALSE)) {
		fprintf(fp, "#   define %s\t", entry);
		if (rp->rt_class == POINTER)
		    fprintf(fp, "*(%s *)", rp->rt_type);
		fprintf(fp, "%s\n", rp->rt_empty? rp->rt_empty
						: rp->rt_default);
	    }
	}
    }
}

static struct idlst *decs;

declared(str, declare)
    char    *str;
{
    register struct idlst *dec;

    for (dec = decs; dec; dec = dec->id_next)
	if (eq(dec->id, str))
	    return TRUE;

    if (declare) {
	if ((dec = (struct idlst *)malloc(sizeof(*dec))) == NULL) {
	    fprintf(stderr, "Out of memory\n");
	    exit(10);
	}
	dec->id = str;
	dec->id_next = decs;
	decs = dec;
    }

    return FALSE;
}

do_table(fp, rt1, devtype1, rt2, devtype2)
    FILE *fp;
    struct routine *rt1, *rt2;
{
    register struct dev_list	*dp;
    register int    n, maxn;

    maxn = rt2 ? max(max_num[devtype1],max_num[devtype2]) : max_num[devtype1];
    for (n = 0; n <= maxn; ++n) {
	for (dp = devtab; dp; dp = dp->dev_next) {
	    if (dp->dev_num != n)
		continue;
	    if (dp->dev_type == devtype1) {
		fprintf(fp, "/*%s*/\t", dp->dev_name);
		filled_list(fp, dp, dp->dev_mask, rt1);
		fprintf(fp, "\t/* %d */\n", n);
		break;
	    } else if (dp->dev_type == devtype2) {
		fprintf(fp, "/*%s*/\t", dp->dev_name);
		filled_list(fp, dp, dp->dev_mask, rt2);
		fprintf(fp, "\t/* %d */\n", n);
		break;
	    }
	}
	if (dp == NULL) {
	    fprintf(fp, "/*xx*/\t");
	    filled_list(fp, NULL, 0L, rt1);
	    fprintf(fp, "\t/* %d */\n", n);
	}
    }
}

filled_list(fp, dp, mask, routines)
    FILE *fp;
    struct dev_list *dp;
    long mask;
    struct routine *routines;
{
    register struct routine *rp;

    for (rp = routines; rp->rt_name != NULL; ++rp) {
	if (mask & bit(rp - routines)) {
	    if (rp->rt_class == POINTER)
		putc('&', fp);
	    if (rp->rt_class == LITERAL)
		fprintf(fp, "%s, ", rp->rt_name);
	    else
		fprintf(fp, "%s, ", toentry(dp, rp->rt_name));
	} else if (dp == NULL && rp->rt_empty)
	    fprintf(fp, "%s, ", rp->rt_empty);
	else
	    fprintf(fp, "%s, ", rp->rt_default);
    }
}

static struct idlst *incs;

include(fp, name)
    FILE    *fp;
    char    *name;
{
    register struct idlst *inc;

    for (inc = incs; inc; inc = inc->id_next)
	if (eq(inc->id, name))
	    return;

    fprintf(fp, "#include \"%s.h\"\n", name);
    if ((inc = (struct idlst *)malloc(sizeof(*inc))) == NULL) {
	fprintf(stderr, "Out of memory\n");
	exit(10);
    }
    inc->id = name;
    inc->id_next = incs;
    incs = inc;
}
