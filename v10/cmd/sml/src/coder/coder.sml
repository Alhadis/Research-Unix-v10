(* Copyright 1989 by AT&T Bell Laboratories *)
(* coder.sml
 *
 * J.H. Reppy
 * Cornell University
 * Ithaca, NY 14853
 * jhr@cs.cornell.edu
 *
 * HISTORY:
 *   11/20/89  created
 *
 * This is a machine independent code scheduler for RISC machines with 32-bit
 * instructions.  We assume that the machine has delayed branches.
 *)

signature CODER =
sig
    eqtype label sharing type label = BaseCoder.label
    type instruction
    type sdi

    val baseLab : label   (* The symbolic base address of the current code block. *)

    val newLabel : unit -> label
    val define : label -> unit

    val emitLong : int -> unit
    val emitString : string -> unit
    val emitReal : string -> unit
    val emitLabel : (label * int) -> unit
	(* L3: emitLabel(L2, k) is equivalent to L3: emitLong(k+L2-L3) *)

    val mark : unit -> unit

    val emit : instruction -> unit
    val delay : instruction -> unit
    val emitSDI : sdi -> unit

    val comment : string -> unit

    val finish : unit -> unit

end (* signature CODER *)

functor Coder (
  structure M : MACHINSTR and E : EMITTER
  sharing type M.instruction = E.instruction
      and type M.label = E.label) : CODER =
struct

    open BaseCoder M

    datatype sdi_nd = SDI_ND of {
	instr : sdi,
	size : int ref
      }

    datatype data
      = LABEL of label
      | MARK
      | LONGconst of int
      | STRINGconst of string
      | REALconst of string
      | ADDRconst of (label * int)

    datatype blk_list
      = CODEBLK of (instruction list * blk_list)
      | SDI of (sdi_nd * blk_list)
      | DATABLK of (data list * blk_list)
      | NILBLK

    fun revCode l = let
	  fun rev (NILBLK, l) = l
	    | rev (CODEBLK(x, rest), l) = rev(rest, CODEBLK(x, l))
	    | rev (SDI(x, rest), l) = rev(rest, SDI(x, l))
	    | rev (DATABLK(x, rest), l) = rev(rest, DATABLK(x, l))
	  in
	    rev (l, NILBLK)
	  end

    datatype codept
      = NILpt
      | LABELpt of (label * codept)
      | SDIpt of (sdi_nd * codept)

    val baseLab = newLabel()  (* The base address of the current code block. *)

    val codeList = ref (DATABLK([LABEL baseLab], NILBLK))
    val codePtList = ref NILpt
    val codeLen = ref 0
    val numSlots = ref 0
    val numNops = ref 0

    fun emitCode I = (codeList :=
	  case !codeList
	   of (CODEBLK(cl, rest)) => CODEBLK(I::cl, rest)
	    | lst => CODEBLK([I], lst))

  (* Insert an instruction/nop pair into the code list.  If the instruction is a jump,
   * then start a DATABLK to insure that the jump is the end of a code block. *)
    fun emitDelay I = let
	  val clst = case !codeList
	       of (CODEBLK(cl, rest)) => CODEBLK(nop::I::cl, rest)
		| lst => CODEBLK([nop, I], lst)
	  in
	    codeList :=
	      case (instrKind I) of IK_JUMP => DATABLK(nil, clst) | _ => clst
	  end

    fun emitData D = (codeList :=
	  case !codeList
	   of (DATABLK(dl, rest)) => DATABLK(D::dl, rest)
	    | lst => DATABLK([D], lst))

    fun reset () = (
	  codeList := DATABLK([LABEL baseLab], NILBLK);
	  codePtList := NILpt; codeLen := 0;
	  numSlots := 0; numNops := 0)

    fun padString s = (case ((size s) mod 4)
	   of 0 => s
	    | 1 => (s ^ "\000\000\000")
	    | 2 => (s ^ "\000\000")
	    | 3 => (s ^ "\000"))

    fun emitLong i = (
	  emitData (LONGconst i);
	  codeLen := !codeLen + 4)

    fun emitString s = let
	  val s' = padString s
	  in
	    emitData (STRINGconst s');
	    codeLen := !codeLen + (size s')
	  end

    fun emitReal r = (
	  emitData (REALconst r);
	  codeLen := !codeLen + 8)

    fun emitLabel args = (
	  emitData (ADDRconst args);
	  codeLen := !codeLen + 4)

    fun define (l as Label{addr, ...}) = (
	  addr := !codeLen;
	  codePtList := LABELpt(l, !codePtList);
	  emitData (LABEL l))

    fun mark () = (emitData MARK; codeLen := !codeLen + 4)

    fun emit I = (emitCode I; codeLen := !codeLen + 4)
    fun delay I = (emitDelay I; codeLen := !codeLen + 8)

    fun emitSDI I = let
	  val minSz = minSize I
	  val nd = SDI_ND{instr = I, size = ref minSz}
	  in
	    codeList := SDI(nd, !codeList);
	    codePtList := SDIpt(nd, !codePtList);
	    codeLen := !codeLen + minSz
	  end

    val comment = E.comment


    fun computeSizes () = let
	  fun reverse l = let  (* reverse l and remove leading labels *)
		fun clean (LABELpt(_, rest)) = clean rest
		  | clean l = l
		fun rev (NILpt, l) = clean l
		  | rev (LABELpt(lab, rest), l) = rev(rest, LABELpt(lab, l))
		  | rev (SDIpt(lab, rest), l) = rev(rest, SDIpt(lab, l))
		in
		  rev (l, NILpt)
		end
	  fun deltaSize (SDI_ND{instr, size}) = let
		val (isMax, newSz) = sizeOf instr
		val dsz = newSz - (!size)
		in
		  if (dsz > 0)
		    then (size := newSz; (isMax, dsz))
		    else (isMax, 0)
		end
	  fun adjust (NILpt, 0, _) = ()
	    | adjust (NILpt, delta, l) = (
		codeLen := !codeLen + delta;
		adjust (reverse l, 0, NILpt))
	    | adjust (LABELpt(lab as Label{addr, ...}, rest), delta, l) = (
		addr := !addr + delta;
		adjust(rest, delta, LABELpt(lab, l)))
	    | adjust (SDIpt(sdi, rest), delta, l) = (
		case (deltaSize sdi)
		 of (true, dsz) => adjust(rest, delta+dsz, l)
		  | (false, dsz) => adjust(rest, delta+dsz, SDIpt(sdi, l)))
	    val codePts = reverse(!codePtList)
	  in
	    codePtList := NILpt;
	    adjust (codePts, 0, NILpt)
	  end (* computeSizes *)


  (** Instruction scheduling and machine code emission **)

    datatype instr_nd         (* Nodes in the resource dependency graph *)
      = IND of {
	id : int,               (* unique id for equality testing *)
	instr : instruction,    (* The instruction *)
	nsuccs : int,           (* The number of successors *)
	succs : instr_nd list,
	succlock : bool,        (* If this instruction has an interlock with one of *)
				(* its successors, then this is true. *)
	maxpathlen : int,       (* The length of the longest path to a leaf. *)
	npreds : int ref        (* The number of predecessors.  This is incremented *)
				(* when building the graph, and decremented as *)
				(* each predecessor is scheduled. *)
      }

    fun member (IND{id = x, ...}, lst) = let
	  fun mem nil = false
	    | mem (IND{id = y, ...}::rest) = ((x = y) orelse (mem rest))
	  in
	    mem lst
	  end

    fun merge (nil, lst) = lst
      | merge (nd :: rest, lst) = if (member(nd, lst))
	  then merge (rest, lst)
	  else merge (rest, nd :: lst)

    fun incPreds nil = ()
      | incPreds (IND{npreds, ...} :: rest) = (npreds := !npreds + 1; incPreds rest)

  (* Order a pair of instructions *)
    fun orderInstrPair (nd1 as IND a, nd2 as IND b) =
	  if ((#succlock a) = (#succlock b))
	    then let val n1 = (#nsuccs a) and n2 = (#nsuccs b)
	      in
		if (n1 = n2)
		  then let val p1 = (#maxpathlen a) and p2 = (#maxpathlen b)
		    in
		      if ((p1 > p2) orelse ((p1 = p2) andalso ((#id a) > (#id b))))
			then (nd1, nd2)
			else (nd2, nd1)
		    end
		  else if (n1 > n2)
		    then (nd1, nd2)
		    else (nd2, nd1)
(********************************
		if ((n1 > n2)
		orelse ((n1 = n2) andalso ((#maxpathlen a) >= (#maxpathlen b))))
		  then (nd1, nd2)
		  else (nd2, nd1)
********************************)
	      end
	    else if (#succlock a)
	      then (nd1, nd2)
	      else (nd2, nd1)

  (* Schedule and emit the instructions of a straight-line block of code. *)    
    fun schedBlock (exitInstr, blks) = let
	  val exitDep = case exitInstr
		 of NONE => (fn _ => false)
		  | (SOME e) => let
		      val (exitUses, exitDefs) = rUseDef e
		      val f = exists (fn r => (exists (fn x => (r = x)) exitUses))
		      val g = exists (fn r => (exists (fn x => (r = x)) exitDefs))
		      in
			fn I => let
			      val (u, d) = rUseDef I
			      in
				(f d) orelse (g d) orelse (g u)
			      end
		      end
	(* make a new instr_nd *)
	  fun mkINd (n, I, nil) =
		IND{id = n, instr = I,
		  nsuccs = 0, succs = nil, succlock = exitDep I,
		  maxpathlen = 0, npreds = ref 0}
	    | mkINd (n, I, succLst) = let
		fun f (nil, len, lock, mpl) = (len, lock, mpl)
		  | f (IND{instr, maxpathlen, ...} :: rest, len, lock, mpl) =
		      f (rest, len+1, hazard (I, instr),
			if (maxpathlen > mpl) then maxpathlen else mpl)
		val (len, lock, mpl) = f (succLst, 0, false, 0)
		in
		  IND{id = n, instr = I,
		    nsuccs = len, succs = succLst, succlock = lock,
		    maxpathlen = mpl+1, npreds = ref 0}
		end
	(* resource use/def vectors *)
	  val lastUse = array (numResources, nil)
	  val lastDef = array (numResources, nil)
	(* find resource dependencies *)
	  fun findDeps rsrc = let
		fun add (nil, lst) = lst
		  | add (r :: rest, lst) = add (rest, merge(rsrc sub r, lst))
		in
		  add
		end
	  val findUseDeps = findDeps lastUse
	  val findDefDeps = findDeps lastDef
	(* update resource use/def vectors *)
	  fun updateUseDefs nd = let
		val ndl = [nd]
		val updateUses =
		      app (fn r => update(lastUse, r, nd::(lastUse sub r)))
		val updateDefs =
		      app (fn r => (update(lastDef, r, ndl); update(lastUse, r, nil)))
		in
		  fn (ruses, rdefs) => (updateDefs rdefs; updateUses ruses)
		end
	(* extract the dependency graph roots from the use/def vectors *)
	  fun roots () = let
		fun isRoot (IND{npreds, ...}) = (!npreds = 0)
		fun rootsOf (nil, lst) = lst
		  | rootsOf (nd::rest, lst) = if (isRoot nd)
		      then rootsOf (rest, nd::lst)
		      else rootsOf (rest, lst)
		fun mergeRoots (~1, lst) = lst
		  | mergeRoots (i, lst) = let
		      val rlst = rootsOf (merge (lastDef sub i, lastUse sub i), nil)
		      in
			mergeRoots (i-1, merge (rlst, lst))
		      end
		in
		    mergeRoots (numResources-1, nil)
		end
	(* Build the dependency graph for a list of instructions, returning the list
	 * of roots (instructions without predecessors). *)
	  fun buildDepGraph blkList = let
		fun doInstrs (nil, n) = n
		  | doInstrs (I :: rest, n) = (
		      case (instrKind I)
		       of IK_NOP => (numSlots := !numSlots + 1)
			| IK_INSTR => let
			    val (ruses, rdefs) = rUseDef I
			  (* find use/def, def/use and def/def dependencies *)
			    val succLst = findUseDeps (rdefs,
				  findDefDeps (rdefs, findDefDeps (ruses, nil)))
			    val nd = mkINd (n, I, succLst)
			    in
			      incPreds succLst;
			      updateUseDefs nd (ruses, rdefs)
			    end
			| IK_JUMP => (
			    ErrorMsg.impossible "[Coder.doInstrs: unexpected jump]")
		      (* end case *);
		      doInstrs (rest, n+1))
		fun build (NILBLK, _) = ()
		  | build (CODEBLK(instrs, rest), n) =
		      build (rest, doInstrs (instrs, n))
		  | build _ = ErrorMsg.impossible "[Coder.build: bad block]"
		in
		  build (blkList, 0)
		end (* buildDepGraph *)

	(* Choose the next instruction from a list of candidates.  Instructions that
	 * don't interlock with the previously scheduled instruction are given
	 * priority. *)
	  fun chooseNextInstr (prev, (x :: rest)) = let
		val prevLock = case prev
		     of (NONE) => (fn _ => false)
		      | (SOME prev) => (fn (IND{instr, ...}) => hazard(prev, instr))
	      (* choose1 & choose2 find the best instruction to schedule next.
	       * choose1 is used as long as the current choice interlocks with prev,
	       * choose2 is used after a non-interlocking choice is found. *)
		fun choose1 (choice, done, nil) = (choice, done)
		  | choose1 (choice, done, nd::rest) = if (prevLock nd)
		      then let
			val (a, b) = orderInstrPair (choice, nd)
			in
			  choose1 (a, b::done, rest)
			end
		      else choose2 (nd, choice::done, rest)
		and choose2 (choice, done, nil) = (choice, done)
		  | choose2 (choice, done, nd::rest) = if (prevLock nd)
		      then choose2 (choice, nd::done, rest)
		      else let
			val (a, b) = orderInstrPair (choice, nd)
			in
			  choose2 (a, b::done, rest)
			end
		val (IND{succs, instr, ...}, remainder) = if (prevLock x)
		      then choose1 (x, nil, rest)
		      else choose2 (x, nil, rest)
		fun addSuccs (nil, candidates) = candidates
		  | addSuccs ((nd as IND{npreds, ...}) :: rest, candidates) = let
		      val n = !npreds
		      in
			npreds := n-1;
			if (n = 1)
			  then addSuccs(rest, nd::candidates)
			  else addSuccs(rest, candidates)
		      end
		in
		  (instr, addSuccs(succs, remainder))
		end
	    | chooseNextInstr _ = (ErrorMsg.impossible "[Coder.chooseNextInstr]")

	(* assign an order to the instructions, based on the dependency graph. *)
	  fun assignOrder () = let
		fun emitNop () = (numNops := !numNops + 1; nop)
		fun checkLock (i1, i2, cl) = if (needsNop(i1, i2))
		      then (numNops := !numNops + 1; nop :: cl)
		      else cl
		fun order (NONE, nil, _) = let
		      val (SOME e) = exitInstr
		      in
			numNops := !numNops + 1;
			[nop, e]
		      end
		  | order (SOME prev, nil, cl) = (
		      case exitInstr
		       of NONE => (prev :: cl)
			| (SOME e) => (
			    if (exitDep prev)
			      then (
				numNops := !numNops + 1;
				nop :: e :: checkLock(prev, e, prev :: cl))
			      else (prev :: e :: cl))
		      (* end case *))
		  | order (prev, rest, cl) = let
		      val (next, remainder) = chooseNextInstr (prev, rest)
		      val newCL = case prev
			   of (SOME x) => checkLock(x, next, (x :: cl))
			    | NONE => cl
		      in
			order (SOME next, remainder, newCL)
		      end
		in
		  rev (order(NONE, roots(), nil))
		end (* assignOrder *)
	  in
	    buildDepGraph blks;
	    assignOrder ()
	  end (* schedBlock *)

    fun sched (NILBLK, blst) = blst
      | sched (DATABLK(dl, rest), blst) = let
	  fun adjust (nil, dl) = dl
	    | adjust ((lab as LABEL(Label{addr, ...})) :: rest, dl) = (
		addr := !addr - 4*(!numSlots - !numNops);
		adjust (rest, lab :: dl))
	    | adjust (d :: rest, dl) = adjust(rest, d :: dl)
	  in
	    sched (rest, DATABLK(adjust (dl, nil), blst))
	  end
      | sched (arg, blst) = let
	  fun insertCode (nil, nil, bl) = bl
	    | insertCode (nil, cl, bl) = CODEBLK(cl, bl)
	    | insertCode (I::rest, cl, bl) = (
		case (instrKind I)
		 of IK_JUMP =>
		      insertCode ((tl rest), nil, DATABLK(nil, CODEBLK(nop::I::cl, bl)))
		  | _ => insertCode (rest, I::cl, bl))
	  fun findBlk (CODEBLK(cl, rest), lst) = findBlk(rest, CODEBLK(cl, lst))
	    | findBlk (SDI(SDI_ND{instr, size}, rest), lst) =
		findBlk (insertCode(expand (instr, !size), nil, rest), lst)
	    | findBlk (rest, bl) = (rest, bl)
	  val (rest, b as CODEBLK(cl, bl)) = findBlk (arg, NILBLK)
	  val newCL = (case cl
		 of (i1 :: i2 :: instrs) => (case (instrKind i1, instrKind i2)
		     of (IK_NOP, IK_JUMP) => (
			  numSlots := !numSlots + 1;
			  schedBlock (SOME i2, CODEBLK(instrs, bl)))
		      | _ => schedBlock (NONE, b))
		  | _ => schedBlock (NONE, b))
	  in
	    sched (rest, CODEBLK(newCL, blst))
	  end

  (* reverse the code list, while expanding SDIs and reversing instruction lists.
   * This pass is the alternative to the scheduling pass. *)
    fun noSched (NILBLK, bl) = bl
      | noSched (CODEBLK(cl, rest), bl) = noSched (rest, CODEBLK(rev cl, bl))
      | noSched (SDI(SDI_ND{instr, size}, rest), bl) =
	  noSched (rest, CODEBLK(expand(instr, !size), bl))
      | noSched (DATABLK(dl, rest), bl) = noSched (rest, DATABLK(rev dl, bl))

    fun finish () = let
	  val emitInstrs = app E.emitInstr
	  fun emitDataList nil = ()
	    | emitDataList (d :: rest) = (case d
		 of (LABEL lab) => E.define lab
		  | MARK => E.mark ()
		  | (LONGconst n) => E.emitLong n
		  | (STRINGconst s) => E.emitString s
		  | (REALconst r) => E.emitReal r
		  | (ADDRconst args) => E.emitAddr args
		(* end case *);
		emitDataList rest)
	  fun emitBlk NILBLK = ()
	    | emitBlk (CODEBLK(cl, rest)) = (emitInstrs cl; emitBlk rest)
	    | emitBlk (DATABLK(dl, rest)) = (emitDataList dl; emitBlk rest)
	    | emitBlk _ = (ErrorMsg.impossible "[Coder.finish.emitBlk]")
	  fun schedule cl = if (!System.Control.CG.scheduling)
		then revCode (sched (revCode cl, NILBLK))
		else noSched (cl, NILBLK)
	  val _ = computeSizes ()
	  val newCL = schedule (!codeList before (codeList := NILBLK))
	  in
	    E.init (!codeLen - 4*(!numSlots - !numNops));
	    emitBlk newCL;
	    reset()
	  end (* finish *)

end (* Coder *)
