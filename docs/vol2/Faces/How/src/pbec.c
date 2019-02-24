#include <stdio.h>

#define SZ	512

int lasta, lastb;
char buf[512];
char *back = (char *) 0;
int Factor=0;
int ncurve=0;
int Xoffset = 16;
int Yoffset = 100;

main(argc, argv)
	char **argv;
{
	extern int lcnt;
	int i;

	switch (argc) {
	case 5:
		back = argv[4];
	case 4:
		Yoffset = 100-atoi(argv[3]);
		Xoffset = 16+atoi(argv[2]);
	case 2:
		Factor = atoi(argv[1]);	/* >0 stretches the letter in x */
	case 1:
		break;
	default:
		fprintf(stderr, "usage: pcurve ");
		fprintf(stderr, "[N [dX dY [background]]] < font > script\n");
		exit(1);
	}

	prelude();
	printf("def curve() {\n");
	getcurve();

	runcontour();
	printf("}\n");
	printf("x {\n");
		printf("	curve()\n");
	for (i = 100; i < lcnt; i += 100)
		printf("	curve%d()\n", i);
	printf("}\nx new=old\n");
}

getcurve()
{ int a, b, c;

	for (;;)
	{	if (fgets(buf, 512, stdin) == NULL)
			return;
		c = 0;
		while (strcmp(buf, "1e6") == 0)
		{	c = 1;
			if (fgets(buf, 512, stdin) == NULL)
				return;
		}
		sscanf(buf, "%d %d", &a, &b);
		addnode(((a+Xoffset)*SZ)/512, ((b+Yoffset)*SZ)/512, c);
	}
}

prelude()
{
	printf("def mapper(t0, t1, f0, f1, h) {\n");
	printf("if (abs(t0-t1)>1)\n");
	printf("{	mapper((t0+t1)/2, t1, (f0+f1)/2, f1, h)\n");
	printf("	mapper(t0, (t0+t1)/2, f0, (f0+f1)/2, h)\n");
	printf("} else\n");
	printf("	new[t0, h] = $1[f0, h]\n");
	printf("}\n");
	if (back)
		printf("get \"%s\"\n", back);
	else
		printf("x new=y*%d/%d;\n", 255, SZ-1);
	printf("def setter(aa, b, c, d) {\n");
	printf("	for (x=aa; x<b; x++)\n");
	if (back)
		printf("		new[x,c]=$2[x,c];\n");
	else
		printf("		new[x,c]=d;\n");
	printf("}\n");
}
