signature SPREADSHEET =
sig
    val dim : int
    val set : int * int * string -> unit   (* the ints here run from 0 to dim-1 *)
    val eval : unit -> unit
    val get : int * int -> string * int
end
