#ifndef LIB_H
#define LIB_H
char
	*basename(char*),
	*ctime(long*),
	*slashname(char*),
	*strcat(char*, char*),
	*strcpy(char*, char*),
	*strncat(char*, char*, int),
	*strncpy(char*, char*, int)
;

long lseek(int,long,int);
int ReadOK(int, char*, int);
int WriteOK(int, char*, int);

int
	alarm(unsigned),
	chdir(char*),
	close(int),
	dup2(int,int),
	execl(char* ...),
	fork(),
	getpid(),
	ioctl(int,int ...),
	killpg(int,int),
	open(char*, int),
	setpgrp(int,int),
	sleep(unsigned),
	sprintf(char*, const char* ...),
	sscanf(char*, const char* ...),
	strcmp(char*,char*),
	strncmp(char*,char*,int),
	system(char*),
	tdkexec(char*,char*)
;

char *Name(char*,int);
char *SysErr(char* = "");

long modified(int);

int	alldigits(char*);

char	*calloc(int,int);
char	*malloc(int);
#endif
