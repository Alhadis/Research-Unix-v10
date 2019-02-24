PHOME=${PHOME:-${TOOLS:+$TOOLS/lib/prefer}}
PHOME=${PHOME:-/usr/lib/prefer}

$PHOME/mypubenter -o ${1-prefer.out} $PHOME/ptemplate
