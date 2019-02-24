#! /bin/sh -
# send mail to a user for the given job
# option -f: say this is the final report
exec 2>> %SDIR%/log

PATH=%LDIR%:/bin:/usr/bin
cd %SDIR%

case "$1" in
-f) final=1; shift;;
*) final=0;;
esac

test "$1" || exit 1

user=`ls -ld $1 | awk '{print $3}'`
host=`hostname`

trap 'rm -f /tmp/genmail.$$' 0 1 2 3
rm -f /tmp/genmail.$$

case $final in
0) echo current status report of dist job $host!$1 '(' `cat $1/files` ')';;
1) echo final status report of dist job $host!$1 '(' `cat $1/files` ')';;
esac > /tmp/genmail.$$

while read num sys
do
	test -f $1/$num.mail || {
		if [ -s $1/$num.done ]
		then
			echo $sys completed with output:
			sed 's/^/	/' $1/$num.done
			touch $1/$num.mail
		elif [ -f $1/$num.done ]
		then
			echo $sys completed with no output.
			touch $1/$num.mail
		else
			echo $sys not completed.
		fi
	}
done < $1/ctl >> /tmp/genmail.$$

mail $user < /tmp/genmail.$$
