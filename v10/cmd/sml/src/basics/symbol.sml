(* Copyright 1989 by AT&T Bell Laboratories *)
structure Symbol : SYMBOL = 
struct
    datatype symbol = Symbol of {name: string, number: int}
    fun eq(s1:symbol,s2) = (s1 = s2)
    fun symbol (name: string) =
	Symbol{name = name, number = StrgHash.hashString name}
    fun name (Symbol{name,...}) = name
    fun number (Symbol{number,...}) = number
end

