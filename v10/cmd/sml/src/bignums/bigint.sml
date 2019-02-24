(* Copyright 1989 by AT&T Bell Laboratories *)
abstraction Bigint : BIGINT = struct

type bigint = bool list

fun bigint 0 = nil
  | bigint i = let val x = i div 2
		in (x+x<i)::bigint x
	       end

fun adwc(false::ar, false::br, carry) = carry::adwc(ar,br,false)
  | adwc(a::ar,  false::br, false) = a::adwc(ar,br,false)
  | adwc(false::ar, b::br, false) = b::adwc(ar,br,false)
  | adwc(true::ar, true::br, carry) = carry::adwc(ar,br,true)
  | adwc(a::ar, true::br, true) = a::adwc(ar,br,true)
  | adwc(true::ar, b::br, true) = b::adwc(ar,br,true)
  | adwc(nil, false::br, carry) = carry::adwc(nil,br,false)
  | adwc(nil, b::br, false) = b::adwc(nil,br,false)
  | adwc(nil, true::br, true) = false::adwc(nil,br,true)
  | adwc(nil, nil, false) = nil
  | adwc(nil, nil, true) = [true]
  | adwc(a, nil, carry) = adwc(nil, a, carry)

fun a + b = adwc(a,b,false)

fun a * nil = nil
  | (false::a) * b = a * (false::b)
  | (true::a) * b = b + a * (false::b)
  | nil * b = nil

fun getbit(a::r, 0) = a
  | getbit(_::r, i) = getbit(r,i-1)
  | getbit(nil, i) = false

fun >>(a,0) = a
  | >>(a::r,i) = >>(r,i-1)
  | >>(nil,i) = nil

fun size x = length x

end
