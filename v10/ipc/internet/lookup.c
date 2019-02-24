main(ac, av)
	int ac;
	char *av[];
{
	if(strchr(av[1], '.')==0){
		printf("%s == %s\n", av[1], in_ntoa(in_address(av[1])));
	} else {
		printf("%s == %s\n", av[1], in_host(in_address(av[1])));
	}
}
