(* Copyright 1989 by AT&T Bell Laboratories *)
(* printtype.sml *)

signature PRINTTYPE = sig
  structure Basics : BASICS
  val typeFormals : int -> string list
  val printTyvar : Basics.tyvar -> unit
  val printTycon : Basics.tycon -> unit
  val printType : Basics.ty -> unit
  val resetPrintType : unit -> unit
end

structure PrintType : PRINTTYPE = struct

structure Basics = Basics

open ErrorMsg Basics List2 PrintUtil

val boundTyvarNames: string array =
    let val names = array(50,"")
	fun fill k =
	    (update(names,k,
	    	    if k < 26
		    then chr(k+ord"a")
		    else chr(k div 26 + ord"a") ^ chr(k mod 26 + ord"a"));
             fill(k+1))
     in fill 0 handle Subscript => ();
	names
    end

val metaTyvarNames: string array = arrayoflist
      ["S","T","U","V","W","X","Y","Z","SS","TT","UU","VV","WW","XX","YY","ZZ",
       "S1","T1","U1","V1","W1","X1","Y1","Z1","S2","T2","U2","V2","W2","X2",
       "Y2","Z2"]

fun typeFormals n =
    let fun loop i =
	if i>=n then []
	else (boundTyvarNames sub i)::loop(i+1)
     in loop 0
    end

local  
  val count = ref(~1)
  val metaTyvars = ref([]:tyvar list)
in
  fun metaTyvarName(tv: tyvar) =
      let fun find([],_) =
	        (metaTyvars := tv::(!metaTyvars);
		 inc count;
		 !count)
	    | find(tv'::rest,k) =
	        if tv = tv'
		then !count - k
		else find(rest,k+1)
       in metaTyvarNames sub (find(!metaTyvars,0))
      end
  fun resetPrintType() =
      (count := ~1; metaTyvars := [])
end

val internals = System.Control.internals

fun printTyvar1(tv as (ref kind): tyvar, sign: {weakness:int, eq:bool} list) : unit =
     case kind
       of IBOUND n =>
	    (let val {weakness=w,eq=e} = nth(sign,n) 
				         handle Nth => {weakness=infinity,eq=false}
	      in if e then print "''" else print "'";
	         if w < infinity
		 then if !System.Control.weakUnderscore
		      then print "_"
		      else print w
		 else ();
		 print(boundTyvarNames sub n)
	     end)
	| META{depth,weakness,eq} => 
	    (if eq then print "''" else print "'";
	     if weakness < infinity then print weakness else ();
	     print(metaTyvarName tv);
	     if !internals andalso depth < infinity
	     then (print "["; print depth; print "]")
	     else ())
	| UBOUND{name,...} =>
	    (printSym name; print "U")
	| INSTANTIATED _ => (print "'"; print(metaTyvarName tv); print "I")
	    
fun printTyvar(tv: tyvar) = printTyvar1(tv,[])

fun prkind (ABStyc) = print "ABS/"
  | prkind (DEFtyc _) = print "DEF/"
  | prkind (DATAtyc _) = print "DATA/"
  | prkind (RECORDtyc _) = print "RECORD/"

fun printPath(p,stamp) =
    let fun get([id],STRstr{table,env,...}) =
	      (case !(EnvAccess.lookTYCinTable(table,id))
		of INDtyc i =>
		     (case env
		       of REL{t,...} => t sub i
		        | DIR => impossible "printPath.get 1")
		 | SHRtyc p => TypesUtil.getEpathTyc(p,env)
		 | tyc => tyc)
	  | get(id::rest,STRstr{table,env,...}) =
	      let val STRvar{binding,...} = EnvAccess.lookSTRinTable(table,id)
		  val str' = case (binding,env)
			       of (INDstr i,REL{s,...}) => s sub i 
			        | (SHRstr (i::r),REL{s,...}) =>
				    TypesUtil.getEpath(r,s sub i)
				| (STRstr _, _) => binding
				| _ => impossible "printPath.get 2"
	       in get(rest,str')
	      end
	  | get _ = impossible "get in printPath"
	fun try(name::untried,[]) = 
	     (let val tyc = !(EnvAccess.lookTYC(name))
	       in if (TypesUtil.tycStamp tyc = stamp)
		  then printSym name
		  else try(untried,[name])
	      end
	      handle Env.Unbound => try(untried,[name])
	           | Env.UnboundTable => try(untried,[name]))
	  | try(name::untried,tried) =
	     (let val STRvar{binding,...} = EnvAccess.lookSTR name
		  val tyc = get(tried,binding)
	       in if (TypesUtil.tycStamp tyc = stamp)
		  then printSequence "." printSym (name::tried)
		  else try(untried,name::tried)
	      end
	      handle Env.Unbound => try(untried,name::tried)
	           | Env.UnboundTable => try(untried,name::tried))
	  | try([],tried) = (print "?."; printSequence "." printSym tried)
     in try(p,[])
    end
    handle Env.UnboundTable => impossible "UnboundTable escapes printPath"
         | e => impossible "other exception escaping printPath"

val TYCON{stamp=arrowStamp,...} = !BasicTypes.arrowTycon

fun strength(ty) =
    case ty
      of VARty(ref(INSTANTIATED ty')) => strength(ty')
       | FLEXRECORDty(ref(CLOSED ty')) => strength(ty')
       | CONty(ref tycon, args) =>
	   (case tycon
	      of TYCON{kind=ABStyc, stamp,...} => 
		   if stamp = arrowStamp then 0 else 2
	       | TYCON{kind=RECORDtyc labels,...} =>
		   if Tuples.isTUPLEtyc(tycon) then 1 else 2
	       | _ => 2)
       | _ => 2

fun printTycon(TYCON{kind=RECORDtyc(labels),...}) =
       printClosedSequence("{",",","}") printSym labels
  | printTycon(TYCON{kind=UNDEFtyc _,path=name::_,...}) =
      (printSym name; print "?")
  | printTycon(TYCON{path,stamp,kind=DEFtyc(TYFUN{body,...}),...}) =
      (if !internals
       then (print "DEF/"; print stamp; print "["; printType body; print "]")
       else ();
       printPath(path,stamp))
  | printTycon(TYCON{path,stamp,kind,...}) =
      (if !internals then (prkind kind; print stamp; print "/") else ();
       printPath(path,stamp))
  | printTycon(INDtyc i) =
      (print "IND/"; print i; print "/")
  | printTycon(SHRtyc p) =
      (print "SHR/"; printSequence "," print p; print "/")
  | printTycon(RELtyc p) =
      (print "REL/"; printSequence "," print p; print "/")

and printType1(ty: ty, sign: {weakness:int,eq:bool} list) : unit =
    let fun prty ty =
	    case ty
	      of VARty(ref(INSTANTIATED ty')) => prty(ty')
	       | VARty(tv) => printTyvar1(tv,sign)
	       | CONty(ref tycon, args) =>
		   (case tycon
		      of TYCON{kind=ABStyc, stamp,...} => 
			   if stamp = arrowStamp
			   then let val [domain,range] = args
				 in if strength domain = 0
				    then (print "("; prty domain; print ")")
				    else prty domain;
				    print " -> ";
				    prty range
				end
			   else (printTypeArgs args; printTycon tycon)	
		       | TYCON{kind=RECORDtyc labels,...} =>
			   if Tuples.isTUPLEtyc(tycon)
			   then printTUPLEty args
			   else printRECORDty(labels, args)
		       | _ => (printTypeArgs args; printTycon tycon))
	       | POLYty{sign,tyfun=TYFUN{arity,body}} => printType1(body,sign)
	       | FLEXRECORDty(ref(CLOSED ty)) => prty ty
	       | FLEXRECORDty(ref(OPEN fields)) => printFields fields
	       | ERRORty => print "error"
	       | UNDEFty => print "undef"

	and printTypeArgs [] = ()
	  | printTypeArgs [ty] = 
	     (if strength ty <= 1
	      then (print "("; prty ty; print ")")
	      else prty ty;
	      print " ")
	  | printTypeArgs tys = printClosedSequence ("(", ",", ") ") prty tys

	and printTUPLEty [] = print "unit"
	  | printTUPLEty tys = 
	      printSequence " * " 
		(fn ty => if strength ty <= 1
			  then (print "("; prty ty; print ")")
			  else prty ty)
	        tys

	and printField(lab,arg) = (printSym lab; print ":"; prty arg)

	and printRECORDty([],[]) = print "unit"
	  | printRECORDty(lab::labels, arg::args) =
	      (print "{";
	       printField(lab,arg);
	       app2 
		 (fn field => (print ","; printField field))
		 (labels,args);
	       print "}")
	  | printRECORDty _ = ErrorMsg.impossible "PrintType.printRECORDty"

	and printFields [] = print "{...}"	
	  | printFields (field::fields) =
	      (print "{";
	       printField field;
	       app 
		 (fn field => (print ","; printField field))
		 fields;
	       print ",...}");

     in prty ty
    end  (* printType1 *)

and printType(ty: ty) : unit = printType1(ty,[])
    
end (* structure PrintType *)
