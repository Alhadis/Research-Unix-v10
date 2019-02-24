(* Copyright 1989 by AT&T Bell Laboratories *)
(* The following functions were adapted from the 4.3BSD math library.
   Eventually, each machine supported should have a hand-coded math
   functor with more efficient versions of these functions.

***************************************************************************
*                                                                         * 
* Copyright (c) 1985 Regents of the University of California.             *
*                                                                         * 
* Use and reproduction of this software are granted  in  accordance  with *
* the terms and conditions specified in  the  Berkeley  Software  License *
* Agreement (in particular, this entails acknowledgement of the programs' *
* source, and inclusion of this notice) with the additional understanding *
* that  all  recipients  should regard themselves as participants  in  an *
* ongoing  research  project and hence should  feel  obligated  to report *
* their  experiences (good or bad) with these elementary function  codes, *
* using "sendbug 4bsd-bugs@BERKELEY", to the authors.                     *
*                                                                         *
* K.C. Ng, with Z-S. Alex Liu, S. McDonald, P. Tang, W. Kahan.            *
* Revised on 5/10/85, 5/13/85, 6/14/85, 8/20/85, 8/27/85, 9/11/85.        *
*                                                                         *
***************************************************************************

*)

signature MATH =
  sig
    exception Sqrt and Exp and Ln
    val sqrt : real -> real
    val sin : real -> real
    val cos : real -> real
    val arctan : real -> real
    val exp : real -> real
    val ln : real -> real
  end

structure Math : MATH = struct

structure Assembly : ASSEMBLY = Core.Assembly

infix 7 * /
infix 6 + -
infix 4 > < >= <=

exception Real = Assembly.Real
exception Sqrt
exception Exp
exception Ln

val scalb = Assembly.A.scalb
val logb = Assembly.A.logb

val ~ = InLine.fneg
val op + = InLine.fadd
val op - = InLine.fsub
val op * = InLine.fmul
val op / = InLine.fdiv
val op > = InLine.fgt
val op < = InLine.flt
val op >= = InLine.fge
val op <= = InLine.fle

val ieql = InLine.ieql
val ineq = InLine.ineq
val feql = InLine.feql

val negone = ~1.0
val zero = 0.0
val half = 0.5
val one = 1.0
val two = 2.0
val four = 4.0

fun copysign(a,b) =
      case (a<zero,b<zero) of
	      (true,true) => a
	    | (false,false) => a
	    | _ => ~a

fun abs x = if x < zero then ~x else x
fun mod(a,b) = InLine.-(a,InLine.*(InLine.div(a,b),b))
local fun rtoi (pred,init) =
	  let fun loop n =
	      if pred n
	      then init
	      else let val (i,r) = loop(n * half)
		       val i' = InLine.lshift(i,1)
		       val r' = r+r
		   in if n-r' < one then (i',r') else (InLine.+(i',1),r'+one)
		   end
	  in loop
	  end
      val pton = rtoi(fn x => x < one, (0,zero))
      val nton = rtoi(fn x => x >= negone, (~1,negone))
      fun fl n = if n < zero then nton n else pton n
      fun tr n = #1(pton n)
in 
fun truncate n = if n < zero then InLine.~(tr(~n)) else tr n
val floor = Assembly.A.floor
fun realfloor n = #2(fl n)
fun ceiling n = InLine.~(floor(~n))
end
local fun loop 0 = zero
        | loop n = let val x = two * loop(InLine.rshift(n,1))
		   in case InLine.andb(n,1) of 0 => x | 1 => x + one
		   end
in fun real n = if InLine.<(n,0) then ~(loop(InLine.~ n)) else loop n
end

(* Not a true drem - rounding on .5 should go to the even case. *)
fun drem(x,y) =
    let val n = x/y+half
	val N = if abs n < 9.007199254741E15 (* about 2^53 *)
		then realfloor n
		       handle Real _ =>
			 scalb(realfloor(scalb(n,InLine.~(logb n))),logb n)
		else n
    in  x - N * y
    end

(* sin/cos *)
local
    val S0 = ~1.6666666666666463126E~1
    val S1 =  8.3333333332992771264E~3
    val S2 = ~1.9841269816180999116E~4
    val S3 =  2.7557309793219876880E~6
    val S4 = ~2.5050225177523807003E~8
    val S5 =  1.5868926979889205164E~10
in  fun sin__S z = (z*(S0+z*(S1+z*(S2+z*(S3+z*(S4+z*S5))))))
end

local
    val C0 =  4.1666666666666504759E~2
    val C1 = ~1.3888888888865301516E~3
    val C2 =  2.4801587269650015769E~5
    val C3 = ~2.7557304623183959811E~7
    val C4 =  2.0873958177697780076E~9
    val C5 = ~1.1250289076471311557E~11
in  fun cos__C z = (z*z*(C0+z*(C1+z*(C2+z*(C3+z*(C4+z*C5))))))
end

val PIo4   =  7.8539816339744827900E~1
val PIo2   =  1.5707963267948965580E0
val PI3o4  =  2.3561944901923448370E0
val PI     =  3.1415926535897931160E0
val PI2    =  6.2831853071795862320E0

local
    val thresh =  2.6117239648121182150E~1
in  fun S y = y + y * sin__S(y*y)
    fun C y =
	let val yy = y*y
	    val c = cos__C yy
	    val Y = yy/two
	in  if Y < thresh then one - (Y - c)
	    else half - (Y - half - c)
	end
end
    fun sin x =
	let val x = drem(x,PI2)
	    val a = abs x
	    val y = if a >= PI3o4 then copysign(PI-a,x) else a
	in  if y >= PIo4 then copysign(C(PIo2-a),x)
	    else S y
	end

    fun cos x =
	let val x = drem(x,PI2)
	    val a = abs x
	    val (s,y) = if a >= PI3o4
			    then (fn x => ~x,PI-a)
			    else (fn x => x,a)
	in  if y < PIo4 then s(C y)
	    else S(PIo2-a)
	end

local
    val p1 =  1.3887401997267371720E~2
    val p2 =  3.3044019718331897649E~5
    val q1 =  1.1110813732786649355E~1
    val q2 =  9.9176615021572857300E~4
in  fun exp__E(x:real,c:real) =
	let val small=1.0E~19
	    val x = abs x
	    val z = x*x
	    val p = z*(p1+z*p2)
	    val q = z*(q1+z*q2)
	    val xp= x*p 
	    val xh= x*half
	    val w = xh-(q-xp)
	    val p = p+p
	    val c = c+x*((xh*w-(q-(p+xp)))/(one-w)+c)
	in  if(copysign(x,one)>small) then z*half+c
	    else copysign(zero,x) (* could be inexact *)
	end
end

(* for exp and ln *)
val ln2hi = 6.9314718036912381649E~1
val ln2lo = 1.9082149292705877000E~10
val sqrt2 = 1.4142135623730951455E0
val lnhuge =  7.1602103751842355450E2
val lntiny = ~7.5137154372698068983E2
val invln2 =  1.4426950408889633870E0

fun exp(x:real) =
    let fun exp_norm() =
	    let (* argument reduction : x --> x - k*ln2 *)
		val k = floor(invln2*x+copysign(half,x)) (* k=NINT(x/ln2) *)
		val K = real k
		(* express x-k*ln2 as z+c *)
		val hi = x-K*ln2hi
		val lo = K*ln2lo
		val z = hi - lo
		val c = (hi-z)-lo
		(* return 2^k*[expm1(x) + 1] *)
		val z = z + exp__E(z,c)
	    in  scalb(z+one,k)
	    end
    in	if x <= lnhuge then if x >= lntiny
			    then exp_norm() handle Real _ => raise Exp
			    else zero (* exp(-big#) underflows to zero *)
				      (* exp(-INF) is zero *)
	else raise Exp (* exp(INF) is INF, exp(+big#) overflows to INF *)
    end

local
    val L1 = 6.6666666666667340202E~1
    val L2 = 3.9999999999416702146E~1
    val L3 = 2.8571428742008753154E~1
    val L4 = 2.2222198607186277597E~1
    val L5 = 1.8183562745289935658E~1
    val L6 = 1.5314087275331442206E~1
    val L7 = 1.4795612545334174692E~1
in  fun log__L(z) = z*(L1+z*(L2+z*(L3+z*(L4+z*(L5+z*(L6+z*L7))))))
end

fun ln(x:real) =
    let fun findln() =
	let (* argument reduction *)
	    val k = logb(x)
	    val x = scalb(x,InLine.~ k)
	    val (x,k) = if ieql(k,~1022) (* subnormal no. *)
	    	    then let val n = logb(x)
	    		 in  (scalb(x,InLine.~ n),InLine.+(k,n)) end
	    	    else (x,k)
	    val (k,x) = if x >= sqrt2 then (InLine.+(k,1),x*half) else (k,x)
	    val K = real k
	    val x = x - one
	    (* compute log(1+x) *)
	    val s = x/(two+x)
	    val t = x*x*half
	    val z = K*ln2lo+s*(t+log__L(s*s))
	    val x = x + (z - t)
	in  K*ln2hi+x end
    in  if x <= zero then raise Ln
	else findln() handle Real _ => raise Ln
    end

local
    val small=1.0E~9
    val big=1.0E18
    val athfhi =  4.6364760900080609352E~1
    val athflo =  4.6249969567426939759E~18
    val at1fhi =  9.8279372324732905408E~1
    val at1flo = ~2.4407677060164810007E~17
    val a1     =  3.3333333333333942106E~1
    val a2     = ~1.9999999999979536924E~1
    val a3     =  1.4285714278004377209E~1
    val a4     = ~1.1111110579344973814E~1
    val a5     =  9.0908906105474668324E~2
    val a6     = ~7.6919217767468239799E~2
    val a7     =  6.6614695906082474486E~2
    val a8     = ~5.8358371008508623523E~2
    val a9     =  4.9850617156082015213E~2
    val a10    = ~3.6700606902093604877E~2
    val a11    =  1.6438029044759730479E~2
in
fun atan2(y:real,x:real) =
    let (* copy down the sign of y and x *)
	val signy = copysign(one,y)
	val signx = copysign(one,x)
	exception Done of real
	fun findatan2(x,y,t) =
	    let (* begin argument reduction *)
		val k = floor(four * (t+0.0625))
		val (hi,lo,t) =
			if t < 2.4375 then
			(* truncate 4(t+1/16) to integer for branching *)
			case k of
			(* t is in [0,7/16] *)
			      0 => if t < small
				   then raise Done
					   (copysign(if signx>zero then t 
								   else PI-t,
						     signy))
				   else (zero,zero,t)
			    | 1 => if t < small
				   then raise Done
					   (copysign(if signx>zero then t 
								   else PI-t,
						     signy))
				   else (zero,zero,t)
   			(* t is in [7/16,11/16] *)
			    | 2 => (athfhi,athflo,((y+y)-x)/(x+x+y))
			(* t is in [11/16,19/16] *)
			    | 3 => (PIo4,zero,(y-x)/(x+y))
			    | 4 => (PIo4,zero,(y-x)/(x+y))
			(* t is in [19/16,39/16] *)
			    | _ => let val z = y-x
				       val y = y+y+y
				       val t = x+x
				   in (at1fhi,at1flo,( (z+z)-x ) / ( t + y ))
				   end
			(* end of if (t < 2.4375) *)
			else let val t = if t <= big
					 (* t is in [2.4375, big] *)
					 then ~x / y
					 (* t is in [big, INF] *)
					 else zero
			     in (PIo2,zero,t) end
		(* end of argument reduction *)
		(* compute atan(t) for t in [~.4375, .4375] *)
		val z = t*t
		val z = t*(z*(a1+z*(a2+z*(a3+z*(a4+z*(a5+z*(a6+z*(a7+z*(a8+
			z*(a9+z*(a10+z*a11)))))))))))
		val z = lo - z
		val z = z + t
		val z = z + hi
	    in  copysign(if signx>zero then z else PI-z,signy)
	    end
    in
    (* if x is 1.0, compute *)
	if feql(x,one) then findatan2(x,copysign(y,one),y) handle Done r => r
    (* when y = 0 *)
	else if feql(y,zero) then if feql(signx,one) then y else copysign(PI,signy)
    (* when x = 0 *)
	else if feql(x,zero) then copysign(PIo2,signy)
	else (* compute y/x *)
	    let val x = copysign(x,one)
		val y = copysign(y,one)
		val k =	logb(y)
		val m =	InLine.-(k,logb(x))
		val (t,y,x) = if InLine.>(m,60) then (big+big,y,x)
			      else if InLine.<(m,~80) then (y/x,y,x)
			      else (y/x,scalb(y,InLine.~ k),
					scalb(x,InLine.~ k))
	    in  findatan2(x,y,t) handle Done r => r end
    end
end

fun arctan(x:real) = atan2(x,one)

fun sqrt(x: real) =
      if x<=zero then if x<zero then raise Sqrt else x
      else 
        let val k = 6 (* log base 2 of the precision *)
	    val n = InLine.rshift(logb(x),1)
	    val x = scalb(x, InLine.~(InLine.lshift(n,1)))
	    fun iter(0,g) = g
              | iter(i,g) = iter(InLine.-(i,1), half * (g + x/g))
         in scalb(iter(k,one),n)
        end

end
