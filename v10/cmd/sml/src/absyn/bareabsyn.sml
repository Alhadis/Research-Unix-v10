(* Copyright 1989 by AT&T Bell Laboratories *)
(* Abstract syntax of bare ML *)

structure BareAbsyn : BAREABSYN = struct

structure Basics = Basics
structure Access = Access
type linenum = string * int

open Basics Access

datatype numberedLabel = LABEL of {name : Symbol.symbol, number: int};

datatype exp	= VARexp of var ref
		| CONexp of datacon
		| INTexp of int
		| REALexp of string
		| STRINGexp of string
		| RECORDexp of (numberedLabel * exp) list
		| SEQexp of exp list		
		| APPexp of exp * exp		
		| CONSTRAINTexp of exp * ty
		| HANDLEexp of exp * handler
		| RAISEexp of exp
		| LETexp of dec * exp
		| CASEexp of exp * rule list
		| FNexp of rule list
	        | MARKexp of exp * linenum * linenum

and rule	= RULE of pat * exp

and handler	= HANDLER of exp

and pat = WILDpat
	| VARpat of var
	| INTpat of int
	| REALpat of string
	| STRINGpat of string
	| CONpat of datacon
	| RECORDpat of {fields : (label * pat) list, flex : bool,
		        typ : ty ref, pats : pat list ref}
	| APPpat of datacon * pat
	| CONSTRAINTpat of pat * ty
	| LAYEREDpat of pat * pat

and strexp = VARstr of structureVar
	   | STRUCTstr of {body: dec list, locations: trans list}
	   | APPstr of {oper: functorVar, argexp: strexp, argthin: thinning}
	   | LETstr of dec * strexp

and dec	= VALdec of vb list
	| VALRECdec of rvb list
	| TYPEdec of tb list
	| DATATYPEdec of {datatycs: tycon ref list, withtycs: tb list}
	| ABSTYPEdec of {abstycs: tycon ref list, withtycs: tb list, body: dec}
	| EXCEPTIONdec of eb list
	| STRdec of strb list
	| ABSdec of strb list
	| FCTdec of fctb list
	| SIGdec of signatureVar list
	| LOCALdec of dec * dec
	| SEQdec of dec list
	| OPENdec of structureVar list
        | IMPORTdec of string list
        | MARKdec of dec * linenum * linenum

and vb = VB of {pat:pat, exp:exp, tyvars: tyvar list}

and rvb = RVB of {var:var, exp:exp, resultty: ty option, tyvars: tyvar list}

and fb = FB of {var:var, clauses: clause list, tyvars: tyvar list}

and clause = CLAUSE of {pats: pat list, resultty: ty option, exp:exp}

and tb = TB of {tyc : tycon ref, def : ty}

and eb = EBgen of {exn: datacon, etype: ty option}
       | EBdef of {exn: datacon, edef: datacon}

and strb = STRB of {strvar: structureVar, def: strexp,
		    thin: thinning, constraint: Structure option}

and fctb = FCTB of {fctvar: functorVar, param: structureVar, def: strexp,
		    thin: thinning, constraint: Structure option}

end (* structure BareAbsyn *)
