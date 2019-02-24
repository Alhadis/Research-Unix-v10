#include <stdio.h>
#include <sys/inet/in.h>
#include "config.h"

/* imported */
extern char *in_getw();
extern char *strchr();
extern char *strcpy();

/*
 *	Look up the service matching the given parameters.
 */
struct in_service *
in_service(name, proto, port)
char *name;
char *proto;
unsigned long port;
{
	static struct in_service serv;
	static char sname[64], sproto[64];
	char buf[512], *p;
	FILE *fp;

	if((fp = fopen(SERVICES, "r")) == 0){
		perror(SERVICES);
		return(0);
	}
	while(fgets(buf, sizeof(buf), fp)){
		if(buf[0] == '\n' || buf[0] == '#')
			continue;
		if((p = in_getw(buf, sname)) == 0)
			continue;
		serv.name = sname;
		if(name != NULL && strcmp(name, serv.name) != 0)
			continue;
		if(in_getw(p, sproto)==0)
			continue;
		serv.port = atoi(sproto);
		if((port || name==NULL) && port != serv.port)
			continue;
		p = strchr(sproto, '/');
		serv.proto = p == NULL ? "tcp" : p+1;
		if(proto != NULL && strcmp(proto, serv.proto) != 0)
			continue;

		/* we've got a match */
		fclose(fp);
		return(&serv);
	}
	fclose(fp);
	if (name == NULL) {
		sprintf(sname, "tcp.%d", port);
		strcpy(sproto, proto!=NULL ? proto : "tcp");
		serv.proto = sproto;
		serv.name = sname;
		serv.port = port;
		return(&serv);
	}
		
	return(NULL);
}
