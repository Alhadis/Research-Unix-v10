#include	<stdio.h>

/*
 *  fget -- spooler to get file from GCOS
 */


#define	DPR	0
#define	FGET	1
#define	FSEND	0
#define	GCAT	0
#define	LPR	0

#define	NAME	"fget"
#define	MAXCOPY	204800L
#define FIRSTCHAR 't'-1
#define CARDSIZE 130

char	cfname[]	= "/usr/spool/dpd/cfsXXXXX";
char	dfname[]	= "/usr/spool/dpd/dfsXXXXX";
char	lfname[]	= "/usr/spool/dpd/lfsXXXXX";
char	tfname[]	= "/usr/spool/dpd/tfsXXXXX";
char	zfname[]	= "/usr/spool/dpd/zfsXXXXX";

char	*fzf;
FILE	*pfdes;
char	wdir[BUFSIZ];
FILE	*uff;
char	fname[CARDSIZE]  /* unix file name */;
char	*fmode;

#include	"spool.c"
#include	"gcos.c"

main(agc, agv)
int agc;
char *agv[];
{
	extern FILE *popen();
	int fi, i, j, nslash, c;
	int time;
	int poff, foff;

	argc = agc;    
	argv = agv;
	pidfn();
	remotsw = TOSS;		/*default is toss online output*/
	wantmail = 1;		/*default is send mail.*/
	grade = '1';
	fmode = "a";
	fi = 0;
	foff = 0;
	poff = 0;

	while (argc>1 && (arg = argv[1])[0]=='-') {
		if(!comopt(arg[1]))
			switch (arg[1]) {

			case 'a':
				fmode = "a";
				break;

			case 'b':
				fmode = "b";
				break;

			case 'd':
				arg = getarg('d');
				while((fname[fi] = arg[fi])) ++fi;
				fname[fi++] = '/';
				break;

			case 'u':
				arg = getarg('u');
				i = 0;
				while((gcosid[i] = arg[i]))
					if(++i > 12) {
						gcosid[i] = '\0';
						break;
					}
				uidf = 1;
				break;

			default:
				fprintf(stderr, "%s: Unrecognized option: %s\n", NAME, arg);
				break;
			}
		argc--;
		argv++;
	}

	spool1();		/*setup initial GCOS cards.*/
	card('L',"$	select	cc/unixsupport/access");
	if(gcdebug)
		card('L',"$	select	cc/test/fget");
	else
		card('L',"$	select	cc/unixsupport/fget");
	card('L',fget_id);
	while(--argc) {
		arg = *++argv;
		if(mailfile == 0)
			mailfile = arg;
		/* extract a unix file name */

		i = 0;
		foff = fi;
		nslash = 0;
		while((c = argv[0][i++]) != '\0') {
			if(c == '/') { 
				nslash++; 
				foff = fi; 
				continue; 
			}
			fname[foff++] = c;
		}
		fname[foff++] = '\0';
		fname[fi+12] = '\0';

		/* create the ultimate output file */

		if(debug == 0){
			if((uff = fopen(fname, "w")) == NULL) {
				fprintf(stderr, "%s: Cannot create %s\n", NAME,fname);
				continue;
			}
			chmod(fname,0666);	/*insure file writeable by daemon. MRW*/
		}

		/* now make a link for the dpd */

		if(zfname[INCHAR] < 'z')    /*don't create file names past 'z'. MRW*/
			zfname[INCHAR]++;
		fzf = zfname;
		if(link(fname, fzf) != 0){
			if( fi > 0 ) { 
				fzf = fname; 
			}

			else{
				if(poff == 0){
					if((pfdes = popen("pwd", "r")) != NULL){
						int c;
						while ((c = getc(pfdes)) != EOF)
							wdir[poff++] = c;
						pclose(pfdes);
						wdir[poff-1] = '/';
					}
				}
				if(wdir[0] != '/') { 
					fprintf(stderr, "%s: Cannot find directory pathname\n", NAME);
					goto cleanup; 
				}
				i = poff; 
				j = 0;
				while ( wdir[i++] = fname[j++]);
				if(link(wdir, fzf) != 0) fzf = wdir;
			}
		}

		card('L', sprintf(buf, "%s %s%s%s %s",
				fmode,
				(nslash == 0 || *arg == '/') ? gcosid : "",
				(nslash == 0) ? "/" : "",
				arg,
				fzf ) );

		if(debug == 0){
			i = 2;
			while((putc(buf[i++], uff)) != '\0') ;
			c = putc( '\n', uff );
				if(c == EOF){
					fprintf(stderr, "%s: Error writing %s\n", NAME, fname);
					break;
				}
			fclose(uff);
		}
		nact++;

	}
	if(fork() != 0){
		spool2();		/*put out final cards, and spool job.*/
		out();
	}

	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	dfname[INCHAR]++;
	for( time = 4 ; time < 200 ; time += time ) {
		sleep(time);
		if(access(dfname, 0) != 0) break;
	}

	execl("/usr/lib/fget.demon", "fget.demon", 0);
	execl("/etc/fget.demon", "fget.demon", 0);
	fprintf(stderr, "%s: Execl of fget.demon failed.\n", NAME);
	exit(1);

cleanup:
	out();
}
