# include "stdio.h"
# include "assert.h"
# include "ctype.h"
# include "sys/param.h"
# include "sys/stat.h"
# include "sys/dir.h"
extern char pwdir[];

/* pwd command, modified to write in argument */

char	dot[]	= ".";
char	dotdot[] = "..";
char	name[512];
char	output[512];
int	file;
int	off	= -1;
struct	stat	d, dd;
struct	direct	dir;

char *
getpwd ()
{
	int rdev, rino;

	strcpy(output, "/");
	stat("/", &d);
	rdev = d.st_dev;
	rino = d.st_ino;
	for (;;) {
		stat(dot, &d);
		if (d.st_ino==rino && d.st_dev==rdev)
			{
			prname();
			break;
			}
		if ((file = open(dotdot,0)) < 0) {
			fprintf(stderr,"pwd: cannot open ..\n");
			exit(1);
		}
		fstat(file, &dd);
		chdir(dotdot);
		if(d.st_dev == dd.st_dev) {
			if(d.st_ino == dd.st_ino)
				{
				prname();
				break;
				}
			do
				if (read(file, (char *)&dir, sizeof(dir)) < sizeof(dir)) {
					fprintf(stderr,"read error in ..\n");
					exit(1);
				}
			while (dir.d_ino != d.st_ino);
		}
		else do {
				if(read(file, (char *)&dir, sizeof(dir)) < sizeof(dir)) {
					fprintf(stderr,"read error in ..\n");
					exit(1);
				}
				stat(dir.d_name, &dd);
			} while(dd.st_ino != d.st_ino || dd.st_dev != d.st_dev);
		close(file);
		cat();
	}
	chdir (output);
	return(output);
}

prname()
{
	if (off<0)
		off = 0;
	name[off]='\0';
	strcat (output, name);
	return ;
}

cat()
{
	register i, j;

	i = -1;
	while (dir.d_name[++i] != 0);
	if ((off+i+2) > 511)
		{
		fprintf(stderr, "Directory string exceeds 512 characters in length\n");
		exit(1);
		}
	for(j=off+1; j>=0; --j)
		name[j+i+1] = name[j];
	off=i+off+1;
	name[i] = '/';
	for(--i; i>=0; --i)
		name[i] = dir.d_name[i];
}
copyto( sdest, source)
	char *sdest;
	char *source;
{
char *s, target[100];
FILE *f1, *f2;
int k;
struct stat stbuf;
for(s=sdest; *s; s++);
while (!isspace(*s) && s>sdest)
	s--;
if (!isspace(*s))
	{
	fprintf(stderr, "No destination '%s'\n", sdest);
	return;
	}
if (*++s == '/')
	sprintf(target, "%s", s);
else
	sprintf(target, "%s/%s", pwdir, s);
k = stat (target, &stbuf);
if (k==0 && (stbuf.st_mode & S_IFDIR)!=0)
	{
	strcat (target, "/");
	strcat (target, source);
	}
f2 = fopen(target, "a");
if (f2==NULL)
	{
	fprintf(stderr, "Can not open %s\n",target);
	return;
	}
f1 = fopen(source, "r");
while ( (k= getc(f1)) != EOF)
	putc(k, f2);
fclose(f1);
fclose(f2);
}
