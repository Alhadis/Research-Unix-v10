      integer count(10)
      real val(10)
      read(*,*) count, val
      read(*,*) (count(i), val(i), i = 1, 10)
      read(*,*,err=10) count, val
c      read(*,*,end=10,err=20) count,val
 20   continue
 10	end
