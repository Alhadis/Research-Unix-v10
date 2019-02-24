	if [ `cat $HISTFILE | lct` -gt "$HISTMAXL" ]

for i in `ls [0-9]*# | egrep '^[0-9]+##?$' | sed -e 's/#*$//'`

do case "`ps -lx$i" in ?*);; *) rm -f ${i}# ${i}##;; esac

NBRFILES=`ls -f $pubdir/jbk | fgrep -vi -x '.

..'|lct`
