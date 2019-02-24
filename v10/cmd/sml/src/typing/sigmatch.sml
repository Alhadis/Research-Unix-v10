(* Copyright 1989 by AT&T Bell Laboratories *)
(* sigmatch.sml *)

structure SigMatch : SIGMATCH = struct

structure Basics = Basics

open List2 PrintUtil ErrorMsg Access Stampset Basics BareAbsyn BasicTypes
     EnvAccess EnvAccess.Env TypesUtil PrintType ModUtil

(* debug print functions *)
val prIntPath = printClosedSequence ("[",",","]") (print:int->unit)
fun prSymPath spath = printSequence "." printSym (rev spath)

val symName = Symbol.name
val anonName = Symbol.symbol "Anon"
fun for a b = app b a

exception CompareTypes
exception REFtyc
val refstamp = tycStamp(!refTycon)
and arraystamp = tycStamp(!arrayTycon)

fun compType(specty, specsign:polysign, actty, actsign:polysign, actarity): unit =
    let val env = array(actarity,UNDEFty)
	fun comp(ty1, VARty(ref(INSTANTIATED(ty2)))) =
	      comp(ty1,ty2)
	  | comp(ty1, FLEXRECORDty(ref(CLOSED ty2))) = comp(ty1,ty2)
	  | comp(ty1, VARty(ref(IBOUND i))) =
	     (case env sub i
		of UNDEFty =>
		    let val {weakness=aw,eq=ae} = nth(actsign,i)
		     in if aw < infinity
			then let fun checkweak(VARty(ref(IBOUND n))) =
					let val {weakness=sw,...} = nth(specsign,n)
					 in if sw > aw then raise CompareTypes
					    else ()
					end
				    | checkweak(CONty(_,args)) = app checkweak args
				    | checkweak _ = impossible "compType/checkweak"
			      in checkweak ty1
			     end
			else ();
			if ae 
			then checkEqTySig(ty1,specsign)
			     handle CHECKEQ => raise CompareTypes
			else ();
			update(env,i,ty1)
		    end
		 | ty => if equalType(ty1,ty)
			 then ()
			 else raise CompareTypes)
	  | comp(ty1 as CONty(ref tycon, args), ty2 as CONty(ref tycon', args')) =
	      if eqTycon(tycon,tycon')
	      then app2 comp (args,args')
	      else (comp(reduceType ty1, ty2)
		    handle ReduceType =>
		      comp(ty1, reduceType ty2)
		      handle ReduceType => raise CompareTypes)
	  | comp(_, ERRORty) = ()
	  | comp _ = raise CompareTypes
     in comp(specty,actty)
    end

fun compareTypes(spec: ty, actual: ty, name) : unit =
    let fun error() =
	    (complain "value type in structure doesn't match signature spec";
	     PrintType.resetPrintType();
	     print ("  name: " ^ symName name ^ "\n  spec:   ");
	     PrintType.printType(spec);
	     print "\n  actual: ";
	     PrintType.printType(actual); newline())
     in case spec
	  of POLYty{sign,tyfun=TYFUN{body,...}} =>
	      (case actual
		 of POLYty{sign=sign',tyfun=TYFUN{arity,body=body'}} =>
		      (compType(body,sign,body',sign',arity)
		       handle CompareTypes => error())
		  | _ => error())
	   | ERRORty => ()
	   | _ =>
	      (case actual
		 of POLYty{sign,tyfun=TYFUN{arity,body}} =>
		      (compType(spec,[],body,sign,arity)
		       handle CompareTypes => error())
		  | _ => if equalType(spec,actual)
			 then ()
			 else error())
    end


(* making abstraction structures *)

fun abstract(sgn as STRstr{kind=SIGkind{stamps={strStamps=sigStrStamps,
		    			        tycStamps=sigTycStamps},
					...},
			   ...},
	     str, {strStamps, tycStamps}) =
    let val transStrStamp = join(strStamps,sigStrStamps)
	val transTycStamp = join(tycStamps,sigTycStamps)
	fun abstractTyc(sigtyc,strtyc) =
	    case sigtyc
	      of TYCON{kind=DATAtyc _,...} => strtyc
	       | _ => let val stamp = tycStamp sigtyc
		       in if tycFixed(stamp)
			  then strtyc
			  else setTycStamp(transTycStamp(stamp),sigtyc)
		      end
	fun abstractStr(STRstr{stamp,sign,table,env,...},
			str as STRstr{env=env',...}) =
	      if strFixed stamp
	      then str
	      else let val newenv as REL{s,t} = abstractEnv(env,env')
		       val newstr = STRstr{stamp=transStrStamp(stamp),
				    	   env=newenv,
					   sign=sign,table=table,
					   kind=STRkind{path=[]}} (* ??? def of kind *)
		    in ArrayExt.app((setParent newstr), s, 2);
		       newstr
		   end
	  | abstractStr (INDstr i,_) =
	      impossible ("3437 in sigmatch: " ^makestring i)
	  | abstractStr _ = impossible "9833 in sigmatch (abstractStr)"
	and abstractEnv(REL{s=sSig,t=tSig}:strenv, REL{s=sStr,t=tStr}:strenv) =
	    let val sNew = array(Array.length sSig, NULLstr)
		val tNew = array(Array.length tSig, NULLtyc)
		fun foreachStr i =
		    (update(sNew,i,abstractStr(sSig sub i, sStr sub i));
		     foreachStr(i+1))
		fun foreachTyc i =
		    (update(tNew,i,abstractTyc(tSig sub i, tStr sub i));
		     foreachTyc(i+1))
	     in foreachStr 2  (* ignoring parent and parameter slots *)
		handle Subscript =>
		foreachTyc 0
		handle Subscript =>
		REL{s=sNew,t=tNew}
	    end
     in	abstractStr(sgn,str)
    end
  | abstract _ = impossible "8375 in sigmatch (abstract)"

(* signature matching *)

fun matchx (parent: Structure)
    	   (mapfns as {mapstr,mapstr1,maptyc})
    	   (abs, path, stamps,
	    sgn as STRstr{stamp,sign,...},
	    str as STRstr{stamp=stamp',sign=sign',table,env,...},
	    param: Structure) 
	   : Structure * thinning =
      if strFixed(stamp) andalso stamp <> stamp'
      then (print "fixed signature stamp: "; print stamp;
	    print "\nstructure stamp: "; print stamp';
	    print "\npath: "; prSymPath path; print "\n";
	    condemn "fixed signature doesn't agree with structure")
      else if sign = sign'
      then (mapstr(sgn,str);
	    (if abs then abstract(sgn,str,stamps) else str, NONE))
      else let val v = mkLvar()
	       val _ = (openStr(); openOld({path=[v],strenv=env},table))
	       val (str',transl) =
		     realizex parent mapfns (abs,path,stamps,stamp',sgn,param)
	    in closeStr();
	       (str',SOME(v,transl))
	   end
  | matchx _ _ _ = impossible "843 in sigmatch"

and realizex (parent: Structure)
    	     (mapfns as {mapstr1,maptyc,mapstr})
	     (abs, path, stamps, strStamp,
	      sgn as STRstr{stamp = boundStamp, sign, table,
		     	    env = sigenv as REL{s=sSig,t=tSig},
			    kind = SIGkind{bindings,share,...},...},
	      param: Structure)
	     : Structure * trans list =
    let val sNew = array(Array.length sSig, NULLstr)
	val tNew = array(Array.length tSig, NULLtyc)
	val newenv = REL{s=sNew,t=tNew}
        val newstr = STRstr{stamp=strStamp,sign=sign,table=table,env=newenv,
		  	    kind=STRkind{path=path}}
        fun checkSpec spec =
	    case spec
	      of STRbind(STRvar{name=[id],binding=INDstr i,...}) =>
		   let val STRvar{access,binding=str',...} =
			    lookSTRlocal id
			    handle Unbound =>
			      condemn("unmatched structure spec: " ^ symName id)
		       val (str,thin) = matchx newstr mapfns
					    (false, id::path, stamps,
					     sSig sub i, str',NULLstr)
		    in update(sNew,i,str);
		       [case thin
		          of NONE => VALtrans access
			   | SOME(v,transl) => THINtrans(access,v,transl)]
		   end
	       | TYCbind(ref(INDtyc i)) =>
		  let val sigTycon = tSig sub i
		      val name = tycName sigTycon
		      val strTycon = !(lookTYClocal name) 
				     handle Unbound =>
				       condemn("unmatched type spec: "^
					        symName(name))
		      val s = tycStamp sigTycon
		      val s' = tycStamp strTycon
		   in update(tNew,i,strTycon);
		      if tycFixed s andalso s <> s'
		      then if equalTycon(sigTycon,strTycon)
			   then maptyc(s,strTycon)
			   else condemn("bad match for fixed type spec "
				        ^ symName(name))
		      else (case (sigTycon, strTycon)
			     of (TYCON{arity,kind=DATAtyc dcons,...},
				 TYCON{arity=arity',kind=DATAtyc dcons',...}) =>
				  if arity = arity'
				     andalso length(dcons) = length(dcons')
				  then maptyc(s,strTycon)
				  else condemn("mismatching datatype spec: "
					       ^ symName(name))
			      | (TYCON{kind=DATAtyc _,...}, _) => 
				  condemn("unmatched datatype spec: "^symName(name))
			      | (TYCON{arity,kind=ABStyc,eq,...}, _) =>
				  if arity <> tyconArity(strTycon)
				  then condemn("mismatching tycon arities: "
					       ^ symName(name))
				  else if (!eq=YES) andalso not(isEqTycon(strTycon))
				  then condemn("mismatched eqtype spec: "
					       ^ symName(name))
				  else maptyc(s,strTycon)
			      | _ => impossible "realizex/checkSpec/TYCbind");
		       nil
		   end
	       | CONbind(DATACON{name,typ,rep=VARIABLE _,const,...}) =>
		   let val DATACON{typ=typ',rep=VARIABLE(access),...} =
			     lookCONlocal name
			     handle Unbound =>
			      condemn ("unmatched exception spec: "^symName(name))
			     | Bind =>
			      condemn ("unmatched exception spec: "^symName(name))
		    in compareTypes(typeInContext(!typ,newenv),!typ',name);
		       [VALtrans access]
		   end
	       | CONbind(DATACON{name,typ,...}) =>
		   let val DATACON{typ=typ',...} =
			     lookCONlocal name
			     handle Unbound =>
			      condemn ("unmatched data constructor spec: "
				       ^symName(name))
		    in compareTypes(typeInContext(!typ,newenv),!typ',name);
		       nil
		   end
	       | VARbind(VALvar{name=[id],typ,...}) =>
		  (case (lookVARCONlocal id
			  handle Unbound =>
			    condemn("unmatched val spec: "^symName(id)))
		     of VARbind(VALvar{access,typ=typ',...}) =>
			  (* no propagation of INLINE access!! *)
			 (compareTypes(typeInContext(!typ,newenv),!typ',id);
 			  [case access of INLINE _ => VALtrans access
					| PATH _ => VALtrans access
					| LVAR _  => 
					   if !ErrorMsg.anyErrors
					   then VALtrans access
					   else impossible "sigmatch.1"
					| _ => impossible "sigmatch.2"])
		      | CONbind(dcon as DATACON{typ=typ',...}) =>
			  (compareTypes(typeInContext(!typ,newenv),!typ',id);
			   [CONtrans dcon])
		      | _ => impossible "sigmatch.476")
	       | _ => nil (* nonchecked binding (FIXbind) *)
        fun checkList (a::rest) =
	      (checkSpec a handle Syntax => nil) @ checkList rest
          | checkList nil = nil

	val _ = update(sNew,0,parent)  (* define parent before checking specs *)
	val _ = update(sNew,1,param)   (* ditto for param *)
	val trans = checkList bindings
	val _ = Sharing.checkSharing(table,newenv,share)
	val str = if abs then abstract(sgn,newstr,stamps) else newstr
     in mapstr1(boundStamp,str);
	linkParents str;  (* should be redundant *)
	(str, trans)
    end
  | realizex _ _ _ = impossible "783 in sigmatch"

val defaultMapfns = 
    let fun ignore _ = ()
     in {mapstr=ignore,mapstr1=ignore,maptyc=ignore}
    end

val match0 = matchx NULLstr
val match = matchx NULLstr defaultMapfns
val realize = realizex NULLstr defaultMapfns

end (* structure SigMatch *)
