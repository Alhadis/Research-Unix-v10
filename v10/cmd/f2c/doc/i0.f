	subroutine grok(f,g,h)
	external f, g, h
	logical g
	call foo(1,g)
	call foo(2,f)
	call zit(1,f)
	call zit(2,h)
	call zot(f(3))
	end
