(* Copyright 1989 by AT&T Bell Laboratories *)
(* stampset.sml *)

structure Stampset: STAMPSET =
struct

  type stamp = int
  type stampset = {base: int, limit : int ref}
     (* represents a set of stamps *)
  type stampsets = {strStamps: stampset, tycStamps: stampset}
  type 'a stampmap = 'a array * int
     (* represents a finite mapping over a stampset to type 'a *)

  val nextbase = ref  0

  fun newStampset() : stampset =
    (* generate a new stampset, guaranteed to be disjoint from any
       existing stampset (unless a stampset has more than 10000 members!) *)
      let val base = !nextbase
       in nextbase := !nextbase + 10000;
          {base = base, limit = ref(base)}
      end

  fun newStampsets() : stampsets =
      {strStamps = newStampset(), tycStamps = newStampset()}

  fun member(s: stamp, {base,limit as ref lim}: stampset) : bool =
      (* tests membership in a stampset *)
      base <= s andalso s < lim

  fun newStamp({limit,...}: stampset) : stamp =
      (* generate a new member of the given stampset *)
      !limit before inc limit
      
  fun newMap({base,limit}: stampset, default: '1a) : '1a stampmap =
      (* generate a new stampmap over a given stampset with given default value *)
      (array(!limit-base,default),base)

  fun updateMap((a,b): 'a stampmap) (s: stamp, x: 'a) : unit =
      (* add mapping to a stampmap *)
      update(a,s-b,x)

  fun applyMap((a,b): 'a stampmap, s: stamp) : 'a =
      (* apply stampmap to a stamp *)
      a sub (s-b)

  fun join({base,limit} : stampset, {base=base',limit=limit'}: stampset)
      : stamp -> stamp =
      (* join(A,B) produces a translation function for elements of B and
         adds translated version of B to A, side-effecting A *)
     let val limit0 = !limit
      in limit := limit0 + (!limit' - base');
	 (fn s => (s - base') + limit0)
     end

  val fixedStrStamps : stampset = newStampset()
  val fixedTycStamps : stampset = newStampset()
  val globalStamps : stampsets =
       {strStamps = fixedStrStamps,
        tycStamps = fixedTycStamps}
  
  val sigStamps : stampset = newStampset()
 
  fun strFixed(s : stamp) : bool = member(s,fixedStrStamps)
  fun tycFixed(s : stamp) : bool = member(s,fixedTycStamps)

end (* STAMPSET *)
