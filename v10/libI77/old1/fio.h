#include "stdio.h"
#include "errno.h"
#ifndef NULL
/* ANSI C */
#include "stddef.h"
#endif

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#ifdef MSDOS
#ifndef NON_UNIX_STDIO
#define NON_UNIX_STDIO
#endif
#endif

#ifdef UIOLEN_int
typedef int uiolen;
#else
typedef long uiolen;
#endif

/*units*/
typedef struct
{	FILE *ufd;	/*0=unconnected*/
	char *ufnm;
#ifndef MSDOS
	long uinode;
	int udev;
#endif
	int url;	/*0=sequential*/
	flag useek;	/*true=can backspace, use dir, ...*/
	flag ufmt;
	flag uprnt;
	flag ublnk;
	flag uend;
	flag uwrt;	/*last io was write*/
	flag uscrtch;
} unit;

extern flag f__init;
extern cilist *f__elist;	/*active external io list*/
extern flag f__reading,f__external,f__sequential,f__formatted;
#ifdef KR_headers
#define Void /*void*/
extern int (*f__getn)(),(*f__putn)();	/*for formatted io*/
extern long f__inode();
extern VOID sig_die();
extern int (*f__donewrec)(), t_putc(), x_wSL();
extern int c_sfe();
#else
#define Void void
#ifdef __cplusplus
extern "C" {
#endif
extern int (*f__getn)(void),(*f__putn)(int);	/*for formatted io*/
extern long f__inode(char*,int*);
extern void sig_die(char*,int);
extern void f__fatal(int,char*);
extern int t_runc(alist*);
extern int f__nowreading(unit*), f__nowwriting(unit*);
extern int fk_open(int,int,ftnint);
extern int en_fio(void);
extern void f_init(void);
extern int (*f__donewrec)(void), t_putc(int), x_wSL(void);
extern void b_char(char*,char*,ftnlen), g_char(char*,ftnlen,char*);
extern int c_sfe(cilist*), z_rnew(void);
extern int isatty(int);
#ifdef __cplusplus
	}
#endif
#endif
extern FILE *f__cf;	/*current file*/
extern unit *f__curunit;	/*current unit*/
extern unit f__units[];
#define err(f,m,s) {if(f) errno= m; else f__fatal(m,s); return(m);}

/*Table sizes*/
#define MXUNIT 100

extern int f__recpos;	/*position in current record*/
extern int f__cursor;	/* offset to move to */
extern int f__hiwater;	/* so TL doesn't confuse us */

#define WRITE	1
#define READ	2
#define SEQ	3
#define DIR	4
#define FMT	5
#define UNF	6
#define EXT	7
#define INT	8

#define buf_end(x) (x->_flag & _IONBF ? x->_ptr : x->_base + BUFSIZ)
#ifdef __cplusplus
extern "C" {
extern long int f_back(alist *);
extern long int f_clos(cllist *);
extern void f_exit(void);
extern void flush_(void);
extern int y_rsk(void);
extern int y_getc(void);
extern int y_putc(int);
extern int y_rev(void);
extern int y_err(void);
extern int y_newrec(void);
extern int c_dfe(cilist *);
extern long int s_rdfe(cilist *);
extern long int s_wdfe(cilist *);
extern long int e_rdfe(void);
extern long int e_wdfe(void);
extern long int do_lio(long int *, long int *, char *, long int);
extern int c_due(cilist *);
extern long int s_rdue(cilist *);
extern long int s_wdue(cilist *);
extern long int e_rdue(void);
extern long int e_wdue(void);
extern long int f_end(alist *);
extern int f__canseek(struct _iobuf *);
extern char *ap_end(char *);
extern int op_gen(int, int, int, int);
extern char *gt_num(char *, int *);
extern char *f_s(char *, int);
extern int ne_d(char *, char **);
extern int e_d(char *, char **);
extern char *i_tem(char *);
extern char *f_list(char *);
extern int pars_f(char *);
extern int type_f(int);
extern long int do_fio(long int *, char *, long int);
extern void fmt_bg(void);
extern char *f__icvt(long int, int *, int *, int);
extern int z_getc(void);
extern int z_putc(int);
extern int c_si(icilist *);
extern int y_ierr(void);
extern long int s_rsfi(icilist *);
extern int z_wnew(void);
extern long int s_wsfi(icilist *);
extern long int e_rsfi(void);
extern long int e_wsfi(void);
extern void c_liw(icilist *);
extern int s_wsni(icilist *);
extern long int s_wsli(icilist *);
extern long int e_wsli(void);
extern long int f_inqu(inlist *);
extern int t_getc(void);
extern long int e_rsle(void);
extern int l_R(int);
extern int l_C(void);
extern int l_L(void);
extern int l_CHAR(void);
extern int c_le(cilist *);
extern int l_read(long int *, char *, long int, long int);
extern long int s_rsle(cilist *);
extern char *mktemp(char *);
extern int f__isdev(char *);
extern long int f_open(olist *);
extern long int f_rew(alist *);
extern int xrd_SL(void);
extern int x_getc(void);
extern int x_endp(void);
extern int x_rev(void);
extern long int s_rsfe(cilist *);
extern int i_getc(void);
extern int i_ungetc(int, struct _iobuf *);
extern long int s_rsli(icilist *);
extern long int e_rsli(void);
extern int s_rsni(icilist *);
extern int x_rsne(cilist *);
extern long int s_rsne(cilist *);
extern long int e_rsfe(void);
extern long int e_wsfe(void);
extern int c_sue(cilist *);
extern long int s_rsue(cilist *);
extern long int s_wsue(cilist *);
extern long int e_wsue(void);
extern long int e_rsue(void);
extern int do_us(long int *, char *, long int);
extern long int do_ud(long int *, char *, long int);
extern long int do_uio(long int *, char *, long int);
extern long int f__inode(char *, int *);
extern void f__mvgbt(int, int, char *, char *);
extern int mv_cur(void);
extern int x_putc(int);
extern int xw_end(void);
extern int xw_rev(void);
extern long int s_wsfe(cilist *);
extern long int s_wsle(cilist *);
extern long int e_wsle(void);
extern long int s_wsne(cilist *);
extern void x_wsne(cilist *);
	}
#endif
