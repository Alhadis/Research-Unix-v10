(* Union-find algorithm *)

datatype 'a setelem 
  = NILSET
  | ELEM of 'a * 'a setelem ref * int ref

exception SETINFO

fun new_setelem x = ELEM(x, ref nilset, ref 1)

fun set_union(NILSET, f) = f
  | set_union(e, NILSET) = e
  | set_union(e as ELEM(_,e_next,e_size), f as ELEM(_,f_next,f_size)) =
      if !e_size < !f_size
      then (f_size := !e_size + !f_size; e_next := f)
      else (e_size := !e_size + !f_size; f_next := e)

fun find NILSET = NILSET
  | find (e as ELEM(_,ref NILSET,_)) = e
  | find (ELEM(_,f,_)) = let g = find !f in (f := g; g) end

fun setinfo NILSET = raise SETINFO
  | setinfo e = let ELEM(x,_,_) = find e in x end
