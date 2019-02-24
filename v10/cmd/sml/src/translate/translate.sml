(* Copyright 1989 by AT&T Bell Laboratories *)
signature TRANSLATE = sig
	val transDec : Absyn.dec -> Lambda.lexp -> Lambda.lexp
end

structure Translate : TRANSLATE =
struct
open Access Absyn Lambda Basics BasicTypes Nonrec ErrorMsg Unboxed

val unitLexp = RECORD []

fun composeNOT (x) =  
    let val v = mkLvar()
    in FN(v,SWITCH(APP(x, VAR v),
                   [(DATAcon falseDcon, CON(trueDcon,unitLexp)),
		    (DATAcon trueDcon, CON(falseDcon,unitLexp))],NONE))
    end

val elemgtr = (fn ((LABEL{number=x,...},_),(LABEL{number=y,...},_))=> x>y);
val sorted = Sort.sorted elemgtr
val sortrec = Sort.sort elemgtr
		
val [bogusID,matchsym,bindsym] = map Symbol.symbol ["bogus","Match","Bind"]

fun raisematch() = (WILDpat,RAISE(CON(!CoreInfo.exnMatch,unitLexp)))

fun reraise() =
    let val v = mkLvar()
    in (VARpat(VALvar{name=[bogusID],access=LVAR v, typ=ref UNDEFty}), RAISE(VAR v))
    end

val printDepth = System.Control.Print.printDepth

fun translatepath [v] = VAR v
  | translatepath (x::p) = SELECT(x,translatepath p)
  | translatepath nil = impossible "translate.translatepath nil"

fun fill (APPpat(_,p)) = fill p
  | fill (CONSTRAINTpat (p,_)) = fill p
  | fill (LAYEREDpat (p,q)) = (fill p; fill q)
  | fill (RECORDpat {pats = ref (_::_),...}) = ()
  | fill (RECORDpat {fields,flex=false,pats,...}) =
	 pats := map (fn (_,p) => (fill p; p)) fields
  | fill (pat as RECORDpat {fields,flex=true,typ,pats}) =
	(app (fn (_,p) => fill p) fields;
	 let fun find (FLEXRECORDty(ref(CLOSED ty))) = find(ty)
	       | find (t as CONty(ref(TYCON{kind=RECORDtyc labels,...}),_)) = 
				(typ := t; labels)
	       | find _ = (PrintAbsyn.printPat(pat,!printDepth);
			   condemn "unresolved flexible record")
	     fun merge (a as ((id,p)::r), lab::s) =
		   if Symbol.eq(id,lab) then p :: merge(r,s) 
		   else WILDpat :: merge(a,s)
               | merge (nil, lab::s) = WILDpat :: merge(nil,s)
	       | merge (nil,nil) = nil
	       | merge _ = impossible "merge in translate"
          in pats := (merge(fields, find(!typ)) handle Syntax => [WILDpat])
         end)
  | fill _ = ()

fun polyequal() = translatepath(!CoreInfo.polyequalPath)

fun getEqualElem (CONty(_,[CONty(_,[t,_]),_])) = t
  | getEqualElem _ = VARty(ref(IBOUND 0))

fun thinStr(e,NONE) = e
  | thinStr(e,SOME(v,locs)) = APP(FN(v,RECORD(map transLoc locs)), e)

and transLoc trans =
 case trans
   of VALtrans(PATH p) => translatepath p
    | VALtrans(INLINE P.eql) => polyequal()
    | VALtrans(INLINE P.neq) => composeNOT(polyequal())
    | VALtrans(INLINE i) => PRIM i
    | THINtrans(PATH p,v,locs) => thinStr(translatepath p, SOME(v,locs))
    | CONtrans(d as DATACON{const=true,...}) => CON(d, unitLexp)
    | CONtrans(d as DATACON{const=false,...}) => 
        let val v = mkLvar() in FN(v,CON(d, VAR v)) end
    | _ => impossible "transLoc in translate"



fun transStr (VARstr(STRvar{access=PATH(path),...})) = translatepath path
  | transStr (STRUCTstr{body,locations}) = 
		makedec (SEQdec body) (RECORD(map transLoc locations))
  | transStr (APPstr{oper=FCTvar{access=LVAR(v),...},argexp,argthin}) =
	      APP(VAR v, thinStr(transStr argexp, argthin))
  | transStr (LETstr(d,body)) = makedec d (transStr body)
  | transStr _ = impossible "Translate.transStr"

and makedec (VALdec vbl) =
    fold (fn (VB{pat=VARpat(VALvar{access=INLINE(_),...}),...},b) => b
	   | (VB{pat=CONSTRAINTpat(VARpat(VALvar{access=INLINE _,...}),_),
		    exp=_,...},b) => b
	   | (VB{pat=VARpat(VALvar{access=LVAR v,...}),exp,...},b) => 
	        APP(FN(v,b), translate exp)
	   | (VB{pat,exp,...},b) => 
	      (fill pat; 
	       APP(MC.bindCompile 
		   [(pat,b),(WILDpat,RAISE(CON(!CoreInfo.exnBind,unitLexp)))],
	           translate exp)))
	 vbl
  | makedec (a as VALRECdec rvbl) =
      (makedec (nonrec a)
       handle Isrec =>
       (fn e => FIX(fold
        (fn (RVB{var=VALvar{access=LVAR(var),...},exp,...}, (vlist,llist,lexp))
              => (var::vlist,  translate exp :: llist,  lexp)
          | _ => impossible "#73 in translate")
        rvbl (nil,nil,e))))
  | makedec (LOCALdec(localdec,visibledec)) = 
          makedec(SEQdec[localdec,visibledec])

  | makedec (EXCEPTIONdec ebl) =
      fold(fn (EBgen{exn=DATACON{rep=VARIABLE(LVAR v),name,const,...},...},lexp)=>
	    	APP(FN(v,lexp),
		    if const
		    then RECORD[unitLexp,CON(refDcon,STRING (Symbol.name name))]
		    else CON(refDcon,STRING (Symbol.name name)))
	    | (EBdef{exn=DATACON{rep=VARIABLE(LVAR v),...},
	             edef=DATACON{rep=VARIABLE(PATH p),...}},
	       lexp) => APP(FN(v,lexp),translatepath p)
	    | _ => impossible "in makedec EXCEPTIONdec")
	  ebl

  | makedec (SEQdec decl) =
     (* fold (fn (dec,exp) => makedec dec exp) decl *)
      let fun f(a::r) = (makedec a) o (f r) | f nil = (fn e=>e) in f decl end

  | makedec (DATATYPEdec _) = (fn e => e)
  | makedec (ABSTYPEdec{body,...}) = makedec body
  | makedec (TYPEdec _) = (fn e => e)
  | makedec (STRdec sbl) =
      fold(fn (STRB{strvar=STRvar{access=LVAR(v),...},def,thin,...},lexp) =>
	      APP(FN(v,lexp),thinStr(transStr def, thin))
	    | _ => impossible "makedec(STRdec) in translate")
	  sbl
  | makedec (ABSdec sbl) = makedec(STRdec sbl)
  | makedec (FCTdec fbl) =
      fold(fn (FCTB{fctvar=FCTvar{access=LVAR(v),binding,...},def,thin,
	            param=STRvar{access=LVAR p,...},...},
	       lexp) =>
              APP(FN(v,lexp),FN(p,thinStr(transStr def, thin)))
	    | _ => impossible "makedec(FCTdec) in translate")
	  fbl
  | makedec (SIGdec _) = (fn e => e)
  | makedec (OPENdec _) = (fn e => e)
  | makedec (MARKdec(dec,a,b)) = makedec dec

and transrules rules = map (fn (RULE(p,e)) => ((fill p; p), translate e)) rules

and translate exp =
  case exp 
   of  INTexp i => INT i
     | REALexp r => REAL r
     | STRINGexp s => STRING s
     | RECORDexp l =>
	if sorted l
	then RECORD(map (fn(_,e)=>translate e) l)
	else let val vars = map (fn (l,e) => (l,(e,mkLvar()))) l
		 fun bind ((_,(e,v)), x) = APP(FN(v, x), translate e)
	     in fold bind vars (RECORD(map (fn(_,(_,v))=>VAR v) (sortrec vars)))
	     end
    | SEQexp [e] => translate e
    | SEQexp (e::r) => APP(FN(mkLvar(), translate(SEQexp r)), translate e)
    | APPexp(CONexp dcon, e) => CON (dcon, translate e)
    | MARKexp(e,_,_) => translate e
    | CONexp(dcon as DATACON{const=false,...}) =>
	let val v = mkLvar () in FN(v,CON (dcon, VAR v)) end
    | CONexp (dcon as DATACON{const=true,...}) => CON(dcon, unitLexp)
    | VARexp (ref(VALvar{access=PATH(path),...})) => translatepath path
    | VARexp (ref(VALvar{access=INLINE P.eql,typ=ref ty,...})) => 
	      Equal.equal(getEqualElem ty)
    | VARexp (ref(VALvar{access=INLINE P.neq,typ=ref ty,...})) => 
	      composeNOT(Equal.equal(getEqualElem ty))
    | VARexp (ref(VALvar{access=INLINE P.:=,typ=ref ty,...})) => 
	      PRIM(unboxedAssign ty)
    | VARexp (ref(VALvar{access=INLINE P.update,typ=ref ty,...})) => 
	      PRIM(unboxedUpdate ty)
    | VARexp (ref(VALvar{access=INLINE(n),typ=ref ty,...})) => PRIM n
    | VARexp (ref(OVLDvar{name,...})) =>
	     (complain("unresolved overloading: "^Symbol.name name); unitLexp)
    | APPexp (f,a) => APP(translate f, translate a)
    | CONSTRAINTexp (e,t) => translate e
    | HANDLEexp (e,HANDLER(FNexp l)) =>
	let val rules = transrules l
	    fun anywild (WILDpat,_) = true
	      | anywild (VARpat _,_) = true
	      | anywild _ = false
	    val rules = if exists anywild rules then rules@[(WILDpat,unitLexp)]
			else rules@[reraise(),(WILDpat,unitLexp)]
	 in HANDLE (translate e, MC.matchCompile rules)
	end
    | RAISEexp e => RAISE (translate e)
    | FNexp l => MC.matchCompile((transrules l)@[raisematch()])
    | CASEexp (e,l) => APP(MC.matchCompile((transrules l)@[raisematch()]),
		           translate e)
    | LETexp (d,e) => makedec d (translate e)
    | x => (PrintAbsyn.printExp(x,0,!printDepth); impossible "in translate")

val transDec = makedec

end (* structure Translate *)
