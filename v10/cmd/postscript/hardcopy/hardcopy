#
# Generate paper output from the data that a PostScript program normally
# sends back to a host computer using file output operators.
#

POSTLIB=/usr/lib/postscript
PROLOGUE=$POSTLIB/hardcopy.ps

OPTIONS=
MODE=portrait

NONCONFORMING="%!PS"
ENDPROLOG="%%EndProlog"
BEGINSETUP="%%BeginSetup"
ENDSETUP="%%EndSetup"
TRAILER="%%Trailer"

SETUP=HardcopySetup
DONE="(%stdout)(w) file -1 write"

while [ -n "$1" ]; do
    case $1 in
	-c)  shift; OPTIONS="$OPTIONS /#copies $1 store";;
	-c*) OPTIONS="$OPTIONS /#copies `echo $1 | sed s/-c//` store";;

	-f)  shift; OPTIONS="$OPTIONS /font /$1 def";;
	-f*) OPTIONS="$OPTIONS /font /`echo $1 | sed s/-f//` def";;

	-p)  shift; MODE=$1;;
	-p*) MODE=`echo $1 | sed s/-p//`;;

	-m)  shift; OPTIONS="$OPTIONS /magnification $1 def";;
	-m*) OPTIONS="$OPTIONS /magnification `echo $1 | sed s/-m//` def";;

	-s)  shift; OPTIONS="$OPTIONS /pointsize $1 def";;
	-s*) OPTIONS="$OPTIONS /pointsize `echo $1 | sed s/-s//` def";;

	-x)  shift; OPTIONS="$OPTIONS /xoffset $1 def";;
	-x*) OPTIONS="$OPTIONS /xoffset `echo $1 | sed s/-x//` def";;

	-y)  shift; OPTIONS="$OPTIONS /yoffset $1 def";;
	-y*) OPTIONS="$OPTIONS /yoffset `echo $1 | sed s/-y//` def";;

	-L)  shift; PROLOGUE=$1;;
	-L*) PROLOGUE=`echo $1 | sed s/-L//`;;

	--)  shift; break;;

	-*)  echo "$0: illegal option $1" >&2; exit 1;;

	*)   break;;
    esac
    shift
done

case "$MODE" in
    l*) OPTIONS="$OPTIONS /landscape true def";;
    *)  OPTIONS="$OPTIONS /landscape false def";;
esac

echo $NONCONFORMING
cat $PROLOGUE
echo $ENDPROLOG
echo $BEGINSETUP
echo $OPTIONS
echo $SETUP
echo $ENDSETUP

cat $*

echo $TRAILER
echo $DONE

