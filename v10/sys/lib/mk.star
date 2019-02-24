LD=../lib
LIBS=$LD/${cc}asstar.o $LD/${cc}fs.a $LD/${cc}io.a $LD/${cc}star.a $LD/${cc}bvax.a $LD/${cc}os.a $LD/${cc}vm.a $LD/${cc}inet.a
LOW=../ml/param.s ../ml/logen.s
MKDEP=$LD/devs $LD/tab $LD/conf.star $LD/low.star
CFLAGS=-DKERNEL -I..

%.u: %.l.o %.c.o $LIBS
	date=`date | sed 's/....\(............\)........\(....\)/\1 \2/'`
	echo 'char version[] = "Unix 10e '$date'";' >vers.c
	${cc:-cc} -c vers.c
	ld -n -X -o $target -T 80000000 -e start $prereq vers.o

%.l.o: $LOW %.s.s
	as -o $target $prereq

%.s.s:N: %.c.c

%.c.c: $MKDEP %.m
	$LD/mkconf -t $LD/tab -l $LD/low.star -d $LD/devs -s $stem.s.s -c $stem.c.c $LD/conf.star $stem.m

%.c.o: %.c.hn
%.hn:Pexit 1: %.c
	$LD/hupdate $target `$LD/listdep $CFLAGS $prereq`
