(* parse.sml *)

structure Parse : PARSE =
struct

structure BareAbsyn = BareAbsyn

exception Eof

fun interdec (lex as {nextToken, prompt, advance}: Lex.lexer ) =
let 

  open ErrorMsg Symbol PrintUtil Lex
  open Token
  open Access Basics BasicTypes TypesUtil Absyn
  open Env
  open EnvAccess
  open ModUtil
  open SigMatch
  open FirstSets
  open Misc

  infix -->

(* constants *)

val maxTypSpecs = 100  (*maximum number of type specs in a signature *)
val maxStrSpecs = 100  (*maximum number of structure specs in a signature *)

(* utility functions *)

fun at tok = if !nextToken = tok then (advance(); true) else false

fun checkToken tok =
    if at(tok)
    then ()
    else complain("expected "^Token.tokenName tok^
			    ", found "^Token.tokenName(!nextToken))

fun getSymbol () = case !nextToken of
		     Token.ID s => (advance(); s)
                   | Token.ASTERISK => (advance(); ASTERISKsym)
		   | Token.EQUAL => (advance(); EQUALsym)
		   | Token.TYVAR s => (advance(); s)
		   | Token.IDDOT s => (advance(); s)
		   | tok => ErrorMsg.impossible("getSymbol: " ^ 
					     Token.tokenName tok)
fun expop () =
    case !nextToken
      of EQUAL => lookFIX(EQUALsym)
       | ASTERISK => lookFIX(ASTERISKsym)
       | ID s => lookFIX(s)
       | _ => NONfix

fun patop () =
    case !nextToken
      of ASTERISK => lookFIX (ASTERISKsym)
       | ID s => lookFIX(s)
       | _ => NONfix

fun ident() =
    case !nextToken
      of ID s => (advance();s)
       | ASTERISK => (advance();ASTERISKsym)
       | EQUAL => (advance();EQUALsym)
       | tok => (complain("expected identifier, found " ^ tokenName tok);
		 bogusID)

fun nonfix_ident() =
	if (case !nextToken of
	      ID s => lookFIX(s)=NONfix
	    | ASTERISK => lookFIX(ASTERISKsym)=NONfix
	    | _ => false)
	 then getSymbol()
	 else (complain("expected nonfix-identifier, found "
			  ^ tokenName(!nextToken));
		 bogusID)
    
fun opid() =
    case !nextToken
      of ID s	=> nonfix_ident()
       | ASTERISK => nonfix_ident()
       | OP	=> (advance(); 
		    case !nextToken
		     of ID s => getSymbol()
		      | ASTERISK => getSymbol()
		      | EQUAL => getSymbol()
		      | tok => (complain ("op not followed by identifier, found "
				        ^ tokenName tok); bogusID))
       | tok => (complain("expected identifier or OP, found " ^ tokenName tok);
		 bogusID)

fun getSTR id = lookSTR id
		 handle Unbound => 
		    (complain("unbound structure name: " ^ name id);
		     bogusSTR)

fun getEXN id = lookCON(id) handle Unbound => unboundEXN id

fun rightAssoc(elem:(unit->'a), tok:token, cons:('a*'b->'b), single:('a->'b))
    : 'b =
    let fun ra() = 
            let val e1 = elem()
            in if at(tok) then cons(e1,ra()) else single e1
            end
    in  ra()
    end;

fun leftAssoc(elem, tok, cons, single) =
    let fun la e = if at tok then la(cons(e,elem())) else single e
     in la(elem())
    end

fun precedence(elem,g,checkop) =
     let fun parse(f, bp, e) =
	    case checkop()
	     of INfix(lbp,rbp) =>
	        if lbp > bp
		 then let val id = getSymbol()
		          val rhs = parse ((fn x=>g(id,e,x)),rbp,elem())
		       in parse(f,bp,rhs)
		      end
	         else f e
	      | _ => f e
      in parse((fn x=>x), ~100, elem())
     end

fun andList(elem) =
    let val e1 = elem()
     in (if at(AND) then e1 :: andList(elem) else [e1])
    end

fun andListProtect(elem) = andList (fn () => protect(protectScope,elem))

(* parsing functions *)

(* qualified id interpretation *)

fun symPath() =
    case !nextToken
      of IDDOT s => getSymbol() :: symPath()
       | ID s => [getSymbol()]
       | ASTERISK => [getSymbol()]
       | EQUAL => [getSymbol()]
       | _ => (complain "incomplete qualified identifier"; [bogusID])

fun qid(lookLast) = lookPath(symPath(),lookLast)

(* record labels *)

fun selector() =
    let fun sel1 id = 
	    let val v = namedLvar id
		val tyref = ref UNDEFty
		val v1 = VALvar{name=[id],access=LVAR(v),typ=tyref}
		val v2 = VALvar{name=[id],access=PATH[v],typ=tyref}
	     in FNexp[RULE(RECORDpat{fields=[(id,VARpat v1)],
				     flex=true,
				     typ=ref UNDEFty, pats=ref nil},
			   VARexp(ref v2))]
	    end
     in case !nextToken
	  of ID _ => sel1(ident())
	   | INT i => let val s = makestring i
		       in if i < 1
			  then complain ("nonpositive integer label in selector,\
			                 \ found " ^ s)
			  else ();
			  sel1(Symbol.symbol(s))
		      end
	              before advance()
	   | _ => (complain "illegal selector function"; bogusExp)
    end

fun labels(parseOne, separator, dotsOK, abbrev) =
    if (case !nextToken
          of ID _ => true
	   | INT _ => true
	   | DOTDOTDOT => true
	   | _ => false)
    then let fun lablist () = 
	         case !nextToken
		   of ID _ => field(ident(),abbrev)
		    | INT i => let val s = makestring i
			       in advance();
				  if i < 1
				  then complain ("nonpositive integer label, \
						 \found " ^ s)
				  else ();
				  field(Symbol.symbol(s),
				          (fn id =>
					    condemn("numeric label abbreviations allowed only in patterns: " ^
						    Symbol.name id)))
			       end
		    | DOTDOTDOT => nil
		    | tok => (complain("expected label, found " ^
					tokenName tok); nil)
	     and field(id,abbrev) =
		 (id,
		  if at(separator) then parseOne()
		    else if !nextToken = COMMA orelse
			    !nextToken = COLON orelse
			    !nextToken = AS orelse
			    !nextToken = RBRACE then abbrev(id)
		    else condemn("expected " ^ Token.tokenName separator ^
			         " after label, found " ^
				 tokenName(!nextToken)),
		  ref 0)
		 :: (if at(COMMA) then lablist() else nil)
	     val l = lablist()
	     val dots = at(DOTDOTDOT)
	     val sl = sort3 l
	  in if length l > length sl
	     then complain "duplicate label in record"
	     else ();
	     if dots andalso not dotsOK
		then complain "use of ... outside pattern" else ();
	     checkToken(RBRACE);
	     (l,sl,dots)
	 end
    else (checkToken(RBRACE); (nil,nil,false))

exception Clausal of symbol * pat   (* for returning clausal patterns from pat *)


(* types *)

fun noAbbrev(_) = 
    (complain "expected colon after label in record type, found comma";
     UNDEFty)	    
fun ty() =
    rightAssoc(ty1,ARROW,
	       (fn (t,ts) => CONty(arrowTycon, [t,ts])),
	       (fn t => t))
and ty1() = 
    case rightAssoc(ty2,ASTERISK,op::,single)
      of [t] => t
       | l => tupleTy l
and ty2() =
    (* incorporates tyapp and aty nonterminals *)
    let	fun qid_s(t) =
	    case !nextToken
	      of ID _ =>
		   qid_s(CONty(!lookArTYC(getSymbol(),1), [t]))
	       | IDDOT _ =>
		   qid_s(CONty(!lookPathArTYC(symPath(),1), [t]))
	       | _ => t
     in qid_s(case !nextToken
	       of LPAREN =>
		    let val t1 = (advance(); ty())
		     in if at(RPAREN)
			then t1
			else if at(COMMA)
			then let val tys = t1 :: ty_pc()
				 val arity = length tys
			      in checkToken(RPAREN);
				 case !nextToken
				  of ID s =>
				      CONty(!lookArTYC(ident(),arity),
					    tys)
				   | IDDOT s => 
				      CONty(!lookPathArTYC(symPath(),arity),
					    tys)
				   | tok => condemn("expected type \
					    \constructor, found "
					    ^ tokenName tok)
			     end
			else (complain("expected RPAREN or COMMA in type\
				\args, found " ^ tokenName(!nextToken));
			      t1)
		    end
		| ID s	=> CONty(!lookArTYC(ident(),0),[])
		| IDDOT s => CONty(!lookPathArTYC(symPath(),0),[])
		| Token.TYVAR s => VARty(lookTyvar(getSymbol()))
		| LBRACE =>
		    (advance();
		     let val (l,sl,_) = labels(ty,COLON,false, noAbbrev)
		      in recordTy(map (fn (id,ty,_) => (id, ty)) sl)
		     end)
		| tok => condemn("expected a type expression, found token "
				 ^ tokenName tok))
    end
and ty_pc() = rightAssoc(ty,COMMA,op::,single)


fun markexp f x = if !System.Control.Debug.debugging
		 then let val one = (!ErrorMsg.fileName,!ErrorMsg.lineNum)
		      val e = f x
		      val two = (!ErrorMsg.fileName,!ErrorMsg.lineNum)
		   in case e
			of MARKexp _ => e
		         | e' => if one=two then MARKexp(e',one,one)
					    else MARKexp(e',one,two)
		  end
		else f x

fun markdec f x =  if !System.Control.Debug.debugging
		 then let val one = (!ErrorMsg.fileName,!ErrorMsg.lineNum)
		      val e = f x
		      val two = (!ErrorMsg.fileName,!ErrorMsg.lineNum)
		   in case e
			of MARKdec _ => e
		         | e' => if one=two then MARKdec(e',one,one)
					    else MARKdec(e',one,two)
		  end
		else f x


(* expressions -- including local declarations *)

fun exp (stamps: Stampset.stampsets) =
    case !nextToken
     of FN => (advance(); FNexp(match(stamps)))
      | CASE => CASEexp((advance(); exp(stamps)),
			(checkToken(OF); match(stamps)))
      | WHILE => WHILEexp((advance(); exp(stamps)),
			  (checkToken(DO); markexp exp stamps))
      | IF => IFexp((advance(); exp(stamps)), (checkToken(THEN); markexp exp stamps),
       	            (checkToken(ELSE); markexp exp stamps))
      | RAISE => RAISEexp(advance(); exp(stamps))
      | _ => let val e = exp1(stamps)
              in if !nextToken = HANDLE
		 then (advance(); HANDLEexp(e,HANDLER(FNexp(match(stamps)))))
		 else e
             end

and match (stamps) = rightAssoc((fn () => rule stamps),BAR,op::,single)

and rule (stamps) = 
     let val bl = ref nil : (symbol * var) list ref
      in protect(protectScope,
	    (fn () => RULE(pat(bl,true)
		           handle Clausal(id,_) => 
			     condemn("undefined op in pattern: "^name id),
		           (checkToken(DARROW);
			    if !nextToken=EQUAL then advance() else ();
(* Capitalization convention
		            app checkBinding (!bl);
*)
			    bindVARs(!bl); markexp exp stamps))))
     end

and exp_ps (stamps) = rightAssoc((fn () => exp stamps),SEMICOLON,op::,single)

and exp1 (stamps) = leftAssoc((fn () => markexp exp2 stamps),
			      ORELSE,ORELSEexp,(fn x=>x))

and exp2 (stamps) = leftAssoc((fn () => markexp exp3 stamps),
			      ANDALSO,ANDALSOexp,(fn x=>x))

 (* N.B. above markexp's will cause too much marking, but this is harmless*) 

and exp3(stamps) =
    let val e = precedence((fn () => markexp exp5 stamps), 
 		(fn(id,a,b)=>APPexp(markexp lookID(id), 
 				    markexp TUPLEexp[a,b])), expop)
     in if at(COLON) then CONSTRAINTexp(e,ty()) else e
    end

and exp5 (stamps) =
     let fun loop e =
	     if firstAexp lookFIX (!nextToken)
	     then loop(markexp APPexp(e,markexp aexp stamps))
	     else e
      in loop(markexp aexp stamps)
     end

(* note that IF WHILE CASE RAISE FN  are matched below, but
   are not in firstAexp.  This is intentional *)

and aexp (stamps) =
     case !nextToken
       of ID _	 => lookID(nonfix_ident())
        | OP	 => lookID(opid())
	| IDDOT s  => qid(lookIDinStr)
        | INT i	 => INTexp(i) before advance()
        | REAL s => REALexp(s) before advance()
        | STRING s => STRINGexp(s) before advance()
	| HASH => (advance(); selector())
        | LBRACE => (advance(); exp_brace(stamps))
        | LPAREN => (advance(); exp_paren(stamps))
        | LBRACKET => (advance(); exp_bracket(stamps))
        | LET	 => 
	    protect(protectScope,
		    (fn()=>(advance();
		     	   (LETexp(ldecs([],stamps),
			           (checkToken(IN); SEQexp(exp_ps(stamps)))))
			   before checkToken(END))))
        | FN =>   exp(stamps)
        | CASE => exp(stamps)
        | WHILE => exp(stamps)
        | IF => exp(stamps)
	| RAISE => exp(stamps)
        | tok	 => (complain ("atomic expression expected, found " ^
			       tokenName tok);
	            bogusExp)

and exp_brace (stamps) =
    let val (l,sl,_) =
	    labels((fn () => exp stamps),EQUAL,false,
	           (fn x => (complain "illegal record-name element abbreviation";
		             bogusExp)))
	fun assign (i,(_,_,r)::tl) = (r:=i; assign(i+1,tl))
	  | assign (n,nil) = ()
     in assign(0,sl);
	RECORDexp(map (fn (id,e,ref n) => (LABEL{name=id,number=n},e)) l)
    end

and exp_paren (stamps) =
     if at(RPAREN)
        then unitExp (* TUPLEexp(nil) *)
        else let val e = exp(stamps)
              in case !nextToken
		   of RPAREN => (advance(); e)
		    | COMMA =>
		       (advance();
			TUPLEexp(e::exp_pc(stamps)) before checkToken(RPAREN))
		    | SEMICOLON =>
		       (advance();
			SEQexp(e::exp_ps(stamps)) before checkToken(RPAREN))
		    | tok => (complain ("expected comma, right paren, or\
			     \ semicolon, found " ^ tokenName tok); e)
             end

and exp_bracket (stamps) =
     if at(RBRACKET)
        then LISTexp(nil)
        else LISTexp(exp(stamps) ::
              if !nextToken = RBRACKET
	       	 then (advance(); nil)
       		 else (checkToken(COMMA);
       		       exp_pc(stamps) before checkToken(RBRACKET)))

and exp_pc (stamps) = rightAssoc((fn () => exp stamps),COMMA,op::,single)

and pat (bl: (symbol * var) list ref, full: bool) =
    (* full false means parse atomic pattern *)
   let fun restrictLAYEREDpat(x as (VARpat _, _)) = LAYEREDpat x
         | restrictLAYEREDpat(y,z) =
	      (complain "pattern to left of AS must be a variable"; z)

       fun pat0 () = rightAssoc(pat1,AS,restrictLAYEREDpat,(fn x=>x))

       and pat1 () = 
	   let val e = precedence(
		         pat2, 
		         (fn(id,a,b)=>
			    APPpat(lookCON id, TUPLEpat[a,b])
			    handle Unbound => 
			      raise Clausal(id, TUPLEpat[a,b])),
			 patop)
	    in if at(COLON) then CONSTRAINTpat(e,ty()) else e
	   end

       and pat2 () =
	   let fun useCon(dcon as DATACON{const,name,...}) =
		    case (const,firstApat lookFIX (!nextToken))
		      of (true,false) => CONpat(dcon)
		       | (false,true) => APPpat(dcon,apat())
		       | (_,x) => (complain("improper use of constructor "^
			              Symbol.name(name)^" in pattern");
				   (if x then (apat(); ()) else ());
				   WILDpat)
	       fun simpleId(id) =
		   useCon(lookCON id)
		   handle Unbound => 
		     if firstApat lookFIX (!nextToken)
		       then raise Clausal(id, apat())
		       else VARpat(newVAR(bl,id))
	    in case !nextToken
		 of ID s => (if lookFIX(s) = NONfix
			    then ()
			    else complain("pattern starts with infix: "
					 ^ name(s));
			   simpleId(getSymbol()))
		  | OP => simpleId(opid())
		  | IDDOT s => useCon(qid lookCONinStr)
		  | _ => apat()
	   end

       and pat_id(id) = 
	   (case lookCON id
	     of	dcon as DATACON{const=true,...} => CONpat(dcon)
	      | _ => (complain("nonconstant data constructor: " ^ name(id));
		      WILDpat))
	   handle Unbound => VARpat(newVAR(bl,id))

       and apat() = 
	   case !nextToken
	     of OP	=> pat_id(opid())
	      |	ID s	=> pat_id(nonfix_ident())
	      | IDDOT s   => CONpat(qid(lookCONinStr))
	      |	INT i	=> INTpat(i) before advance()
	      | REAL s  => REALpat(s) before advance()
	      | STRING s => STRINGpat(s) before advance()
	      | WILD	=> (advance(); WILDpat)
	      |	LPAREN =>  (advance(); pat_paren())
	      |	LBRACKET => (advance(); pat_bracket())
	      |	LBRACE =>  (advance(); pat_brace())
	      | tok => (complain("expected an atomic pattern, found " 
			         ^ tokenName tok); WILDpat)

       and pat_paren () =
	    if at(RPAREN)
	       then unitPat
	       else let val p = pat0()
		     in case !nextToken of
			    RPAREN => (advance(); p)
			  | COMMA =>
			     (advance();
			      TUPLEpat(p::pat_pc()) before checkToken(RPAREN))
			  | tok => (complain ("expected right paren or comma\
				   \ (in pattern), found " ^ tokenName tok);
				  p)
		    end

       and pat_bracket () =
	   LISTpat(if at(RBRACKET)
		     then nil
		     else pat_pc() before checkToken(RBRACKET))

(* bug:  we allow  {a,b,c} to stand for {a=a,b=b,c=c} but we don't
    allow {a as zzz} to stand for {a=a as zzz}
*)

       and pat_id_as id =
	    let val e = pat_id id
                val e' = if at(COLON) then CONSTRAINTpat(e,ty()) else e
	     in if at(AS) then LAYEREDpat(e',pat0()) else e'
	    end

       and pat_brace () =
	   let val (_,sl,dots) = labels(pat0,EQUAL,true,pat_id_as)
	    in RECORDpat{
	         fields = map (fn(id,pat,_) => (id,pat)) sl,
		 flex = dots,
		 typ = ref UNDEFty,
		 pats = ref nil}
	   end

       and pat_pc() = rightAssoc(pat0,COMMA,op::,single)

    in if full then pat0() else apat()
   end

(* variable bindings *)

and recdec x = VALRECdec(rvb_pa x)
and valdec x = VALdec(vb_pa x)
and vb x = markdec (if at(REC) then recdec else valdec) x    

and vb_pa (stamps) =
    let val bl = ref nil : (symbol * var) list ref
	fun vb () = 
	    protect(protectTyvars(NONE),
	      (fn () =>
		let val pat = pat(bl,true)
			      handle Clausal(id,_) =>
			        condemn("undefined op in pattern: "^name id)
		    and exp = (checkToken(EQUAL); exp(stamps))
		    and tvs = currentTyvars()
		 in case (pat,exp)
		      of (CONSTRAINTpat(VARpat(VALvar{name as [n],typ,...}), ty),
			   VARexp(ref(VALvar{access as INLINE _,...})))
			   => let val _::rest = !bl
				  val w = VALvar{name=name,typ=typ,access=access}
			       in bl := (n,w) :: rest;
			          VB{pat=CONSTRAINTpat(VARpat w, ty),
				     exp=exp,tyvars=tvs}
			      end

		       | (VARpat(VALvar{name as [n],typ,...}),
			   VARexp(ref(VALvar{access as INLINE _,...})))
			   => let val _::rest = !bl
				  val w = VALvar{name=name,typ=typ,access=access}
			       in bl := (n,w) :: rest;
			          VB{pat=VARpat w, exp=exp, tyvars=tvs}
			      end
		       | _ => VB{pat=pat,exp=exp,tyvars=tvs}
		end))
     in andListProtect(vb)
	before bindVARs(!bl)
    end

and rvb_pa (stamps) = 
    let val bl = ref nil : (symbol * var) list ref
	fun rvb () =  protect(protectTyvars(NONE),
	      (fn () =>  (* record bug *)
		let val var=newVAR(bl,opid())
		    and resultty=constraint_op()
		    and e = (checkToken(EQUAL); exp stamps)
		    and tvs=currentTyvars()
		 in case e of FNexp _ => ()
		       | MARKexp(FNexp _,_,_) => ()
		       | _ => complain "fn expression required in val rec declaration";
		    RVB{var = var, resultty = resultty, exp = e, tyvars = tvs}
		end))
     in protect(protectPatchList, (fn()=>
	  protect(protectScope, (fn()=>
	    (openRec(); andListProtect(rvb)) ))
	  before bindVARs(!bl) ))
    end

and fb_pa (stamps) = 
    let val bl = ref nil : (symbol * var) list ref
	fun fb () = protect(protectTyvars(NONE),
	  (fn () =>
	     let val funSymbol: symbol option ref = ref NONE
	         val clauses=rightAssoc((fn () => clause funSymbol stamps),
					BAR,op::,single)
		 val CLAUSE{pats=p1,...}::_ = clauses
		 val len = length p1
	      in if exists (fn CLAUSE{pats,...} => length pats <> len) clauses
		  then complain "not all clauses have the same number of patterns"
		  else ();
		 FB{var = let val SOME id = !funSymbol in newVAR(bl,id) end,
		    clauses = clauses,
		    tyvars = currentTyvars()} 
	     end))
     in protect(protectPatchList, fn()=>
	  protect(protectScope, fn()=>
	    (openRec(); markdec (FUNdec o andListProtect) fb))
	  before bindVARs(!bl))
    end

and clause funsym stamps = 
    let val bl = ref nil : (symbol * var) list ref
        fun pat_p () = if firstApat lookFIX (!nextToken)
			 then (pat(bl,false)  (* atomic pattern *)
			       handle Clausal(id,_) =>
				 condemn("undefined op in pattern: "^name id))
			      :: pat_p ()
			 else nil
     in (pat(bl,true); condemn("no defined function in clausal lhs"))
	handle Clausal(id,pat1) =>
          (case !funsym
	     of NONE => funsym := SOME id
	      | SOME f => if Symbol.eq(id,f) then ()
		            else complain "identifiers in clauses don't match";
	   let val pats = pat1::pat_p()
	       val resultty = constraint_op()
	       val exp = protect(protectScope,
			   (fn()=>(checkToken(EQUAL);
			           if !nextToken=DARROW then advance() else ();	
				   bindVARs(!bl); markexp exp stamps)))
	    in CLAUSE{pats=pats, resultty=resultty, exp=exp}
	   end)
    end

and constraint () = (checkToken(COLON); ty())

and constraint_op() =
    if at(COLON)
    then SOME(ty())
    else NONE

and tb(notwith,path,stamps: Stampset.stampsets) = 
    let	fun tb1() =
	    let fun equalargs([],[]) = true
		  | equalargs(tv::rest,VARty(tv')::rest') =
		      tv = tv' andalso equalargs(rest,rest')
		  | equalargs _ = false
		val args = tyvars()
		val name = ident()
		val _ = checkToken(EQUAL)
		val typ = protect(protectTyvars(SOME args), ty)
	        val _ = TypesUtil.bindTyvars args;
		val binding =
		      case typ
			of CONty(tycref as ref(TYCON{stamp,arity,eq,path=path',kind}),
				 args') =>
			     if Stampset.tycFixed(stamp) andalso equalargs(args,args')
			     then case kind
				    of UNDEFtyc _ =>
				        (tycref :=
					  TYCON{stamp=stamp,arity=arity,eq=eq,
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
		TB{tyc=binding,def=typ}
	    end
     in TYPEdec(andList(tb1))
    end

and tyvars() =
    case !nextToken
      of Token.TYVAR s => [mkTyvar(mkUBOUND(s))] before advance()
       | LPAREN =>
	    (advance();
	     tyvar_pc() before
	     checkToken(RPAREN))
       | _ => nil

and tyvar_pc() = rightAssoc(tyvar,COMMA,op::,single)

and tyvar() = mkTyvar(mkUBOUND(
	       case !nextToken
	        of Token.TYVAR s => (advance(); s)
		 | tok => (complain ("expected type variable, found "
				    ^ tokenName tok); bogusID)))

and db(path,stamps) =
    let val (datatycs,withtycs) =
		protect(protectDb(), (fn()=>
		  (andList(db1(ty,path,stamps)),
		   if at(WITHTYPE)
		   then let val TYPEdec x = tb(false,path,stamps) in x end
		   else nil)))
	val checkeq = defineEqTycon (fn x => x)
     in app (fn (ref tyc) => checkeq tyc) datatycs;
	app (fn TB{tyc,...} => checkeq(!tyc)) withtycs;
	DATATYPEdec{datatycs=datatycs,withtycs=withtycs}
    end

and db1(parsety,path,stamps) () =
    let val args = tyvars()
   	val name = ident()
	val arity = length args
	val rangeType = CONty(!lookArTYC(name,arity), map VARty args)
	fun constr() =
	    let val sym = (if at OP
			   then warn "unnecessary op in datatype declaration"
			   else ();
			   ident())
		val const = not(at(OF))
		val typ = if const then rangeType
				else CONty(arrowTycon, [parsety(), rangeType])
	     in (sym,const,typ)
            end
     in protect(protectTyvars(SOME args),
	 (fn()=>
	   let val dcl = (checkToken(EQUAL); rightAssoc(constr,BAR,op::,single))
	       val sdcl = sort3 dcl
	       val sign = ConRep.boxed(sdcl)
               fun binddcons ((sym,const,typ)::restdcl,rep::restsign) =
		   let val dcon =
		       DATACON{name = sym, const = const, rep = rep, sign = sign,
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
	     in if length sdcl < length dcl
	       	    then complain "duplicate constructor name" else ();
		TypesUtil.bindTyvars args;
		let val tycref = ref(mkDATAtyc(name::path,arity,
				               binddcons(sdcl,sign),MAYBE,stamps))
		 in bindTYC(name,tycref);
		    tycref
		end
	    end))
    end

and ab(path,stamps) =
    let val mAbstype = openScope()
	val DATATYPEdec{datatycs,withtycs} = db(path,stamps)
	val withtycons = map (fn TB{tyc,...} => tyc) withtycs
	val abstycs = makeAbstract(datatycs,withtycons)
	val mWith = (openScope(); current())
	val body = (checkToken(WITH); ldecs(path,stamps))
	fun bind tyc = bindTYC(tycName(!tyc), tyc)
     in checkToken(END);
	splice(mAbstype,mWith);
	app bind datatycs (* abstycs *);
	app bind withtycons;
	ABSTYPEdec{abstycs=datatycs,withtycs=withtycs,body=body}
    end

and eb() = EXCEPTIONdec(andList(eb1))

and eb1() =
    let val name = ident()
(* Capitalization convention
	val _ = if looksLikeExn name then ()
		else warn "Exception name should be capitalized"
*)
     in case !nextToken
	  of OF =>
	      (advance();
	       let val etype = ty()
		   val exn = DATACON{name = name,
				     const = false,
				     typ = ref(etype --> exnTy),
				     rep = VARIABLE(LVAR(namedLvar(name))),
				     sign = []}
	        in bindCON(name,exn);
		   EBgen{exn=exn,etype=SOME etype}
	       end)
	   | EQUAL =>
	       (advance();
	        let val edef as DATACON{const,typ,rep,sign,...} = 
		        case !nextToken
			  of IDDOT s => qid lookEXNinStr
			   | ID s => getEXN(getSymbol())
			   | tok =>
			     (complain("expected exception name, found token"
				       ^ tokenName tok);
			      unboundEXN(bogusID))
		    val exn = DATACON{name=name,const=const,typ=ref(!typ),sign=sign,
			      	      rep=VARIABLE(LVAR(namedLvar(name)))}
		 in bindCON(name,exn);
		    EBdef{exn=exn,edef=edef}
		end)
	   | _ =>
	       let val exn = DATACON{name = name,
			             const = true,
				     typ = ref exnTy,
				     rep = VARIABLE(LVAR(namedLvar(name))),
				     sign = []}
	        in bindCON(name,exn);
		   EBgen{exn=exn,etype=NONE}
	       end
    end


and ebx() = EXCEPTIONdec(andList(eb1x))

and eb1x() =
    let val name = ident()
	val etype = constraint_op()
	val (const,typ) = case etype
			      of NONE => (true,exnTy)
			       | SOME t => if isUnitTy(t)
					   then (true,exnTy)
					   else (false,t-->exnTy)
	val edef = if at(EQUAL)
	           then SOME(case !nextToken
			       of IDDOT _ => qid lookEXNinStr
			        | ID s => getEXN(getSymbol())
				| _ => unboundEXN(bogusExnID) )
		   else NONE
        val exn = case edef
		    of NONE => 
		         DATACON{name=name, const=const, typ=ref typ,
				 rep=VARIABLE(LVAR(namedLvar(name))),
				 sign=[]}
		     | SOME(DATACON{name=n,const,typ,rep,sign}) =>
			    DATACON{name=name,const=const,typ=ref(!typ),rep=rep,
			    	    sign=sign}  (* changes only name *)
     in bindCON(name, exn);
	case edef
	  of NONE => EBgen{exn=exn,etype=etype}
	   | SOME exn' => EBdef{exn=exn,edef=exn'}
    end

and ldec(path,stamps) =
      case !nextToken
	of VAL =>
	      (advance(); vb(stamps))
         | FUN =>
	      (advance(); fb_pa(stamps))
         | TYPE =>
	      (advance(); tb(true,path,stamps))
         | DATATYPE =>
	      (advance(); db(path,stamps))
         | ABSTYPE =>
	      (advance(); ab(path,stamps))
	 | EXCEPTION =>
	      (advance(); eb())
         | Token.LOCAL =>
	     let val envLocal = openScope()
		 val ld1 = (advance(); ldecs([],stamps))
		 val envIn = (checkToken(IN); openScope(); current())
		 val ld2 = ldecs(path,stamps)
	      in checkToken(END);
		 splice(envLocal,envIn);
 		 markdec LOCALdec(ld1,ld2)
	     end
	 | Token.OPEN =>  (* confusion with Env.OPEN when Env not constrained *)
	      let val strs = (advance(); qid_p())
	       in app openStructureVar strs;
 		  markdec OPENdec strs
	      end
         | INFIX =>
	      let val prec = case (advance(); optprec()) of SOME n=>n|NONE=>0
	       in app (fn i => bindFIX(i,FIXvar{name=i,binding=infixleft prec})) (ops());
	          SEQdec(nil)
	      end
         | INFIXR =>
	      let val prec = case (advance(); optprec()) of SOME n=>n|NONE=>0
	       in app (fn i => bindFIX(i,FIXvar{name=i,binding=infixright prec})) (ops());
	          SEQdec(nil)
	      end
         | NONFIX =>
	      (advance();
	       app (fn i => bindFIX(i,FIXvar{name=i,binding=NONfix})) (ops()); SEQdec(nil))
	 | OVERLOAD =>
	      let val id = (advance(); ident())
		  val scheme = (checkToken(COLON);
		    protect(protectScope, (fn () =>  (* localize tyvars *)
		      protect(protectTyvars(NONE), (fn () =>
		        let val body = ty()  (* generalize type variables *)
			    val tvs = currentTyvars()
			 in TypesUtil.bindTyvars tvs;
			    TYFUN{arity=length(tvs),body=body}
			end)))))
		  fun option() =
		      let val VARexp(ref (v as VALvar{typ,...})) = exp(stamps)
		       in {indicator = TypesUtil.matchScheme(scheme,!typ),
			   variant = v}
		      end
		  val l = (checkToken(AS); andList(option))
	       in bindVAR(id,OVLDvar{name=id,options=ref l,scheme=scheme});
		  SEQdec nil
	      end
         | tok => (complain ("expected a declaration, found " ^
			    tokenName tok); vb(stamps))

and ldecs(path_stamps) =
  let fun ldecs() =
      if firstLdec(!nextToken)
        then ldec(path_stamps) :: (at(SEMICOLON); ldecs())
        else []
   in case ldecs() of [dec] => dec | seq => SEQdec seq
  end

and optprec() = case !nextToken of INT i => (advance();SOME(i)) | _ => NONE

and qid_p(): structureVar list =  (* error if no identifier's ? *)
    case !nextToken
      of ID s => getSTR(ident()) :: qid_p()
       | IDDOT _ => qid(lookSTRinStr)::qid_p()
       | _ => nil

and ops() =
  let fun ops1() =
        case !nextToken
          of ID s => (s) :: (advance(); ops1())
	   | EQUAL => (EQUALsym) :: (advance(); ops1())
	   | ASTERISK => (ASTERISKsym) :: (advance(); ops1())
           | _ => nil
   in case ops1()
	of [] => (complain("operator or identifier expected, found "
		  ^ tokenName (!nextToken)); [])
         | l => l
  end


(* signatures *)

fun addzeros(0,l) = l
  | addzeros(n,l) = addzeros(n-1,0::l)

fun sigbody(depth: int, stamps: Stampset.stampsets) : Structure = 
    let val tComps = array(maxTypSpecs,NULLtyc)
	and tCount = ref 0
	fun tNext x = (update(tComps,!tCount,x);
		       INDtyc(!tCount before inc tCount))
	val sComps = array(maxStrSpecs,NULLstr)
 	and sCount = ref 2  (* slots 0,1 reserved for parent, fct param (if any) *)
	fun sNext x = (update(sComps,!sCount,x);
		       INDstr(!sCount before inc sCount))
	val tempenv = REL{t=tComps,s=sComps}
	fun pairs (nil : spath list list) : (spath*spath) list = nil
	  | pairs ((a::b::r) :: s) = (a,b) :: pairs((b::r) :: s)
	  | pairs ( _ :: s ) = pairs s
	val strSharing : spath list list ref = ref nil
	val typeSharing : spath list list ref = ref nil

        val slot = ref 0
	fun nextSlot() = (!slot before inc slot)

	val table = newTable()
	val tables = ref [table]

	(* includeSig used to implement include specs *)

	fun includeSig({strStamps=strStamps0, tycStamps=tycStamps0}: Stampset.stampsets,
		       STRstr{kind=SIGkind{bindings,stamps={strStamps,tycStamps},...},
			      env=REL{s=senv,t=tenv},...}) =
	    let val transStrStamp = Stampset.join(strStamps0,strStamps)
		val transTycStamp = Stampset.join(tycStamps0,tycStamps)
		val sOffset = !sCount - 2 (* offset for structure indices *)
		val tOffset = !tCount     (* offset for tycon indices *)

		(* adjustPath(depth: int, path: int list) *)
		fun adjustPath(0,[i]) = [i+tOffset]
		  | adjustPath(0,i::r) = (i+sOffset) :: r
		  | adjustPath(0,[]) = impossible "sigBody.includeSig.adjustPath"
		  | adjustPath(d,0::(r as _::_)) = 0 :: adjustPath(d-1,r)
		  | adjustPath(d,p) = p

		fun adjustType(depth,ty) =
		    let fun adjust(CONty(ref(RELtyc(p)),args)) =
			      CONty(ref(RELtyc(adjustPath(depth,p))), map adjust args)
			  | adjust(CONty(reftyc,args)) =
			      CONty(reftyc, map adjust args)
			  | adjust(POLYty{sign,tyfun=TYFUN{arity,body}}) =
			      POLYty{sign=sign,
				     tyfun=TYFUN{arity=arity,body=adjust body}}
			  | adjust ty = ty
		     in adjust ty
		    end

		fun transTBinding depth binding =
		    case binding
		     of VARbind(VALvar{name,typ,access}) =>
			  VARbind(VALvar{name=name,access=access,
					 typ=ref(adjustType(depth,!typ))})
		      | CONbind(DATACON{name,typ,const,rep,sign}) =>
			  CONbind(DATACON{name=name, const=const, sign=sign, rep=rep,
					  typ=ref(adjustType(depth,!typ))})
		      | _ => binding

		fun transLBinding table binding =
		    case binding
		     of VARbind(VALvar{name=[n],typ,access}) =>
			  IntStrMap.map table (NameSpace.varKey n)
		      | CONbind(DATACON{name,typ,const,rep,sign}) =>
			  IntStrMap.map table (NameSpace.conKey name)
		      | _ => binding

		fun newTyc(tyc as TYCON{stamp,kind,...}) =
		    if Stampset.tycFixed(stamp)
		    then tyc
		    else (case kind
			   of ABStyc => setTycStamp(transTycStamp(stamp),tyc)
			    | DATAtyc _ => setTycStamp(transTycStamp(stamp),tyc)
			    | _ => tyc)
		  | newTyc _ = impossible "Parse.includeSig.newTyc"

		fun newEnv(depth,REL{s,t}) =
		     REL{s=mapSubstrs(newStr depth,s), t=ArrayExt.map(newTyc,t,0)}
		  | newEnv _ = impossible "Parse.includeSig.newEnv"

		and newStr depth (str as STRstr{stamp,sign,table,env,
						kind=SIGkind{stamps,share,bindings}}) =
		    if Stampset.strFixed(stamp)
		    then str
		    else let val newenv as REL{s,t} = newEnv(depth+1,env)
			     val newtable =
				 IntStrMap.transform (transTBinding depth) table
			     val new =
				 STRstr{stamp=transStrStamp(stamp),
					table=newtable,
					kind=SIGkind{stamps=stamps,share=share,
						     bindings=map
							      (transLBinding newtable)
							      bindings},
					      env=newenv, sign=sign}
			  in ArrayExt.app(ModUtil.resetParent new, s, 2);
			     new
			 end
		  | newStr _ (INDstr i) = impossible("sigbody.newStr INDstr "^
						   makestring i)
		  | newStr _ (SHRstr _) = impossible "sigbody.newStr SHRstr"
		  | newStr _ (NULLstr) = impossible "sigbody.newStr NULLstr"
		  | newStr _ _ = impossible "sigbody.newStr STRkind"

		fun adjustBinding binding =
		    case binding
		     of VARbind(VALvar{name=[n],typ,...}) =>
			  bindVAR(n,VALvar{name=[n],typ=ref(adjustType(0,!typ)),
					   access=SLOT(nextSlot())})
		      | CONbind(DATACON{name,typ,const,rep as VARIABLE(SLOT _),sign}) =>
			  bindCON(name,DATACON{name=name,
					       const=const,
					       sign=sign,
					       typ=ref(adjustType(0,!typ)),
					       rep=VARIABLE(SLOT(nextSlot()))})
		      | CONbind(DATACON{name,typ,const,rep,sign}) =>
			  bindCON(name,DATACON{name=name,
					       const=const,
					       sign=sign,
					       typ=ref(adjustType(0,!typ)),
					       rep=rep})
		      | TYCbind(ref(INDtyc i)) =>
			  let val tyc = tenv sub i
			      val name = tycName tyc
			   in bindTYC(name,ref(tNext(newTyc(tyc))))
			  end
		      | STRbind(STRvar{name as [n],binding=INDstr i,...}) =>
			  bindSTR(n,STRvar{name=name,
					   binding=sNext(newStr 1 (senv sub i)),
					   access=SLOT(nextSlot())})
		      | FIXbind(fixvar as FIXvar{name,...}) =>
			  bindFIX(name,fixvar)
		      | _ => impossible "sigBody.adjustBinding"

	     in map adjustBinding bindings
	    end (* includeSig *)
	  | includeSig _ = impossible "Parse.includeSig - bad arg"

        (* the following four functions help implement the open spec.
	   lookPathSTRinSig looks like it belongs in EnvAccess *)

	fun lookPathSTRinSig (spath as first::rest) : Structure * int list =
	    let	fun complainUnbound() =
		    (complain "unbound structure in signature";
		     print "  name: "; printSequence "." printSym spath;
		     newline();
		     raise Syntax)
		(* second arg of get is expected to be a signature *)
		fun get([id],STRstr{table,env as REL{s,...},...}) = 
		     (case lookSTRinTable(table,id)
			   handle UnboundTable => complainUnbound()
		       of STRvar{binding=INDstr i,...} => (s sub i, [i])
			| STRvar{binding=SHRstr(p as i::r),...} =>
			    (getEpath(r,s sub i), p) (* not possible? *)
			| _ => impossible "lookPathSTRinSig.get")
		  | get(id::rest,STRstr{table,env=REL{s,...},...}) =
		      let val STRvar{binding=INDstr k,...} =
				lookSTRinTable(table,id)
				handle UnboundTable => complainUnbound()
			  val (str,p) = get(rest, s sub k)
		       in (str, k::p)
		      end
		  | get([],str) = (str,[])
		  | get(p,NULLstr) =
		     impossible "sigbody.lookPathSTRinSig.get - NULLstr"
		  | get(p,INDstr _) =
		     impossible "sigbody.lookPathSTRinSig.get - INDstr"
		  | get(p,SHRstr _) =
		     impossible "sigbody.lookPathSTRinSig.get - SHRstr"
		  | get _ = impossible "sigbody.lookPathSTRinSig.get - bad args"
		fun lookInStr(str) =
		      (case rest
			 of [] => str
			  | _ => 
			    let val STRvar{binding,...} =
				    lookPathinStr(str, [], spath, lookSTRinStr)
			     in binding
			    end,
		       [1])
		val leadStr = lookSTR0 first
			      handle Unbound => complainUnbound()
	     in case leadStr
		  of (STRvar{binding=INDstr i,...},{path as h::r,strenv=REL{s,...}}) =>
			if h < 0 (* indicates signature component *)
			then let val (str,p) = get(rest, s sub i)
			      in (str,path@(i::p))
			     end
			else lookInStr(s sub i)
		   | (STRvar{binding=SHRstr(i::r),...},{strenv=REL{s,...},...}) =>
			lookInStr(getEpath(r, s sub i))
		   | (STRvar{binding as STRstr _,...},_) => lookInStr binding
		   | _ => impossible "sigbody.lookPathSTRinSig - leadStr"
	    end
	  | lookPathSTRinSig _ = impossible "sigbody.lookPathSTRinSig - bad arg"

	fun openStrIds(): spath list =
	    case !nextToken
	      of ID _ => [ident()] :: openStrIds()
	       | IDDOT _ => symPath()::openStrIds()
	       | _ => nil

	fun openStrInSig(p:spath) =
	    case lookPathSTRinSig p
	     of (STRstr{table,env,...},p) => openOld({path=p,strenv=env},table)
	      | _ => impossible "openStrInSig -- bad arg"

        fun mergeTables tables =
	    let val bottom::rest = rev tables
	     in revfold
		  (fn (table,acc) => (IntStrMap.app (IntStrMap.add acc) table; acc))
		  rest bottom
	    end

	fun spec_s() =
	     if firstSpec(!nextToken)
		 then (spec() @ (at(SEMICOLON); spec_s()))
		 else nil

	and spec() =
	    case !nextToken
	      of STRUCTURE => (advance(); strspec())
	       | DATATYPE => (advance(); dtyspec())
	       | TYPE => (advance(); tyspec NO)
	       | EQTYPE => (advance(); tyspec YES)
	       | VAL => (advance(); valspec())
	       | EXCEPTION => (advance(); exnspec())
	       | INFIX => (advance(); infixspec(infixleft))
	       | INFIXR => (advance(); infixspec(infixright))
	       | NONFIX => 
		   (advance();
		    app (fn i => bindFIX(i,FIXvar{name=i,binding=NONfix})) (ops());
		    nil)
	       | SHARING => (advance(); sharespec())
	       | INCLUDE => (advance(); includespec())
	       | Token.LOCAL => (advance(); localspec())
	       | Token.OPEN => (advance(); openspec())
	       | tok => condemn("expected a spec (component of signature)\
			\ found " ^ tokenName tok)


	and localspec() =
	    (spec_s(); checkToken(IN); spec_s() before checkToken(END))

	and openspec() =
	      let val strpaths = openStrIds()
		  val newtable = newTable()
	       in case strpaths
		   of [] => complain "no structure ids in open spec"
		    | _ => 
		       (app openStrInSig strpaths;
		        openNew({path=[~depth],strenv=tempenv},newtable);
			tables := newtable :: !tables);
		  [] (* no bindings returned *)
	      end

	and includespec() =
	    let val name = ident()
		val SIGvar{binding,...} = lookSIG name
	     in includeSig(stamps,binding)
	    end

	and strspec() = 
	    rightAssoc(strspec1,AND,op :: , single)

	and strspec1() =
	    let val name = ident()
 	        val _ = checkToken(COLON)
 		val sgn =
 		  case !nextToken
 		    of ID s =>
 			  let val name = s before advance()
 			      val SIGvar{binding,...} = lookSIG(name)
 			   in ModUtil.shiftSigStamps(stamps,binding)  
 			  end
 		     | Token.SIG =>
 			 (advance();
 			  sigbody(depth+1,stamps)
 			  before checkToken(END))
 		     | tok => condemn("expected a signature or signature-identifier, \
 				      \found: "^tokenName tok)
 	     in bindSTR(name,STRvar{name=[name],access=SLOT(nextSlot()),
				    binding=sNext(sgn)})
	    end

	and dtyspec() =
	    let val dtycs =
		 (protect(protectDb(), fn() =>
		      map (fn (r as ref tyc) => 
			    (r := tNext tyc; (TYCbind r, tyc)))
			(rightAssoc(db1(ty,[],stamps),AND,op ::,single))))
		val tycbinds = map (fn (x,_) => x) dtycs
		val tycons = map (fn (_,y) => y) dtycs
		fun collectdcons(tyc::rest,dcbinds) =
		     let val TYCON{kind=DATAtyc(dcons),...} = tyc
			 fun binddcons(DATACON{name,...}::rest',dcbs) =
			      binddcons(rest',
			       (let val (b,_) = Env.look(NameSpace.conKey(name))
				 in b::dcbs
				end
				handle Unbound => dcbs))
			   | binddcons([],dcbs) = dcbs
		      in collectdcons(rest,binddcons(dcons,dcbinds))
		     end
		  | collectdcons([],dcbinds) = dcbinds
	     in app (defineEqTycon (tyconInContext tempenv)) tycons;
		tycbinds @ collectdcons(tycons,[])
	    end

	and tyspec eq = 
	    rightAssoc(tyspec1 eq, AND, op ::, single)

	and tyspec1 eq () =
	    let val arity = length(tyvars())
		val name = ident()
		val tycref = ref(tNext(mkABStyc([name],arity,eq,stamps)))
	     in bindTYC(name, tycref)
	    end

	and valspec() =     
	    rightAssoc(valspec1,AND,op ::,single)

	and valspec1() =
	    let val name = 
		    (if at OP
		     then warn "unnecessary op in val specification"
		     else ();
		     case !nextToken
		      of ID s => getSymbol()
		       | ASTERISK => getSymbol()
		       | EQUAL => getSymbol()
		       | tok =>
			  (complain("val spec: expected identifier, found "
			   ^ tokenName tok); bogusID))
		val _ = checkToken(COLON)
		val typ =
		    protect(protectScope, (fn () =>
		      (* localize type variables *)
		      protect(protectTyvars(NONE), (fn () =>
			let val body = ty()
			    val tvs = currentTyvars()
			 in case tvs
			      of [] => body
			       | _ =>
				 let val sign = TypesUtil.bindTyvars1 tvs
				  in POLYty
				      {sign = sign,
				       tyfun = TYFUN{arity = length tvs, 
						     body = body}}
				 end
			end))))
	     in bindVAR(name,VALvar{name=[name],typ=ref typ,access=SLOT(nextSlot())})
	    end

	and exnspec() = 
	    rightAssoc(exnspec1,AND,op ::,single)

	and exnspec1() =
	    let val name = ident()
		val (const,typ) =
		    if at(OF) then
		      (false,
		       protect(protectScope, (fn () =>
			 (* localize type variables *)
			 protect(protectTyvars(NONE), (fn () =>
			   let val body = ty()
			       val tvs = currentTyvars()
			    in case length tvs
				 of 0 => body --> exnTy
				  | n => 
				    (TypesUtil.bindTyvars tvs;
				     POLYty
				      {sign = mkPolySign n,
				       tyfun = TYFUN{arity = n,
						     body = body --> exnTy}})
			   end)))))
		    else (true,exnTy)
	     in bindCON(name, DATACON{name=name, const=const, typ= ref typ,
				      rep=VARIABLE(SLOT(nextSlot())),
				      sign=[]})
	    end

	and infixspec(mkinfix) =
	    let val prec = case optprec() of SOME n=>n|NONE=>0
	     in app (fn i => bindFIX(i,FIXvar{name=i,binding=mkinfix prec}))
		    (ops());
		nil
	    end

	and sharespec() =
	    (rightAssoc(sharespec1,AND,discard,discard); nil)

	and sharespec1() =
	    case !nextToken
	      of TYPE => (advance(); typeSharing := patheqn() :: !typeSharing)
	       | ID s => strSharing := patheqn() :: !strSharing
	       | IDDOT _ => strSharing := patheqn() :: !strSharing
	       | tok => condemn("unexpected token after \"sharing\": "
				^tokenName tok)

	and patheqn() : spath list =
	    rightAssoc(symPath,EQUAL,op ::,single)

        val stamp = Stampset.newStamp(#strStamps stamps)
	val _ = openStr()
	val _ = openNew({path=[~depth],strenv=tempenv},table)
	val savedlookArTYC = !lookArTYC
	val savedlookPathArTYC = !lookPathArTYC
	val bindings = protect(
		 ((fn () => (lookArTYC := lookArTYCinSig depth;
			     lookPathArTYC :=
			       lookPathArTYCinSig depth)),
		  (fn () => (lookArTYC := savedlookArTYC;
			     lookPathArTYC := savedlookPathArTYC))),
		 spec_s)
	val _ = closeStr()
	val table = mergeTables(!tables)
	val senv = ArrayExt.copy(sComps,!sCount)
	val env = REL{s=senv, t=ArrayExt.copy(tComps,!tCount)}
	val sShare = pairs(!strSharing)
	val tShare = pairs(!typeSharing)
	val shareSpec =
	      if null sShare andalso null tShare
	      then {s=[],t=[]}
	      else Sharing.doSharing(table,env,stamps,{s=sShare,t=tShare})
	val result =
	      STRstr{stamp=stamp,
		     sign=Stampset.newStamp(Stampset.sigStamps),
		     table=table,
		     env=env,
		     kind=SIGkind{share=shareSpec,
				  bindings=bindings,
				  stamps=stamps}}
     in ArrayExt.app((ModUtil.setParent result),senv,2);
	result
    end (* fun sigbody *)

fun sign () : Structure =
    case !nextToken
      of ID s =>
	    let val name = s before advance()
		val SIGvar{binding,...} = lookSIG(name)
	     in binding
	    end
       | Token.SIG =>
	   (advance();
	    sigbody(1,Stampset.newStampsets())
	    before checkToken(END))
       | tok => condemn("expected a signature or signature-identifier, \
		        \found: "^tokenName tok)

fun sigconstraint () =
    (checkToken(COLON);
     sign())

fun sigconstraint_op () =
    if !nextToken = COLON
    then (advance(); SOME(sign()))
    else NONE

(* signature bindings *)

fun sigb() = 
    let fun sigb1() =
	    let val name = ident()
	     in checkToken(EQUAL);
	     	let val sigvar = SIGvar{name=name,binding=sign()}
		 in bindSIG(name, sigvar);
		    sigvar
		end
	    end
     in rightAssoc(sigb1,AND,op ::,single)
    end

(* structure expressions *)

fun str(abs: bool, constraint: Structure option, path: spath,
	stamps: Stampset.stampsets, param: Structure)
       : strexp * Structure * thinning =
    case !nextToken
      of IDDOT _ =>
	   let val strVar as STRvar{binding,...} = qid(lookSTRinStr)
	    in case constraint
		 of NONE => (VARstr strVar, binding, NONE)
		  | SOME sgn =>
		      let val (str,thin) =
			    SigMatch.match(abs,path,stamps,sgn,binding,param)
		       in (VARstr strVar, str, thin)
		      end
	   end
       | Token.STRUCT => 
	   (advance();
	    let val _ = openStr()
		val body = sdecs(path,stamps)
	     in (case constraint
		   of NONE =>
		       let val (thin,table) = BuildMod.buildStrTable ()
			in (STRUCTstr{body=body,locations=thin},
			    mkSTR(path,table,DIR,stamps),
			    NONE)
		       end
		    | SOME sgn => 
		       let val (str,thin) =
				 SigMatch.realize(abs,path,stamps,
				    Stampset.newStamp(#strStamps stamps),
				    sgn,param)
			in closeStr();
			   (STRUCTstr{body=body,locations=thin}, str, NONE)
		       end)
		before checkToken(END)
	    end)
       | ID s => 
	   let val id = getSymbol()
	    in if at(LPAREN)  (* functor application *)
	       then let val fctVar as FCTvar{binding=fct,...} = lookFCT id
			val (argexp,argstr) =
			      (* parse arg without using parameter sig *)
			     (if !nextToken = RPAREN
			      then (STRUCTstr{body=[],locations=[]},nullStr)
			      else if firstSdec(!nextToken)
			      then let val _ = openStr()
				       val body = sdecs([anonParamName],stamps)
				       val (thin,table) = BuildMod.buildStrTable ()
				    in (STRUCTstr{body=body,locations=thin},
					mkSTR([anonParamName],table,
					      DIR,stamps))
				   end
			      else let val FUNCTOR{paramName,...} = fct
				       val (strexp,str,_) =
					     str(false,NONE,[paramName],stamps,NULLstr)
				    in (strexp,str)
				   end)
			     before checkToken(RPAREN)
			val (result,thin1) = 
			      Functor.applyFunctor(fct,argstr,path,stamps)
			val strexp = APPstr{oper=fctVar,
					    argexp=argexp,
					    argthin=thin1}
		     in case constraint
			  of NONE => (strexp,result,NONE)
			   | SOME sgn =>
			       let val (thinned,thin2) =
				       SigMatch.match(abs,path,stamps,sgn,result,param)
				in (strexp,thinned,thin2)
			       end
		    end
	       else let val strVar as STRvar{binding,...} = getSTR id
		     in case constraint
			 of NONE => (VARstr strVar, binding, NONE)
			  | SOME sgn =>
			     let val (str,thin) =
				     SigMatch.match(abs,path,stamps,sgn,binding,param)
			      in (VARstr strVar, str, thin)
			     end
		    end
	   end
       | LET => protect(protectScope,
		    (fn()=>(advance();
		    	    let val locals = sdecs(path,stamps)
				val _ = checkToken(IN)
				val (bodyexp,bodystr,thin) =
				      str(abs,constraint,path,stamps,param)
				val _ = checkToken(END)
			     in (LETstr(SEQdec(locals),bodyexp),bodystr,thin)
			    end)))
       | tok => condemn("expected a structure-expression, found " ^
			 tokenName tok)

and sdecs(args as (path: spath, stamps: Stampset.stampsets))
         : dec list =
    let fun sdec() : dec =
	    if at(STRUCTURE)
 	      then markdec STRdec(strb(false,path,stamps))
	    else if at(ABSTRACTION)
 	      then markdec ABSdec(strb(true,path,stamps))
	    else if at(SIGNATURE)   (* monster structure hack *)
	      then (warn "signature found inside structure";
		    SIGdec(sigb()))
	    else if at(Token.FUNCTOR)   (* monster structure hack *)
	      then (warn "functor found inside structure";
 		    markdec FCTdec(fctb()))
 	    else if at Token.LOCAL
	      then let val envLocal = openScope()
		       val ld1 = sdecs args
		       val envIn = (checkToken(IN); openScope(); current())
		       val ld2 = sdecs args
		    in checkToken(END);
		       splice(envLocal,envIn);
		       markdec LOCALdec(SEQdec ld1,SEQdec ld2)
		   end
	    else let val dec = ldec(path,stamps)
		  in Typecheck.decType(dec);
		     dec
		 end
     in if firstSdec(!nextToken)
	then sdec() :: (at(SEMICOLON); sdecs(args))
	else nil
    end

(* structure bindings *)

and strb(abstract:bool,path:spath,stamps:Stampset.stampsets) =
    let fun strb1() =
	let val name = ident()
	    val constraint = 
		  if abstract
		  then SOME(sigconstraint())
		  else sigconstraint_op()
	    val _ = checkToken(EQUAL) 
	    val (strexp,str,thin) =
		  str(abstract,constraint,name::path,stamps,NULLstr)
	    val strVar = STRvar{access=LVAR(namedLvar(name)),
				name=[name],
				binding=str}
	 in (name, strVar,
	     STRB{strvar=strVar, def=strexp, constraint=constraint, thin=thin})
	end
     in map (fn (name,strVar,strSyn) => (bindSTR(name,strVar); strSyn))
	    (rightAssoc(strb1, AND, op ::, single))
    end


(* functor bindings *)

and fctb() =
    map (fn (name,fctVar,fctSyn) => (bindFCT(name,fctVar); fctSyn))
	(rightAssoc(fctb1, AND, op ::, single))

and fctb1() =
    let val name = ident()
	val mEntry = openScope()
	val (pname,paccess,param,spreadParams) =
	     (checkToken(LPAREN);
	      (case !nextToken
		of RPAREN => (anonParamName,LVAR(namedLvar(anonParamName)),nullSig,
			      false)
	         | ID s => let val tenv = array(0, NULLtyc)
			       val senv = array(2, NULLstr)
			       val _ = openNew({path=[~1], strenv=REL{t=tenv,s=senv}},
					       newTable())
			       val name = ident()
			       val access = LVAR(namedLvar(name))
			       val _ = checkToken(COLON)
			       val param = sign()
			    in update(senv,1,param);
			       bindSTR(name,STRvar{name=[name],
					   	   access=access,
						   binding=INDstr(1)});
			       (name,access,param,false)
			   end
		 | tok => if firstSpec(tok)
			  then let val plvar = namedLvar(anonParamName)
				   val param as STRstr{env,table,...} =
				         sigbody(2,Stampset.newStampsets())
				in openOld({path=[~1,1],strenv=env},table);
				   (anonParamName,LVAR(plvar),param,true)
			       end
			  else condemn ("expected functor parameter spec, found "
				       ^tokenName tok))
	      before checkToken(RPAREN))
	val resSign = 
	    if !nextToken = COLON
	    then (advance(); SOME(sign()))
	    else NONE
	val _ = if spreadParams
		then let val STRstr{table,env,...} = param
			 and LVAR plvar = paccess
		      in resetEnv(mEntry);
			 openOld({path=[plvar],strenv=env},table)
		     end
		else ()
        val _ = checkToken(EQUAL)
	val bodystamps = Stampset.newStampsets()
        val (bodyexp,bodystr,thin) = str(false,resSign,[],bodystamps,param)
        val openBody =
	    case bodystr
	      of STRstr{stamp=bodystamp,env=DIR,...} =>
		   Stampset.member(bodystamp,(#strStamps bodystamps))
	       | _ => false
	val paramVis = 
	    case resSign
	      of SOME _ => true
	       | NONE => openBody
	val body = 
	    if openBody
	    then Functor.abstractBody(bodystr,param,bodystamps,
		   Stampset.newStamp(Stampset.sigStamps))
	    else bodystr
	val paramvar = STRvar{name = [pname], access = paccess, binding = param}
	val fctv = FCTvar{name=name, 
			  access=LVAR(namedLvar(name)),
			  binding=FUNCTOR{paramName=pname,
					  param=param,
					  body=body,
					  paramVis=paramVis,
					  stamps=bodystamps}}
	val fb = FCTB{fctvar=fctv, param=paramvar, def=bodyexp, thin=thin,
		      constraint=resSign}
     in resetEnv(mEntry);
	(name,fctv,fb)
    end


(* top level declarations *)

fun importdec()=
    let fun loop() = 
            (case !nextToken of 
               SEMICOLON => []
             | STRING s  => (advance(); s :: loop())
             | _ => condemn("string constant (file name) expected, found " ^
                            tokenName (!nextToken))
            )
        val files = loop()
     in case files of
          [] => condemn("string constant (file name) expected, found " ^
                            tokenName (!nextToken))
        | _  => files
    end

val globalStamps = Stampset.globalStamps
val itsym = Symbol.symbol "it"

fun inner_interdec() =
	(prompt := !System.Control.secondaryPrompt;
         case !nextToken
	   of SIGNATURE => (advance(); SIGdec(sigb()))
 	    | Token.FUNCTOR => (advance(); markdec FCTdec(fctb()))
 	    | STRUCTURE =>
	        (advance(); markdec STRdec(strb(false,[],globalStamps)))
 	    | ABSTRACTION =>
	        (advance(); markdec STRdec(strb(true,[],globalStamps)))
            | IMPORT=>(advance(); IMPORTdec(importdec()))
	    | EOF => raise Eof
	    | tok => let val dec =
			     if firstLdec(!nextToken)
			     then ldec([],Stampset.globalStamps)
			     else if firstExp lookFIX (!nextToken)
			     then (markdec (fn() => VALdec[VB
				  (protect(protectTyvars(NONE),(fn() =>
				    {exp=exp(Stampset.globalStamps),
				     pat=let val v = newVAR(ref nil,itsym)
					  in bindVAR(itsym,v);
					     VARpat v
					 end,
				     tyvars=currentTyvars()})))]) ())
			     else condemn("declaration or expression expected, found " ^
					   tokenName tok)
		      in Typecheck.decType(dec); dec
		     end)

in inner_interdec()
end (* fun interdec *)

end (* structure Parse *)
