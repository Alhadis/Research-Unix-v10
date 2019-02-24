/*
 *	       N E T  C O N F I G
 *
 * Ethernet network setup procedure.
 *
 * Usage: netconfig device unit { protocol }
 *
 *	  where	    device is the ethernet device type (ec, il, etc.)
 *		    unit is the physical unit number of the device
 *		    protocol is a protocol name (ip, chaos, arp, etc.),
 *			 optionally followed by protocol-specific arguments
 *
 *
 * (c) Copyright 1985  Nirvonics, Inc.
 *
 * Written by Kurt Gollhardt
 * Last update Thu Mar 14 11:40:10 1985
 *
 * This software is the property of Nirvonics, Inc.
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ethernet.h>

#define CONFIG_DIR  "/usr/ether"

extern int dump_ld;

#define MAX_ARGS 10

char *argv[MAX_ARGS];


main(ac,av)
     char **av;
{
     int       fd, unit, min_minor, max_minor, c, usage, dumping;
     char      *dev, devfile[40], prog[40], name[40];
     extern int	    optind;
     extern char    *optarg;

     while ((c = getopt(ac, av, "d")) != EOF)
	  switch (c) {
	       case 'd':
		    dumping++;
		    break;
	       case '?':
		    usage++;
		    break;
	  }

     if (ac - optind < 3)
	  usage++;

     if (usage) {
	  fprintf(stderr,
	       "Usage: netconfig [-options] ether_device unit {protocol}\n",
	       av[0]);
	  fprintf(stderr,
	       "Options: -d: insert a dump line-discipline\n");
	  exit(1);
     }

     dev = av[optind++];
     if (strncmp(dev, "/dev/", 5) == 0)
	  dev += 5;
     sprintf(devfile, "/dev/%s", dev);
     unit = atoi(av[optind++]);
     min_minor = unit * CHANS_PER_UNIT;
     max_minor = min_minor + CHANS_PER_UNIT - 1;

     signal(SIGHUP, SIG_IGN);

     while (optind < ac) {
	  if ((fd = findslot(devfile, min_minor, max_minor, 2)) < 0) {
	       fprintf(stderr,
			 "Can't open %s unit %d for address resolution\n",
			 devfile, unit);
	       exit(1);
	  }
	  dup2(fd, 0);
	  close(fd);

	  if (dumping && ioctl(0, FIOPUSHLD, &dump_ld) < 0)
		    fprintf(stderr, "Can't push dump line discipline\n");

	  makeargv(av[optind++]);
	  sprintf(prog, "%s/%sconfig", CONFIG_DIR, argv[0]);
	  sprintf(name, "%s %s%d", argv[0], dev, unit);
	  argv[0] = name;

	  switch (fork()) {
	       case -1:
		    fprintf(stderr, "netconfig: fork failed\n");
		    exit(1);
	       case 0:
		    break;
	       default:	 /* child */
		    execv(prog, argv);
		    fprintf(stderr, "%s: exec failed\n", prog);
		    _exit(1);
	  }
     }
}


makeargv(s)
     char *s;
{
     register int   argc;

     for (argc = 0; *s && argc < MAX_ARGS-1; ++argc) {
	  argv[argc] = s;
	  while (*s && !isspace(*s))
	       ++s;
	  if (*s)
	       *s++ = '\0';
	  while (*s && isspace(*s))
	       ++s;
     }

     argv[argc] = 0;
}
