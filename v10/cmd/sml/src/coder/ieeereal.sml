(* Copyright 1989 by AT&T Bell Laboratories *)
(* ieeereal.sml
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY  14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   03/15/89  created
 *   11/20/89  changed argument sig of IEEEReal
 *
 * Support for IEEE floating-point constants (for M68881 and SPARC FPU).
 *
 * Double precision format (for normalized numbers):
 *   Bias = 1023.
 *   Exponent = 11 bits.
 *   Range of exponent = [1..2046]
 *   Mantissa = 52 (+1) bits.
 *   Value = (-1)^s * 2^(e-1023) * 1.f
 *
 * Sub-normal numbers (biased exponent = 0)
 *   Bias = 1022
 *   Mantissa = 52 bits.
 *   Value = (-1)^s * 2^-1022 * 0.f
 *)

functor IEEEReal (val emitWord : int -> unit) : PRIMREAL =
struct

    val significant = 53 (* 52 + redundant 1 bit *)

    fun outofrange s = ErrorMsg.complain("Real constant "^s^" out of range")

  (* Convert a portion of a boolean array to the appropriate integer. *)
    exception Bits
    fun bits(a, start, width) = let
	  fun b true = 1
	    | b false = 0
	  fun f 0 = b (a sub start)
	    | f n = b (a sub (start+n)) + 2 * f(n-1)
	  in
	    if (Array.length a < start+width) orelse (start < 0) orelse (width < 0)
	      then raise Bits
	      else f (width-1)
	  end

  (* Emit a real constant with the given sign, the mantissa frac and with the
   * unbiased exponent exp.
   *)
    fun emitreal (sign, frac, exp) = let
	  val exponent = exp + 1022
	  fun emit () = let
		val word0 = case frac sub 0 (* zero? *)
		       of true => Bits.orb(Bits.lshift(sign,15),
			    Bits.orb(Bits.lshift(exponent,4), bits(frac,1,4)))
			| false => 0
		val word1 = bits(frac,5,16)
		val word2 = bits(frac,21,16)
		val word3 = bits(frac,37,16)
		in
		  emitWord word0;
		  emitWord word1;
		  emitWord word2;
		  emitWord word3
		end
	  in
	    if exponent < 1
	      then outofrange "" (** A sub-normal number **)
	      else if exponent > 2047
		then outofrange "" (* A hack *)
		else emit()
	  end

end (* functor IEEEReal *)
