(* Copyright 1989 by AT&T Bell Laboratories *)
(* All string arguments to system calls should be zero padded *)
signature ASSEMBLY =
  sig
      datatype datalist = DATANIL | DATACONS of (string * string * datalist)
      type func
      datatype funclist = FUNC of (func * string * funclist)
      type object
      structure A : sig
	val array : int * 'a -> 'a array
	val callc : 'b (* func*)  * 'a -> 'c
	val create_b : int -> string
	val create_s : int -> string
	val floor : real -> int
	val logb : real -> int
	val scalb : real * int -> real
	val syscall : int * string list * int -> int
      end
      exception Div
      exception Interrupt
      exception Overflow
      exception Real of string
      exception SystemCall of string
      exception UnboundTable
      val array0 : 'a array
      val bytearray0 : string
      val collected : int ref
      val collectedfrom : int ref
      val current : string ref
      val datalist : datalist
      val errorstrings : string array
      val external : funclist
      val gcmessages : int ref
      val gcprof : string ref
      val majorcollections : int ref
      val minorcollections : int ref
      val opsys : int   (* 1 = vax bsd ultrix, 4.2, 4.3
			   2 = sunos 3.0, 4.0 
			   3 = vax v9 (bell labs) *)
      val pstruct : object ref
      val ratio : int ref
      val softmax : int ref
end
