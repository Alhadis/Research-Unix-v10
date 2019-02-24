signature FORMULA =
 sig
     type formula
     val NUM : int -> formula
     val CELLREF : formula * formula -> formula
     val BINOP : (int*int->int) * formula * formula -> formula
     val IF : formula * formula * formula -> formula
     val eval : formula -> int array array -> int
 end

