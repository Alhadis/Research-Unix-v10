	subroutine s5unpk(a,b,n)
c	a(1), a(2), ... contain characters in a4 format
c	s5unpk unpacks the first n characters
c	and places them into b(1), b(2), ... b(n)
c	in a1 format.(Caller thinks a and b are integer arrays)
c	Each element of b must be padded with trailing blanks.
c	Unused elements of b are unaffected.
	integer i,n
	character *120 a
	character *480 b
	blank = 32
c	write(6,3000)
c3000	format("entering unpakc")
	do 1 i=0,n-1
	b(4*i+1:4*i+1) = a(i+1:i+1)
	b(4*i+2:4*i+2) = ' '
	b(4*i+3:4*i+3) = ' '
	b(4*i+4:4*i+4) = ' '
1	continue
400	format( a4)
c	write(6,4000)
c4000	format("leaving unpk")
	return
	end
