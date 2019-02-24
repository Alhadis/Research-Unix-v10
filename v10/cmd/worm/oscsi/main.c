#include	<stdio.h>
#include	"jukebox.h"
#include	"hdr.h"

main(int argc, char *argv[])
{
	int c;
	int err = 0, cold = 0, warm = 0;
	char *toload = 0, *uload = 0;
	char *drive = 0;
	char buf[256];
	extern int optind;
	extern char *optarg; 

	setbuf(stdout, (char *)0);	/* turn off buffering */
	/* gather options */
	while ((c = getopt(argc,argv,"cn:l:wu:")) != -1)
		switch (c)
     		{
		case 'l':	toload = optarg; break;
		case 'c':       cold = 1; break;
		case 'w':       warm = 1; break;
		case 'u':	uload = optarg ; break;
		case 'n':	drive = optarg ; break;
		default:	err = 1; break;
		}
	if(err)
		exit(1);
	/* now actually do some work */
	if (toload){
		if (j_load(toload, buf, 30))
			printf("load %s failed: %s\n", toload, buf);
		else
			printf("loaded %s on %s\n", toload, buf);
	}
	if(drive){
		if(j_volid(atoi(drive), buf))
			printf("j_volid(%s) failed: %s\n", drive, buf);
		else
			printf("%s is mounted on drive %d\n", buf, atoi(drive));
	}
	if(cold){
		printf("invent cold: %d\n", cold);
		cold_inventory(30, buf);
		
	}
	if (warm) {
		printf("invent warm: %d\n", warm);
		warm_inventory(buf);
		
	}


	if (uload){
		if (j_unload(uload, buf)) 
			printf("unload %s failed: %s\n", uload, buf);
		else
			printf("unloaded %s from %s\n", uload, buf);
		

	}
	exit(0);
}
