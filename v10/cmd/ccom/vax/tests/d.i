struct S {
	int (*f)();
};
main()
{	struct S *s;
	(*s->f)(s);
	if((*s->f)(s) == 1);
}

