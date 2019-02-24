#include <stdio.h>
FILE *fin;
int redir;
main(){
	char cline[200], fline[200], dline[30], ndline[30], tuid[12], tm[12];
	char fid[100], rid[100], rline[113], *gets(), fname[100], buf[200],
		*fgets();
	FILE *f, *fopen(), *popen();

	f = popen("date", "r");
	fgets(dline, 30, f);
	dline[28] = '\0';
	pclose(f);

	printf("Target uid: ");
	gets(tuid);
	printf("Target machine: ");
	gets(tm);
	printf("Sender: ");
	gets(fid);
	printf("Sending machine: ");
	gets(rid);

	strcpy(cline, "uux - ");
	strcat(cline, tm);
	strcat(cline, "!rmail '(");
	strcat(cline, tuid);
	strcat(cline, ")'");

	strcpy(fline, "From ");
	strcat(fline, fid);
	strcat(fline, " ");

	strcpy(dline+16, dline+19);

	*rline = '\0';
	if(*rid){
	   strcpy(rline, " remote from ");
	   strcat(rline, rid);
	}

	printf("%s\n", cline);
	printf("%s%s%s\n", fline, dline, rline);
	printf("CR or new date stamp: ");
	*ndline = '\0';
	gets(ndline);
	if(*ndline) strcpy(dline, ndline);
	strcat(fline, dline);
	strcat(fline, rline);
	if(*ndline) printf("%s\n", fline);

	puts("Last chance to abort.  Type message or <filename");
	redir = getchar();
	if(redir == '<'){
	   gets(fname);
	   if((fin = fopen(fname, "r")) == NULL){
	      printf("Can't open %s\n", fname);
	      exit(1);
	   }
	   redir = 1;
	}
	else{
	   ungetc(redir, stdin);
	   redir = 0;
	}

	f = popen(cline, "w");
	fprintf(f, "%s\n", fline);

	while(next(buf) != EOF) fprintf(f, "%s\n", buf);

	pclose(f);
}
next(buf)
char *buf;
{
	if(redir){
	   if(fgets(buf, 200, fin) == NULL) goto quit;
	   buf[strlen(buf)-1] = '\0';
	}
	else if(gets(buf) == NULL) goto quit;
	if(*buf == '.' && *(buf+1) == '\0') goto quit;
	return 0;
quit:	return EOF;
}
