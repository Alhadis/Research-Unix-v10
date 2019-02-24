(* Copyright 1989 by AT&T Bell Laboratories *)
structure Reorder : sig val reorder : Lambda.lexp -> Lambda.lexp end = 
struct

open Lambda

fun inorder({regs=ra:int,side=sa,exp=_}
              :: (rest as {regs=rb,side=sb,exp=_} :: _)) =
    (ra >= rb orelse sa andalso sb) andalso inorder rest
  | inorder _ = true

fun cost(r::rest) = max(r, 1+cost rest)
  | cost nil = 0

fun insert(a as{side=true,...}, l as {side=true,...}::_) = a::l
  | insert(a as{regs=ra,side,exp,var}, l as (b as {regs=rb,...})::c) =
       if (ra:int) >= rb then a::l else b::insert(a,c)
  | insert(a,l) = a::l

fun sort(do_it, l) = if inorder l 
     then {regs=cost(map (#regs) l), side=exists (#side) l, 
	   exp= do_it(map (#exp) l)}
     else let val l' = map (fn{regs,side,exp}=>
			      {regs=regs,side=side,exp=exp,
					var=Access.mkLvar()})
			   l
              val e0 = do_it (map (fn {var,...}=> VAR var) l')
              val l'' = fold insert l' nil
	   in {regs= cost(map (#regs) l''),
	       side = exists (#side) l'',
	       exp = fold (fn({var,exp,...},e)=>APP(FN(var,e),exp)) l'' e0}
	  end

val many = 12   (* how many regs to charge a function call *)

val rec reorder : lexp -> {regs: int, side: bool, exp: lexp} =
  fn e as VAR _ => {regs=0, side=false, exp=e}
   | FN(v,e) => {regs=1, side=false, exp= FN(v, #exp(reorder e))}
   | FIX(vl,el,e) => let val {regs,side,exp} = reorder e 
                      in {regs=regs+1,side=side,exp=FIX(vl,el,exp)}
                     end
   | APP(PRIM i, b) => let val {regs=r,side=s,exp=e1} = reorder b
		           val e2 = APP(PRIM i, e1)
		        in if Prim.pure i then {regs=r,side=s,exp=e2}
					  else {regs=many,side=true,exp=e2}
		       end
   | e as APP(a,b) => let val {regs=r,side=s,exp=e1} = 
			    sort( fn[x,y]=>APP(x,y), map reorder [a,b])
		       in {regs=many,side=true,exp=e1}
		      end
   | e as INT i => {regs=0, side=false, exp=e}
   | e as REAL i => {regs=0, side=false, exp=e}
   | e as STRING i => {regs=0, side=false, exp=e}
   | SWITCH(e0,l,d) => 
         let val {regs,side,exp}= reorder e0
             val (lr,ls,l') = fold (fn((c,e),(r,s,l)) =>
				    let val {regs,side,exp}=reorder e
				      in (max(r,regs),s orelse side,(c,exp)::l)
				     end) l (regs,side,nil)
          in case d 
              of SOME d' => let val {regs=dr,side=ds,exp=de} = reorder d'
			     in {regs=max(lr,dr),side=ls orelse ds,
				 exp=SWITCH(exp,l',SOME de)}
			    end		      
               | NONE => {regs=lr,side=ls,exp=SWITCH(exp,l',NONE)}
          end
   | RECORD l => sort(RECORD, map reorder l)
   | SELECT(i,e) => let val {regs,side,exp} = reorder e
                     in {regs=max(1,regs),side=side,exp=SELECT(i,exp)}
                    end
   | RAISE e =>     let val {regs,side,exp} = reorder e
                     in {regs=max(1,regs),side=side,exp=RAISE exp}
                    end
   | HANDLE(a,b) => let val {regs=ra,side=sa,exp=ea} = reorder a
                        val {regs=rb,side=sb,exp=eb} = reorder b
                     in {regs=ra,side=sa orelse sb,exp=HANDLE(ea,eb)}
                    end
   | e as PRIM i => {regs=0,side=false,exp=e}

val reorder = fn x => #exp (reorder x)

end
