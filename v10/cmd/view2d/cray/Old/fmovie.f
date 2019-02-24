	real f(15,10)
	integer i, j
	do 100 i=1,15
	  do 100 j=1,10
	    f(i,j) = i*i + 3*j*j
100	continue
	call fmovie(15,10,23.,-1e6,f)
	end
