
(* December 1988, Jussi Rintanen, Helsinki University of Technology *)

(* This is the interface between the abstract definition of ML Twig lexical
   analyzer and the actual implementation as a lexer specified with ML Lex. *)

signature LEXER =
  sig
    exception LexError
    datatype lexresult = IDENTIFIER of string | INT of string | EQ | RPAREN |
      LPAREN | COLON | SEMICOLON | COMMA | OTHER of string | SPACE of string |
      TREEREF of int list | EOF
      
    val make_lexer : instream -> (unit -> lexresult)
    val current_line : unit -> int
  end;
  
structure Lexer : LEXER =
  struct
    structure ActualLexer = TwigLexer
    open ActualLexer.UserDeclarations ActualLexer
    fun make_lexer stream = ActualLexer.makeLexer (input stream)
  end;
