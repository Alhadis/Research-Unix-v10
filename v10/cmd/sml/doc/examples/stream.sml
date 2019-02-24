(* stream.sml *)

(* Streams as a structure *)
(* The type is named Stream to avoid conflict with the i/o type stream *)

signature STREAM = sig
    type 'a Stream
    val lazyCons: (unit -> '1a * '1a Stream) -> '1a Stream
    and cons: '1a * '1a Stream -> '1a Stream
    and head: 'a Stream -> 'a
    and tail: 'a Stream -> 'a Stream
	and suffix : int -> 'a Stream -> 'a list
	and lazyMap : ('1a -> '1b) -> '1a Stream -> '1b Stream
end

structure Stream : STREAM = struct
    datatype 'a Stream = mkstream of 'a str ref
	 and 'a str = SOLID of 'a * 'a Stream
		    | SUSPENDED of unit -> ('a * 'a Stream);
    fun solidify(mkstream(ref(SOLID p))) = p 
      | solidify(mkstream(s as ref(SUSPENDED f))) =
	  let val p = f() in (s := SOLID p; p) end
    fun lazyCons(f) = mkstream(ref(SUSPENDED f))
    and cons(x,ss) = mkstream(ref(SOLID(x,ss)))
    and head s = let val (hd,_) = solidify s in hd end
    and tail s = let val (_,tl) = solidify s in tl end
    and suffix 0 s = nil
      | suffix n s = (head s)::(suffix (n-1) (tail s))
    fun lazyMap f s =
	let fun g (mkstream(ref(SOLID(x, s)))) = lazyCons(fn () => (f x, g s))
	      | g (s) = lazyCons
			 (fn () => let val (x, s') = solidify s in (f x, g s') end)
	 in lazyCons(fn () => solidify(g s))
	end
end
