/*
 * #!/bin/sh
 * #
 * # prt substitute - seems to work, but you have a bit more to do. My
 * # previous mail about the header (I deleted it) should describe other
 * # interesting header fields.
 * # usage: prt [-B] [-b bin] [-c copies] [-d dest] [-f form] [-h] [-j jobname]
 * # 		[-K "KEYWORD value"] [-l language] [-m] [-o "filter options"]
 * # 		[-p printmode] [-q] [-s sides] [-u userid] [-z] [-] [filename]

 * 
 * TMPFILE=/tmp/prt.$$
 * SYSTEM=mhuxo
 * REQUESTQ=/usr/add-on/unison/requestq
 * 
 * CMDUSED=`basename $0`
 * CMDVERNUM=3.1
 * TCMDISSUED=910619105115		# `date -s`
 * ACCTNUM=mh0000			# need something here.
 * BIN=m100
 * COPIES=1
 * DEPARTMENT=11211
 * DESTINATION=mh/kodak
 * FORM=ANY
 * MODE=ANY
 * LANGUAGE=postscript
 * ORIGSYS=`hostname`
 * SIDES=1
 * USERSYS=`hostname`
 * USERID=`set \`who am i 2>/dev/null\`;echo $1`
 * USERNAME=$USERID
 * CMDLINE=$0
 * 
 * trap "rm -f $TMPFILE $TMPFILE.in" 0 1 2 3 15
 * 
 * MakeID() {
 * 	#
 * 	# Think normal prt id's are 4 characters followed by at most 8
 * 	# chars of the system name - all apparently uncensored.
 * 	#
 * 	awk 'BEGIN{printf("%.4x%.8s", '$$', "'$USERSYS'")}' </dev/null
 * #	echo 7r4tsirius
 * }
 * 
 * Send() {
 * 	cat $1 > $TMPFILE.in
 * 	TOTALBYTES=`cat $TMPFILE.in | wc -c`
 * 	REQID=`MakeID`
 * 	case "$1" in
 * 	-) FILENAME=stdin;;
 * 	*) FILENAME=$1;;
 * 	esac
 * 
 * 	cat >$TMPFILE <<//EndHeader
 * @BYTECOUNT	0000000000
 * @DATA HEADER BEGIN
 * REQID		$REQID
 * CMDUSED		$CMDUSED
 * CMDVERNUM	$CMDVERNUM
 * TCMDISSUED	$TCMDISSUED
 * ACCTNUM		$ACCTNUM
 * BIN		$BIN
 * COPIES		$COPIES
 * DEPARTMENT	$DEPARTMENT
 * DEST		${DEST:-mh}
 * DEVICE		$DEVICE
 * DEVICEGROUP	$DEVICEGROUP
 * FILENAME	$FILENAME
 * FILTER		
 * FILTEROPTS	
 * FORM		$FORM
 * RGRADE		s
 * JOBNAME		
 * $KOPTS
 * LANGUAGE	$LANGUAGE
 * MAIL		y
 * MODE		$MODE
 * ORIGSYS		$ORIGSYS
 * SIDES		$SIDES
 * TOTALBYTES	$TOTALBYTES
 * USERDATA	y
 * USERSSYS	$USERSYS
 * USERID		$USERID
 * USERNAME	$USERNAME
 * CMDLINE		$CMDUSED -m -d$DESTINATION -l$LANGUAGE
 * @DATA HEADER END
 * //EndHeader
 * 
 * 	cat $TMPFILE.in >>$TMPFILE
 * 	ed $TMPFILE >/dev/null <<//EndEdit
 * /^@BYTECOUNT	00*$/s/00*/`ls -log $TMPFILE|awk '{printf("%10d\n", $5)}'`/
 * w
 * q
 * //EndEdit
 * 	chmod ugo+r $TMPFILE		# probably unnecessary on your systems
 * 	uucp -C $TMPFILE $SYSTEM!$REQUESTQ/$REQID
 * 	echo request id $REQID on $SYSTEM
 * }
 * 
 * for i in "$@"
 * do
 *     case "$i" in
 * 	-a*) ACCTNUM=`echo $i | sed s/-a//`;;
 * 	-b*) BIN=`echo $i | sed s/-b//`;;
 * 	-c*) COPIES=`echo $i | sed s/-c//`;;
 * 	-d*) DESTINATION=`echo $i | sed s/-d//`;;
 * 	-f*) FORM=`echo $i | sed s/-f//`;;
 * 	-K*) case "$KOPTS" in
 * 	     "") KOPTS=`eval echo $i | sed s/-K//`;;
 * 	      *) KOPTS="$KOPTS
 * 		`echo $i | sed s/-K//`";;
 * 	     esac;;
 * 	-l*) LANGUAGE=`echo $i | sed s/-l//`;;
 * 	-s*) SIDES=`echo $i | sed s/-s//`;;
 * 	-u*) USERNAME=`echo $i | sed s/-u//`;;
 * 	-p*) MODE=`echo $i | sed s/-p//`;;
 * 	--)  shift; break;;
 * 	-*)  echo "$0: bad option $i"; exit 1;;
 * 	*)   break;;
 *     esac
 *     shift
 * done
 * 
 * DEST=`echo $DESTINATION | sed -n 's/\/.*//p'`
 * DEST=${DEST:-mh}
 * DEVICE=`basename $DESTINATION`
 * DEVICEGROUP=$DEVICE
 * 
 * for i in ${*:--}
 * do
 * 	Send $i
 * done
 */

main(int argc, char *argv[]) {
	
}
