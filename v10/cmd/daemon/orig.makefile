dpd:	spider.dpd.c daemon.c daemon0.c /usr/include/gcos.c
	cc -n -s -O spider.dpd.c -o dpd

odpd:	phone.dpd.c daemon.c daemon0.c /usr/include/gcos.c /usr/lib/libg.a
	cc -n -s -O phone.dpd.c -lg -o odpd

lpd:	lpd.c daemon.c daemon0.c
	cc -n -s -O lpd.c -o lpd

fget.demon: spider.fgd.c daemon.c daemon0.c /usr/include/gcos.c
	cc -n -s -O spider.fgd.c -o fget.demon

fget.odemon: phone.fgd.c daemon.c daemon0.c /usr/include/gcos.c
	cc -n -s -O phone.fgd.c -lg -o fget.odemon
