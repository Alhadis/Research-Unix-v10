(* Copyright 1989 by AT&T Bell Laboratories *)
functor CPScomp(CM : CMACHINE) : 
		sig val compile : Lambda.lexp -> unit end =
struct

structure CPSg = CPSgen(CM)
structure CPSopt = CPSopt(val maxfree = 3+length(CM.miscregs))
structure Closure = Closure(val maxfree = 3+length(CM.miscregs))
structure Spill = Spill(val maxfree = 3+length(CM.miscregs))

open ErrorMsg Access Basics BareAbsyn ProcessFile

 val write = CM.comment

 fun time (f,m,s) x =
        let val _ = debugmsg m
	    val t = System.Timer.start_timer()
            val r = f x
	    val t' = System.Timer.check_timer t
            val _ = (write "After "; write m; write ":\n")
        in  System.Stats.update(s,t');
	    timemsg(m ^ ": " ^ System.Timer.makestring t' ^ "s");
	    flush_out(std_out);
	    r
        end

fun compile lexp =
 let
  val reorder = time(Reorder.reorder,"reorder",System.Stats.codeopt)
  val lexp = reorder lexp

  val convert   = time(Convert.convert,"convert",System.Stats.convert)
  val (function, ctable) = convert lexp
  fun fprint (function as (f,vl,cps)) =
	  (if !System.Control.CG.printit
		then CPSprint.show write (Intmap.map ctable)
				(CPS.FIX([function],CPS.PRIMOP(P.+,[],[],[])))
		else ();
	   if !System.Control.CG.printsize then CPSsize.printsize cps else ())
  val _ = fprint function;

  val cpsopt = time(CPSopt.reduce ctable,"cpsopt",System.Stats.cpsopt)
  val function = let val (f,vl,cps) = function in (f,vl, cpsopt cps) end
  fun newconst c = let val v = mkLvar()
		    in  Intmap.add ctable (v,CPS.INTconst c); v
		   end
  fun prof(a,b,ce) = CPS.PRIMOP(P.profile, [newconst a,newconst b],nil,[ce])
  val ctable = Intmap.map ctable
  val constant = fn w => ((ctable w; true) handle Ctable => false)
  val _ = fprint function

  val closure   = time(Closure.closeCPS,"closure",System.Stats.closure)
  val (function,known,unknown) = closure(function,constant,prof)
  val constant = fn w => constant w orelse known w orelse unknown w
  val _ = fprint function

  val globalfix = time(GlobalFix.globalfix,"globalfix",System.Stats.globalfix)
  val carg = globalfix(function,known)
  val _ = app fprint (map #1 carg)

  val spill     = time(Spill.spill,"spill",System.Stats.spill)
  val constant' = let val s = Intset.new()
		      val _ = app (Intset.add s o #1 o #1) carg
		      val isfun = Intset.mem s
		   in fn v => constant v orelse isfun v
	          end
  val carg = spill(carg,constant',prof)
  val _ = (app fprint (map #1 carg); write "\n")

  val codegen   = time(CPSg.codegen,"codegen",System.Stats.codegen)
  val _ = codegen(carg,ctable,constant')
  val _ = debugmsg "done"
  in ()
 end
end (* functor CPScomp *)
