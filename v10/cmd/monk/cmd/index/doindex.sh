
#
# initalizations
#
case `uuname -l` in
	
mhuxm)
	MHOME=/m2/c1122/frodo/lib/monk
	;;
	
esac

#
# path names for programs and sed scripts
#
MHOME=${MHOME-/usr/lib/monk}

PATH=$MHOME:$PATH
export PATH

#
# create the index
#
grep '^ix:' $* |
sed -f $MHOME/doclean |
permute |
sed -f $MHOME/dofont |
dosort |
uniq |
crunch |
sequal |
subindex |
sed -f $MHOME/undofont
