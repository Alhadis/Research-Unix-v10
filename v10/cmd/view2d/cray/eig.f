      integer unit, nx, ny
      parameter (nx=20,ny=20)
      real f(nx,ny), fmin, fmax, outsid
      real pi, z
      real time
      unit=30
      time=0
      outsid = -99999
      pi=4*atan(1.)
      z=2*pi/(nx-1)
      do 200 k=1,1
      do 100 i=1,nx
        do 100 j=1,ny
          f(i,j) = sqrt(k+5.)*sin((i+2*j+0.2*k-2)*z)
     $               *cos((2*i-j+0.5*k-1.5)*z)
     $            + .2
  100 continue
      fmin=0
      fmax=0
      call mvefil(unit,time,nx,ny,fmin,fmax,outsid,f)
      time = time + .1
  200 continue
      end
