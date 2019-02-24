#!/bin/sh -p
#
#	test authorization server
#
#	- read a user name
#	- generate and send a challenge string
#	- read response
#	- send YES\n or NO\n
#

PATH=/usr/net:/bin:/usr/bin;	export PATH

read user
makechal "$user"
read response
checkauth "$user" "$makechal" "$response"
case $? in
0)	echo "YES";;
*)	echo "NO";;
esac
exit

