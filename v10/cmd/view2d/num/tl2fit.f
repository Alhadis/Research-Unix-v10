      subroutine tl2fit(nx,ny,nn,d,w,c)
c     planar l2 fit  - tensor grid version
c     input:
c            nx = number of points in x
c            ny = number of points in y
c            nn = nx*ny
c             d = data points
c                 on output,  d(*,*) contains residuals
c             w = workspace, of size 6*n*n
c                 internal blocking:
c                  1..3  x
c                  4     y
c                  5     qraux
c                  6     qty
c             c = coefficients    c(1) + c(2)*x + c(3)*y
      integer i, j, k, n
      real c(3), w(nn,6), d(nx,ny), z
      integer i,info
      k=1
      do 100 i=1,nx
      do 100 j=1,ny
        w(k,1)=1
        w(k,2)=(i-1)/(nx-1.)
        w(k,3)=(j-1)/(ny-1.)
        w(k,4)=d(i,j)
        k=k+1
 100    continue
      call sqrdc(w,nn,nn,3,w(1,5),z,z,0)
      call sqrsl(w,nn,nn,3,w(1,5),w(1,4),z,w(1,6),c,z,z,100,info)
      if(info.ne.0)then
        write(6,1001) info
 1001   format(' sqrsl abort. info=',i5)
        stop
        end if
      end
