# like europa, but with drives 0/1 swapped
# for emergency use
root	regfs	ra	0100
swap	ra	001	20480
swap	ra	011	20480
dump	uddump	1	10240	20480	# dump to ra01

uvqbus 0	voff 0x200

uda50 0	ub 0	reg 0772150	vec 0254
ra 0	uda50 1	unit 7

uda50 1 ub 0	reg 0760334	vec 0300
ra 1	uda50 0	unit 0
ra 2	uda50 1 unit 1
ra 3	uda50 1 unit 6

uda50 2	ub 0	reg 0774500	vec 0260
ta 0	uda50 2 unit 0

dk 0	ub 0 reg 0767770 vec 0310
deqna 0	ub 0 reg 0774440 vec 0320

drum	0
console	0
mem	0
stdio	0

ttyld	256
nttyld	64
mesgld	512
rmesgld	0
cmcld	0
dkpld	256
unixpld	0
connld	0

#
# tcp/ip stuff
#
ip	64
udp	32
tcp	64
arp	128
ipld	0
udpld	0
bufld	4
tcpld	0

regfs	24
procfs	0
msfs	0
netafs	0
netbfs	0
pipefs	0
errfs	0

blkbuf	40
block	2500
