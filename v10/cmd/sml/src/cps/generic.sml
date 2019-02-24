(* Copyright 1989 by AT&T Bell Laboratories *)
functor CPSgen(M: CMACHINE) :
  sig structure CPS : CPS
    val codegen : (((CPS.lvar * CPS.lvar list * CPS.cexp) * bool) list
		   * (CPS.lvar -> CPS.const)
		   * (CPS.lvar -> bool))
		   -> unit
  end =
struct

structure CPS = CPS
open CPS M System.Tags Access

datatype frag = STANDARD of (lvar * lvar list * cexp) option ref
	      | KNOWN of (lvar list * cexp) * EA list option ref
	      | CONSTfrag of const

val standardformals2 = [standardcont, standardarg]
val standardformals3 = [standardclosure,standardarg,standardcont]
val notastandardformal::_ = miscregs
val any = notastandardformal

fun isreg' r = case isreg r of NONE => false | _ => true

val maxConceivableRegs = 50
val knowngen = System.Control.CG.knowngen
val stdgen = System.Control.CG.stdgen

local
  val allregs = standardformals3 @ miscregs
  val num2reg = array(maxConceivableRegs, hd allregs)
  val _ = app (fn r => case isreg r of SOME i => update(num2reg,i,r)) allregs
  val allregs' = map (fn r => case isreg r of SOME i => i) allregs
  val okreg = array(maxConceivableRegs, false)
  fun mark b = (fn r => case isreg r of (SOME i) => update(okreg,i,b) | _ => ())
  val _ = app (mark true) allregs

 in
    exception Getscratch
    fun getscratch(preferred, prohibited) =
        let fun f(x::a) = if okreg sub x then num2reg sub x else f a
              | f nil = raise Getscratch
         in app (mark false) prohibited;
	    (case isreg preferred
	     of SOME i => (if okreg sub i then preferred else f allregs')
	      | _ => f allregs')
	     before app (mark true) prohibited
	    handle e => (app (mark true) prohibited; raise e)
        end
end

fun split pred nil = (nil,nil)
  | split pred (a::r) = let val (x,y) = split pred r
			 in if pred a then (a::x, y) else (x, a::y)
		        end

fun codegen(funs : ((lvar * lvar list * cexp) * bool) list,
	    ctable : lvar -> const, isconstant : lvar -> bool) =
(* isconstant means either constant or label *)
let 
    exception Regbind
    val regbindtable : EA Intmap.intmap = Intmap.new(32, Regbind)
    val addbinding = Intmap.add regbindtable

    exception Know
    val knowtable : frag Intmap.intmap = Intmap.new(32, Know)
    val addknow = Intmap.add knowtable
    val know = Intmap.map knowtable

    exception Freemap
    val freemaptable : lvar list Intmap.intmap = Intmap.new(32, Freemap)
    val freemap = Intmap.map freemaptable

    fun makefrag ((f,vl,e),known) = 
	(addknow(f, if known then (inc knowngen; KNOWN((vl,e),ref NONE))
			     else (inc stdgen; STANDARD(ref(SOME(f,vl,e)))));
	 addbinding(f,newlabel());
	 FreeMap.freemap isconstant (Intmap.add freemaptable) e;
	 f)

    val frags = ref(map makefrag funs)
    fun addfrag f = frags := f :: !frags

    fun regbind v =
      Intmap.map regbindtable v
      handle Regbind =>
      (case ctable v of
	 INTconst i => (immed(i+i+1) handle Overflow =>
		        ErrorMsg.impossible "Overflow in cps/generic.sml")
       | f => let val lab = newlabel()
	      in addbinding(v,lab); addknow(v, CONSTfrag f); addfrag v; lab
	      end)

    fun root(RECORD(_,_,e)) = root e
      | root(SELECT(_,_,_,e)) = root e
      | root(OFFSET(_,_,_,e)) = root e
      | root(SWITCH(_,e::_)) = root e
      | root(PRIMOP(_,_,_,e::_)) = root e
      | root(e as APP _) = e

    val root1 = ref(APP(0,[]))

    fun alloc(v,cexp,default,continue) =
	let val APP(f,wl) = !root1
	    val proh = map regbind (freemap v)
	    fun delete (z,nil) = nil
	      | delete (z, a::r) = if eqreg a z then r else a::delete(z,r)
	    fun get(good,bad) =
		let val r = getscratch(good,bad@proh)
		            handle Getscratch => getscratch(default,proh)
	         in addbinding(v,r); continue r
		end
	    fun find fmls = 
               let fun g(w::wl, r::rl) = if w=v then get(r, delete(r,fmls))
					        else g(wl,rl)
		     | g(nil,nil) = get(default, fmls)
		     | g _ = ErrorMsg.impossible "cps vax 33"
		in g(wl,fmls)
	       end
         in if v=f then get(default,standardformals3)
	    else
	    case (know f handle Know => STANDARD(ref NONE))
	     of KNOWN(_,ref(SOME fmls)) => find fmls
	      | KNOWN(_,ref NONE) => get(default,nil)
	      | STANDARD _ => case length wl
		 	       of 2 => find standardformals2
				| 3 => find standardformals3
			        | _ => ErrorMsg.impossible "cps vax 44"
        end

    fun shuffle(func, args,formals) =
      let val (fv,used,args,formals) =
	    let val fv = regbind func
	     in if exists (eqreg fv) formals
		  then let val x = getscratch(any, args@formals)
			in move(fv,x); addbinding(func,x);
			   (x,[x],args,formals)
		       end
		    handle Getscratch =>
		     (addbinding(func,notastandardformal);
		      (notastandardformal, nil, fv::args, notastandardformal::formals))
		  else (fv,[fv],args,formals)
	    end
	  fun mate(a::al, b::bl)= (a,b)::mate(al,bl) 
	    | mate _ = nil
	  val (inreg,notinreg) = split (isreg' o #1) (mate(args,formals))
	  val (matched, notmatched) = split (fn(x,y)=>eqreg x y) inreg

	  fun f(nil, used) = ()
	    | f (pairs,used) = 
	    let val u' = map #1 pairs @ used
	        fun movable (a, b) = not (exists (eqreg b) u')
	     in case split movable pairs
	         of (nil,(a,b)::r) => 
			    let val x = getscratch(any,u')
			      in move(a,x); f((x,b)::r, used)
			    end
	          | (m,m') => (app move m; f(m', (map #2 m) @ used))
	    end
      in f(notmatched, (map #1 matched) @ used);
         app move notinreg;
	 jmp fv
     end

    fun allocparams(args,formals) =
       let fun f(already,a::ar,b::br) =
		let val z = getscratch(a, already@ar)
		 in addbinding(b,z); 
		    if eqreg a z then () else move(a,z);
		       f(z::already,ar,br)
		end
	     | f(l,nil,nil) = rev l
	in f(nil,args,formals)
       end

  (* Compute the maximum amount of allocation done by this function (in bytes). *)
    fun sumAlloc exp = let
	  fun sum (RECORD (fields, _, exp'), max) = sum (exp', max+(length fields)+1)
	    | sum (SELECT (_, _, _, exp'), max) = sum (exp', max)
	    | sum (OFFSET (_, _, _, exp'), max) = sum (exp', max)
	    | sum (APP _, max) = max
	    | sum (SWITCH (_, lst), max) = max + lstMax(lst, 0)
	    | sum (PRIMOP (P.makeref, _, _, [exp']), max) = sum (exp', max+2)
	    | sum (PRIMOP (P.delay, _, _, [exp']), max) = sum (exp', max+2)
	    | sum (PRIMOP (P.update, _, _, [exp']), max) = sum (exp', max+4)
	    | sum (PRIMOP (P.:=, _, _, [exp']), max) = sum (exp', max+4)
	    | sum (PRIMOP (P.fadd, _, _, [exp']), max) = sum (exp', max+3)
	    | sum (PRIMOP (P.fsub, _, _, [exp']), max) = sum (exp', max+3)
	    | sum (PRIMOP (P.fmul, _, _, [exp']), max) = sum (exp', max+3)
	    | sum (PRIMOP (P.fdiv, _, _, [exp']), max) = sum (exp', max+3)
	    | sum (PRIMOP (P.fneg, _, _, [exp']), max) = sum (exp', max+3)
	    | sum (PRIMOP (_, _, _, [exp']), max) = sum (exp', max)
	    | sum (PRIMOP (_, _, _, lst), max) = max + lstMax(lst, 0)
	  and lstMax (nil, max) = max
	    |lstMax (e::rest, max) = let val m = sum (e, 0)
		in
		  if m > max then lstMax(rest, m) else lstMax(rest, max)
		end
	  in
	    (sum (exp, 0)) * 4
	  end

    fun genfrag f = case (regbind f, know f)
	    of (_, STANDARD(ref NONE)) => ()
	     | (lab, STANDARD(r as ref (SOME(fname,[f,a,c],e)))) =>
		    (r := NONE;
		     List2.app2 addbinding ([f,a,c],standardformals3);
		     align(); mark();
		     comment(Access.lvarName fname ^ ":\n");
		     genFun(lab, e, SOME standardclosure))
	     | (lab, STANDARD(r as ref (SOME(fname,[f,a],e)))) =>
		    (r := NONE;
		     List2.app2 addbinding ([f,a],standardformals2);
		     align(); mark();
		     comment(Access.lvarName fname ^ ":\n");
		     genFun(lab, e, SOME standardcont))
	     | (_, STANDARD _) => ErrorMsg.impossible "standard with wrong args"
	     | (_, KNOWN _) => ()
	     | (lab, CONSTfrag(REALconst r)) =>
		    (align(); mark(); emitlong(8 * power_tags + tag_embedded);
		     define lab; comment("# real constant " ^ r ^ "\n");
		     realconst r)
	     | (lab, CONSTfrag(STRINGconst s)) =>
		    (align(); mark();
		     emitlong(size s * power_tags + tag_embedded);
		     define lab; emitstring s; align())

  (* generate a new code label *)
    and genlab(lab, cexp) = (root1 := root cexp; define lab; gen cexp)

  (* generate a new function header *)
    and genFun (lab, cexp, closure) = let
	  val maxAllocSz = sumAlloc cexp
	  in
	    root1 := root cexp;
	    define lab;
	    if (maxAllocSz > 0)  (** Won't support "true" concurrency **)
	      then checkLimit (maxAllocSz) else ();
	    case closure of SOME reg => beginStdFn(reg, lab) | _ => ();
	    gen cexp
	  end

    and gen cexp =
	case cexp
	 of RECORD(vl,w,e) =>
		 alloc(w, e,any,  fn w' => 
			   (record((immed(16*(length vl)+1),OFFp 0) ::
				      map (fn(x,p)=>(regbind x, p)) vl,
			           w');
			    gen e))
	  | SELECT(i,v,w,e) =>
	    alloc(w, e,any,  fn w' => (select(i,regbind v,w'); gen e))
	  | OFFSET(i,v,w,e) =>
	    let val v' = regbind v
	    in alloc(w, e,v', fn w' => (offset(i,v',w'); gen e))
	    end
	  | APP(f,args) =>
	    (case (map regbind args,
		   know f handle Know => STANDARD(ref NONE))
	      of (args',KNOWN(_,ref(SOME formals))) =>
			  shuffle(f, args', formals)
	       | (args', KNOWN((vl,cexp), r as ref(NONE))) => let
			  val lab = newlabel();
			  in
		    	    r := SOME(allocparams(args',vl));
			  (* replace fall-through with a jump to insure a mark
			   * at the beginning of every function.
			   *)
			    jmp lab; align(); mark(); define lab;
		    	    comment(Access.lvarName f ^ ":\n");
			    genFun (regbind f, cexp, NONE)
			  end
	       | (args' as [_,_], STANDARD (ref NONE)) =>
			  shuffle(f, args',standardformals2)
	       | (args' as [_,_,_], STANDARD (ref NONE)) =>
			  shuffle(f, args',standardformals3)
	       | (args' as [_,_], STANDARD(ref(SOME _))) =>
		          (shuffle(f, args',standardformals2); genfrag f)
	       | (args' as [_,_,_], STANDARD(ref(SOME _))) =>
		          (shuffle(f, args',standardformals3); genfrag f))
	  | SWITCH(v,l) => 
		let val lab = newlabel()
		    val labs = map (fn _ => newlabel()) l;
		    fun f(i, s::r) = (emitlab(i, s); f(i+4, r))
		      | f(_, nil) = ()
		    fun h(lab::labs, e::es) = (genlab(lab, e); h(labs,es))
		      | h(nil,nil) = ()
		 in fetchindexl(lab, arithtemp, regbind v);
		    jmpindexb lab;
(*		    align();   temporarily removed so 68020 will work. *)
		    define lab;
		    f (0, labs);
		    h(labs,l)
		end

	  | PRIMOP (i,vl,wl,el) => primops i (vl,wl,el)

(* warning:  on three-address instructions, be careful about 
   non-pointers in registers.  On some machines,
    addl3(a,b,c) is translated to:   mov(b,c); add(a,c);
   and it's dangerous when b is a non-pointer.  In such a case,
   usually a is "safe", so that addl3(b,a,c) works better.
  The rule is, therefore:  if the destination is a pointer register,
    then b must also be a tagged value *)

    and arithprof i = () (* profile(Profile.ARITHOVH+i,2) *)
    and compare(branch,test) ([v,w],[],[d,e]) =
		let val lab = newlabel()
		 in branch(test,regbind v, regbind w, lab); 
		    gen d; genlab(lab, e)
		end
    and primops p =
        case p of
          P.+ => (fn ([v,w],[x],[e]) =>
	  let val v' = regbind v and w' = regbind w
	  in case (isimmed v', isimmed w') of
	       (SOME k, _) =>
                 (arithprof 0; alloc(x,e,w', fn x' => addl3t(immed(k-1),w',x')))
(* the next case must be done (by all machines) with v and x in
   root registers (for offset computations in "boot") *)
	     | (_, SOME k) =>
                 (arithprof 0; alloc(x,e,v', fn x' => addl3t(immed(k-1),v',x')))
	     | _ => alloc(x,e,w',fn x' => (arithprof 1;
					   subl3(immed 1,v',arithtemp);
					   addl3t(arithtemp, w', x')));
	     gen e
	  end)
       | P.orb => (fn ([v,w],[x],[e]) =>
        let val w' =  regbind w
         in alloc(x,e,w', fn x' => (orb(regbind v, w', x'); gen e))
        end)
       | P.andb => (fn ([v,w],[x],[e]) =>
        let val w' =  regbind w
         in alloc(x,e,w', fn x' => (andb(regbind v, w', x'); gen e))
        end)
       | P.xorb => (fn ([v,w],[x],[e]) =>
         let val v' = regbind v and w' = regbind w
	 in alloc(x,e,any,fn x' => (case (isimmed v', isimmed w') of
	      (SOME k,_) => xorb(immed(k-1), w', arithtemp)
	    | (_,SOME k) => xorb(v', immed(k-1), arithtemp)
	    | _ => (xorb(v', w', arithtemp); orb(immed 1, arithtemp, x'));
				    gen e))
	 end)
       | P.notb => (fn ([v],[x],[e]) =>
          alloc(x,e,regbind v, fn x' =>
	     (notb(regbind v, x');
	      orb(immed 1, x', x');
	      gen e)))
       | P.lshift => (fn ([v,w],[x],[e]) =>
         let val v' = regbind v and w' = regbind w
	 in alloc(x,e,any, fn x' => 
		  (case (isimmed v', isimmed w') of
		     (SOME k,_) =>
          (ashr(immed 1,w',arithtemp); ashl(arithtemp,immed(k-1), x'))
		   | (_,SOME k) => 
	  (addl3(immed(~1),v',arithtemp);
	   ashl(immed(Bits.rshift(k,1)), arithtemp, x'))
		   | _ => 
          (ashr(immed 1, w',arithtemp);
	   addl3(immed(~1),v',arithtemp2);
           ashl(arithtemp, arithtemp2, x'));
		   orb(immed 1, x', x');
		   gen e))
	 end)
       | P.rshift => (fn ([v,w],[x],[e]) =>
         let val v' = regbind v and w' = regbind w
         in alloc(x,e,v', fn x' => 
            (case isimmed w' of
	       SOME k => ashr(immed(Bits.rshift(k,1)), v', x')
	     | _ => (ashr(immed 1, w',arithtemp); ashr(arithtemp, v', x'));
	     orb(immed 1, x', x');
	     gen e))
         end)
       | P.- => (fn ([v,w],[x],[e]) =>
	 let val v' = regbind v and w' = regbind w
	 in case (isimmed v', isimmed w') of
	      (SOME k, _) => (arithprof 0; alloc(x,e,w', fn x' => 
			      subl3t(w', immed(k+1), x')))
	    | (_, SOME k) => (arithprof 0; alloc(x,e,v', fn x' =>
				     subl3t(immed(k-1),v',x')))
	    | _ => alloc(x, e,v',fn x' => (arithprof 1;
					   subl3(immed 1,w',arithtemp);
					   subl3t(arithtemp, v', x')));
	    gen e
	 end)
       | P.* => (fn ([v,w],[x],[e]) =>
         let val v' = regbind v and w' = regbind w
         in alloc(x,e,any,fn x' =>
            (arithprof 3;
	     case (isimmed v', isimmed w') of
               (SOME k,_) => (ashr(immed 1, w', arithtemp);
			      mull2t(immed(k-1),arithtemp))
	     | (_,SOME k) => (ashr(immed 1, v', arithtemp);
			      mull2t(immed(k-1),arithtemp))
	     | _ => (ashr(immed 1, v', arithtemp);
		     subl3(immed 1, w', arithtemp2);
		     mull2t(arithtemp2,arithtemp));
	     orb(immed 1,arithtemp,x');
	     gen e))
         end)				  
       | P.div => (fn ([v,w],[x],[e]) =>
         let val v' = regbind v and w' = regbind w
         in alloc(x, e,any, fn x' =>
		  (arithprof 4;
		   case (isimmed v', isimmed w') of
		      (SOME k,_) =>
			(move(immed(Bits.rshift(k,1)),arithtemp);
			 ashr(immed 1, w', arithtemp2);
			 divl2(arithtemp2,arithtemp))
		    | (_,SOME k) => 
			(ashr(immed 1, v', arithtemp);
			 divl2(immed(Bits.rshift(k,1)),arithtemp))
		    | _ => 
			(ashr(immed 1, v', arithtemp);
			 ashr(immed 1, w', arithtemp2);
			 divl2(arithtemp2,arithtemp));
		    addl3(arithtemp, arithtemp, arithtemp);
		    orb(immed 1, arithtemp,x');
		    gen e))
         end)
       | P.! => (fn ([v],[w],[e]) => gen(SELECT(0,v,w,e)))
       | P.:= => (fn ([v,w],[],[e]) =>
	    let val v' = regbind v
	     in record([(immed(16*3+1),OFFp 0), (v', OFFp 0),
		       (immed 1, OFFp 0), (storeptr, OFFp 0)], storeptr);
	        storeindexl(regbind w, v', immed 1);
	        gen e
	    end)
       | P.unboxedassign => (fn ([v,w],[],[e]) =>
              (storeindexl(regbind w, regbind v, immed 1); gen e))
       | P.~ => (fn ([v],[w],[e]) =>
	 alloc(w,e,any,fn w' => (arithprof 0;subl3t(regbind v,immed 2,w');gen e)))
       | P.makeref =>
	 (fn ([v],[w],[e]) =>
	    alloc(w, e,any, fn w' =>
		(if !CGoptions.profile then profile(Profile.REFCELLS,2) else ();
		 record([(immed(power_tags+tag_array),OFFp 0),
			 (regbind v, OFFp 0)], w');
		 gen e)))
       | P.delay =>
	 (fn ([i,v],[w],[e]) =>
	    alloc(w, e,any, fn w' =>
		(if !CGoptions.profile then profile(Profile.REFCELLS,2) else ();
		 record([(regbind i, OFFp 0),(regbind v, OFFp 0)], w');
		 gen e)))
       | P.ieql => compare(ibranch,NEQ)
       | P.ineq => compare(ibranch,EQL)
       | P.> => compare(ibranch,LEQ)
       | P.>= => compare(ibranch,LSS)
       | P.< => compare(ibranch,GEQ)
       | P.<= => compare(ibranch,GTR)
       | P.subscript => (fn ([v,w],[x],[e]) =>
			alloc(x, e,any, fn x' =>
			    (arithprof 1;
			     fetchindexl(regbind v, x', regbind w);
			     gen e)))
       | P.update => (fn ([a, i, v], [], [e]) =>
	    let val a' = regbind a and i' = regbind i
	     in arithprof 1;
	        record([(immed(16*3+1),OFFp 0), (a',OFFp 0),
		        (i', OFFp 0), (storeptr, OFFp 0)], storeptr);
	        storeindexl(regbind v, a', i');
	        gen e
	    end)
       | P.unboxedupdate => (fn ([a, i, v], [], [e]) =>
	(arithprof 1;
	 storeindexl(regbind v, regbind a, regbind i);
	 gen e))
       | P.alength => (fn ([a], [w], [e]) =>
	alloc(w,  e,any,  fn w' =>
	    (arithprof 1;
	     select(~1, regbind a, arithtemp);
	     ashr(immed(width_tags-1),arithtemp, arithtemp);
(*	     orb(immed 1, arithtemp, w');
	this didn't work on the mc68020, dammit! *)
	     orb(immed 1, arithtemp, arithtemp);
	     move(arithtemp,w');
	     gen e)))
       | P.slength => (fn ([a], [w], [e]) =>
	  alloc(w, e,any, fn w' =>
	    let val a' = regbind a
	     in if isreg' a'
		   then select(~1,a',arithtemp)
		   else (move(a',w'); select(~1,w',arithtemp));
	        ashr(immed(width_tags-1), arithtemp, arithtemp);
(*	     orb(immed 1, arithtemp, w');
	this didn't work on the mc68020, dammit! *)
	     arithprof 1;
	     orb(immed 1, arithtemp, arithtemp);
	     move(arithtemp,w');
	        gen e
	    end))
       | P.store => (fn ([s,i,v], [], [e])  =>
	       (arithprof 2;
	        ashr(immed 1, regbind i, arithtemp);
		ashr(immed 1, regbind v, arithtemp2);
		storeindexb(arithtemp2, regbind s);
	        gen e))
       | P.ordof => (fn ([s,i], [v], [e]) =>
	    alloc(v, e,any, fn v' =>
	    let val s' = regbind s
	     in arithprof 3;
	        ashr(immed 1, regbind i, arithtemp);
	        if isreg' s' then fetchindexb(s', arithtemp2)
		 else (move(s',v'); fetchindexb(v',arithtemp2));
		addl3(arithtemp2,arithtemp2,arithtemp2);
		orb(immed 1, arithtemp2, v');
	        gen e
	    end))
       | P.fneg => (fn ([x], [y], [e]) =>
	    alloc(y, e,any, fn y' => (mnegg(regbind x, y'); gen e)))
       | P.profile => (fn ([index,incr],[],[c]) =>
			(case (isimmed(regbind index), isimmed(regbind incr))
			  of (SOME i, SOME v) => profile(i div 2,(v div 2)*2);
			 gen c))
       | P.boxed => (fn ([x],[],[a,b]) =>
		    let val lab = newlabel()
		     in bbs(immed 0, regbind x, lab); gen a; genlab(lab, b)
		    end)
       | P.gethdlr => (fn ([],[x],[e]) =>
		  alloc(x, e,any, fn x' => (move(exnptr,x'); gen e)))
       | P.sethdlr => (fn ([x],[],[e]) => (move(regbind x, exnptr); gen e))
       | P.fmul =>  (fn ([x,y], [z], [e]) =>
		alloc(z,  e,any, fn z' =>
		 (mulg3(regbind x, regbind y, z'); gen e)))
       | P.fdiv =>  (fn ([x,y], [z], [e]) =>
		alloc(z,  e,any, fn z' =>
		 (divg3(regbind x, regbind y, z'); gen e)))
       | P.fadd => (fn ([x,y], [z], [e]) =>
		alloc(z,  e,any, fn z' =>
		 (addg3(regbind x, regbind y, z'); gen e)))
       | P.fsub => (fn ([x,y], [z], [e]) =>
		alloc(z,  e,any, fn z' =>
		 (subg3(regbind x, regbind y, z'); gen e)))
       | P.feql => compare(gbranch,NEQ)
       | P.fneq => compare(gbranch,EQL)
       | P.fgt => compare(gbranch,LEQ)
       | P.flt => compare(gbranch,GEQ)
       | P.fge => compare(gbranch,LSS)
       | P.fle => compare(gbranch,GTR)
       
in  emitlong 1; (* Bogus tag for spacing, boot_v. *)
    let fun loop nil = ()
          | loop (frag::r) = (frags := r; genfrag frag; loop(!frags))
    in loop(!frags)
    end
(*    before print "Done!\n" *)
end

end
