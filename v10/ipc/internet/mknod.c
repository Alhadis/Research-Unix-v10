#include  "stdio.h"

main(argc, argv)
char **argv;
{
     mkdir(argv[1], atoi(argv[2]));
}
