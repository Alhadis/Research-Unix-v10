#!/bin/sh
umask 077
whoami=`uuname -l`
logname=`who am i | sed 's/ .*//'`
export whoami logname
T=/usr/tmp/asd$$
trap 'rm -f $T.[12]; exit 0' 1 2 3
if mkpkg "$@" >$T.1
then    seal -k $T.1 >$T.2
        rm -f $T.1
        (trap '' 1 2 3
        exec >/dev/null </dev/null 2>/dev/null 3>/dev/null
        for i in ${dest:=`echo $whoami | comm -23 /etc/asd/machines -`}
        do /usr/bin/uux - -r "$i!asdrcv ${source:=$whoami $logname}" <$T.2
           /usr/lib/uucp/uucico -r1 -s$i
        done
        rm -f $T.2)&
else    rm -f $T.[12]
fi
