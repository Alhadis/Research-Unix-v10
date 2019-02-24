#!/bin/sh
mach=`sed -n 2p /etc/backup`
b=`sed 1q /etc/backup`
id=$b/dbvolid
VOL=`tail -1 $id`	# set below

echo "backing up database for $mach:"
cd $b/bkp
DEV=${DEV:=`worm mount $VOL`}
if worm write -f$DEV $VOL *.[TFN]
then
	rm -f *.[TFN]
else
	if worm stat -f$DEV $VOL
	then
		awk '{ last = $1 }
		END	{
			match(last, "[0-9]+[ab]$")
			pre = substr(last, 1, RSTART-1)
			last = substr(last, RSTART, RLENGTH)
			x = substr(last, length(last), 1)
			if(x == "a") print pre last+0 "b"
			else print pre last+1 "a"
		}' < $id | sort -o $id	# lazy way to write on top
		VOL=`tail -1 $id`	# set above
		echo "allocated new volume: $VOL"
		exec $0
	fi
	echo "please mount $VOL in $mach's worm$DEV and as root, run"
	echo "	/usr/backup/bin/wormdb"
fi
