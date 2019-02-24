#	@(#)fulltext:docsubmit	1.18

# THis routine was originally written by Lorinda Cherry - it gathers up
# all the files connected with a documents, and transmits them via
# uucp to   mhuxd!~/full.text/
#
# It bundles all the files involved together into one cpio file
# called <1st 5 chars of machine name><process id>.cp

# Finding a good awk is a pain - set it here if you want
L=/usr/lib/docsubmit
AWK=""
if test -z "${AWK}"
then
    if ans=`sh -c 'logdir exptools' 2>&1` ; then
	:
    else
	ans=`grep '^exptools:' /etc/passwd | sed "s/^[^:]*:[^:]*:[^:]*:[^:]*:[^:]*:\([^:]*\).*/\1/"`
	fi
    if test -r ${ans}/bin/nawk
    then
	AWK=${ans}/bin/nawk
	fi
    fi
if test -z "${AWK}"
then
    if test -r /usr/local/bin/awk
    then
	AWK=/usr/local/bin/awk
	fi
    fi
if test -z "${AWK}"
then
    AWK=awk
    fi

# Get machine nodename - swiped from rn build routine Configure
if ans=`sh -c hostname 2>&1` ; then
    MACH=$ans
elif ans=`sh -c 'uuname -l' 2>&1` ; then
	MACH=$ans
elif ans=`sh -c 'uname -n' 2>&1` ; then
	MACH=$ans
else
	MACH=failedget
	fi
# Get the users LOGNAME
if ans=`sh -c logname 2>&1` ; then
    LOGNAME=$ans
else
    LOGNAME=`echo $HOME|sed 's/.*\///'`
    fi

T=/tmp
AWKTEMP=/tmp/Doc.Awk.$$
OUTP="junk"
COVER="junk"
DOCNUM="junk"
REF=""
# cut the machine name down to a max of 5 characters
DIR=`echo ${MACH} | sed 's/\(.....\).*/\1/'`$$
NAME=$DIR
DIR=$T/$DIR
trap 'rm -f ${AWKTEMP} $DIR/* $T/$NAME.cp ; rmdir $DIR 2>>/dev/null; trap 0;exit' 0 1 2 15
if test $# -eq 0
then
	echo "docsubmit -C -c file.cover -N number -r ref_file -t  file(s)"
	echo "   -C if document complete without coversheet, else -c file.cover"
	echo "   -N number   if document number not included in the files"
	echo "   -r to include a refer file"
	echo "   -t if source is Knuths TeX instead of troff/nroff\n"
	echo "  Two examples, first with cover sheet, second without:"
	echo "    docsubmit -c mydoc.cover mydoc.text mydoc.append"
	echo "    docsubmit -C -N 79111-880324-01 mydoc.text"
	exit 0
fi
for i in $*
do case $i in
    -r) shift; REF=$1; shift; continue;;
    -c) shift; COVER=$1; shift; continue;;
    -C) shift; COVER=$$; continue;;
    -t) shift; TEX="yes"; ARG=$1; shift; continue;;
    -N) shift; DOCNUM=$1; shift; continue;;
    -*) echo docsubmit unknown flag $i; exit 1;;
    esac
    done
ARGS=$*
if test "$COVER" = "junk" 
then
    echo "You must include the coversheet file for the paper to be submitted"
    echo "If the coversheet is in the files (or the files are the complete document),"
    echo "       indicate this by using the -C flag"
    echo "Use the flag  -N number   if document number not included in the files"
    echo "     ===> Full text of document not sent <==="
    exit 1
    fi

# check the entered document number
if test ${DOCNUM} != "junk"
then
    x=`expr "${DOCNUM}" : '^[a-zA-Z0-9]*-[89][0-9]*-[0-9][0-9]'`
    if test $x -lt 2
    then
	echo "\nDocument numbers must be of form  OrgNum-Date-seq"
	echo "   Where Date is usually YYMMDD (e.g. 881224) [always starts with a year]"
	echo "   and the sequence number is always 2 digits (e.g. 03)"
	echo "     ===> Full text of document not sent <==="
	exit 1
	fi
    fi
OUTP=$DIR/$NAME
mkdir $DIR
if test $TEX
then
	case $OUTP in
	*.*) echo "the name for a TeX paper cannot contain . tex gets confused"
		echo "	use _ or - instead"
		exit 1
	esac
	BBL=$OUTP.bbl
	OUTP=$OUTP.tex
	echo "%...\\\"" `date` >$OUTP
	echo "%...\\\"" EmailAddress ${MACH}"!"${LOGNAME} >>$OUTP
	if test "${DOCNUM}" != "junk"
	then
	    echo "%...\\\" DocNum ${DOCNUM}" >>$OUTP
	    fi
	echo "%...\\\"" `pwd` >>$OUTP
	${AWK} -f $L/tex.awk dir=$DIR texinp=$TEXINPUTS $ARG.tex >>$OUTP
	# Check that it ended okay
	if test $? -ne 0
	then
	    echo "Sorry - couldnt collect together the full text of your document."
	    echo "No full text was sent! Contact the docsubmit provider with questions."
	    exit 1 
	    fi

	if test -s $ARG.bbl
	then
		cp $ARG.bbl $BBL
	else if test \( -s $ARG.bib \) -a \( ! -s $DIR/$ARG.bib \)
	then
		cp $ARG.bib $BIB
	fi
	fi
else
	echo "...\\\"" `date` >$OUTP
	echo "...\\\"" EmailAddress ${MACH}"!"${LOGNAME} >>$OUTP
	if test "${DOCNUM}" != "junk"
	then
	    echo "...\\\" DocNum ${DOCNUM}" >>$OUTP
	    fi
	echo "...\\\"" `pwd` >>$OUTP
	if test $REF
	then
	cat >>$OUTP <<x
...\"refer first
x
	fi
	if test $COVER = $$
	then
		PARMS="$ARGS"
	else
		PARMS="$COVER $ARGS"
	fi
	${AWK} -f $L/troff.awk dir=$DIR $PARMS >>$OUTP
	if test $? -ne 0
	then
	    echo "Sorry - couldnt collect together the full text of your document."
	    echo "No full text was sent! Contact the docsubmit provider with questions."
	    exit 1 
	    fi

	if test $REF
	then
	cat >>$OUTP <<x
...\"refer begin
x
	cat $REF >>$OUTP
	cat >>$OUTP <<x
...\"refer end
x
	fi
fi
cd $T
ls $NAME/*|cpio -oc >$NAME.cp
chmod 0644 $NAME.cp
uucp -C $NAME.cp mhuxd!~/full.text/
echo "
The full text of your document has been sent to the Library Network.
Please note that an official paper copy MUST still be filed.
"
exit 0
