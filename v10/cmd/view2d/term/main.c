#include	<CC/jerq.h>
#include	"frame.pri"
#include	"clock.pub"

Movie *m;
Clock *clock;
extern char *genfn(int);
extern void sprintf(char *s ...);
extern int getn();
Menu menu = { (char **)0, &genfn };

main()
{
	int n, done;

	request(RCV|SEND|MOUSE|KBD);
	clock = new Clock();
	m = new Movie();
	m->read();

	done = 0;
	while(done == 0)
	{
		if(own()&MOUSE)
		{
			if(button1())
				if(hitpot(1))
				{
					m->adjust();
					m->show();
				}
			if(button2())
				helppot(2);
			if(button3())
				switch(n = menuhit(&menu, 3))
				{
				case -1:
					break;
				case 1:
					done = confirm(3);
					break;
				default:
					n -= 2;
					m->showframe(n);
					break;
				}
		}
		if(m->run->val)
		{
			clock->set();
			m->show();
			m->advance();
			wait(CPU);
			clock->synch();
		}
		else
			sleep(2);
	} 
	sendnchars(2, "x\n");
	sleep(500);
}

char buf[128];

char *
genfn(int i)
{
	switch(i)
	{
	case 0:
		sprintf(buf, "%d frames t=%d", m->maxframe, m->maxtime);
		break;
	case 1:
		sprintf(buf, "exit");
		break;
	default:
		i -= 2;
		if(i >= m->maxframe)
			return((char *)0);
		else
			sprintf(buf, "frame %d t=%s", i, m->ft(i));
		break;
	}
	return(buf);
}

extern int
nrand(int n)
{
	extern rand();

	return(muldiv(rand(), n, 32767));
}

getch()
{
	register i;

	if((i = rcvchar()) == -1)
	{
		wait(RCV);
		i = rcvchar();
	}
	return(i);
}

getn()
{
	register i, j;

	i = getch();
	i <<= 8;
	j = 0xFF & getch();
	return((i|j)&0xffff);
}

extern void
gets(register char *s)
{
	while(*s++ = getch());
}
