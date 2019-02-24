/*
** Jerq I/O control codes
*/

#define	JTYPE		('j'<<8)
#define	JBOOT		(JTYPE|1)
#define	JTERM		(JTYPE|2)
#define	JMUX		(JTYPE|3)
#define	JTIMO		(JTYPE|4)
#define	JWINSIZE	(JTYPE|5)
#define	JTIMOM		(JTYPE|6)
#define	JZOMBOOT	(JTYPE|7)
#define	JEXIT		(JTYPE|8)
#define	JDELETE		(JTYPE|9)
#define	JCHDIR		(JTYPE|10)

struct winsize
{
	char	bytesx, bytesy;
	short	bitsx, bitsy;
};
