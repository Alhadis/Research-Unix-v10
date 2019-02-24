#
# Center pages and put cropmarks at each corner. Physical page size
# is set with -w and -h. The default is 8.5 by 11.0 inches. Device
# dependent code to change paper size (e.g. with setpageparams) goes
# in the prologue. You may need to customize the device dependent
# code that we distribute. By default it only supports variable page
# sizes on Linotronic typesetters, and assumes those typesetters are
# using 12 inch wide paper. Use -d to disable execution of device
# dependent PostScript code.
#
# What's here was written quickly and will likely be very different
# in our next release. It should be part of a more general program!!
# 

POSTLIB=/usr/lib/postscript
PROLOGUE=$POSTLIB/cropmarks.ps

EXPANDPAGE=true
PAGEWIDTH=8.5
PAGEHEIGHT=11.0
SCALETOFIT=false
XOFFSET=0.0
YOFFSET=0.0

NONCONFORMING="%!PS"
ENDPROLOG="%%EndProlog"
BEGINSETUP="%%BeginSetup"
ENDSETUP="%%EndSetup"

while [ -n "$1" ]; do
    case $1 in
	-d)  EXPANDPAGE=false;;

	-h)  shift; PAGEHEIGHT=$1;;
	-h*) PAGEHEIGHT=`echo $1 | sed s/-h//`;;

	-s)  SCALETOFIT=true;;

	-w)  shift; PAGEWIDTH=$1;;
	-w*) PAGEWIDTH=`echo $1 | sed s/-w//`;;

	-x)  shift; XOFFSET=$1;;
	-x*) XOFFSET=`echo $1 | sed s/-x//`;;

	-y)  shift; YOFFSET=$1;;
	-y*) YOFFSET=`echo $1 | sed s/-y//`;;

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
echo "CropmarkDict begin"
echo "/pageheight $PAGEHEIGHT def"
echo "/pagewidth $PAGEWIDTH def"
echo "/expandpage $EXPANDPAGE def"
echo "/scaletofit $SCALETOFIT def"
echo "/xoffset $XOFFSET def"
echo "/yoffset $YOFFSET def"
echo "setup"
echo "end"
echo $ENDSETUP

cat $*

