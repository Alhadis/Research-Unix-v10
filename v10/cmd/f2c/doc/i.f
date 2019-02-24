	subroutine zoo
	external f
	double precision f
	external g
	call zap(1,f)
	call zap(2,g)
	end
	subroutine goo
	call g
	end
