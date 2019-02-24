(* Copyright 1989 by AT&T Bell Laboratories *)
structure Hoist : sig val hoist : (string->unit)->CPS.cexp->CPS.cexp end =
struct
 open Access CPS SortedList

type fv = lvar list

datatype cexp'
  = RECORD' of (lvar * accesspath) list * lvar * cexp' * fv
  | SELECT' of int * lvar * lvar * cexp' * fv
  | OFFSET' of int * lvar * lvar * cexp' * fv
  | APP' of lvar * lvar list
  | FIX' of function' list * fv * cexp' * fv
  | SWITCH' of lvar * (cexp' * fv) list
  | PRIMOP' of Access.primop * lvar list * lvar list * (cexp' * fv) list
withtype function' = lvar * lvar list * cexp'

 fun sum f = let fun h [] = 0 
		   | h (a::r) = f a + h r
	      in h
	     end

 fun hoist click cexp =
  let (* val _ = CPSprint.show (output std_out) (Intmap.map ctab) cexp *)
      val clicked = ref false
      val click = fn x => (clicked := true; click x)
      infix 6 \/ val op \/ = merge
      infix 7 /\ val op /\ = intersect
      infix 6 -- val op -- = fn(a,b) => remove(b,a)
      val rec hoist = 
	fn RECORD(vl, w, e) =>
	   (case hoist e
	    of (e as FIX'(l,v1,e',v2), v3) => 
		    if member v1 w
		      then (RECORD'(vl, w, e, v3), v3--[w]\/uniq(map #1 vl))
		      else let val defined = uniq(map #1 l)
			       val v4 = v2--[w]\/uniq(map #1 vl)
			    in (FIX'(l,v1,RECORD'(vl,w,e',v2),v4), v1\/(v4--defined))
			   end
	    | (e,v1) => (RECORD'(vl, w, e, v1), v1--[w]\/uniq(map #1 vl)))
	 | SELECT(i,v,w,e) =>
	   (case hoist e of
	      (e as FIX'(l,v1,e',v2), v3) => if member v1 w
		      then (SELECT'(i, v, w, e, v3), v3--[w]\/[v])
		      else let val defined = uniq(map #1 l)
			       val v4 = v2--[w]\/[v]
			    in (FIX'(l,v1,SELECT'(i,v,w,e',v2),v4),v1\/(v4--defined))
			   end
	    | (e,v1) => (SELECT'(i, v, w, e, v1),v1--[w]\/[v]))
	 | PRIMOP(i,vl,wl,[e]) =>
	   (case hoist e
	     of(e as FIX'(l,v1,e',v2),v3) =>
	      (case uniq wl /\ v1
	        of [] => let val v4 = v2--uniq wl\/uniq vl
		             val defined = uniq(map #1 l)
			  in (FIX'(l,v1,PRIMOP'(i,vl,wl,[(e',v2)]),v4),v1\/(v4--defined))
			 end
	         | _  =>  (PRIMOP'(i, vl, wl, [(e,v3)]),v3--uniq wl\/uniq vl))
	    | (e,v1) => (PRIMOP'(i, vl, wl, [(e,v1)]),v1--uniq wl\/uniq vl))
	 | PRIMOP(i,vl,wl,el) => 
		    let val el' = map hoist el
		     in (PRIMOP'(i,vl,wl,el'), foldmerge(map #2 el')--uniq wl\/uniq vl)
		    end
	 | APP(f,vl) => (APP'(f,vl), uniq(f::vl))
	 | SWITCH(v,el) => 
		    let val el' = map hoist el
		     in (SWITCH'(v, el'), foldmerge(map #2 el')\/[v])
		    end
	 | FIX(l,e) =>
	   let fun h((f,vl,(e as FIX'(l',v1,e',v2),v3))::r) =
			 let val (ll, v4) = h r
			  in case uniq vl /\ v1
			    of [] => (click "p"; 
				      ((f,vl,e')::l'@ll,
				        v2--(uniq vl)\/v1\/v4))
			     | _ => ((f,vl,e) :: ll, v3--(uniq vl)\/v4)
			 end
		 | h((f,vl,(e,v3)):: r) = 
			    let val (ll, v4) = h r
			     in ((f,vl,e) :: ll, v3--(uniq vl)\/v4)
			    end
		 | h [] = ([],[])
	       val (l,v1) = h (map (fn(f,vl,a)=>(f,vl,hoist a)) l)
	       val defined = uniq(map #1 l)
	       val v1 = v1 -- defined
	       val (e,v2) = hoist e
	       exception Down
	       fun check vl = case defined /\ uniq vl of [] => () 
						    | _ => raise Down
	       fun present (_,vx) = case defined/\vx
				      of []=>0 | _ => 1
	       val rec down' = fn (cexp,vx) => 
			    case defined /\ vx
			     of [] => (cexp,vx)
			      | _ => down cexp 
				     handle Down => (FIX'(l,v1,cexp,vx),
						     vx--defined\/v1)
	       and down =
		fn RECORD'(vl,w,e,v3) => (check(map #1 vl); 
			    let val (e',v4) = down e
			     in (RECORD'(vl,w,e',v4),v4--[w]\/uniq(map #1 vl))
			    end)
		 | SELECT'(i,v,w,e,v3) => 
			    let val (e',v4) = down e
			     in (SELECT'(i,v,w,e',v4), v4--[w]\/[v])
			    end
	         | PRIMOP'(i,vl,wl,[(e,_)]) => (check vl;
			    let val (e',v4) = down e
			     in (PRIMOP'(i,vl,wl,[(e',v4)]), v4--uniq wl\/uniq vl)
			    end)
		 | PRIMOP'(i,vl,wl,el) => 
		     (check vl;
		      if sum present el < 2 
			    then let val el' = map down' el
				  in (PRIMOP'(i,vl,wl,el'), foldmerge(map #2 el')--uniq wl\/uniq vl)
				 end
			    else raise Down)
		 | SWITCH'(v,el) => (* can't switch on a function *)
		       if sum present el < 2
			  then let val el' = map down' el
				in (SWITCH'(v,el'), foldmerge(map #2 el')\/[v])
			       end
			  else raise Down
		 | e as APP'(f,vl) => (check(f::vl); click "s"; (e, uniq(f::vl)))
		 | FIX'(m,v3,e',v4) => 
			let val v5 = v1\/(v3--defined)
			 in click "r"; 
			    (FIX'(l@m,v5,e',v4),v5\/(v4--(defined\/uniq(map #1 m))))
			end
	    in down e handle Down => (FIX'(l,v1,e,v2),
				      v2--defined\/v1)
	   end
      val rec clean =
	fn RECORD'(vl,w,e,_) => RECORD(vl,w,clean e)
	 | SELECT'(i,v,w,e,_) => SELECT(i,v,w, clean e)
	 | PRIMOP'(i,vl,wl,el) => PRIMOP(i,vl,wl,map (clean o #1) el)
         | SWITCH'(v,el) => SWITCH(v, map (clean o #1) el)
	 | APP'(f,vl) => APP(f,vl)
	 | FIX'(l,_,e,_) => FIX(map (fn (f,vl,e)=>(f,vl,clean e)) l, clean e)
      val cexp' = #1(hoist cexp)
   in if !clicked then clean cexp' else cexp
  end

end
