root	regfs	ra	0100
swap	ra	001	20480
swap	ra	011	20480
dump	uddump	0	10240	20480	# dump to ra01

uvqbus 0	voff 0x200
uda50 0	ub 0	reg 0172150 vec 0320
ra 0	uda50 0	unit 0
ra 1	uda50 0	unit 1
ra 2	uda50 0	unit 2
ra 3	uda50 0 unit 3
ta 0	uda50 0 unit 0
uda50 1 ub 0	reg 0774500 vec 0330	# tk50, in case
ra 4	uda50 1 unit 0
ra 5	uda50 1 unit 1
ra 6	uda50 1 unit 2
ra 7	uda50 1 unit 3
ta 1	uda50 1 unit 0
deqna 0	ub 0	reg 0774440 vec 0310

drum	0
console	0
mem	0
stdio	0

ttyld	64
nttyld	32
mesgld	256
rmesgld	0
connld	0

#
# tcp/ip stuff
#
ip	32
udp	16
tcp	32
arp	128
ipld	0
udpld	0
bufld	4
tcpld	0

regfs	10
procfs	0
msfs	0
netafs	0
netbfs	0
pipefs	0
errfs	0

blkbuf	40
stream	512
