      common /cmname/ c
      complex c(10)
      double precision d(10)
      real r(10)
      integer i(10)
      logical l(10)
      equivalence (c(1),d(1),r(1),i(1),l(1))
      data c(1)/(1.,0.)/
      data d(2)/2d0/, r(5)/3e0/, i(6)/4/, l(7)/.true./
      call sam(c,d(1),r(2),i(3),l(4),'some hollerith',14)
      end
