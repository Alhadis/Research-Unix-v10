(* Copyright 1989 by AT&T Bell Laboratories *)
(* functor.sml *)

structure Functor : FUNCTOR =
struct

open ErrorMsg Stampset Basics TypesUtil EnvAccess BareAbsyn SigMatch ModUtil
     PrintUtil

(* debug print functions *)
val prIntPath = printClosedSequence ("[",",","]") (print:int->unit)
fun prSymPath spath = printSequence "." printSym (rev spath)

fun for l f = app f l

type epath = int list

fun backout([],m) = m
  | backout(_::r,m) = backout(r,0::m)

(* relative(curloc,rootpath) caluculates the relative path from a given location,
   curloc, to the location indicated by a path, rootpath, which will be
   relative to a given root structure, i.e. the functor body structure *)

fun relative(l,y as [_]) = backout(l,y)
  | relative(i::l,y as j::m) = if i=j then relative(l,m) else backout(l,0::y)
  | relative([],y) = y
  | relative _ = impossible "Functor.relative"

(* abstraction of functor body; replacing all bound type constructors
   appearing in body table with relative versions *)

fun abstractBody
      (str : Structure,
       param as STRstr{env=paramenv,
		       stamp=paramstamp,
		       sign=paramsign,
		       kind=SIGkind{stamps={strStamps=paramStrStamps,
					    tycStamps=paramTycStamps},...},...},
       bodyStamps as {strStamps=bodyStrStamps,
		      tycStamps=bodyTycStamps}: stampsets,
       baseSigStamp : int)
      : Structure  =

let val paramStrStampMap = newMap(paramStrStamps,[]: (int * int list) list)
    val paramTycStampMap = newMap(paramTycStamps,[]: int list)
    val bodyStrStampMap  = newMap(bodyStrStamps,[]: (int * int list) list)
    val bodyTycStampMap  = newMap(bodyTycStamps,[]: int list)

    (* initMaps, used to initialize param and body stampMaps *)
    fun initMaps (strStamps,tycStamps,strStampMap,tycStampMap) =
	let fun scanenv (REL{s,t},path) =
		let fun tscan i =
			let val tyc as TYCON{stamp,path=spath,kind,eq,arity} =
				  t sub i
			 in if member(stamp,tycStamps)
			    then if null(applyMap(tycStampMap,stamp))
				 then updateMap tycStampMap (stamp,rev(i::path))
				 else (print " mapped\n")
			    else (print " not bound\n");
			    tscan(i+1)
			end
		    fun sscan i =
			(case s sub i
			  of STRstr{env,stamp,sign,kind,...} =>
			       if member(stamp,strStamps)
			       then (updateMap strStampMap	
				      (stamp,(sign,rev(i::path))
					     ::applyMap(strStampMap,stamp));
				     scanenv(env, i::path))
			       else ()
			   | NULLstr => ()  (* for empty parameter slots *)
			   | _ => impossible "abstractBody.initMap.scanenv.sscan";
			 sscan(i+1))
		 in tscan 0
		    handle Subscript =>
		      sscan 1
		      handle Subscript => ()
		end
	      | scanenv _ = impossible "abstractBody.initMap.scanenv"
	 in scanenv
	end

    val initParamMaps = initMaps(paramStrStamps,paramTycStamps,
				 paramStrStampMap,paramTycStampMap)
    val initBodyMaps  = initMaps(bodyStrStamps,bodyTycStamps,
				 bodyStrStampMap,bodyTycStampMap)

    fun abstractType(path: int list, ty: ty) : ty =
	let fun absty ty =
		case ty
		  of VARty(ref(INSTANTIATED ty')) => absty ty'
		   | FLEXRECORDty(ref(CLOSED ty')) => absty ty'
		   | POLYty{tyfun=TYFUN{body,arity},sign} =>
		      POLYty{tyfun=TYFUN{body=absty body,arity=arity},sign=sign}
		   | VARty _ => ty
		   | CONty(tycref as ref tyc,args) =>
		     CONty(
		      (case tyc
			 of RELtyc p => impossible "abstractBody.abstractType 1"
			      (* was tycref  -- shouldn't happen *)
			  | TYCON{stamp,kind,path=spath,...} =>
			      if member(stamp,paramTycStamps)
			      then 
				ref(RELtyc(relative(path,
					    applyMap(paramTycStampMap,stamp))))
			      else if member(stamp,bodyTycStamps) then
				 let val p = applyMap(bodyTycStampMap,stamp)
				  in if null(p)
				     then (complain("hidden tycon in functor body: ");
					   prSymPath spath; newline(); tycref)
				     else ref(RELtyc(relative(path,p)))
				 end
			      else tycref   (* free tycon in body *)
			  | _ => impossible "abstractBody.abstractType 2"),
		       map absty args)
		   | ERRORty => ty
		   | _ => impossible "abstractBody.abstractType 3"
	 in absty ty 
	end

    (* absLocSig is used for substructures that are instantiations of local
      (open) signature expressions -- it is safe the do the abstractType in place
      because the VALvars, etc. and their typ component refs cannot not be shared *)

    fun absLocSig(str as STRstr{table,stamp,sign,env=REL{s,t},kind},parent,epath) =
	let fun absbnd(VARbind(VALvar{typ,...})) =
		  typ := abstractType(epath,!typ)
	      | absbnd(CONbind(DATACON{typ,...})) =
		  typ := abstractType(epath,!typ)
	      | absbnd(STRbind(STRvar{binding=INDstr i,...})) =
		  let val str' as STRstr{sign,...} = s sub i
		   in if sign > baseSigStamp
		      then absLocSig(str',str,epath@[i])
			   (* substruction also locally constrained *)
		      else ()
		  end
	      | absbnd _ = ()
	 in IntStrMap.app (fn (_,_,b) => absbnd b) table;
	    update(s,0,parent)	
	end
      | absLocSig _ = impossible "absLogSig - bad arg"

    fun strGt((_,_,STRbind(STRvar{binding=STRstr{stamp=stamp1,...},...})),
	      (_,_,STRbind(STRvar{binding=STRstr{stamp=stamp2,...},...}))) =
	stamp1 > stamp2
      | strGt _ = impossible "tycGt"

    fun tycGt((_,_,TYCbind(ref(TYCON{stamp=stamp1,...}))),
	      (_,_,TYCbind(ref(TYCON{stamp=stamp2,...})))) =
	stamp1 > stamp2
      | tycGt _ = impossible "tycGt"

    (* first sort and index all tycons.  This can cause a tycon to be promoted,
       but claim this doesn't matter *)

    fun abstractStr(STRstr{stamp,sign,env=DIR,table,kind},parent,param,epath) =
	  let val binders = IntStrMap.intStrMapToList table
	      val strs = fold (fn (b as (i,s,STRbind _),l) => b::l | (_,l) => l)
			      binders []
	      val tycs = fold (fn (b as (i,s,TYCbind _),l) => b::l | (_,l) => l)
			      binders []
	      val sortedstrs = Sort.sort strGt strs
	      val sortedtycs = Sort.sort tycGt tycs
	      val newtable = Env.newTable()
	      val add = IntStrMap.add newtable
	      val tycCount = ref 0
	      val strCount = ref 2
	      val tenvtemp = ref([]: tycon list)
	      val senvtemp = ref([]: Structure list)
	    (* probably would be ok to update typ in place in abstractBind *)
	      fun abstractBind(i,s,VARbind(VALvar{name,access,typ})) =
		    add(i,s,VARbind(VALvar{name=name,access=access,
					   typ=ref(abstractType(epath,!typ))}))
		| abstractBind(i,s,CONbind(DATACON{name,const,rep,sign,typ})) =
		    add(i,s,CONbind(DATACON{name=name,const=const,rep=rep,sign=sign,
					    typ=ref(abstractType(epath,!typ))}))
		| abstractBind(i,s,b as FIXbind _) = add(i,s,b)
		| abstractBind _ = ()
	   in for sortedtycs
	       (fn (i,s,tb as TYCbind(tycref as ref(TYCON{stamp,...}))) => 
		   if member(stamp,bodyTycStamps)
		   then let val path = applyMap(bodyTycStampMap,stamp)
			 in if null path  (* normal case *)
			    then (updateMap bodyTycStampMap
				    (stamp,rev(!tycCount::epath));
				  add(i,s,TYCbind(ref(INDtyc(!tycCount))));
				  tenvtemp := !tycref :: !tenvtemp;
				  inc tycCount)
			    else add(i,s,TYCbind(ref(SHRtyc(relative(epath,path)))))
			end
		   else if member(stamp,paramTycStamps)
		      (* parameter bound tycon -- e.g. from opened param *)
		   then let val path = applyMap(paramTycStampMap,stamp)
			 in add(i,s,TYCbind(ref(SHRtyc(relative(epath,path)))))
			end
		   else (* fixed tycon - e.g. from opened external structure *)
			add(i,s,tb)
	         | _ => impossible "abstractStr 16");
	      for sortedstrs
	       (fn (i,s,sb as STRbind(STRvar{name,access,
					     binding as STRstr{stamp,sign,...}})) =>

		   let fun addStr(stampMap,paths) =
			(updateMap stampMap
			   (stamp,(sign,rev(!strCount :: epath))::paths);
			 add(i,s,STRbind(STRvar{name=name,access=access,
			 			binding=INDstr(!strCount)}));
			 senvtemp := binding :: !senvtemp;
			 inc strCount)
		       fun look((sign',path)::rest) =
			     if sign = sign'
			     then path
			     else look rest
			 | look [] = []
		    in if member(stamp,bodyStrStamps) then
			  let val paths = applyMap(bodyStrStampMap, stamp)
			      val path = look paths
			   in if null path
			      then addStr(bodyStrStampMap,paths)
			      else add(i,s, STRbind(
					STRvar{name=name,access=access,
				    	       binding=SHRstr(relative(epath,path))}))
			  end
		       else if member(stamp,paramStrStamps) (* parameter structure *)
		       then let val paths = applyMap(paramStrStampMap, stamp)
				val path = look paths
			     in if null path
				then addStr(paramStrStampMap,paths)
				else add(i,s,STRbind(
					STRvar{name=name,access=access,
				    	       binding=SHRstr(relative(epath,path))}))
			    end
		       else (* fixed structure *)
			    add(i,s,sb)
		   end
	         | _ => impossible "abstractStr 17");
	      let val substrs = rev(!senvtemp)
		  val senv  = array(length substrs + 2, NULLstr)
		  val tenv  = arrayoflist(rev(!tenvtemp))
		  val newenv = REL{s=senv,t=tenv}
		  val newstr = STRstr{stamp=stamp,sign=sign,kind=kind,
				      table=newtable,
				      env=newenv}
(* -- possible alternative to revfold call below --
		  fun fillsenv([],i) = ()
		    | fillsenv(str::rest,i) =
			(update(senv,i,abstractStr(str,newstr,NULLstr,i::epath));
			 fillsenv(rest,i+1))
*)
	       in update(senv,0,parent);
		  update(senv,1,param);
		  revfold 
		    (fn (str,i) =>
			(update(senv,i,abstractStr(str,newstr,NULLstr,i::epath));
			 i+1))
		    substrs 2;
		  app abstractBind binders;
		  newstr
	      end
	  end
      | abstractStr(str as STRstr{stamp,sign,env as REL _,...},parent,_,epath) =
	 (if sign > baseSigStamp (* constrained by local, open signature *)
	  then absLocSig(str,parent,epath)
	  else (); (* either constrained by global, closed signature, 
		      or instance of unconstrained functor body --
		      in either case no further abstraction needed *)
	  if member(stamp,bodyStrStamps)
	  then initBodyMaps(env,epath)
	  else ();
	  str)
      | abstractStr _ = impossible "abstractStr - bad arg"
	(* abstracStr *)

     in updateMap paramStrStampMap (paramstamp,[(paramsign,[1])]);
	initParamMaps(paramenv,[1]);	   	(* initialize paramTycStampMap *)
	abstractStr(str,NULLstr,param,[])
    end  (* abstractBody *)
  | abstractBody _ = impossible "abstractBody - bad args"


fun applyFunctor(FUNCTOR{paramName,param,body,paramVis,
    			 stamps={strStamps=bodyStrStamps,
    			 	 tycStamps=bodyTycStamps}},
                 arg,path,globalStamps)
     : Structure * thinning =
    let val STRstr{kind=SIGkind{stamps={strStamps=paramStrStamps,
	    				tycStamps=paramTycStamps},...},...}
	      = param
	val {strStamps=globalStrStamps,tycStamps=globalTycStamps} = globalStamps
	val paramTycMap = newMap(paramTycStamps,NULLtyc) : tycon stampmap
	val paramStrMap = newMap(paramStrStamps,NULLstr) : Structure stampmap
	val bodyTycMap = newMap(bodyTycStamps,NULLtyc) : tycon stampmap
	val bodyStrMap = newMap(bodyStrStamps,NULLstr) : Structure stampmap

	val ptycRealize = updateMap paramTycMap
	val pstrRealize = updateMap paramStrMap
	val btycRealize = updateMap bodyTycMap
	val bstrRealize = updateMap bodyStrMap

	fun maptyc(stamp,tycon) =
	    if member(stamp,paramTycStamps) then ptycRealize(stamp,tycon) else ()
	fun mapstr1(stamp,str) =
	    if member(stamp,paramStrStamps) then pstrRealize(stamp,str) else ()
	fun mapstr(sgn as STRstr{stamp,env=REL{s,t},kind=SIGkind{bindings,...},...},
		   str as STRstr{table,env as env',...}) =
	    let fun mapbinding(STRbind(STRvar{name=[id],binding=INDstr i,...})) =
		      let val sgn' = s sub i
			  val str' = let val STRvar{binding,...} = 
					       lookSTRinTable(table,id)
				      in case binding
					   of INDstr j =>
					       (case env'
					         of REL{s=s',...} => s' sub j
						  | DIR => impossible "mapstr 1")
					    | SHRstr(j::r) =>
					       (case env'
					         of REL{s=s',...} =>
						      getEpath(r,s' sub j)
						  | DIR => impossible "mapstr 2")
					    | st => st
				     end
		       in mapstr(sgn',str')
		      end
		  | mapbinding(TYCbind(ref(INDtyc i))) =
		      let val tycon = t sub i
			  val name = tycName tycon
			  val tycon' = case !(lookTYCinTable(table,name))
					 of INDtyc j =>
					     (case env'
					       of REL{t=t',...} => t' sub j
					        | DIR => impossible "mapstr 3")
					  | SHRtyc p => getEpathTyc(p,env)
					  | tyc => tyc
		       in maptyc(tycStamp tycon,tycon')
		      end
		  | mapbinding _ = ()
	     in if member(stamp,paramStrStamps)
		then (pstrRealize(stamp,str);
		      app mapbinding bindings)
		else ()
	    end
          | mapstr _ = impossible "applyFunctor.mapstr"

	fun insttyc(tyc) =
	    case tyc
	      of TYCON{stamp,arity,eq,path=path',kind} =>
		   if member(stamp,paramTycStamps)
		   then applyMap(paramTycMap, stamp)
		   else if member(stamp,bodyTycStamps) (* generative *)
		   then (case applyMap(bodyTycMap, stamp)
			  of NULLtyc =>  (* not yet instantiated *)
			       let val newkind =
				       case kind
					 of DEFtyc(TYFUN{arity,body}) =>
					      DEFtyc(TYFUN{arity=arity,
							   body=instdef body})
					  | _ => kind
				   val newtyc =
				       TYCON{stamp = newStamp(globalTycStamps),
					     eq = if !eq = YES then eq else ref MAYBE,
					     arity = arity,
					     path = path'@path,
					     kind = newkind}
				in btycRealize(stamp,newtyc);
				   newtyc
			       end
			   | tyc' => tyc')
		   else tyc  (* free type constructor *)
	       | _ => (print "insttyc: "; PrintType.printTycon tyc; print "\n";
		       impossible "Functor.applyFunctor.insttyc")
	and instdef ty =
	    case ty
	      of CONty(ref tycon,args) =>
		   CONty(ref(insttyc tycon), map instdef args)
	       | _ => ty

	fun inststr(str as STRstr{stamp,sign,table,env,kind}) =
	    if member(stamp,paramStrStamps)  (* parameter stamp *)
	    then case applyMap(paramStrMap, stamp)
		  of str' as STRstr{stamp=stamp',sign=sign',...} =>
		       if sign = sign'
		       then str'
		       else STRstr{stamp=stamp',sign=sign,table=table,
			    	   env=instenv(env),
				   kind=case kind
					 of STRkind _ => kind
					  | _ => STRkind{path=[]}}
		   | NULLstr => impossible "inststr NULLstr"
		   | _ => impossible "inststr ???"
	    else if member(stamp,bodyStrStamps)  (* generative stamp *)
	    then case applyMap(bodyStrMap, stamp)
		  of NULLstr  =>
		     let val STRkind{path=path'} = kind
		         val newstr =
			     STRstr{stamp = newStamp(globalStrStamps),
				    sign = sign,
				    table = table,
				    env = instenv(env),
				    kind = STRkind{path=path'@path}}
		      in bstrRealize(stamp,newstr);
			 newstr
		     end
		   | str' as STRstr{stamp=stamp',sign=sign',...} =>
		     if sign = sign'
		     then str'
		     else let val STRkind{path=path'} = kind
			      val newstr =
				  STRstr{stamp = stamp',
					 sign = sign,
					 table = table,
					 env = instenv(env),
					 kind = STRkind{path=path'@path}}
			   in newstr
			  end
		   | _ => impossible "inststr ####d"
	    else str  (* absolute *)
	  | inststr (NULLstr) = NULLstr  (* undefined parameter case *)
	  | inststr (INDstr i) = impossible ("Functor.applyFunctor.instsr: INDstr "
					       ^makestring i)
	  | inststr (SHRstr _) = impossible ("Functor.applyFunctor.instsr: SHRstr")

	and instenv(REL{s,t}: strenv) : strenv =
	    let fun mapi (f,a,start,default) =
		  let val b = array(Array.length a, default)
		      fun loop i = (update(b,i,f(a sub i)); loop(i+1))
		   in loop start handle Subscript => b
		  end
	     in REL{s=mapi(inststr,s,1,NULLstr),t=mapi(insttyc,t,0,NULLtyc)}
	    end
	  | instenv(DIR) = impossible "instenv"

        fun checkeqstr(STRstr{env as REL{s,t},...}) =
	    (ArrayExt.app(defineEqTycon(tyconInContext env), t, 0);
	     ArrayExt.app(checkeqstr,s,2))
          | checkeqstr(STRstr{env as DIR,...}) =
	      impossible "Functor.applyFunctor.checkeqstr 1"
          | checkeqstr(NULLstr) = impossible "Functor.applyFunctor.checkeqstr 2"
          | checkeqstr _ = impossible "Functor.applyFunctor.checkeqstr 3"

	val (argstr,thin) = match0 {mapstr=mapstr,mapstr1=mapstr1,maptyc=maptyc}
			      (false,[paramName],globalStamps,param,arg,NULLstr)
	    (* argstr is the instantiated parameter structure *)
        val xx = inststr(body)
	val str as STRstr{env as REL{s,...},...} = xx (* inststr(body) *)
     in (* fill parameter slot of instantiated body with instantiated parameter *)
	if paramVis
	then update(s,1,argstr)
	else ();
	linkParents str;
	checkeqstr str;
	(str, thin)
    end
    handle Bind => impossible "Functor.applyFunctor -- Bind exception"

end (* structure Functor *)
