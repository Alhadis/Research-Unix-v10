#include <stdio.h>
#include "typedef.h"
#include "basic.h"

#define MAXPLOT 	32	/* number of bytes in plot buffer */
#define SFLAG		035
#define EFLAG		0377
#define CLEAR		030
#define UP		0
#define DOWN		1

#define LIMIT		1024.	/* Screen limit */
#define XMAX		280
#define YMAX		192
#define XSCALE		(LIMIT/XMAX)
#define YSCALE		(LIMIT/YMAX)

static struct plotinfo {
	int		p_des;		/* plot file descriptor */
	char		*p_file;	/* the plot file name */
	char		p_buff[MAXPLOT];	/* the plot buffer */
	int		p_pen;		/* current pen position */
	char		*p_ptr; 	/* plot buffer pointer */
	int		p_x;		/* current x position */
	int		p_y;		/* current y position */
	}	p;


/*
 * pltini --- initialize the plot structure
 */

pltini(file)
char	*file;
{
	if (p.p_des > 0)
		pltcls();	/* close it if open */
	if (file == NULL)
		file = p.p_file;
	if (file == NULL)
		file = "!plot";
	if (file[0] == '!')
		p.p_des = pipeout("/bin/sh", "sh", "-c", &file[1], 0);
	else
		p.p_des = creat(file, 0666);
	if (p.p_des < 0)
		err("cannot open %s for plotting", file);
	p.p_ptr = p.p_buff;
	p.p_pen = UP;
	p.p_file = file;
}


/*
 * pltcls --- close the plot file
 */

pltcls()
{

	if (p.p_des > 0) {
		penup();
		pltflush();
		close(p.p_des);
		}
	p.p_des = 0;
}


/*
 * pltflush --- flush the plot buffer
 */

pltflush()
{
	register int	l;

	l = p.p_ptr - p.p_buff;
	if (l)
		if (write(p.p_des, p.p_buff, l) < 0)
			err("write error during plotting");
	p.p_ptr = p.p_buff;
}


/*
 * plot --- move the pen to the specified coordinates
 */

plot(x, y)
{
	register int	ix, iy;

	if (tflg)
		fprintf(stderr, "plot %d %d\n", x, y);
	if (x < 0 || x >= XMAX)
		err("invalid x co-ordinate %d", x);
	if (y < 0 || y >= YMAX)
		err("invalid y co-ordinate %d", y);
	if (p.p_pen == DOWN) {
		ix = x * XSCALE;
		iy = (YMAX - y) * YSCALE;
		plotc((ix >> 6) & 077);
		plotc(ix & 077);
		plotc((iy >> 6) & 077);
		plotc(iy & 077);
		}
	p.p_x = x;
	p.p_y = y;
}


/*
 * plotc --- enter a character into the plot buffer
 */

plotc(ch)
{

	*p.p_ptr++ = ch;
	if (p.p_ptr >= &p.p_buff[MAXPLOT])
		pltflush();
}


/*
 * penup --- raise the pen
 */

penup()
{

	if (p.p_pen == DOWN)
		plotc(EFLAG);
	p.p_pen = UP;
	pltflush();
}


/*
 * pendn --- lower the pen
 */

pendn()
{

	if (p.p_pen == UP) {
		plotc(SFLAG);
		p.p_pen = DOWN;
		plot(p.p_x, p.p_y);
		}
}


/*
 * erase --- erase the plot display
 */

erase()
{

	penup();
	plotc(CLEAR);
	plotc(EFLAG);
	pltflush();
}
