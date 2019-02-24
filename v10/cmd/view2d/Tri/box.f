******************from mhuxt!rksmith on 25 Feb 85******************
*DECK INTBOX
      subroutine intbox (nx,xmin,xmax,ny,ymin,ymax,f,tr,
     *                   nv,fk,floor,vx,vy,itri)
      dimension f(nx,1),tr(nx,1),fk(1),vx(1),vy(1),itri(3,1)
c
c         intbox interpolates points from triangular grid
c         (fk,vx,vy) onto rectangular grid ( f(nx,ny) )
c
c  tr triangle indices
c  f  values on grid
c  floor   out-of-bounds
c  itri(3,nt)
      if (nx.lt.1.or.ny.lt.1) return
      if (nx.eq.1) then
         dx=1.0
      else
         dx=(xmax-xmin)/float(nx-1)
      end if
c
      if (ny.eq.1) then
         dy=1.0
      else
         dy=(ymax-ymin)/float(ny-1)
      end if
c
c             The main loop
c
      yp=ymin
      do 200 iy=1,ny
         xp=xmin
         do 100 ix=1,nx
            it=int(tr(ix,iy))
            if (it.lt.1) then
               f(ix,iy)=floor
            else
c
c      linear interpolation on triangle it
c
               iv1=itri(1,it)
               iv2=itri(2,it)
               iv3=itri(3,it)
c
               x13=vx(iv1)-vx(iv3)
               x23=vx(iv2)-vx(iv3)
               y13=vy(iv1)-vy(iv3)
               y23=vy(iv2)-vy(iv3)
               det=x13*y23-y13*x23
               c1=y23*(xp-vx(iv3))-x23*(yp-vy(iv3))
               c2=x13*(yp-vy(iv3))-y13*(xp-vx(iv3))
c
               df=c1*(fk(iv1)-fk(iv3))+c2*(fk(iv2)-fk(iv3))
               f(ix,iy)=fk(iv3)+df/det
c
            end if
  100       xp=xp+dx
  200    yp=yp+dy
c
      return
      end
*DECK MACBOX
      subroutine macbox (nv,vx,vy,nt,itri,
     *                   nx,xmin,xmax,ny,ymin,ymax,tr)
      dimension vx(1),vy(1),itri(3,1)
      dimension tr(nx,1)
c
c     find triangles containing box points
c
      if (nx.lt.1.or.ny.lt.1) return
      if (nx.eq.1) then
         dx=1.0
      else
         dx=(xmax-xmin)/float(nx-1)
         if (dx.eq.0.0) dx=1.0
      end if
      dx1=1.0/dx
      xdx=xmin-dx
c
      if (ny.eq.1) then
         dy=1.0
      else
         dy=(ymax-ymin)/float(ny-1)
         if (dy.eq.0.0) dy=1.0
      end if
      dy1=1.0/dy
      ydy=ymin-dy
c
c     search triangles for points
c
      eps=1.0e-5
      cnorm=1.0-4.0*eps
      do 202 iy=1,ny
         do 201 ix=1,nx
            tr(ix,iy)=0.0
  201       continue
  202    continue
c
      do 230 it=1,nt
c
         iv1=itri(1,it)
         iv2=itri(2,it)
         iv3=itri(3,it)
c
         vxmin=dx1*(amin1(vx(iv1),vx(iv2),vx(iv3))-xmin)+1.0
         ixmin=max0(int(vxmin),1)
         vxmax=dx1*(amax1(vx(iv1),vx(iv2),vx(iv3))-xmin)+1.0
         ixmax=min0(int(vxmax),nx)
         vymin=dy1*(amin1(vy(iv1),vy(iv2),vy(iv3))-ymin)+1.0
         iymin=max0(int(vymin),1)
         vymax=dy1*(amax1(vy(iv1),vy(iv2),vy(iv3))-ymin)+1.0
         iymax=min0(int(vymax),ny)
c
c        find box points contained in this macro triangle
c
         x13=vx(iv1)-vx(iv3)
         x23=vx(iv2)-vx(iv3)
         y13=vy(iv1)-vy(iv3)
         y23=vy(iv2)-vy(iv3)
         det=x13*y23-y13*x23
c
         x3=xdx-vx(iv3)
         y3=ydy-vy(iv3)
c
         do 220 iy=iymin,iymax
            yp=y3+float(iy)*dy
            do 210 ix=ixmin,ixmax
               xp=x3+float(ix)*dx
c
               c1=(y23*xp-x23*yp)/det
               if (c1+eps.lt.0.0) go to 210
               c2=(x13*yp-y13*xp)/det
               if (c2+eps.lt.0.0) go to 210
               c3=1.0-c1-c2
               if (c3+eps.lt.0.0) go to 210
c
                  tr(ix,iy)=float(it)+0.5
c
  210          continue
  220       continue
c
  230    continue
c
      return
      end


