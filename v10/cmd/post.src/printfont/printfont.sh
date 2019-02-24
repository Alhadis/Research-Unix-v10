#
# Formatted dump of encoded characters in one or more PostScript fonts.
# Arguments should be PostScript font names or the word all, which dumps
# all ROM and disk based fonts.
#

POSTLIB=/usr/lib/postscript
PROLOGUE=$POSTLIB/printfont.ps

OPTIONS=
COPYFILE=
MODE=portrait
FONTENCODING=Default

NONCONFORMING="%!PS"
ENDPROLOG="%%EndProlog"
BEGINSETUP="%%BeginSetup"
ENDSETUP="%%EndSetup"
TRAILER="%%Trailer"

SETUP=setup

while [ -n "$1" ]; do
    case $1 in
	-a)  shift; OPTIONS="$OPTIONS /axescount $1 def";;
	-a*) OPTIONS="$OPTIONS /axescount `echo $1 | sed s/-a//` def";;

	-b)  shift; OPTIONS="$OPTIONS /radix $1 def";;
	-b*) OPTIONS="$OPTIONS /radix `echo $1 | sed s/-b//` def";;

	-c)  shift; OPTIONS="$OPTIONS /#copies $1 store";;
	-c*) OPTIONS="$OPTIONS /#copies `echo $1 | sed s/-c//` store";;

	-f)  shift; OPTIONS="$OPTIONS /labelfont /$1 def";;
	-f*) OPTIONS="$OPTIONS /labelfont /`echo $1 | sed s/-f//` def";;

	-g)  shift; OPTIONS="$OPTIONS /graynotdef $1 def";;
	-g*) OPTIONS="$OPTIONS /graynotdef `echo $1 | sed s/-g//` def";;

	-p)  shift; MODE=$1;;
	-p*) MODE=`echo $1 | sed s/-p//`;;

	-q)  OPTIONS="$OPTIONS /longnames false def /charwidth false def";;

	-m)  shift; OPTIONS="$OPTIONS /magnification $1 def";;
	-m*) OPTIONS="$OPTIONS /magnification `echo $1 | sed s/-m//` def";;

	-v)  OPTIONS="$OPTIONS /longnames true def /charwidth true def";;

	-w)  shift; OPTIONS="$OPTIONS /linewidth $1 def";;
	-w*) OPTIONS="$OPTIONS /linewidth `echo $1 | sed s/-w//` def";;

	-x)  shift; OPTIONS="$OPTIONS /xoffset $1 def";;
	-x*) OPTIONS="$OPTIONS /xoffset `echo $1 | sed s/-x//` def";;

	-y)  shift; OPTIONS="$OPTIONS /yoffset $1 def";;
	-y*) OPTIONS="$OPTIONS /yoffset `echo $1 | sed s/-y//` def";;

	-z)  shift; OPTIONS="$OPTIONS /zerocell $1 def";;
	-z*) OPTIONS="$OPTIONS /zerocell `echo $1 | sed s/-z//` def";;

	-C)  shift; COPYFILE="$COPYFILE $1";;
	-C*) COPYFILE="$COPYFILE `echo $1 | sed s/-C//`";;

	-E)  shift; FONTENCODING=$1;;
	-E*) FONTENCODING=`echo $1 | sed s/-E//`;;

	-L)  shift; PROLOGUE=$1;;
	-L*) PROLOGUE=`echo $1 | sed s/-L//`;;

	-*)  echo "$0: illegal option $1" >&2; exit 1;;

	*)   break;;
    esac
    shift
done

case "$MODE" in
    l*) OPTIONS="$OPTIONS /landscape true def";;
    *)	OPTIONS="$OPTIONS /landscape false def";;
esac

echo $NONCONFORMING
cat $PROLOGUE
echo $ENDPROLOG
echo $BEGINSETUP
cat ${COPYFILE:-/dev/null}
echo $OPTIONS

case "$FONTENCODING" in
    /*) cat $FONTENCODING;;
    ?*) cat ${POSTLIB}/${FONTENCODING}.enc 2>/dev/null
esac

echo $SETUP
echo $ENDSETUP

for i do
    case "$i" in
	all) echo AllFonts;;
	/*)  echo "$i PrintFont";;
	?*)  echo "/$i PrintFont";;
    esac
done

echo $TRAILER

