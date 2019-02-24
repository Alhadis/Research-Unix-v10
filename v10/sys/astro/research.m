root	regfs	ra	0100
swap	ra	01	20480
swap	ra	011	20480
dump	uddump	1	10240	20480	# dump to ra11

dw750 0	bus 0	tr 8	voff 0x200
uda50 0	ub 0	reg 0772150	vec 0254
ra 0	uda50 0	unit 0
ra 1	uda50 0	unit 1
ra 2	uda50 0 unit 2
ra 3	uda50 0 unit 3
dz11 0	ub 0	reg 0760100	vec 0300
dz11 1	ub 0	reg 0760110	vec 0310
dn11 0	ub 0	reg 0775200	vec 0430
mg   0  ub 0 	reg 0767720 	vec 0360
drbit 0	ub 0	reg 0767570
ni1010a 0 ub 0	reg 0764000	vec 0340
ni1010a 1 ub 0	reg 0765000	vec 0350
kmc11b 0 ub 0	reg 0760200	vec 0600
kdi	1
kmcdk	256
drum	0
console	0
mem	0
stdio	0

ttyld	32
mesgld	64
rmesgld	0
cmcld	0
dkpld	256
unixpld	0
connld	0
bufld	32

regfs	10
procfs	0
msfs	0
errfs	0
netbfs	0
pipefs	0

#
# internet stuff
#
ip	32
udp	32
tcp	64
arp	128
ipld	0
udpld	0
tcpld	0

# extra blocks for added comfort
block	700
blkbuf	40
stream	512
queue	2048

maxdsize 32000
maxssize 32000
