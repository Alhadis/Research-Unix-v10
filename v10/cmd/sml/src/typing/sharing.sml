(* Copyright 1989 by AT&T Bell Laboratories *)
(* sharing.sml *)

structure Sharing : SHARING =
struct

  structure Basics = Basics

  open ErrorMsg PrintUtil Basics EnvAccess TypesUtil

  (* a couple of useful iterators *)

  fun for a b = app b a

  fun upto (start,finish) f = 
      let fun loop i = if i>=finish then () else (f i; loop(i+1))
       in loop start
      end

  fun getStr([],str) = str
    | getStr(id::rest,STRstr{table,env,...}) =
	let val STRvar{binding,...} = lookSTRinTable(table,id)
	    val str = case (binding,env)
		       of (INDstr i,REL{s,...}) => s sub i
		        | (SHRstr(i::r),REL{s,...}) => getEpath(r, s sub i)
		        | (STRstr _, _) => binding
			| _ => impossible "Sharing.getStr"
	 in getStr(rest,str)
	end
	handle Env.UnboundTable =>
	  condemn("unbound structure id in sharing specification: "
		  ^ Symbol.name id)

  fun findStr(id::rest,table,env) : Structure =
       (let val STRvar{binding,...} = lookSTRinTable(table,id)
            val str = case (binding,env)
			of (INDstr i,REL{s,...}) => s sub i
			 | (SHRstr(i::r),REL{s,...}) => getEpath(r,s sub i)
			 | (STRstr _, _) => binding
			 | _ => impossible "Sharing.findStr"
	 in getStr(rest,str)
	end
	handle Env.UnboundTable =>  (* look for global structure *)
	  let val STRvar{binding,...} =
		    lookSTR(id) handle Env.Unbound =>
		      condemn("unbound structure id in sharing specification: "
		              ^ Symbol.name id)
	   in getStr(rest,binding)
	  end)
    | findStr([],_,_) = impossible "Sharing.findStr with empty path"

  fun findTycon(path,table,env) : tycon =
      let val (id::rpath) = rev path 
       in case rev rpath
	    of [] => ((case !(lookTYCinTable(table,id))
			 of INDtyc i =>
			      (case env
			        of REL{t,...} => t sub i
				 | DIR => impossible "Sharing.findTycon")
			  | SHRtyc p => getEpathTyc(p,env)
			  | tyc => tyc)
		      handle Env.UnboundTable =>
		      !(lookTYC id) 
		      handle Env.Unbound =>
		      condemn("unbound type in sharing spec: "^ Symbol.name id))
	     | path' => !(lookTYCinStr(findStr(path',table,env),id))
      end
(*
  fun sameStructure(STRstr{env,...}, STRstr{env=env',...}) = (env = env')
    | sameStructure _ = false  

  (* similar to ModUtil.resetParent except for sameStructure test --
     would ModUtil.resetParent suffice? *)
  fun resetParent (newparent: Structure, oldparent: Structure) =
      fn (STRstr{env=REL{s,...},...}) =>
	  ((case s sub 0
	     of NULLstr => update(s,0,newparent)
	      | _ => if sameStructure(s sub 0, oldparent)
		     then update(s,0,newparent)
		     else ())
	   handle Subscript => ())
       | NULLstr => ()
       | _ => impossible "Sharing.resetParent"
*)
  fun doSharing(table,env as REL{s=senv,t=tenv},{strStamps,tycStamps},
		{s=strShare,t=typeShare}) =
      let fun freeStrStamp s = not(Stampset.member(s,strStamps))
          fun freeTycStamp s = not(Stampset.member(s,tycStamps))
	  val {assoc,getassoc,union,find} = Siblings.new(freeStrStamp)
	        : Structure Siblings.siblingClass
	  val {union=tunion,find=tfind} = Unionfind.new(freeTycStamp)

	  fun strMerge(p' as STRstr{stamp=p,...}, q' as STRstr{stamp=q,...}) =
	      if (assoc(p,p'); find p) = (assoc(q,q'); find q)
	      then ()
	      else let val pclass = getassoc p
		       and qclass = getassoc q
		    in union(p,q);
		       for pclass (fn x =>
			 for qclass (fn y =>
			   sMerge(x,y)))
		   end

	  and sMerge(str1 as STRstr{stamp=s1,kind=k1,env=REL{s=senv1,t=tenv1,...},...},
		     str2 as STRstr{stamp=s2,kind=k2,
			     	    env=env2 as REL{s=senv2,t=tenv2,...},
			     	    table,...}) =
	      case (k1,k2)
		of (STRkind _, STRkind _) =>
		      if s1 = s2
		      then ()
		      else condemn "sharing constraint - \
				   \incompatible fixed structures"
		 | (STRkind _, SIGkind _) => sMerge(str2,str1)
		 | (SIGkind{bindings,...},  _) =>
		     for bindings 
		       (fn STRbind(STRvar{name=[id],binding,...}) =>
			    (let val STRvar{binding=target,...} =
				       lookSTRinTable(table,id)
			      in strMerge((case binding
					     of INDstr i => senv1 sub i 
					      | _ => binding),
					  (case target
					     of INDstr i => senv2 sub i
					      | SHRstr(i::r) => getEpath(r,senv2 sub i)
					      | _ => target))
			     end
			     handle Env.UnboundTable => ())
			 | TYCbind(ref tycon) =>
			    (let val tyc1 = case tycon
					      of INDtyc i => tenv1 sub i
					       | _ => tycon
			         val tyc2 =
				       case !(lookTYCinTable(table,tycName tyc1))
					 of INDtyc i => tenv2 sub i 
					  | SHRtyc p => getEpathTyc(p,env2)
					  | tyc => tyc
			      in tunion(tycStamp tyc1,tycStamp tyc2);
				 ()
			     end
			     handle Env.UnboundTable => ())
			 | _ => ())

	  fun shareSig(REL{s,t}) =
	      (upto (2, Array.length s) (fn i =>
		 let val str as STRstr{stamp,sign,table,env as REL{s=s',...},kind} =
			   s sub i
		  in case kind
		      of SIGkind _ => 
			   let val stamp' = find stamp
			    in if stamp' = stamp
			       then ()
			       else let val new =
					   STRstr{stamp=stamp',sign=sign,
						  table=table,
						  env=env,kind=kind}
				     in update(s,i,new);
					ArrayExt.app(ModUtil.resetParent new,s',1)
				    end;
			       shareSig env
			   end
		       | STRkind _ => impossible "Sharing.doSharing.shareSig"
		 end);
	       upto (0,Array.length t) (fn i =>
		 let val tycon = t sub i
		     val stamp = tycStamp tycon
		     val stamp' = tfind stamp
		  in if stamp = stamp'
		     then ()
		     else update(t,i,setTycStamp(stamp',tycon))
		 end))

	  val strPathPairs = ref [] : (spath*spath) list ref
	  val typePathPairs = ref [] : (spath*spath) list ref

       in for strShare (fn p as (p1,p2) =>
	    let val str1 as STRstr{stamp=s1,...} = findStr(p1,table,env)
		and str2 as STRstr{stamp=s2,...} = findStr(p2,table,env)
	     in if freeStrStamp s1 orelse freeStrStamp s2 then ()
		  else strPathPairs := p :: !strPathPairs;
		strMerge(str1,str2)
	    end);
	  for typeShare (fn p as (p1,p2) =>
	    let val s1 = tycStamp(findTycon(p1,table,env))
		and s2 = tycStamp(findTycon(p2,table,env))
	     in if freeTycStamp(s1) orelse freeTycStamp(s2)
	        then ()
		else typePathPairs := p :: !typePathPairs;
	        tunion(s1,s2)
	    end);
	  shareSig env;
	  {s= !strPathPairs, t= !typePathPairs}
      end  (* doSharing *)

  fun checkSharing(table,env,{s=strShare,t=typeShare}) =
      (for strShare (fn p as (p1,p2) =>
	 let val STRstr{stamp=s1,...} = findStr(p1,table,env)
	     and STRstr{stamp=s2,...} = findStr(p2,table,env)
	  in if s1 <> s2
	     then condemn "structure sharing violation"
	     else ()
	 end);
       for typeShare (fn (p1,p2) =>
	 let val tyc1 = findTycon(p1,table,env)
	     and tyc2 = findTycon(p2,table,env)
	  in if equalTycon(tyc1,tyc2)
	     then ()
	     else (PrintType.printTycon tyc1; print "\n";  (* DEBUGGING *)
		   PrintType.printTycon tyc2; print "\n";  (* DEBUGGING *)
		   condemn "type sharing violation")
	 end))

end (* structure Sharing *)
