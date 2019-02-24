(* Copyright 1989 by AT&T Bell Laboratories *)
structure Nonrec =
struct
local open Access Basics Absyn
in
    exception Isrec

    fun nonrec (VALRECdec[RVB{var=var as VALvar{access=LVAR(lvar),...},
		              exp,resultty,tyvars}]) =
	let val rec findexp =
		fn VARexp(ref(VALvar{access=PATH[v],...})) =>
					if v=lvar then raise Isrec else ()
		 | VARexp(ref(VALvar{access=_,...})) => ()
	         | RECORDexp l => app (fn (lab,e)=>findexp e) l
		 | SEQexp l => app findexp l
		 | APPexp (a,b) => (findexp a; findexp b)
		 | CONSTRAINTexp (e,_) => findexp e
		 | HANDLEexp (e, HANDLER h) => (findexp e; findexp h)
		 | RAISEexp e => findexp e
		 | LETexp (d,e) => (finddec d; findexp e)
		 | CASEexp (e,l) => (findexp e; app (fn RULE (_,e) => findexp e) l)
		 | FNexp l =>  app (fn RULE (_,e) => findexp e) l
		 | MARKexp(e,a,b) => findexp e
		 | _ => ()
	    and finddec =
		fn VALdec vbl => app (fn (VB{exp,...})=>findexp exp) vbl
		 | VALRECdec rvbl => app (fn(RVB{exp,...})=>findexp exp) rvbl
		 | LOCALdec (a,b) => (finddec a; finddec b)
		 | SEQdec l => app finddec l
		 | MARKdec (dec,a,b) => finddec dec
		 | _ => ()
	 in findexp exp;
	    VALdec[VB{pat=VARpat var, tyvars=tyvars,
		      exp = case resultty of 
				SOME ty => CONSTRAINTexp(exp,ty)
			      | NONE => exp}]
	end

      | nonrec _ = raise Isrec

end (* local *)
end (* struct *)
