LIB=/usr/cda/lib
BIN=/usr/cda/bin
DPD=
SPOOL=/usr/spool/dpd
export LIB BIN SPOOL
PATH=/usr/cda/bin:$PATH
export PATH
case $# in
0)	;;
*)	"$@";;
esac
