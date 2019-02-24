struct a {int a;} a, b, c;
struct a xx();
main()
{	int i;
	foo1((a, a = xx()));
	foo2((a, a));
	foo3(a);
	foo4(i *= 2,i *= 3);
	foo5(&xx());
}
