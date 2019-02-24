(* Copyright 1989 by AT&T Bell Laboratories *)
(* stampset.sig *)

(* two abstract types: stampset, realization

  stampset -- represents a set of stamps
  'a realization -- represents a (finite) mapping of elements of a stampset to 'a

  stampsets and stampmaps are objects, in that they have state.  The only
  changes to their states will be to add stamps or mappings, respectively.

  Each signature will contain a pair of stampsets: a type stampset and a
  structure stampset.  These represent the bound stamps in the signature.

  Functors will also contain a pair of stampsets specifying the generatively bound
  stamps in their body structure.

  A global pair of stampsets will contain the "fixed" or "free" stamps.

  All type (resp. structure) stampsets will be disjoint.

*)

signature STAMPSET =
sig

  type stamp
  type stampset      (* represents a set of stamps *)
  type stampsets     (* a record containing two stampsets for structures
		        and type constructors *)
  type 'a stampmap   (* represents a finite mapping over a stampset to type 'a *)

  val newStampset: unit -> stampset
    (* generate a new stampset, guaranteed to be disjoint from any
       existing stampset *)

  val newStampsets : unit -> stampsets
    (* generate a record containing two new stampsets *)

  val member : stamp * stampset -> bool
    (* tests membership in a stampset *)

  val newStamp : stampset -> stamp
    (* generate a new member of the given stampset *)

  val newMap : stampset * '1a -> '1a stampmap
    (* generate a new stampmap over a given stampset with given default value *)

  val updateMap : 'a stampmap -> stamp * 'a -> unit
    (* add mapping to a stampmap *)

  val applyMap : 'a stampmap * stamp -> 'a
    (* apply stampmap to a stamp *)

  val join : stampset * stampset -> stamp -> stamp 
    (* join(A,B) produces a translation function for elements of B and
       adds translated version of B to A, side-effecting A *)

  val fixedStrStamps : stampset
  val fixedTycStamps : stampset
  val globalStamps : stampsets
  val sigStamps : stampset
 
  val strFixed : stamp -> bool
  val tycFixed : stamp -> bool

end (* STAMPSET *)
