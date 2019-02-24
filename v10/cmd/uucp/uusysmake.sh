#! /bin/sh
ln -s /tmp/LCK.uucp.Sys /tmp/LCK.uucp.Sys || exit
cd /usr/lib/uucp
cbt creat Bsystems
cat `cat Sysfiles` | awk '
BEGIN	{
		alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		OFS = "	"
	}

!/^#/	{
		sys[$1]++
		printf("%s.%s	%s\n", $1, substr(alph, sys[$1], 1), $0)
	}
' | sort | cbt build Bsystems
rm -f /tmp/LCK.uucp.Sys
