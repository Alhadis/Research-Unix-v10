/** Coco (communicating coroutines) message passing package **/

struct mqueue {
	int mtype;
	struct mqueue *mnext;	/* qlength zero if null */
};

struct mproc {
	struct mqueue *stdin;
	struct mqueue *stdout;
	int (*f)();
	Rectangle r;	/* coordinates initial window */
};

send (to, what) {}
recv (what) int *what; {}

main()
{
	starters();
	switch (recv()) {
	LINE : getline(Gity, Gitres); break;
	RECT : Getrect(3, &Gitrec); break;
	WRITE: writefile(); break;
	FLUSH: flushost(); break;
	QUIT : quit(); break;
	}
}
