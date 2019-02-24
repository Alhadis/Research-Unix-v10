/*
 *	       A R P  C O N F I G
 *
 * Ethernet address resolution setup procedure.
 * For use with the Chaosnet Line Discipline and/or TCP/IP.
 * Invoked from 'netconfig' with fd 0 open to an ethernet channel.
 *
 *
 * (c) Copyright 1985  Nirvonics, Inc.
 *
 * Written by Kurt Gollhardt
 * Last update Thu Mar 14 13:24:01 1985
 *
 * This software is the property of Nirvonics, Inc.
 * All rights reserved.
 *
 */

#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/order.h>
#include <sys/ethernet.h>

extern int arp_ld;
int arp_type;


main(ac,av)
     char **av;
{
     unsigned char  en_addr[6];
     int       i;

     if (ioctl(0, FIOPUSHLD, &arp_ld) < 0) {
	  fprintf(stderr, "Can't push arp line discipline\n");
	  exit(1);
     }
     arp_type = hfirst_short(ETHERPUP_ARPTYPE);
     if (ioctl(0, ENIOTYPE, &arp_type) < 0) {
	  fprintf(stderr, "Can't set arp type\n");
	  exit(1);
     }

     if (ioctl(0, ENIOADDR, en_addr) < 0) {
	  fprintf(stderr, "Can't get ethernet address\n");
	  exit(1);
     }

     printf("%s: running on ethernet", av[0]);
     for (i = 0; i < 6; i++)
	  printf(" %02x", en_addr[i]);
     putchar('\n');
     fflush(stdout);

     pause();
}
