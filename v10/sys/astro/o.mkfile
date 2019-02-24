#
# 1127 systems
#
COMETS=encke.u research.u seki.u westphal.u
NAUTS=coma.u bowell.u pipe.u alice.u n.bowell.u
STARS=alice.u
MFLOWS=kwee.u
MFAIRS=r70.u europa.u europa1.u

all:V: $NAUTS $COMETS $MFLOWS $STARS

$COMETS:V:
	cc=$cc mk -f ../lib/mk.comet $target
$NAUTS:V:
	cc=$cc mk -f ../lib/mk.naut $target
$MFLOWS:V:
	cc=$cc mk -f ../lib/mk.mflow $target
$STARS:V:
	cc=$cc mk -f ../lib/mk.star $target
$MFAIRS:V:
	cc=$cc mk -f ../lib/mk.mfair $target
