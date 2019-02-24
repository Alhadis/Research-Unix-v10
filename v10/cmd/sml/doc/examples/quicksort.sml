 fun quick(le:'a*'a->bool) =
    let fun sort [] = []
           | sort [x] = [x]
           | sort (a::rest) = (* the head "a" is the pivot *)
               let fun split(left,right,[]) = sort left @ (a::sort right)
                        | split(left,right,x::l) =
                         if le(x,a) then split(x::left,right,l)
                                    else split(left,x::right,l)
               in split([],[],rest)
              end
      in sort
     end;
