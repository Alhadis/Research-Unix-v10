extern int getopt(int, char **, char *);
extern int optind;
extern int opterr;
extern char *optarg;
/* EVIL EVIL HACK TO DECLARE FDOPEN */
extern FILE *fdopen(int, char *);
