#!/bin/sh
x=`sed 1q /etc/backup`/volidmap
awk 'BEGIN	{ vol="'$1'"; vol = substr(vol, 2, length(vol)-1) }
	{ last = $1 }
END	{ a = match(last, "[0-9]+[ab]$")
	head = substr(last, 1, a-1)
	num = substr(last, a, length(last)-a)
	x = substr(last, length(last), 1)
	if(x == "a") print head num "b" "\t" vol
	else print head num+1 "a" "\t" vol }' < $x >> $x
