(* stringmap.sml *)

signature STRINGMAP =
  sig type 'a stringmap
      exception Stringmap
      val new : unit -> '1a stringmap
      val add : 'a stringmap -> string * 'a -> unit
      val rem : 'a stringmap -> string -> unit
      val map : 'a stringmap -> string -> 'a
      val app : (string * 'a -> unit) -> 'a stringmap -> unit
  end

structure Stringmap : STRINGMAP =
struct
  type 'a stringmap = (string * 'a) list array
  exception Stringmap
  val hashFactor = 5
  and tableSize = 211

  (* a string hashing function
     returns a number between 0 and tableSize-1 *)
  fun hash(str: string) : int =
      let val i = ref 0
	  and n = ref 0
	  and nchars = String.length str
       in while !i < nchars do
	      (n := (hashFactor * !n + ordof(str, !i)) mod tableSize;
	       i := !i + 1);
	  !n
      end

  (* create a new stringmap *)
  fun new (): '1a stringmap = array(tableSize,nil)

  (* add a mapping pair s +-> x to the stringmap a *)
  fun add a (s,x) = 
    let val index = hash s
     in update(a,index,(s,x)::(a sub index))
    end

  (* apply the stringmap a to the index string s *)
  fun map a s = 
    let fun find ((s',x)::r) = if s=s' then x else find r
	  | find nil = raise Stringmap
     in find (a sub (hash s))
    end

  (* remove all pairs mapping string s from stringmap a *)
  fun rem a s = let fun f ((b as (s',j))::r) = 
				if s=s' then f r else b :: f r
	              | f nil = nil
		    val index = hash s
		 in update(a,index, f(a sub index))
		end

  (* apply a function f to all mapping pairs in stringmap a *)
  fun app (f: string * 'a -> unit) a =
      let fun zap 0 = ()
	    | zap n = let val m = n-1 in List.app f (a sub m); zap m end
      in  zap tableSize
      end

end  (* Stringmap *)
