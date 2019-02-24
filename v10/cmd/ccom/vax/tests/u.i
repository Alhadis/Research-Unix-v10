typedef struct seq {
	short cnt, size;
	struct seqd {
		char *p;
		int beg, end;
	} *data;
} seq;
main(s)
seq *s;
{
	foo((s->cnt *= 2) * sizeof(struct seq));
}
