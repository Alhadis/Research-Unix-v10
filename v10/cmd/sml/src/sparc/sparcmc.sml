(* Copyright 1989 by AT&T Bell Laboratories *)
(* sparcmc.sml
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY 14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   11/20/89  created
 *
 *    The SPARC machine code emitter.
 *)

structure SparcMCode =
struct
    local open ByteArray in

    val code = ref (array(0, 0))

    fun getCodeString () = let
	  val s = extract (!code, 0, length (!code))
	  in
	    code := array(0, 0);
	    s
	  end

    end (* local *)
end (* SparcMCode *)

structure SparcMCEmit : EMITTER =
struct

(* +DEBUG *)
    fun diag (s : string) f x = (f x) handle e => (
	print "?exception "; print (System.exn_name e);
	print " in SparcCM."; print s; print "\n";
	raise e)
(* -DEBUG *)

    open SparcMCode SparcInstr

  (* Bit-wise operations *)
    val << = Bits.lshift
    val >> = Bits.rshift
    val ++ = Bits.orb
    val & = Bits.andb
    infix << >> ++ &

    val loc = ref 0     (* the location counter *)

    fun emitByte n = let
	  val i = !loc
	  in
	    loc := i+1;  ByteArray.update (!code, i, n)
	  end
(* +DEBUG *)
handle ByteArray.Subscript => (
  print "?exception Subscript in SparcMCEmit.emitByte. loc = ";
  print(!loc); print "\n";
  raise ByteArray.Subscript)
(* -DEBUG *)
(* DEBUG *) val emitByte = diag "emitByte" emitByte

    fun emitWord n = if (n < 0)
	  then emitWord (n + 65536)
	  else (emitByte (n >> 8); emitByte (n & 255))

    fun emitLong n = if (n < 0)
	  then let
	    val a = ~n
	    val b = (a >> 16)
	    val c = (a & 65535);
	    in
	      emitWord (~c + (if b = 0 then 0 else ~1)); emitWord (~b)
	    end
	  else (emitWord(n >> 16); emitWord (n & 65535))

    fun emitString s = let
	  fun copy i = (emitByte(ordof(s, i)); copy(i+1))
	  in
	    (copy 0) handle Ord => ()
	  end

    structure IEEERealConst = RealConst(IEEEReal(val emitWord = emitWord))

    val emitReal = IEEERealConst.realconst

    fun emitAddr (lab as Label{addr = ref a, ...}, k) = emitLong (k + a - !loc)

    fun define _ = ()

    local
      open System.Tags
    in
    fun mark () = emitLong (((!loc + 4) div 4) * power_tags + tag_backptr)
    end (* local *)

    local

      fun valOf (LABELexp{base, dst, offset}) = ((addrOf dst) + offset) - (addrOf base)

      fun immed13 (IMrand i) = (i & 8191)                     (* 13 bits *)
	| immed13 (LABrand labexp) = ((valOf labexp) & 8191)  (* 13 bits *)
	| immed13 (LOrand labexp) = ((valOf labexp) & 1023)   (* 10 bits *)
	| immed13 _ = ErrorMsg.impossible "[SparcMCEmit.immed13]"

    (* emit a 3 operand instruction with "11" in bits 31-30 *)
      fun emitOp11 opcode (REG a, REGrand(REG b), REG c) = (
	    emitWord(49152 ++ (c << 9) ++ (opcode << 3) ++ (a >> 2));
	    emitWord(((a & 3) << 14) ++ b))
	| emitOp11 opcode (REG a, b, REG c) = (
	    emitWord(49152 ++ (c << 9) ++ (opcode << 3) ++ (a >> 2));
	    emitWord(((a & 3) << 14) ++ 8192 ++ (immed13 b)))

      val emit_ld = emitOp11 0
      val emit_st = emitOp11 4
      val emit_ldb = emitOp11 1
      val emit_stb = emitOp11 5
      fun emit_ldf (r, ri, FREG fr) = emitOp11 32 (r, ri, REG fr)
      fun emit_stf (r, ri, FREG fr) = emitOp11 36 (r, ri, REG fr)

    (* emit a branch instruction *)
    fun emitBcc opcode lab = let
	  val disp = (((addrOf lab) - !loc) div 4)
	  val d = if disp < 0 then (disp + 4194304) else disp
	  in
	    if ((disp < ~2097152) orelse (2097152 <= disp))
	      then ErrorMsg.impossible "[SparcMCEmit.emitBcc]" else ();
	    emitLong ((opcode << 22) ++ d)
	  end

    val emit_ba = emitBcc 66        (* 1000010 *)
    val emit_be = emitBcc 10        (* 0001010 *)
    val emit_bne = emitBcc 74       (* 1001010 *)
    val emit_ble = emitBcc 18       (* 0010010 *)
    val emit_bge = emitBcc 90       (* 1011010 *)
    val emit_bl = emitBcc 26        (* 0011010 *)
    val emit_bg = emitBcc 82        (* 1010010 *)
    val emit_fbe = emitBcc 78       (* 1001110 *)
    val emit_fbne = emitBcc 14      (* 0001110 *)
    val emit_fble = emitBcc 110     (* 1101110 *)
    val emit_fbge = emitBcc 94      (* 1011110 *)
    val emit_fbl = emitBcc 38       (* 0100110 *)
    val emit_fbg = emitBcc 54       (* 0110110 *)

    (* emit a 3 operand instructions with "10" in bits 31-30. *)
      fun emitOp10 opcode (REG a, REGrand(REG b), REG c) = (
	    emitWord(32768 ++ (c << 9) ++ (opcode << 3) ++ (a >> 2));
	    emitWord(((a & 3) << 14) ++ b))
	| emitOp10 opcode (REG a, b, REG c) = (
	    emitWord(32768 ++ (c << 9) ++ (opcode << 3) ++ (a >> 2));
	    emitWord(((a & 3) << 14) ++ 8192 ++ (immed13 b)))

      val emit_jmpl = emitOp10 56       (* 111000 *)

    (* integer operations *)
      val emit_add = emitOp10 0         (* 000000 *)
      val emit_addcc = emitOp10 16      (* 010000 *)
      val emit_taddcctv = emitOp10 34   (* 100010 *)
      val emit_sub = emitOp10 4         (* 000100 *)
      val emit_subcc = emitOp10 20      (* 010100 *)
      val emit_sll = emitOp10 37        (* 100101 *)
      val emit_sra = emitOp10 39        (* 100111 *)
      val emit_and = emitOp10 1         (* 000001 *)
      val emit_andcc = emitOp10 17      (* 010001 *)
      val emit_or = emitOp10 2          (* 000010 *)
      val emit_xor = emitOp10 3         (* 000011 *)
      val emit_xnor = emitOp10 7        (* 000111 *)

      (* emit a floating-point instruction of three args; this has "10" in
       * bits 31-30 and "110100" in bits 24-19.
       *)
      fun emitFOp3 opcode (FREG a, FREG b, FREG c) = (
	    emitWord (33184 ++ (c << 9) ++ (a >> 2));
	    emitWord (((a & 3) << 14) ++ (opcode << 5) ++ b))
       (* emit a 2 operand floating-point instruction (same bits as above) *)
      fun emitFOp2 opcode (FREG a, FREG b) = (
	    emitWord (33184 ++ (b << 9));
	    emitWord ((opcode << 5) ++ a))
       (* emit a 2 operand floating-point instruction with "110101" in bits 24-19. *)
      fun emitFOp2' opcode (FREG a, FREG b) = (
	    emitWord (33192 ++ (a >> 2));
	    emitWord (((a & 3) << 14) ++ (opcode << 5) ++ b))
      val emit_fadd = emitFOp3 66       (* 001000010 *)
      val emit_fsub = emitFOp3 70       (* 001000110 *)
      val emit_fmul = emitFOp3 74       (* 001001010 *)
      val emit_fdiv = emitFOp3 78       (* 001001110 *)
      val emit_fneg = emitFOp2 5        (* 000000101 *)
      val emit_fcmp = emitFOp2' 82      (* 001010010 *)
    in

    fun emitInstr (I_nop) = emitLong 16777216 (* really "sethi 0,%g0" *)
      | emitInstr (I_ld args) = emit_ld args
      | emitInstr (I_ldb args) = emit_ldb args
      | emitInstr (I_ldf args) = emit_ldf args
      | emitInstr (I_st args) = emit_st args
      | emitInstr (I_stb args) = emit_stb args
      | emitInstr (I_stf args) = emit_stf args
      | emitInstr (I_sethi(arg, REG rd)) = let
	  val im = case arg
		 of (IMrand i) => i
		  | (HIrand labexp) => ((valOf labexp) >> 10)
		  | _ => ErrorMsg.impossible "[SparcMCEmit.emitInstr:sethi]"
	  in
	    emitWord(256 ++ (rd << 9) ++ ((im >> 16) & 63));
	    emitWord(im & 65535)
	  end
      | emitInstr (I_ba lab) = emit_ba lab
      | emitInstr (I_bcc(EQL, lab)) = emit_be lab
      | emitInstr (I_bcc(NEQ, lab)) = emit_bne lab
      | emitInstr (I_bcc(LSS, lab)) = emit_bl lab
      | emitInstr (I_bcc(LEQ, lab)) = emit_ble lab
      | emitInstr (I_bcc(GTR, lab)) = emit_bg lab
      | emitInstr (I_bcc(GEQ, lab)) = emit_bge lab
      | emitInstr (I_fbcc(EQL, lab)) = emit_fbe lab
      | emitInstr (I_fbcc(NEQ, lab)) = emit_fbne lab
      | emitInstr (I_fbcc(LSS, lab)) = emit_fbl lab
      | emitInstr (I_fbcc(LEQ, lab)) = emit_fble lab
      | emitInstr (I_fbcc(GTR, lab)) = emit_fbg lab
      | emitInstr (I_fbcc(GEQ, lab)) = emit_fbge lab
      | emitInstr (I_jmpl args) = emit_jmpl args
      | emitInstr (I_add args) = emit_add args
      | emitInstr (I_addcc args) = emit_addcc args
      | emitInstr (I_taddcctv args) = emit_taddcctv args
      | emitInstr (I_sub args) = emit_sub args
      | emitInstr (I_subcc args) = emit_subcc args
      | emitInstr (I_sll args) = emit_sll args
      | emitInstr (I_sra args) = emit_sra args
      | emitInstr (I_and args) = emit_and args
      | emitInstr (I_andcc args) = emit_andcc args
      | emitInstr (I_or args) = emit_or args
      | emitInstr (I_xor args) = emit_xor args
      | emitInstr (I_not(r1, rd)) = emit_xnor (r1, REGrand(REG 0), rd)
      | emitInstr (I_tvs) = (emitWord 36816; emitWord 8199)  (* "tvs 0x7" *)
      | emitInstr (I_fadd args) = emit_fadd args
      | emitInstr (I_fsub args) = emit_fsub args
      | emitInstr (I_fmul args) = emit_fmul args
      | emitInstr (I_fdiv args) = emit_fdiv args
      | emitInstr (I_fneg args) = emit_fneg args
      | emitInstr (I_fcmp args) = emit_fcmp args
(* DEBUG *) val emitInstr = diag "emitInstr" emitInstr

    end (* local *)

    fun comment _ = ()

    fun init n = (code := ByteArray.array(n, 0); loc := 0)

end (* structure SparcMCEmit *)
