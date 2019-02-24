(* Copyright 1989 by AT&T Bell Laboratories *)
structure EnvAccess : ENVACCESS = struct
(* lookup and binding functions *)

structure Access = Access
structure Basics = Basics
structure Env = Env

open ErrorMsg PrintUtil Access Basics Basics.Symbol BasicTypes TypesUtil Env
     NameSpace

val debugBind = System.Control.debugBind

fun openStructureVar(STRvar{access=PATH p,binding,...}) =
    (case binding
      of STRstr{table,env,...} => openOld({path=p,strenv=env},table)
       | INDstr _ => impossible "EnvAccess.openStructureVar -- INDstr arg"
       | SHRstr _ => impossible "EnvAccess.openStructureVar -- SHRstr arg"
       | NULLstr => impossible "EnvAccess.openStructureVar -- NULLstr arg")
  | openStructureVar _ = impossible "EnvAccess.openStructureVar -- bad access value"

val bogusID = Symbol.symbol "bogus"

val bogusStrStamp = Stampset.newStamp(Stampset.fixedStrStamps)

local val b = STRstr{stamp=bogusStrStamp, sign=0, table=newTable(), env=DIR,
		     kind=STRkind{path=[bogusID]}}
 in val bogusSTR = STRvar{name=[bogusID], access=PATH[0], binding=b}
    val bogusSTR' = STRvar{name=[bogusID], access=SLOT 0, binding=b}
end

(* type constructors *)

val bogusTyc = mkDEFtyc([bogusID],TYFUN{arity=0,body=ERRORty},YES,Stampset.globalStamps)

fun lookTYCinTable(table,id) =
    let val TYCbind tycref = IntStrMap.map table (tycKey id)
     in tycref
    end

fun lookTYCinStr(STRstr{table,env,stamp,...}: Structure, id: symbol) : tycon ref =
    ((case lookTYCinTable(table,id)
	of ref(INDtyc i) =>
	     (case env
	       of REL{s,t} => ref(t sub i)
	        | DIR => impossible "EnvAccess.lookTYCinStr 1")
	 | ref(SHRtyc p) => ref(getEpathTyc(p,env))
	 | tyc => tyc)
     handle UnboundTable => 
	(if stamp=bogusStrStamp then ()
	 else complain("unbound type in structure: " ^ Symbol.name id);
	 ref bogusTyc))
  | lookTYCinStr _ = impossible "EnvAccess.lookTYCinStr 2"

fun lookTYC' look (id:symbol) =
    case look(tycKey(id))
      of (TYCbind(tycref as ref(INDtyc i)), {strenv=REL{s,t},path}) =>
	    (ref(t sub i) 
	     handle Subscript => 
	       impossible "EnvAccess.lookTYC' 1")
       | (TYCbind(tycref as ref(SHRtyc p)), {strenv,path}) =>
	    (ref(getEpathTyc(p,strenv)) 
	     handle Subscript => 
	       impossible "EnvAccess.lookTYC' 2")
       | (TYCbind tycref, _) => tycref
       | _ => impossible "EnvAccess.lookTYC' 3"

val lookTYC = lookTYC' look
val lookTYClocal = lookTYC' lookStrLocal

(* addzeros also defined in Parse *)
fun addzeros(0,l) = l
  | addzeros(n,l) = addzeros(n-1,0::l)

fun bindTYC(id: symbol, tc: tycon ref) =
    let val binding = TYCbind tc 
     in add(tycIndex id, name id, binding); binding
    end


(* tycon lookup with arity checking *)

fun checkArity(tycon, arity) =
    if tyconArity(tycon) <> arity
    then complain("type constructor "^(Symbol.name(tycName(tycon)))^
	          " has wrong number of arguments: "^makestring arity)
    else ()

fun lookArTYC0(id,arity) =
    let val tycref as ref tyc = lookTYC id
     in checkArity(tyc,arity);
        tycref
    end
    handle Unbound => 
      (complain("unbound type constructor: " ^ Symbol.name id);
       ref bogusTyc)

fun lookArTYCinSig (depth: int) (id: symbol, arity: int) =
    (case look(tycKey id)
      of (TYCbind(tycref as ref(INDtyc i)), {strenv=REL{s,t},path=h::r}) =>
	   if h >= 0
	   then let val tyc = t sub i
		 in checkArity(tyc,arity);
		    ref tyc
		end
	   else (checkArity(t sub i, arity);
		 ref(RELtyc(addzeros(depth+h,r@[i]))))
       | (TYCbind(tycref as ref(SHRtyc p)), {strenv,path}) =>
	   let val tyc = getEpathTyc(p,strenv)
	    in checkArity(tyc,arity);
	       ref tyc
	   end
       | (TYCbind tycref, _) => (checkArity(!tycref,arity); tycref)
       | _ => impossible "EnvAccess.lookTYCinSig")
    handle Unbound => 
      (complain("unbound type constructor in signature: " ^ Symbol.name id);
       ref bogusTyc)

val lookArTYC : (symbol * int -> tycon ref) ref = ref lookArTYC0


(* patching type constructor references in datatype declarations *)

fun protectDb () =
    let val patchList : tycon ref list ref = ref []
	val savedLook = !lookArTYC
	fun localLook(id,ary) =
	    let val tycref = ref (mkUNDEFtyc(id,ary))
	     in patchList := tycref :: !patchList;
		tycref
	    end
	fun patch (tc::l) =
	    let val ref(TYCON{path=id::_,arity,kind=UNDEFtyc newpath,...}) = tc
	     in let val ref tycon = !lookArTYC(id,arity)
	         in tc := case newpath
			   of NONE => tycon
			    | SOME path => setTycPath(path,tycon)
	        end
		handle Unbound =>
		  complain("unbound type constructor (in datatype): " ^
		           Symbol.name id);
	        patch l
	    end
	  | patch nil = ()
     in ((fn () => lookArTYC := localLook),
         (fn () => (lookArTYC := savedLook; patch(!patchList))))
    end

(* constructors *)

fun dconApplied(DATACON{name,const,typ,rep,sign},{path,strenv}:info) : datacon =
    DATACON{name = name, const = const, sign=sign,
            rep = (case rep
		     of VARIABLE(SLOT n) => VARIABLE(PATH(n::path))
		      | VARIABLE(LVAR v) => VARIABLE(PATH [v])
		      | _ => rep),  (* nonexception datacon *)
            typ = ref(typeInContext(!typ,strenv))}

fun lookCONinTable(table,id) = 
    case IntStrMap.map table (varKey(id))
      of CONbind c => c
       | _ => raise UnboundTable

fun lookCON' lookfn id =
    case lookfn(varKey(id))
      of (CONbind c,info) => dconApplied(c,info)
       | _ => raise Unbound

val lookCON = lookCON' look
val lookCONlocal = lookCON' lookStrLocal

val bogusCON = DATACON{name=bogusID,const=true,typ=ref ERRORty,
		       rep=UNDECIDED,sign=[]}

fun lookCONinStr(STRstr{table,env,stamp,...},id,ap,qid): datacon =
    (dconApplied(lookCONinTable(table,id),{path=ap,strenv=env})
     handle UnboundTable => 
	(if stamp=bogusStrStamp then ()
	 else complain("unbound constructor in structure: " ^ Symbol.name id);
	 bogusCON))
  | lookCONinStr _ = impossible "EnvAccess.lookCONinStr"

fun bindCON (id: symbol, c: datacon) = 
    let val binding = CONbind c 
     in add(conIndex id, name id, binding); binding
    end

(* variables *)

fun unboundVAR id = (complain ("unbound variable " ^ name id);
		     VARbind(mkVALvar(id, ref(VARty(mkTyvar defaultMETA)))))

fun varApplied(v: var, {path, strenv}: info, qid) : var =
    case v
      of VALvar{access,name,typ} =>
	   VALvar{access =
		    (case access
		       of SLOT(n) => PATH(n::path)
			| LVAR(n) => PATH([n])
			| INLINE _ => access
			| PATH _ => impossible "varApplied: access = PATH"),
		  typ = 
		    if Prim.special(access)
		    then ref(!typ)
		    else (case path
			   of [] => typ
			    | _ => ref(typeInContext(!typ,strenv))),
		  name = qid}
       | _ => v

fun lookVARinTable(table, id) =
    case IntStrMap.map table (varKey id)
      of VARbind v => v
       | _ => raise UnboundTable

fun lookVARCONinTable(table,id) = IntStrMap.map table (varKey id)

fun lookVARCONinStr(STRstr{table,env,stamp,...},id,ap,qid): binding =
    ((case lookVARCONinTable(table,id)
       of VARbind(var) => VARbind(varApplied(var,{path=ap,strenv=env},qid))
	| CONbind(dcon) => CONbind(dconApplied(dcon,{path=ap,strenv=env}))
	| _ => impossible "EnvAccess.lookVARCONinStr 1")
     handle UnboundTable =>
	(if stamp=bogusStrStamp then ()
	 else complain("unbound variable or constructor in structure: "
		       ^ Symbol.name id);
	 CONbind bogusCON))
  | lookVARCONinStr(NULLstr,id,_,_) =
      (printSym id; print "\n"; impossible "EnvAccess.lookVARCONinStr 2")
  | lookVARCONinStr(_,id,_,_) =
      (printSym id; print "\n"; impossible "EnvAccess.lookVARCONinStr 3")

fun lookVARCON id = 
    case lookRec(varKey id)
      of LOCAL(VARbind v, info) => VARbind(varApplied(v,info,[id]))
       | LOCAL(CONbind d, info) => CONbind(dconApplied(d,info))
       | GLOBAL(CONbind d, info) => CONbind(dconApplied(d,info))
       | GLOBAL(VARbind _, _) => raise Unboundrec
       | _ => impossible "EnvAccess.lookVARCON"

fun lookVARCONlocal id = 
    case lookStrLocal(varKey id)
      of (VARbind v, info) => VARbind(varApplied(v,info,[id]))
       | (CONbind d, info) => CONbind(dconApplied(d,info))
       | _ => impossible "EnvAccess.lookVARCON"

fun lookVARRecLocal id = 
    case lookRecLocal(varKey id)
      of (VARbind v, info) => varApplied(v,info,[id])
       | _ => impossible "EnvAccess.lookVARRecLocal"

(* patching deferred variables *)

val varPatchList : var ref list ref = ref nil

fun getPatchVar id =
    let val v = ref (UNKNOWNvar id)
     in varPatchList := v :: !varPatchList;
	v
    end

exception Patched

fun patchVars (pl as (varRef as ref(UNKNOWNvar id))::pl', tl) =
     ((varRef := lookVARRecLocal id; raise Patched)
       handle Unboundrec => 
		 patchVars(pl',varRef::tl)  (* not yet bound; try later *)
	    | Unbound => (* no more rec layers *)
		let val VARbind v = unboundVAR id
		 in varRef := v; patchVars(pl',tl)
		end
	    | Patched => patchVars(pl', tl))
  | patchVars (nil, tl) = tl
  | patchVars _ = impossible "EnvAccess.patchVars"

val protectPatchList =
    ((fn () => !varPatchList before (varPatchList := nil)),
     (fn (vpl) => varPatchList := patchVars(!varPatchList,vpl)))
	 (* bug -- exit function only works right for normal exit from protect *)

fun capitalized string =
    (* string starts with a capital letter *)
    let val firstchar = ordof(string,0)
     in firstchar >= Ascii.uc_a andalso firstchar <= Ascii.uc_z
    end

(* Could be used to enforce the Capitalization convention, but isn't *)
fun checkBinding(id: symbol,_) =
    if capitalized(Symbol.name id)
    then warn("Capitalized variable in rule: "^ Symbol.name id)
    else ()


fun newVAR(bl: (symbol * var) list ref, id: symbol) : var =
    let fun checkid ((i,b)::bl) =
	      if Symbol.eq(i,id)
	        then complain "repeated var in pattern"
	        else checkid bl
	  | checkid nil = ()
     in checkid(!bl);
        let val v = mkVALvar(id,ref UNDEFty)
	 in bl := (id, v) :: !bl;
	    v
        end
    end

fun bindVAR(id: symbol, v: var) = 
	let val binding = VARbind v
	 in add(varIndex id, name id, binding); binding
	end

fun bindVARs(binders: (symbol * var) list) =
    app (fn b as (id,bind) =>
	    (if !debugBind
	     then (print "bindVARs: "; printSym id; newline())
	     else ();
	     bindVAR b))
	binders


(* type variables *)

datatype mode = EXP | TYPEDEC

val tyvarsMode = ref(EXP)
val boundTyvars = ref([]:tyvar list)

fun protectTyvars NONE = 
    ((fn () => (!boundTyvars before (boundTyvars := []))),
     (fn btv => boundTyvars := btv))
  | protectTyvars (SOME tvs) = 
    ((fn () => (!boundTyvars before (boundTyvars := tvs; tyvarsMode := TYPEDEC))),
     (fn btv => (boundTyvars := btv; tyvarsMode := EXP)))

fun currentTyvars () = !boundTyvars

fun lookTYV id = 
    let val (TYVbind tyv, _) = lookStrLocal(tyvKey id) in tyv end

fun lookTyvar (id: symbol) =
    case !tyvarsMode
      of TYPEDEC =>
	   let fun find ((tv as ref(UBOUND{name=id',...}))::resttvs) =
		   if Symbol.eq(id,id')
		      then tv
		      else find(resttvs)
		 | find([]) =
		    (complain "lookTyvar -- unbound tyvar in closed scope";
		     mkTyvar(INSTANTIATED UNDEFty))
		 | find _ = impossible "EnvAccess.lookTyvar.find"
	    in find(!boundTyvars)
	   end
       | EXP =>
	   lookTYV id
	   handle Unbound =>  (* here we could check for weakness > 0 *)
	     let val tyv = mkTyvar(mkUBOUND id)
	      in add(tyvIndex id, name id, TYVbind tyv);
		 boundTyvars := tyv :: !boundTyvars;
		 tyv
	     end;


(* exceptions *)

fun notInitialLowerCase string =
    (* string does NOT start with lower-case alpha *)
    let val firstchar = ordof(string,0)
     in firstchar < Ascii.lc_a orelse firstchar > Ascii.lc_z
    end

(* Could be used to enforce the Capitalization convention *)
fun looksLikeExn sym = notInitialLowerCase(Symbol.name sym)

fun unboundEXN id =
    (complain("unbound exn: " ^ name id); bogusCON)

fun lookEXNinStr(STRstr{table,env,stamp,...},id,ap,qid) =
    (dconApplied(lookCONinTable(table,id),{path=ap,strenv=env})
     handle UnboundTable => 
	(if stamp=bogusStrStamp then ()
	 else complain("unbound exception in path: " ^ Symbol.name id);
	 bogusCON))
  | lookEXNinStr _ = impossible "EnvAccess.lookEXNinStr"


(* signatures *)

val bogusSIGStampsets = Stampset.newStampsets()
val bogusSIGbody = 
    STRstr{stamp=Stampset.newStamp(#strStamps bogusSIGStampsets),
           sign=Stampset.newStamp(Stampset.sigStamps),
           table=newTable(),
	   env=DIR,
	   kind=SIGkind{share={s=nil,t=nil},
		        bindings=nil,stamps=bogusSIGStampsets}}
val bogusSIG=SIGvar{name=bogusID,binding=bogusSIGbody}

fun lookSIG id = 
    let val (SIGbind sign,_) = look(sigKey id)
     in sign
    end
    handle Unbound => (complain("unbound signature: " ^ name id); bogusSIG)

fun bindSIG(id: symbol, s: signatureVar) = add(sigIndex id, name id, SIGbind s)


(* structures *)

fun strApplied(STRvar{name,access,binding},{path=ap,strenv},qid) =
    STRvar{name=qid,
	   binding=(case (binding,strenv)
		     of (INDstr i,REL{s,...}) => s sub i
		      | (SHRstr(i::r),REL{s,...}) => getEpath(r,s sub i)
		      | (STRstr _, _) => binding
		      | _ => impossible "strApplied: bad binding/env"),
	   access=(case access
		     of SLOT(n) => PATH(n::ap)
		      | LVAR(n) => PATH [n]
		      | _ => impossible "strApplied: access = PATH or INLINE")}

fun lookSTRinTable(table, id) = 
    let val STRbind strvar = IntStrMap.map table (strKey id) in strvar end

fun lookSTR0 id = 
    let val (STRbind str, info) = look(strKey id)
     in (str,info)
    end

fun lookSTR' look id =
    let val (STRbind str, info) = look(strKey id)
     in strApplied(str,info,[id])
    end
val lookSTR = lookSTR' look
val lookSTRlocal = lookSTR' lookStrLocal

fun lookSTRinStr(STRstr{table,env,stamp,...},id,ap,qid) =
    (strApplied(lookSTRinTable(table,id),{path=ap,strenv=env},qid)
     handle UnboundTable => 
	(if stamp=bogusStrStamp then ()
	 else complain("unbound structure in path: " ^ Symbol.name id);
	 bogusSTR))
  | lookSTRinStr _ = impossible "EnvAccess.lookSTRinStr"

fun bindSTR(id: symbol, strvar: structureVar) =
   let val binding = STRbind strvar
    in add(strIndex id, name id, binding);
       binding
   end


(* functors *)

val bogusFCT = FCTvar{name=bogusID, access=PATH[0],
		     binding=FUNCTOR{paramName=bogusID,
				     param=bogusSIGbody,
				     body=bogusSIGbody,
				     paramVis=false,
				     stamps=Stampset.newStampsets()}}

fun lookFCT id = 
    let val (FCTbind fv,_) = look(fctKey id) in fv end 
    handle Unbound =>
      (complain("unbound functor identifier: " ^ Symbol.name id);
	bogusFCT)

fun bindFCT(id: symbol, f: functorVar) = add(fctIndex id, name id, FCTbind f)

(* fixity bindings *)

fun lookFIX id = 
    if true (* !(Symbol.infixed id) *)
    then let val (FIXbind(FIXvar{binding,...}),_) = look(fixKey id)
	  in binding
	 end
	 handle Unbound => ((* Symbol.infixed id := false; *) NONfix)
    else NONfix

fun bindFIX(id: symbol, f: fixityVar) = 
   let val binding = FIXbind f
    in add(fixIndex id, name id, binding); binding
   end

(* lookup using symbolic path *)
fun lookPathinStr
      (str: Structure, ap: Access.path, spath as _::rest : symbol list,
       lookLast: Structure * symbol * Access.path * symbol list -> 'a) : 'a =
    let fun getStr([id],str,ap) = lookLast(str,id,ap,spath)
	  | getStr(id::rest,STRstr{table,stamp,env,...},ap) =
	      let val STRvar{access=SLOT n,binding,...} = 
		      lookSTRinTable(table,id)
		      handle UnboundTable => 
			(if stamp=bogusStrStamp then ()
		         else (complain("unbound intermediate structure: "
				        ^ name id);
		               print "  in path: ";
			       printSequence "." printSym spath;
		               newline());
		         bogusSTR')
	       in getStr(rest,
		  	 (case binding
			   of INDstr i => 
			      (case env
			        of REL{s,...} => s sub i
			         | DIR => impossible "lookPathinStr.getStr 1")
			    | SHRstr(i::r) => 
			      (case env
			        of REL{s,...} => getEpath(r,s sub i)
			         | DIR => impossible "lookPathinStr.getStr 2")
			    | _ => binding),
			 n::ap)
	      end
	  | getStr _ = impossible "EnvAccess.lookPathinStr.getStr"
     in getStr(rest,str,ap)
    end
  | lookPathinStr _ = impossible "EnvAccess.lookPathinStr"

fun lookPath(spath as first::rest,
             lookLast: Structure * symbol * Access.path * symbol list -> 'a) : 'a =
    let	val STRvar{access=PATH(ap),binding,...} =
	      lookSTR first
	      handle Unbound => 
	        (complain("unbound head structure: " ^ name first);
		 print "  in path: "; printSequence "." printSym spath;
		 newline();
		 bogusSTR)
     in lookPathinStr(binding,ap,spath,lookLast)
    end
  | lookPath _ = impossible "EnvAccess.lookPath"


fun lookPathArTYC0 (path: symbol list, arity: int) =
    let val tycref as ref tyc = lookPath(path, fn(str,id,_,_) => lookTYCinStr(str,id))
     in checkArity(tyc,arity);
	tycref
    end

(* debug print functions *)
val prIntPath = printClosedSequence ("[",",","]") (print:int->unit)
fun prSymPath spath = printSequence "." printSym (rev spath)

fun lookPathArTYCinSig (depth: int) (spath as first::rest, arity) : tycon ref =
    let	fun complainUnbound() =
	    (complain "unbound type constructor in signature";
	     print "  name: "; printSequence "." printSym spath;
	     newline();
	     raise Syntax)
	(* second arg of get is expected to be a signature *)
	fun get([id],STRstr{table,env as REL{t,...},...}) = 
	     (case lookTYCinTable(table,id)
		   handle UnboundTable => complainUnbound()
	       of ref(INDtyc i) => (checkArity(t sub i, arity); [i])
	        | ref(SHRtyc p) => (checkArity(getEpathTyc(p,env), arity); p)
		| _ => impossible "lookPathArTYCinSig.get")
	  | get(id::rest,STRstr{table,env=REL{s,...},...}) =
	      let val STRvar{binding=INDstr k,...} =
			lookSTRinTable(table,id)
			handle UnboundTable => complainUnbound()
	       in k::get(rest, s sub k)
	      end
	  | get([],_) = impossible "EnvAccess.lookPathArTYCinSig.get - empty path"
	  | get(p,NULLstr) =
	     (prSymPath p; print "\n";
	      impossible "EnvAccess.lookPathArTYCinSig.get - NULLstr")
	  | get(p,INDstr _) =
	     (prSymPath p; print "\n";
	      impossible "EnvAccess.lookPathArTYCinSig.get - INDstr")
	  | get(p,SHRstr _) =
	     (prSymPath p; print "\n";
	      impossible "EnvAccess.lookPathArTYCinSig.get - SHRstr")
	  | get _ = impossible "EnvAccess.lookPathArTYCinSig.get - bad args"
	fun lookInStr(str) =
	    let val tycref = 
		    lookPathinStr(str, [], spath,
			 (fn(str,id,_,_) => lookTYCinStr(str,id)))
	     in checkArity(!tycref,arity);
		tycref
	    end
	val leadStr = lookSTR0 first
		      handle Unbound => complainUnbound()
     in case leadStr
	  of (STRvar{binding=INDstr i,...},{path=h::r,strenv=REL{s,...}}) =>
	      if h < 0 (* indicates signature component *)
	      then ref(RELtyc(addzeros(depth+h,r@(i::get(rest, s sub i)))))
	      else lookInStr(s sub i)
	   | (STRvar{binding=SHRstr(i::r),...},{strenv=REL{s,...},...}) =>
	        lookInStr(getEpath(r, s sub i))
	   | (STRvar{binding as STRstr _,...},_) => lookInStr binding
	   | _ => impossible "EnvAccess.lookPathArTYCinSig - leadStr"
    end
  | lookPathArTYCinSig _ _ = impossible "lookPathArTYCinSig - bad arg"

val lookPathArTYC = ref lookPathArTYC0


(* functions to collect stale lvars for unbinding *)
exception LOOKLVAR

fun lookLvar (env: env) (key: int * string) =
    case lookEnv(env,key)
      of (VARbind(VALvar{access=LVAR v,...}),_) => v
       | (STRbind(STRvar{access=LVAR v,...}),_) => v
       | (FCTbind(FCTvar{access=LVAR v,...}),_) => v
       | _ => raise LOOKLVAR

fun runbound index =
    case index mod namespaces
      of 0 => true  (* var *)
       | 4 => true  (* structure *)
       | 5 => true  (* functor *)
       | _ => false

fun staleLvars(newenv,oldenv) : int list =
    let val lvarset = ref([] : int list)
	val get = lookLvar oldenv
        fun collect (i,s,_) = 
	    if runbound i
	    then (lvarset := get(i,s) :: !lvarset)
		 handle LOOKLVAR => ()
		      | Unbound => ()
	    else ()
     in appenv collect (newenv,oldenv);
        !lvarset
    end

(* reset state of EnvAccess *)
fun reset() =
    (varPatchList := nil;
     boundTyvars := [];
     tyvarsMode := EXP)

end (* structure EnvAccess *)
