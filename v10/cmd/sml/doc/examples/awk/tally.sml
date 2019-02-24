structure Table = StringTable(type elem = int val default = 0);

val table = Table.new();

fun step (fields: string list) =
    let val name::value::_ = fields
     in Table.set table (name, Table.get table name + Awk.makeInt value)
    end

fun final () =
    Table.app table (fn (s,n) => (print s; print "\t"; print n; print "\n";()))

(* invoke by

   Awk.awk(stream,step,final)

*)


