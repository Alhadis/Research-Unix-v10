(* Copyright 1989 by AT&T Bell Laboratories *)
signature SPILL =
  sig val spill : (CPS.function * bool) list * (CPS.lvar -> bool)
				 * (int * int * CPS.cexp -> CPS.cexp) ->
			(CPS.function * bool) list
  end

functor Spill(val maxfree : int) : SPILL =
struct
open FreeMap Access SortedList CPS
val error = ErrorMsg.impossible
fun sublist test =
  let fun subl(a::r) = if test a then a::(subl r) else subl r
        | subl [] = []
  in  subl
  end

local val spillname = Symbol.symbol "spillrec"
in    fun spillLvar() = namedLvar spillname
end

val ilist = PrintUtil.printClosedSequence ("[",",","]") Integer.print

fun cut(0,_) = []
  | cut(i,a::x) = a::cut(i-1,x)
  | cut(_,[]) = []

fun nextuse x =
 let fun xin[] = false | xin(y::r) = x=y orelse xin r
     fun g(level,a) =
     let val rec f =
      fn ([],[]) => level
       | ([],next) => g(level+1,next)
       | (SWITCH(v,l)::r,next) => if x=v then level else f(r,l@next)
       | (RECORD(l,w,c)::r,next) =>
	 if xin(map #1 l) then level else f(r,c::next)
       | (SELECT(i,v,w,c)::r,next) => if x=v then level else f(r,c::next)
       | (OFFSET(i,v,w,c)::r,next) => if x=v then level else f(r,c::next)
       | (PRIMOP(i,a,w,cl)::r,next) => if xin a then level else f(r,cl@next)
       | (APP(v,vl)::r,next) => if xin(v::vl) then level else f(r,next)
     in f(a,[])
     end
     fun h y = g(0,[y])
 in h
 end

local val sort = Sort.sort (fn ((i:int,_),(j,_)) => i>j)
in fun sortdups(cexp,dups) =
       map #2 (sort (map (fn dup as (v,w) => (nextuse v cexp, dup)) dups))
end

(* should do the first n and then only go 
   deep enough to prove that it is not needed *)

fun next_n_dups(0,cexp,dups) = []
  | next_n_dups(n,cexp,dups) =
    if n >= length dups
    then dups
    else cut(n,sortdups(cexp,dups))

fun show (SWITCH(v,l)) = (print "SWITCH "; print v; print "\n")
  | show (RECORD(_,w,_)) = (print "RECORD "; print w; print "\n")
  | show (SELECT(_,_,w,_)) = (print "SELECT "; print w; print "\n")
  | show (OFFSET(_,_,w,_)) = (print "OFFSET "; print w; print "\n")
  | show (PRIMOP(_,_,w::_,_)) = (print "PRIMOP "; print w; print "\n")
  | show (PRIMOP(_,vl,_,_)) = (print "PRIMOP "; ilist vl; print "\n")
  | show (APP(f,vl)) = (print "APP "; print f; ilist vl; print "\n")

nonfix before
val \/ = merge and /\ = intersect
infix 6 \/   infix 7 /\
fun mash (constant,freevars,spillmem) =
let fun f(results : lvar list,
	  uniques : lvar list,
          dups : (lvar*lvar) list,
 	  spill : (lvar list * lvar) option,
	  cexp : cexp) =
    let fun prepare l = sublist (fn x => not(constant x)) (uniq l)
        val (before,after) =  (* variables free in this operation, and after
	  			  not including the newly-bound variables *)
	 let val rec free =
	      fn SWITCH(v,l) => foldmerge(prepare[v] :: map free l)
	       | RECORD(l,w,c) =>  prepare (map #1 l) \/ freevars w
	       | SELECT(i,v,w,c) => prepare[v] \/ freevars w
	       | OFFSET(i,v,w,c) => prepare[v] \/ freevars w
	       | PRIMOP(i,a,[],cl) => foldmerge(prepare a :: map free cl)
	       | PRIMOP(i,a,[res],cl) => prepare a \/ freevars res
	       | APP(f,vl) => prepare(f::vl)
	     fun here(vl,wl) = (prepare vl, wl)
	   in case cexp
	      of SWITCH(v,l) => here([v],foldmerge(map free l))
	       | RECORD(l,w,c) =>  here(map #1 l,freevars w)
	       | SELECT(i,v,w,c) => here([v],freevars w)
	       | OFFSET(i,v,w,c) => here([v],freevars w)
	       | PRIMOP(i,a,[],cl) => here(a,foldmerge(map free cl))
	       | PRIMOP(i,a,[res],cl) => here(a,freevars res)
	       | APP(f,vl) => here(f::vl,[])
	 end

        val uniques = uniques \/ results
	val uniques_after = uniques /\ after
        val uniques_before = (uniques /\ before) \/ uniques_after
        val spill_after = 
	    case spill of
	      NONE => NONE
	    | SOME(contents,_) =>
	      case uniq contents /\ after of
	        [] => NONE
	      | _ => spill
	val maxfree' = case spill of NONE => maxfree | SOME _ => maxfree-1
	val maxfreeafter = case spill_after of
			     NONE => maxfree | SOME _ => maxfree-1
	val avail = maxfree' - length(uniques_before \/ results)
	val dups = next_n_dups(avail,cexp,dups)

        fun getpath p v =
	  if constant v orelse member uniques_before v
	  then (v, OFFp 0)
	  else let fun try((w,x)::l) = if v=w then (x,OFFp 0) else try l
		     | try [] = let val SOME (l,sv) = spill
			            fun find(i,w::l) = 
				        if v=w
				        then ((* if p
 					      then (print "Retching ";
						    print v;
						    print "\n")
					      else (); *)
					      (sv,SELp(i,OFFp 0)))
				        else find(i+1,l)
				      | find(_,[]) = error "not found in spill"
			        in find(0,l)
			        end
	       in try dups
	       end

	fun makeSpillRec args =
	    let val contents = prepare args \/ after
	        val spillrec = map (getpath true) contents
		val sv = spillLvar()
		val dups' = map (fn x => (x,x)) uniques_before @ dups
	     in (* ilist contents; print "\n"; *)
		RECORD(spillrec,sv,f([],[],dups',SOME(contents,sv),cexp))
	    end

        fun g(args,res,conts,gen) = 
	if length(prepare args \/ uniques_after) > maxfreeafter orelse
	   length res + length uniques_after > maxfreeafter
	then makeSpillRec args
	else let val paths = map (fn x => (x, getpath false x)) (uniq args)
		 fun fetchit (_,(_,OFFp 0)) = false | fetchit _ = true
	     in case sublist fetchit paths of
		  [(v,(w,SELp(i,OFFp 0)))] =>
		  let val x = dupLvar v
 		  in (* print "Fetching ";
		     print v;
		     print "\n"; *)
		     SELECT(i,w,x,f([],uniques_before,(v,x)::dups,
		     	            spill_after,cexp))
		  end
		| (v,(w,SELp(i,OFFp 0)))::_ =>
		  let val x = dupLvar v
 		  in (* print "fetching ";
		     print v;
		     print "\n"; *)
		     SELECT(i,w,x,f([],uniques_before,(v,x)::dups,spill,cexp))
		  end
	        | [] => let fun f' cexp = f(uniq res,uniques_after,
					    dups,spill_after,cexp)
		        in gen(map (#1 o (getpath false)) args,res,map f' conts)
			end
	     end

     in case ((*show cexp;*) cexp)
         of SWITCH(v,l) => g([v],[],l,fn([v],[],l)=>SWITCH(v,l))
          | RECORD(l,w,c) =>
	    if 1+length uniques_after > maxfreeafter
	    then makeSpillRec (map #1 l)
	    else let val paths = map (fn (v,p) =>
					 let val (v',p') = getpath true v 
					 in (v', combinepaths(p',p))
					 end)
	                             l
	         in RECORD(paths,w,f([w],uniques_after,dups,spill_after,c))
		 end
          | SELECT(i,v,w,c) => g([v],[w],[c], fn([v],[w],[c])=>SELECT(i,v,w,c))
	  | OFFSET(i,v,w,c) => g([v],[w],[c], fn([v],[w],[c])=>OFFSET(i,v,w,c))
	  | PRIMOP(i,a,r,cl) => g(a,r,cl, fn(a,r,cl)=>PRIMOP(i,a,r,cl))
	  | APP(f,vl) => g(f::vl,[],[],fn(f::vl,[],[])=>APP(f,vl))
   end
 in f
end

fun spill(carg,constant,prof) =
let 
exception SpillCall
val callmap = Intmap.new(64,SpillCall) : (lvar list * lvar list) Intmap.intmap
val callinfo = Intmap.map callmap
val note = Intmap.add callmap
val t = Intset.new()
val spillarg = Intset.add t
val spillmem = Intset.mem t

fun spill_call(arg as ((f,vl,body),k)) =
    if length vl < maxfree
    then (arg,uniq vl,[],NONE)
    else let val dups = map (fn x => (x,x)) vl
	     val dups' = next_n_dups(maxfree-2,body,dups)
	     val sv = spillLvar()
	     val new = map #1 dups'
	     val new_vl = sv :: new
	     val spill = SOME(vl,sv)
         in note(f,(new,vl));
	    (((f,new_vl,body),k),[],dups',spill)
	 end

val rec fixApp =
    fn e as APP(f,vl) =>
       (let val (new,old) = callinfo f
	    val sv = spillLvar()
	    val sr = map (fn x => (x,OFFp 0)) vl
	    fun find [] = []
	      | find(x::r) =
	        let fun loop(a::b,c::d) =
		        if x=a then c :: find r else loop(b,d)
		      | loop _ = error "fixApp in spill"
	        in loop(old,vl)
	        end
        in spillarg sv;
	   RECORD(sr,sv,APP(f,sv :: find new))
        end handle SpillCall => e)
     | SWITCH(v,l) => SWITCH(v,map fixApp l)
     | RECORD(l,w,c) => RECORD(l,w,fixApp c)
     | SELECT(i,v,w,c) => SELECT(i,v,w,fixApp c)
     | OFFSET(i,v,w,c) => OFFSET(i,v,w,fixApp c)
     | PRIMOP(i,a,w,l) => PRIMOP(i,a,w,map fixApp l)
    val carg' = map spill_call carg
    val carg'' = map (fn (((f,vl,b),k),uniq,dups,spill) =>
                         (((f,vl,fixApp b),k),uniq,dups,spill)) carg'
    val freevars = 
	let exception SpillFreemap
	    val m = Intmap.new(32, SpillFreemap) : lvar list Intmap.intmap
	    val _ = app (freemap constant (Intmap.add m) o #3 o #1 o #1) carg''
         in Intmap.map m
        end
    exception TooMany
    fun checkv w = if length(freevars w) >= maxfree then raise TooMany else ()
    val rec check =
     fn FIX _ => error "FIX in cps/spill"
      | APP(l,args) => ()
      | SWITCH(v,l) => app check l
      | RECORD(l,w,c) => (checkv w; check c)
      | SELECT(i,v,w,c) => (checkv w; check c)
      | OFFSET(i,v,w,c) => (checkv w; check c)
      | PRIMOP(i,args,[],l) => app check l
      | PRIMOP(i,args,w::_, l) => (checkv w; app check l)
    val masher = mash(constant,freevars,spillmem)

in map
   (fn (((f,vl,body),k),uniq,dups,spill as SOME _) =>
       ((f,vl,masher([],uniq,dups,spill,body)), k)
     | (arg as ((f,vl,body),k),uniq,dups,NONE) =>
       ((check body; arg)
        handle TooMany => ((f,vl,masher([],uniq,dups,NONE,body)), k)))
   carg''
end

end (* structure Spill *)
