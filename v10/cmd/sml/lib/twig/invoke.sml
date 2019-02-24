structure Invoke =
  struct
    structure Main = Main

(* User Interface *)

    fun cut_off_last_char s = (implode o rev o tl o rev o explode) s

    fun user_interface () =
      (output std_out "ML-Twig version 1.0, June 1989\n\
       \Name of the specification file : "; flush_out std_out;
       cut_off_last_char(input_line std_in))

    val dummy = Main.main(user_interface())
end;
