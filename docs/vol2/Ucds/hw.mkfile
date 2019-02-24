NPROC=3
HWLIB=/n/coma/usr/tom/hw/lib
PINLIB=$HWLIB/*.pins /n/coma/usr/ucds/lib/lib/pins
PKGLIB=$HWLIB/*.pkg

ENVIRON="$ENVIRON
upper(){
	echo \$* | tr a-z A-Z
}
paltype(){
	sed -n '/^\.t[ 	]/{
		s/.*PAL//p
		q
	}' \$1
}"

WFILES=${JFILES:%.j=%.w}
GFILES=${JFILES:%.j=%.g}
AFILES=${JFILES:%.j=%.a}
FILES=${JFILES:%.j=%}
PALP=${PAL:%.e=%.p}
MIN=${PAL:%.e=%.min}

WHAT='again|annotate|check|clip|commit|draw|fix|list|net|pins|place|schematics|smoke|wr|wrap'

^($WHAT)\$:RN: $NAME.\\1

%.p: %.e
	lde -W $stem.e >$stem.p

%.min: %.e %.p
	lde -a $stem.e | quine | cover | hazard >$stem.ng
	pal -d `paltype $stem.p` $stem.ng >/dev/null
	mv $stem.ng $stem.min

'^([^.]+)\.drom\.([^.]+)$':R: \\1.min \\1.p
	man=`upper $stem2`
	type=`paltype $stem1.p`
	pal -d $type $stem1.min | tee to | drom -w -t$man$type
	sleep 5
	drom -t$man$type >from
	cmp to from

%.w: %.j
	jraw -w -k $stem.j | sed 's/\$N00/$'$stem'/' >$stem.w

%.w: %.g
	jraw -w $stem.g | sed 's/\$N00/$'$stem'/' >$stem.w

%.a: %.j %.w $NAME.wx
	annotate $NAME.wx $stem.w
	cat $stem.j >>$stem.a	

%.A:V: %.g %.w $NAME.wx
	Annotate $NAME.wx $stem.w
	cat $stem.g >>$stem.a	

%.annotate: $WFILES $NAME.wx
	Annotate $NAME.wx $WFILES
	for i in $FILES
	do
		cat $i.g >>$i.a
	done

%.schematics: %.annotate
	grawp $AFILES | pic | troff | gcan

%.list:Q: $NAME.wx
	wxlist $prereq

%.smoke: %.wx %.pins
	smoke -px $prereq >eyes

%.fx: %.wx
	fizz cvt $stem.wx >$stem.fx

%.place: %.brd %.pkg %.fx
	if [ ! -f $stem.pos ]; then touch $stem.pos; fi
	cat $prereq >$stem.-pos
	fizz place $stem.-pos $stem.pos
	rm $stem.-pos

%.check: %.brd %.pkg %.fx %.pos
	fizz check -uw $prereq

%.clip: %.brd %.pkg %.fx %.pos
	fizz clip $prereq

%.draw:Q: %.brd %.pkg %.fx %.pos
	fizz draw -p $prereq

%.wr: %.brd %.pkg %.fx %.pos
	fizz wrap -c -v $prereq >N$target
	if [ -f $target ]; then
		rework $target N$target && mv NEW.wr N$target
	else
		mv N$target $target
	fi

%.wrap:Q:
	if [ -f N$stem.wr ]; then
		wrap -s -r$ORIENT -d30 UN.wr
		wrap -s -r$ORIENT -d30 RE.wr
	else
		wrap -s -r$ORIENT -d30 $stem.wr
	fi

%.fix:
	wrap -s -r$ORIENT -d30 fix.wr

%.commit:
	mv N$stem.wr $stem.wr && rm -f UN.wr RE.wr .lastwire

$NAME.pins: $WFILES $PALP my.pins io.pins
	ucds mkpins $prereq $PINLIB >pins.ng 2>errors
	sed '/^\.[tT][tT]/s/[zZ]/g/g' pins.ng >$target
	rm pins.ng

$NAME.wx: $NAME.pins $WFILES 
	ucds cdmglob -f -v $prereq >$NAME.wX 2>errors
	sed '
		/\/\//s/	.*\/\//	/
		/\.\//s/\([ 	][ 	]*[^/][^/]*\)\/.*\/\.\(\/.*\)/\1\2/
		s/	\//	/
	' $NAME.wX >$target
	rm $NAME.wX

$NAME.pkg: $NAME.wx my.pkg $PKGLIB
	getpkgs $prereq >pkg.ng 2>errors
	mv pkg.ng $target

pal:Q:	$MIN
	for i in $PALP
	do
		echo -n $i
		echo `paltype $i` | findparts -n
	done

my.pkg my.pins io.pins:
	touch $target

name:
	whatis NAME JFILES PAL WFILES GFILES FILES PALP MIN

TIDYLIST='*.ng fizz.* mon.out core eyes errors from to'
tidy:
	rm -f $TIDYLIST
clean:
	rm -f $NAME.pins $NAME.pkg *.[pw] *.[fw]x *.min $TIDYLIST
