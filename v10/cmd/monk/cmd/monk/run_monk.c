#include	<stdio.h>
#include	<ctype.h>
#include	"search.h"
#include	"warn.h"
#include	"rd.h"

#define	USAGE	"monk [-d database_dir -Dn ] filename\n"
#define	OPTIONS	"d:Dn"

main(argc, argv)
int	argc;
char	**argv;
{
	extern int	optind;
	extern char	*optarg;
	extern short	db_mode;
	extern int	dont_tell_me;

	struct environment	*env;
	int	c, nfiles;
	char	*db_path;
	char	*filename;

	db_path = DB_PATH;
	db_mode = COMPRESSED;
	while ((c = getopt(argc, argv, OPTIONS)) != EOF )
		switch (c) {
			case 'd':
				db_path = optarg;
				break;
			case 'n':
				db_mode = STANDARD;
				break;
			case 'D':
				dont_tell_me = 0;
				break;
			case '?':
				warn_user(0, "usage: %s; no option %c\n",
								USAGE, c);
		}
/* until remove ungetstring at end of style cannot handle stdin */
	if ((nfiles = argc - optind) <= 0) {
		warn_user(0, "usage: %s\n", USAGE);
		exit(0);
	}
	env = (struct environment *) 0;
	while (nfiles--) {
		filename = argv[optind++];
		get_path(filename);
		clear_parent_name();
		env = r_userfile(filename, db_path, env, TRUE);
	}
	end_allenvir(env);
	exit(0);
}
