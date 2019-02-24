(* Copyright 1989 by AT&T Bell Laboratories *)
signature STAMP =
sig
  type state
  val new : int * int -> state
  val current : state -> int * int
  val move : state * (int * int) -> unit
  val set : state * int * int -> unit
  val newTyc : state -> int
  val newStr : state -> int
  val newRecord : int -> int
  val base : int
  val fixed : int -> bool
  val bound : int -> bool
  val global : state
  val newSig : unit -> int
end

structure Stamp : STAMP =
struct

  type state = {tycCount : int ref, strCount : int ref}

  fun new(t: int, s: int) = {tycCount = ref t, strCount = ref s}

  fun current({tycCount,strCount}: state) = (!tycCount, !strCount)

  fun set({tycCount,strCount}: state, t: int, s: int) =
      (tycCount := t; strCount := s)

  fun move({tycCount,strCount}: state, (t: int, s: int)) =
      (tycCount := !tycCount + t; strCount := !strCount + s)

  fun newTyc({tycCount,...}: state) =
      !tycCount before inc tycCount

  fun newStr({strCount,...}: state) =
      !strCount before inc strCount


  val base = 1000000

  fun fixed s = s >= base

  fun bound s = s < base

  fun newRecord(n:int) = base + (n mod base)
    (* this allows a slight possibility that two distinct record tycons may
       be given the same stamp *)

  val global = new(2*base, 2*base)


 (* signature stamp generator *)

  val sigCount = ref(1)
  fun newSig () = !sigCount before inc sigCount

end (* structure Stamp *)
