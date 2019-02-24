
















































typedef	unsigned char	 u_char;
typedef	unsigned short	 u_short;
typedef	unsigned int	 u_int;
typedef	unsigned long	 u_long;
typedef	unsigned short	 ushort;		



typedef	struct	 _physadr { int  r[1]; } * physadr;




typedef	struct	 label_t	{
	int	val[12];
}  label_t;

typedef	struct	 _quad { long val[2]; }  quad;
typedef	long	 daddr_t;
typedef	char *	 caddr_t;
typedef	 u_long	 ino_t;
typedef	long	 swblk_t;
typedef	long	 size_t;
typedef	long	 time_t;
typedef	short	 dev_t;
typedef	long	 off_t;
typedef	 u_short	 uid_t;
typedef	 u_short	 gid_t;












typedef long	 fd_mask;





typedef	struct  fd_set {
	 fd_mask	fds_bits[	(((256)+(( (sizeof( fd_mask) * 8		)	)-1))/( (sizeof( fd_mask) * 8		)	))];
}  fd_set;












































extern	struct	 _iobuf {
	int	_cnt;

	unsigned char	*_ptr;
	unsigned char	*_base;

	int	_bufsiz;
	short	_flag;
	char	_file;		

	char	*_name;		

}  _iob[];






































struct  _iobuf	* fopen();
struct  _iobuf	* fdopen();
struct  _iobuf	* freopen();
long		 ftell();
char		* fgets();

struct  _iobuf	* popen();
char		* gets();

























































extern	char	 _ctype_[];








































































































	




























int	(* signal())();





struct	 sigvec {
	int	(*sv_handler)();	
	int	sv_mask;		
	int	sv_flags;		
};







struct	 sigstack {
	char	*ss_sp;			
	int	ss_onstack;		
};











struct  sigcontext {
	



	int	sc_onstack;		
	int	sc_mask;		
	int	sc_pc;			
	


	int	sc_regs[32];	
	int	sc_mdlo;	
	int	sc_mdhi;	
	


	int	sc_ownedfp;	
	int	sc_fpregs[32];	
	int	sc_fpc_csr;	
	int	sc_fpc_eir;	
	



	


	int	sc_cause;	
	int	sc_badvaddr;	
	int	sc_badpaddr;	
};


































































































struct	 stat
{
	 dev_t	st_dev;
	 ino_t	st_ino;
	unsigned 	short st_mode;
	short		st_nlink;
	 uid_t	st_uid;
	 gid_t	st_gid;
	 dev_t	st_rdev;
	 off_t	st_size;
	 time_t	st_atime;
	int		st_spare1;
	 time_t	st_mtime;
	int		st_spare2;
	 time_t	st_ctime;
	int		st_spare3;
	long		st_blksize;
	long		st_blocks;
	long		st_spare4[2];
};






































































































































































extern char *strcat();
extern char *strncat();
extern char *strcpy();
extern char *strncpy();
extern int strlen();
extern int strcmp();
extern int strncmp();
extern char *strchr();
extern char *strrchr();


typedef struct string {
	char *base;	
	char *end;	
	char *ptr;	
} string;










extern string *s_new();
extern void s_free();
extern string *s_append();
extern string *s_nappend();
extern string *s_array();
extern string *s_copy();
extern string *s_seq_read();
extern char *s_read_line();
extern int s_read_to_eof();
extern string *s_parse();















typedef struct namelist namelist;
struct namelist {
	namelist *next;
	char *name;
};


































































































































































































































































































































































































































































































































struct	 direct {
	 u_long	d_fileno;		
	 u_short	d_reclen;		
	 u_short	d_namlen;		
	char	d_name[255 + 1];	
};

















typedef struct  _dirdesc {
	int	dd_fd;
	long	dd_loc;
	long	dd_size;
	long	dd_bbase;
	long	dd_entno;
	long	dd_bsize;
	char	*dd_buf;
}  DIR;


extern	 DIR * opendir();
extern	struct  direct * readdir();
extern	long  telldir();
extern	void  seekdir();

extern	void  closedir();





























typedef int (*SIG_TYP)();















extern char *UPASROOT;
int warn = -1;
int remove = -1;
int cleanup;
int verbose;
int testmode;
int Xonly;
int Conly;
string *replyaddr;
string *dest;
int mypid;
char **getcmd();



struct {
	string	*dest;
	time_t	time;
} destlist[50		];		
int	ndest = 0;

int	debug;





















usage()
{
	fprintf((& _iob[2]), "smtpsched [-cvt] [-w #days] [-r #days] [-s #scheds] [dir]\n");
	exit(1);
}

main(ac, av)
	int ac;
	char *av[];
{
	direct *dirp;
	direct d;
	direct *dp;
	int max=0;
	int c;
	extern int optind;
	extern char *optarg;

	mypid = getpid();

	


	signal(13	, (int (*)())1);
	signal(1	, (int (*)())1);

	while ((c = getopt(ac, av, "XCcvtr:w:s:d")) != (-1))
		switch (c) {
		case 'X':
			Xonly = 1;
			break;
		case 'C':
			Conly = 1;
			break;
		case 't':
			testmode = 1;
			break;
		case 'v':
			verbose = 1;
			break;
		case 'c':
			cleanup = 1;
			break;
		case 'r':
			remove = atoi(optarg);
			break;
		case 's':
			max = atoi(optarg);
			break;
		case 'w':
			warn = atoi(optarg);
			break;
		case 'd':
			debug = 1;
			break;
		default:
			usage();
		}
	openlog();

	


	if(chdir("/usr/spool/smtpq")<0){
		log("can't chdir to %s\n", "/usr/spool/smtpq");
		exit(1);
	}

	


	if(max && toomany(max)<0)
		exit(0);

	



	if(optind!=ac){
		for(; optind<ac; optind++)
			while(dodir(av[optind], 1		) && !warn && !remove)
				;
		return 0;
	}

	





	dirp = opendir(".");
	if(dirp<0){
		log("couldn't read %s\n", "/usr/spool/smtpq");
		exit(1);
	}
	while(dp = readdir(dirp)){
		d = *dp;	
		if(strcmp(d.d_name, ".")!=0 && strcmp(d.d_name, "..")!=0)
			dodir(d.d_name, (warn>=0 || remove>=0) ? 2	 : 1		);
	}
	closedir(dirp);
	return 0;
}




dodir(dname, action)
	char *dname;
{
	int i;

	i = dodirdir(dname, action, "X.");
	i += dodirdir(dname, action, "C.");
	return i;
}





dodirdir(dname, action, direction)
	char *dname;
	char *direction;
{
	DIR *dirp;
	direct *dp;
	int i;
	int changed=0;
	int ents=0;
	static string *ds;
	static string *ls;
	extern int errno;

	ds =  (ds ? (*(ds->ptr = ds->base) = '\0' , ds) : s_new());
	s_append(ds, dname);
	ls =  (ls ? (*(ls->ptr = ls->base) = '\0' , ls) : s_new());
	s_append(ls, direction);
	s_append(ls, dname);

	


	switch(action){
	case 0		:
		for(i=0; i<3; i++){
			if(lock( ls->base)==0)
				break;
			sleep(5);
		}
		if(i==3)
			return changed;
		break;
	case 1		:
		if(lock( ls->base)<0){
			log("couldn't lock %s\n", dname);
			return changed;
		}
		break;
	case 2	:
		break;
	}

	



	if(chdir( ds->base)<0){
		if(action != 2	)
			unlock( ls->base);
		return changed;
	}

	


	dirp = opendir(".");
	if(dirp<0){
		log("couldn't read directory %s\n", dname);
		if(chdir("..")<0){
			log("can't chdir ..\n");
			fflush((& _iob[1]));
			exit(1);
		}
		if(action != 2	)
			unlock( ls->base);
		return changed;
	}
	while(dp = readdir(dirp)){
		if(strcmp(dp->d_name, ".")!=0 && strcmp(dp->d_name, "..")!=0
		&& dp->d_name[0] == *direction) {
			switch(dofile(dname, dp->d_name)){
			case 0:
				
				changed = 1;
				break;
			case 1:
				
				ents += 1;
				break;
			}
		}
	}
	closedir(dirp);

	


	if(chdir("..")<0){
		log("can't chdir ..\n");
		fflush((& _iob[1]));
		exit(1);
	}

	


	if(cleanup && ents==0){
		log("%s empty\n",  ds->base);
		if(rmdir( ds->base)<0)
			log("can't unlink: %d\n", errno);
	}

	if(action != 2	)
		unlock( ls->base);
	return changed;
}







dofile(dname, name)
	char *dname;
	char *name;
{
	int rv;
	int fd, ofd;
	char *ef;
	extern char *fileoftype();
	struct stat sb;
	time_t now, Edate, Cdate;

	rv = -1;

	


	if(cleanup && inconsistent(name)){
		log("%s/%s inconsistent\n", dname, name);
		unlink(name);
		return 0;
	}

	


	if(name[1]!='.' || (name[0]!='C' && name[0]!='X'))
		return 1;

	



	if(warn>=0 || remove>=0) {
		if(checkage(name)==0) {
			log("%s/%s too old\n", dname, name);
			doremove(name);
			return 0;
		}
		return 1;
	}

	


	if(cleanup)
		return 1;

	


	ef = fileoftype('E', name);
	if (name[0]=='C') {
		now = time((time_t *)0);
		Cdate = now;
		Edate = now-4*3600L		-1;
		if (stat(name, &sb)==0)
			Cdate = sb.st_ctime;
		if (stat(ef, &sb)==0)
			Edate = sb.st_mtime;
		if (now-Cdate>6*3600L		 && now-Edate<4*3600L		
		 || now-Cdate>1*3600L		  && now-Edate<1*3600L		) {
			if (verbose)
				log("ignore %s/%s: not time yet\n", dname, name);
			if (debug==0)
				return 1;
		}
	}

	


	if (testmode) {
		log("would process %s/%s\n", dname, name);
		return 1;
	}
	


	ofd = dup(2);
	close(2);
	fd = open(ef, 1);
	if(fd<0)
		fd = creat(ef, 0666);
	if(fd>=0){
		lseek(fd, 0l, 2);
	
		


		if(name[0]=='C') {
			rv = dosmtp(dname, name);
		} else if(name[0]=='X') {
			rv = dormail(dname, name);
		}

		


		close(2);
		(void) dup(ofd);
		close(ofd);
	}

	


	if(rv==0) {
		doremove(name);
		return 0;
	}
	return 1;
}




doremove(ctl)
	char *ctl;
{
	fflush((& _iob[1]));
	unlink(fileoftype('E', ctl));
	unlink(ctl);
	unlink(fileoftype('D', ctl));
}





dormail(dname, ctl)
	char *dname;
	char *ctl;
{
	char **av;

	log("dormail %s/%s\n", dname, ctl);

	


	av = getcmd(ctl, "/bin/rmail");
	if(av && docmd(ctl, av)==0){
		log("success\n");
		return 0;
	} else {
		log("failure\n");
		return -1;
	}
}





dosmtp(dname, ctl)
	char *dname;
	char *ctl;
{
	static string *cmd;
	int status, i;
	char **av;
	time_t t0, t1;

	log("dosmtp %s/%s\n", dname, ctl);

	


	cmd =  (cmd ? (*(cmd->ptr = cmd->base) = '\0' , cmd) : s_new());
	s_append(cmd, UPASROOT);
	s_append(cmd, "/smtp");
	av = getcmd(ctl,  cmd->base);
	if (av==0) {
		log("cmdfail\n", 0);
		return -1;
	}
	



	for (i=0; i<ndest; i++) {
		if (strcmp( dest->base,  destlist[i].dest->base)==0) {
			if (destlist[i].time > 5*60		) {
				log("passed %s (%d sec)\n",  dest->base, destlist[i].time);
				fprintf((& _iob[2]), "can't contact destination\n");
				return -1;
			}
			break;
		}
	}
	if (i==ndest) {
		if (ndest<50		)
			ndest++;
		else
			i = 0;		
		destlist[i].dest = s_copy( dest->base);
		destlist[i].time = 0;
	}
	if (debug)
		log("time %d for %s\n", destlist[i].time,  dest->base);
	time(&t0);
	switch(status=docmd(ctl, av)){
	case 0:		
		log("success\n");
		destlist[i].time = 0;
		return 0;

	case 69	:	
	case 	77	:		
	case 	67	:		
	case 	68	:		
		log("fail %d\n", status);
		returnmail(ctl, 1);		
		destlist[i].time = 0;
		return 0;

	case 73	:	
	case 	74	:		
	case 	71	:		
	case 	72	:		
	case 70	:	
	case 	64	:		
	case 	65	:	
	case 	66	:	
	case 76	:	
	case 75	:	
		log("fail %d\n", status);
		time(&t1);			
		destlist[i].time += t1-t0;
		return -1;

	default:	
		log("fail %d\n", status);
		time(&t1);
		destlist[i].time += t1-t0;
		return -1;
	}
}







struct  _iobuf *
parseline1(ctl)
	char *ctl;
{
	struct  _iobuf *fp;
	static string *line;

	fp = fopen(ctl, "r");
	if(fp==0)
		return 0;

	


	line =  (line ? (*(line->ptr = line->base) = '\0' , line) : s_new());
	if(s_read_line(fp, line)==0){
		fprintf((& _iob[2]), "smtpsched: error reading ctl file %s\n", ctl);
		fclose(fp);
		return 0;
	}
	replyaddr = s_parse( (line->ptr = line->base , line),  (replyaddr ? (*(replyaddr->ptr = replyaddr->base) = '\0' , replyaddr) : s_new()));
	dest = s_parse(line,  (dest ? (*(dest->ptr = dest->base) = '\0' , dest) : s_new()));
	return fp;
}








char **
getcmd(ctl, cmd)
	char *cmd;
{
	static string *args;
	struct  _iobuf *fp;
	static char *av[1024];
	int ac=0;
	char *cp;

	fp = parseline1(ctl);
	if (fp==0)
		return 0;

	


	av[ac++] = cmd;
	args =  (args ? (*(args->ptr = args->base) = '\0' , args) : s_new());
	if(s_read_line(fp, args)==0){
		fprintf((& _iob[2]), "smtpsched: error reading ctl file %s\n", ctl);
		fclose(fp);
		return 0;
	}
	fclose(fp);
	for(cp =  args->base; *cp && ac<1023;){
		av[ac++] = cp++;
		while(*cp && !	(( _ctype_+1)[*cp]&010))
			cp++;
		while(	(( _ctype_+1)[*cp]&010))
			*cp++ = 0;
	}
	av[ac] = 0;
	return av;
}




docmd(ctl, av)
	char *ctl;
	char **av;
{
	int fd;
	int pid, status;
	int n;

	


	switch(pid = fork()){
	case -1:
		return 1;
	case 0:
		


		close(0);
		fd = open(fileoftype('D', ctl), 0);
		if(fd<0){
			perror("smtpsched: error reading data file:\n");
			exit(1);
		}
	
		


		close(1);
		fd = dup(2);
	
		


		execvp(av[0], av);
		exit(1);
	default:
		


		while((n = wait(&status))>=0)
			if(n == pid)
				break;
		if(status&0xff)
			return -1;
		else
			return (status>>8)&0xff;
	}

}







toomany(max)
	int max;
{
	struct  _iobuf *ifp=0;
	struct  _iobuf *ofp=0;
	int cur=0;
	int pid;

	


	if(lock(".smtpscheds")<0)
		return -1;

	


	ofp = fopen(".nsmtpscheds", "w");
	if(ofp==0){
		fprintf((& _iob[2]), "can't open %s\n", ".nsmtpscheds");
		goto error;
	}
	ifp = fopen(".smtpscheds", "r");
	if(ifp!=0){
		


		while(fscanf(ifp, "%d", &pid)==1){
			if(kill(pid, 0) == 0){
				cur++;
				if(fprintf(ofp, "%d\n", pid)<0){
					fprintf((& _iob[2]), "error writing %s\n", ".nsmtpscheds");
					goto error;
				}
			}
		}
		if(cur >= max)
			goto error;
	}

	


	if(fprintf(ofp, "%d\n", getpid())<0){
		fprintf((& _iob[2]), "error writing %s\n", ".nsmtpscheds");
		goto error;
	}
	if(ifp!=0)
		fclose(ifp);
	if(fclose(ofp)==(-1))
		goto error;
	unlink(".smtpscheds");
	if(link(".nsmtpscheds", ".smtpscheds")<0)
		fprintf((& _iob[2]), "can't link %s to %s file\n", ".smtpscheds", ".nsmtpscheds");
	unlink(".nsmtpscheds");
	unlock(".smtpscheds");
	return 0;
error:
	


	if(ifp!=0)
		fclose(ifp);
	if(ofp!=0)
		fclose(ofp);
	unlink(".nsmtpscheds");
	unlock(".smtpscheds");
	return -1;
}








inconsistent(file)
	char *file;
{
	struct stat s;
	int days;

	


	switch(file[0]){
	case 'C':
	case 'X':
		


		if(stat(fileoftype('D', file), &s)<0)
			return 1;
		break;
	case 'E':
		


		if(stat(fileoftype('X', file), &s)<0
		&& stat(fileoftype('C', file), &s)<0)
			return 1;

		


		if(stat(fileoftype('D', file), &s)<0)
			return 1;
		break;
	case 'D':
		


		if(stat(fileoftype('X', file), &s)==0
		|| stat(fileoftype('C', file), &s)==0)
			break;

		


		if(stat(file, &s)<0)
			return 0;
		days = (time((long *)0) - s.st_ctime)/(24*60*60);
		if(days>0)
			return 1;
		break;
	default:
		break;
	}
	return 0;
}





checkage(ctl)
	char *ctl;
{
	struct stat s;
	int days;
	char buf[256];
	struct  _iobuf *fp;

	


	if(stat(ctl, &s)<0)
		return -1;
	days = (time((long *)0) - s.st_ctime)/(24*60*60);

	


	if(remove>=0 && days>=remove){
		fp = parseline1(ctl);
		if(fp==0)
			return -1;
		fclose(fp);

		log("r %d %d\n", days, remove);
		return returnmail(ctl, 1);
	}

	


	if(warn>=0 && days>=warn){
		fp = parseline1(ctl);
		if(fp==0)
			return -1;
		fclose(fp);

		log("w %d %d\n", days, warn);
		returnmail(ctl, 0);
	}

	return -1;
}




returnmail(ctl, warn)
	char *ctl;
{
	int pid, status;
	string *cmd;
	int pfd[2];
	char buf[132];
	int fd, n;
	int reads;
	struct  _iobuf *fp;
	struct  _iobuf *ifp;
	long now;
	char asct[27];

	log("returnmail %s to %s about %s\n", ctl,  replyaddr->base,
		 dest->base);

	if(pipe(pfd)<0)
		return -1;

	switch(pid=fork()){
	case -1:
		close(pfd[0]);
		close(pfd[1]);
		return -1;
	case 0:
		


		close(0);
		dup(pfd[0]);
		close(pfd[1]);
		execl("/bin/rmail", "/bin/rmail",   replyaddr->base, 0);
		exit(1);
	default:
		


		close(pfd[0]);
		fp = fdopen(pfd[1], "w");
		if(fp==0) {
			close(pfd[1]);
			break;
		}

		


		now = time((long *)0);
		strcpy(asct, ctime(&now));
		asct[24] = 0;
		fprintf(fp, "From postmaster %s remote from \n", asct);

		


		if(warn) {
			fprintf(fp, "Subject: smtp mail failed\n\n");
			fprintf(fp, "Your mail to %s is undeliverable.\n",
				 dest->base);
		} else {
			fprintf(fp, "Subject: smtp mail warning\n\n");
			fprintf(fp, "Your mail to %s is not yet delivered.\n",
				 dest->base);
			fprintf(fp, "Delivery attempts continue.\n");
		}

		


		fprintf(fp, "---------- diagnosis ----------\n");
		ifp = fopen(fileoftype('E', ctl), "r");
		if(ifp!=0){
			for(reads=0; reads<20; reads++) {
				if(fgets(buf, sizeof(buf), ifp)==0)
					break;
				fputs(buf, fp);
			}
			fclose(ifp);
		}

		


		fprintf(fp, "---------- unsent mail ----------\n");
		ifp = fopen(fileoftype('D', ctl), "r");
		if(ifp!=0){
			for(reads=0; reads<50; reads++) {
				if(fgets(buf, sizeof(buf), ifp)==0)
					break;
				fputs(buf, fp);
			}
			fclose(ifp);
		}
		fclose(fp);

		


		while((n = wait(&status))>=0)
			if(n == pid)
				break;
		return status ? -1 : 0;
	}
	close(pfd[1]);
	return -1;
}

openlog()
{
	string *f;

	if (debug)
		return;
	f = s_new();
	s_append(f, "/usr/spool/smtpq");
	s_append(f, "/smtpsched.log");
	close(0);
	if(open("/dev/null", 0)==0){
		close(1);
		close(2);
		close(3);
		if(open( f->base, 2)==1
		|| creat( f->base, 0666)==1){
			lseek(1, 0L, 2);
			dup(1);
			dup(1);
		}
	}
	s_free(f);
}

log(f, a, b, c)
	char *f;
{
	char *dp;
	struct tm *bp;
	long thetime;
	extern struct tm *localtime();
	extern char *asctime();
	extern long time();

	thetime = time((long *)0);
	bp = localtime(&thetime);
	dp = asctime(bp);

	lseek(1, 0L, 2);
	printf("%d %.12s: ", mypid, dp+4);
	printf(f, a, b, c);
	fflush((& _iob[1]));
}
