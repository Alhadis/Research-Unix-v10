(* Copyright 1989 by AT&T Bell Laboratories *)
(* sparcinstr.sml
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY 14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   11/20/89  created
 *)

structure SparcInstr =
struct

    open BaseCoder

    datatype register = REG of int
    datatype fregister = FREG of int

    datatype labelexp
      = LABELexp of {           (* An offset relative to a label.  The value of a *)
	  base : label,         (* label expression is ((dst - base) + offset). *)
	  dst : label,
	  offset : int
	}

    datatype operand
      = REGrand of register     (* A register value *)
      | IMrand of int           (* A small integer constant (13 bits) *)
      | LABrand of labelexp     (* A small valued label expression (13 bits) *)
      | HIrand of labelexp      (* The high 22 bits of a label expression *)
      | LOrand of labelexp      (* The low 10 bits of a label expression *)

    datatype condition = EQL | NEQ | GTR | GEQ | LSS | LEQ

    datatype instruction
      = I_nop
      | I_ld of ops3                        (* ld: load word *)
      | I_ldb of ops3                       (* ldb: load byte (unsigned) *)
      | I_ldf of fmemops                    (* ldf: load floating-point register *)
      | I_st of ops3                        (* st: store word *)
      | I_stb of ops3                       (* stb: store byte *)
      | I_stf of fmemops                    (* stf: store floating-point register *)
      | I_sethi of (operand * register)     (* sethi *)
      | I_ba of label                       (* branch always *)
      | I_bcc of (condition * label)
      | I_fbcc of (condition * label)
      | I_jmpl of ops3                      (* jmpl: jump and link *)
      | I_add of ops3                       (* add *)
      | I_addcc of ops3                     (* add and set condition codes *)
      | I_taddcctv of ops3                  (* tagged add with overflow trap *)
      | I_sub of ops3
      | I_subcc of ops3                     (* subtract and set condition codes *)
      | I_sll of ops3
      | I_sra of ops3
      | I_and of ops3
      | I_andcc of ops3                     (* logical and and set condition codes *)
      | I_or of ops3
      | I_xor of ops3
      | I_not of (register * register)
      | I_tvs                               (* tvs: trap on integer overflow *)
      | I_fadd of fops3                     (* floating-point addition *)
      | I_fsub of fops3                     (* floating-point subtraction *)
      | I_fmul of fops3                     (* floating-point multiplication *)
      | I_fdiv of fops3                     (* floating-point division *)
      | I_fneg of fops2                     (* floating-point negation *)
      | I_fcmp of fops2                     (* floating-point comparison *)
      withtype ops3 = (register * operand * register)
      and fmemops = (register * operand * fregister)
      and fops3 = (fregister * fregister * fregister)
      and fops2 = (fregister * fregister)

    datatype ikind = IK_NOP | IK_JUMP | IK_INSTR

    fun instrKind (I_nop) = IK_NOP
      | instrKind (I_ba _) = IK_JUMP
      | instrKind (I_bcc _) = IK_JUMP
      | instrKind (I_fbcc _) = IK_JUMP
      | instrKind (I_jmpl _) = IK_JUMP
      | instrKind _ = IK_INSTR

    val nop = I_nop



  (** Span dependent instructions **
   * The implementation of these instructions depends on the value of the
   * label expressions.  The last register argument is a temporary register
   * to be used in address computations (if necessary).
   *)

    datatype sdi
      = SetBaseAddr of (bool ref * register * labelexp * register)
      | LoadAddr of (labelexp * register * register)
      | Load of (labelexp * register * register)
      | LoadF of (labelexp * fregister * register)

    fun minSize (SetBaseAddr _) = 0
      | minSize (LoadF _) = 8
      | minSize _ = 4

    local
    (* Sizes of SDIs *)
      val sz0max = (true, 0) and sz12max = (true, 12)
      val sz16max = (true, 16) and sz20max = (true, 20)
      val sz4 = (false, 4) and sz8 = (false, 8) and sz12 = (false, 12)
    in

  (* Return the size of the various span-dependent instructions.  This should
   * be consistent with expand in "sparccoder.sml" *)
    fun sizeOf I = let
	  fun span (LABELexp{base, dst, offset}) =
		((addrOf dst) + offset) - (addrOf base)
	  fun sizeOf' labexp = let
		val x = span labexp
		in
		  if (x < ~4096) orelse (4095 < x) then sz12max else sz4
		end
	  in
	    case I
	     of SetBaseAddr(ref false, _, _, _) => sz0max
	      | SetBaseAddr(ref true, _, labexp, _) => let
		  val x = span labexp
		  in if (4095 < x)
		    then if (8190 < x)
		      then sz16max
		      else sz12  (* use two subtract immediates *)
		    else if (x < ~4096)
		      then sz16max
		      else sz8
		  end
	      | LoadAddr(labexp, _, _) => sizeOf' labexp
	      | Load(labexp, _, _) => sizeOf' labexp
	      | LoadF(labexp, _, _) => let
		  val x = span labexp
		  in
		    if (x < ~4092) orelse (4092 <= x) then sz20max else sz8
		  end
	  end

    end (* local *)

    local
      val baseReg = REG 27            (* %i3 *)
      val zeroR = REG 0               (* %g0 *)
      val zeroRand = REGrand zeroR
    in

  (* expand SDIs into real instruction sequences. *)
    fun expand (SetBaseAddr(ref false, _, _, _), 0) = []
      | expand (SetBaseAddr(ref true, contReg, labexp, _), 8) = [
	  I_ld(contReg, IMrand 0, baseReg),
	  I_sub(baseReg, LABrand labexp, baseReg)]
      | expand (SetBaseAddr(ref true, contReg, LABELexp{base, dst, offset}, _), 12) = let
	  val n = if (((addrOf dst) - (addrOf base)) < 0) then ~4096 else 4095
	  val labexp = LABELexp{base=base, dst=dst, offset=offset - n}
	  in [
	    I_ld(contReg, zeroRand, baseReg),
	    I_sub(baseReg, IMrand n, baseReg),
	    I_sub(baseReg, LABrand labexp, baseReg)]
	  end
      | expand (SetBaseAddr(ref true, contReg, labexp, tmpR), 16) = [
	  I_ld(contReg, zeroRand, baseReg),
	  I_sethi(HIrand labexp, tmpR),
	  I_or(tmpR, LOrand labexp, tmpR),
	  I_sub(baseReg, REGrand tmpR, baseReg)]
      | expand (LoadAddr(labexp, dst, _), 4) = [I_add(baseReg, LABrand(labexp), dst)]
      | expand (LoadAddr(labexp, dst, tmpR), 12) = [
	  I_sethi(HIrand labexp, tmpR),
	  I_or(tmpR, LOrand labexp, tmpR),
	  I_add(baseReg, REGrand tmpR, dst)]
      | expand (Load(labexp, dst, _), 4) = [I_ld(baseReg, LABrand(labexp), dst)]
      | expand (Load(labexp, dst, tmpR), 12) = [
	  I_sethi(HIrand(labexp), tmpR),
	  I_or(tmpR, LOrand labexp, tmpR),
	  I_ld(baseReg, REGrand tmpR, dst)]
      | expand (LoadF(labexp as LABELexp{base, dst, offset}, FREG i, _), 8) = [
	  I_ldf(baseReg, LABrand(labexp), FREG i),
	  I_ldf(baseReg, LABrand(LABELexp{base=base, dst=dst, offset=offset+4}),
	    FREG(i+1))]
      | expand (LoadF(labexp, FREG i, tmpR), 20) = [
	  I_sethi(HIrand(labexp), tmpR),
	  I_or(tmpR, LOrand(labexp), tmpR),
	  I_add(baseReg, REGrand tmpR, tmpR),
	  I_ldf(tmpR, zeroRand, FREG i),
	  I_ldf(tmpR, IMrand 4, FREG(i+1)) ]

    end (* local *)


  (** Resource usage **
   *
   * The sparc resources are the condition codes, floating-point condition codes,
   * registers %r1-%r31, floating-point registers (viewed as a single resource)
   * and  memory.  We treat %g6 (the dataptr) specially.  We assume that %g6 is
   * only used in 'add', 'taddcctv', 'st', 'stf', and 'or' (move) instructions.
   * We use %g6 to denote the "allocation" resource.  This interferes in a
   * non-standard way with the memory resource and with itself.  Store instructions
   * using %g6 as a base register don't define the memory resource, but the
   * move of %g6 (using 'or') to another register is an implicit definition of
   * the memory resource (since it makes the allocation visible), the move also
   * defines the allocation resource, since it should only occur after the record
   * has been initialized.  There is an implicit use dependency between the
   * 'tvs' instruction and the exnptr register.
   *)

    val numRegs = 31        (* %r1-%r31 *)

    val numResources =      (* registers + fp regs + mem, cond codes, fp cond codes *)
	  (numRegs + 4)

    local
      val memRId = 0
      val memR = [memRId]
      fun addRegR (REG 0, lst) = lst
	| addRegR (REG i, lst) = (i :: lst)
      fun regR r = addRegR(r, nil)
      val allocRId = 6 and allocR = [6]
      val exnptrRId = 7
      val fregsRId = (numRegs + 1)
      val ccRId = (fregsRId + 1)
      val fccRId = (ccRId + 1)
      val fregsR = [fregsRId]
      val ccR = [ccRId] and fccR = [fccRId]
      fun rUseDef3 (a, REGrand b, c) = (addRegR(a, regR b), regR c)
	| rUseDef3 (a, _, c) = (regR a, regR c)
      fun rUseDef3cc (a, REGrand b, c) = (addRegR(a, regR b), ccRId :: (regR c))
	| rUseDef3cc (a, _, c) = (regR a, ccRId :: (regR c))
      val fregUseDef = (fregsR, fregsR)
      val allR = let
	    fun f (~1, l) = l | f (i, l) = f(i-1, i::l)
	    in
	      f (numResources-1, nil)
	    end
    in

    fun rUseDef (I_nop) = ([], [])
      | rUseDef (I_ld args) = let val (u, d) = rUseDef3 args in (memRId :: u, d) end
      | rUseDef (I_ldb args) = let val (u, d) = rUseDef3 args in (memRId :: u, d) end
      | rUseDef (I_ldf(a, REGrand b, c)) = (memRId :: addRegR(a, regR b), fregsR)
      | rUseDef (I_ldf(a, _, c)) = (memRId :: regR a, fregsR)
      | rUseDef (I_st(REG 6, REGrand b, c)) = (6 :: addRegR(b, regR c), [])
      | rUseDef (I_st(REG 6, _, c)) = (6 :: (regR c), [])
      | rUseDef (I_st(a, REGrand b, c)) = (addRegR(a, addRegR(b, regR c)), memR)
      | rUseDef (I_st(a, _, c)) = (addRegR(a, regR c), memR)
      | rUseDef (I_stb(a, REGrand b, c)) = (addRegR(a, addRegR(b, regR c)), memR)
      | rUseDef (I_stb(a, _, c)) = (addRegR(a, regR c), memR)
      | rUseDef (I_stf(REG 6, REGrand b, c)) = (6 :: addRegR(b, fregsR), [])
      | rUseDef (I_stf(REG 6, _, c)) = (6 :: fregsR, [])
      | rUseDef (I_stf(a, REGrand b, c)) = (addRegR(a, addRegR(b, fregsR)), memR)
      | rUseDef (I_stf(a, _, c)) = (addRegR(a, fregsR), memR)
      | rUseDef (I_sethi(_, r)) = ([], regR r)
      | rUseDef (I_ba _) = ([], [])
      | rUseDef (I_bcc _) = (ccR, [])
      | rUseDef (I_fbcc _) = (fccR, [])
      | rUseDef (I_jmpl args) = rUseDef3 args
      | rUseDef (I_add args) = rUseDef3 args
      | rUseDef (I_addcc args) = rUseDef3cc args
      | rUseDef (I_taddcctv _) = (allR, allR) (* GC limit check *)
      | rUseDef (I_sub args) = rUseDef3 args
      | rUseDef (I_subcc args) = rUseDef3cc args
      | rUseDef (I_sll args) = rUseDef3 args
      | rUseDef (I_sra args) = rUseDef3 args
      | rUseDef (I_and args) = rUseDef3 args
      | rUseDef (I_andcc args) = rUseDef3cc args
      | rUseDef (I_or(_, REGrand(REG 6), REG c)) = (* this completes the allocation *)
	  ([6], [6, c, memRId])
      | rUseDef (I_or args) = rUseDef3 args
      | rUseDef (I_xor args) = rUseDef3 args
      | rUseDef (I_not(a, b)) = (regR a, regR b)
      | rUseDef (I_tvs) = ([ccRId, exnptrRId], [])
      | rUseDef (I_fadd args) = fregUseDef
      | rUseDef (I_fsub args) = fregUseDef
      | rUseDef (I_fmul args) = fregUseDef
      | rUseDef (I_fdiv args) = fregUseDef
      | rUseDef (I_fneg(a, b)) = fregUseDef
      | rUseDef (I_fcmp(a, b)) = (fregsR, fccR)

    local
	fun uses (r, I) = let
	      fun uses3 (REG a, REGrand(REG b), _) = ((r = a) orelse (r = b))
		| uses3 (REG a, _, _) = (r = a)
	      fun st_uses (REG a, REGrand(REG b), REG c) =
		    ((r = a) orelse (r = b) orelse (r = c))
		| st_uses (REG a, _, REG c) = ((r = a) orelse (r = c))
	      in
		case I
		 of (I_ld args) => uses3 args
		  | (I_ldb args) => uses3 args
		  | (I_ldf (REG a, REGrand(REG b), _)) => ((r = a) orelse (r = b))
		  | (I_ldf (REG a, _, _)) => (r = a)
		  | (I_st args) => st_uses args
		  | (I_stb args) => st_uses args
		  | (I_stf (REG a, REGrand(REG b), _)) => ((r = a) orelse (r = b))
		  | (I_stf (REG a, _, _)) => (r = a)
		  | (I_jmpl args) => uses3 args
		  | (I_add args) => uses3 args
		  | (I_addcc args) => uses3 args
		  | (I_taddcctv _) => true  (* GC limit check *)
		  | (I_sub args) => uses3 args
		  | (I_subcc args) => uses3 args
		  | (I_sll args) => uses3 args
		  | (I_sra args) => uses3 args
		  | (I_and args) => uses3 args
		  | (I_andcc args) => uses3 args
		  | (I_or args) => uses3 args
		  | (I_xor args) => uses3 args
		  | (I_not(REG a, _)) => (r = a)
		  | (I_tvs) => (r = exnptrRId)
		  | _ => false
	      end
    in
    fun hazard (I_ld(_, _, REG dst), I) = uses (dst, I)
      | hazard (I_ldb(_, _, REG dst), I) = uses (dst, I)
      | hazard (I_ldf _, I) = (case I of (I_stf _) => true
	    | (I_fadd _) => true | (I_fsub _) => true | (I_fmul _) => true
	    | (I_fdiv _) => true | (I_fneg _) => true | (I_fcmp _) => true
	    | _ => false)
      | hazard (I, I_stf _) = (case I
	   of (I_fadd _) => true | (I_fsub _) => true | (I_fmul _) => true
	    | (I_fdiv _) => true | (I_fneg _) => true | (I_fcmp _) => true
	    | _ => false)
      | hazard _ = false
    end (* local *)
    end (* local *)

    fun needsNop (I_fcmp _, I_fbcc _) = true
      | needsNop _ = false

end (* SparcInstr *)
