(* Copyright 1989 by AT&T Bell Laboratories *)
(* bareabsyn.sig *)

signature BAREABSYN = sig

structure Basics : BASICS
structure Access : ACCESS
type linenum

datatype numberedLabel = LABEL of {name : Basics.Symbol.symbol, number: int};

datatype exp	= VARexp of Basics.var ref
		| CONexp of Basics.datacon
		| INTexp of int
		| REALexp of string
		| STRINGexp of string
		| RECORDexp of (numberedLabel * exp) list
		| SEQexp of exp list		
		| APPexp of exp * exp		
		| CONSTRAINTexp of exp * Basics.ty
		| HANDLEexp of exp * handler
		| RAISEexp of exp
		| LETexp of dec * exp
		| CASEexp of exp * rule list
		| FNexp of rule list
	        | MARKexp of exp * linenum * linenum

and rule	= RULE of pat * exp

and handler	= HANDLER of exp

and pat		= WILDpat
		| VARpat of Basics.var
		| INTpat of int
		| REALpat of string
		| STRINGpat of string
		| CONpat of Basics.datacon
		| RECORDpat of
		    {fields : (Basics.label * pat) list,
		     flex : bool,
		     typ : Basics.ty ref,
		     pats : pat list ref}
		| APPpat of Basics.datacon * pat
		| CONSTRAINTpat of pat * Basics.ty
		| LAYEREDpat of pat * pat

and strexp	= VARstr of Basics.structureVar
		| STRUCTstr of
		    {body: dec list,
		     locations: Basics.trans list}  (* component paths *)
		| APPstr of
		    {oper: Basics.functorVar,
		     argexp: strexp,
		     argthin: Basics.thinning}
		| LETstr of dec * strexp

and dec		= VALdec of vb list
		| VALRECdec of rvb list
		| TYPEdec of tb list
		| DATATYPEdec of
		    {datatycs: Basics.tycon ref list,
		     withtycs: tb list}
		| ABSTYPEdec of
		    {abstycs: Basics.tycon ref list,
		     withtycs: tb list,
		     body: dec}
		| EXCEPTIONdec of eb list
		| STRdec of strb list
		| ABSdec of strb list
		| FCTdec of fctb list
		| SIGdec of Basics.signatureVar list
		| OPENdec of Basics.structureVar list
		| LOCALdec of dec * dec
		| SEQdec of dec list
                | IMPORTdec of string list
	        | MARKdec of dec * linenum * linenum

and vb		= VB of
		    {pat:pat,
		     exp:exp,
		     tyvars: Basics.tyvar list}
and rvb		= RVB of
		    {var:Basics.var,
		     exp:exp,
		     resultty: Basics.ty option,
		     tyvars: Basics.tyvar list}
and fb		= FB of
		    {var:Basics.var,
		     clauses: clause list,
		     tyvars: Basics.tyvar list}
and clause	= CLAUSE of
		    {pats: pat list,
		     resultty: Basics.ty option,
		     exp:exp}
and tb		= TB of
		    {tyc : Basics.tycon ref,
		     def : Basics.ty}
and eb	 	= EBgen of
		    {exn: Basics.datacon,
		     etype: Basics.ty option}
		| EBdef of
		    {exn: Basics.datacon,
		     edef: Basics.datacon}
and strb	= STRB of 
		    {strvar: Basics.structureVar,
		     def: strexp,
		     thin: Basics.thinning,
		     constraint: Basics.Structure option}  (* info only *)
and fctb	= FCTB of
		    {fctvar: Basics.functorVar,
		     param: Basics.structureVar,
		     def: strexp,
		     thin: Basics.thinning,
		     constraint: Basics.Structure option}  (* info only *)

end (* signature BAREABSYN *)
