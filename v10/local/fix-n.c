#include <stdio.h>
#include <sys/types.h>
#include <a.out.h>
char *malloc();
main(argc, argv)
char **argv;
{
	char *memt, *memd;
	struct exec a;

	argc--, argv++;
	while (argc > 0) {
		close(0);
		if (open(argv[0], 0) < 0)
			perror(argv[0]);
		else {
			read(0, &a, sizeof(a));
			if (a.a_magic != 0410)
				fprintf(stderr, "%s: %o file\n",
				    argv[0], a.a_magic);
			else {
				int i = (a.a_text+1023)&~1023;
				int j = (a.a_text+511)&~511;
				printf("old way bnd %d, new way %d\n", j, i);
				if (j == i) {
					printf("ok as is\n");
					goto easy;
				}
				memt = malloc(a.a_text+1024);
				memd = malloc(a.a_data);
				read(0, memt, a.a_text);
				read(0, memd, a.a_data);
				close(1);
				if (creat(argv[0], 0777) < 0)
					perror(argv[0]);
				else {
					a.a_syms = 0;
					a.a_text = i;
					write(1, &a, sizeof (a));
					write(1, memt, i);
					write(1, memd, a.a_data);
					free(memt);
					free(memd);
				}
				fprintf(stderr, "%s: fixed\n", argv[0]);
			}
		}
easy:
		argc--, argv++;
	}
}
