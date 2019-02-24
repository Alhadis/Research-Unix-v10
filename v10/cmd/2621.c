/*
_t_e_s_t _u_l _m_o_d_e
*/
#include <sgtty.h>
#include <signal.h>

#define ULON	"\033&dA"
#define ULOFF	"\033&d@"

main(argc, argv)
char *argv[];
{
	int i;
	char buf[512];
	int onintr();
	int pos_, k, ulmode;

	if (argc > 1) {
		if ((i = open(argv[1], 0)) < 0) {
			perror(argv[1]);
			EXIT(1);
		}
		close(0);
		dup(i);
		close(i);
	}
	if (signal(SIGINT, SIG_IGN) == SIG_DFL)
		signal(SIGINT, onintr);

	pos_ = ulmode = 0;
	while ((i = read(0, buf, sizeof(buf))) > 0) {
		for (k = 0; k < i; k++) {
			if (buf[k] == '_' && pos_ == 0) {
				pos_ = 1;
				continue;
			}
			if (pos_ == 1)
				if (buf[k] == '\b') {
					pos_ = 2;
					if (ulmode)
						continue;
					printf("%s", ULON);
					ulmode++;
					continue;
				} else {
					if (ulmode)
						printf("%s", ULOFF);
					putchar('_');
					ulmode = 0;
					pos_ = 0;
				}
			if (pos_ == 2) {
				putchar(buf[k]);
				pos_ = 0;
				continue;
			}
			pos_ = 0;
			if (ulmode)
				printf("%s", ULOFF);
			ulmode = 0;
			putchar(buf[k]);
		}
	}
	printf("%s", ULOFF);
	EXIT(0);
}

onintr()
{
	printf("%s", ULOFF);
	EXIT(0);
}

EXIT(i)
{
	printf("%s", ULOFF);
	exit(i);
}
