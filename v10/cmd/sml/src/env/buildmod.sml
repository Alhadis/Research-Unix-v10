(* Copyright 1989 by AT&T Bell Laboratories *)
(* buildmod.sml *)

(* building structures *)

structure BuildMod : BUILDMOD =
struct

open ErrorMsg Symbol Access Basics TypesUtil Env NameSpace

(* ordering of binders -- comparing by bound symbol for runtime components *)
fun binderGt(bind1: binder, bind2: binder) =
    case (bind1,bind2)
      of ((ind1,_,FIXbind(_)),(ind2,_,FIXbind(_))) => ind1 > ind2
       | ((_,_,FIXbind(_)),_) => true
       | (_,(_,_,FIXbind(_))) => false
       | ((_,n1,VARbind(_)),(_,n2,VARbind(_))) => n1 > n2
       | ((_,_,VARbind(_)),_) => true
       | (_,(_,_,VARbind(_))) => false
       | ((_,n1,CONbind(_)),(_,n2,CONbind(_))) => n1 > n2
       | ((_,_,CONbind(_)),_) => true
       | (_,(_,_,CONbind(_))) => false
       | ((ind1,_,TYCbind(_)),(ind2,_,TYCbind(_))) => ind1 > ind2
       | ((_,_,TYCbind(_)),_) => true
       | (_,(_,_,TYCbind(_))) => false
       | ((_,n1,STRbind(_)),(_,n2,STRbind(_))) => n1 > n2
       | ((_,_,STRbind(_)),_) => true
       | (_,(_,_,STRbind(_))) => false
       | ((ind1,_,FCTbind(_)),(ind2,_,FCTbind(_))) => ind1 > ind2
       | ((_,_,FCTbind(_)),_) => true
       | (_,(_,_,FCTbind(_))) => false
       | ((ind1,_,SIGbind(_)),(ind2,_,SIGbind(_))) => ind1 > ind2
       | _ => impossible "EnvAccess.binderGt"

fun extendPath(LVAR(v): access, []: path) = PATH[v] (* locally defined *)
  | extendPath(SLOT(n), p) = PATH(n::p)  (* element of opened structure *)
  | extendPath(x as PATH _, _) = x  (* defined exception *)
  | extendPath(x as INLINE _, _) = x
  | extendPath(access,path) = impossible "extendPath in envaccess"

fun dconInStr(dc as DATACON{name,const,typ,rep,sign},env,slotNo) : datacon =
    DATACON{name = name, const = const, sign = sign,
            rep = (case rep
		     of VARIABLE(access) => VARIABLE(SLOT slotNo)
		      | _ => rep),
            typ = ref(typeInContext(!typ,env))}

fun last (x as [_]) = x | last(a::b) = last b | last [] = impossible "last"

fun buildStrTable () : trans list * symtable =
    let val newtable = newTable()
	val add = IntStrMap.add newtable
	val look = IntStrMap.map newtable
	fun getBindings() =
	    (* no sorting done, except chronological by collectTable *)
	    let val r = ref (nil : (binder * info) list)
		fun add x = r := x :: !r
	     in collectTable add;
		!r
	    end
	fun fill (nil,count) = nil
	  | fill ((bdg as (i,s,binding),{path,strenv})::rest,count) =
	       (look(i,s); fill(rest,count)) handle UnboundTable =>	
	     case binding
	      of VARbind(var as VALvar{access,name,typ}) =>
		   (add(i,s,
		        VARbind(
			  case access
			   of INLINE(_) => var
			    | _ =>
				VALvar{access = SLOT count,
				       typ = ref(typeInContext(!typ,strenv)),
				       name = last name}));
		    VALtrans(extendPath(access,path))::fill(rest,count+1))
	       | CONbind(exn as DATACON{rep=VARIABLE(access),...}) =>
		   (add(i,s,CONbind(dconInStr(exn,strenv,count)));
		    VALtrans(extendPath(access,path))::fill(rest,count+1))
	       | STRbind(STRvar{name,access,binding}) =>
		   let val newbinding =
			   case binding
			    of INDstr i =>
			       (case strenv
			         of REL{s=senv,...} => senv sub i
				  | DIR => impossible "buildStrTable.fill 1")
			     | SHRstr(i::r) =>
			       (case strenv
			         of REL{s=senv,...} => getEpath(r,senv sub i)
				  | DIR => impossible "buildStrTable.fill 2")
			     | _ => binding
		    in add(i,s, STRbind(STRvar{name=last name,
				               binding=newbinding,
				               access=SLOT(count)}));
		       VALtrans(extendPath(access,path))::fill(rest,count+1)
		   end
	       | TYCbind(tyconRef) =>
		   (add(i,s,TYCbind(ref(tyconInContext strenv (!tyconRef))));
		    fill(rest,count))
	       | CONbind(dcon) =>
		   (add(i,s,CONbind(dconInStr(dcon,strenv,0)));
		    fill(rest,count))
	       | _ => (add bdg; fill(rest,count))
     in (fill(getBindings(),0), newtable)
    end

end (* structure BuildMod *)
