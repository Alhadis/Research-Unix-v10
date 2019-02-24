/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

/* abort.h - three terse message, warning, and abort functions.
	out(s,a,...)	 print message to stdout
	err(s,a,...)	 print warning to stderr
	abort(s,a,...)	 complain to stderr and exit abnormally.
   The 's' argument is a printf string, and the 'a',... are (up to 15) arguments
   matching %-fields.  These routines prefix the program name and append `\n' to
   the printf string, then write to:
	  `out': stdout
	  `err' & `abort': stderr
   Abort also says " - abort", and exits abnormally.
   This #include should be preceded by `#define CMDNAME "xxx"'  so that `xxx:'
   will be prefixed to msg and abort strings. */

#ifndef CMDNAME
#define CMDNAME "hsb"
#endif

#define MSGMAX 120

#ifndef boolean
#define boolean int
#define T 1
#define F 0
#endif

/* WARNING: these function-call versions do not port well:
   e.g. floating-point arguments are botched on MIPS and SGI machines */

out(s,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)
    char *s;
{   static char m[MSGMAX];
	strcpy(m,CMDNAME);
	strcat(m,": ");
	strcat(m,s);
	strcat(m,"\n");
	fprintf(stdout,m,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15);
	}

err(s,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)
    char *s;
{   char m[MSGMAX];
	strcpy(m,CMDNAME);
	strcat(m,": ");
	strcat(m,s);
	strcat(m,"\n");
	fprintf(stderr,m,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15);
	}

abort(s,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)
    char *s;
{   static char m[MSGMAX];
	strcpy(m,s);
	strcat(m," - abort");
	err(m,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15);
	exit(1);
	}

/* These hybrid macro/function forms ports well, but are awkward to use... */

char *_F(s)
   char *s;
{  static char m[MSGMAX];
	strcpy(m,CMDNAME);
	strcat(m,": ");
	strcat(m,s);
	strcat(m,"\n");
	return(m);
	}

#define out0(Z) fprintf(stdout,_F(Z))
#define out1(Z,A) fprintf(stdout,_F(Z),A)
#define out2(Z,A,B) fprintf(stdout,_F(Z),A,B)
#define out3(Z,A,B,C) fprintf(stdout,_F(Z),A,B,C)
#define out4(Z,A,B,C,D) fprintf(stdout,_F(Z),A,B,C,D)
#define out5(Z,A,B,C,D,E) fprintf(stdout,_F(Z),A,B,C,D,E)
#define out6(Z,A,B,C,D,E,F) fprintf(stdout,_F(Z),A,B,C,D,E,F)
#define out7(Z,A,B,C,D,E,F,G) fprintf(stdout,_F(Z),A,B,C,D,E,F,G)
#define out8(Z,A,B,C,D,E,F,G,H) fprintf(stdout,_F(Z),A,B,C,D,E,F,G,H)
#define out9(Z,A,B,C,D,E,F,G,H,I) fprintf(stdout,_F(Z),A,B,C,D,E,F,G,H,I)
#define out10(Z,A,B,C,D,E,F,G,H,I,J) fprintf(stdout,_F(Z),A,B,C,D,E,F,G,H,I,J)
#define out11(Z,A,B,C,D,E,F,G,H,I,J,K) fprintf(stdout,_F(Z),A,B,C,D,E,F,G,H,I,J,K)

#define err0(Z) fprintf(stderr,_F(Z))
#define err1(Z,A) fprintf(stderr,_F(Z),A)
#define err2(Z,A,B) fprintf(stderr,_F(Z),A,B)
#define err3(Z,A,B,C) fprintf(stderr,_F(Z),A,B,C)
#define err4(Z,A,B,C,D) fprintf(stderr,_F(Z),A,B,C,D)
#define err5(Z,A,B,C,D,E) fprintf(stderr,_F(Z),A,B,C,D,E)
#define err6(Z,A,B,C,D,E,F) fprintf(stderr,_F(Z),A,B,C,D,E,F)
#define err7(Z,A,B,C,D,E,F,G) fprintf(stderr,_F(Z),A,B,C,D,E,F,G)
#define err8(Z,A,B,C,D,E,F,G,H) fprintf(stderr,_F(Z),A,B,C,D,E,F,G,H)
#define err9(Z,A,B,C,D,E,F,G,H,I) fprintf(stderr,_F(Z),A,B,C,D,E,F,G,H,I)
#define err10(Z,A,B,C,D,E,F,G,H,I,J) fprintf(stderr,_F(Z),A,B,C,D,E,F,G,H,I,J)
#define err11(Z,A,B,C,D,E,F,G,H,I,J,K) fprintf(stderr,_F(Z),A,B,C,D,E,F,G,H,I,J,K)

char *_G(s)
   char *s;
{  static char m[MSGMAX];
	strcpy(m,CMDNAME);
	strcat(m,": ");
	strcat(m,s);
	strcat(m," - abort\n");
	return(m);
	}

#define abort0(Z) fprintf(stderr,_G(Z))
#define abort1(Z,A) fprintf(stderr,_G(Z),A)
#define abort2(Z,A,B) fprintf(stderr,_G(Z),A,B)
#define abort3(Z,A,B,C) fprintf(stderr,_G(Z),A,B,C)
#define abort4(Z,A,B,C,D) fprintf(stderr,_G(Z),A,B,C,D)
#define abort5(Z,A,B,C,D,E) fprintf(stderr,_G(Z),A,B,C,D,E)
#define abort6(Z,A,B,C,D,E,F) fprintf(stderr,_G(Z),A,B,C,D,E,F)
#define abort7(Z,A,B,C,D,E,F,G) fprintf(stderr,_G(Z),A,B,C,D,E,F,G)
#define abort8(Z,A,B,C,D,E,F,G,H) fprintf(stderr,_G(Z),A,B,C,D,E,F,G,H)
#define abort9(Z,A,B,C,D,E,F,G,H,I) fprintf(stderr,_G(Z),A,B,C,D,E,F,G,H,I)
#define abort10(Z,A,B,C,D,E,F,G,H,I,J) fprintf(stderr,_G(Z),A,B,C,D,E,F,G,H,I,J)
#define abort11(Z,A,B,C,D,E,F,G,H,I,J,K) fprintf(stderr,_G(Z),A,B,C,D,E,F,G,H,I,J,K)

boolean ask_more()
{   FILE *tty;
    char reply[20];
    static int skip = 0;
	if(skip<=0) {
		fputs("MORE? ",(tty=fopen("/dev/tty","w")));  fclose(tty);
		fgets(reply,20,(tty=fopen("/dev/tty","r")));  fclose(tty);
		switch (reply[0]) {
			case '\0': case 'y': case 'Y':
				return(T);
			case 'n': case 'N': case 'q': case 'Q':
				return(F);
			case '0': case '1': case '2': case '3': case '4':
			case '5': case '6': case '7': case '8': case '9':
				skip = atoi(reply);
				return(T);
			case '*':
				skip = INT_MAX;
				return(T);
			default:
				skip = 0;
				return(T);
			};
		}
	else skip--;
	return(T);
	}

boolean ask_quit()
{   FILE *tty;
    char reply[20];
	fputs("QUIT? ",(tty=fopen("/dev/tty","w")));  fclose(tty);
	fgets(reply,20,(tty=fopen("/dev/tty","r")));  fclose(tty);
	switch (reply[0]) {
		case 'y': case 'Y':
			return(F);
		case 'n': case 'N': case 'q': case 'Q':
			return(T);
		};
	return(F);
	}

