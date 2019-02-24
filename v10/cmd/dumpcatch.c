/*
 *             D U M P  C A T C H
 *
 * This program examines kernel memory through /dev/kmem, and looks at
 * the dump buffers in the dumpld line discipline (see /usr/sys/dev/dumpld.c).
 * The command takes a filename prefix.  For any active dump buffers,
 * an identification code is appended to the filename prefix, and everything
 * placed into the buffer by dumpld is appended to the resulting file.
 * The identification code is 'M.m.ls', where 'M' is the major device number,
 * 'm' is the minor device number, 'l' is the line-discipline number (from 0),
 * and 's' (side) is 'w' for the writer or 'r' for the reader.  If the old
 * version of dumpld is installed in the kernel, the identification code is
 * just the buffer number (from 0).
 *
 *
 * Written by Kurt Gollhardt  (Nirvonics, Inc.)
 * Last update Sun Mar 31 03:32:57 1985
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <nlist.h>
#include <sys/types.h>
#include <sys/dumpl.h>
#include <sys/stream.h>

#define getme(s, kind, off, type)  _get(kind, off, 1, sizeof(type), &s)
#define getarray(s, kind, n, type) _get(kind, 0, n, sizeof(type), s)
#define getspace(s, n, type)  s = (type *)malloc((n) * sizeof(type))

struct nlist nl[] = {
#define _ndumpbuf   0
     {"_ndumpbuf"},
#define _dumpld     1
     {"_dumpld"},
#define _dumpver    2
     {"_dumpver"},
#define _dumpinf   3
     {"_dumpinf"},
     {""}
};

#define MEMFILE     "/dev/kmem"
#define SYMFILE     "/unix"

struct blockdump {
     struct dumpheader	 h;
     char		 buf[1024];
     char		 *fillp;
};

int  mem;
struct dumpld	    *dumpld;
struct dumpinf	    *dumpinf;
struct blockdump    *bdump;
int  ndumpbuf, dumpver;
FILE **file;
char **readp;

char *malloc(), *calloc();


main(ac, av)
     char *av[];
{
     struct dumpld  *di;
     register int   i;

     if (ac != 2) {
          fprintf(stderr, "Usage: %s filename_prefix\n", av[0]);
	  exit(1);
     }

     if (access(SYMFILE, 0) < 0) {
          perror(SYMFILE);
	  exit(2);
     }
     if ((mem = open(MEMFILE, 0)) < 0) {
          perror(MEMFILE);
	  exit(2);
     }

     nlist(SYMFILE, nl);
     if (nl[_ndumpbuf].n_value == 0) {
          fprintf(stderr, "No dumpld line-discipline present\n");
	  exit(5);
     }
     getme(ndumpbuf, _ndumpbuf, 0, int);

     if (nl[_dumpver].n_value == 0)
	  dumpver = 0;
     else
	  getme(dumpver, _dumpver, 0, int);

     getspace(dumpld, ndumpbuf, struct dumpld);
     getspace(readp, ndumpbuf, char *);
     file = (FILE **)calloc(sizeof(FILE *), ndumpbuf);
     if (dumpld == NULL || file == NULL || readp == NULL) {
          fprintf(stderr, "Can't allocate enough memory\n");
	  exit(3);
     }
     if (dumpver > 0) {
	  getspace(dumpinf, ndumpbuf, struct dumpinf);
	  getspace(bdump, ndumpbuf, struct blockdump);
	  if (dumpinf == NULL || bdump== NULL) {
	       fprintf(stderr, "Can't allocate enough memory\n");
	       exit(3);
	  }
	  for (i = 0; i < ndumpbuf; ++i)
	       bdump[i].fillp = (char *)&bdump[i];
     }

     for (;;) {
          getarray(dumpld, _dumpld, ndumpbuf, struct dumpld);
	  if (dumpver > 0)
	       getarray(dumpinf, _dumpinf, ndumpbuf, struct dumpinf);
	  for (di = dumpld; di < &dumpld[ndumpbuf]; di++)
	       if (di->base != NULL) {
	            if (file[di - dumpld] == 0)
		         new_file(di - dumpld, av[1]);
                    if (readp[di - dumpld] != di->fillp)
		         read_buf(di - dumpld, di);
               }
     }
}


new_file(i, prefix)
     char *prefix;
{
     char filename[100];

     if (dumpver == 0)
	  sprintf(filename, "%s%d", prefix, i);
     else
	  sprintf(filename, "%s%d.%d.%d%c", prefix, major(dumpinf[i].dev),
			      minor(dumpinf[i].dev), i/2, (i&1 ? 'w' : 'r'));
     if ((file[i] = fopen(filename, "a")) == NULL) {
          perror(filename);
	  exit(4);
     }

     readp[i] = dumpld[i].base;
}


read_buf(i, di)
     register struct dumpld   *di;
{
     if (di->fillp < readp[i]) {
          dump(readp[i], (di->base + di->size) - readp[i], file[i], i);
	  readp[i] = di->base;
     }
     if (di->fillp > readp[i]) {
          dump(readp[i], di->fillp - readp[i], file[i], i);
	  readp[i] = di->fillp;
     }
     fflush(file[i]);
}


char buf[BUFSIZ];

dump(loc, n, stream, i)
     char *loc;
     FILE *stream;
{
     int  count;

     if (dumpver > 0)
	  return fancy_dump(loc, n, stream, i);

     while (n > 0) {
          count = (n > BUFSIZ ? BUFSIZ : n);
	  getarray(buf, loc, count, char);
	  fwrite(buf, 1, count, stream);
	  n -= count;
	  loc += count;
     }
}


fancy_dump(loc, count, stream, i)
     char      *loc;
     FILE      *stream;
{
     register struct blockdump	   *bd = &bdump[i];
     register int   n, have_block;

     while (count > 0) {
	  have_block = 0;
	  n = bd->buf - bd->fillp;
	  if (n <= 0) {
	       have_block = 1;
	       n += bd->h.count;
	  }
	  if (count < n) {
	       have_block = 0;
	       n = count;
	  }
	  if (n > 0) {
	       getarray(bd->fillp, loc, n, char);
	       loc += n;  bd->fillp += n;
	       count -= n;
	  }
	  if (have_block) {
	       dump_block(bd, stream);
	       bd->fillp = (char *)bd;
	  }
     }
}


#define BYTE_PER_LINE	 16

dump_block(bd, stream)
     struct blockdump	 *bd;
     FILE		 *stream;
{
     print_type(bd->h.type, stream);
     fprintf(stream, ":%s", (bd->h.count == 0 ? "\n" : ""));
     bd->fillp = bd->buf;
     while (bd->h.count > 0) {
	  dump_line(bd, stream);
	  bd->h.count -= BYTE_PER_LINE;
	  bd->fillp += BYTE_PER_LINE;
     }
}


dump_line(bd, stream)
     struct blockdump	 *bd;
     FILE		 *stream;
{
     register int   n, byte;
     register char  *p;
     char ascii[BYTE_PER_LINE+1];

     putc('\t', stream);
     p = bd->fillp;
     for (n = 0; n < BYTE_PER_LINE; n++) {
	  byte = *p++ & 0xFF;
	  if (n < bd->h.count)
	       fprintf(stream, "%02x ", byte);
	  else
	       fprintf(stream, "   ");
	  ascii[n] = (n < bd->h.count && isprint(byte) ? byte : '.');
     }
     ascii[BYTE_PER_LINE] = '\0';
     fprintf(stream, "!%s!\n", ascii);
}


char *mtype_names[] = {
     "DATA",   "BREAK",	 "HANGUP", "DELIM",  "ECHO",   "ACK",	 "IOCTL",
     "DELAY",  "CTL",	 "PASS",   "SIGNAL", "FLUSH",  "STOP",	 "START",
     "IOCACK", "IOCNAK", "CLOSE",  "YDEL",   "NDEL",   "IOCWAIT",NULL
};
int  mtype_codes[] = {
     M_DATA,   M_BREAK,	 M_HANGUP, M_DELIM,  M_ECHO,   M_ACK,	 M_IOCTL,
     M_DELAY,  M_CTL,	 M_PASS,   M_SIGNAL, M_FLUSH,  M_STOP,	 M_START,
     M_IOCACK, M_IOCNAK, M_CLOSE,  M_YDEL,   M_NDEL,   M_IOCWAIT,0
};

print_type(type, stream)
     FILE      *stream;
{
     register int   i;

     for (i = 0; mtype_names[i] != NULL; ++i)
	  if (mtype_codes[i] == type) {
	       fputs(mtype_names[i], stream);
	       return;
	  }

     fprintf(stream, "[%02x]", type);
}


_get(addr, off, n, size, loc)
unsigned long addr;
char *loc;
{
     if (addr < sizeof(nl)/sizeof(nl[0]))
          addr = nl[addr].n_value;
     addr += off*size;
     lseek(mem, addr, 0);
     read(mem, loc, n*size);
     return(addr);
}
