root	regfs	ra	0100
swap	ra	01	20480
swap	ra	011	20480
swap	ra	021	20480
dump	uddump	2	10240	20480	# dump to ra11

dw750 0	bus 0	tr 8	voff 0x200
uda50 0	ub 0	reg 0772150	vec 0154
ra 0	uda50 0	unit 0
ra 1	uda50 0	unit 1
ra 2	uda50 0	unit 2
ra 3	uda50 0	unit 3
dz11 0	ub 0	reg 0760100	vec 0300
kmc11b 0 ub 0	reg 0160200	vec 0600
kdi	1
drum	0
console	0
mem	0
stdio	0

ttyld	32
mesgld	256
rmesgld	0
cmcld	0
unixpld	0
connld	0

regfs	15
procfs	0
msfs	0
errfs	0
netbfs	0
pipefs	0
