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
	char *opw1, *opw2, *pw;

	pw = "";

	opw1 = getpass("Signature key:");
	if(opw1) opw1 = strdup(opw1);
	if(opw1) opw2 = getpass("Again:");
	if(opw1==0 || opw2==0) {
		fprintf(stderr,"resign: failed to get password\n");
		exit(1);
	}
	if(strcmp(opw1,opw2)!=0) exit(1);

	switch( enroll(name, opw1, pw) ) {
	case 0:
		printf("BAD resign of %s\n", name);
		return 1;
		break;
	default:
		printf("Server uncommunicative; try again later\n");
		return 1;
		break;
	case 1:
		printf("GOOD resign of %s\n", name);
		return 0;
		break;
	}
}
