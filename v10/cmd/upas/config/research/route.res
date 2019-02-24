#!/bin/sh -p
PATH=/bin:/usr/bin:/usr/lib/upas; export PATH
sender=$1;shift
system=$1;shift
sys=`cat /etc/whoami`

# save the mail
cat > /tmp/ur$$

#
# Try smtp via datakit.  Mail to a!b!c from d!e!f will be
# transformed into mail to c@b from f%e@d
# 
netname=`qns value dk $system | egrep "astro|mercury|phone"`
case $netname in
"") ;;
*)  tosmtp -u $sys!$sender dk!$system!smtp $* < /tmp/ur$$ && rm -f /tmp/ur$$ && exit 0
    ;;
esac

# try queuing it for uucp
uux - -a "$sender" "$system!rmail" "($*)" < /tmp/ur$$ && rm -f /tmp/ur$$ && exit 0

# try queuing it for smtp
inetname=`qns value dom $system att,org`
case $inetname in
"")	rv=1
	;;
*)	/usr/lib/upas/smtpqer $sender $system $* < /tmp/ur$$
	rv=$?
	;;
esac

# nothing left to try, clean up
rm -f /tmp/ur$$
exit $rv
