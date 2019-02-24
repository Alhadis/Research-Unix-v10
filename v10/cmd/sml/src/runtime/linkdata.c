/* Copyright 1989 by AT&T Bell Laboratories */
/* linkdata.c -- group files into the text segment of a .o file */

#define SYMBOL	    "_datalist"
#define END_SYMBOL  "_endmo"

#include <stdio.h>
#include <a.out.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "tags.h"

#define LONG	    (sizeof(long))
#define align(x)    (((x) + (LONG - 1)) & ~(LONG - 1))

#ifdef SPARC
#define relocation_info reloc_info_sparc
#endif

#define RELINFO_SZ  (sizeof (struct relocation_info))

struct relocation_info rconst, *rel;
struct nlist nlconst, *nl;

struct str_table {  /* The string table buffer. */
    int	    len;    /* length including this word */
    char    strs[4];
} *str_tbl;
char *str_ptr;	/* the next location in the string table. */

/* add_name:
 * Extract the structure name from the file name and add it to the string table
 * buffer (with an initial "_").
 */
void add_name (file)
    char *file;
{
    register char   *p, *begin;
    int		    len, res;

  /* find the beginning of the file name. */
    for (p = begin = file;  *p;	 p++) {
	if (*p == '/')
	    begin = p+1;
    }

  /* find the ".mo" */
    for (p = begin, len = 0;  *p && (*p != '.');  p++, len++)
	continue;

    *str_ptr++ = '_';
    strncpy(str_ptr, begin, len);
    str_ptr += len;
    *str_ptr++ = '\0';
    str_tbl->len += (len+2);
}

void die(s)
    char *s;
{
    perror(s);
    exit(1);
}

extern long *malloc();	/* wrong but convenient */


/* link:
 * append the code from file to linked code.  pc is the relative starting address,
 * last = 1, if this is the last file to link.
 */
int link (file, pc, last)
    char *file; long pc; int last;
{
    int fd;
    long i, len, n, nbytes;
    char *s;
    struct stat st;
    static int bufsize = 0;
    static long *buf;

    if ((fd = open(file, 0)) == -1) {
	char	buf[64];
	sprintf(buf, "open: %s", file);
	die(buf);
    }
    if (fstat(fd, &st) == -1)
	die("stat");

    n = st.st_size;
    len = strlen(file);
    nbytes = align(n) + align(len) + 6 * LONG;

    if (n + len + 40 > bufsize) {
	bufsize = n + len + 4096;
	if (buf != NULL)
	    free(buf);
	buf = malloc(bufsize);
	if (buf == NULL)
	    die("malloc(buf)");
    }

  /* initialize the header record and add its fields to the list of
   * relocatable addresses.
   */
    buf[0] = mak_desc(3, tag_record);
#ifdef SPARC
#define REL(addr, addend)   \
    (*rel = rconst, rel->r_addend = (addend), (rel++)->r_address = (addr))

    buf[1] = 0;
    REL(pc + LONG, pc + 5 * LONG);
    buf[2] = 0;
    REL(pc + 2 * LONG, pc + 6 * LONG + align(len));
    if (last)
	buf[3] = 1;			   /* either ML 0 */
    else {
	buf[3] = 0;
	REL(pc + 3 * LONG, pc + nbytes + LONG);
    }
#else

#define REL(addr)   (*rel = rconst, (rel++)->r_address = (addr))

    buf[1] = pc + 5 * LONG;		   /* address of file name */
    REL(pc + LONG);
    buf[2] = buf[1] + LONG + align(len);   /* address of code */
    REL(pc + 2 * LONG);
    if (last)
	buf[3] = 1;			   /* either ML 0 */
    else {
	buf[3] = pc + nbytes + LONG;	   /* or the address of next module */
	REL(pc + 3 * LONG);
    }
#endif

    buf[4] = mak_desc(len, tag_string);	   /* tag for file name */
    strcpy(&buf[5], file);
    s = ((char *) &buf[5]) + len;
    for (i = align(len); i > len; i--)
	*s++ = 0;

    *((long *) s) = mak_desc(n, tag_string);  /* tag for code */
    s += LONG;

  /* record this structure in the symbol and string tables */
    *nl = nlconst;
    nl->n_un.n_strx = str_tbl->len;
    nl->n_value = pc + 6*LONG + align(len);  /* relative code address */
    nl++;
    add_name(file);

    if (read(fd, s, n) != n)
	die("read");
    s += n;
    for (i = align(n); i > n; i--)
	*s++ = 0;

    if (close(fd) == -1)
	die("close");

    assert(s - nbytes == (char *) buf);
    write(1, buf, nbytes);
    return nbytes;
}

int main(argc, argv)
    int argc;
    char *argv[];
{
    long pc = 0;
    struct exec exec;
    struct nlist *nlbase;
    struct relocation_info *relbase;
    int i, sz;

  /* allocate space for the string table */
    sz = strlen(SYMBOL)+strlen(END_SYMBOL)+LONG+1;
    for (i = 1;	 i < argc;  i++)
	sz += strlen(argv[i])+2;
    str_tbl = (struct str_table *) malloc(align(sz));
    if ((long)str_tbl == NULL)
	die("malloc(str_tbl)");

  /* initialize the string table buffer with the "_datalist" string */
    str_ptr = str_tbl->strs;
    strcpy(str_ptr, SYMBOL);
    str_ptr += strlen(SYMBOL)+1;
    str_tbl->len = LONG + strlen(SYMBOL)+1;

  /* Allocate space for nlist and reloc info buffers */
    nl = nlbase = (struct nlist *) malloc((argc+1) * sizeof(struct nlist));
    if ((long) nl == NULL)
	die("malloc(nl)");
    rel = relbase = (struct relocation_info *) malloc(3 * argc * RELINFO_SZ);
    if ((long) rel == NULL)
	die("malloc(rel)");

  /* build a nlist constant to initialize from, and initialize the datalist entry. */
    nlconst.n_other	= 0;
    nlconst.n_desc	= 0;
    if (argc > 1) {
	nlconst.n_type	= N_EXT | N_TEXT;	/* global in text area */
	nlconst.n_value = 4;			/* for datalist */
    }
    else {
      /* Actually, there is only one symbol (ie., datalist) */
	nlconst.n_type	= N_EXT | N_ABS;
	nlconst.n_value = 1;	/* ML zero */
    }
    *nl = nlconst; (nl++)->n_un.n_strx = 4; /* datalist, first string table entry */

  /* build a relocation constant to assign from */
#ifdef SPARC
    rconst.r_address   = 0;
    rconst.r_index     = N_TEXT;
    rconst.r_extern    = 0;	   /* r_index is the segment (text) */
    rconst.r_type      = RELOC_32;
    rconst.r_addend    = 0;
#else
    rconst.r_address   = 0;
    rconst.r_symbolnum = 4; /* i don't know why this works.  magic? */
    rconst.r_pcrel     = 0;
    rconst.r_length    = 2; /* long */
    rconst.r_extern    = 0;
#endif

  /* initialize the header, we fill in the text size info later */
#ifdef M68
    exec.a_machtype = M_68020;
#endif
#ifdef SPARC
    exec.a_dynamic	= 0;
    exec.a_machtype	= M_SPARC;
    exec.a_toolversion	= 1;
#endif
    exec.a_magic    = OMAGIC;
    exec.a_data	    = 0;
    exec.a_bss	    = 0;
  /* one per mo plus "datalist" and "_endmo" */
    exec.a_syms	    = (argc+1) * sizeof(struct nlist);
    exec.a_entry    = 0;
    exec.a_drsize   = 0;
    if (write(1, &exec, sizeof exec) == -1)
	die("write");

    for (i=1; i<argc; i++)
	pc += link(argv[i], pc, i==argc-1);

    exec.a_text	    = pc;
    exec.a_trsize   = (rel - relbase) * RELINFO_SZ;

    if (write(1, relbase, exec.a_trsize) == -1)
	die("write(rel)");

  /* Add "_endmo" to the name list and string table */
    *nl = nlconst;
    nl->n_un.n_strx = str_tbl->len;
    nl->n_type	    = N_EXT | N_TEXT;
    nl->n_value	    = pc;
    nl++;
    strcpy(str_ptr, END_SYMBOL);
    str_tbl->len += strlen(END_SYMBOL)+1;

  /* write out the symbol table. */
    if (write(1, nlbase, exec.a_syms) == -1)
	die("write(nl)");

  /* write out the string table */
    if (write(1, str_tbl, str_tbl->len) == -1)
	die("write(str_tbl)");
    
  /* update the header to reflect the true text size */
    if (lseek(1, 0, 0L) == -1)
	die("lseek");
    if (write(1, &exec, sizeof exec) == -1)
	die("write(exec)");

    return 0;
}
