46a47
> #undef Void
53c54
< extern int c_sfe();
---
> extern int c_sfe(), err_fl(), xrd_SL();
71a73,74
> extern int err__fl(int,int,char*);
> extern int xrd_SL(void);
75a79
> extern int (*f__doend)(Void);
79a84
> #define errfl(f,m,s) return err__fl(f,m,s)
98,197d102
< #ifdef __cplusplus
< extern "C" {
< extern long int f_back(alist *);
< extern long int f_clos(cllist *);
< extern void f_exit(void);
< extern void flush_(void);
< extern int y_rsk(void);
< extern int y_getc(void);
< extern int y_putc(int);
< extern int y_rev(void);
< extern int y_err(void);
< extern int y_newrec(void);
< extern int c_dfe(cilist *);
< extern long int s_rdfe(cilist *);
< extern long int s_wdfe(cilist *);
< extern long int e_rdfe(void);
< extern long int e_wdfe(void);
< extern long int do_lio(long int *, long int *, char *, long int);
< extern int c_due(cilist *);
< extern long int s_rdue(cilist *);
< extern long int s_wdue(cilist *);
< extern long int e_rdue(void);
< extern long int e_wdue(void);
< extern long int f_end(alist *);
< extern int f__canseek(struct _iobuf *);
< extern char *ap_end(char *);
< extern int op_gen(int, int, int, int);
< extern char *gt_num(char *, int *);
< extern char *f_s(char *, int);
< extern int ne_d(char *, char **);
< extern int e_d(char *, char **);
< extern char *i_tem(char *);
< extern char *f_list(char *);
< extern int pars_f(char *);
< extern int type_f(int);
< extern long int do_fio(long int *, char *, long int);
< extern void fmt_bg(void);
< extern char *f__icvt(long int, int *, int *, int);
< extern int z_getc(void);
< extern int z_putc(int);
< extern int c_si(icilist *);
< extern int y_ierr(void);
< extern long int s_rsfi(icilist *);
< extern int z_wnew(void);
< extern long int s_wsfi(icilist *);
< extern long int e_rsfi(void);
< extern long int e_wsfi(void);
< extern void c_liw(icilist *);
< extern int s_wsni(icilist *);
< extern long int s_wsli(icilist *);
< extern long int e_wsli(void);
< extern long int f_inqu(inlist *);
< extern int t_getc(void);
< extern long int e_rsle(void);
< extern int l_R(int);
< extern int l_C(void);
< extern int l_L(void);
< extern int l_CHAR(void);
< extern int c_le(cilist *);
< extern int l_read(long int *, char *, long int, long int);
< extern long int s_rsle(cilist *);
< extern char *mktemp(char *);
< extern int f__isdev(char *);
< extern long int f_open(olist *);
< extern long int f_rew(alist *);
< extern int xrd_SL(void);
< extern int x_getc(void);
< extern int x_endp(void);
< extern int x_rev(void);
< extern long int s_rsfe(cilist *);
< extern int i_getc(void);
< extern int i_ungetc(int, struct _iobuf *);
< extern long int s_rsli(icilist *);
< extern long int e_rsli(void);
< extern int s_rsni(icilist *);
< extern int x_rsne(cilist *);
< extern long int s_rsne(cilist *);
< extern long int e_rsfe(void);
< extern long int e_wsfe(void);
< extern int c_sue(cilist *);
< extern long int s_rsue(cilist *);
< extern long int s_wsue(cilist *);
< extern long int e_wsue(void);
< extern long int e_rsue(void);
< extern int do_us(long int *, char *, long int);
< extern long int do_ud(long int *, char *, long int);
< extern long int do_uio(long int *, char *, long int);
< extern long int f__inode(char *, int *);
< extern void f__mvgbt(int, int, char *, char *);
< extern int mv_cur(void);
< extern int x_putc(int);
< extern int xw_end(void);
< extern int xw_rev(void);
< extern long int s_wsfe(cilist *);
< extern long int s_wsle(cilist *);
< extern long int e_wsle(void);
< extern long int s_wsne(cilist *);
< extern void x_wsne(cilist *);
< 	}
< #endif
