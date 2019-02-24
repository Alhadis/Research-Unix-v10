To: research!dbm
Status: R

(* Find the intersection of a ray with a cylinder in three-space.  If
   there is more than one intersection, return the parameter of
   intersection closest to the start of the ray.  If there is no
   intersection, return NONE.

   The ray is specified by its starting point (rayStart) and direction
   vector (rayVec).  The cylinder is specified by a starting point (cStart)
   a length vector (cVec), and a radius (cRad).  The cylinder is finite;
   it does not extend beyond the direction vector at either end.

*)

local open Real
      val EP = 0.0000001

  fun newton f x =
    let val y = f x 
     in if abs(y)>EP then newton f (x+EP*y/(y-f(x+EP))) else x 
    end

  infix 7 @   fun (a,b,c) @ (d,e,f) = a*d+b*e+c*f     (* dot product *)
  infix 7 *!  fun (a,b,c) *! t = (a*t,b*t,c*t)	    (* vector * scalar *)
  infix 6 +!  fun (a,b,c) +! (d,e,f) = (a+d,b+e,c+f)  (* vector addition *)
  infix 6 -!  fun (a,b,c) -! (d,e,f) = (a-d,b-e,c-f)  (* vector subtraction *)

  fun along (C,D) t = C +! (D *! t)

in 

fun cylhit (ray as (rayStart, rayVec), cyl as (cStart, cVec), cRad) =
let val rayStart = rayStart -! cStart
    fun cylpos p = (cVec@p) / (cVec@cVec)
        (* parameter of closest point on cVec to the point p *)

    fun cyldist p = let val v = p -! (along cyl (cylpos p)) in v@v end
        (* distance of p from closest point on cVec *)

    fun cyldist' p = p*!(cVec@cVec) -! cVec*!(p@cVec)
	(* gradient of the cyldist function *)

    val tf = newton(fn t => cyldist' (along ray t) @ rayVec) 0.0
	(* parameter along the ray of the closest point to cVec *)

 in if cyldist (along ray tf) > cRad*cRad then NONE else
   let val t0 = newton(fn t => cyldist(along ray t)-(cRad*cRad)) 0.0
	(* one of the two intersection points *)
       val t1 = newton(fn t => cyldist(along ray t)-(cRad*cRad)) (tf+tf-t0)
	(* the other intersection point *)
       fun inRange v = v >= 0.0 andalso v <= 1.0
    in case (t0 > 0.0 andalso inRange (cylpos (along ray t0)),
	     t1 > 0.0 andalso inRange (cylpos (along ray t1)))
       of (false,false) => NONE
        | (true,false) => SOME t0
        | (false,true) => SOME t1
        | (true,true) => SOME (if (t0<t1) then t0 else t1)
   end
end

end
