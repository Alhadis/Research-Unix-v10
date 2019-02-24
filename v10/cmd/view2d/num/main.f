      real u(3), d(3,3), xyz(3,5), work(25)
      data u / .5, .7, 0 /
      data d / 0., 0., 0.,
     $         1., 0., 1.,
     $         0., 1., 10. /
      data xyz / 0., 0., 0.,
     $           1., 0., 0.,
     $           0., 1., 10.,
     $           0., 2., 20.5,
     $           0., 3., 29.4 /
      call interp(u,d(1,1), d(1,2),d(1,3))
      write(6,1001)u(3)
 1001 format(' interp:',f8.3)
c      call l2fit(5,xyz,u,work)
c      write(6,1002)u
 1002 format(' l2fit:',3f8.3)
      end
