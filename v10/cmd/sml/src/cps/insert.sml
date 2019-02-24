(* Copyright 1989 by AT&T Bell Laboratories *)
structure Insert : sig val insert : (CPS.function * bool) list ->
				    (CPS.function * bool) list
		   end =
struct
  open Access CPS
  fun makecallers (arg : (CPS.function * bool) list) =
    let local open Intmap
	   in exception Body
	      val bodymap: (function * lvar list ref) intmap =
						 Intmap.new(32, Body)
	      val getbody = map bodymap
	      val setbody = add bodymap

	      exception Caller
	      val callermap : function list intmap = Intmap.new(32, Caller)
	      val callers = map callermap
	      val setcallers = add callermap
	 end
        fun initcalled(func as (f,vl,e)) = setbody(f, (func,ref nil))

	fun findcalled(f,vl,e) =
	  let fun g(RECORD(_,_,e)) = g e
		| g(SELECT(_,_,_,e)) = g e
		| g(OFFSET(_,_,_,e)) = g e
		| g(SWITCH(_,el)) = app g el
		| g(PRIMOP(_,_,_,el)) = app g el
		| g(APP(f',_)) = let val (_,r) = getbody f'
				  in r := f :: !r
				 end handle Body => ()
	   in g e
	  end

	val sort = Sort.sort (op > : int * int -> bool)
	fun mashcallers (f,vl,e) =
	  let val (_, ref callers) = getbody f
	   in setcallers(f, map (#1 o getbody) (SortedList.uniq(sort callers)))
	  end

     in app (initcalled o #1) arg;
	app (findcalled o #1) arg;
	app (mashcallers o #1) arg;
	callers
    end

fun insert(arg : (CPS.function * bool) list) =
 let val callers = makecallers arg
     
     val OKset = Intset.new()
     val markOK = Intset.add OKset
     val isOK = Intset.mem OKset

     fun dfs(f,vl,e) =
      let fun bad(RECORD(_,_,e)) = false
	    | bad(SELECT(_,_,_,e)) = bad e
	    | bad(OFFSET(_,_,_,e)) = bad e
	    | bad(SWITCH(_,el)) = exists bad el
	    | bad(PRIMOP(P.:=,_,_,_)) = false
	    | bad(PRIMOP(P.update,_,_,_)) = false
	    | bad(PRIMOP(_,_,_,el)) = exists bad el
	    | bad(APP(f,_)) = not(isOK f)
       in if isOK f orelse bad e
	    then ()
	    else (markOK f; app dfs (callers f))
      end

     fun makeOK(func as (f,vl,e)) =
      let fun g(e as RECORD _) = e
	    | g(SELECT(i,v,w,e)) = SELECT(i,v,w,g e)
	    | g(OFFSET(i,v,w,e)) = OFFSET(i,v,w,g e)
	    | g(SWITCH(v,el)) = SWITCH(v, map g el)
	    | g(e as PRIMOP(P.:=,_,_,_)) = e
	    | g(e as PRIMOP(P.update,_,_,_)) = e
	    | g(PRIMOP(p,vl,wl,el)) = PRIMOP(p,vl,wl, map g el)
	    | g(e as APP(f',v::_)) = if isOK f' then e
				     else RECORD([(v,OFFp 0)],mkLvar(),e)
       in if isOK f then func
	  else let val func' = (f,vl,g e)
	        in dfs func'; func'
	       end
      end

  in app (dfs o #1) arg;
     map (fn (func,known) => (makeOK func, known)) arg
 end
end
