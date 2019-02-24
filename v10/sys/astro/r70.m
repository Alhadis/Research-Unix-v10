#
# none of this is even slightly correct yet
#

root	regfs	ra	0120
swap	ra	021	20480
swap	ra	023	249848
dump	uddump	2	10240	20480	# dump to ra21

uvqbus 0	voff 0x200
uda50 0	ub 0	reg 0772150	vec 0254
ra 0	uda50 0 unit 0
ra 1	uda50 0 unit 1
ra 2	uda50 0	unit 2
ra 3	uda50 0 unit 3

uda50 1	ub 0	reg 0760334	vec 0260
ra 4	uda50 1	unit 0
ra 5	uda50 1	unit 1
dk 0	ub 0 reg 0767770 vec 0300
deqna 0	ub 0 reg 0774440 vec 0310
deqna 1	ub 0 reg 0774460 vec 0314
drum	0
console	0
mem	0
stdio	0

ttyld	32
mesgld	256
rmesgld	0
dkpld 96
unixpld	0
connld	0

regfs	48
procfs	0
msfs	0
errfs	0
netbfs	0
pipefs	0

#
# tcp/ip stuff
#
ip	32
udp	32
tcp	128
arp	128
ipld	0
udpld	0
bufld	8
tcpld	0

blkbuf	200
block	3200
