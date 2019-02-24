(* Copyright 1989 by AT&T Bell Laboratories *)
signature FREEMAP =
  sig
    val freemap : (CPS.lvar -> bool)
		    -> (CPS.lvar * CPS.lvar list -> unit)
			-> (CPS.cexp -> CPS.lvar list)
    val freemapClose : CPS.cexp * (CPS.lvar -> bool)
			-> (CPS.lvar -> CPS.lvar list)
  end

structure FreeMap : FREEMAP =
struct
open CPS SortedList

fun sublist test =
  let fun subl(a::r) = if test a then a::(subl r) else subl r
        | subl nil = nil
  in  subl
  end

fun freemap constant add =
let val clean = (sublist (not o constant)) o uniq
    val enter = fn (x,y) => if constant x then y else enter(x,y)
    fun setvars (w,free) = let val g = rem(w,free)
			   in add(w,g); g
			   end
    val rec freevars =
	 fn APP(v,args) => enter(v,clean args)
	  | SWITCH(v,l) => enter(v,foldmerge (map freevars l))
	  | RECORD(l,w,ce) => merge(clean (map #1 l), setvars(w, freevars ce))
	  | SELECT(_,v,w,ce) => enter(v, setvars(w, freevars ce))
	  | OFFSET(_,v,w,ce) => enter(v, setvars(w, freevars ce))
	  | PRIMOP(_,args,ret,ce) =>
	     let fun f(nil,a) = a
		   | f(w::wl,a) = f(wl,setvars(w,a))
	     in  merge(clean args,f(ret,foldmerge(map freevars ce)))
	     end
	  | FIX _ => ErrorMsg.impossible "FIX in Freemap.freemap"
in freevars
end

(* Produces a free variable mapping at each function binding.
   The mapping includes the functions bound at the FIX, but
   not the arguments of the function.
   It assumes that the only lvars which refer to constants
   are the arguments of PRIMOPs, contents of RECORDs, and
   arguments of APPs.  In particular, the optimizer had better
   get rid of SWITCHes on constants. *)
fun freemapClose(ce,constant) =
let exception Freemap
    val vars : lvar list Intmap.intmap = Intmap.new(32, Freemap)
    val notconst = sublist (not o constant)
    fun setvars(v,l) = (Intmap.add vars (v,l); l)
    val rec freevars =
	 fn FIX(l,ce) =>
		let val functions = uniq(map #1 l)
		    val freel = map (fn(v,args,body) =>
				       setvars(v,remove(uniq args,freevars body)))
				    l
		in  remove(functions,foldmerge((freevars ce)::freel))
		end
	  | APP(v,args) => enter(v,notconst(uniq args))
	  | SWITCH(v,l) => foldmerge ([v]::(map freevars l))
	  | RECORD(l,w,ce) => merge(notconst(uniq(map #1 l)),
				    rem(w,freevars ce))
	  | SELECT(_,v,w,ce) => enter(v,rem(w,freevars ce))
	  | OFFSET(_,v,w,ce) => enter(v,rem(w,freevars ce))
	  | PRIMOP(_,args,ret,ce) =>
	      let val args = notconst(uniq args)
	      in merge(args,remove(uniq ret,foldmerge(map freevars ce)))
	      end
in  freevars ce; Intmap.map vars
end

(* temporary, for debugging *)
fun timeit f a =
  let val t = System.Timer.start_timer()
      val r = f a
  in  System.Stats.update(System.Stats.freemap,System.Timer.check_timer t);
      r
  end
val freemap = timeit freemap
val freemapClose = timeit freemapClose

end (* structure FreeMap *)

