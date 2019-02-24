int *_mcbase;
int _setmatexit = 1;

_matexit()
{
	monitor((char *)0, (char *)0, (int *)0, 0, 0);
}

monitor(lowpc, highpc, buf, bufsiz, cntsiz)
char *lowpc, *highpc;
int *buf, bufsiz;
{
	register o;
	static *sbuf, ssiz;
	struct phdr {
		int *lpc;
		int *hpc;
		int ncnt;
	};
	struct cnt {
		int *pc;
		long ncall;
	};

	if (lowpc == 0) {
		profil((short *)0, 0, 0, 0);
		o = creat("mon.out", 0666);
		write(o, sbuf, ssiz);
		close(o);
		return;
	}
	if(_setmatexit){
		_setmatexit = 0;
		atexit(_matexit);
	}
	sbuf = buf;
	ssiz = bufsiz;
	buf[0] = (int)lowpc;
	buf[1] = (int)highpc;
	buf[2] = cntsiz;
	_mcbase = (int *) (((int)buf) + sizeof(struct phdr));
	buf = (int *) (((int)_mcbase) + (o = cntsiz*sizeof(struct cnt)));
	bufsiz -= o + sizeof(struct phdr);
	memset((char *)_mcbase, 0, o);	/* clear counts */
	if (bufsiz<=0)
		return;
	o = bufsiz*65536.0/(highpc - lowpc);
	profil(buf, bufsiz, lowpc, o);
}
