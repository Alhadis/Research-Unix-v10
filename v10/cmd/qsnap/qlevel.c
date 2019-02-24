#include "defines.h"

main(argc, argv)
	char *argv[];
{
	int Level = 0;
	char buf[256];

	qreset();
	if (argc > 1)
		Level = atoi(argv[1]);

	brightness(Level, Level, Level, Level);
	qcmd("\073", 1);
	qpause();
	qread(buf, 10);
	printf("\n");
	printf("brightness level: %d: %d\n", Level, buf[Level+1]&0xFF);
}
