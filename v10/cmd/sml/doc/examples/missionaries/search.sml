(* state searching module *)

signature BUFFER =
sig
  type 'a buffer
  exception EMPTY
  val empty: 'a buffer
  val get: 'a buffer -> 'a * 'a buffer
  val put: 'a * 'a buffer -> 'a buffer
end

signature STATE_SPACE =
sig
  type state
  val goal: state -> bool
  val moves: state -> state list
end

signature SEARCH =
sig
  structure States: STATE_SPACE
  val search: States.state -> unit
end

functor Search(structure States: STATE_SPACE and Buffer: BUFFER): SEARCH =
struct

  structure States = States

  open States Buffer

  fun search state = 
      let fun try pending =
	      let val (current,pending) = get pending
	       in if goal current
	          then (print "found"; ())
		  else try(fold put (moves current) pending)
	      end
       in try(put(state,empty))
	  handle EMPTY => (print "failed"; ())
      end

end  (* Search *)
