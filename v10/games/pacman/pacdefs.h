/* dfp #define POS(row,col)    fputs(tgoto(vs_cm,(col),(row)),stdout)*/
#define POS(row,col)	tputs(tgoto(vs_cm,(col),(row)),1,putch)
/* dfp */
#define	PLOT(A,B,C)	POS(A,B);putchar(C)
#define	SPLOT(A,B,S)	POS(A,B);(void)fprintf(stdout, "%s", S)
#define TMPF	"/usr/tmp/pacmanXXXXXX"
#define GAME1	'1'
#define GAME2	'2'
#define GAME3	'3'
#define GAME4	'4'
#define MAXSCORE	"/usr/games/lib/paclog"
#define MSSAVE	5	/* maximum scores saved per game type */
#define MGTYPE	4	/* Maximum game types */
#define	MAXPAC	3	/* maximum number of pacmen to start */
#define	MAXMONSTER	4	/* max number of monsters */
#define EMPTY	'E'
#define FULL	'F'
#define LEFT	'h'
#define RIGHT	'l'
#define	NORTH	'k'	/* means UP, but UP defined in vsinit() */
#define NNORTH	'w'
#define	DOWN	'j'
#define NDOWN	'x'
#define HALT	' '
#define DELETE	'\177'
#define ABORT	'\34'
#define QUIT	'q'
#define CNTLS	'\23'
#define BUF_SIZE	32
#define	TRUE	1
#define	FALSE	0
#define	UPINT	(-1)
#define	DOWNINT	1
#define	LEFTINT	(-2)
#define	RIGHTINT	2
#define	PACMAN	'@'
#define	MONSTER	'M'
#define	RUNNER	'S'
#define	TREASURE	'$'
#define	CHOICE		'*'
#define	GOLD		'+'
#define	POTION		'X'
#define	VACANT		' '	/* space */
#define	WALL		'O'
#define	GATE		'-'
#define	START	0
#define	RUN	1
#define	FAST	1
#define	SLOW	0
#define	PSTARTX	18
#define	PSTARTY	17
#define	MSTARTX	16	/* monster starting position */
#define	MSTARTY	10	/* monster starting position */
#define	MBEGINX	18	/* monster beginning position */
#define	MBEGINY	7	/* monster beginning position */
#define	TRYPOS	13
#define	TRXPOS	20
#define	GOTONE	1
#define	TURKEY	(-1)
#define	DUP	1
#define	DDOWN	4
#define	DRIGHT	3
#define	DLEFT	2
#define	DNULL	0
#define	BRDX	40
#define	BRDY	23
#define XWRAP	38
#define	TREASVAL	20
#define	KILLSCORE	10
#define	BEEP		''	/* ctrl-g */
#define	MSTARTINTVL	10
#define	POTINTVL	25
#define GOLDCNT	185
#define PUP	'^'
#define PDOWN	'v'
#define PLEFT	'<'
#define PRIGHT	'>'

struct	pac
{
	int	xpos;	/* horizontal position */
	int	ypos;	/* vertical   position */
	int	dirn;	/* direction of travel */
	int	speed;	/* FAST/SLOW	       */
	int	danger;	/* TRUE if can eat     */
	int	stat;	/* status		*/
};

#define nap(x)  
