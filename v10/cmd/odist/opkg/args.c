#include "asd.h"
#include <libv.h>
#include <string.h>

int bflag;			/* -b make a backup package */
int kflag;			/* -k crypto key (deprecated) */
int Kflag;			/* -K crypto key from file (deprecated) */
int nflag;			/* -n don't actually install files */
int vflag;			/* -v verbose output */
char *xstr;			/* -x command to execute */
char *Xstr;			/* -X file to execute */
struct replist *replist;	/* -D substitutions */

int
getargs (int argc, char **argv, char *optkey, int (*func)(FILE *, char *))
{
	register int c;
	int rc = 0;
	char *keyfile;

	while ((c = getopt (argc, argv, optkey)) != EOF) {
		register struct replist *rl;
		register char *p, *q;

		switch (c) {

		case 'b':
			bflag++;
			break;

		case 'k':
			kflag++;
			break;

		case 'n':
			nflag++;
			break;
		
		case 'v':
			vflag++;
			break;

		case 'x':
			if (xstr) {
				fprintf (stderr, "duplicate -x ignored\n");
				rc++;
			} else if (Xstr) {
				fprintf (stderr, "cannot have both -x and -X\n");
				rc++;
			} else
				xstr = optarg;
			break;

		case 'X':
			if (Xstr) {
				fprintf (stderr, "duplicate -X ignored\n");
				rc++;
			} else if (xstr) {
				fprintf (stderr, "cannot have both -x and -X\n");
				rc++;
			} else
				Xstr = copy (transname (optarg));
			break;

		case 'D':
			p = strchr (optarg, '=');
			if (p == NULL) {
				fprintf (stderr, "invalid option %s\n", optarg);
				exit (1);
			}
			rl = new (struct replist);

			/* copy the pathname to rl->source */
			rl->source = alloc ((unsigned) (p - optarg + 1));
			p = rl->source;
			q = optarg;
			while (*q != '=')
				*p++ = *q++;
			*p = '\0';

			/* now expand rl->source */
			p = rl->source;
			rl->source = copy (fullname (p));
			free (p);

			/* expand rl->dest */
			rl->dest = copy (fullname (q + 1));

			/* link rl into the chain */
			rl->link = replist;
			replist = rl;
			break;

		case 'K':
			Kflag++;
			keyfile = optarg;
			break;

		case '?':
		default:
			rc++;
			break;
		}
	}

	if (rc) {
		fprintf (stderr, "%s: bad argument\n", argv[0]);
		exit (rc);
	}

	if (kflag && Kflag) {
		fprintf (stderr, "%s: cannot specify both k and K\n", argv[0]);
		exit (1);
	}

	/* read key from terminal if requested */
	if (kflag) {
		register char *p;
		p = getpass ("Key:");

		/* a null key is treated as no key at all */
		if (p && *p)
			setup (p);
		else
			kflag = 0;
	}
	
	/* read key from file if requested */
	if (Kflag) {
		char key[100];
		register FILE *kf;
		register char *p;

		/* try to open the file */
		kf = fopen (keyfile, "r");
		if (kf == NULL) {
			perror (keyfile);
			exit (1);
		}

		/* read the first line */
		p = fgets (key, sizeof (key), kf);

		fclose (kf);

		/* if EOF, assume no key */
		if (p == NULL) {
			Kflag = 0;
		} else {

			/* delete the trailing newline */
			p = key;
			while (*p != '\n' && *p != '\0')
				p++;
			*p = '\0';

			/* if the key is empty, assume no key */
			if (key[0] == '\0')
				Kflag = 0;
			else
				setup (key);
		}
	}

	if (func) {
		/* process the arguments */
		if (optind >= argc)
			rc = (*func) (stdin, "standard input");
		else {
			register int i;
			for (i = optind; i < argc; i++) {
				register char *fn = argv[i];
				register FILE *f = fopen (fn, "r");
				if (f) {
					rc += (*func) (f, argv[i]);
					fclose (f);
				} else {
					fprintf (stderr, "%s: can't open %s\n", argv[0], fn);
					rc++;
				}
			}
		}
	}

	return rc;
}
