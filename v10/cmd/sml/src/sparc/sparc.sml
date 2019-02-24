(* Copyright 1989 by AT&T Bell Laboratories *)
(* sparc.sml
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY 14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   11/20/89  created
 *)

functor SparcCM (
  structure C : CODER
  sharing type C.instruction = SparcInstr.instruction
      and type C.sdi = SparcInstr.sdi
      and type C.label = SparcInstr.label) : CMACHINE =
struct

    structure C' : sig
	eqtype label
	val newLabel : unit -> label
	val emitLong : int -> unit
	val emitString : string -> unit
	val emitReal : string -> unit
	val emitLabel : (label * int) -> unit
	val mark : unit -> unit
	val comment : string -> unit
    end = C
    open C'

    structure S' : sig
	eqtype label
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
      end = SparcInstr
    open S'

    val zeroR = REG 0                       (* %g0 *)
    val zeroRand = REGrand zeroR

    local

      fun emit_ld args = C.emit (SparcInstr.I_ld args)
      fun emit_ldb args = C.emit (SparcInstr.I_ldb args)
      fun emit_ldf args = C.emit (SparcInstr.I_ldf args)
      fun emit_st args = C.emit (SparcInstr.I_st args)
      fun emit_stb args = C.emit (SparcInstr.I_stb args)
      fun emit_stf args = C.emit (SparcInstr.I_stf args)
      fun emit_sethi args = C.emit (SparcInstr.I_sethi args)
      fun emit_ba args = C.delay (SparcInstr.I_ba args)
      fun emit_bcc args = C.delay (SparcInstr.I_bcc args)
      fun emit_fbcc args = C.delay (SparcInstr.I_fbcc args)
      fun emit_jmpl args = C.delay (SparcInstr.I_jmpl args)
      fun emit_jmp (r, offset) = C.delay (SparcInstr.I_jmpl(r, offset, zeroR))
      fun emit_add args = C.emit (SparcInstr.I_add args)
      fun emit_addcc args = C.emit (SparcInstr.I_addcc args)
      fun emit_taddcctv args = C.emit (SparcInstr.I_taddcctv args)
      fun emit_sub args = C.emit (SparcInstr.I_sub args)
      fun emit_subcc args = C.emit (SparcInstr.I_subcc args)
      fun emit_sra args = C.emit (SparcInstr.I_sra args)
      fun emit_sll args = C.emit (SparcInstr.I_sll args)
      fun emit_and args = C.emit (SparcInstr.I_and args)
      fun emit_andcc args = C.emit (SparcInstr.I_andcc args)
      fun emit_or args = C.emit (SparcInstr.I_or args)
      fun emit_xor args = C.emit (SparcInstr.I_xor args)
      fun emit_not args = C.emit (SparcInstr.I_not args)
      fun emit_tvs () = C.emit SparcInstr.I_tvs
      fun emit_fadd args = C.emit (SparcInstr.I_fadd args)
      fun emit_fsub args = C.emit (SparcInstr.I_fsub args)
      fun emit_fmul args = C.emit (SparcInstr.I_fmul args)
      fun emit_fdiv args = C.emit (SparcInstr.I_fdiv args)
      fun emit_fneg args = C.emit (SparcInstr.I_fneg args)
      fun emit_fcmp args = C.delay (SparcInstr.I_fcmp args)

      local
	fun mkLabExp (lab, n) = LABELexp{base= C.baseLab, dst= lab, offset= (n-4096)}
      in

      fun setBaseAddr (contReg, lab, tmpR) = C.emitSDI (
	    SparcInstr.SetBaseAddr(ref true, contReg, mkLabExp(lab, 0), tmpR))

      fun loadAddr (lab, n, dst, tmpR) = C.emitSDI (
	    SparcInstr.LoadAddr(mkLabExp(lab, n), dst, tmpR))

      fun load (lab, n, dst, tmpR) = C.emitSDI (
	    SparcInstr.Load(mkLabExp(lab, n), dst, tmpR))

      fun loadF (lab, n, dst, tmpR) = C.emitSDI (
	    SparcInstr.LoadF(mkLabExp(lab, n), dst, tmpR))

      end (* local *)

    in

    datatype EA
      = Immed of int
      | ImmedLab of label
      | Direct of register

    val immed = Immed
    fun isimmed (Immed n) = SOME n | isimmed _ = NONE
    fun isreg (Direct(REG i)) = SOME i | isreg _ = NONE
    fun eqreg (a : EA) b = (a = b)

  (** Dedicated registers **)
    val exnptr = Direct(REG 7)              (* %g7 *)
    val storeptr = Direct(REG 5)            (* %g5 *)
    val arithtemp = Direct(REG 8)           (* %o0 *)
    val arithtemp2 = Direct(REG 9)          (* %o1 *)
    val standardclosure = Direct(REG 26)    (* %i2 *)
    val standardarg = Direct(REG 24)        (* %i0 *)
    val standardcont = Direct(REG 25)       (* %i1 *)
    val miscregs = map (Direct o REG) [     (* %g1-%g3, %l0-%l7, %i4-%i5 *)
	  1, 2, 3, 16, 17, 18, 19, 20, 21, 22, 23, 28, 29
	  ]

    val dataptrR = REG 6                    (* %g6 *)
    val limitptrR = REG 4                   (* %g4 *)
    val limitptrRand = REGrand limitptrR
    val arithtempRand = REGrand(REG 8)
    val spR = REG 14                        (* %sp (%o6) *)
    val linkR = REG 15                      (* %o7, link register *)

  (** Temporary registers **
   * We use registers %o2-%o5 as temporaries.  They are used in a round-robin
   * order to facilitate instruction scheduling.
   *)
    local
      val rear = ref 0 and queue = ref 0
      fun ins i = let
	    val r = !rear
	    in
	      queue := Bits.orb(Bits.lshift(i, r), !queue);
	      rear := r + 5
	    end
      fun remove () = let
	    val q = !queue
	    val x = Bits.andb (q, 31)
	    in
	      queue := Bits.rshift (q, 5);
	      rear := !rear - 5;
	      x
	    end
      val _ = app ins [10, 11, 12, 13]      (* %o2-%o5 *)
    in

  (* Registers %o2, %o3 & %o4 are also used to call ml_mul and ml_div. *)
    val arg1EA = Direct(REG 10) and arg2EA = Direct(REG 11)
    val opAddrR = REG 12

  (* Get a temporary register. *)
    fun getTmpReg () = REG(remove())

   (* If r is a temporary register, then free it. *)
    fun freeReg (REG r) = if ((9 < r) andalso (r < 14)) then (ins r) else ()

  (* Free a temporary register. *)
    fun freeTmpReg (REG r) = ins r

    end (* local *)


  (* align is a nop, since strings are automatically padded. *)
    fun align () = ()

    val mark = mark

    val emitlong = emitLong
    val realconst = emitReal
    val emitstring = emitString

    fun emitlab (n, ImmedLab lab) = emitLabel (lab, n)
      | emitlab _ = ErrorMsg.impossible "[SparcCM.emitlab]"

    val newlabel = ImmedLab o newLabel
    fun define (ImmedLab lab) = C.define lab
      | define _ = ErrorMsg.impossible "[SparcCM.define]"

    datatype immed_size = Immed13 | Immed32

    fun sizeImmed n = if (~4096 <= n) andalso (n < 4096) then Immed13 else Immed32


  (** Utility operations **)

    fun emitMove (src, dst) = emit_or (zeroR, REGrand src, dst)

    fun loadImmed32 (n, r) = let
	  val lo10 = Bits.andb(n, 1023)
	  in
	    emit_sethi (IMrand(Bits.rshift(n, 10)), r);
	    if (lo10 <> 0) then emit_or(r, IMrand lo10, r) else ()
	  end

    fun loadImmed (n, r) = (
	  case (sizeImmed n)
	   of Immed13 => emit_or(zeroR, IMrand n, r)
	    | Immed32 => loadImmed32 (n, r))

    fun op32 f (r1, n, r2) = let val tmpR = getTmpReg()
	  in
	    loadImmed32 (n, tmpR);
	    f (r1, REGrand tmpR, r2);
	    freeTmpReg tmpR
	  end

    fun loadReg(r, offset, dst) = (
	  case (sizeImmed offset)
	   of Immed13 => emit_ld (r, IMrand offset, dst)
	    | Immed32 => (op32 emit_ld) (r, offset, dst))

    fun store (src, r, offset) = (
	  case (sizeImmed offset)
	   of Immed13 => emit_st (r, IMrand offset, src)
	    | Immed32 => (op32 emit_st) (r, offset, src))

    fun addImmed (r, n, dst) = (
	  case (sizeImmed n)
	   of Immed13 => emit_add (r, IMrand n, dst)
	    | Immed32 => (op32 emit_add) (r, n, dst))

    fun compareImmed (r, n) = (
	  case (sizeImmed n)
	   of Immed13 => emit_subcc (r, IMrand n, zeroR)
	    | Immed32 => (op32 emit_subcc) (r, n, zeroR))


  (** CMachine instructions **)

  (* move (src, dst) *)
    fun move (Immed n, Direct r) = loadImmed (n, r)
      | move (ImmedLab lab, Direct r) = let val tmpR = getTmpReg()
	  in
	    loadAddr (lab, 0, r, tmpR);
	    freeTmpReg tmpR
	  end
      | move (Direct r1, Direct r2) = emitMove (r1, r2)
      | move _ = ErrorMsg.impossible "[SparcCM.move]"

  (* checkLimit (n):
   * Generate code to check the heap limit to see if there is enough free space
   * to allocate n bytes.
   * NOTE: The handler in "runtime/SPARC.dep.c" is sensitive to the code
   * sequences generated here.
   *)
    fun checkLimit maxAllocSize = (
	  if (maxAllocSize <= 4096)
	    then emit_taddcctv (dataptrR, limitptrRand, zeroR)
	    else let
	      val n = maxAllocSize - 4096
	      val tmpR = getTmpReg()
	      in
		if (n < 2048)
		  then (
		    emit_add (limitptrR, IMrand n, tmpR);
		    emit_taddcctv (dataptrR, REGrand tmpR, zeroR))
		  else (
		    emit_sethi (IMrand(Bits.rshift(n, 10)), tmpR);
		    emit_or (tmpR, IMrand(Bits.andb(n, 1023)), tmpR);
		    emit_add (limitptrR, REGrand tmpR, tmpR);
		    emit_taddcctv (dataptrR, REGrand tmpR, zeroR));
		freeTmpReg tmpR
	      end)

  (* beginStdFn (cl, lab):
   * Note the beginning of a standard function with entry label lab, and
   * register cl containing its closure.  This requires generating code to
   * load the base code block address into baseCodePtr.
   *)
    fun beginStdFn (Direct closureReg, ImmedLab lab) = let val tmpR = getTmpReg()
	  in
	    setBaseAddr (closureReg, lab, tmpR);
	    freeTmpReg tmpR
	  end
      | beginStdFn _ = ErrorMsg.impossible "[SparcCM.beginStdFn]"

  (* jmp (dst):
   * Unconditional jump to destination.
   *)
    fun jmp (ImmedLab lab) = emit_ba lab
      | jmp (Direct r) = emit_jmp (r, zeroRand)
      | jmp _ = ErrorMsg.impossible "[SparcCM.jmp]"

  (* record (vl, dst):
   * makes a new record, puts address of it into the destination specified
   * by the second arg. The contents are numbered from ~1 and up.
   *)
    fun record (vl : (EA * CPS.accesspath) list, Direct dst) = let
	  val len = length vl
	  val minBlockSize = 6
	(* generate code to move one or more adjacent fields from one record into
	 * adjacent fields in the new record.  If the block is big enough, then
	 * use a block copy loop.
	 *)
	  fun blockMove (srcR, startindx, path, offset) = let
	      (* check a CPS path to see how large the block is *)
		fun chkpath (cnt, i,
		    path as (Direct r, CPS.SELp(j, CPS.OFFp 0)) :: rest) =
		      if (r = srcR) andalso (i+offset = j)
			then chkpath (cnt+1, i-1, rest)
			else (cnt, path)
		  | chkpath (cnt, _, rest) = (cnt, rest)
	      (* generate code to move fields individually *)
		fun moveFields (0, _) = ()
		  | moveFields (n, indx) = let val tmpR = getTmpReg()
		      in
			loadReg(srcR, (indx+offset)*4, tmpR);
			store (tmpR, dataptrR, indx*4);
			freeTmpReg tmpR;
			moveFields(n-1, indx-1)
		      end
		val (blksz, rest) = chkpath(1, startindx-1, path)
		in
		  if (blksz < minBlockSize)
		    then moveFields(blksz, startindx)
		    else if (offset = 0)
		      then let
			val lab = newLabel()
			val indxR = getTmpReg() and tmpR = getTmpReg()
			in
			  loadImmed (startindx*4, indxR);
			  C.define lab;
			  emit_ld (srcR, REGrand indxR, tmpR);
			  compareImmed (indxR, (startindx-blksz)*4);
			  emit_st (dataptrR, REGrand indxR, tmpR);
			  emit_sub (indxR, IMrand 4, indxR);
			  emit_bcc (GTR, lab);
			  freeTmpReg indxR; freeTmpReg tmpR
			end
		      else let
			val lab = newLabel()
			val indxR1 = getTmpReg() and indxR2 = getTmpReg()
			val tmpR = getTmpReg()
			in
			  loadImmed ((startindx+offset)*4, indxR1);
			  loadImmed (startindx*4, indxR2);
			  C.define lab;
			  emit_ld (srcR, REGrand indxR1, tmpR);
			  emit_sub (indxR1, IMrand 4, indxR1);
			  emit_st (dataptrR, REGrand indxR2, tmpR);
			  emit_sub (indxR2, IMrand 4, indxR2);
			  compareImmed (indxR1, (startindx+offset-blksz)*4);
			  emit_bcc (GTR, lab);
			  freeTmpReg indxR1; freeTmpReg indxR2; freeTmpReg tmpR
			end;
		  freeReg srcR;
		  (startindx-blksz, rest)
		end (* blockMove *)
      (* For each field in the record generate the necessary moves to initialize
       * it in the new record.
       *)
	  fun fields (_, nil) = ()
	    | fields (i, (Direct r, CPS.SELp(j, CPS.OFFp 0)) :: rest) =
		fields (blockMove (r, i, rest, j-i))
	    | fields (i, (Direct r, CPS.SELp(j, p)) :: rest) = let
		val tmpR = getTmpReg()
		in
		  loadReg(r, j*4, tmpR);
		  freeReg r;
		  fields (i, (Direct tmpR, p) :: rest)
		end
	    | fields (i, (Direct r, CPS.OFFp 0) :: rest) = (
		store (r, dataptrR, i*4);
		freeReg r;
		fields (i-1, rest))
	    | fields (i, (Direct r, CPS.OFFp j) :: rest) = let
		val tmpR = getTmpReg()
		val offset = j*4
		in
		  case sizeImmed offset
		   of Immed13 => emit_add (r, IMrand offset, tmpR)
		    | Immed32 => (
			loadImmed32 (offset, tmpR);
			emit_add (r, REGrand tmpR, tmpR))
		  (* end case *);
		  store (tmpR, dataptrR, i*4);
		  freeTmpReg tmpR; freeReg r;
		  fields (i-1, rest)
		end
	    | fields (i, (x, p) :: rest) =  let
		val tmpR = getTmpReg()
		in
		  move (x, Direct tmpR);
		  fields (i, (Direct tmpR, p) :: rest)
		end
	  in
	    fields (len-2, rev vl);
	    emitMove (dataptrR, dst);
	    addImmed (dataptrR, len*4, dataptrR)
	end
      | record _ = ErrorMsg.impossible "[SparcCM.record]"

  (* select (i, x, y):  y <- mem[x + 4*i] *)
    fun select (i, Direct r, Direct dst) = loadReg(r, i*4, dst)
      | select (i, ImmedLab lab, Direct dst) = let val tmpR = getTmpReg()
	  in
	    load (lab, i*4, dst, tmpR);
	    freeTmpReg tmpR
	  end
      | select _ = ErrorMsg.impossible "[SparcCM.select]"

  (* offset (i, x, y):  y <- (x + 4*i) *)
    fun offset (i, Direct r, Direct dst) = addImmed (r, 4*i, dst)
      | offset (i, ImmedLab lab, Direct dst) = let val tmpR = getTmpReg()
	  in
	    loadAddr (lab, i, dst, tmpR);
	    freeTmpReg tmpR
	  end
      | offset _ = ErrorMsg.impossible "[SparcCM.offset]"

  (* fetchindexb (x, y) fetches an unsigned byte:  y <- mem[x+arithtemp] *)
    fun fetchindexb (Direct r, Direct dst) = emit_ldb (r, arithtempRand, dst)
      | fetchindexb _ = ErrorMsg.impossible "[SparcCM.fetchindexb]"

  (* storeindexb (x, y) stores a byte:  mem[y+arithtemp] <- x *)
    fun storeindexb (Direct src, Direct r) = emit_stb (r, arithtempRand, src)
      | storeindexb _ = ErrorMsg.impossible "[SparcCM.storeindexb]"

  (* jmpindexb (x):  pc <- (x+arithtemp) *)
    fun jmpindexb (ImmedLab lab) = let
	  val tmpR1 = getTmpReg() and tmpR2 = getTmpReg()
	  in
	    loadAddr (lab, 0, tmpR1, tmpR2);
	    emit_jmp (tmpR1, arithtempRand);
	    freeTmpReg tmpR1; freeTmpReg tmpR2
	  end
      | jmpindexb _ = ErrorMsg.impossible "[SparcCM.jmpindexb]"

  (* fetchindexl (x, y, z) fetches a word:  y <- mem[x+2*(z-1)] *)
    fun fetchindexl (Direct r1, Direct dst, Direct r2) = let
	  val tmpR = getTmpReg()
	  in
	    emit_sub (r2, IMrand 1, tmpR);
	    emit_add (tmpR, REGrand tmpR, tmpR);
	    emit_ld (r1, REGrand tmpR, dst);
	    freeTmpReg tmpR
	  end
      | fetchindexl (Direct r1, Direct dst, Immed i) = loadReg(r1, 2*(i-1), dst)
      | fetchindexl (ImmedLab lab, Direct dst, Direct r) =  let
	  val tmpR1 = getTmpReg() and tmpR2 = getTmpReg()
	  in
	    loadAddr (lab, ~2, tmpR1, tmpR2);
	    emit_add (r, REGrand tmpR1, tmpR1);
	    emit_ld (r, REGrand tmpR1, dst);
	    freeTmpReg tmpR1; freeTmpReg tmpR2
	  end
      | fetchindexl _ = ErrorMsg.impossible "[SparcCM.fetchindexl]"

  (*storeindexl (x, y, z) stores a word:  mem[y+2*(z-1)] <- x *)
    fun storeindexl (Direct src, Direct r1, Direct r2) = let val tmpR = getTmpReg()
	  in
	    emit_sub (r2, IMrand 1, tmpR);
	    emit_add (tmpR, REGrand tmpR, tmpR);
	    emit_st (r1, REGrand tmpR, src);
	    freeTmpReg tmpR
	  end
      | storeindexl (Direct src, Direct r, Immed i) = store (src, r, 2*(i-1))
      | storeindexl (Immed n, x, y) = let val tmpR = getTmpReg()
	  in
	    loadImmed (n, tmpR);
	    storeindexl (Direct tmpR, x, y);
	    freeTmpReg tmpR
	  end
      | storeindexl (ImmedLab lab, x, y) = let
	  val tmpR1 = getTmpReg() and tmpR2 = getTmpReg()
	  in
	    loadAddr (lab, 0, tmpR1, tmpR2);
	    storeindexl (Direct tmpR1, x, y);
	    freeTmpReg tmpR1; freeTmpReg tmpR2
	  end
(** NOTE: in a sane world the following case would be unecessary, but they
 ** are used in an ugly profiling hack.
 **)
      | storeindexl (Direct src, ImmedLab lab, Immed i) = let
	  val tmpR1 = getTmpReg() and tmpR2 = getTmpReg()
	  in
	    loadAddr (lab, 2*(i-1), tmpR1, tmpR2);
	    emit_st (tmpR1, zeroRand, src);
	    freeTmpReg tmpR1; freeTmpReg tmpR2
	  end
      | storeindexl _ = ErrorMsg.impossible "[SparcCM.storeindexl]"

  (* ashl (n, x, y) shift left: y <- (x << n), with  n >= 0 *)
    fun ashl (Direct cntR, Direct src, Direct dst) =
	  emit_sll(src, REGrand cntR, dst)
      | ashl (Immed cnt, Direct src, Direct dst) =
	  emit_sll (src, IMrand(Bits.andb(cnt, 31)), dst)
      | ashl (Direct cntR, Immed src, Direct dst) = let val tmpR = getTmpReg()
	  in
	    loadImmed (src, tmpR);
	    emit_sll (tmpR, REGrand cntR, dst);
	    freeTmpReg tmpR
	  end
      | ashl _ = ErrorMsg.impossible "[SparcCM.ashl]"

  (* ashr (n, x, y) shift right: y <- (x >> n), with  n >= 0 *)
    fun ashr (Direct cntR, Direct src, Direct dst) =
	  emit_sra (src, REGrand cntR, dst)
      | ashr (Immed cnt, Direct src, Direct dst) =
	  emit_sra (src, IMrand(Bits.andb(cnt, 31)), dst)
      | ashr (Direct cntR, Immed src, Direct dst) = let val tmpR = getTmpReg()
	  in
	    loadImmed (src, tmpR);
	    emit_sra (tmpR, REGrand cntR, dst);
	    freeTmpReg tmpR
	  end
      | ashr (Immed cnt, Immed src, Direct dst) = (
print "[missed constant fold ("; print src; print " >> "; print cnt; print ")]\n";
	  loadImmed (Bits.rshift(src, cnt), dst))
      | ashr _ = ErrorMsg.impossible "[SparcCM.ashr]"

    local
	fun adjArgs f (a as Immed _, b, c) = f (b, a, c)
	  | adjArgs f args = f args
	fun adjSubArgs f (a, Immed b, c) = let val tmpR = getTmpReg()
	      in
		loadImmed (b, tmpR);
		f (Direct tmpR, a, c);
		freeTmpReg tmpR
	      end
	  | adjSubArgs f (a, b, c) = f (b, a, c)
	fun arithOp f (Direct r1, Direct r2, Direct dst) = f (r1, REGrand r2, dst)
	  | arithOp f (Direct r, Immed n, Direct dst) = (
	      case (sizeImmed n)
	       of Immed13 => f (r, IMrand n, dst)
		| Immed32 => let val tmpR = getTmpReg()
		    in
		      loadImmed32 (n, tmpR);
		      f (r, REGrand tmpR, dst);
		      freeTmpReg tmpR
		    end)
	  | arithOp _ _ = ErrorMsg.impossible "[SparcCM.arithOp]"
	val addt = adjArgs (arithOp (fn args => (emit_addcc args; emit_tvs())))
    in

    val orb = adjArgs (arithOp emit_or)
    val andb = adjArgs (arithOp emit_and)
    val xorb = adjArgs (arithOp emit_xor)
    fun notb (Direct src, Direct dst) = emit_not (src, dst)
      | notb _ = ErrorMsg.impossible "[SparcCM.notb]"

    val addl3 = adjArgs (arithOp emit_add)
    fun addl3t (Immed a, b as Immed _, dst) = let val tmpR = getTmpReg ()
	(* This should only occur when we need to build a constant larger than
	 * 2^29.  Note, we assume that "b" is tagged (see "cps/generic.sml").
	 *)
	  in
	    loadImmed (a, tmpR);
	    addt (Direct tmpR, b, dst);
	    freeTmpReg tmpR
	  end
      | addl3t args = addt args

    val subl3 = adjSubArgs (arithOp emit_sub)
    val subl3t = adjSubArgs (arithOp (fn args => (emit_subcc args; emit_tvs())))

    end (* local *)

  (* mull2t/divl2t:
   * mull2t (a, b):  b <- (a * b) (with overflow checking done by ml_mul)
   * divl2t (a, b):  b <- (b div a)
   *)
    local
	fun intOp opAddrOffset (a, b as Direct _) = (
	      emit_ld (spR, opAddrOffset, opAddrR);
	      move (a, arg2EA);
	      move (b, arg1EA);
	      emit_jmpl (opAddrR, zeroRand, linkR);
	      move (arg1EA, b))
	  | intOp _ _ = ErrorMsg.impossible "[SparcCM.intOp]"
	val mulAddrOffset = IMrand 72
	val divAddrOffset = IMrand 76
    in
    val mull2t = intOp mulAddrOffset
    val divl2 = intOp divAddrOffset
    end (* local *)

  (* bbs (i, dst, lab): test the i'th bit of dst and jump to lab if it is zero *)
    fun bbs (Immed i, Direct r, ImmedLab lab) = (
	  emit_andcc (r, IMrand(Bits.lshift(1, i)), zeroR);
	  emit_bcc (NEQ, lab))
      | bbs _ = ErrorMsg.impossible "[SparcCM.bbs]"

    local
    (* reverse a condition (eg., (a <= b) <==> (b >= a) *)
      fun revCond NEQ = NEQ | revCond EQL = EQL
	| revCond LEQ = GEQ | revCond GEQ = LEQ
	| revCond LSS = GTR | revCond GTR = LSS
    in

  (* ibranch (cond, a, b, lab): if (a <cond> b) then pc <- lab *)
    fun ibranch (cond, a as Immed _, b as Direct _, l) = ibranch (revCond cond, b, a, l)
      | ibranch (cond, Direct r1, b, ImmedLab lab) = (
	  case b
	   of Direct r2 => emit_subcc (r1, REGrand r2, zeroR)
	    | Immed n => (
		case (sizeImmed n)
		 of Immed13 => emit_subcc (r1, IMrand n, zeroR)
		  | Immed32 => let val tmpR = getTmpReg()
		      in
			loadImmed32 (n, tmpR);
			emit_subcc (r1, REGrand tmpR, zeroR);
			freeTmpReg tmpR
		      end)
	    | _ => ErrorMsg.impossible "[SparcCM.ibranch.case]"
	  (* end case *);
	  emit_bcc (cond, lab))
      | ibranch _ = ErrorMsg.impossible "[SparcCM.ibranch]"

    end (* local *)


  (** Floating point instructions **
   * These instructions take ML real values as arguments (ie., addresses of
   * heap objects) and store their results in the heap.
   *)

    local
      val f0 = FREG 0
      val f1 = FREG 1
      val f2 = FREG 2
    (* the tag code for real values. *)
      val realTag = (8*System.Tags.power_tags + System.Tags.tag_string)
    (* finishReal(r): emit code to store the result of a real operation
     * (in %f0, %f1) in the heap, and put the result address in r.
     *)
      fun finishReal r = let val tmpR = getTmpReg()
	    in
	      loadImmed (realTag, tmpR);
	      emit_st (dataptrR, IMrand(~4), tmpR);
	      emit_stf (dataptrR, zeroRand, f0);
	      emit_stf (dataptrR, IMrand 4, f1);
	      emitMove (dataptrR, r);
	      emit_add (dataptrR, IMrand 12, dataptrR);
	      freeTmpReg tmpR
	    end
    (* Fetch a ML real value into a floating-point register pair *)
      fun fetchReal (Direct r, FREG i) = (
	    emit_ldf (r, zeroRand, FREG i);
	    emit_ldf (r, IMrand 4, FREG(i+1)))
	| fetchReal (ImmedLab lab, dst) = let val tmpR = getTmpReg()
	    in
	      loadF (lab, 0, dst, tmpR);
	      freeTmpReg tmpR
	    end
	| fetchReal _ = ErrorMsg.impossible "[SparcCM.fetchReal]"

      fun floatOp fOp (a, b, Direct dst) = (
	    fetchReal (a, f0);
	    fetchReal (b, f2);
	    fOp (f0, f2, f0);
	    finishReal dst)
	| floatOp _ _ = ErrorMsg.impossible "[SparcCM.floatOp]"
    in

  (* Negate the first arg and return a pointer to the result in the second *)
    fun mnegg (src, Direct dst) = (
	  fetchReal (src, f0);
	  emit_fneg (f0, f0);
	  finishReal dst)
      | mnegg _ = ErrorMsg.impossible "[SparcCM.mnegg]"

  (* Add the first two arguments and store the result in the third *)
    val addg3 = floatOp emit_fadd

  (* Subtract the second argument from the first and store the result in the third *)
    val subg3 = floatOp emit_fsub

  (* Multiply the first two arguments and store the result in the third *)
    val mulg3 = floatOp emit_fmul

  (* Divide the first argument by the second and store the result in the third *)
    val divg3 = floatOp emit_fdiv

  (* Conditionally branch on the float values of two arguments. *)
    fun gbranch (cond, a, b, ImmedLab lab) = (
	  fetchReal (a, f0);
	  fetchReal (b, f2);
	  emit_fcmp (f0, f2);
	  emit_fbcc (cond, lab))
      | gbranch _ = ErrorMsg.impossible "[SparcCM.gbranch]"

    end (* local *)

    fun profile _ = ()

    end (* local *)

end (* functor SparcCM *)
