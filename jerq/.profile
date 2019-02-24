if [ "$REXEC" != 1 ]
then
	stty erase "^h" tabs
fi
PS1=`cat /etc/whoami`'$ '
PATH=:/usr/jerq/bin:/usr/andrew/bin:/usr/rob/bin:/bin:/usr/bin
CDPATH=:$HOME
HISTORY=/tmp/jerq.$$
> $HISTORY
QEDFILE=/usr/rob/qed/q/qfile
TERM=5620
export PS1 QEDFILE PATH TERM CDPATH HISTORY
umask 0
