#include "stdio.h"
#include "cbt.h"
extern bfile *bopen();
extern mbuf bkey();

char *index;
char *dfile;
char sep = '!';
char preflg;
char nl;

/* get key from index, print all records from dfile.
 * default file names and separator in the index are as given
 * preflg => prefix searching
 */
char line[256];
char buf1[256], buf2[2048];
bfile *bi, *bd;

main(argc, argv)
char **argv;
{	int i;
	char *p;
	for(i = 1; i < argc && *argv[i] == '-'; i++)
		for(p = argv[i] + 1; *(p-1) && *p; p++)
			switch(*p) {
			default:
				fprintf(stderr, "unk flag %c\n", *p);
				exit(1);
			case 'p':
				preflg = 1;
				break;
			case 'n':
				nl = *++p;
				break;
			case 's':
				sep = *++p;
				break;
			}
	if(i < argc) {
		dfile = argv[i++];
		bd = bopen(dfile, 0);
		if(bd == NULL) {
			perror(dfile);
			exit(1);
		}
	}
	if(i < argc) {
		index = argv[i++];
		bi = bopen(index, 0);
		if(bi == NULL) {
			perror(index);
			exit(1);
		}
	}
	for(;;) {
		(void) fgets(line, sizeof(line), stdin);
		if(feof(stdin))
			exit(1);
		for(p = line; *p && *p != '\n'; p++)
			;
		if(*p == '\n')
			*p = 0;
		if(index)
			doindex();
		dodata();
	}
}

doindex()
{	mbuf key, found, newkey, rec;
	int cnt;
	char *q;
	key.mlen = strlen(line);
	key.mdata = line;
	if(bseek(bi, key) == EOF) {
		printf("not found\n");
		return;
	}
	for(cnt = 0; ; cnt++) {
		if(bread(bi, &found, (mbuf *)NULL) == EOF)
			break;
		if(found.mlen < key.mlen || strncmp(found.mdata, line, key.mlen)
			break;
		if(!preflg && found.mdata[key.mlen] != sep)
			break;
		for(q = found.mdata; q < found.mdata + found.mlen; q++)
			if(*q == sep)
				break;
		if(*q != sep) {
			fprintf(stderr, "retrieved bad index:%s\n",
				found.mdata);
			continue;
		}
		newkey.mdata = ++q;
		newkey.mlen = found.mlen - (q - found.mdata);
		if(bseek(bd, newkey) != FOUND) {
			fprintf(stderr, "erro, didn't find %s\n",
				newkey.mdata);
			continue;
		}
		bread(bd, (mbuf *)NULL &rec);
		for(i = 0; i < rec.mlen; i++)
			if(rec.mdata[i] != nl)
				putchar(rec.mdata[i]);
			else
				putchar('\n');
		putchar('\n');
	}
	if(cnt == 0)
		printf("not found\n");
}
/*1010001100101110*/
