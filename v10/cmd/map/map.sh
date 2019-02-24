#!/bin/sh
# MF -m -f or "", M map files, A other arguments
# MFLAG and FFLAG 0 or 1 for -m or -f ever used
FEATURE=no
MAPPROG=${MAPPROG-/usr/lib/map}
MAPDIR=${MAPDIR-/n/cetus/usr/maps}
A= F=0 FFLAG=0 MFLAG=0
for i in $*
do
	case $i in
	-f)
		F=1 FFLAG=1	A="$A -m ";;
	-m)
		F=0 MFLAG=1	A="$A $i ";;
	-*)
		F=0		A="$A $i ";;
	*)
		case $F$i in
		1riv*4)		A="$A 201 202 203 204 ";;
		1riv*3)		A="$A 201 202 203 ";;
		1riv*2)		A="$A 201 202 ";;
		1riv*)		A="$A 201 ";;
		1iriv*[34])	A="$A 206 207 208 ";;
		1iriv*2)	A="$A 206 207 ";;
		1iriv*)		A="$A 206 ";;
		1coast*4|1shore*4|1lake*4)	A="$A 101 102 103 104 ";;
		1coast*3|1shore*3|1lake*3)	A="$A 101 102 103 ";;
		1coast*2|1shore*2|1lake*2)	A="$A 101 102 ";;
		1coast*|1shore*|1lake*)		A="$A 101 ";;
		1ilake*[234]|1ishore*[234])	A="$A 106 107 ";;
		1ilake*|1ishore*)		A="$A 106 ";;
		1reef*)			A="$A 108 ";;
		1canal*[34])		A="$A 210 211 212 ";;
		1canal*2)		A="$A 210 211 ";;
		1canal*)		A="$A 210 ";;
		1glacier*)		A="$A 115 ";;
		1state*|1province*)	A="$A 401 ";;
		1countr*[34])		A="$A 301 302 303 ";;
		1countr*2)		A="$A 301 302 ";;
		1countr*)		A="$A 301 ";;
		1salt*[234])		A="$A 109 110 ";;
		1salt*)			A="$A 109 ";;
		1ice*[234]|1shel*[234])	A="$A 113 114 ";;
		1ice*|1shel*)		A="$A 113 ";;
		1*)	echo map: unknown feature $i 1>&2
			exit 1 ;;
		*)
			A="$A $i"
		esac
	esac
done

case $MFLAG$FFLAG in
01)
	case "$A " in
	*" 101 "*)
		: ;;
	*)
		A="$A -C black -m 101"
	esac
esac

MAP=${MAP-world} MAPDIR=$MAPDIR $MAPPROG $A
