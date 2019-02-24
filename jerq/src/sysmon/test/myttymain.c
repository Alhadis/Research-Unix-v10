main()
{
	char *tty;
	char *mytty();

	tty=mytty();
	if(tty==0)
		printf("no tty\n");
	else
		printf("%s\n", tty);
}
