(* stringtab.sml *)

functor StringTable(type elem val default: elem) = 
struct

  type awkTable = elem ref Stringmap.stringmap

  fun new() = Stringmap.new(): awkTable

  fun set (t: awkTable) (s:string, x:elem) : unit =
      let val r = Stringmap.map t s in r := x end
      handle Stringmap.Stringmap => Stringmap.add t (s,ref x)

  fun get (t: awkTable) (s:string) : elem =
      !(Stringmap.map t s)
      handle Stringmap.Stringmap => default

  fun app (t: awkTable) (f: string * elem -> unit) =
      Stringmap.app (fn (s,ref e) => f(s,e)) t

end
