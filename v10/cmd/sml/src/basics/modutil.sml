(* modutil.sml *)

structure ModUtil : MODUTIL =
struct

open ErrorMsg Basics TypesUtil Stampset

fun mapSubstrs(f,senv) =
  (* Creates a new copy of a structure environment array by applying f to
     substructures.  Leaves parent and param slots of new env undefined *)  
    let val new = array(Array.length senv, NULLstr)
	fun loop i = (update(new,i,f(senv sub i)); loop(i+1))
     in loop 2 handle Subscript => new
    end

(* setParent only sets parents that are initially NULLstr *)
fun setParent (parent: Structure) =
    fn (STRstr{env=REL{s,...},...}) =>
	((case s sub 0
	   of NULLstr => update(s,0,parent)
	    | _ => ())
	 handle Subscript => ())
     | (STRstr{env=DIR,...}) => ()
     | NULLstr => ()
     | _ => impossible "ModUtil.setParent"

(* resetParent redefines parents unconditionally *)
fun resetParent (parent: Structure) =
    fn (STRstr{env=REL{s,...},...}) =>
         (update(s,0,parent) handle Subscript => impossible "ModUtil.resetParent 1")
     | (STRstr{env=DIR,...}) => ()
     | NULLstr => ()
     | _ => impossible "ModUtil.resetParent 2"

fun linkParents(str as STRstr{env=REL{s,...},...}) =
    ArrayExt.app((fn str' => (setParent str str'; linkParents str')), s, 1)
  | linkParents(STRstr{env=DIR,...}) = ()
  | linkParents(NULLstr) = ()
  | linkParents _ = ErrorMsg.impossible "ModUtil.linkParents"

fun shiftStamps(transStrStamp,transTycStamp,newStamps,fixstamp,fixsign,fixstr) =
    let fun newTyc(tyc as TYCON{stamp,arity,eq,path,kind}) =
	    if tycFixed(stamp)
	    then tyc
	    else (case kind
		   of ABStyc => setTycStamp(transTycStamp(stamp),tyc)
		    | DATAtyc _ => setTycStamp(transTycStamp(stamp),tyc)
	            | DEFtyc(TYFUN{arity,body}) =>
		       TYCON{stamp=transTycStamp(stamp),
		       	     arity=arity,
			     eq=eq,
			     path=path,
			     kind=DEFtyc(TYFUN{arity=arity,
			     		       body=newType(body)})}
	            | _ => tyc)
	  | newTyc _ = impossible "ModUtil.shiftStamps.newTyc - bad arg"
	and newType(ty) =
	    case ty
	      of VARty(ref(INSTANTIATED ty')) => newType ty'
	       | FLEXRECORDty(ref(CLOSED ty')) => newType ty'
	       | POLYty{tyfun=TYFUN{body,arity},sign} =>
		   POLYty{tyfun=TYFUN{body=newType body,arity=arity},sign=sign}
	       | VARty _ => ty
	       | CONty(tycref as ref tyc,args) =>
		   CONty(ref(newTyc(tyc)), map newType args)
	       | ERRORty => ty
	       | _ => impossible "shiftStamps.newType"
        fun newEnv(REL{s,t}) =
	    let val s' = array(Array.length s, NULLstr)
		fun loop i = (update(s',i,newStr(s sub i)); loop(i+1))
	     in loop 1 handle Subscript => 
		  REL{s=s',t=ArrayExt.map(newTyc,t,0)}
	    end
	  | newEnv _ = impossible "ModUtil.shiftStamps.newEnv - bad arg"
	and newStr(str as STRstr{stamp,sign,table,env,kind}) =
	    if Stampset.strFixed(stamp)
	    then str
	    else if stamp=fixstamp andalso sign=fixsign
	    then fixstr
	    else let val newenv as REL{s,...} = newEnv env
		     val new = STRstr{stamp=transStrStamp(stamp),
			       	      kind=case kind
					     of SIGkind{stamps,share,bindings} =>
						 SIGkind{stamps=newStamps,
						 	 share=share,
							 bindings=bindings}
					      | _ => kind,
				      env=newenv,
				      sign=sign,table=table}
		     val setpar = setParent new
		     fun loop i = (setpar(s sub i); loop(i+1))
		  in loop 1 handle Subscript => new
		 end
	  | newStr(NULLstr) = NULLstr
          | newStr(INDstr i) = impossible("ModUtil.shiftStamps.newStr INDstr "^
					   makestring i)
	  | newStr(SHRstr _) = impossible "ModUtil.shiftStamps.newStr SHRstr"
     in newStr
    end

fun shiftSigStamps(base : stampsets, sgn as STRstr{kind=SIGkind{stamps,...},...}) =
    let val {strStamps=sbase,tycStamps=tbase} = base
	and {strStamps=soffset,tycStamps=toffset} = stamps
        val transStrStamp = join(sbase,soffset)
	and transTycStamp = join(tbase,toffset)
     in shiftStamps(transStrStamp,transTycStamp,base,~1,~1,NULLstr) sgn
    end
  | shiftSigStamps _ = impossible "ModUtil.shiftSigStamps -- bad arg"

fun shiftFctStamps(FUNCTOR{paramName,paramVis,param,body,
    			   stamps as {strStamps=bodyStrStamps,
				      tycStamps=bodyTycStamps}}) =
    let val newParamStamps = newStampsets()
	val newBodyStamps = newStampsets()
	val STRstr{stamp=pstamp,sign=psign,
	           kind=SIGkind{stamps=pstamps as {strStamps=paramStrStamps,
					           tycStamps=paramTycStamps},
				...},
		   ...} =
	      param
	val bodyStrTrans = join(#strStamps newBodyStamps, bodyStrStamps)
	val bodyTycTrans = join(#tycStamps newBodyStamps, bodyTycStamps)
	val paramStrTrans = join(#strStamps newParamStamps, paramStrStamps)
	val paramTycTrans = join(#tycStamps newParamStamps, paramTycStamps)
	fun transStrStamps x =
	    if member(x,bodyStrStamps)
	    then bodyStrTrans x
	    else if member(x,paramStrStamps)
	    then paramStrTrans x
	    else x
	fun transTycStamps x =
	    if member(x,bodyTycStamps)
	    then bodyTycTrans x
	    else if member(x,paramTycStamps)
	    then paramTycTrans x
	    else x
	val newparam = shiftStamps(paramStrTrans,paramTycTrans,newParamStamps,
				   ~1,~1,NULLstr)
	    			  param
	val newbody = shiftStamps (transStrStamps,transTycStamps,newBodyStamps,
	    			   pstamp,psign,newparam)
			          body
     in FUNCTOR{paramName=paramName,
		paramVis=paramVis,
		param=newparam,
		body=newbody,
		stamps=newBodyStamps}
    end

end (* structure ModUtil *)
