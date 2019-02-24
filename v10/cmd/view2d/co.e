#  simple contour plotting
#    connect-the-dots, rather than approximate hyperbolas.
#    contour (implicitly) perturbed downward by infinitesimal.
#  extensively revised 12 Apr 1985
#  added NaN  9 Jul 1985

procedure g2co(w,d,n1,n2,f,nc,cl,NaN)
integer n1, n2, j0, j1, j2, nc
real w(6), d(2,2), f(n1,n2), cl(nc)
real c, dx(2), ll(2), NaN
integer j, jj, xp, nt
real fl, fh, t(2,4)
real swap
real xx(2,60)
dx(1)=(d(1,2)-d(1,1))/(n1-1)
dx(2)=(d(2,2)-d(2,1))/(n2-1)
xp=0
do j0 = 1,nc{
   c=cl(j0)
   do j1 = 1,n1-1{
      ll(1)=d(1,1)+(j1-1)*dx(1)
      do j2 = 1,n2-1{
         ll(2)=d(2,1)+(j2-1)*dx(2)
         #write(,j1:i(4),j2:i(4))
         #write(,f(j1,j2+1):f(7,1),f(j1+1,j2+1):f(7,1))
         #write(,f(j1,j2  ):f(7,1),f(j1+1,j2  ):f(7,1))

         #   find intersections of contours with sides
         nt=0
         #   ... west ...
         fl=f(j1,j2)
         fh=f(j1,j2+1)
         if(fl<=NaN) next
         if(fh<=NaN) next
         if( (c-fl)*(c-fh)<0 |
               c==fl & fh<c  |
               c==fh & fl<c  ){
            nt=nt+1
            t(1,nt)=0
            t(2,nt)=(c-fl)/(fh-fl)
         }
         #   ... north ...
         fl=fh
         fh=f(j1+1,j2+1)
         if(fh<=NaN) next
         if( (c-fl)*(c-fh)<0 |
               c==fl & fh<c  |
               c==fh & fl<c  ){
            nt=nt+1
            t(1,nt)=(c-fl)/(fh-fl)
            t(2,nt)=1
         }
         #   ... south ...
         fl=f(j1,j2)
         fh=f(j1+1,j2)
         if(fh<=NaN) next
         if( (c-fl)*(c-fh)<0 |
               c==fl & fh<c  |
               c==fh & fl<c  ){
            nt=nt+1
            t(1,nt)=(c-fl)/(fh-fl)
            t(2,nt)=0
         }
         #   ... east ...
         fl=fh
         fh=f(j1+1,j2+1)
         if( (c-fl)*(c-fh)<0 |
               c==fl & fh<c  |
               c==fh & fl<c  ){
            nt=nt+1
            t(1,nt)=1
            t(2,nt)=(c-fl)/(fh-fl)
         }

         #  swap north and south to correctly match with east and west
         if(nt==4){
            if( t(1,2)>t(1,3) |
                (t(1,2)==t(1,3) & f(j1,j2)<f(j1+1,j2))  ){
               swap=t(1,2)
               t(1,2)=t(1,3)
               t(1,3)=swap
               swap=t(2,2)
               t(2,2)=t(2,3)
               t(2,3)=swap
            }
         }

         if ( nt==2 | nt==4 ){
           do jj=1,nt/2{
             j=2*jj-1
             if( t(1,j+1)!=t(1,j) | t(2,j+1)!=t(2,j) ){
               xp=xp+2
               xx(1,xp-1)=ll(1)+dx(1)*(t(1,j))
               xx(2,xp-1)=ll(2)+dx(2)*(t(2,j))
               xx(1,xp)=ll(1)+dx(1)*(t(1,j+1))
               xx(2,xp)=ll(2)+dx(2)*(t(2,j+1))
             }
           }
         }else if( nt!=0 ){
           write(,"can't happen!  nt=",nt,j1,j2)
           write(,f(j1,j2+1),f(j1+1,j2+1))
           write(,f(j1,j2  ),f(j1+1,j2  ))
           stop
         }

         if(xp>=50){
            g2la(w,xx,xp)
            xp=0
            }

         }  # do j2
      } # do j1

   if(xp>0){
      g2la(w,xx,xp)
      xp=0
      }
   } # do j0
end
