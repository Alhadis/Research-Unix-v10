extern int sony_inq(ClientData , Tcl_Interp *, int , char **);
extern int sony_alt(ClientData , Tcl_Interp *, int , char **);
extern int sony_conf(ClientData , Tcl_Interp *, int , char **);
extern int sony_status(ClientData , Tcl_Interp *, int , char **);
extern int sony_set(ClientData , Tcl_Interp *, int , char **);
extern int sony_rel(ClientData , Tcl_Interp *, int , char **);
extern int sony_eject(ClientData , Tcl_Interp *, int , char **);
extern int sony_diskid(ClientData , Tcl_Interp *, int , char **);
extern int sony_internal(ClientData , Tcl_Interp *, int , char **);
extern int sony_media(ClientData , Tcl_Interp *, int , char **);
extern int sony_readid(ClientData , Tcl_Interp *, int , char **);
extern int sony_copy(ClientData , Tcl_Interp *, int , char **);
extern int sony_sense(ClientData , Tcl_Interp *, int , char **);
extern int sony_status(ClientData , Tcl_Interp *, int , char **);
extern void sony_extsense(uchar *, char *, int);

extern int shelfside(char *arg, char *err);
extern int sony_istatus(struct scsi_return *, char *);
