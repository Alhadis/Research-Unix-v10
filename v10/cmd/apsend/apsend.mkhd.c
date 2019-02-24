/******     apsend.mkhd.c     *****/

#include <stdio.h>
#include <pwd.h>
#include <time.h>
#include <ctype.h>
#include "apsend.h"

main()
{
	FILE *hdfile,*dtfile;
	char header[160],tempbuf[80],*temp,tempbuf1[80],*temp1,tempbuf2[80],*temp2;
	char tarray[20],parray[6],narray[8];
	char *pid, *acct, *bin, *passacct, *nodename, *version, *aversion, *mhinfo, c;
	char *tempbin, *tempacct;
	int uid, i, astatus, passtype, found;
	struct passwd *getpwuid(), *ps;
	struct tm *localtime(), *lt;
	long time(), now;
	char *asctime(), *at, *getenv();

	for (i=0;i<160;i++) {
		header[i] = ' ';
	}
	for (i=0;i<80;i++) {
		tempbuf[i] = '\0';
		tempbuf1[i] = '\0';
		tempbuf2[i] = '\0';
	}
	temp = tempbuf;
	temp1 = tempbuf1;
	temp2 = tempbuf2;
	found = passtype = 0;
	astatus = 1;
	tempbin = tempacct = 0;

	pid = getenv("PID");
	strcpy (parray,pid);
	nodename = getenv("NODE");
	strcpy (narray,nodename);
	sprintf (&header[UJN],"%s.%s",narray,parray);
	uid = getuid();
	ps = getpwuid(uid);
	if (ps == NULL) {
		printf ("login not found; job not submitted\n");
		exit (1);
	}
	strcpy (&header[ULOG],ps->pw_name);

	if ((mhinfo = getenv("MHINFO")) != NULL) {
		strcpy (temp1,mhinfo);
		if ((tempbuf1[5] == ',') || (tempbuf1[5] == '\0'))
			i = 5;
		else
			i = 6;
		tempbuf1[i] = tempbuf1[i+34] = '\0';
		tempbin = &tempbuf1[i+1];
		strcpy (&header[BIN],tempbin);
		tempacct = temp1;
	}
	if (((bin = getenv("MAILADDR")) != NULL) || (((bin = getenv("BBIN")) != NULL) && (*bin != '0'))) {
		strcpy (temp2,bin);
		tempbuf2[34] = '\0';
		strcpy (&header[BIN],temp2);
	}
	else
		bin = tempbin;
	if ((acct = getenv("ACCOUNT")) != NULL){
		strcpy (temp2,acct);
		if ((tempbuf2[5] == ',') || (tempbuf2[5] == '\0'))
			i = 5;
		else
			i = 6;
		tempbuf2[i] = '\0';
		acct = temp2;
	}
	else
		acct = tempacct;
	if (acct != NULL){
		astatus = chkacct( acct,i);
		if (astatus != 0){
			printf ("valid GCOS account number needed; job not submitted\n");
			exit(2);
		}
		strcpy (&header[MHACCT],acct);
	}

/* try to get GCOS account and/or bin info from passwd file */
/* determine which kind of passwd file we have: 
	if no '(' in GCOS field, must be research type;
		typical format is ':mhxxxx,myyy:'
	if ')' found, followed by < 5 char string, must be ISCC standard style;
		typical format is ':zzzzz(mhxxxx)myyy,zzzzz:' ; trailing ',zzzzz' optional;
	if ')' found followed by 5-6 char string, must be USG style;
		typical format is ':zzzzz(mhxxxx)mhxxxx,myyy:';
		use account number immediately following ')';
*/

	if ((acct == NULL) || (bin == NULL)){
		passacct = ps->pw_gecos;
		strcpy (temp,passacct);
		if ((acct == NULL) && (tempbuf[0] == '\0')){
			printf ("valid GCOS account number needed; job not submitted\n");
			exit(3);
		}
		for (i=0;i<80;i++){
			if (tempbuf[i] == '('){
				passtype=2;
				break;
			}
			if ((tempbuf[i] == ':' ) || (tempbuf[i] == '\0')){
				passtype=1;
				break;
			}
		}
		if (passtype == 2){
			for (i=0;i<80;i++){
				if (tempbuf[i] == ')')
					found = i;
				if ((found != 0) && (tempbuf[i] == ','))
					if (((i - found) == 6) || ((i - found) == 7))
						passtype = 3;
				if ((tempbuf[i] == ':') || (tempbuf[i] == '\0'))
					break;
			}
		}
		if (passtype == 1){
			passacct = temp;
			while ((c = *temp++) != '\0')
				if (c == ',') break;
		}
		if (passtype == 2){
			while (*temp++ != '(');
			passacct = temp;
			while ((c = *temp++) != '\0')
				if (c == ')') break;
		}
		if (passtype == 3){
			while (*temp++ != ')');
			passacct = temp;
			while ((c = *temp++) != '\0')
				if (c == ',') break;
		}
		if (bin == NULL)
			strcpy (&header[BIN],temp);
		temp--;
		*temp = '\0';
		i = temp - passacct;
		if (acct == NULL){
			astatus = chkacct( passacct,i);
			if (astatus != 0){
				printf ("valid GCOS account number needed; job not submitted\n");
				exit(4);
			}
			strcpy (&header[MHACCT],passacct);
		}
	}
	now = time((long *)0);
	lt = localtime(&now);
	sprintf (&header[TODAPS],"%02d",lt->tm_mon+1);
	sprintf (&header[TODAPS+2],"%02d",lt->tm_mday);
	sprintf (&header[TODAPS+4],"%02d",lt->tm_hour);
	sprintf (&header[TODAPS+6],"%02d",lt->tm_min);
	sprintf (&header[TODAPS+8],"%02d",lt->tm_sec);
	version = getenv("VERS");
	strcpy (tarray,version);
	sprintf (&header[VERSION],"%s",tarray);
	aversion = getenv("AVERS");
	strcpy (tarray,aversion);
	sprintf (&header[AVERSION],"%s",tarray);
	sprintf (&header[159],"%c",'\n');
	for (i=0;i<160;i++)
		if (header[i] == '\0')
			header[i] = ' ';
	strcpy (tarray,"/tmp/apshead");
	strncat (tarray,parray,5);
	hdfile = fopen(tarray,"w");
	strcpy (tarray,"/tmp/apsdate");
	strncat (tarray,parray,5);
	dtfile = fopen(tarray,"w");
	if (hdfile == NULL || dtfile == NULL){
		printf ("can't open /tmp file(s); job not submitted\n");
		exit(5);
	}
	for (i=0;i<160;i++){
		c = header[i];
		putc(c,hdfile);
	}
	at = asctime(lt);
	fprintf (dtfile,"%s",at);
	exit(0);
}
chkacct(acct,n)
	char acct[]; int n;
	{
	if ( n == 5){
		if ((acct[0] == 'm') || (acct[0] == 'M')){
			acct[6] = '\0';
			acct[5] = acct[4];
			acct[4] = acct[3];
			acct[3] = acct[2];
			acct[2] = acct[1];
			acct[1] = 'h';
		}
		else
			return (1);
	}
	if ((isalpha(acct[0]) != 0) && (isalpha(acct[1]) != 0) &&
		(isalnum (acct[2]) != 0) && (isdigit (acct[3]) != 0) &&
		(isdigit (acct[4]) != 0) && (isdigit (acct[5]) != 0))
			;
	else
		return (1);
	if ((acct[2] == '0') && (acct[3] == '0') && (acct[4] == '0')
		&& (acct[5] == '0'))
		return (2);   
	else
		return (0);
	}
