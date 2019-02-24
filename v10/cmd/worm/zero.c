char block[1024];

main()
{
	lseek(1, 0L, 0);
	write(1, block, sizeof block);
	exit(0);
}
