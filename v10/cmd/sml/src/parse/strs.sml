(* Copyright 1989 by AT&T Bell Laboratories *)
signature STRS = 
sig type strArg
    type strtype type spectype
    type functorFormal
    type 'a pathstamped
    type 'a susp
    type functorActual
    type symbol type structureVar type strb type signatureVar
    type dec type Functor type functorVar type strexp type Structure
    type signtype type fctb
    val makeSTRBs:	(symbol*structureVar*strb) list pathstamped
			 -> dec pathstamped
    val makeSTRB:	symbol * Structure option susp * strtype -> bool -> 
			 (symbol * structureVar * strb) list pathstamped
    val make_str_qid:	symbol list -> strtype
    val make_str_struct: dec list pathstamped -> strtype
    val spread_args:	dec list pathstamped -> functorActual
    val single_arg:	strtype -> functorActual
    val make_str_app:	symbol * functorActual -> strtype
    val make_str_let:	dec list pathstamped * strtype -> strtype
    val make_sigb:	symbol * signtype -> signatureVar list susp
    val makeSIGdec:	signatureVar list susp -> dec pathstamped
    val single_formal:	symbol * signtype -> functorFormal
    val spread_formal:	spectype -> functorFormal
    val makeFCTB:	symbol * functorFormal * Structure option susp * strtype ->
			    (symbol * functorVar * fctb) list susp
    val makeFCTdec: (symbol * functorVar * fctb) list susp ->
			dec pathstamped
    val makeLOCALsdecs:	dec list pathstamped * dec list pathstamped
			   -> dec list pathstamped
end

structure Strs: STRS = 
struct

  open ErrorMsg Symbol PrintUtil
  open Access Basics BasicTypes TypesUtil Absyn
  open Env
  open EnvAccess
  open ModUtil
  open SigMatch
  open Misc

type stampsets = Stampset.stampsets
type 'a susp = unit -> 'a
type 'a pathstamped = symbol list * stampsets -> 'a
type signtype = Signs.signtype
type spectype = Signs.spectype

type strArg = {abstract:bool,constraint: Structure option,
		path:symbol list, param: Structure,
		stamps:Stampset.stampsets}
type strtype = strArg -> strexp * Structure * thinning

type functorFormal = (symbol * access * Structure * bool) susp
type functorActual = Functor * strArg -> strexp * Structure
fun makeSTRBs strb' $ =
  STRdec(map (fn (name,strVar,strSyn) => (bindSTR(name,strVar); strSyn)) (strb' $))

fun makeSTRB(name,constraint,str) abstract ($ as (path,stamps)) =
 case (constraint(),abstract)
  of (NONE,true) =>
	(complain "abstraction requires a signature constraint";
	 makeSTRB(name,fn()=>NONE,str) false $)
   | (signopt,_) =>
  let val (strexp,str,thin)=
		str{abstract=abstract,constraint=signopt,path=name::path,
	            param=NULLstr,stamps=stamps}
      val strVar = STRvar{access=LVAR(namedLvar(name)),	name=[name],
				binding=str}
   in [(name, strVar,
	     STRB{strvar=strVar, def=strexp, constraint=signopt, thin=thin})]
  end

fun make_str_qid qid ({abstract,path,stamps,constraint,...}:strArg) =
   let val strVar as STRvar{binding,...} = lookPath(qid,lookSTRinStr)
    in case constraint
	 of NONE => (VARstr strVar, binding, NONE)
	  | SOME sgn =>
	      let val (str,thin) =
		    SigMatch.match(abstract,path,stamps,sgn,binding,param)
	       in (VARstr strVar, str, thin)
	      end
   end

fun make_str_struct sdecs ({abstract,path,param,stamps,constraint,...}:strArg) =
    let val _ = openStr()
	val body = sdecs(path,stamps)
     in case constraint
	 of NONE => let val (thin,table) = BuildMod.buildStrTable ()
		     in (STRUCTstr{body=body,locations=thin},
		    	 mkSTR(path,table,DIR,stamps),
		   	 NONE)
	      	    end
	  | SOME sgn => let val (str,thin) =
				 SigMatch.realize(abstract,path,stamps,
				    Stampset.newStamp(#strStamps stamps),
				    sgn,param)
			 in closeStr();
			    (STRUCTstr{body=body,locations=thin}, str, NONE)
		        end
    end

fun spread_args sdecs (_,({stamps,...}:strArg)) =
  let val _ = openStr()
      val body = sdecs([anonParamName],stamps)
      val (thin,table) = BuildMod.buildStrTable ()
   in (STRUCTstr{body=body,locations=thin},
	mkSTR([anonParamName],table,DIR,stamps))
  end

fun single_arg str (fct,({stamps,...}:strArg)) =
    let val FUNCTOR{paramName,...} = fct
        val (strexp,str,_) = str{abstract=false,constraint=NONE,
			 path=[paramName],param=NULLstr,stamps=stamps}
     in (strexp,str)
    end

fun make_str_app (id,arg) (info as ({abstract,path,stamps,constraint,...}:strArg)) =
    let val fctVar as FCTvar{binding=fct,...} = lookFCT id
	val (argexp,argstr) = arg(fct,info)
	val (result,thin1) = Functor.applyFunctor(fct,argstr,path,stamps)
	val strexp = APPstr{oper=fctVar, argexp=argexp, argthin=thin1}
     in case constraint
	  of NONE => (strexp,result,NONE)
	   | SOME sgn =>
	       let val (thinned,thin2) =
		       SigMatch.match(abstract,path,stamps,sgn,result,param)
		in (strexp,thinned,thin2)
	       end
    end

fun make_str_let (sdecs,str) (info as ({path,stamps,...}:strArg)) =
   protect(protectScope,fn()=>
      let val locals = sdecs(path,stamps)
	  val (bodyexp,bodystr,thin) = str info
       in (LETstr(SEQdec locals, bodyexp),bodystr,thin)
      end)

fun make_sigb(name,sign) () =
   let val sigvar = SIGvar{name=name,binding= sign(1,Stampset.newStampsets())}
    in bindSIG(name, sigvar); [sigvar]
   end

fun makeSIGdec sigb ([],stamps) = SIGdec(sigb())
  | makeSIGdec sigb (path,stamps) =
			 (warn "signature found inside structure or functor"; 
			  makeSIGdec sigb ([],stamps))

fun single_formal(name,sign) () =
   let val senv = array(2, NULLstr) and tenv = array(0, NULLtyc)
       val _ = openNew({path=[~1], strenv=REL{t=tenv,s=senv}}, newTable())
       val access = LVAR(namedLvar name)
       val param = sign(1,Stampset.newStampsets())
    in update(senv,1,param);
       bindSTR(name,STRvar{name=[name], access=access, binding=INDstr(1)});
       (name,access,param,false)
   end

fun spread_formal spec_s () =
    let val plvar = namedLvar anonParamName
        val param as STRstr{env,table,...} =
              Signs.makeSIG spec_s (2,Stampset.newStampsets())
     in openOld({path=[~1,1],strenv=env},table);
        (anonParamName,LVAR(plvar),param,true)
    end

fun makeFCTB(name,fparam,constraint_op,str) () =
   let val mEntry = openScope()
       val (pname,paccess,param,spreadParams) = fparam()
       val resSign = constraint_op ()
       val _ = if spreadParams
	       	then let val STRstr{table,env,...} = param
       			 and LVAR plvar = paccess
	       	      in resetEnv(mEntry);
	       		 openOld({path=[plvar],strenv=env},table)
	       	     end
	       	else ()
       val bodystamps = Stampset.newStampsets()
       val (bodyexp,bodystr,thin) = 
		str{abstract=false,constraint=resSign,path=[],
			stamps=bodystamps,param=param}
       val openBody = case bodystr
		       of STRstr{stamp=bodystamp,env=DIR,...} =>
			   Stampset.member(bodystamp,(#strStamps bodystamps))
		        | _ => false
       val paramVis = case resSign of SOME _ => true | NONE => openBody
       val body = if openBody
		    then Functor.abstractBody(bodystr,param,bodystamps,
			   Stampset.newStamp(Stampset.sigStamps))
		    else bodystr
       val paramvar = STRvar{name=[pname], access=paccess, binding=param}
       val fctv = FCTvar{name=name, 
	  		 access=LVAR(namedLvar(name)),
       		  	 binding=FUNCTOR{paramName=pname, param=param,
       				  	 body=body, paramVis=paramVis,
       				  	 stamps=bodystamps}}
       val fb = FCTB{fctvar=fctv, param=paramvar, def=bodyexp, thin=thin,
	       	      constraint=resSign}
    in resetEnv(mEntry);
       [(name,fctv,fb)]
   end

fun makeFCTdec fctb ([],stamps) =
  FCTdec(map (fn(name,fctVar,fctSyn)=>(bindFCT(name,fctVar); fctSyn)) (fctb()))
  | makeFCTdec fctb (path,stamps) =
   (warn "functor found inside structure or functor"; 
    makeFCTdec fctb ([],stamps))

fun makeLOCALsdecs(sdecs1,sdecs2) $ =
  let val envLocal = openScope()
      val ld1 = sdecs1 $
      val envIn = (openScope(); current())
      val ld2 = sdecs2 $
   in splice(envLocal,envIn);
      [LOCALdec(SEQdec ld1, SEQdec ld2)]
  end

end
