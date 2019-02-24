PHOME=${PHOME:-${TOOLS:+$TOOLS/lib/prefer}}
PHOME=${PHOME:-/usr/lib/prefer}
export PHOME
case $1 in
	-d)	$PHOME/pdoc $2;;

	*)	$PHOME/pbib $1;;
esac
