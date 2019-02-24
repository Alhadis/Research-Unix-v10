structure S = Spreadsheet(Parse(Semant));
open S;

val say = output std_out
fun sayn (i:int) = say(makestring i)

fun show(i,j) = 
    let val (s,v) = get(i,j)
     in say "["; sayn i; say ","; sayn j;
	say "] = "; sayn v;
	say "    "; say s; say "\n"
    end;

(* set up a simple spreadsheet *)
set(0,0,"40");
set(1,0,"50");
set(2,0,"[0,0]+[1,0]");

eval();

app show [(0,0),(1,0),(2,0),(3,0)];
