	block data
	common /abc/ x
	integer x(4)
	data x/101,102,103,104/
	end
	common /uvw/ r, s
	complex r(4), s
	integer i, ia(4), j, k(4,4), L, m(4), n
	equivalence (ia(3),j), (k(3,2),L)
	automatic m, n
	data i/23/, ia/6,7,8,9/
	write(*,*) 'initial i =', i
	do 10 i = 1, 4
		m(i) = i + 1000
		r(i) = i + 2000
		do 10 j = 1, 4
 10			k(i,j) = 10*i + j
	s = i + 2000
	call foo(j,ia,k,L,m,n)
	call goo(4,k)
	end
	subroutine foo(j,ia,k,L,m,n)
	common /abc/ x
	integer x(4), y(2), m(4), n
	equivalence (x(2),y)
	common /uvw/ r, s
	complex r(4), s
	double complex t
	equivalence (r(3),t)

	integer j, ia(4), k(4,4)
	integer i, i2
	write(*,*) 'j, L, n, s, t =', j, L, n, s, t
	write(*,*) 'ia =',ia
	write(*,*) 'r = ', r
	do 10 i = 1, 4
 10		write(*,*) (k(i,i2), i2 = 1,4)
	write(*,*) 'y =', y
	write(*,*) 'm = ...', m
	end
	subroutine goo(n,k)
	integer n, k(n,n)
	integer i,j
	write(*,*) 'goo has k = ...'
	do 10 i = 1, n
 10		write(*,*) (k(i,j), j = 1, n)
	end
