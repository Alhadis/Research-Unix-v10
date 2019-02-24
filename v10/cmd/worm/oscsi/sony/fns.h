extern int sony_inq(int, int *, int, char **, char *);
extern int sony_alt(int, int *, int, char **, char *);
extern int sony_conf(int, int *, int, char **, char *);
extern int sony_status(int, int *, int, char **, char *);
extern int sony_set(int, int *, int, char **, char *);
extern int sony_rel(int, int *, int, char **, char *);
extern int sony_eject(int, int *, int, char **, char *);
extern int sony_diskid(int, int *, int, char **, char *);
extern int sony_internal(int, int *, int, char **, char *);
extern int sony_media(int, int *, int, char **, char *);
extern int sony_readid(int, int *, int, char **, char *);
extern int sony_copy(int, int *, int, char **, char *);
extern int sony_sense(int, int *, int, char **, char *);
extern void sony_extsense(uchar *, char *, int);

extern int shelfside(char *arg, char *err);
