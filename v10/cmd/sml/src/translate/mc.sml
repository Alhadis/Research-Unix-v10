(* Copyright 1989 by AT&T Bell Laboratories *)
signature MC = sig
	structure A : BAREABSYN
	structure L : LAMBDA
	val matchCompile : (A.pat * L.lexp) list -> L.lexp
	val bindCompile : (A.pat * L.lexp) list -> L.lexp
	end

structure MC : MC = struct

structure A : BAREABSYN = BareAbsyn
structure L : LAMBDA = Lambda

open A L
open Access Basics ErrorMsg

val printDepth = System.Control.Print.printDepth

val patsUsed : int list ref = ref []
val maybeUsed : int list ref = ref []
val results : (lvar * lvar list) list ref = ref []

fun mark(taglist,tag : int) =
    let fun newtag[] = [tag]
	  | newtag(taglist as (t::more)) =
		if tag = t then taglist
		else if tag < t then tag :: taglist
		else t :: newtag more
    in taglist := newtag (!taglist)
    end

fun unused rules =
    let fun find([],[],_) = []
	  | find([],_::r,i) = i :: find([],r,i+1)
	  | find(taglist as tag::tags,_::r,i) =
		if tag = i
		then find(tags,r,i+1)
		else i :: find(taglist,r,i+1)
	  | find _ = ErrorMsg.impossible "unused in mc"
    in  find(!patsUsed,rules,0)
    end
fun redundant rules =
	if length rules = length (!patsUsed) then []
	else unused rules
fun areNotRedundant () =
	case !maybeUsed of
	  [] => ()
	| (tag::_) => (mark(patsUsed,tag); maybeUsed := [])
fun areRedundant () = maybeUsed := []

fun bind(x,v,(bindings,tag)) = ((v,x)::bindings,tag)

fun layer (x,CONSTRAINTpat(pat,_),rhs) = layer(x,pat,rhs)
  | layer (x,VARpat(VALvar{access=LVAR v,...}),rhs) = bind(x,v,rhs)
  | layer _ = impossible "bad layered pattern in mc"

fun convert(bindings,tag) =
	let val (f,free) = nth(!results,tag)
	    fun order [] = []
	      | order (v::tl) =
		let fun f [] = ErrorMsg.impossible "convert in translate/mc.sml"
		      | f ((w,z)::tl) = if v=w then z else f tl
		in  f bindings :: order tl
		end
	    val args = order free
	in  mark(patsUsed,tag); APP(VAR f,RECORD(map VAR args))
	end
fun convertDefault(bindings,tag) =
	let val (f,free) = nth(!results,tag)
	    fun order [] = []
	      | order (v::tl) =
		let fun f [] = ErrorMsg.impossible "convertD in translate/mc.sml"
		      | f ((w,z)::tl) = if v=w then z else f tl
		in  f bindings :: order tl
		end
	    val args = order free
	in  mark(maybeUsed,tag); APP(VAR f,RECORD(map VAR args))
	end

val rec freevars =
	fn VARpat(VALvar{access=LVAR v,...}) => [v]
	 | RECORDpat{pats=ref pats,...} => SortedList.foldmerge(map freevars pats)
	 | APPpat(_,pat) => freevars pat
	 | CONSTRAINTpat(pat,_) => freevars pat
	 | LAYEREDpat(l,r) => SortedList.merge(freevars l,freevars r)
	 | _ => []

fun setup rules =
  let val arg = mkLvar()
      fun t(i,(pat,result)::more) =
	  let val (header,r) = t(i+1,more)
	      val record = mkLvar()
	      val returnit = mkLvar()
	      val free = freevars pat
	      fun f(_,[]) = result
	        | f(i,v::tl) = APP(FN(v,f(i+1,tl)),SELECT(i,VAR record))
	  in results := (returnit,free) :: !results;
	     (fn l => header(APP(FN(returnit,l),FN(record,f(0,free)))),
	      (pat,([],i)) :: r)
	  end
	| t _ = (fn x => x,[])
  in  patsUsed := []; maybeUsed := [];
      let val (header,r) = t(0,rules)
      in (fn l => FN(arg,header(APP(l,VAR arg))),r)
      end
  end

fun bindfields(record,fields,e)=
  let fun select(i, []) = e
        | select(i, x::xs) = APP(FN(x,select(i+1,xs)),SELECT(i,VAR record))
  in  select(0,fields)
  end

fun andSwitch x =
let
fun andS [] = ([],[])
  | andS ((p::fields,rhs)::more) =
     (case p of
       INTpat i =>
	let val (cases,default) = andS more
	    fun addto ((switch as (INTcon j,pats))::more) =
		    if i = j then ((INTcon i,(fields,rhs)::pats)::more)
		    else switch :: addto more
	      | addto [] = [(INTcon i,(fields,rhs)::default)]
	      | addto _ = impossible "983 type error in match compiler"
	in  (addto cases,default)
	end
     | REALpat r =>
	let val (cases,default) = andS more
	fun addto ((switch as (REALcon s,pats))::more) =
		if r = s then ((REALcon r,(fields,rhs)::pats)::more)
		else switch :: addto more
	  | addto [] = [(REALcon r,(fields,rhs)::default)]
	  | addto _ = impossible "48 type error in match compiler"
	in  (addto cases,default)
	end
     | STRINGpat s =>
	let val (cases,default) = andS more
	    fun addto ((switch as (STRINGcon t,pats))::more) =
		  if s = t then ((STRINGcon s,(fields,rhs)::pats)::more)
		  else switch :: addto more
	      | addto [] = [(STRINGcon s,(fields,rhs)::default)]
	      | addto _ = impossible "482 type error in match compiler"
	in  (addto cases,default)
	end
     | CONpat(dcon as DATACON{name=r1,...}) =>
	let val (cases,default) = andS more
	  fun addto ((switch as (DATAcon(DATACON {name=r2,...}),pats))::more) =
		  if Symbol.eq(r1,r2)
		    then (DATAcon dcon,(fields,rhs)::pats)::more
		    else switch :: addto more
	    | addto [] = [(DATAcon dcon,(fields,rhs)::default)]
	    | addto _ = impossible "87 type error in match compiler"
	in  (addto cases,default)
	end
     | APPpat(dcon as DATACON{name=r1,...},p) =>
	let val (cases,default) = andS more
	fun addto ((switch as (DATAcon(DATACON {name=r2,...}),pats))::more) =
		if Symbol.eq(r1,r2)
		then ((DATAcon dcon,(p::fields,rhs)::pats)::more)
		else switch :: addto more
	  | addto [] =
		let fun addwild (fields,rhs) = (WILDpat::fields,rhs)
		in  [(DATAcon dcon,(p::fields,rhs)::(map addwild default))]
		end
	  | addto _ = impossible "444 type error in match compiler"
	in  (addto cases,default)
	end
     | WILDpat =>
	let val (cases,default) = andS more
	fun addto (((con as DATAcon(DATACON{const=false,...})),pats)::more) =
		(con,(WILDpat::fields,rhs)::pats) :: addto more
	  | addto ((con,pats)::more) =
		(con,(fields,rhs)::pats) :: addto more
	  | addto [] = []
	in  (addto cases,(fields,rhs)::default)
	end
     | VARpat(VALvar{access=LVAR v,...}) => 
       andS ((WILDpat::fields,bind(x,v,rhs))::more)
     | LAYEREDpat(v,p) => andS((p::fields,layer(x,v,rhs))::more)
     | CONSTRAINTpat(p,_) => andS((p::fields,rhs)::more)
     | _ => impossible "andS in mc")
  | andS _ = impossible "andS2 in mc"
in  andS
end

fun orSwitch x =
let fun diffPats samefn =
	let fun diff [] = []
	      | diff ((hd as (p,rhs))::more) =
		 case p of
		    WILDpat => [hd]
		  | VARpat(VALvar{access=LVAR v,...}) => [(WILDpat,bind(x,v,rhs))]
		  | LAYEREDpat(v,p) =>
			diff ((p,layer(x,v,rhs))::more)
		  | CONSTRAINTpat(p,_) =>
			diff ((p,rhs)::more)
		  | _ => (if samefn p then diff more else hd::diff more)
			  handle Match =>
	      		    impossible "orS.diff: type error in match compiler"
	in  diff
	end
fun orS [] =  impossible "orSwitch [] in mc"
  | orS (arg as (p,rhs)::more) =
     case p of
      INTpat i =>
	let val (cases,default) = orS (diffPats (fn INTpat j => i=j) arg)
	in  ((INTcon i,convert rhs)::cases,default)
	end
    | REALpat r =>
	let val (cases,default) = orS (diffPats (fn REALpat s => r=s) arg)
	in  ((REALcon r,convert rhs)::cases,default)
	end
    | STRINGpat s =>
	let val (cases,default) = orS (diffPats (fn STRINGpat t => s=t) arg)
	in  ((STRINGcon s,convert rhs)::cases,default)
	end
    | WILDpat => ([],SOME(convert rhs))
    | VARpat(VALvar{access=LVAR v,...}) => ([],SOME(convert(bind(x,v,rhs))))
    | CONSTRAINTpat(p,_) => orS ((p,rhs)::more)
    | LAYEREDpat(v,p) => orS ((p,layer(x,v,rhs))::more)
    | _ => impossible "orS in mc"
in  orS
end

fun mcand (arg as ([_],_)::_,[x]) =
	let val singlelist = fn ([pat],rhs) => (pat,rhs)
			      | _ => impossible "singlelist in match compiler"
	in  APP(mcor (map singlelist arg), VAR x)
	end
  | mcand (arg as (p::fields,rhs)::more,xl as x::xs) =
    let fun mconto (con as DATAcon(con1 as DATACON{const = false,...}),pats) =
	     let val new = mkLvar ()
	     in  (con,APP(FN(new,mcand (MCopt.opt (pats,new::xs))),DECON (con1,VAR x)))
	     end
	  | mconto (con as DATAcon(DATACON {const = true,...}),pats) =
	     (con,mcand (MCopt.opt (pats,xs)))
	  | mconto _ = impossible "mconto in mc"
    in
     case p of
       WILDpat => mcand([(fields,rhs)],xs)
     | VARpat(VALvar{access=LVAR v,...}) => mcand([(fields,bind(x,v,rhs))],xs)
     | LAYEREDpat(v,p) => mcand(((p::fields,layer(x,v,rhs))::more),xl)
     | CONSTRAINTpat(p,_) => mcand((p::fields,rhs)::more,xl)
     | APPpat(DATACON{sign = [_],...},_) =>
	let val newx = mkLvar()
	    val ([(DATAcon dcon,list)],_) = andSwitch x arg
	in  APP(FN(newx,mcand(MCopt.opt(list,newx::xs))),DECON(dcon,VAR x))
	end
     | APPpat(DATACON{sign,...},_) =>
	let val (cases,default) = andSwitch x arg
	in  SWITCH(VAR x,
		   map mconto cases,
		   if length cases = length sign then NONE
		       else SOME (mcand (MCopt.opt (default,xs))))
	end
     | CONpat(DATACON{sign=[_],...}) => mcand([(fields,rhs)],xs)
     | CONpat(DATACON{sign,...}) =>
	let val (cases,default) = andSwitch x arg
	in  SWITCH(VAR x,
		   map mconto cases,
		   if length cases = length sign then NONE
		       else SOME (mcand (MCopt.opt (default,xs))))
	end
     | RECORDpat{pats=ref [],...} => mcand([(fields,rhs)],xs)
     | RECORDpat{pats,...} =>
	let val newfields = map (fn _ => mkLvar()) (!pats)
	    val wild = map (fn _ => WILDpat) newfields
	    fun expand [] = []
	      | expand ((p::fields,rhs)::more) =
		 (case p of
		   RECORDpat{pats,...} => (!pats@fields,rhs) :: expand more
	         | LAYEREDpat(v,p) => expand ((p::fields,layer(x,v,rhs))::more)
	         | CONSTRAINTpat(p,_) => expand ((p::fields,rhs)::more)
	         | WILDpat => (wild@fields,rhs) :: expand more
	         | VARpat(VALvar{access=LVAR v,...}) =>
			    (wild@fields,bind(x,v,rhs)) :: expand more
		 |  _ => impossible "mcand.expand in mc")
	      | expand _ = impossible "mcand.expand2 in mc"
	    in  bindfields(x,newfields,mcand(MCopt.opt(expand arg,newfields@xs)))
	    end
     | _ => (* INTpat,REALpat,STRINGpat; possibly bad VARpats *)
	let val (cases,default) = andSwitch x arg
	in  SWITCH(VAR x,
		   map (fn (con,pats) => (con,mcand(MCopt.opt(pats,xs)))) cases,
		   SOME(mcand(MCopt.opt(default,xs))))
	end
    end
  | mcand _ = impossible "mcand in mc"

and conSwitch x =
let
fun conS [] = ([],NONE)
  | conS (arg as (p,rhs)::more) =
     case p of
       CONpat(dcon as DATACON{name=r1,...}) =>
	let fun diff [] = []
	      | diff ((hd as (p,rhs))::more) =
		 case p of
		  CONpat(DATACON{name=r2,...}) =>
			if Symbol.eq(r1,r2) then diff more
			else (hd::diff more)
		| APPpat (_,_) => hd::diff more
		| WILDpat => [hd]
		| VARpat _ => [hd]
		| CONSTRAINTpat(p,_) => diff ((p,rhs)::more)
		| LAYEREDpat(v,p) => diff ((p,layer(x,v,rhs))::more)
		| _ => impossible "conS.diff: type error in match compiler"
	    val (cases,default) = conS (diff more)
	in  ((DATAcon dcon,convert rhs)::cases,default)
	end
     | APPpat(dcon as DATACON{name=r1,...},_) =>
	let fun divide [] = ([],[])
	      | divide ((hd as (p,rhs))::more) =
		case p of
		  CONpat _ =>
		      let val (same,diff) = divide more
		      in  (same,hd::diff)
		      end
		| APPpat(DATACON{name=r2,...},p) =>
		      let val (same,diff) = divide more 
		      in  if Symbol.eq(r1,r2)
			  then ((p,rhs)::same,diff)
			  else (same,hd::diff)
		      end
		| WILDpat => ([hd],[hd])
		| VARpat(VALvar{access=LVAR v,...}) =>
			([(WILDpat,bind(x,v,rhs))],[hd])
		| CONSTRAINTpat(p,_) => divide ((p,rhs)::more)
		| LAYEREDpat(v,p) => divide ((p,layer(x,v,rhs))::more)
		| _ => impossible "conS.divide: type error in match compiler"
	    val con = DATAcon dcon
            val (same,diff) = divide arg
	    val lexp = mcor same	    (* Order imp. here:  side- *)
	    val (cases,default) = conS diff (* effects in redund. chk. *)
        in  ((con,APP(lexp,DECON(dcon,VAR x)))::cases,default)
        end
     | WILDpat => ([],SOME(convertDefault rhs))
     | VARpat(VALvar{access=LVAR v,...}) =>
			([],SOME(convertDefault(bind(x,v,rhs))))
     | LAYEREDpat(v,p) => conS ((p,layer(x,v,rhs))::more)
     | CONSTRAINTpat(p,_) => conS ((p,rhs)::more)
     | _ => impossible "conS: type error in match compiler"
in  conS
end

and mcor [] = impossible "mcor.[] in mc"
  | mcor (arg as (p,rhs)::more) =
      let val x = mkLvar()
      in  case p of
	      CONpat(DATACON{sign=[],...}) => (* exception *)
		let val (cases,default) = conSwitch x arg
		in  areNotRedundant();
		    FN(x,SWITCH(VAR x,cases,default))
		end
	    | APPpat (DATACON{sign=[],...},_) => (* exn *)
		let val (cases,default) = conSwitch x arg
		in  areNotRedundant();
		    FN(x,SWITCH(VAR x,cases,default))
		end
	    | CONpat(DATACON{sign=[_],...}) => FN(x, convert rhs)
	    | CONpat(DATACON{sign,...}) =>
		let val (cases,default) = conSwitch x arg
		in  FN(x,SWITCH(VAR x, cases,
		        (if length cases = length sign
			   then (areRedundant(); NONE)
			   else (areNotRedundant(); default))))
		end
	    | APPpat(DATACON{sign=[_],...},_) =>
		let val ([(con,lexp)],_) = conSwitch x arg
		in  areRedundant();
		    FN(x,lexp)
		end
	    | APPpat(DATACON{sign,...},_) =>
		let val (cases,default) = conSwitch x arg
		in  FN(x,SWITCH(VAR x, cases,
		       (if length cases = length sign
			then (areRedundant(); NONE)
			else (areNotRedundant(); default))))
		end
	    | INTpat _ =>
		let val (cases,default) = orSwitch x arg
		in  FN(x,SWITCH(VAR x,cases,default))
		end
	    | REALpat _ =>
		let val (cases,default) = orSwitch x arg
		in  FN(x,SWITCH(VAR x,cases,default))
		end
	    | STRINGpat _ =>
		let val (cases,default) = orSwitch x arg
		in  FN(x,SWITCH(VAR x,cases,default))
		end
	    | RECORDpat{pats=ref [],...} => FN(x, convert rhs)
	    | RECORDpat{pats,...} =>
		let val newfields = map (fn _ => mkLvar()) (!pats)
		    val wild = map (fn _ => WILDpat) newfields
		    fun expand [] = []
		      | expand ((p,rhs)::more) =
			 case p of
			  RECORDpat{pats,...} => (!pats,rhs) :: expand more
			| LAYEREDpat(v,p) => expand ((p,layer(x,v,rhs))::more)
			| CONSTRAINTpat(p,_) => expand ((p,rhs)::more)
			| WILDpat => [(wild,rhs)]
			| VARpat(VALvar{access=LVAR v,...}) =>
			      [(wild,bind(x,v,rhs))]
			| _ => impossible "mcor.expand in mc"
		in  FN(x,bindfields(x,newfields,mcand(MCopt.opt(expand arg,newfields))))
		end
	    | WILDpat => FN(x, convert rhs)
	    | VARpat(VALvar{access=LVAR v,...}) => FN(x,convert(bind(x,v,rhs)))
	    | LAYEREDpat(v,p) => FN(x,APP(mcor((p,layer(x,v,rhs))::more),VAR x))
	    | CONSTRAINTpat(p,_) => mcor((p,rhs)::more)
	    | _ => impossible "mcor: type error in match compiler"
      end (* fun mcor *)

open PrintUtil
fun matchPrint [] _ _ = ()
  | matchPrint [(pat,_)] _ _ = () (* never print last rule *)
  | matchPrint ((pat,_)::more) [] _ =
	(print "        "; PrintAbsyn.printPat(pat,!printDepth); print " => ...\n";
	 matchPrint more [] 0)
  | matchPrint ((pat,_)::more) (taglist as (tag::tags)) i =
	if i = tag
	then (print "  -->   "; PrintAbsyn.printPat(pat,!printDepth);
	      print " => ...\n"; matchPrint more tags (i+1))
	else (print "        "; PrintAbsyn.printPat(pat,!printDepth);
	      print " => ...\n"; matchPrint more taglist (i+1))

fun bindPrint ((pat,_)::_) =
      (print "        "; PrintAbsyn.printPat(pat,!printDepth); print " = ...\n")
  | bindPrint _ = impossible "bindPrint in mc"

fun noVarsIn ((pat,_)::_) =
	let fun var WILDpat = true (* might want to flag this *)
	      | var (VARpat _) = true
	      | var (LAYEREDpat _) = true
	      | var (CONSTRAINTpat(p,_)) = var p
	      | var (APPpat(_,p)) = var p
	      | var (RECORDpat{pats=ref patlist,...}) = exists var patlist
	      | var _ = false
	in  not(var pat)
	end
  | noVarsIn _ = impossible "noVarsIn in mc"

open System.Control.MC

fun genCompile(flag1,warning1,flag2,test,warning2,printer) rules = 
let val (header,rules') = setup rules
    val match = header(mcor rules')
    val unused = redundant rules
    val last = length rules - 1
    val printit = if !flag1 andalso not(exists (fn i => i=last) unused)
		  then (warn(warning1 ^ " not exhaustive"); true)
		  else false
    val printit = if !flag2 andalso test(rules,unused,last)
		  then (warn warning2; true)
		  else printit
in  if !printArgs
	then (warn "MC called with:"; MCprint.printMatch rules)
	else ();
    if printit
	then printer(rules,unused)
	else ();
    if !printRet
	then (print "MC:  returns with\n"; MCprint.printLexp match; newline())
	else ();
    match
end handle Syntax => (warn "MC called with:"; MCprint.printMatch rules;
		      raise Syntax)

val bindCompile = 
    genCompile(bindExhaustive,
    	       "binding",
	       bindContainsVar,
	       fn (rules,unused,last) => noVarsIn rules,
	       "binding contains no variables",
	       fn(rules,unused) => bindPrint rules)

val matchCompile = 
    genCompile(matchExhaustive,
    	       "match",
	       matchRedundant,
	       fn(rules,unused,last) => exists (fn i => i<last) unused,
	       "redundant patterns in match",
	       fn(rules,unused) => matchPrint rules unused 0)

end (* struct MC *)
