datatype limit = Unbounded | Limited of int

fun cat Unbounded (instream,outstream) =
    while not (end_of_stream instream) do
	  output outstream (input instream (can_input instream))
  | cat (Limited limit) (instream,outstream) =
    while not (end_of_stream instream) do
	  output outstream (input instream (min(can_input instream,limit)))
      
val ucat = cat Unbounded
val lcat = cat(Limited 1024)

fun cat1 (instream,outstream) =
    while not (end_of_stream instream) do
	  output outstream (input instream 1)

fun filecat cat = fn (fin,fout) => cat(open_in fin,open_out fout)
fun screencat cat = fn fin => cat(open_in fin,std_out)

