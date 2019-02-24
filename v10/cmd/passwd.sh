#!/bin/sh -p
PATH=/bin:/usr/bin
export PATH
#	invoke normal passwd command with argument
#
	/bin/passwdx $*
#	test if successful and if we should continue
case $? in
0) ;;
*) exit 1 ;;
esac
#	test if user updating entry in /etc/passwd
case $1 in
-n)
	case $# in
	1)	linex1=`tail -1 /etc/passwd` ;;
	2)	linex1=`grep ^$2: /etc/passwd` ;;
	esac
	userx1=`echo $linex1 | awk -F: '{print $1}'`
	#
	#	update user list on machine
	#
	echo -n "full name: "
	read namex1
	echo -n "dept no: "
	read deptx1
	echo -n "sponsor: "
	read sponsx1
	egrep -v "^$userx1[ 	]" /usr/adm/usrlist > /tmp/p$$
	echo "$userx1	$deptx1	$namex1 $sponsx1" >>/tmp/p$$
	sort -o /usr/adm/usrlist /tmp/p$$
	rm /tmp/p$$
	;;
-a)
	case $# in
	1)	userx1=`id | sed 's/.*(\(.*\)) g.*/\1/'`
		linex1=`grep ^$userx1: /etc/passwd` ;;
	2)	linex1=`grep ^$2: /etc/passwd` ;;
	esac
	;;
*) exit 0 ;;
esac
echo -n "mail address: "
read mailx1
#
#	update mailing forwardlist for user
#
mail postmaster <<EOF
account = $linex1 
name = $namex1
address = $mailx1
dept = $deptx1
sponsor = $sponsx1
EOF
#
