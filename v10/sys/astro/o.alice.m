# 855bb-ce (88a79317x)
# ba11-aw

root	regfs	ra	0100
swap	ra	042	249848
swap	ra	043	249848
# swap	ra	001	20480
# swap	ra	011	20480
# swap	ra	021	20480
# swap	ra	031	20480
# swap	ra	041	20480
# swap	ra	051	20480
#dump	uddump	0x1001	10240	20480	# dump to ra11: dw1, first uda addr

bia 2	bi 2	node 2	voff 0x400

debna 0 bi 2	node 7	vec 0x40
dwbua 1	bi 2	node 4	vec 0x30	voff 0xa00	# klesi-b
uda50 0	ub 1	reg 0774500	vec 0310		# uda50-aa	
ta 0	uda50 0	unit 0					# tu81-e-ba (pi81115411)



kdb50 0	bi 2	node 3	vec 0x10			# kdb50-b
ra 0	kdb50 0	unit 0					# ra81-aa (cx-90294)
ra 2	kdb50 0	unit 2					# ra81-aa (cx-10431)
ra 4	kdb50 0	unit 4
ra 6	kdb50 0 unit 6

bia 3	bi 3	node 2	voff 0x600

kdb50 1	bi 3	node 4	vec 0x10			# kdb50-b
ra 1	kdb50 1	unit 1					# ra81-aa (cx-a37480)
ra 3	kdb50 1	unit 3					# ra81-aa (cx-27089)
ra 5	kdb50 1	unit 5					# ra81-aa (cx-26828)
ra 7	kdb50 1 unit 7

dwbua 0	bi 3	node 0	vec 0x20	voff 0x200	# dwbua-ca

kmc11b 0	ub 0	reg 0760200	vec 0600	# kmc11-b
kmc11b 1	ub 0	reg 0760210	vec 0610	# kmc11-b
uda50 3 ub 0	reg 0160404 vec 0320
ta 1	uda50 3 unit 0
drbit	0	ub 0	reg 0767570
dn11	0	ub 0	reg 0775200	vec 0430	# acu



dz11 0	ub 0	reg 0760100	vec 0300		# dz11-a
dz11 1	ub 0	reg 0760110	vec 0310		# dz11-a

kdi	2
drum	0
console	0
nautcons 0
fineclock 0
mem	0						# 138 Mbytes
stdio	0
ip	4
udp	16
tcp	256
arp	128

ttyld	128
nttyld	32
mesgld	300
rmesgld	0
unixpld	0
connld	0
ipld	0
udpld	0
tcpld	0
bufld	32

regfs	30
procfs	0
msfs	0
netbfs	0
errfs	0
pipefs	0
