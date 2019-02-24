	subroutine s5pack(a,b,n)
c	integers a(1), a(2), ...a(n) contain characters in a1 format.
c	the first byte of each integer contains the character
c	the three remaining bytes are blank. note that the calling program
c	expects a and b to be integer arrays, but we treat a and b
c	internally as character arrays with the correspondence
c	character a(4*i+1;4*i+1)=first byte of integer a(i+1)
c	s5pack(a,b,n) packs these characters into
c	elements of the integer array b in a4 format using only as many
c	elements of b as necessary.
c	The last element of b used must be padded
c	with trailing blanks. Unused elements of b are not affected.
	character *480 a
	character b
	integer n,i,k
c	write(6,1000)
c1000	format("entered s5pack")
	do 1 i=0,n-1
	b(i+1:i+1) = a(4*i+1:4*i+1)
1	continue
	k = mod(n,4)
	if(k.gt.0) then
	do 2 i=n+1,n+4-k
	b(i:i) = ' '
2	continue
	end if
c	write(6,2000)
c2000	format("leaving s5pak")
	return
	end
