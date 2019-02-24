(* Copyright 1989 by AT&T Bell Laboratories *)
structure IntNull = IntShare(
structure Machm = struct
                val _ = System.Control.interp := true;
                fun generate lexp = ErrorMsg.impossible "no code generator!"
             end
  val fileExtension = ".nul");
