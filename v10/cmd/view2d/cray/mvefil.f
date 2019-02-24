      subroutine mvefil (unit,time,nx,ny,fmin,fmax,outsid,f)
*
*     mvefil writes the array f(nx,ny) in a format for view2d movies.
*     input:
*          unit   = integer variable specifying output unit
*          time   = real variable specifying time
*          nx     = integer variable specifying number of x points
*          ny     = integer variable specifying number of y points
*          fmin   = real variable specifying minimum of f(i,j)
*          fmax   = real variable specifying maximum of f(i,j)
*               If fmin=fmax on input, the range will be set inside.
*          outsid  = real variable specifying points outside region
*                   if f(i,j) <= outsid => point is outside region
*          f      = real array of points to be plotted
*                   f(i,j)  i = 1,...,nx  j = 1,...,ny
*   suggested jcl:
* DISPOSE,DN=FT30,MF=XH,DC=ST,TEXT='|/b2/c1127/ehg/rks astro/swift file'.    
*
*  Each data point is represented in the packed ASCII character format
*     by a 12 bit integer between 0 and 4095.  
*             f(i,j)=fmin+(fmax-fmin)*icode(i,j)/4095
*     icode is packed as two 6 bit integers ( / hi / lo / ).  Each 6 bits
*     is represented as an printable ASCII character, 6 bits + ! (033 octal).
*     A point which lies outside the computation region is flaged with (~~).
*     written by R. Kent Smith,   May 1984
*       to supply programs written by Andrew Hume and Eric Grosse
      integer bits,shift,lbuf
      parameter (bits=64,lbuf=128)
      parameter (shift=2**(bits-16))
      integer high,low,zero
      data high,low,zero,otside/7700b,77b,2r!!,2r~~/
      integer hi,lo,buf(lbuf)
*
      integer unit,nx,ny
      real time
      real fmin,fmax,f(1)
*
      if (unit.le.0) return
*
      nxny=nx*ny
      if (fmin.ge.fmax) then
*
         fmin=1e500
         fmax=-fmin
         do 110 i=1,nxny
            c=f(i)
            if(c.gt.outsid) then
               fmin=amin1(c,fmin)
               fmax=amax1(c,fmax)
            end if
  110       continue
*
         end if
*
      write (unit,6000) time,nx,ny,fmin,fmax
 6000 format(1x,e12.6,2i6,1x,e12.6,1x,e12.6)
*
      if (fmax.gt.fmin) then
         slope=4095.0/(fmax-fmin)
      else
         slope=0.0
      end if
      nseg=(nxny-1)/lbuf
      num=nxny-nseg*lbuf
      lf=0
      do 220 iseg=1,nseg+1
*
         do 210 i=1,num
*
            code=slope*(f(lf+i)-fmin)
            index=int(code+0.5)
*
            lo=low.and.index
            hi=high.and.index
            ibuf=4*hi+lo+zero
            buf(i)=cvmgp(ibuf,otside,index)*shift
*
  210       continue
*
         write (unit,6100) (buf(i),i=1,num)
 6100    format(32a2)
*
         lf=lf+num
         num=lbuf
  220    continue
*
      return
      end
