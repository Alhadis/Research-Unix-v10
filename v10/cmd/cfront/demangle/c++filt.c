/*ident	"@(#)ctrans:demangler/c++filt.c	1.1"*/
/*
 * C++ Demangler Source Code
 * @(#)master	1.5
 * 7/27/88 13:54:37
 */
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
extern char *demangle();
extern char *malloc(),*realloc();
#define TAB 8
#define START 10

int mflag = 0, sflag = 0;

char *
dem(c)
char *c;
{
	char *d,*s = c;
#if vax || sun
	if(*c == '_') c++;
#endif
	d = demangle(c);
	if(d == c)
		return s;
	else
		return d;
}

void
space(n)
int n;
{
	while(n--) putchar(' ');
}

void
subprint(c,i,l)
char *c;
int i,l;
{
	int n;
	for(n=i;n<l;n++)
		putchar(c[n]);
}

struct dlist { char *mname,*dname; } *ds;
int di = 2;
int dn = START;

int
doline(c)
char *c;
{
	int state = 0,q = 0,last=0;
	register int i;

	for(i=0;c[i];i++) {

		/* if state is zero, we have not yet located
		 * a c identifier.  If it is non-zero, we have.
		 */
		if(state == 0) {
			if(isalnum(c[i]) || c[i] == '_')
				state++;
		}
		if(state == 1) {
			char *x,*d;
			int ln,n = i;

			/* find end of c identifier */
			while(c[n] && (isalnum(c[n]) || c[n] == '_'))n++;
			q = n;

			/* find end of spaces following identifier */
			while(c[q] && c[q] == ' ') q++;

			/* extract the c identifier from the line */
			x = malloc(n-i+1);
			strncpy(x,c+i,n-i);
			x[n-i] = 0;

			/* print out the line from the end of the
			 * last identifier+following spaces to the beginning of
			 * this identifier.
			 */
			subprint(c,last,i);

			/* demangle the identifier */
			d = dem(x);
			if(d != x) {
				int flag=0,ix;

				/* print out the demangled name.
				 * assure that it is followed by at least
				 * one space.  More if there is room.
				 */
				printf("%s",d);
				ln = strlen(d);

				/* Print out mangled name if sflag is set.
				 * otherwise, just remember it.
				 */
				if(sflag) {
					int tab = 3*TAB - ln%(3*TAB);
					space(tab);
					ln += tab;
					printf("%s",x);
					ln += strlen(x);
					ln++;
				}

				if(q - i <= ln)
					putchar(' ');
				else
					space(q - i - ln);

				if(mflag) {

					/* check to see if mangled name
					 * is already in the dlist (list of
					 * mangled-demangled names).
					 */
					for(ix=0;ix<di;ix++) {
						if(strcmp(x,ds[ix].mname)==0) {
							flag++;
							break;
						}
					}

					if(flag == 0) {

						/* assure that the list is
						 * big enough to accomodate the
						 * new names.
						 */
						if(di >= dn) {
							dn *= 2;
							ds = (struct dlist *)realloc(ds,
								sizeof(struct dlist)*(dn+1));
							assert(ds != 0);
						}

						/* enter the new names in the list. */
						ds[di].mname = x;
						ds[di].dname = malloc(ln+1);
						strcpy(ds[di].dname,d);
						di++;
					} else free(x);
					/* x is freed only if it is not
					 * put in the list.
					 */
				}

			} else {
				subprint(c,i,q);
				free(x);
			}

			/* move counter to the end
			 * of this (identifier + following spaces)
			 * and continue
			 */
			last = i = q;

			/* counter the ++ of the for loop */
			i--;
			state = 0;
		}
	}

	/* print remainder of line */
	printf("%s\n",c+q);
}

main(argc,argv)
int argc;
char **argv;
{
	int c;
	char *s;
	int i = 0;
	int n = START;
	int copt;
	s = malloc(n+1);
	*s = 0;

	/* The only argument to this command is
	 * -n.  If it is given, the the C++ symbol
	 * map is not generated.  Instead, the symbols
	 * are printed side by side on stdout.
	 */
	while((copt = getopt(argc,argv,"smv")) != EOF) {
		switch (copt) {
		case 'm':
			mflag = 1;
			break;
		case 's':
			sflag = 1;
			break;
		case 'v':
			fprintf(stderr,"Demangler Version 1.5, Date 7/27/88\n");
			break;
		default:
			fprintf(stderr, "usage: %s [ -smv ]\n", argv[0]);
			exit(1);
		}
	}
	/* Initialize C++ symbol map.  The column headings
	 * are entries in the table -- this assures that they
	 * will be alligned correctly.
	 */
	if(mflag) {
		ds = (struct dlist *)malloc(sizeof(struct dlist)*(dn+1));
		ds[0].mname = "mangled:";
		ds[0].dname = "demangled:";
		ds[1].mname = ds[1].dname = "";
	}

	while((c = getchar()) != EOF) {

		/* read one line at a time */
		if(c != '\n') {

			/* expand tabs to spaces */
			if(c == '\t') {
				int nb = TAB - i%TAB;
				c = ' ';
				while(--nb) {
					if(i >= n) {
						n *= 2;
						s = realloc(s,n+1);
						assert(s != 0);
					}
					s[i++] = c;
					s[i] = 0;
				}
			}

			/* append a character to the buffer */
			/* make sure the buffer is big enough */
			if(i >= n) {
				n *= 2;
				s = realloc(s,n+1);
				assert(s != 0);
			}
			s[i++] = c;
			s[i] = 0;

		} else {
			doline(s);
			s[i = 0] = 0;
		}
	}

	if(sflag || di <= 2)
		return 0;

	printf("\nC++ symbol mapping\n");
	{
		int width = 0;
		register int ix;

		/* determine size of columns */
		for(ix=0;ix<di;ix++) {
			int ln = strlen(ds[ix].dname);
			if(ln > width) width = ln;
		}

		width++;
		for(ix=0;ix<di;ix++) {
			printf("%s",ds[ix].dname);
			space(width-strlen(ds[ix].dname));
			printf("%s\n",ds[ix].mname);
		}
	}
}
