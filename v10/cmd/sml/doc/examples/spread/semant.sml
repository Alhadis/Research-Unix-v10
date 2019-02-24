structure Semant : FORMULA =
struct
     type formula = int array array -> int
     fun NUM i = fn a => i
     fun CELLREF (e1,e2) = fn a => a sub e1 a sub e2 a
     fun BINOP (f,e1,e2) = fn a => f(e1 a, e2 a)
     fun IF (e1,e2,e3) = fn a => if e1 a <> 0 then e2 a else e3 a
     val eval = fn x => x
end
