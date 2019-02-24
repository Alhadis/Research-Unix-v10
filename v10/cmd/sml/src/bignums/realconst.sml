(* Copyright 1989 by AT&T Bell Laboratories *)
(*
RealConst: generate ML real constants.
RealConst uses long multiplication to find the correct bit pattern for
the real.  This method is slow, but accurate, and works to any precision,
which means that floats can be cross-compiled correctly.

The function emitreal should take (int * bool array * int) which represents
a real value as (sign * fraction * exponent).
The sign is 0 if the real is positive, 1 if negative.
The fraction is a boolean array representing the bits; note that the most
significant bit is in position 0.
The exponent is the binary exponent of the normalized fraction.
"Normalized" here means a number between 0 and 1.

The algorithm works inefficient on forms like 10000000.0; forms like 1E7 (with
no bogus zeros) are better.  Also inefficient on forms like 0E23 or 1E~212.
*)

signature PRIMREAL = sig
val significant : int
val outofrange : string -> unit
val emitreal : (int * bool array * int) -> unit
end

signature REALCONST = sig
val realconst : string -> unit
end

functor RealConst(P : PRIMREAL) : REALCONST =
struct

open P

(* Use more than the required precision, then round at the end.  Because
   bigints are signed, this is actually one bit less precision than you might
   think.  This criterion works well enough for the 53 bits required by
   Vax G format and IEEE double format, but has not been tested with other
   values of significant. *)
val precision = significant + 16 - ((significant + 8) mod 8)

(* A float is a WHOLE "fraction" and an exponent base TWO. *)
type float = {frac : Bigint.bigint, exp : int}

val bigint = Bigint.bigint
val plus = Bigint.+
val times = Bigint.*
infix plus times

(* Take a bigint and return a bool array of bits which will represent the
   fraction.  The high (1/2) bit is in array position 0.  Assumes that
   the bigint is positive.  This will work if the bigint is smaller than
   the array or vice versa;  however, the number will be truncated, not
   rounded. *)
fun makebits (f,n) =
    let val s = Bigint.size f
	val bits = array(n,false)
	fun onebit b = Bigint.getbit(f,s-1-b)
	fun copybit n = (update(bits,n,onebit n); copybit (n+1))
			handle Subscript => ()
    in  copybit 0;
	bits
    end

(* round a float to n significant digits *)
local val one = bigint 1 in
fun round (float as {frac=f,exp=e},n) =
    let val shift = Bigint.size f + 1 - n
    in
	if shift <= 0 then float
	else {frac = if Bigint.getbit(f,shift-1)
		     then Bigint.>>(f, shift) plus one
		     else Bigint.>>(f, shift),
	      exp = e + shift}
    end
end

(* maketenth:  create the float of one tenth, to any number of significant
   digits, with no rounding on the last digit. *)
local val zero = bigint 0 and one = bigint 1 and two = bigint 2 in
fun maketenth 1 = {frac=one,exp= ~4}
  | maketenth n =
    let val {frac,exp} = maketenth(n-1)
	val rec tenthbit = fn 0 => zero | 1 => one
			    | 2 => one | 3 => zero | n => tenthbit(n mod 4)
	val f = (frac times two) plus tenthbit n
	val e = exp - 1
    in
	{frac=f,exp=e}
    end
end

(* float values ten and one tenth, to the correct precision. *)
val ten = {frac=bigint 5, exp = 1}
val tenth = round(maketenth(precision+1),precision)

(* Multiplies two floats together to the correct precision *)
fun mult {frac=f1,exp=e1} {frac=f2,exp=e2} =
    let val f = f1 times f2
	val e : int = e1 + e2
	    (* shouldn't need the type constraint, our comp bug *)
    in
	round({frac=f,exp=e},precision)
    end

(* Create a dynamic array of powers of ten *)
structure DFA = Dynamic(struct open Array
			       type float = {frac : Bigint.bigint, exp : int}
			       type elem = unit->float
			       type array = elem array
			   end)
local open DFA
      exception Unknown
      fun makelem e = (fn () => e)
      val one = {frac=bigint 1,exp=0}
in
    val pos10 = array(fn () => raise Unknown)	(* 10^2^n *)
    val _ = update(pos10,0,makelem ten)
    val neg10 = array(fn () => raise Unknown)	(* 10^~2^n *)
    val _ = update(neg10,0,makelem tenth)
    fun access(arr,n) = (arr sub n) ()
			handle Unknown => let val last = access(arr,n-1)
					       val new = mult last last
					   in  update(arr,n,makelem new);
					       new
					   end

    fun pow10_2 0 = one
      | pow10_2 n = if n > 0 then access(pos10,n-1) else access(neg10,~n-1)
    fun raisepower(f,0) = f
      | raisepower(f,e) =
	    let val sign = if e<0 then ~1 else 1
		fun power(f,p) = mult f (pow10_2(sign*p))
		fun raisep(f,0,_) = f
		  | raisep(f,e,p) =
		    if Bits.andb(e,1) = 1 then raisep(power(f,p),Bits.rshift(e,1),p+1)
		    else raisep(f,Bits.rshift(e,1),p+1)
	    in  raisep(f,abs e,1)
	    end
end

(* Takes a string list of the form {digit*.[digit*]}, and returns a bigint and
   the exponent base 10.  Requires that the list contain a decimal point and
   no trailing zeros (useless zeros after the decimal point). *)
local val ten = bigint 10 and zero = bigint 0 in
fun reducefrac f =
    let fun getexp nil = 0
	  | getexp ("."::_) = 0
	  | getexp (_::tl) = getexp tl - 1
	fun getwhole nil = zero
	  | getwhole ("."::tl) = getwhole tl
	  | getwhole ("0"::tl) = ten times getwhole tl
	  | getwhole (n::tl) = bigint(ord n - ord "0") plus (ten times getwhole tl)
	val backwards = rev f
	val whole = getwhole backwards
	val exp = getexp backwards
    in
	(whole,exp)
    end
end

exception Toobig

(* Takes a legal ML float string and returns an (int * bigint * int)
   which is the sign, whole "fraction", and power of ten exponent *)
fun getparts s =
    let datatype trailing = SIGNIFICANT | TRAILING
	(* separate the fraction from the exponent, adding a decimal point if
	   there is none and eliminating trailing zeros *)
	fun separate (nil,s) = (nil,nil,s)
	  | separate ("E"::tl,SIGNIFICANT) = (["."],tl,SIGNIFICANT)
	  | separate ("E"::tl,TRAILING) = (nil,tl,TRAILING)
	  | separate ("0"::tl,s) =
		let val (r,e,s) = separate(tl,s)
		in  case s of TRAILING => (r,e,TRAILING)
			    | SIGNIFICANT => ("0"::r,e,SIGNIFICANT)
		end
	  | separate ("."::tl,_) =
		let val (r,e,_) = separate(tl,TRAILING)
		in  ("."::r,e,SIGNIFICANT)
		end
	  | separate (hd::tl,s) =
		let val (r,e,_) = separate(tl,s)
		in  (hd::r,e,SIGNIFICANT)
		end
	val (unsigned,sign) = case explode s of "~"::more => (more,1)
					      | other => (other,0)
	val (frac_s,exp_s,_) = separate(unsigned,SIGNIFICANT)
	fun atoi strlist =
	    let	val numlist = map (fn n => ord n - ord "0") strlist
	    in  List.revfold (fn (a:int,b) => b*10 + a) numlist 0
	    end
	val exp10 = (case exp_s of nil => 0
				 | "~"::more => ~(atoi more)
				 | other => atoi other)
		    handle Overflow => raise Toobig
	val (frac,exp) = reducefrac frac_s
    in 
	(sign,frac,exp10 + exp)
    end

(* Takes a legal ML float string and returns an (int * bool array * int)
   which is the sign, fraction (with the high(1/2) bit in array posn 0),
   and exponent.  This is the form expected by the functor parameter
   emitreal. *)
fun makereal f = 
    let val (sign,frac10,exp10) = getparts f
	val float = raisepower(round({frac=frac10,exp=0},precision),exp10)
	val (newf as {frac,exp}) = round(float,significant+1)
	val size = Bigint.size frac
	val bits = makebits(frac,significant)
    in
	case size of 0 => (0,bits,0)
		   | _ => (sign,bits,exp + size)
    end

fun realconst f = emitreal(makereal f)
		  handle Toobig => (outofrange f; emitreal(makereal "0.0"))

end (* functor RealConst *)
