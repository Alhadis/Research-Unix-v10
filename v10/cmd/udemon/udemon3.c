#

#define ERROR	0
#include "udemon.h"
#include <stdio.h>
#include "udemon.e"


extern int parent;
char	*info "/usr/tom/info";
/* tprint -- print file store message in intelligable form */


char *opname[OPNUM] {
	"OK",
	"DEFINE",
	"REDEFINE",
	"DELETE",
	"?",
	"EOM",
	"?",
	"XMT",
	"EVAL",
	"POP",
	"CDIR",
	"?",
	"OPEN",
	"CLOSE",
	"SEEK",
	"READ",
	"WRITE",
	"TRUNCATE",
	"LOGIN",
	"QUIT",
	"LIST",
	"STATUS",
	"TIME",
	"UNIQUE",
	"APPEND",
	"?",
	"ASSIGN",
	"FCREATE",
	"IF",
	"FI",
	"SKIP",
	"DCREATE",
	"?",
	"UNLESS"
};

char *tname[MAXTYPE] {
	"\"",
	"W ",
	"??? ",
	"ERROR ",
	"FREF ",
	"X ",
	"STATUS ",
	"DIR ",
	"FILE ",
	"INT ",
	"",
	"??? ",
	"??? ",
	"L",
	"ACC"
};

char tsize[MAXTYPE] { 
	-1,2,0,4,2,4,69,0,4,4,2,0,0,1,0};


tprint(msg,num)
char *msg; 
int num;
{
	char w, *p; 
	int c, d;
	int ibuf[2];

	if (trace == 0) 
		return;
	for (p=msg; p<(msg+num);){
		w = *p++;
		if (w<0){
			w =& 0177;
			if (w >= OPNUM) {
				printf("illegal op %d",w); 
				break;
			}
			printf("%s ",opname[w]);
			if (w == (EOM - 128)) 
				break;
		}
		else
		    if (w == IITYPE){
			p =+ copy(ibuf,p,4);
			printf("INT %s ",locv(ibuf[0],ibuf[1]));
		}
		else
		    if (w > 0){
			if ((w >= '!') && (w <= '~')){
				msg[num] = '\0';
				printf("cmd = %s",msg);
				break;
			}
			if (w>MAXTYPE) {
				printf("illegal type = %d",w); 
				break;
			}
			printf("%s",tname[w-1]);
			if (w==STYPE) 
				printf("%d:",*p++);
			c = tsize[w-1];
			if (c < 0) {
				printf("%s\" ",p); 
				while (*p++ != 0);
			}
			else
			    if (c == 1) printf("%d ",*p++);
			else
			    for (; c>0;c =- 2){
				p =+ copy(&d,p,2);
				printf("0%o",d);
				if (c>2) 
					printf(","); 
				else
				    printf(" ");
			}
		}
	}
	printf("\n");
}


prt(a, b, c, d, e, f){
	if (trace == 0) return;
	printf(a, b, c, d, e, f);
}

error(n,m)
int n,m;
{
	extern int errno;
	extern int sys_nerr;
	extern char *sys_errlist[];
	int i, err;

	i = (lp == mlink) ? (mp - mbuf) : (mlink[1].l_mp - mbuf);
	err = (n==0) ? errno : n;
	*rp++ = ETYPE;
	rp =+ copy(rp,&i,2);
	*rp++ = err;
	*rp++ = m;
	if (trace){
		printf("error(%d,%d) at %d - ",err,m,i);
		if (n==0)
			printf("%s\n",
			errno>sys_nerr?"unknown error":sys_errlist[errno]);
		else
		    printf("%s\n",emess[n-100]);
	}
	respond(0);
	reset();
}

mesg(s)
char	*s;
{
	FILE *fo;
	int	ttime[2],save;

	if((fo = fopen(info,"w")) == NULL)
		return;
	lseek(fo,0L,2);
	fprintf(fo,"%s:\n%s\n%s\n",tiu,ctime(ttime),s);
	fflush(fo);
	close(fo);
}
