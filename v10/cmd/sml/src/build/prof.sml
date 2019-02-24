(* Copyright 1989 by AT&T Bell Laboratories *)
signature PROF = sig
 type profileInfo
 val instrumDec : Absyn.dec -> (Absyn.dec * profileInfo)
 val instrumStrb : Absyn.strb -> (Absyn.strb * profileInfo)
 val instrumFctb : Absyn.fctb -> (Absyn.fctb * profileInfo)
 val bindLambda : (Lambda.lexp * profileInfo) -> Lambda.lexp
end

abstraction Prof : PROF =
struct

open Access Absyn Lambda Basics BasicTypes ErrorMsg

type profileInfo = (lvar * string) list

val symbol = Symbol.symbol

(* Profiling globals *)
val profiling = System.Control.Profile.profiling
val profileList = ref([]: (Access.lvar * string) list)
		
val anonSym = symbol "anon"

val intreftype = CONty(refTycon,[intTy])
val alpha = VARty(mkTyvar(IBOUND 0))

fun topCCa() = VALvar{name = [symbol "toplevel.CC"],
		      access = PATH(!CoreInfo.toplevelPath),
		      typ = ref intreftype}
val assop = VALvar{name = [symbol "unboxedassign"],
	           access = INLINE P.unboxedassign,
		   typ = ref(tupleTy[intreftype,intTy] --> unitTy)}
val derefop = VALvar{name = [symbol "!"],
		     access = INLINE P.!,
		     typ = ref(CONty(refTycon,[alpha]) --> alpha)}
val addop = VALvar{name = [symbol "iadd"],
		   access = INLINE P.+,
		   typ = ref(tupleTy[intTy,intTy] --> intTy)}
fun currentvar() = VALvar{name = [symbol "current"],
		          access = PATH(!CoreInfo.currentPath), 
			  typ = ref intreftype}

fun tmpvar str = 
	let  val lvar = mkLvar()
	     val name = [symbol str]
	in (VALvar{name=name, access=LVAR(lvar), typ = ref UNDEFty},
	    VALvar{name=name, access=PATH[lvar], typ = ref UNDEFty})
	end

fun clean (path as name::names) = if Symbol.eq(name,anonSym) then names else path
  | clean x = x

fun instrdec(sp as (names,ccvara), VALdec vbl) = 
    let fun instrvb (vb as VB{pat=VARpat(VALvar{access=INLINE _,...}),...}) = vb
	  | instrvb (vb as VB{pat=CONSTRAINTpat
		    (VARpat (VALvar{access=INLINE _,...}),_),...}) = vb
	  | instrvb (VB{pat as VARpat(VALvar{access=LVAR v,name=[n],...}),exp,tyvars}) =
	      VB{pat=pat,exp=instrexp(n::clean names,ccvara)false exp,
								  tyvars=tyvars}
	  | instrvb (VB{pat,exp,tyvars}) =
		    VB{pat=pat, exp=instrexp sp false exp, tyvars=tyvars}
    in VALdec (map instrvb vbl)
    end
  | instrdec(sp as (names,ccvara), VALRECdec rvbl) = 
    let fun instrrvb (RVB{var=var as VALvar{access=LVAR v, name=[n],...},
							exp,resultty,tyvars}) =
               RVB{var=var, exp=instrexp (n::clean names, ccvara) false exp, 
					resultty=resultty, tyvars=tyvars}
	  | instrrvb _ = impossible "VALRECdec in instrdec"
    in VALRECdec(map instrrvb rvbl)
    end
  | instrdec(sp, ABSTYPEdec {abstycs,withtycs,body}) = 
	ABSTYPEdec {abstycs=abstycs,withtycs=withtycs, body=instrdec(sp,body)}
  | instrdec(sp, STRdec strbl) = STRdec (map (fn strb => instrstrb(sp,strb)) strbl)
  | instrdec(sp, ABSdec strbl) = ABSdec (map (fn strb => instrstrb(sp,strb)) strbl)
  | instrdec(sp, FCTdec fctbl) = FCTdec (map (fn fctb => instrfctb(sp,fctb)) fctbl)
  | instrdec(sp, LOCALdec(localdec,visibledec)) =
	LOCALdec(instrdec (sp,localdec), instrdec (sp,visibledec))
  | instrdec(sp, SEQdec decl) = SEQdec (map (fn dec => instrdec(sp,dec)) decl)
  | instrdec(sp, MARKdec(dec,a,b)) = MARKdec(instrdec (sp,dec), a,b)
  | instrdec(sp, other) = other

and instrstrexp(names, STRUCTstr {body,locations}) =
      STRUCTstr{body = (map (fn dec => instrdec((names,topCCa()),dec)) body),
				    locations=locations}
  | instrstrexp(names, APPstr {oper,argexp,argthin}) = 
      APPstr{oper=oper, argexp=instrstrexp(names,argexp),argthin=argthin}
  | instrstrexp(names, VARstr x) = VARstr x
  | instrstrexp(names, LETstr(d,body)) = 
		LETstr(instrdec((names,topCCa()),d), instrstrexp(names,body))

and instrstrb ((names,ccvara), STRB{strvar=strvar as STRvar{name,...},
						  def,thin,constraint}) = 
        STRB{strvar=strvar,def = instrstrexp(name@names,def),
				 thin=thin, constraint=constraint}

and instrfctb ((names,ccvara), FCTB{fctvar=fctvar as FCTvar{name,...},
						  param,def,thin,constraint}) =
        FCTB{fctvar=fctvar,param=param,def=instrstrexp(name::names,def),
					thin=thin, constraint=constraint}

and instrexp(sp as (names,ccvara)) =
 let fun istail tail =
     let fun iinstr exp = istail false exp
	 fun oinstr exp = istail true exp
	 fun instrrules tr = map (fn (RULE(p,e)) => RULE(p, tr e))
	 fun BUMPCCexp ccvara =    
	     APPexp(VARexp(ref assop), 
		    TUPLEexp[VARexp(ref ccvara),
			     APPexp(VARexp(ref addop), 
			            TUPLEexp[APPexp(VARexp(ref derefop),
				                    VARexp(ref ccvara)),
						    INTexp(1)])])
	 fun SETCURRENTexp ccvara = 
	     APPexp(VARexp(ref assop),
	            TUPLEexp[VARexp(ref (currentvar())), VARexp(ref ccvara)]) 
	 val rec instr:(exp->exp) =
	     fn RECORDexp l => RECORDexp(map (fn (lab,exp) => (lab,iinstr exp)) l)
	      | SEQexp l =>
		let fun seq [e] = [instr e]
		      | seq (e::r) = (iinstr e)::(seq r)
		      | seq nil = nil
		in SEQexp (seq l)
		end
	      | APPexp (f,a) =>
	        let fun safe(VARexp(ref(VALvar{access=INLINE P.callcc,...})))=false
		      | safe(VARexp(ref(VALvar{access=INLINE _,...}))) = true
		      | safe _ = false
		in if tail orelse (safe f)
		   then APPexp (iinstr f, iinstr a)
		   else let val (lvarb,lvara) = tmpvar("appvar")
			in LETexp (VALdec[VB{pat=VARpat(lvarb),
			                exp=APPexp(iinstr f, iinstr a),
					tyvars=nil}],
			   SEQexp([SETCURRENTexp(ccvara), VARexp(ref lvara)]))
			end
		end
	      | CONSTRAINTexp(e,t) => CONSTRAINTexp(instr e, t)
	      | HANDLEexp (e, HANDLER(FNexp l))=> 
		let fun rule(RULE(p,e)) = 
				RULE(p,SEQexp[SETCURRENTexp ccvara, instr e])
		in HANDLEexp (instr e, HANDLER(FNexp(map rule l)))
		end
	      | RAISEexp e => RAISEexp(oinstr e)
	      | LETexp (d,e) => LETexp (instrdec(sp,d), instr e)
	      | CASEexp (e,l) => CASEexp(iinstr e, instrrules instr l)
	      | FNexp l =>
		let fun dot (a,[z]) = Symbol.name z :: a
		      | dot (a,x::rest) = dot("." :: Symbol.name x :: a, rest)
		      | dot _ = impossible "no path in instrexp"
		    val name =  implode (dot ([], names))
		    val ccsym = symbol (name^".CC")
		    val ccvar = namedLvar(ccsym)
		    val ccvara' = VALvar{name=[ccsym],
				     access=PATH[ccvar],
				     typ = ref intreftype}
		    val (lvarb,lvara) = tmpvar "fnvar";
		in profileList := (ccvar,name) :: !profileList;
		   FNexp ([RULE(VARpat(lvarb), 
		                SEQexp ([BUMPCCexp(ccvara'),
					 SETCURRENTexp(ccvara'),
					 CASEexp(VARexp(ref lvara),
					 instrrules (instrexp (anonSym::names,
					                 ccvara') true) l)]))])
		end
	      | MARKexp(e,a,b) => MARKexp(instr e, a, b)
	      | e => e 
     in instr
     end
 in istail
 end

fun instrumDec absyn = 
    (profileList := [];
     (if !profiling then instrdec(([],topCCa()),absyn) else absyn, !profileList))

fun instrumStrb absyn = 
    (profileList := [];
     (if !profiling then instrstrb(([],topCCa()),absyn) else absyn, !profileList))

fun instrumFctb absyn = 
    (profileList := [];
     (if !profiling then instrfctb(([],topCCa()),absyn) else absyn, !profileList))

fun bindLambda (lexp, profileList) =
    fold (fn ((var,string),x) => APP(FN(var,x),STRING("aaaaaaaa"^string)))
	 profileList
	 (RECORD[lexp,RECORD(map (VAR o #1) profileList)])

end (* structure Instrum *)
