procedure g2sc(s,x,n)
integer n, i, j
real s(6), x(2,n), denom, xmin(2), xmax(2)
do i=1,2{
   xmin(i)=x(i,1)
   xmax(i)=x(i,1)
   do j=2,n{
      xmin(i)=amin1(xmin(i),x(i,j))
      xmax(i)=amax1(xmax(i),x(i,j))
      }
   denom=xmax(i)-xmin(i)+1.0e-4*(abs(xmax(i))+abs(xmin(i)))+1.0e-12
   s(i)=1.0e0/denom
   s(i+4)=-s(i)*xmin(i)
   }
s(4)=s(2)
s(2)=0
s(3)=0
end
#-------------------------------------------------------------------
procedure g2sce(s,x,n)
integer n, i, j
real s(6), x(2,n), denom, xmin(2), xmax(2)
denom=1.0e-20
do i=1,2{
   xmin(i)=x(i,1)
   xmax(i)=x(i,1)
   do j=2,n{
      xmin(i)=amin1(xmin(i),x(i,j))
      xmax(i)=amax1(xmax(i),x(i,j))
      }
   denom=amax1(denom,
      xmax(i)-xmin(i)+1.0e-4*(abs(xmax(i))+abs(xmin(i)))+1.0e-12)
   }
s(1)=1/denom
s(2)=0
s(3)=0
s(4)=1/denom
s(5)=0.5-0.5*s(1)*(xmax(1)+xmin(1))
s(6)=0.5-0.5*s(4)*(xmax(2)+xmin(2))
end
#------------------------------------------------------------------
procedure g2box(s,d)
real box(2,5), s(6), d(2,2)
box(1,1)=d(1,1)
box(2,1)=d(2,1)
box(1,2)=d(1,2)
box(2,2)=d(2,1)
box(1,3)=d(1,2)
box(2,3)=d(2,2)
box(1,4)=d(1,1)
box(2,4)=d(2,2)
box(1,5)=d(1,1)
box(2,5)=d(2,1)
g2li(s,box,5)
end
#------------------------------------------------------------------
procedure g2gr(s,n1,n2)
integer n1,n2,j1,j2
real s(6),x(2,1)
do j2=1,n2{
   x(2,1)=j2
   do j1=1,n1{
      x(1,1)=j1
      g2sy(s,x,1,'+')
      }
   }
end
#------------------------------------------------------------------
procedure g2ti(s,d)
integer i, lab1, nlab, labk
integer ifloor
real s(6), p(2,2), tic1(2), tic2(2)
real d(2,2), l, h
l=d(1,1)
h=d(1,2)
labk=ifloor(alog10((h-l)/2.0001))
lab1=ifloor(0.0001+l*10.0**(-labk))
nlab=ifloor(0.0001+h*10.0**(-labk))-lab1+1
tic1(1)=lab1*10.0**labk
tic1(2)=(lab1+nlab-1)*10.0**labk
do i=1,nlab{
   p(2,1)=d(2,1)-0.01*(d(2,2)-d(2,1))
   p(1,1)=(lab1+i-1)*10.0**labk
   p(2,2)=d(2,1)
   p(1,2)=p(1,1)
   g2li(s,p,2)
   }
l=d(2,1)
h=d(2,2)
labk=ifloor(alog10((h-l)/2.0001))
lab1=ifloor(0.0001+l*10.0**(-labk))
nlab=ifloor(0.0001+h*10.0**(-labk))-lab1+1
tic2(1)=lab1*10.0**labk
tic2(2)=(lab1+nlab-1)*10.0**labk
do i=1,nlab{
   p(1,1)=d(1,1)-0.01*(d(1,2)-d(1,1))
   p(2,1)=(lab1+i-1)*10.0**labk
   p(1,2)=d(1,1)
   p(2,2)=p(2,1)
   g2li(s,p,2)
   }
#write(,{" tics ":c,tic1:e(15,5)," to ":c,tic2:e(15,5)})
end
#------------------------------------------------------------------
procedure number ( s, q, i )
   real q(2)
   integer i, digits(10)
   real s(6)
   initial digits = ("0","1","2","3","4","5","6","7","8","9")
   g2sy(s,q,1,digits(1+mod(i,10)))
   end
#------------------------------------------------------------------
integer function ifloor(x)
real x
ifloor=int(x)
if(x<ifloor){ifloor=ifloor-1}
end
#------------------------------------------------------------------
procedure g2ca(s,d,n1,n2,f)
integer n1, n2, j1, j2
real s(6), d(2,2), f(n1,n2), c(5)
real fmax, fmin
fmax=-1e30
fmin=-fmax
#write(,{" n1,n2=",n1:i(10),n2:i(10)})
do j1=1,n1{
   do j2=1,n2{
      fmax=amax1(fmax,f(j1,j2))
      fmin=amin1(fmin,f(j1,j2))
      }}
#write(,{"fmax, fmin=":c,fmax:e(15,3),fmin:e(15,3)})
do j1=1,5{
   c(j1)= fmin+j1*(fmax-fmin)/6
   #write(,{" contour ":c,c(j1):e(15,7)})
   }
g2co(s,d,n1,n2,f,5,c)
end
#  g2   a simple plot package
#       NPLOT VERSION
#       (ehg 30 aug 84)
#
#  g2open    initialize system.
#  g2ff      start a picture.
#  g2clos    finish up plotting.
#  g2sc(s,x,n)  set up coordinate transform
#    where  x is a collection of points dimensioned (2,n)
#    and on output s is a coordinate transform
#       [ w(1) ]   [ s(1) s(2) ] [ x(1) ]   [ s(5) ]
#       [      ] = [           ] [      ] + [      ]
#       [ w(2) ]   [ s(3) s(4) ] [ x(2) ]   [ s(6) ].
#  g2sce(s,x,n)  does the same, but forces s(1)=s(4)
#     (so that circles come out as circles and not ellipses)
#  g2li(s,x,n)  draws lines between the points x.
#  g2lit(j)  sets the line type.
#    where j=0 solid, =1 dash, =2 dots.
#  g2sy(s,x,n,'+')  puts a '+' at each of the points x.
#  g2arc(s,center,start,stop) draws a circular arc counterclockwise from start
#    to stop, about specified center
#  g2ti(s,d)  draws tic marks around a rectangle where d is dimensioned (2,2),
#    d(,1)=lower left corner, and d(,2)=upper right corner.
#  g2box(s,d)  draws the rectangle d
#  g2co(s,d,n1,n2,f,nc,c)  draws contours at levels c(j), 1<=j<=nc
#    for function values f given on a n1 by n2 array corresponding 
#    to rectangle d.
#
#------------------------------------------------------------------
procedure g2open
write(,"..o")
write(,"..ra -0.1 -0.1 1.1 1.1")
end
#------------------------------------------------------------------
procedure g2ff
integer frame
initial frame = 0
frame=frame+1
if ( frame > 1 ) { write(,"..pau"); write(,"..e") }
end
#------------------------------------------------------------------
procedure g2clos
write(,"..cl")
end
#------------------------------------------------------------------
procedure g2tx(s,x,n,char)
#  write n characters at x
integer n, i, j
real char(1), s(6), x(2), xp, yp
  xp=s(1)*x(1)+s(2)*x(2)+s(5)
  yp=s(3)*x(1)+s(4)*x(2)+s(6)
  write(,"..m ",xp:f(10,7),yp:f(10,7))
  write(,"..t ",char:c(4))
end
#------------------------------------------------------------------
procedure g2li(s,x,n)
integer n, j
real s(6), x(2,n), x1, y1
j=1
x1=s(1)*x(1,j)+s(2)*x(2,j)+s(5)
y1=s(3)*x(1,j)+s(4)*x(2,j)+s(6)
write(,"..m ",x1:f(10,7),y1:f(10,7))
do j=2,n{
  x1=s(1)*x(1,j)+s(2)*x(2,j)+s(5)
  y1=s(3)*x(1,j)+s(4)*x(2,j)+s(6)
  write(,"..v ",x1:f(10,7),y1:f(10,7))
  }
end
#------------------------------------------------------------------
procedure g2la(s,x,n)
# (like g2li, but only connect pairs of points)
integer n, j
real s(6), x(2,n), xb, xf, yb, yf
do j=1,n/2{
  xb=s(1)*x(1,2*j-1)+s(2)*x(2,2*j-1)+s(5)
  yb=s(3)*x(1,2*j-1)+s(4)*x(2,2*j-1)+s(6)
  xf=s(1)*x(1,2*j)  +s(2)*x(2,2*j)  +s(5)
  yf=s(3)*x(1,2*j)  +s(4)*x(2,2*j)  +s(6)
  write(,"..li ",xb:f(10,7),yb:f(10,7),xf:f(10,7),yf:f(10,7))
  }
end
#------------------------------------------------------------------
procedure g2lit(l)
integer l
if(l==0){
  write(,"..co white/solid/H*")
  }
else if(l==1){
  write(,"..co red/longdashed/H#")
  }
else if(l==2){
  write(,"..co green/dotdashed/H$")
  }
end
#------------------------------------------------------------------
procedure g2sy(s,x,n,char)
integer n, j
real char
real s(6), x(2,n), xp, yp
do j=1,n{
  xp=s(1)*x(1,j)+s(2)*x(2,j)+s(5)
  yp=s(3)*x(1,j)+s(4)*x(2,j)+s(6)
  write(,"..m ",xp:f(10,7),yp:f(10,7))
  write(,"..t ",char:c(1))
  }
end
#------------------------------------------------------------------
procedure g2arc ( s, c, a, b )
  real s(6), c(2), a(2), b(2), radius, start
  real finish, pi, x(2), y(2), z(2)
  pi=4*atan(1.0e0)
  x(1)=s(1)*c(1)+s(2)*c(2)+s(5)
  x(2)=s(3)*c(1)+s(4)*c(2)+s(6)
  y(1)=s(1)*a(1)+s(2)*a(2)+s(5)
  y(2)=s(3)*a(1)+s(4)*a(2)+s(6)
  z(1)=s(1)*b(1)+s(2)*b(2)+s(5)
  z(2)=s(3)*b(1)+s(4)*b(2)+s(6)
  radius=sqrt((x(1)-y(1))**2+(x(2)-y(2))**2)
  if ( radius > 5. ) {
     write(,"..li ",y(1):f(10,7),y(2):f(10,7),z(1):f(10,7),z(2):f(10,7))
     }
  else {
     start=atan2(y(2)-x(2),y(1)-x(1))
     finish=atan2(z(2)-x(2),z(1)-x(1))
     write(,"..a ",y:f(10,7),z:f(10,7),x:f(13,7),radius:f(13,7))
     }
  end
