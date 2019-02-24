#!/bin/sh

#
# remove any possibly exported variables
#
unset Acmd Bcmd STDIN ccmd cncmd cwcmd dest ecmd eqn eqn202 eqnaps eqni10 eqni300
unset eqnim300 eqnother eqnpost files gcmd icmd ipacmd
unset macro nocompact options output pcmd pitch preprocessors
unset rcmd roff spooler tcmd tpcmd verbose

#
# initalizations
#
case `uuname -l` in

alux*)
	MHOME=`sed -n "/^frodo:/s/.*:\(.*\):.*/\1/p" /etc/passwd`/lib/monk
	STDIN=-
	TERM=-Ti300
	eqnaps="-Taps -d /usr/pub/apseqnchar $STDIN"
	eqni300="-Ti300 -d /usr/pub/i300eqnchar $STDIN"
	spooler=lpr
	;;

cinnamon|coqui|coyote|dolphin|golem|lynx|marmot|polecat|python)
	MHOME=/usr/local/lib/monk
	STDIN=-
	TERM=-T${TYPESETTER-202}
	eqnaps="-Taps -d /usr/pub/apseqnchar $STDIN"
	spooler=lp
	;;

sneezy)
	MHOME=/usr/llib/monk
	STDIN=-
	TERM=-Ti300
	eqnaps="-Taps -d /usr/pub/apseqnchar $STDIN"
	eqni10="-r240 -m6 -d /usr/pub/eqnchar $STDIN"
	eqni300="-r300 -m6 -d /usr/pub/i300eqnchar $STDIN"
	eqnim300="-r300 -m6 -d /usr/pub/im300eqnchar $STDIN"
	eqnpost="-r720 -m6 -d /usr/pub/eqnchar $STDIN"
	output="|uux hunny!lp -"
	;;

*)
	if [ "$TOOLS" ]
	then
		MHOME=$TOOLS/lib/monk
		STDIN=-
		TERM=-Tpost
		eqnaps="-Taps -d /usr/pub/apseqnchar $STDIN"
		eqni10="-Ti10 -m6 -d /usr/pub/eqnchar $STDIN"
		eqni300="-Ti300 -m6 -d /usr/pub/i300eqnchar $STDIN"
		eqnim300="-Ti300 -m6 -d /usr/pub/im300eqnchar $STDIN"
		eqnpost="-Tpost -m6 -d /usr/pub/eqnchar $STDIN"
		spooler=prt
	else
		STDIN=/dev/stdin
		TERM=-T${TYPESETTER-post}
		eqnaps="-Taps -d /usr/pub/apseqnchar $STDIN"
		spooler=lp
	fi
	;;

esac

#
# path names for libraries, macros, eqn definitions and default references
#
MHOME=${MHOME-/usr/lib/monk}

MDB=${MDB-$MHOME/db}

MP=${TMAC-$MHOME/tmac.p}

eqn202=${eqn202-"-T202 -d /usr/pub/eqnchar $STDIN"}
eqnaps=${eqnaps-"-Taps -d /usr/pub/eqnchar $STDIN"}
eqni10=${eqni10-"-Ti10 -d /usr/pub/eqnchar $STDIN"}
eqni300=${eqni300-"-Ti300 -d /usr/pub/eqnchar $STDIN"}
eqnim300=${eqni300-"-Tim300 -d /usr/pub/eqnchar $STDIN"}
eqnpost=${eqnpost-"-Tpost -d /usr/pub/eqnchar $STDIN"}
eqnother=${eqnother-"-d /usr/pub/eqnchar $STDIN"}

REFS=${REFS-$MHOME/prefs}

eqn=${eqn-eqn}
roff=${roff-troff}

PATH=$MHOME:$PATH
export PATH

#
# get options
#
for i
do
	case "$i" in

#
# troff, reference or output options
#
	-12)
		pitch=-12
		;;

	-A*)
		Acmd="|"`expr "$i" : '..\(.*\)'`
		;;

	-B*)
		Bcmd="|"`expr "$i" : '..\(.*\)'`
		;;

	-E)
		options="$options -e"
		;;

	-N)
		nocompact=-n
		;;

	-R*)
		REFS=`expr "$i" : '..\(.*\)'`
		;;

	-Sapsend)
		spooler=apsend
		;;

	-Sd202)
		spooler=d202
		;;

	-Si300)
		spooler=i300
		;;

	-Slp)
		spooler=lp
		;;

	-Slpr)
		spooler=lpr
		;;

	-Sproof)
		spooler=proof
		;;

	-Sprt)
		spooler=prt
		;;

	-Sthink)
		spooler=think
		;;

	-S-)
		spooler=-
		;;

	-T202)
		TERM=$i
		eqn='eqn'
		roff='troff'
		;;

	-Taps)
		TERM=$i
		eqn='eqn'
		roff='troff'
		;;

	-Ti10)
		TERM=-Ti10
		eqn='eqn'
		roff='troff'
		;;

	-Ti300)
		TERM=-Ti300
		eqn='eqn'
		roff='troff'
		;;

	-Tim300)
		TERM=-Tim300
		eqn='eqn'
		roff='troff'
		;;

	-Tpost)
		TERM=-Tpost
		eqn='eqn'
		roff='troff'
		;;

	-Tthink)
		TERM=$i
		eqn='neqn'
		roff='nroff'
		spooler=think
		;;

	-T*)
		if [ x"$i" = x-T ]
		then
			TERM=-T$TERM
		else
			TERM=$i
		fi
		eqn='neqn'
		roff='nroff'
		spooler=-
		;;

	-a)
		options="$options $i"
		eqn='eqn'
		roff=troff
		spooler=-
		;;

	-x)
		verbose=on
		;;

	-*)
		preprocessors="$preprocessors $i"
		;;

	*)
		if [ ! -r $i ]
		then
			echo 1>&2 "monk: $i is not readable"
			if [ -s /tmp/e$$ ]
			then
				cat /tmp/e$$; rm /tmp/e$$
			fi
			exit 1
		fi
		if [ -d $i ]
		then
			echo 1>&2 "monk: $i is a directory"
			if [ -s /tmp/e$$ ]
			then
				cat /tmp/e$$; rm /tmp/e$$
			fi
			exit 1
		fi
		files="$files $i"
		;;

	esac
done

#
# find what pre processors monk needs
#
preprocessors=`
egrep '\|(remember|reference|ipa|graph|ideal|table|picture|ped|equation|e|computeroutput) *[(<[{'\''"\`]|\|begin *[(<[{'\''"\`] *(remember|reference|ipa|graph|ideal|table|picture|ped|equation|e|computeroutput)' $files |
awk '
/\|remember *[(<[{'\''"\`]/		{cite++}
/\|begin *[(<[{'\''"\`] *remember/	{cite++}
/\|reference *[(<[{'\''"\`]/		{refer++}
/\|begin *[(<[{'\''"\`] *reference/	{refer++}
/\|ipa *[(<[{'\''"\`]/			{ipa++}
/\|begin *[(<[{'\''"\`] *ipa/		{ipa++}
/\|graph *[(<[{'\''"\`]/		{grap++; pic++}
/\|begin *[(<[{'\''"\`] *graph/		{grap++; pic++}
/\|ideal *[(<[{'\''"\`]/		{ideal++}
/\|begin *[(<[{'\''"\`] *ideal/		{ideal++}
/\|table *[(<[{'\''"\`]/		{tbl++}
/\|begin *[(<[{'\''"\`] *table/		{tbl++}
/\|picture *[(<[{'\''"\`]/		{pic++}
/\|begin *[(<[{'\''"\`] *picture/	{pic++}
/\|ped *[(<[{'\''"\`]/			{tped++}
/\|begin *[(<[{'\''"\`] *ped/		{tped++}
/\|equation *[(<[{'\''"\`]/		{eqn++}
/\|begin *[(<[{'\''"\`] *equation/	{eqn++}
/\|e *[(<[{'\''"\`]/			{eqn++}
/\|begin *[(<[{'\''"\`] *e/		{eqn++}
/\|computeroutput *[(<[{'\''"\`]/	{cw++}
/\|begin *[(<[{'\''"\`] *computeroutput/ {cw++}
END 				{
				 x = ""
				 if (cite)  x = x " -cn"
				 if (refer) x = x " -r"
				 if (ipa)   x = x " -ipa"
				 if (grap)  x = x " -g"
				 if (ideal) x = x " -i"
				 if (tbl)   x = x " -t"
				 if (pic)   x = x " -p"
				 if (tped)  x = x " -tp"
				 if (eqn)   x = x " -e"
				 if (cw)   x = x " -cw"
				 print x
				}
'`" $preprocessors"

#
# get preprocessors and other options
#
for i in $preprocessors
do
	case "$i" in

#
# pre-processors
#
	-c)
		ccmd='|col'
		;;

	-cn)
		cncmd='|cite'
		;;

	-cw)
		cwcmd='|cw'
		;;

	-e)
		ecmd="|$eqn"
		;;

	-g)
		gcmd='|grap'
		;;

	-i)	icmd='|ideal -q'
		;;

	-ipa)	ipacmd='|mipa'
		;;

	-p)
		pcmd='|pic'
		;;

	-r)
		rcmd="|prefer -s att -p $REFS"
		;;

	-t)
		tcmd='|tbl'
		;;

	-tp)
		tpcmd='|tped'
		;;

	-*)
		options="$options $i"
		;;

	esac
done


#
# special handling for terminals
#
	case $roff  in

	troff)
		case $TERM in

		-T202|-Taps|-Ti10|-Ti300|-Tim300|-Tpost)
			;;

		*)
			echo 1>&2 "monk: $TERM is not a valid output device"
			exit 1
			;;

		esac
		;;

	nroff)
		if [ "$pitch" ]
		then
			case $TERM in

			-T300|-T300s|-T450|-T1620)
				TERM="$TERM-12"
				;;
			esac
		fi

		if [ "ccmd" ]
		then
			case $TERM in

			-T300|-T300s|-T450|-T4014|-Ttek|-T1620)
				ccmd="|col -f|greek $TERM"
				;;

			-T300-12|-T300s-12|-T450-12|-T1620-12)
				ccmd='|col -f|greek $TERM'
				options='$options -rW72'
				;;

			-T37|-T4000a|-T382|-TX)
				ccmd='|col -f'
				;;

			-Thp|-T2621|-T2640|-T2645)
				ccmd='|colcrt -'
				;;

			-T735|-T745)
				ccmd='|col -x'
				;;

			-T43)
				ccmd='|col -x'
				options='$options -rW75'
				;;

			-T40/4|-T40/2)
				ccmd='|col -b'
				;;

			esac
		fi

		case $TERM in

		-T1620)
			TERM=-T450
			;;

		-T1620-12)
			TERM=-T450-12
			;;

		-T37|-Ttn300|-T300S|-T300*|-T450*)
			;;

		*)
			TERM=
			;;

		esac
		;;

	*)
		echo 1>&2 "monk: monk must use nroff or troff"
		exit 1
		;;

	esac

#
# special handling for pic and eqn
#
if [ "$pcmd" ]
then
	macro="$macro $MP"

	case $TERM in

	-T202)
		pcmd="$pcmd -D"
		;;

	*)
		pcmd="$pcmd $TERM"
		;;

	esac
fi

if [ "$ecmd" ]
then
	case $TERM in

	-T202)
		ecmd="$ecmd $eqn202"
		;;

	-Taps)
		ecmd="$ecmd $eqnaps"
		;;

	-Ti10)
		ecmd="$ecmd $eqni10"
		;;

	-Ti300)
		ecmd="$ecmd $eqni300"
		;;

	-Tim300)
		ecmd="$ecmd $eqnim300"
		;;

	-Tpost)
		ecmd="$ecmd $eqnpost"
		;;

	*)
		ecmd="$ecmd $eqnother"

	esac

fi

#
# get the correct spooler
#
if [ ! "$output" ]
then
	case $spooler in
	apsend)
		output='|apsend'
		;;

	d202)
		output='|d202'
		;;

	i300)
		if [ "$IDEST" ]
		then
			dest="IDEST=$IDEST"
		fi
		output="|$dest i300"
		;;

	lp)
		if [ "$LPDEST" ]
		then
			dest="LPDEST=$LPDEST"
		fi
		output="|$dest lp"
		;;

	lpr)
		if [ "$LPRINTER" ]
		then
			dest="LPRINTER=$LPRINTER"
		fi
		output="|$dest lpr"
		;;

	proof)
		output="|proof"
		;;

	prt)
		output="|prt -ltroff"
		;;

	think)
		output="|think"
		;;

	-)
		output=
		;;

	esac

fi

#
# do it
#
if [ $verbose ]
then
	echo 1>&2 PATH=$MHOME:\$PATH export PATH
	echo 1>&2 "$MHOME/monk $nocompact -d $MDB $files \\
	$Bcmd $cncmd $rcmd $ipacmd $gcmd $icmd $cwcmd $tcmd $pcmd $tpcmd $ecmd $Acmd \\
	| $roff $TERM $options $macro - $ccmd $output"
fi
sh -c "exec $MHOME/monk $nocompact -d $MDB $files \\
	$Bcmd $cncmd $rcmd $ipacmd $gcmd $icmd $cwcmd $tcmd $pcmd $tpcmd $ecmd $Acmd \\
	| $roff $TERM $options $macro - $ccmd $output" 2> /tmp/e$$

#
# error messages and .cite file
#
egrep '^(\.CD|ix:|\.TC)[ 	]' /tmp/e$$ >> .cite
if [ -s .cite ]
then
	:
else
	rm .cite
fi
egrep 1>&2 -v '^(\.CD|ix:|\.TC)[ 	]' /tmp/e$$
rm /tmp/e$$
