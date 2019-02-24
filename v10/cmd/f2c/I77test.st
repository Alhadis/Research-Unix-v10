./ ADD NAME=in TIME=631757377
d
&nd da=1,2/
d=2 n=j
&nj j=1,ja=2,3
./ ADD NAME=in1 TIME=631756134
&nml ia(4,6,7:11) = 7,8,9,10/
&nml ia(2,4,7:10:2) = 333,444 i=-1/
&nml c(3:4) = "def"/
&nml ia(2:3,4:5, 7:8) = 1,2,3,4,5,6,7,8 x=72/
&nml ia(3:3,4:5, 7:8) = 1,2,3,4 x=72/
./ ADD NAME=x.f TIME=631757750
	integer i, ia(2)
	double precision d, da(2)
	character*7 h, ha(2)
	logical L, la(2)
	real r, ra(2)
	integer*2 j, ja(2)
	complex c, ca(2)
	double complex z, za(2)
	character*1 n
	namelist /ni/ i, ia, n
	namelist /nd/ d, da, n
	namelist /nh/ h, ha, n
	namelist /nr/ r, ra, n
	namelist /nj/ j, ja, n
	namelist /nc/ c, ca, n
	namelist /nz/ z, za, n
	namelist /nl/ l, la, n
 1	write(*,*) 'enter n (one of i,d,h,r,j,c,z):'
	read(*,*,end=999) n
 10	if (n .eq. 'i') goto 20
	if (n .eq. 'd') goto 30
	if (n .eq. 'h') goto 40
	if (n .eq. 'r') goto 50
	if (n .eq. 'j') goto 60
	if (n .eq. 'c') goto 70
	if (n .eq. 'z') goto 80
	if (n .eq. 'l') goto 90
	goto 1
 20	read(*,ni,end=999)
	write(*,ni)
	goto 10
 30	read(*,nd,end=999)
	write(*,nd)
	goto 10
 40	read(*,nh,end=999)
	write(*,nh)
	goto 10
 50	read(*,nr,end=999)
	write(*,nr)
	goto 10
 60	read(*,nj,end=999)
	write(*,nj)
	goto 10
 70	read(*,nc,end=999)
	write(*,nc)
	goto 10
 80	read(*,nz,end=999)
	write(*,nz)
	goto 10
 90	read(*,nl,end=999)
	write(*,nl)
	goto 10
 999	end
./ ADD NAME=x0.f TIME=631570454
	integer i, ia(2:4, 4:6, 7:10)
	double precision x, xa(2,3)
	character*7 c, ca(3,4)
	namelist /nml/ i, ia, x, xa, c, ca
 10	read(*,nml,end=20)
	write(*,nml)
	goto 10
 20	end
./ ADD NAME=y.f TIME=631640584
	character*4 x, y*8
 10	read(*,*,end=20) x,y
	write(*,*) 'x = ', x, '; y = ', y
	goto 10
 20	end
./ ENDUP
