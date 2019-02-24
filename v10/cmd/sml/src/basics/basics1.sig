(* Copyright 1989 by AT&T Bell Laboratories *)
(* basicsprime.sig *)
signature BASICS' = sig    (* derived from TRANSPARENT_BASICS *)
                              
(* externals *)    
    type access (* from Access.access *)
    type 'a arraykey
    type 'a arrayconts 
    type bool
    type fixity 
    type int 
    type label 
    type 'a list
    type lvar
    type 'a option
    type polysign 
    type 'a ref
    type sharespec 
    type spath 
    type stamp 
    type string
    type symbol (* from Symbol.symbol *)
    type 'a intmap (* from Intmap.intmap *)

(* internal types *)
    type binding 
    type bool3  
    type conrep
    type datacon  
    type fixityVar
    type Functor
    type functorVar   
    type ind_var 
    type rowty
    type signatureVar
    type stampInfo
    type strkind
    type Structure
    type structureVar 
    type thinning
    type trans
    type tvkind 
    type ty 
    type tyckind
    type tycon
    type tyfun
    type tyvar  
    type var
                  (* `CONSTRUCTORS' *)

(*    type bool3  *)

      val yes: bool3
      and no: bool3
      and maybe: bool3


(*    type fixity *)

      val Nonfix: fixity
      and Infix: int * int -> fixity


(*    type conrep *)

      val undecided: conrep
      and tagged: int -> conrep
      and constant: int -> conrep
      and transparent: conrep
      and transu: conrep
      and transb: conrep
      and REF: conrep
      and variable: access -> conrep


(*    type tvkind *)

      val ibound: int -> tvkind
      and meta: int * int * bool -> tvkind
      and instantiated: ty -> tvkind
      and ubound: symbol * int * bool -> tvkind

(*    type datacon  *)

      val datacon: symbol * bool * ty * conrep * conrep list -> datacon

(*    type tyckind *)

      val abstyc: tyckind
      and deftyc: tyfun -> tyckind
      and datatyc: datacon list -> tyckind
      and recordtyc: symbol list -> tyckind
      and undeftyc: symbol list option -> tyckind

(*    type tycon *)

      val tycon: int * int * bool3 ref * symbol list * tyckind -> tycon
      and indtyc: int list -> tycon

(*    type ty *)

      val varty: tvkind ref -> ty
      and conty: tycon ref * ty list -> ty
      and flexrecordty: rowty ref -> ty
      and polyty: (int * bool) list * tyfun -> ty
      and undefty: ty
      and errorty: ty

(*    type rowty *)

      val Open: (symbol * ty) list -> rowty
      and closed: ty -> rowty

(*    type tyfun *)
      val tyfun: int * ty -> tyfun

(*    type var *)

      val valvar: access * symbol * ty ref -> var
      and ovldvar: symbol * ind_var list ref * tyfun-> var
      and unknownvar: symbol -> var

(*    type strkind *)
      val strkind: symbol list -> strkind
      and sigkind: ((symbol list * symbol list)list *
                   (symbol list * symbol list)list) *
                   binding list * int * int          -> strkind

(*    type  Structure *)
      val strstr: int * int * (string * binding) intmap * 
                  tycon arraykey * Structure arraykey * strkind -> Structure
      and indstr: int -> Structure

(*    type  Functor *)
      val Functor: symbol * Structure * Structure * int -> Functor

(*    type signatureVar *)
      val sigvar: symbol * Structure -> signatureVar

(*    type structureVar *)
      val strvar: symbol * access * Structure -> structureVar

(*    type functorVar    *)
      val fctvar: symbol * access * Functor -> functorVar

(*    type fixityVar    *)
      val fixvar: symbol * fixity -> fixityVar

(*    type binding  *)
      val varbind: var -> binding
      and conbind: datacon -> binding
      and tycbind: tycon ref -> binding
      and tyvbind: tvkind ref -> binding
      and sigbind: signatureVar -> binding
      and strbind: structureVar -> binding
      and fctbind: functorVar -> binding
      and fixbind: fixityVar -> binding

(*    type trans *)
      val valtrans: access -> trans
      and thintrans: access * lvar * trans list -> trans
      and contrans: datacon -> trans


end (* signature TRANSPARENT_BASICS *)







