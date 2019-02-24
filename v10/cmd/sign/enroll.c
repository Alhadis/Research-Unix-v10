#include <stdio.h>
#include <string.h>


extern char *getpass();
extern char *getlogin();
extern void exit();
char name[10];

main()
{
	strcpy(name,getlogin());
	return enrollfn();
}

enrollfn()
{
	char *opw, *pw1, *pw2;

	opw = "";

	pw1 = getpass("Signature key:");
	if(pw1) pw1 = strdup(pw1);
	if(pw1) pw2 = strdup(getpass("Again:"));
	if(pw1==0 || pw2==0) {
		fprintf(stderr,"enroll: failed to get passwords\n");
		exit(1);
	}
	if(strcmp(pw1, pw2)!=0)exit(1);

	switch( enroll(name, opw, pw1) ) {
	case 0:
		printf("BAD enroll of %s\n", name);
		return 1;
		break;
	default:
		printf("Server uncommunicative; try again later\n");
		return 1;
		break;
	case 1:
		printf("GOOD enroll of %s\n", name);
		return 0;
		break;
	}
}
