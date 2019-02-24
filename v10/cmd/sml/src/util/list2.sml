(* Copyright 1989 by AT&T Bell Laboratories *)
(* list2.sml *)
(* functionals over pairs of lists *)

structure List2 = struct

fun app2 f ([],_) = ()
  | app2 f (_,[]) = ()
  | app2 f (a::l,b::m) = (f(a,b); app2 f (l,m))

fun map2 f ([],_) = []
  | map2 f (_,[]) = []
  | map2 f (a::l,b::m) = f(a,b)::(map2 f (l,m))

fun all2 pred ([],[]) = true
  | all2 pred (a::l,b::m) = pred(a,b) andalso all2 pred (m,l)
  | all2 pred _ = false;

end
