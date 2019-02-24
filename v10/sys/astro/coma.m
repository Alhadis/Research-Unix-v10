root	regfs	ra	0100
swap	ra	003	249848
swap	ra	013	249848
dump	bddump	0x2401	530416	249848	# ra14 == crash dumps

bia 2	bi 2	node 2	voff 0x400

kdb50 0	bi 2	node 4	vec 0x10
kdb50 1 bi 3	node 4	vec 0x18
ra 0	kdb50 0	unit 0
ra 1	kdb50 0	unit 1
ra 2	kdb50 1	unit 2
ra 3	kdb50 1	unit 3
ra 4	kdb50 0 unit 4
ra 5	kdb50 0 unit 5
ra 6	kdb50 1 unit 6
ra 7	kdb50 1 unit 7
debna 0 bi 2	node 7	vec 0x40
dwbua 1	bi 2	node 6 vec 0x30 voff 0x800	# klesi-b
uda50 0	ub 1	reg 0774500 vec 0300
ta 0	uda50 0	unit 0

bia 3	bi 3	node 2	voff 0x600
dwbua 0	bi 3	node 0	vec 0x20 voff 0x200	
kmc11b 0 ub 0	reg 0160200 vec 0600
kmc11b 1 ub 0	reg 0160210 vec 0610
uda50 1 ub 0	reg 0160404 vec 0310
ta 1	uda50 1 unit 0
kdi	2
drum	0
fineclock 0
nautcons 0
console	0
mem	0
stdio	0

ttyld	128
mesgld	300
rmesgld	0
cmcld	0
unixpld	0
connld	0

regfs	30
procfs	0
msfs	0
netbfs	0
errfs	0
pipefs	0

ip	4
udp	16
tcp	256
arp	128

ipld  0
udpld 0
bufld	4
tcpld 0
