      subroutine interp(u,d1,d2,d3)
c     interpolation in a triangle
c     input:  u = point to be interpolated
c             d1..3 = 3 points already known
c                 first subscript: xyz, second: point index
c     (output is in third component of u)
      real u(3)
      real d1(3),d2(3),d3(3)
      integer i,ipvt(3),info
      real a(3,3), r(3)
      do 100 i=1,3
        a(i,1)=1
 100   continue
      do 101 i=2,3
        a(1,i)=d1(i-1)
        a(2,i)=d2(i-1)
        a(3,i)=d3(i-1)
 101   continue
      r(1)=d1(3)
      r(2)=d2(3)
      r(3)=d3(3)
      call sgefa(a,3,3,ipvt,info)
      if(info.ne.0)then
c        write(0,1001) info,u,d1,d2,d3
c 1001   format(' sgefa abort. info=',i5,/4(3e12.4/))
c        stop 1
         if(d1(1).eq.d2(1)) then
            if(d1(2).eq.d2(2)) then
               u(3) = d1(3)
            else
               u(3) = d1(3)+(d2(3)-d1(3))*(u(2)-d1(2))/(d2(2)-d1(2))
            end if
         else
            u(3) = d1(3)+(d2(3)-d1(3))*(u(1)-d1(1))/(d2(1)-d1(1))
         end if
	 return
      end if
      call sgesl(a,3,3,ipvt,r,0)
      u(3)=r(1)+r(2)*u(1)+r(3)*u(2)
      end
