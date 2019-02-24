	subroutine foo(f, g)
	external f
	double precision g
	call goo(f,g(3))
	end
	logical function f(n)
	f = n .gt. 3
	end
	external f, g
	call foo(f, g)
	end
