(* Copyright 1989 by AT&T Bell Laboratories *)
(* sparcas.sml
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY 14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   11/20/89  created
 *
 *    The SPARC assembly code emitter.
 *)

structure SparcAsCode =
struct
    val outfile = ref std_out
end (* SparcAsCode *)


structure SparcAsEmit : EMITTER =
struct

    open SparcAsCode SparcInstr

  (** The location counter **)
    val loc = ref 0
    fun advance n = (loc := !loc + n)
    fun advance4 () = (loc := !loc + 4)

  (** utility routines **)
    local
      val hexDigits = "0123456789abcdef"
      fun f (0, l) = l
	| f (n, l) = (
	    f (Bits.rshift(n, 4), chr(ordof(hexDigits, Bits.andb(n, 15))) :: l))
      fun cvt 0 = ["0", "x", "0"]
	| cvt n = ("0" :: "x" :: f(n, nil))
    in
    fun atoi i = implode(if (i < 0) then "-" :: cvt(~i) else cvt i)
    end

    fun emit s = output (!outfile) s

    fun newLine () = emit "\n"

    val emitLabel = emit o nameOf

    fun emitOffset 0 = ()
      | emitOffset i = (if (i < 0)
	  then (emit "-"; emit(atoi(~i)))
	  else (emit "+"; emit(atoi i)))


    fun emitLong i = (emit "\t.long\t"; emit(atoi i); newLine(); advance4())

    local
      fun oct i = let
	    val m = Integer.makestring
	    in
	      m(i div 64)^m((i div 8)mod 8)^m(i mod 8)
	    end
      fun c_char "\n" = "\\n"
	| c_char "\t" = "\\t"
	| c_char "\\" = "\\\\"
	| c_char "\"" = "\\\""
	| c_char c = if ord c < 32 then "\\"^oct(ord c) else c
      fun a_str s = implode(map c_char (explode s))
    in
    fun emitString s = (
	  emit "\t.ascii \""; emit(a_str s); emit "\"\n"; emit "\t.align\t4\n";
	  advance(size s))
    end (* local *)

    fun emitReal r = (emit "\t.double\t"; emit r; newLine(); advance 8)

    fun emitAddr (lab as Label{addr = ref a, ...}, k) = (
	  emit "\t.long\t("; emitLabel lab; emit "-.)"; emitOffset k;
	  emit "\t| "; emit(atoi(k + a - !loc)); newLine();
	  advance4())

    fun define (lab as Label{addr=ref a, ...}) = (
(*****
	  emitLabel lab;  emit ":\t| "; emit(atoi a); newLine())
*****)
emitLabel lab;  emit ":\t| .="; emit(atoi(!loc)); emit ", "; emit(atoi a); newLine())

    local
      open System.Tags
    in
    fun mark () = (
	  emit "\t.long\t(((.-L0)/4+1)*power_tags)+tag_backptr\t| ";
	  emit (atoi (((!loc + 4) div 4) * power_tags + tag_backptr)); newLine();
	  advance4())
    end (* local *)

    local
      fun comma () = emit ","

      fun emitReg (REG i) = if (i < 16)
	    then if (i < 8)
	      then (emit "%g"; emit (makestring i))
	      else (emit "%o"; emit (makestring (i-8)))
	    else if (i < 24)
	      then (emit "%l"; emit (makestring (i-16)))
	      else (emit "%i"; emit (makestring (i-24)))

      fun emitFReg (FREG i) = (emit "%f"; emit (makestring i))

      fun emitCond EQL = emit "e" | emitCond NEQ = emit "ne"
	| emitCond LSS = emit "l" | emitCond LEQ = emit "le"
	| emitCond GTR = emit "g" | emitCond GEQ = emit "ge"

      fun emitLExp (LABELexp{base, dst, offset=0}) = (
	    emitLabel dst; emit "-"; emitLabel base)
	| emitLExp (LABELexp{base, dst, offset}) = (
	    emit "("; emitLabel dst; emitOffset offset; emit ")-"; emitLabel base)

      fun emitRand (REGrand r) = emitReg r
	| emitRand (IMrand i) = emit (atoi i)
	| emitRand (LABrand lexp) = emitLExp lexp
	| emitRand (LOrand lexp) = (emit "%lo("; emitLExp lexp; emit ")")
	| emitRand (HIrand lexp) = (emit "%hi("; emitLExp lexp; emit ")")

      fun emitArgs (r1, arg, rd) = (
	    emitReg r1; comma(); emitRand arg; comma(); emitReg rd)

      fun emitAddr (r1, b) = (emitReg r1;
	    case b
	     of REGrand(REG 0) => ()
	      | REGrand r2 => (emit "+"; emitReg r2)
	      | IMrand i => emitOffset i
	      | LABrand l => (emit "+"; emitLExp l)
	      | LOrand l => (emit "+%lo("; emitLExp l; emit ")")
	      | _ => (ErrorMsg.impossible "[emitAddr]"))

      fun emitMemAddr args = (emit "["; emitAddr args; emit "]")

      fun emitFArgs (f1, f2, fd) = (
	    emitFReg f1; comma(); emitFReg f2; comma(); emitFReg fd)

    in

    fun emitInstr I = (
	  emit "\t";
	  case I
	   of (I_nop) => emit "nop"
	    | (I_ld(a, b, c)) => (emit "ld "; emitMemAddr(a, b); comma(); emitReg c)
	    | (I_ldb(a, b, c)) => (emit "ldub "; emitMemAddr(a, b); comma(); emitReg c)
	    | (I_ldf(a, b, c)) => (emit "ldf "; emitMemAddr(a, b); comma(); emitFReg c)
	    | (I_st(a, b, c)) => (emit "st "; emitReg c; comma(); emitMemAddr(a, b))
	    | (I_stb(a, b, c)) => (emit "stb "; emitReg c; comma(); emitMemAddr(a, b))
	    | (I_stf(a, b, c)) => (emit "stf "; emitFReg c; comma(); emitMemAddr(a, b))
	    | (I_sethi(x, rd)) => (
		emit "sethi ";
		case x
		 of IMrand i => emit(atoi i)
		  | HIrand _ => emitRand x
		  | _ => ErrorMsg.impossible "[emitInstr.sethi]";
		comma(); emitReg rd)
	    | (I_ba l) => (emit "ba "; emitLabel l)
	    | (I_bcc(cc, l)) => (emit "b"; emitCond cc; emit " "; emitLabel l)
	    | (I_fbcc(cc, l)) => (emit "fb"; emitCond cc; emit " "; emitLabel l)
	    | (I_jmpl(a, b, REG 0)) => (emit "jmp "; emitAddr(a, b))
	    | (I_jmpl(a, b, rd)) => (emit "jmpl "; emitAddr(a, b); comma(); emitReg rd)
	    | (I_add args) => (emit "add "; emitArgs args)
	    | (I_addcc args) => (emit "addcc "; emitArgs args)
	    | (I_taddcctv args) => (emit "taddcctv "; emitArgs args)
	    | (I_sub args) => (emit "sub "; emitArgs args)
	    | (I_subcc(a, b, REG 0)) => (
		emit "cmp "; emitReg a; comma();
		case b
		 of REGrand r2 => emitReg r2
		  | IMrand i => emit(atoi i)
		  | _ => ErrorMsg.impossible "[emitInstr.addcc]")
	    | (I_subcc args) => (emit "subcc "; emitArgs args)
	    | (I_sll args) => (emit "sll "; emitArgs args)
	    | (I_sra args) => (emit "sra "; emitArgs args)
	    | (I_and args) => (emit "and "; emitArgs args)
	    | (I_andcc(a, b, REG 0)) => (
		emit "btst "; emitReg a; comma();
		case b
		 of REGrand r2 => emitReg r2
		  | IMrand i => emit(atoi i)
		  | _ => ErrorMsg.impossible "[emitInstr.andcc]")
	    | (I_andcc args) => (emit "andcc "; emitArgs args)
	    | (I_or(REG 0, arg, rd)) => (emit "mov "; emitRand arg; comma(); emitReg rd)
	    | (I_or args) => (emit "or "; emitArgs args)
	    | (I_xor args) => (emit "xor "; emitArgs args)
	    | (I_not(r1, rd)) => (emit "not "; emitReg r1; comma(); emitReg rd)
	    | (I_tvs) => emit "tvs ST_INT_OVERFLOW"
	    | (I_fadd args) => (emit "faddd "; emitFArgs args)
	    | (I_fsub args) => (emit "fsubd "; emitFArgs args)
	    | (I_fmul args) => (emit "fmuld "; emitFArgs args)
	    | (I_fdiv args) => (emit "fdivd "; emitFArgs args)
	    | (I_fneg(f1, f2)) => (emit "fneg "; emitFReg f1; comma(); emitFReg f2)
	    | (I_fcmp(f1, f2)) => (emit "fcmp "; emitFReg f1; comma(); emitFReg f2)
	  (* end of case *);
	  emit "\t| .="; emit(atoi(!loc));
	  newLine();
	  advance4())

    end (* local *)

    fun comment s = emit s

    fun init (n : int) = (
	  loc := 0;
	  emit "| code size = "; emit(makestring n); emit " bytes\n")

end (* structure SparcAsEmit *)
