#! /bin/sh -
# send mail to a user for the given job
# option -f: say this is the final report
exec 2>> %SDIR%/log

PATH=%LDIR%:/bin:/usr/bin:/usr/ucb:/usr/bsd
cd %SDIR%

if [ "x$1" = x-f ]
then
	shift
	final=1
else
	final=0
fi

if [ $# != 1 ]
then
	exit 1
fi

user=`ls -ld $1 | awk '{print $3}'`
host=`hostname`

trap 'rm -f /tmp/genmail.$$' 0 1 2 3
rm -f /tmp/genmail.$$

if [ $final = 0 ]
then
	echo current status report of dist job $host!$1
else
	echo final status report of dist job $host!$1
fi > /tmp/genmail.$$

while read num sys
do
	if [ -f $1/$num.mail ]
	then
		:
	else
		if [ -s $1/$num.done ]
		then
			echo $sys completed with errors:
			sed 's/^/	/' $1/$num.done
			touch $1/$num.mail
		elif [ -f $1/$num.done ]
		then
			echo $sys completed successfully.
			touch $1/$num.mail
		else
			echo $sys not completed.
		fi
	fi
done < $1/ctl >> /tmp/genmail.$$

mail $user < /tmp/genmail.$$
