# MF -m -f or "", M map files, A other arguments
# MFLAG and FFLAG 0 or 1 for -m or -f ever used
FEATURE=no
MAPDIR=${MAPDIR-/usr/lib/map}
MAPPROG=${MAPPROG-$MAPDIR/map}
M= A= MF= MFLAG=0 FFLAG=0
for i in $*
do
	case $i in
	-m)
		MF=-m MFLAG=1 ;;
	-f)
		MF=-f FFLAG=1 ;;
	-*)
		MF= A="$A $i" ;;
	*)
		case $MF$i in
		-m*)		M="$M $i" ;;
		-friv*4)	M="$M 201 202 203 204";;
		-friv*3)	M="$M 201 202 203";;
		-friv*2)	M="$M 201 202";;
		-friv*)		M="$M 201";;
		-firiv*[34])	M="$M 206 207 208";;
		-firiv*2)	M="$M 206 207";;
		-firiv*)	M="$M 206";;
		-fcoast*4|-fshore*4|-flake*4)	M="$M 102 103 104";;
		-fcoast*3|-fshore*3|-flake*3)	M="$M 102 103";;
		-fcoast*2|-fshore*2|-flake*2)	M="$M 102";;
		-fcoast*|-fshore*|-flake*)	M="$M 101";;
		-filake*[234]|-fishore*[234])	M="$M 106 107";;
		-filake*|-fishore*)		M="$M 106";;
		-freef*)	M="$M 108";;
		-fcanal*[34])	M="$M 210 211 212";;
		-fcanal*2)	M="$M 210 211";;
		-fcanal*)	M="$M 210";;
		-fglacier*)	M="$M 115";;
		-fstate*|-fprovince*)		M="$M 401";;
		-fcountr*[34])	M="$M 301 302 303";;
		-fcountr*2)	M="$M 301 302";;
		-fcountr*)	M="$M 301";;
		-fsalt*[234])	M="$M 109 110";;
		-fsalt*)	M="$M 109";;
		-fice*[234]|-fshel*[234])	M="$M 113 114";;
		-fice*|-fshel*)			M="$M 113";;
		-f*)	echo map: unknown feature $i 1>&2
			exit 1 ;;
		*)
			A="$A $i" MF=
		esac
	esac
done

case "$MFLAG$FFLAG$M " in
00)
	: ;;
1* | *" 101 "*)
	M="-m $M" ;;
01*)
	M="-m 101 $M"
esac

MAP=${MAP-world} MAPDIR=$MAPDIR $MAPPROG $A $M
