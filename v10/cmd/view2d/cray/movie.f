define(N,50)  # size of sample grid in rho-sigma space.
define(M,11)  # number of samples in theta
define(RMAX,3.) # rho search limit
define(SMAX,3.) # sigma search limit

procedure
  integer i, j, k
  real theta, sigma, rho, fmin, fmax, outsid, f(N,N)
  real fun

  do k=1,M{
    theta = (k-1)*50./max(1,M-1)
    do j=1,N{
      sigma = j*SMAX/N
      do i=1,N{
        rho = i*RMAX/N
        f(i,j) = fun(rho,sigma,theta)
      }
    }
    fmin = 0
    fmax = 0
    outsid = -1.e20
    mvefil(30,theta,N,N,fmin,fmax,outsid,f)
  } 

end

real procedure fun ( rho, sigma, theta )
  real rho, sigma, theta
  real t, f, deg
  deg = 4 * atan(1.) / 180
  f = sin(rho*10*deg) * cos(sigma*10*deg)
  return( f )
end
