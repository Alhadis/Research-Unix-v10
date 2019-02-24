static char ID[] = "@(#) sdplib.c: 1.1 3/16/82";
#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>
#include "sdp.h"
#include "define2.h"

#ifdef SHORT
#define	LB01		"LB01 "
#define LB02		"LB02 "
#define LB03		"LB03 "
#define LB04		"LB04 "
#define LB05		"LB05 "
#define LB06		"LB06 "
#define LB07		"LB07 "
#define LB08		"LB08 "
#define LB09		"LB09 "
#define LB10		"LB10 "
#define LB11		"LB11 "
#else
#define	LB01	"%s","LB01 -- fail to malloc\n"
#define LB02	"LB02 -- close error: %d\n%d\n",errno,designator
#define LB03	"LB03 -- creat error: %d\n%s\t%o\n",errno,name,mode
#define LB04	"LB04 -- fail to close in creat routine: %d\n%s\t%d\n",errno,name,fildes
#define LB05	"LB05 -- open error: %d\n%s\t%d\n",errno,name,perms
#define LB06	"LB06 -- read error: %d\n%d\t%o\t%d\n",errno,designator,buffer,nbytes
#define LB07	"LB07 -- lseek error: %d\n%d\t%ld\t\n",errno,designator,position
#define LB08	"LB08 -- unlink error: %d\n%s\n",errno,name
#define LB09	"LB09 -- write error: %d\n%d\t%o\t%d\n",errno,designator,buffer,nbytes
#define LB10	"LB10 -- size of name exceeds maximum length\n%s\t%d\n",name,DIRSIZ
#define LB11	"LB11 -- fail to allocate space for name\n",name
#endif


extern int errno ;
extern struct LIBFILE *Glib_file ;
extern close() ;
extern FD creat() ;
extern free() ;
extern long lseek() ;
extern char *malloc() ;
extern FD open() ;
extern read() ;
extern unlink() ;
extern write() ;

int *
ALLOC(size)			/* allocate memory */
int size;
{
	register char	*ptr;

	ptr = malloc(size);
	if(ptr == NULL)
		ERR_RET(LB01,NULL)
	return((int *)ptr);
}



char *				/* header file name string */
BUILDNAME(name,ordinal)
char	*name;
int	ordinal ;
{
	register int	size;
	int		*build;
	register char	*i ;
	char		buf[COUNT] ;

	if(ordinal == -1)
		sprintf(buf,"%s",SUFFIX) ;
	else
		sprintf(buf,"%d",ordinal) ;
	size = 0 ;
	for(i = name; *i != 0; i++)
		if(*i == '/')
			size = 0 ;
		else
			size++ ;
	if(size + strlen(buf) > DIRSIZ)
		ERR_RET(LB10,NULL)
	if((build = ALLOC(strlen(name) + strlen(buf) + 1)) == NULL)
		ERR_RET(LB11,NULL)
	sprintf(build,"%s%s",name,buf);
	return((char *)build);
}



int
CLOSE(designator)		/* close file */
FD designator;
{
	if(close(designator) == -1)
		ERR_RET(LB02,ERROR)
	return(SUCCESS);
}



int
CREATE(name,mode)		/* create file */
char	*name;
int	mode ;
{
	register FD	fildes;

	fildes = creat(name,mode) ;
	if(fildes == -1)
		ERR_RET(LB03,ERROR)
	if(close(fildes) == -1)
		ERR_RET(LB04,ERROR)
	return(SUCCESS);
}






FREE(ptr)			/* free memory */
char *ptr;
{
	free(ptr);
	return;
}



FD
OPEN(name,perms)		/* open file */
char	*name;
int	perms ;
{
	register FD	fildes;

	fildes = open(name,perms) ;
	if(fildes == -1)
		ERR_RET(LB05,ERROR)
	return(fildes);
}



int
READ(designator,buffer,nbytes)	/* read file */
FD designator;
int nbytes;
char *buffer;
{
	register int	byterd;

	if( (byterd = read(designator,buffer,nbytes)) == -1 )
		ERR_RET(LB06,ERROR)
	return(byterd) ;
}




int
SEEK(designator,position)	/* seek in file */
FD designator;
long position;
{

	if(lseek(designator,position,0) != position)
		ERR_RET(LB07,ERROR)
	return(SUCCESS);
}


int
UNLINK(name)			/* unlink file */
char *name;
{
	if(unlink(name) != 0)
		ERR_RET(LB08,ERROR)
	return(SUCCESS);
}



int
WRITE(designator,buffer,nbytes)	/* write file */
FD designator;
char *buffer;
int nbytes;
{
	if(write(designator,buffer,nbytes) != nbytes)
		ERR_RET(LB09,ERROR)
	return(SUCCESS);
}
