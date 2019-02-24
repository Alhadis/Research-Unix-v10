(* Copyright 1989 by AT&T Bell Laboratories *)
(* misc.sml *)

structure Misc : MISC =
struct

  open ErrorMsg Symbol PrintUtil Access Basics BasicTypes
       TypesUtil Absyn EnvAccess EnvAccess.Env

  val ASTERISKsym = Symbol.symbol "*"
  val EQUALsym = Symbol.symbol "="
 
  fun for l f = app f l

  local fun uniq ((a0 as (a,_,_))::(r as (b,_,_)::_)) = 
		    if Symbol.eq(a,b) then uniq r else a0::uniq r
	  | uniq l = l
      fun gtr((a,_,_),(b,_,_)) = 
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
   in val sort3 = uniq o Sort.sort gtr
  end

  fun protect((enter,exit),doit) =
      let val t = enter()
       in (doit() before exit t)
           handle exn => (exit t; raise exn)
      end

  val protectScope = (openScope,resetEnv)

  (* following could go in Absyn *)
  val bogusID = Symbol.symbol "bogus"
  val bogusExnID = Symbol.symbol "Bogus"
  val bogusExp = VARexp(ref(mkVALvar(bogusID, ref ERRORty)))

  val anonName = Symbol.symbol "Anon"
  val anonParamName = Symbol.symbol "AnonParam"

  val nullSigStamp = Stampset.newStamp(Stampset.sigStamps)
  val nullSigStampsets = Stampset.newStampsets()
  val nullStrenv = REL{s=arrayoflist [NULLstr,NULLstr], t=arrayoflist []}
  val nullSig = 
      STRstr{stamp = Stampset.newStamp(#strStamps nullSigStampsets),
             sign = nullSigStamp,
	     table = newTable(),
	     env = nullStrenv,
	     kind = SIGkind{share = {s=[],t=[]},
		            bindings = [],
			    stamps = nullSigStampsets}}
  val nullStr = 
      STRstr{stamp = Stampset.newStamp(Stampset.fixedStrStamps),
             sign = nullSigStamp,
	     table = newTable(),
	     env = nullStrenv,
	     kind = STRkind{path=[Symbol.symbol "NullStructure"]}}
  val nullParamVar = STRvar{name=[anonParamName],
			    access=LVAR(namedLvar(anonParamName)),
			    binding=nullSig}

  fun discard _ = ()

  fun single x = [x]

  fun varcon (VARbind v) = VARexp(ref v)
    | varcon (CONbind d) = CONexp d
    | varcon _ = impossible "parse.39"

  fun lookID(id : symbol): exp = 
       varcon (lookVARCON id handle Unbound => unboundVAR id)
       handle Unboundrec => VARexp(getPatchVar id)

  val lookIDinStr = varcon o lookVARCONinStr

  (* the following two functions belong in TypesUtil *)
  fun checkNonCircular(l : tycon list) =
      let fun less(TYCON{path=a::_,...},TYCON{kind=DEFtyc(TYFUN{body,...}),...}) =
		let fun find(CONty(ref(TYCON{path=b::_,...}), args)) = 
			    Symbol.eq(a,b) orelse exists find args
		      | find(CONty(_, args)) = exists find args
		      | find _ = false
		 in find body
		end
	    | less _ = impossible "Misc.checkNonCircular"
       in (Topsort.topsort2 less l; ())
	   handle Topsort.Cycle => complain "circular withtype declaration"
      end

  fun makeAbstract(datatycs,withtycs) =
      let val (stamps,abstycs,dconss) =
	      let fun loop((tr as ref(TYCON{stamp,arity,eq,path,
				  	    kind=DATAtyc dcons}))
			   ::rest,stamps,abstycs,dconss) =
		      let val abstyc = TYCON{stamp=stamp,arity=arity,path=path,
					     eq=ref NO,kind=ABStyc}
		       in tr := abstyc;
			  loop(rest,stamp::stamps,abstyc::abstycs,dcons::dconss)
		      end
		    | loop([],stamps,abstycs,dconss) = (stamps,abstycs,dconss)
		    | loop _ = impossible "Misc.makeAbstract.loop"
	       in loop(datatycs,[],[],[])
	      end
	  fun subst(tycref as ref(TYCON{stamp,...})) =
		let fun find(stamp'::stamps,tyc::tycs) =
			  if stamp = stamp' then tycref := tyc else find(stamps,tycs)
		      | find([],_) = ()
		      | find _ = impossible "Misc.makeAbstract.subst.find"
		 in find(stamps,abstycs)
		end
	    | subst _ = ()
	  fun substType(CONty(reftyc,args)) =
		(subst reftyc; app substType args)
	    | substType(POLYty{tyfun=TYFUN{body,...},...}) = substType body
	    | substType _ = ()
       in for dconss (app (fn DATACON{typ,...} => substType(!typ)));
	  for withtycs
	      (fn ref(TYCON{kind=DEFtyc(TYFUN{body,...}),...}) => substType body
	        | _ => impossible "Misc.makeAbstract.fn");
           abstycs
      end

  fun dumpStructure(STRvar{access=PATH p,binding,...}) =
      let val STRstr{table,env,...} = binding
	  val vbs = ref ([]: vb list)
	  and strbs = ref([]: strb list)
	  and tbs = ref([]: tb list)
	  and ebs = ref([]: eb list)
	  fun rebind(index,strg,VARbind(var)) =
		(case varApplied(var,{path=p,strenv=env},[symbol strg])
		  of oldvar as VALvar{name=[n],typ,...} =>
		     let val newvar = mkVALvar(n,typ)
		         val vb = VB{pat = VARpat(newvar),
				     exp = VARexp(ref oldvar),
				     tyvars = []}
		      in vbs := vb :: !vbs;
		         Env.add(index,strg,VARbind newvar)
		     end
		   | oldvar as OVLDvar _ => Env.add(index,strg,VARbind oldvar)
		   | _ => impossible "Misc.dumpStructures.rebind")
	    | rebind(index,strg,STRbind(strvar)) =
		let val oldstrvar as STRvar{name as [n],binding,...} =
			  strApplied(strvar,{path=p,strenv=env},[symbol strg])
		    val newstrvar = STRvar{access=LVAR(namedLvar n),
					   name=name,
					   binding=binding}
		    val strb = STRB{strvar=newstrvar,
				    def=VARstr oldstrvar,
				    thin=NONE,
				    constraint=NONE}
		 in strbs := strb :: !strbs;
		    Env.add(index,strg,STRbind newstrvar)
		end
	    | rebind(index,strg,TYCbind(reftyc as ref tycon)) =
		let val reftyc = case tycon
			           of INDtyc i => 
				        (case env
					  of REL{t,...} => ref(t sub i)
					   | DIR => impossible "dumpStructure.rebind")
				    | SHRtyc p => ref(getEpathTyc(p,env))
				    | _ => reftyc
		    val tb = TB{tyc = reftyc, def = CONty(reftyc,[])}
			       (* bogus args in def field *)
		 in tbs := tb :: !tbs;
		    Env.add(index,strg,TYCbind reftyc)
		end
	    | rebind(index,strg,CONbind(dcon)) =
		let val olddcon as DATACON{name,const,typ,rep,sign} =
			  dconApplied(dcon,{path=p,strenv=env})
		 in case rep
		      of VARIABLE _ =>
			   let val newdcon =
				 DATACON{name=name,const=const,typ=ref(!typ),sign=sign,
					 rep=VARIABLE(LVAR(namedLvar(name)))}	
			       val eb = EBdef{exn=newdcon,edef=olddcon}
			    in ebs := eb :: !ebs;
			       Env.add(index,strg,CONbind newdcon)
			   end
		       | _ => Env.add(index,strg,CONbind olddcon)
		end
	    | rebind(index,strg,FIXbind(fixvar)) =
		Env.add(index,strg,FIXbind fixvar)
	    | rebind _ = ()
       in IntStrMap.app rebind table;
	  SEQdec [STRdec(!strbs),TYPEdec(!tbs),EXCEPTIONdec(!ebs),VALdec(!vbs)]
      end
    | dumpStructure _ = impossible "Misc.dumpStructure"

end (* structure Misc *)
