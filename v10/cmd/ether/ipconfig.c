/*
 *	       I P  C O N F I G
 *
 * Ethernet IP protocol setup procedure.
 * Invoked from 'netconfig' with fd 0 open to an ethernet channel.
 *	  Args: my-name his-name
 *
 *
 * (c) Copyright 1985  Nirvonics, Inc.
 *
 * Written by Kurt Gollhardt
 * Last update Mon Mar 18 16:33:11 1985
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

extern int ip_ld;
int ip_type;


main(ac,av)
     char **av;
{
     char *me, *it;
     unsigned long  myaddr, hisaddr;

     if (ioctl(0, FIOPUSHLD, &ip_ld) < 0) {
	  fprintf(stderr, "Can't push ip line discipline\n");
	  exit(1);
     }
     ip_type = hfirst_short(ETHERPUP_IPTYPE);
     if (ioctl(0, ENIOTYPE, &ip_type) < 0) {
	  fprintf(stderr, "%s: Can't set ip type\n", av[0]);
	  exit(1);
     }

     if (ac != 3) {
	  fprintf(stderr, "%s: args: my-name his-name\n", av[0]);
	  exit(1);
     }
     me = av[1];
     it = av[2];

     myaddr = tcp_addr(me);
     if(myaddr == 0){
          fprintf(stderr, "ipconfig: unknown host %s\n", me);
          exit(1);
     }
     hisaddr = tcp_addr(it);
     if(hisaddr == 0){
          fprintf(stderr, "ipconfig: unknown host/net %s\n", it);
          exit(1);
     }

     if (ioctl(0, IPIOLOCAL, &myaddr) < 0) {
	  fprintf(stderr, "%s: ", av[0]);
	  perror("IPIOLOCAL");
	  exit(1);
     }
     if (hisaddr & 0xff)
	  ioctl(0, IPIOHOST, &hisaddr);
     else
	  ioctl(0, IPIONET, &hisaddr);
     if (ioctl(0, IPIOARP, 0) < 0) {
	  fprintf(stderr, "%s: ", av[0]);
	  perror("IPIOARP");
	  exit(1);
     }

     printf("%s: installed as %s on %s\n", av[0], me, it);
     fflush(stdout);

     pause();
}
