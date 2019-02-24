#!/bin/sh
JD=${DICT-/n/jones/usr/dict}
BD=${DICT-/n/bowell/usr/dict}
JLD=${LIBDICT-/n/jones/usr/lib/dict}
BLD=${LIBDICT-/n/bowell/usr/lib/dict}

ptest(){
	case $opt in
		*x*)	: ;;
		*)	echo no prefix search in $dict 1>&2
	esac
}
case $1 in
-p)	opt=; shift ;;
*)	opt=x
esac
case $1 in
-r)	raw=cat; shift ;;
*)	raw=false
esac
case $# in
0)	opt=i$opt dict=webster ;;
1)	opt=i$opt dict=$1; shift ;;
*)	dict=$1; shift
esac

case $dict in
oxford)
	ptest
	OALDCEDIR=${OALDCEDIR-$BD/oaldce} $BLD/ldefine "$@"
	exit ;;
web2)
	exec look -df$opt "$@" $BD/web2 ;;
web7)
	exec look -dft:$opt "$@" $BD/words ;;
webster)
	ptest
	W7DIR=${W7DIR-$BD/w7full} $BLD/wdefine "$@"
	exit ;;
spell)
	exec look -df$opt "$@" /n/alice/usr/dict/words ;;
pron)
	exec look -df$opt "$@" $BD/web7 ;;
etym*)
	exec look -dft\ $opt "$@" $BD/w7etym ;;
acro*)
	exec look -dft\	$opt "$@" $BD/acro ;;
oed)
	ptest
	$JLD/odefine -$opt "$@" /s5/oed/conv.raw | 
		($raw || $JLD/oedread)
	exit ;;
odqindex)	# jones
	/usr/doug/odqlook -df$opt "$@" /dict1/odq/index.cooked
	exit ;;
odqtext)		# jones
	/usr/doug/odqlook1 -n$opt "$@" /dict1/odq/odq.cooked
	exit ;;
thesaurus)
	$BLD/colthlook -df$opt "$@" $BD/thesaurus |
		($raw || $BLD/colthread) 
	exit ;;
thesaurusa)
	$BLD/colthlook -df$opt "$@" $BD/thesaurusa |
		$BLD/colthread
	exit ;;
slang)
	$BLD/slanglook -df$opt "$@" $BD/slang
	exit ;;
names)
	$JLD/namelook -$opt "$@" $JD/hanks2 | 
		$JLD/namelook1 -ix $JD/hanks3 |
		($raw || $JLD/nameread)
	exit ;;
places)
	exec look -dft\	$opt "$@" /n/bowell/usr1/maps/usplaces ;;
towns)
	ptest
	case $# in
	0)	exec cbt look /n/alice/usr/spool/town/ustowns ;;
	*)	cbt look /n/alice/usr/spool/town/ustowns <<!
$*
!
	esac
	exit ;;
esac
echo 1>&2 "usage: dict [-p] [dictionary [word]] 
	dictionaries:
	webster    Merriam-Webster Collegiate 7th ed (default)
	web7       same, words only
	pron       same, words and pronunciations
	etym       root words from webster, with derivates
	web2       Merriam-Webster Unabridged 2nd ed (words only)
	oxford     Oxford Advanced Learner's Dictionary of Contemporary English
	oed        New English Dictionary (OED), fragment, bu-cz
	slang      New Dictionary of American Slang
	thesaurus  Collins Thesaurus
	thesaurusa same, augmented with backreferences
	spell      wordlist of spell(1)
	names      Oxford Dictionary of Names
	places     128,000 US populated places
	towns      45,000 US places
	acro       17,000 AT&T acronyms
	for further information, see dict(7)"
