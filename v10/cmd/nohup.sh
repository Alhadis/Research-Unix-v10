trap "" 1 15
X=""
if [ -t 1 ]
then	X=1
fi
if [ -t 2 ]
then	X=${X}2
fi

case "$X" in

12)	echo "nohup: sending output to 'nohup.out'"
	exec nice -5 "$@" >>nohup.out 2>&1 ;;

2)	echo "nohup: sending std error to 'nohup.out'" 1>&2
	exec nice -5 "$@" 2>>nohup.out ;;

1)	echo "nohup: sending output to 'nohup.out'"
	exec nice -5 "$@" >>nohup.out ;;

"")	exec nice -5 "$@" ;;

esac
