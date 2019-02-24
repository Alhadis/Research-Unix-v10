#!/bin/sh
awk "BEGIN	{ target = \"$1\""'
		sub("^v", "", target)
		sub("/.*", "", target)
		t = target
		sub("[0123456789]*", "", t)
		if(length(t) != 0){
			printf "argument %s must be numeric\n", target >"/dev/stderr"
			exit 1
		}
	}
	{ if(target < $2) exit 0; last = $1 }
END	{ print last }' < `sed 1q /etc/backup`/volidmap
