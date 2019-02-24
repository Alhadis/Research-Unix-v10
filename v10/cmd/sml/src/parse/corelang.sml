(* Copyright 1989 by AT&T Bell Laboratories *)
signature CORELANG =
sig type appat
    type 'a precStack
    type stampsets
    type 'a stamped
    type 'a pathstamped
    type 'a susp
    type exp type pat type fixity type label type symbol
    type ty type vb type dec type rvb type tyvar type tycon
    type eb type rule type structureVar type tb 
    val exp_start:	exp * fixity -> exp precStack
    val exp_parse:	exp precStack * exp * fixity -> exp precStack
    val exp_finish:	exp precStack -> exp
    val pat_start:	pat * fixity -> appat precStack
    val pat_parse:	appat precStack * pat * fixity -> appat precStack
    val pat_finish:	appat precStack -> pat
    val pat_start_id:	symbol -> appat precStack susp
    val pat_parse_id:	appat precStack susp * symbol -> appat precStack susp
    val pat_id:		symbol -> pat susp
    val make_recordTy:  (symbol * ty) list -> ty
    val makeRECORDexp:  (symbol * exp) list -> exp
    val layered: 	pat * pat -> pat
    val makeRECORDpat:	(label * pat) list * bool -> pat
    val qid_pat:	symbol list -> pat
    val valbind:	pat * exp * tyvar list -> vb
    val makeEB:		symbol -> eb list susp
    val makeEBof:	symbol * ty susp -> eb list susp
    val makeEBeq:	symbol * symbol list -> eb list susp
    val getSTRpath:	symbol list -> structureVar
    val makeVALdec:	vb list stamped -> dec pathstamped
    val makeSEQdec:	dec pathstamped * dec pathstamped -> dec pathstamped
    val makeLOCALdec:	dec pathstamped * dec pathstamped -> dec pathstamped
    val makeOPENdec:	structureVar list susp -> dec pathstamped
    val makeRULE:	pat susp * exp stamped -> rule stamped
    type rawrvb
    val makeVALRECdec:	rawrvb list susp -> dec pathstamped
    type rawclause
    val checkFB:	rawclause list -> rawclause list
    val makeCLAUSE:	appat precStack susp * ty option susp * exp stamped ->
				rawclause susp
    val makeFUNdec:	rawclause list list susp -> dec pathstamped
    val makeTB:		tyvar list * symbol * ty susp -> 
				bool -> tb list pathstamped
    val makeDB:		tycon ref list pathstamped * (bool -> tb list pathstamped) ->
				dec pathstamped
    val makeDB':	tyvar list * symbol * (ty -> (symbol * bool * ty) list)
				-> tycon ref pathstamped
    val makeABSTYPEdec: tycon ref list pathstamped * dec pathstamped ->
				dec pathstamped
    val makeOVERLOADdec: symbol * ty susp * exp list stamped -> dec pathstamped
    val makeFIXdec:	fixity * symbol list -> dec pathstamped
    val toplevelexp:	exp stamped -> dec
    val lookArTYCp:	symbol list * int -> tycon ref
end

structure CoreLang: CORELANG = struct    
open ErrorMsg Symbol PrintUtil 
open Access Basics BasicTypes TypesUtil Absyn
open Env
open EnvAccess
open Misc

type stampsets = Stampset.stampsets
type 'a stamped = stampsets -> 'a
type 'a pathstamped = symbol list * stampsets -> 'a
type 'a susp = unit -> 'a

datatype appat = MONOap of pat | APPap of appat * pat

abstype 'a precStack = INf of int * 'a * 'a precStack
		   | NONf of 'a * 'a precStack
		   | NILf
with fun precedence (app,pair) =
 let fun ensure_nonfix (e,NONfix) = e
       | ensure_nonfix (e,INfix _) = (complain "expected nonfix identifier"; e)

     fun start(e,f) = NONf(ensure_nonfix(e,f), NILf)
	  
     fun parse(NONf(e,r), e',NONfix) = NONf(app(e,e'),r)
       | parse(p as INf _, x,f) = NONf(ensure_nonfix(x,f), p)
       | parse(p as NILf, _,_) = impossible "parse NILf"
       | parse(p as NONf(e1,INf(bp,e2,NONf(e3,r))), e4, f as INfix(lbp,rbp)) =
	    if lbp > bp then INf(rbp,e4,p)
	     else parse(NONf(app(e2,pair(e3,e1)),r),e4,f)
       | parse(p as NONf _, e',INfix(lbp,rbp)) = INf(rbp,e',p)
     
     fun finish (NONf(e1,INf(_,e2,NONf(e3,r)))) = 
		     finish(NONf(app(e2,pair(e3,e1)),r))
       | finish (NONf(e1,NILf)) = e1
       | finish (INf(_,e1,NONf(e2,p))) = 
		     (complain "expected nonfix identifier";
		      finish(NONf(app(e2,e1),p)))
       | finish (NILf) = impossible "finish NILf"
  in {start=start,parse=parse,finish=finish}
 end
end

val {start=exp_start, parse=exp_parse, finish=exp_finish} = 
	precedence(APPexp, fn (a,b) => TUPLEexp[a,b])

fun APPpat' (CONpat dcon,pat) = APPpat(dcon,pat)

fun clean_ap(MONOap(CONpat(DATACON{const=false,name,...}))) = 
	(complain ("data constructor "^Symbol.name name^
		" used without argument in pattern"); WILDpat)
  | clean_ap(MONOap p) = p
  | clean_ap(APPap(ap,p)) =
	case clean_ap ap
	 of CONpat(d as DATACON{const=false,...}) => APPpat(d,p)
          | CONpat(d as DATACON{name,...}) =>
		(complain("improper application of constructor "^Symbol.name name
		  ^" in pattern"); WILDpat)
	  |  _ =>(complain"improper application of non-constructor in pattern";
				WILDpat)

val {start=pat_start0, parse=pat_parse0, finish=pat_finish0} =
	precedence(fn (ap1,ap2) => APPap(ap1, clean_ap ap2),
		   fn (ap1,ap2) =>MONOap(TUPLEpat[clean_ap ap1, clean_ap ap2]))

fun pat_start(p,f) = pat_start0(MONOap p, f)
fun pat_parse(ap,p,f) = pat_parse0(ap, MONOap p, f)
val pat_finish = clean_ap o pat_finish0

fun pat_id id () = CONpat(lookCON id) 
		   handle Unbound => VARpat(mkVALvar(id,ref UNDEFty))

fun pat_start_id ID () = pat_start(pat_id ID (), lookFIX ID)
fun pat_parse_id (app_pat,ID) () = pat_parse(app_pat (), pat_id ID (), lookFIX ID)


fun bindVARp patlist =
 let val vl = ref (nil: symbol list)
     val rec f =
           fn VARpat(v as VALvar{name=[name],...})=> 
		      if exists(fn n => n=name) (!vl)
			 then complain "duplicate variable-name in pattern(s)"
			 else (bindVAR(name,v); vl := name :: !vl)
	    | RECORDpat{fields,...} => app(fn(_,pat)=>f pat) fields
	    | APPpat(_,pat) => f pat
	    | CONSTRAINTpat(pat,_) => f pat
	    | LAYEREDpat(p1,p2) => (f p1; f p2)
	    | _ => ()
  in app f patlist
 end

local fun uniq ((a0 as (a,_))::(r as (b,_)::_)) = 
	    if Symbol.eq(a,b) then uniq r else a0::uniq r
        | uniq l = l
      fun gtr((a,_),(b,_)) = 
		     let val a' = Symbol.name a and b' = Symbol.name b
		         val zero = ord "0" and nine = ord "9"
			 val a0 = ordof(a',0) and b0 = ordof(b',0)
		      in if a0 >= zero andalso a0 <= nine
			  then if b0 >= zero andalso b0 <= nine
				 then size a' > size b' orelse
					  size a' = size b' andalso a' > b'
				 else false
			  else if b0 >= zero andalso b0 <= nine
				then true
				else a' > b'
		     end
      val sort = Sort.sort gtr
 in fun sortRecord l =
	let val l' =  uniq (sort l)
	 in if length l' < length l
		then complain "duplicate label in record" else ()
	    l'
	end
end

fun make_recordTy l = recordTy(sortRecord l)

fun makeRECORDexp fields =
  let val fields' = map (fn(id,exp)=> (id,(exp,ref 0))) fields
      fun assign(i,(_,(_,r))::tl) = (r := i; assign(i+1,tl))
	| assign(_,nil) = ()
      fun f(i,(id,(exp,ref n))::r) = (LABEL{name=id,number=n},exp)::f(i+1,r)
        | f(_,nil) = nil
   in assign(0, sortRecord fields'); RECORDexp(f(0,fields'))
  end

fun layered(x as (VARpat _, _)) = LAYEREDpat x
  | layered(y,z) = (complain "pattern to left of AS must be variable"; z)

fun makeRECORDpat(l,flex) =
	  RECORDpat{fields= sortRecord l, 
		    flex=flex, typ=ref UNDEFty, pats=ref nil}

fun qid_pat qid =
  case lookPath(qid,lookCONinStr)
   of dcon as DATACON{const=true,...} => CONpat dcon
    | DATACON{name,...} => (complain("constructor "^Symbol.name name^
					" requires argument in pattern");
			    WILDpat)

fun valbind(CONSTRAINTpat(VARpat(VALvar{name,typ,...}), ty),
	    exp as VARexp(ref(VALvar{access as INLINE _,...})), tvs) =
	  VB{pat=CONSTRAINTpat(VARpat(VALvar{name=name,typ=typ,access=access}),ty),
	     exp=exp,tyvars=tvs}
  | valbind(VARpat(VALvar{name, typ,...}),
	    exp as VARexp(ref(VALvar{access as INLINE _,...})), tvs) =
	 VB{pat=VARpat(VALvar{name=name,typ=typ,access=access}),exp=exp, tyvars=tvs}
  | valbind(pat,exp,tvs) = VB{pat=pat,exp=exp,tyvars=tvs}

fun getEXN id = lookCON(id) handle Unbound => unboundEXN id

fun makeEB (id:symbol) () =
  let val exn = DATACON{name=id,const=true,typ=ref exnTy,
			rep=VARIABLE(LVAR(namedLvar id)), sign=[]}
   in bindCON(id,exn); [EBgen{exn=exn,etype=NONE}]
  end

fun makeEBof (id,etype) () =
  let val ty = etype()
      val exn = DATACON{name=id,const=false,typ=ref(ty --> exnTy),
			rep=VARIABLE(LVAR(namedLvar id)), sign=[]}
   in bindCON(id,exn); [EBgen{exn=exn,etype=SOME ty}]
  end

fun makeEBeq (id,qid) () =
  let val edef as DATACON{const,typ,rep,sign,...} =
	       case qid of [s] => getEXN s
			 | q => lookPath(q,lookEXNinStr)
      val exn = DATACON{name=id,const=const,typ=ref(!typ),sign=sign,
			rep=VARIABLE(LVAR(namedLvar id))}
   in bindCON(id,exn); [EBdef{exn=exn,edef=edef}]
  end

fun getSTR id = lookSTR id
		 handle Unbound => 
		    (complain("unbound structure name: " ^ name id);
		     bogusSTR)

fun getSTRpath [id] = getSTR id
  | getSTRpath q = lookPath(q,lookSTRinStr)

fun makeVALdec vb (path,st) =
   let val l = vb st
    in bindVARp (map (fn VB{pat,...}=>pat) l); VALdec l
   end

fun makeSEQdec (d1,d2) $ =
  case (d1 $, d2 $)
   of (SEQdec a, SEQdec b) => SEQdec(a@b)
    | (SEQdec a, b) => SEQdec(a@[b])
    | (a, SEQdec b) => SEQdec(a::b)
    | (a,b) => SEQdec[a,b]

fun makeLOCALdec (ldecs1,ldecs2) (path,st) =
  let val envLocal=openScope()
      val ld1 = ldecs1([],st)
      val envIn = (openScope(); current())
      val ld2 = ldecs2(path,st)
   in splice(envLocal,envIn);
      LOCALdec(ld1,ld2)
  end

fun makeOPENdec qid_p _ = 
   let val strs = qid_p()
    in app openStructureVar strs;
       OPENdec strs
   end

fun makeRULE(pat,exp) st =
  protect(protectScope, fn() => 
     let val p = pat() in bindVARp[p]; RULE(p, exp st) end)

type rawrvb = {name:symbol,ty:ty option,match:rule list stamped}

fun makeVALRECdec rvb (path,st) =
   protect(protectScope, fn()=>
    protect(protectTyvars NONE, fn()=>
      let val rvbs = rvb()
	  fun makevar (p as {name,...}:rawrvb) = 
		let val v = mkVALvar(name, ref UNDEFty)
		 in bindVAR(name,v); (v,p)
		end
	  val rvbs' = map makevar rvbs
	  fun evalmat(v, p as {match,...}:rawrvb) = (v,p,match st)
	  val rvbs'' = map evalmat rvbs'
	  val tyv = currentTyvars()
	  fun makervb(v,{name,ty,...}:rawrvb,match) =
		RVB{var=v,resultty=ty,exp=FNexp match,tyvars=tyv}
       in VALRECdec(map makervb rvbs'')
      end))

type rawclause = {name:symbol,pats:pat list,resultty:ty option,exp:exp stamped}

fun checkFB(clauses as ({name,pats,...}:rawclause)::rest) =
   let val len = length pats
    in if exists (fn {pats,...} => len <> length pats) rest
	 then complain "clauses don't all have same number of patterns"
	 else ();
       if exists (fn {name=n,...} => not(Symbol.eq(n,name))) rest
	 then complain "clauses don't all have same function-name"
	 else ();
       clauses
   end

fun makeCLAUSE(app_pat,constraint,exp) () =
  let fun f(pats, MONOap p) = (p,pats)
	| f(pats, APPap(ap,p)) = f(p::pats,ap)
   in case f(nil,pat_finish0(app_pat()))
       of (VARpat(var as VALvar{name=[id],...}), pats as _::_) =>
	    {name=id,pats=pats,resultty=constraint(),exp=exp}
	| (_, pats as _::_) => 
	   (complain"illegal function symbol in clause";
	    {name=bogusID,pats=pats,resultty=constraint(),exp=exp})
        | _ => (complain "no argument-patterns in clause";
	   {name=bogusID,pats=[WILDpat],resultty=constraint(),exp=exp})
  end

fun makeFUNdec fb (_,st) =
 protect(protectScope, fn()=>
  protect(protectTyvars NONE, fn()=>
    let val fbs = fb()
        fun makevar (p as ({name,...}:rawclause)::_) =
		let val v = mkVALvar(name,ref UNDEFty)
		 in bindVAR(name,v); (v,p)
		end
	val fbs' = map makevar fbs
        fun makeclause{name,pats,resultty,exp} =
	  protect(protectScope, fn()=>
	    (bindVARp pats;
	     CLAUSE{pats=pats,resultty=resultty,exp=exp st}))
	fun evalclauses(v,l) = (v,map makeclause l)
	val fbs'' = map evalclauses fbs'
	val tyv = currentTyvars()
	fun makefb (v,c) = FB{var=v,clauses=c,tyvars=tyv}
     in FUNdec(map makefb fbs'')
    end))

fun makeTB(args,name,ty) notwith (path,stamps) =
 let fun equalargs([],[]) = true
       | equalargs(tv::rest,VARty(tv')::rest') =
		  tv = tv' andalso equalargs(rest,rest')
       | equalargs _ = false
     val typ = protect(protectTyvars(SOME args), ty)
     val _ = TypesUtil.bindTyvars args;
     val binding =
        case typ
	 of CONty(tycref as ref(TYCON{stamp,arity,eq,path=path',kind}),args')=>
		if Stampset.tycFixed(stamp) andalso equalargs(args,args')
		  then case kind
		        of UNDEFtyc _ =>
			     (tycref := TYCON{stamp=stamp,arity=arity,eq=eq,
					      path=path',
					     kind=UNDEFtyc(SOME(name::path))};
			      tycref)
			 | _ => ref(TYCON{stamp=stamp,arity=arity,eq=eq,
				    	      path=name::path,kind=kind})
		  else ref(mkDEFtyc(name::path,
				    TYFUN{arity=length args, body=typ},
				    if notwith 
				        then if isEqType typ then YES else NO
				        else MAYBE,
				    stamps))
	 | _ => ref(mkDEFtyc(name::path,
			     TYFUN{arity=length args, body=typ},
			     if notwith 
				then if isEqType typ then YES else NO
				else MAYBE,
			     stamps))
  in bindTYC(name,binding);
     [TB{tyc=binding,def=typ}]
 end
       
fun makeDB(db,tb) (path,stamps) =
  let val (datatycs,withtycs) =
	  protect(protectDb(), fn()=>
		(db(path,stamps), tb false (path,stamps)))
      val checkeq = defineEqTycon (fn x => x)
   in app (fn (ref tyc) => checkeq tyc) datatycs;
      app (fn TB{tyc,...} => checkeq(!tyc)) withtycs;
      DATATYPEdec{datatycs=datatycs,withtycs=withtycs}
  end

fun makeDB'(args,name,constrs) (path,stamps) =
  let val arity = length args
      val rangeType = CONty(!lookArTYC(name,arity), map VARty args)
   in protect(protectTyvars(SOME args),  fn()=>
       let val dcl = constrs rangeType
	   val sdcl = sort3 dcl
	   val sign = ConRep.boxed(sdcl)
           fun binddcons ((sym,const,typ)::restdcl,rep::restsign) =
		let val dcon = DATACON{name=sym, const=const, 
				       rep=rep, sign=sign,
			       typ = if arity > 0
				     then ref(POLYty
					   {sign=mkPolySign arity,
					    tyfun=TYFUN{arity=arity,body=typ}})
				     else ref typ}
		 in bindCON(sym, dcon);
	 	    dcon :: binddcons(restdcl,restsign)
		end
	     | binddcons ([],[]) = []
	     | binddcons _ = impossible "Parse.db1.fn.binddcons"
	   val _ = if length sdcl < length dcl
	       	    then complain "duplicate constructor name" else ()
	   val _ = TypesUtil.bindTyvars args;
	   val tycref = ref(mkDATAtyc(name::path,arity,
				           binddcons(sdcl,sign),MAYBE,stamps))
	in bindTYC(name,tycref);
	   tycref
       end)
  end

fun makeABSTYPEdec(db,ldecs) (path,stamps) =
    let val mAbstype = openScope()
	val DATATYPEdec{datatycs,withtycs} = 
				makeDB(db,fn _=> fn _ => nil) (path,stamps)
	val withtycons = map (fn TB{tyc,...} => tyc) withtycs
	val _ = makeAbstract(datatycs,withtycons)
	val mWith = (openScope(); current())
	val body = ldecs(path,stamps)
	fun bind tyc = bindTYC(tycName(!tyc), tyc)
     in splice(mAbstype,mWith);
	app bind datatycs (* abstycs *);
	app bind withtycons;
	ABSTYPEdec{abstycs=datatycs,withtycs=withtycs,body=body}
    end

fun makeOVERLOADdec(id,ty,exps) (path,stamps) =
    let val scheme =
          protect(protectScope, fn () =>  (* localize tyvars *)
            protect(protectTyvars NONE, fn () =>
              let val body = ty()  (* generalize type variables *)
      	          val tvs = currentTyvars()
      	       in TypesUtil.bindTyvars tvs;
      	          TYFUN{arity=length(tvs),body=body}
      	      end))
        fun option (VARexp(ref (v as VALvar{typ,...}))) =
              {indicator = TypesUtil.matchScheme(scheme,!typ), variant = v}
     in bindVAR(id,OVLDvar{name=id,options=ref(map option (exps stamps)),
		scheme=scheme});
        SEQdec nil
    end

fun makeFIXdec(fixity,ops) _ =
 (app (fn ident => bindFIX(ident,FIXvar{name=ident,binding=fixity})) ops;
  SEQdec nil)

val itsym = Symbol.symbol "it"

fun toplevelexp exp =
 let val dec = 
   protect(protectTyvars NONE, fn()=>
       VALdec[VB {exp = exp Stampset.globalStamps,
	          pat = let val v = newVAR(ref nil, itsym)
			 in bindVAR(itsym,v); VARpat v
			end,
		  tyvars = currentTyvars()}])
  in Typecheck.decType dec; dec
 end

fun lookArTYCp([id],i) = !lookArTYC (id,i)
  | lookArTYCp(name,i) = !lookPathArTYC(name,i)

end
