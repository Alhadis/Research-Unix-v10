#!/bin/sh -p
PATH=/bin:/usr/bin:/usr/lib/upas
sender=$1;shift
system=$1;shift
relay=

case $system in
*.wisc.edu)
	relay=wi/wisc/cs/pokey
	;;
*.uiuc.edu)
	relay=il/uiuc/cs/gateway
	;;
*.berkeley.edu)
	relay=ca/ucb/cs/ucbarpa
	;;
*)
	exit 1
	;;
esac

smtpqer -a dk!$relay!smtp -d .att.com $sender $system $*
