# 		C I P

# Test if download is attempted in first window by extracting last digit
# in tty device number. 
TTY=`tty`
let TTYLAST="`echo $TTY | wc -c`-1"
TTYDIG=`echo $TTY | cut -c$TTYLAST`

if 
	test "$TERM" = "630"
then
	if [ $TTYDIG = 1 ]
	then
		echo "$0 must not be downloaded in first window"
		exit 1
	fi

	jx cip.m ${DMD}/font $*

else
	if [ $TTYDIG = 2 ]
	then
		echo "$0 must not be downloaded in first window"
		exit 1
	fi

	if ismpx -
	then	jx cip.m ${DMD}/font
	else	echo "$0 not available in standalone mode - use layers"
	fi
fi
