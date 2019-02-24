#
# Print a listing of an object, often a dictionary or an array. Something
# like ==, but the output is often easier to read and closer to PostScript
# that can be sent back through the interpreter.
#

POSTLIB=/usr/lib/postscript
PROLOGUE=$POSTLIB/grabit.ps

COPYFILE=
RECURSIVE=true
OMITNAMES="/Grabit /GrabitDict"

NONCONFORMING="%!PS"
ENDPROLOG="%%EndProlog"
BEGINSETUP="%%BeginSetup"
ENDSETUP="%%EndSetup"
TRAILER="%%Trailer"

SETUP=GrabitSetup

while [ -n "$1" ]; do
    case $1 in
	-d)  RECURSIVE=false;;

	-o)  shift; OMITNAMES="$OMITNAMES $1";;
	-o*) OMITNAMES="$OMITNAMES `echo $1 | sed s/-o//`";;

	-C)  shift; COPYFILE="$COPYFILE $1";;
	-C*) COPYFILE="$COPYFILE `echo $1 | sed s/-C//`";;

	-L)  shift; PROLOGUE=$1;;
	-L*) PROLOGUE=`echo $1 | sed s/-L//`;;

	--)  shift; break;;

	-*)  echo "$0: illegal option $1" >&2; exit 1;;

	*)   break;;
    esac
    shift
done

echo $NONCONFORMING
cat $PROLOGUE
echo $ENDPROLOG
echo $BEGINSETUP
cat ${COPYFILE:-/dev/null}
echo "GrabitDict begin"
echo "/recursive $RECURSIVE def"

echo mark
for i in $OMITNAMES; do
    case $i in
	/*) echo "$i";;
	?*) echo "/$i";;
    esac
done
echo GrabitSetup

echo end
echo $ENDSETUP

for i do
    echo "$i Grabit"
done

