(* depth-first state buffer -- implements a stack *)

structure DF : BUFFER =
struct
  type 'a buffer = 'a list
  exception EMPTY
  val empty = []
  fun get [] = raise EMPTY
    | get(x::l) = (x,l)
  fun put(x,l) = x::l
end


(* breadth-first state buffer -- implements an applicative queue *)

structure BF : BUFFER =
struct
  type 'a buffer = 'a list * 'a list
  exception EMPTY
  val empty = ([],[])
  fun get([],[]) = raise EMPTY
    | get(a::r,l) = (a,(r,l))
    | get([],l) = get(rev l,[])
  fun put(x,(m,l)) = (m,x::l)
end
