(* Copyright 1989 by AT&T Bell Laboratories *)
(* primop.sig *)

signature PRIMOP =
sig
  datatype primop = 
      ! | * | + | - | := | < | <= | > | >= | alength | boxed | div | cast |
      eql | fadd |fdiv |feql |fge |fgt |fle |flt |fmul |fneg |fneq |fsub | gethdlr |
      ieql | ineq | neq | makeref | ordof | profile |
      sethdlr | slength | callcc | throw | delay | force |
      store | subscript | unboxedassign | unboxedupdate | update | ~ |
      rshift | lshift | orb | andb | xorb | notb

  val pr_primop: primop -> string

end

;
