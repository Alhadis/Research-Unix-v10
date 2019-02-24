(* Copyright 1989 by AT&T Bell Laboratories *)
functor VaxCM(V : VAXCODER) : CMACHINE = struct

structure V' : sig datatype Register = reg of int

		   eqtype Label sharing type Label = V.Label
		   datatype EA = direct of Register
			    | autoinc of Register
			    | autodec of Register
			    | displace of int * Register
			    | deferred of int * Register
			    | immed of int
			    | immedlab of Label
			    | address of Label
			    | index of EA * Register

		end = V
open V' System.Tags

datatype condition = NEQ | EQL | LEQ | GEQ | LSS | GTR

fun defer(direct r) = displace(0,r)
  | defer(displace z) = deferred z
  | defer(immedlab lab) = address lab
  | defer _ = ErrorMsg.impossible "defer in cpsvax"

val sp' = reg 14
val exnptr = direct(reg 13)
val dataptr as direct dataptr' = direct(reg 12)
val datalimit = direct(reg 8)
val arithtemp as direct arithtemp' = direct(reg 9)
val arithtemp2 = direct(reg 10)
val storeptr = direct(reg 11)
val standardclosure = direct(reg 2)
val standardarg = direct(reg 0)
val standardcont = direct(reg 1)
val miscregs = map (direct o reg) [3,4,5,6,7]

fun newlabel() = immedlab(V.newlabel())
fun emitlab(i,immedlab lab) = V.emitlab(i,lab)
fun define (immedlab lab) = V.define lab

fun beginStdFn _ = ()

(* checkLimit (n):
 * Generate code to check the heap limit to see if there is enough free space
 * to allocate n bytes.
 *)
fun checkLimit maxAllocSize = (
      V.comment ("begin fun, max alloc = "^(makestring maxAllocSize)^"\n");
    if maxAllocSize <= 4096
     then V.addl3(dataptr,datalimit,arithtemp)
     else (V.addl3(dataptr,immed(maxAllocSize-4096+70),arithtemp);
(* the +70 in the line above is to force the immed literal to be stored
     as a 5-byte addressing mode, rather than a 1-byte; this makes things
    simpler for the runtime system *)
           V.addl2(datalimit,arithtemp)))

val align = V.align
val mark = V.mark
val move = V.movl
val emitlong = V.emitlong
val realconst = V.realconst
val emitstring = V.emitstring

fun jmpindexb lab = V.jmp(index(defer lab, arithtemp'))

fun record(vl, z) =
    let open CPS
	val len = List.length vl
	fun f(i,nil) = ()
	  | f(i,(direct r, SELp(j,p))::rest) = f(i,(displace(j*4,r),p)::rest)
	  | f(i,z::(direct s, SELp(j,p))::rest) =
			f(i,z::(displace(4*j,s),p)::rest)
	  | f(i,(x as direct(reg r), OFFp 0)::
		(y0 as (y as direct(reg s), OFFp 0))::rest) =
		if (s+1=r) then (V.movq(y,displace((i-1)*4,dataptr'));
				 f(i-2,rest))
		    else (V.movl(x,displace(i*4,dataptr')); f(i-1,y0::rest))
	  | f(i,(x as displace(j,r),OFFp 0)::(y as displace(k,s),OFFp 0)::rest) =
		if k+4=j andalso r=s
		     then (V.movq(y,displace((i-1)*4,dataptr')); f(i-2,rest))
		     else (V.movl(x,displace(i*4,dataptr')); 
			   f(i-1,(y,OFFp 0)::rest))
	  | f(i,(x,OFFp 0)::rest) = (V.movl(x,displace(i*4,dataptr'));
				     f(i-1,rest))
	  | f(i,(displace kr,SELp(0,p))::rest) = f(i,(deferred kr,p)::rest)
	  | f(i,(direct r, OFFp j)::rest) = (V.moval(displace(j*4,r),
						    displace(i*4,dataptr'));
						 f(i-1,rest))
	  | f(i,(x,p)::rest) = (V.movl(x,arithtemp); f(i,(arithtemp,p)::rest))
      in f(len - 2, rev vl);
	 V.movl(dataptr,z);
         V.addl2(immed(4*len), dataptr)
     end

fun select(i, direct r, s) = V.movl(displace(i*4,r),s)
  | select(0, a, s) = V.movl(defer a, s)

fun offset(i, direct r, s) = V.moval(displace(i*4,r),s)

val addl3 = V.addl3
val addl3t = addl3
val subl3 = V.subl3
val subl3t = subl3
val ashl = V.ashl
fun ashr(immed i, b, c) = V.ashl(immed (~i), b, c)
  | ashr(a,b,c) = (V.subl3(a,immed 0,arithtemp2);
				     (* potential bug, if generic is changed*)
		   V.ashl(arithtemp2,b,c))
val mull2 = V.mull2
val mull2t = mull2
val divl2 = V.divl2
val orb = V.bisl3
fun andb (a,b,c) = (V.subl3(a,immed ~1,arithtemp);  (* potential bug, if
					      generic.sml is changed! *)
		    V.bicl3(arithtemp,b,c))
fun notb (a,b) = V.subl3(a,immed(~1),b)
val xorb = V.xorl3

fun fetchindexl(v,w, immed 1) = V.movl(defer v, w)
  | fetchindexl(direct v, w, immed k) = V.movl(displace(2*k-2,v), w)
  | fetchindexl(v,w,i) = 
	    (V.ashl(immed ~1,i,arithtemp);
	     V.movl(index(defer v, arithtemp'),w))
fun storeindexl(v,w, immed 1) = V.movl(v, defer w)
  | storeindexl(v, direct w, immed k) = V.movl(v, displace(2*k-2, w))
  | storeindexl(v,w,i) = 
	    (V.ashl(immed ~1,i,arithtemp);
	     V.movl(v,index(defer w, arithtemp')))
fun fetchindexb(v,w) = V.movzbl(index(defer v, arithtemp'),w)
fun storeindexb(v,w) = V.movb(v,index(defer w, arithtemp'))

fun finishreal(y) = (V.movl(immed(8*power_tags + tag_string),
		     displace(~4,dataptr'));
		     V.movl(dataptr,y);
                     V.addl2(immed(4*3), dataptr))

fun mnegg(x,y) = (V.mnegg(defer x, defer dataptr); finishreal y)

fun realop f = fn (a,b,c) => (f(defer a, defer b, defer dataptr);
			      finishreal c)
val mulg3 = realop V.mulg3
val divg3 = realop (fn (a,b,c) => V.divg3 (b,a,c))
val addg3 = realop V.addg3
val subg3 = realop (fn (a,b,c) => V.subg3 (b,a,c))

fun cbranch NEQ = V.bneq
  | cbranch EQL = V.beql
  | cbranch LEQ = V.bleq
  | cbranch GEQ = V.bgeq
  | cbranch LSS = V.blss
  | cbranch GTR = V.bgtr

fun ibranch (cond, op1, op2, label) =
	(V.cmpl(op1, op2); cbranch cond (defer label))

fun gbranch (cond, op1, op2, label) =
	(V.cmpg(defer op1, defer op2); cbranch cond (defer label))

fun defer' j = fn x => j(defer x)
val jmp = defer' V.jmp
val bbs = fn(x,y,l) => V.bbs(x,y, defer l)

fun isimmed(immed i) = SOME i
  | isimmed _ = NONE

fun isreg(direct(reg i)) = SOME i | isreg _ = NONE
fun eqreg (a: EA) b = a=b

fun profile(i,incr) = if i >= Profile.PROFSIZE
			then ErrorMsg.impossible ("Bad profiling in vax: trying "
						  ^makestring i^" with size "
						  ^makestring Profile.PROFSIZE)
			else V.addl2(immed incr, displace(4*i,sp'))

val comment = V.comment
end
