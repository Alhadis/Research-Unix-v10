.so ../ADM/mac
.XX dag 147 "Dag \(em A Program for Drawing Directed Graphs"
.		\" PE - end of picture
.de PE
.in
.X "END US PE
.nr X .65v
.if \\n($1>0 .X "SP \\nX PE"
.\"...bp
..
.EQ
delim $$
.EN
.ds HP +2
.ds DG \fIdag\fP
.ds PO P\s-2OST\s+2S\s-2CRIPT\s+2
.ds PC \fIpic\fP
.fp 5 CW
.\"..ND "October 19, 1987  Revised June 11, 1989"
.TL
Dag \(em A Program for Drawing Directed Graphs\(dg
.AU
E. R. Gansner
S. C. North
K. P. Vo
.AI
.MH
.AB
.PP
.I Dag
is a \*(PC or \*(PO preprocessor that draws directed graphs.
It works well on acyclic graphs and other graphs that can be drawn
as hierarchies.
Graph descriptions contain nodes, edges, and optional control statements.  
Here is a drawing of a graph from Forrester's book, \fIWorld Dynamics\fP
(Wright-Allen, Cambridge, MA, 1971).
It took 2.1 CPU seconds on a VAX-8650 to make this drawing.
.PS 3.5 3
arrowht = 0.111111;
arrowwid  = 0.055556;
define Ellipse	% [ellipse wid $2 ht $3 $1] %
.ps 4
Node0: Ellipse("S8",0.750000,0.500000) at (3.250000,7.250000);
Node1: Ellipse("9",0.750000,0.500000) at (3.250000,6.250000);
Node2: Ellipse("S24",0.750000,0.500000) at (1.138889,7.250000);
Node3: Ellipse("25",0.750000,0.500000) at (1.888889,6.250000);
Node4: Ellipse("27",0.750000,0.500000) at (0.375000,3.250000);
Node5: Ellipse("S1",0.750000,0.500000) at (15.083333,8.250000);
Node6: Ellipse("2",0.750000,0.500000) at (14.527778,7.250000);
Node7: Ellipse("10",0.750000,0.500000) at (15.638889,7.250000);
Node8: Ellipse("S35",0.750000,0.500000) at (10.319444,8.250000);
Node9: Ellipse("43",0.750000,0.500000) at (9.319444,7.250000);
Node10: Ellipse("36",0.750000,0.500000) at (11.319444,7.250000);
Node11: Ellipse("S30",0.750000,0.500000) at (8.069444,6.250000);
Node12: Ellipse("31",0.750000,0.500000) at (5.944444,4.250000);
Node13: Ellipse("33",0.750000,0.500000) at (8.444444,5.250000);
Node14: Ellipse("42",0.750000,0.500000) at (4.194444,5.250000);
Node15: Ellipse("T1",0.750000,0.500000) at (6.194444,1.250000);
Node16: Ellipse("26",0.750000,0.500000) at (3.111111,5.250000);
Node17: Ellipse("T24",0.750000,0.500000) at (2.416667,1.250000);
Node18: Ellipse("3",0.750000,0.500000) at (5.944444,5.250000);
Node19: Ellipse("16",0.750000,0.500000) at (14.486111,5.250000);
Node20: Ellipse("17",0.750000,0.500000) at (11.527778,6.250000);
Node21: Ellipse("18",0.750000,0.500000) at (12.194444,5.250000);
Node22: Ellipse("11",0.750000,0.500000) at (6.944444,5.250000);
Node23: Ellipse("14",0.750000,0.500000) at (16.527778,5.250000);
Node24: Ellipse("13",0.750000,0.500000) at (13.527778,6.250000);
Node25: Ellipse("12",0.750000,0.500000) at (12.944444,4.250000);
Node26: Ellipse("32",0.750000,0.500000) at (5.444444,3.250000);
Node27: Ellipse("T30",0.750000,0.500000) at (8.319444,2.250000);
Node28: Ellipse("34",0.750000,0.500000) at (7.444444,4.250000);
Node29: Ellipse("4",0.750000,0.500000) at (4.569444,4.250000);
Node30: Ellipse("15",0.750000,0.500000) at (13.694444,3.250000);
Node31: Ellipse("19",0.750000,0.500000) at (10.777778,5.250000);
Node32: Ellipse("29",0.750000,0.500000) at (9.944444,3.250000);
Node33: Ellipse("37",0.750000,0.500000) at (10.319444,7.250000);
Node34: Ellipse("39",0.750000,0.500000) at (12.527778,6.250000);
Node35: Ellipse("41",0.750000,0.500000) at (9.444444,4.250000);
Node36: Ellipse("38",0.750000,0.500000) at (5.444444,6.250000);
Node37: Ellipse("40",0.750000,0.500000) at (10.027778,6.250000);
Node38: Ellipse("23",0.750000,0.500000) at (4.444444,2.250000);
Node39: Ellipse("5",0.750000,0.500000) at (3.444444,3.250000);
Node40: Ellipse("21",0.750000,0.500000) at (8.444444,4.250000);
Node41: Ellipse("20",0.750000,0.500000) at (11.444444,4.250000);
Node42: Ellipse("28",0.750000,0.500000) at (10.444444,4.250000);
Node43: Ellipse("6",0.750000,0.500000) at (1.125000,2.250000);
Node44: Ellipse("T35",0.750000,0.500000) at (3.444444,2.250000);
Node45: Ellipse("22",0.750000,0.500000) at (4.444444,3.250000);
Node46: Ellipse("7",0.750000,0.500000) at (1.125000,1.250000);
Node47: Ellipse("T8",0.750000,0.500000) at (1.125000,0.250000);
spline ->  from (3.250000,7.000000) to (3.250000,6.500000);
spline ->  from (3.513889,6.069444) to (4.013889,5.472222);
spline ->  from (3.000000,6.055556) to (2.444444,5.333333) to (2.416667,5.166667) to (2.375000,5.000000) to (2.375000,2.500000) to (2.569444,2.208333) to (2.763889,1.916667) to (3.069444,1.875000) to (5.833333,1.319444);
spline ->  from (1.361111,7.041667) to (1.736111,6.472222);
spline ->  from (1.083333,7.000000) to (0.416667,3.500000);
spline ->  from (1.888889,6.000000) to (1.875000,4.500000) to (1.875000,4.250000) to (1.875000,4.000000) to (1.875000,2.500000) to (2.027778,2.194444) to (2.166667,1.888889) to (2.375000,1.875000) to (5.833333,1.305556);
spline ->  from (2.180556,6.097222) to (2.888889,5.444444);
spline ->  from (0.375000,3.000000) to (0.375000,2.500000) to (0.486111,2.194444) to (0.597222,1.902778) to (0.750000,1.875000) to (2.138889,1.416667);
spline ->  from (14.916667,8.027778) to (14.652778,7.486111);
spline ->  from (15.250000,8.027778) to (15.513889,7.486111);
spline ->  from (14.305556,7.055556) to (7.097222,6.625000) to (6.875000,6.611111) to (6.597222,6.361111) to (6.305556,6.125000) to (6.041667,5.486111);
spline ->  from (14.611111,7.000000) to (14.722222,6.500000) to (14.722222,6.277778) to (14.708333,6.069444) to (14.555556,5.500000);
spline ->  from (14.319444,7.041667) to (11.513889,6.500000);
spline ->  from (14.750000,7.041667) to (15.208333,6.347222) to (15.250000,6.166667) to (15.277778,6.000000) to (15.277778,2.500000) to (15.250000,2.375000) to (15.208333,2.250000) to (6.569444,1.291667);
spline ->  from (14.486111,7.000000) to (14.347222,6.305556) to (14.208333,6.097222) to (14.055556,5.902778) to (12.527778,5.361111);
spline ->  from (15.638889,7.000000) to (15.083333,6.888889) to (7.430556,6.611111) to (7.222222,6.305556) to (7.013889,6.000000) to (6.972222,5.500000);
spline ->  from (15.888889,7.055556) to (16.458333,6.333333) to (16.500000,6.166667) to (16.527778,6.000000) to (16.527778,5.500000);
spline ->  from (15.958333,7.125000) to (16.986111,6.444444) to (17.138889,6.222222) to (17.277778,6.000000) to (17.277778,2.500000) to (16.763889,2.208333) to (16.250000,1.916667) to (15.277778,1.875000) to (6.569444,1.277778);
spline ->  from (15.638889,7.000000) to (13.861111,6.375000);
spline ->  from (15.680556,7.000000) to (15.750000,6.500000) to (15.763889,6.250000) to (15.777778,6.000000) to (15.777778,5.500000) to (15.750000,5.263889) to (15.708333,5.041667) to (15.277778,4.958333) to (13.291667,4.347222);
spline ->  from (10.055556,8.069444) to (9.513889,7.458333);
spline ->  from (10.583333,8.069444) to (11.125000,7.458333);
spline ->  from (8.958333,7.180556) to (5.444444,6.500000);
spline ->  from (9.527778,7.041667) to (9.888889,6.486111);
spline ->  from (11.152778,7.027778) to (10.847222,6.388889) to (10.819444,6.194444) to (10.777778,6.000000) to (10.777778,5.500000);
spline ->  from (7.944444,6.013889) to (7.791667,5.375000) to (7.638889,5.138889) to (7.472222,4.916667) to (6.194444,4.444444);
spline ->  from (8.194444,6.013889) to (8.361111,5.500000);
spline ->  from (6.027778,4.000000) to (6.138889,3.500000) to (6.166667,3.250000) to (6.194444,3.000000) to (6.194444,1.500000);
spline ->  from (5.791667,4.027778) to (5.555556,3.486111);
spline ->  from (8.194444,5.069444) to (6.750000,4.500000) to (6.597222,4.402778) to (6.763889,4.180556) to (8.138889,2.472222);
spline ->  from (8.236111,5.041667) to (7.652778,4.458333);
spline ->  from (4.319444,5.013889) to (4.486111,4.500000);
spline ->  from (3.402778,5.097222) to (4.319444,4.430556);
spline ->  from (5.652778,5.097222) to (4.750000,4.472222);
spline ->  from (14.416667,5.000000) to (14.319444,4.500000) to (14.263889,4.333333) to (14.208333,4.166667) to (13.819444,3.486111);
spline ->  from (11.319444,6.041667) to (10.875000,5.486111);
spline ->  from (12.194444,5.000000) to (12.194444,4.500000) to (12.083333,4.194444) to (11.972222,3.902778) to (11.819444,3.875000) to (10.277778,3.361111);
spline ->  from (6.902778,5.000000) to (4.847222,4.416667);
spline ->  from (16.263889,5.069444) to (14.930556,4.291667) to (14.819444,4.222222) to (14.708333,4.152778) to (13.916667,3.444444);
spline ->  from (13.527778,6.000000) to (13.027778,5.902778) to (11.013889,5.444444);
spline ->  from (12.944444,4.000000) to (10.291667,3.347222);
spline ->  from (5.180556,3.069444) to (4.638889,2.458333);
spline ->  from (7.444444,4.000000) to (9.597222,3.347222);
spline ->  from (4.277778,4.083333) to (3.652778,3.458333);
spline ->  from (13.333333,3.180556) to (6.541667,1.347222);
spline ->  from (10.444444,5.138889) to (8.638889,4.458333);
spline ->  from (10.972222,5.041667) to (11.305556,4.486111);
spline ->  from (10.666667,5.013889) to (10.513889,4.500000);
spline ->  from (9.625000,3.125000) to (8.583333,2.430556);
spline ->  from (10.402778,7.000000) to (12.430556,6.486111);
spline ->  from (10.319444,7.000000) to (9.652778,6.625000) to (9.347222,6.458333) to (9.333333,6.222222) to (9.305556,6.000000) to (9.430556,4.500000);
spline ->  from (10.319444,7.000000) to (9.819444,6.888889) to (6.097222,6.597222) to (5.444444,6.500000);
spline ->  from (10.319444,7.000000) to (10.125000,6.486111);
spline ->  from (12.708333,6.027778) to (13.041667,5.388889) to (13.111111,5.277778) to (13.180556,5.166667) to (13.625000,4.375000) to (13.666667,4.180556) to (13.694444,4.000000) to (13.694444,3.500000);
spline ->  from (9.597222,4.027778) to (9.833333,3.486111);
spline ->  from (5.319444,6.013889) to (4.638889,4.500000);
spline ->  from (10.250000,6.041667) to (10.625000,5.472222);
spline ->  from (4.319444,2.013889) to (2.736111,1.375000);
spline ->  from (4.763889,2.111111) to (5.916667,1.416667);
spline ->  from (3.111111,3.138889) to (1.430556,2.388889);
spline ->  from (3.444444,3.000000) to (3.444444,2.500000);
spline ->  from (3.708333,3.069444) to (4.250000,2.458333);
spline ->  from (8.444444,4.000000) to (7.944444,3.888889) to (4.944444,3.611111) to (4.444444,3.500000);
spline ->  from (11.777778,4.138889) to (13.388889,3.402778);
spline ->  from (10.291667,4.027778) to (10.055556,3.486111);
spline ->  from (1.125000,2.000000) to (1.125000,1.500000);
spline ->  from (4.444444,3.000000) to (4.444444,2.500000);
spline ->  from (4.180556,3.069444) to (3.638889,2.458333);
spline ->  from (1.125000,1.000000) to (1.125000,0.500000);
.ps
.PE
.AE
.FS
\(dg This is a revised version of |reference(dag spe).
.FE
.NH 1
Introduction
.PP
Directed graphs have many applications in computing,
such as describing data structures, finite automata,
data flow, procedure calls, and software configuration dependencies. 
A picture is a good way to represent a directed graph.
It is seldom easy to understand much about a graph from a list of edges,
but with a picture one can quickly find individual nodes,
groups of related nodes, and trace paths in the graph.
The main obstacle is that it can be difficult and tedious to
draw graphs by hand.
.PP
.I Dag
is a program that draws directed graphs from edge lists.
It does particularly well on acyclic graphs such as trees, DAGs,
and other hierarchical graphs.
It reads descriptions in a concise language
and outputs drawings in \*(PC |reference(latest pic)
or \*(PO |reference(adobe postscript) format;
a typical use is
.P1
dag file | pic | troff
.P2
.........
.NH 1
Basics
.PP
This section describes how to give a basic description of a graph
as a list of edges.
Figure 1 shows a \*(DG description, and a reduced copy of the resulting picture.
.KS
.PS 6.5 4.5
arrowht = 0.111111;
arrowwid  = 0.055556;
define Ellipse	% [ellipse wid $2 ht $3 $1] %
.ps 4
box invis at (-10,0);
Node0: Ellipse("5th Edition",1.375000,0.500000) at (5.763889,10.250000);
Node1: Ellipse("6th Edition",1.375000,0.500000) at (4.111111,9.250000);
Node2: Ellipse("PWB 1.0",0.875000,0.500000) at (7.416667,9.250000);
Node3: Ellipse("Interdata",1.125000,0.500000) at (6.180556,8.250000);
Node4: Ellipse("Wollongong",1.250000,0.500000) at (3.388889,8.250000);
Node5: Ellipse("Mini Unix",1.125000,0.500000) at (4.819444,8.250000);
Node6: Ellipse("1 BSD",0.750000,0.500000) at (0.694444,8.250000);
Node7: Ellipse("LSX",0.750000,0.500000) at (2.138889,8.250000);
Node8: Ellipse("7th Edition",1.375000,0.500000) at (3.944444,7.250000);
Node9: Ellipse("Unix/TS 3.0",1.375000,0.500000) at (8.194444,5.250000);
Node10: Ellipse("PWB 2.0",0.875000,0.500000) at (7.166667,7.250000);
Node11: Ellipse("8th Edition",1.375000,0.500000) at (5.694444,2.250000);
Node12: Ellipse("32V",0.750000,0.500000) at (4.944444,6.250000);
Node13: Ellipse("V7M",0.750000,0.500000) at (1.111111,3.250000);
Node14: Ellipse("Ultrix-11",1.125000,0.500000) at (1.402778,1.250000);
Node15: Ellipse("Xenix",0.750000,0.500000) at (3.944444,6.250000);
Node16: Ellipse("UniPlus+",1.000000,0.500000) at (2.819444,6.250000);
Node17: Ellipse("9th Edition",1.375000,0.500000) at (6.555556,1.250000);
Node18: Ellipse("2 BSD",0.750000,0.500000) at (0.708333,5.250000);
Node19: Ellipse("2.8 BSD",0.875000,0.500000) at (2.652778,2.250000);
Node20: Ellipse("2.9 BSD",0.875000,0.500000) at (2.652778,1.250000);
Node21: Ellipse("3 BSD",0.750000,0.500000) at (4.944444,5.250000);
Node22: Ellipse("4 BSD",0.750000,0.500000) at (4.944444,4.250000);
Node23: Ellipse("4.1 BSD",0.875000,0.500000) at (4.638889,3.250000);
Node24: Ellipse("4.2 BSD",0.875000,0.500000) at (4.319444,2.250000);
Node25: Ellipse("4.3 BSD",0.875000,0.500000) at (5.180556,1.250000);
Node26: Ellipse("Ultrix-32",1.125000,0.500000) at (3.930556,1.250000);
Node27: Ellipse("PWB 1.2",0.875000,0.500000) at (7.430556,8.250000);
Node28: Ellipse("USG 1.0",0.875000,0.500000) at (9.555556,8.250000);
Node29: Ellipse("CB Unix 1",1.125000,0.500000) at (11.083333,7.250000);
Node30: Ellipse("USG 2.0",0.875000,0.500000) at (9.541667,7.250000);
Node31: Ellipse("CB Unix 2",1.125000,0.500000) at (11.083333,6.250000);
Node32: Ellipse("CB Unix 3",1.125000,0.500000) at (11.361111,5.250000);
Node33: Ellipse("Unix/TS++",1.125000,0.500000) at (10.319444,4.250000);
Node34: Ellipse("PDP-11 Sys V",1.500000,0.500000) at (11.930556,4.250000);
Node35: Ellipse("USG 3.0",0.875000,0.500000) at (9.569444,6.250000);
Node36: Ellipse("Unix/TS 1.0",1.375000,0.500000) at (8.194444,6.250000);
Node37: Ellipse("TS 4.0",0.750000,0.500000) at (10.263889,3.250000);
Node38: Ellipse("System V.0",1.250000,0.500000) at (10.263889,2.250000);
Node39: Ellipse("System V.2",1.250000,0.500000) at (10.263889,1.250000);
Node40: Ellipse("System V.3",1.250000,0.500000) at (10.263889,0.250000);
Node41: Ellipse("10th Edition",1.375000,0.500000) at (6.555556,0.250000);
spline ->  from (5.305556,10.069444) to (4.430556,9.472222);
spline ->  from (6.222222,10.069444) to (7.138889,9.444444);
spline ->  from (4.638889,9.097222) to (5.819444,8.444444);
spline ->  from (3.888889,9.013889) to (3.555556,8.486111);
spline ->  from (4.333333,9.013889) to (4.666667,8.486111);
spline ->  from (3.513889,9.125000) to (0.986111,8.402778);
spline ->  from (3.611111,9.083333) to (2.402778,8.430556);
spline ->  from (7.416667,9.000000) to (7.430556,8.500000);
spline ->  from (7.791667,9.125000) to (9.222222,8.416667);
spline ->  from (5.916667,8.027778) to (4.416667,7.430556);
spline ->  from (6.194444,8.000000) to (6.222222,7.500000) to (6.263889,7.250000) to (6.291667,7.000000) to (6.375000,6.500000) to (6.430556,6.305556) to (6.486111,6.111111) to (7.791667,5.458333);
spline ->  from (6.486111,8.041667) to (6.972222,7.472222);
spline ->  from (0.694444,8.000000) to (0.708333,5.500000);
spline ->  from (4.402778,7.055556) to (5.319444,6.625000) to (5.625000,6.486111) to (5.666667,6.236111) to (5.694444,6.000000) to (5.694444,2.500000);
spline ->  from (4.250000,7.027778) to (4.750000,6.458333);
spline ->  from (3.416667,7.083333) to (1.513889,6.486111) to (1.486111,6.236111) to (1.444444,6.000000) to (1.444444,5.500000) to (1.430556,5.250000) to (1.402778,5.000000) to (1.152778,3.500000);
spline ->  from (3.458333,7.069444) to (2.319444,6.625000) to (2.000000,6.500000) to (2.013889,6.250000) to (2.013889,6.000000) to (2.388889,4.500000) to (2.388889,4.333333) to (2.375000,4.166667) to (2.000000,3.500000) to (1.916667,3.250000) to (1.819444,3.000000) to (1.750000,2.500000) to (1.708333,2.291667) to (1.652778,2.083333) to (1.472222,1.500000);
spline ->  from (3.944444,7.000000) to (3.944444,6.500000);
spline ->  from (3.597222,7.027778) to (3.041667,6.472222);
spline ->  from (8.513889,5.027778) to (10.055556,3.458333);
spline ->  from (7.152778,7.000000) to (7.138889,6.500000) to (7.180556,6.263889) to (7.208333,6.041667) to (7.513889,5.875000) to (7.930556,5.486111);
spline ->  from (5.972222,2.027778) to (6.375000,1.486111);
spline ->  from (4.944444,6.000000) to (4.944444,5.500000);
spline ->  from (1.111111,3.000000) to (1.111111,2.500000) to (1.152778,2.277778) to (1.180556,2.069444) to (1.333333,1.500000);
spline ->  from (1.000000,5.097222) to (1.652778,4.458333) to (1.791667,4.291667) to (1.930556,4.138889) to (2.291667,3.388889) to (2.361111,3.222222) to (2.430556,3.069444) to (2.583333,2.500000);
spline ->  from (2.652778,2.000000) to (2.652778,1.500000);
spline ->  from (2.333333,2.083333) to (1.652778,1.472222);
spline ->  from (4.944444,5.000000) to (4.944444,4.500000);
spline ->  from (4.847222,4.013889) to (4.708333,3.500000);
spline ->  from (4.527778,3.013889) to (4.388889,2.500000);
spline ->  from (4.263889,3.111111) to (2.972222,2.430556);
spline ->  from (4.930556,3.055556) to (5.472222,2.486111);
spline ->  from (4.569444,2.041667) to (5.000000,1.472222);
spline ->  from (4.194444,2.013889) to (4.013889,1.500000);
spline ->  from (7.347222,8.000000) to (7.222222,7.500000);
spline ->  from (9.902778,8.097222) to (10.791667,7.458333);
spline ->  from (9.555556,8.000000) to (9.541667,7.500000);
spline ->  from (11.083333,7.000000) to (11.083333,6.500000);
spline ->  from (9.555556,7.000000) to (9.569444,6.500000);
spline ->  from (11.180556,6.000000) to (11.305556,5.500000);
spline ->  from (11.777778,5.083333) to (12.958333,4.486111) to (13.472222,4.111111) to (12.819444,3.902778) to (10.611111,3.347222);
spline ->  from (11.055556,5.041667) to (10.541667,4.486111);
spline ->  from (11.541667,5.013889) to (11.805556,4.500000);
spline ->  from (10.305556,4.000000) to (10.277778,3.500000);
spline ->  from (9.236111,6.083333) to (8.472222,5.472222);
spline ->  from (8.194444,6.000000) to (8.194444,5.500000);
spline ->  from (10.263889,3.000000) to (10.263889,2.500000);
spline ->  from (10.263889,2.000000) to (10.263889,1.500000);
spline ->  from (10.263889,1.000000) to (10.263889,0.500000);
spline ->  from (6.555556,1.000000) to (6.555556,0.500000);
.ps
.PE
.sp -6i


.ps 7p
.vs 9p
.ft 5
.nf
\&.GD 4 4
"5th Edition"	"6th Edition" "PWB 1.0";
"6th Edition"	"Interdata" "Wollongong"
		"Mini Unix" "1 BSD" "LSX";
"Interdata"	"7th Edition" "Unix/TS 3.0"
		"PWB 2.0";
"7th Edition"	"8th Edition" "32V" "V7M"
		"Ultrix-11" "Xenix" "UniPlus+";
"V7M"		"Ultrix-11";
"8th Edition"	"9th Edition";
"9th Edition"	"10th Edition";
"1 BSD"		"2 BSD";
"2 BSD"		"2.8 BSD";
"2.8 BSD"	"2.9 BSD" "Ultrix-11";
"32V"		"3 BSD";
"3 BSD"		"4 BSD";
"4 BSD"		"4.1 BSD";
"4.1 BSD"	"4.2 BSD" "2.8 BSD" "8th Edition";
"4.2 BSD"	"4.3 BSD" "Ultrix-32";
"PWB 1.0"	"PWB 1.2" "USG 1.0";
"PWB 1.2"	"PWB 2.0";
"USG 1.0"	"CB Unix 1" "USG 2.0";
"CB Unix 1"	"CB Unix 2";
"CB Unix 2"	"CB Unix 3";
"CB Unix 3"	"Unix/TS++" "PDP-11 Sys V";
"USG 2.0"	"USG 3.0";
"USG 3.0"	"Unix/TS 3.0";
"PWB 2.0"	"Unix/TS 3.0";
"Unix/TS 1.0"	"Unix/TS 3.0";
"Unix/TS 3.0"	"TS 4.0";
"Unix/TS++"	"TS 4.0";
"CB Unix 3"	"TS 4.0";
"TS 4.0"		"System V.0";
"System V.0"	"System V.2";
"System V.2"	"System V.3";
\&.GE
.ps
.vs
.ft
.fi
.ce
\fBFigure 1.\fR
.KE
.PP
Graph descriptions begin with \f5.GD\fP (or on some systems,
.CW .GS )
and end with \f5.GE\fP.
If the maximum width and height of the drawing are given on the 
\f5GD\fP line, the drawing is scaled appropriately.
The drawing in Figure 1 has a 4 inch by 4 inch bounding box.
.PP
A graph description is a list of semicolon-terminated statements.
All the statements in Figure 1 are \fIedge\fP statements
that name a tail node and a list of head nodes.
Edges fan out from the tail node.
The statement \f5"5th Edition" "6th Edition" "PWB 1.0";\fP
makes an edge from \f55th Edition\fP to \f56th Edition\fP,
and another from \f55th Edition\fP to \f5PWB 1.0\fP.
Because the syntax of edge statements is simple, graph descriptions can be
generated from the output of other tools, such as \fIcflow\fP, profilers,
or \fImake\fP utilities, without much effort.  There is also a verbose
form for edge statements:
.P1
edge from "5th Edition" to "6th Edition", to "PWB 1.0";
.P2
.PP
The keywords \f5edge\fP, \f5from\fP, \f5to\fP, and commas are optional.
Node names may be quoted to protect white space, punctuation,
or \*(DG keywords.
.PP
As shown, \*(DG places nodes in ranks so that edges
point downward if possible.  Nodes are ordered from
left to right within ranks to reduce edge crossings and long edges.
If there are cycles in the graph it is not possible for all edges to
point downward, so some edges will be inverted.
For instance, the following graph contains a cycle $a -> b ->c -> a$,
so the edge $c -> a$ points upward.
.KS
.ft 5
.sp .5i
.in 1i
.nf
\&.GD
edge from a to b;
edge from b to c;
edge from c to a;
\&.GE
.sp .5i
.in
.ft
.ce
\fBFigure 2.\fR
.fi
.sp -2.5i
.PS 5.5 2
arrowht = 0.069444;
arrowwid  = 0.027778;
define Ellipse	% [ellipse wid $2 ht $3 $1] %
box invis at (-2.5,0);
.ps 10
Node0: Ellipse("a",0.750000,0.500000) at (0.750000,2.250000);
Node1: Ellipse("b",0.750000,0.500000) at (1.125000,1.250000);
Node2: Ellipse("c",0.750000,0.500000) at (0.750000,0.250000);
spline ->  from (0.875000,2.013889) to (1.041667,1.500000);
spline ->  from (1.000000,1.013889) to (0.833333,0.500000);
spline ->  from (0.666667,0.500000) to (0.444444,1.111111) to (0.458333,1.305556) to (0.458333,1.500000) to (0.625000,2.013889);
.ps
.PE
.KE
Edges that point backward can be made:
.KS
.sp .5i
.P1
\&.GD
backedge from a to b;
edge from b to c;
edge from c to a;
\&.GE
.P2
.sp .5i
.ce
\fBFigure 3.\fR
.sp -2.5i
.PS 5.5 2
.ps 10
box invis at (-2.5,0);
arrowht = 0.069444;
arrowwid  = 0.027778;
define Ellipse	% [ellipse wid $2 ht $3 $1] %
.ps 10
Node0: Ellipse("a",0.750000,0.500000) at (0.750000,0.250000);
Node1: Ellipse("b",0.750000,0.500000) at (0.750000,2.250000);
Node2: Ellipse("c",0.750000,0.500000) at (1.125000,1.250000);
spline ->  from (0.875000,2.013889) to (1.041667,1.500000);
spline <-  from (0.625000,2.013889) to (0.458333,1.500000) to (0.458333,1.305556) to (0.444444,1.111111) to (0.666667,0.500000);
spline ->  from (1.000000,1.013889) to (0.833333,0.500000);
.ps
.PE
.KE
.PP
\fIPath\fP statements create chains of edges.
Here is a more compact way to describe part of Figure 1:
.P1
path from "Unix/TS 1.0" to "Unix/TS 3.0" to "TS 4.0" to "System V.0";
.P2
.PP
Beginning a graph description with \f5.GR\fP instead
of \f5.GD\fP makes edges point from left-to-right as 
shown in Figure 4.
(\f5GD\fP means ``graph down'' and \f5GR\fP ``graph right.'')
.KF
.PS 5
arrowht = 0.111111;
arrowwid  = 0.055556;
define Ellipse	% [ellipse wid $2 ht $3 $1] %
.ps 12
Node0: Ellipse("Newton",0.750000,0.500000) at (0.375000,2.125000);
Node1: Ellipse("Moore",0.750000,0.500000) at (1.625000,2.125000);
Node2: Ellipse("Veblen",0.750000,0.500000) at (3.000000,1.750000);
Node3: Ellipse("Birkhoff",1.000000,0.500000) at (3.000000,2.500000);
Node4: Ellipse("Whitney",0.875000,0.500000) at (4.430556,2.500000);
Node5: Ellipse("Church",0.750000,0.500000) at (4.430556,1.750000);
Node6: Ellipse("Turing",0.750000,0.500000) at (5.805556,0.250000);
Node7: Ellipse("Rosser",0.750000,0.500000) at (5.805556,3.250000);
Node8: Ellipse("Ritchie",0.875000,0.500000) at (5.805556,2.500000);
Node9: Ellipse("Kleene",0.750000,0.500000) at (5.805556,1.750000);
Node10: Ellipse("Scott",0.750000,0.500000) at (5.805556,1.000000);
spline ->  from (0.750000,2.125000) to (1.250000,2.125000);
spline ->  from (1.930556,2.277778) to (2.527778,2.416667);
spline ->  from (1.944444,2.000000) to (2.638889,1.833333);
spline ->  from (3.375000,1.750000) to (4.055556,1.750000);
spline ->  from (3.500000,2.500000) to (4.000000,2.500000);
spline ->  from (4.652778,1.555556) to (5.500000,1.152778);
spline ->  from (4.805556,1.750000) to (5.430556,1.750000);
spline ->  from (4.791667,1.805556) to (5.527778,2.305556);
spline ->  from (4.791667,1.819444) to (5.458333,3.166667);
spline ->  from (4.583333,1.513889) to (5.583333,0.458333);
.ps
.sp -.75i
.PE
.ce
\fBFigure 4.\fR
.KE
.PP
A backpath is a chain of back-edges.
.P1
backpath from Subaru to Honda to Ferrari;
.P2
.NH 1
Node Attributes
.PP
This section explains how to control the way nodes are drawn.
The node attributes are shape, size, label,
pointsize of the label, and color.
By default nodes are drawn as \(12\(aa\(aa by \(34\(aa\(aa ellipses
labeled with the node name in 14-point type.
The size may be increased to fit the label.
.PP
\f5draw nodes\fP sets default attributes for nodes created
afterward in a graph description;
nodes already created are not affected.
\f5draw\fP \fInodelist\fP sets attributes of individual nodes.
Nodes in the \fInodelist\fP are created if they do not already exist.
.NH 2
Node Shapes
.PP
The pre-defined shapes are \f5Box\fP, \f5Square\fP, \f5Circle\fP, \f5Doublecircle\fP, \f5Ellipse\fP, \f5Diamond\fP, and \f5Plaintext\fP.
Shape names are capitalized to avoid conflict with \*(PC keywords.
Here is an example involving node shapes:
.P1
edge from a to b;
draw ETA, Apollo, NeXT as Box;
draw nodes as Plaintext;
edge from x to y;
.P2
\f5a\fP and \f5b\fP are drawn as ellipses;
\f5ETA\fP, \f5Apollo\fP, and \f5NeXT\fP as boxes;
and \f5x\fP and \f5y\fP as \f5Plaintext\fP because
\f5Plaintext\fP became the default before \f5x\fP and \f5y\fP were created.
.PP
.EQ
delim off
.EN
User-defined node shapes are created by writing \*(PC macros or \*(PO procedures.
Shapes have three arguments: the node label, width, and height.
In a \*(PC macro these are \f5$1, $2,\fP and \f5$3\fP.
In a \*(PO procedure the arguments are passed on the stack.
To draw a node, \*(DG moves to its center point and calls the shape
with the necessary arguments.
It is usually convenient to define macros within the graph description
in a block between \f5.PS\fP and \f5.PE\fP. 
The contents of this block are passed straight through \*(DG
and appear in the graphics code after the standard prologue but
before any nodes or edges are drawn.
.PP
There are two limitations on user-defined shapes.
First, \*(DG assumes height and width are independent.
So if a user-defined shape has a fixed aspect ratio,
fine-tuning may be needed to make the drawing look right.
Second, because \*(DG doesn't know how to compute the boundary
of user-defined shapes, and \*(PC macros aren't general enough for the job,
edges in \*(PC drawings are clipped to a node's bounding rectangle.
The second limitation does not apply to shapes written in \*(PO
since the user also supplies a clipping procedure (see Appendix B).
Figure 5 shows an example shape macro in \*(PC.
.KF
.sp .3i
.P1
\&.GD
\&.PS
define Triplecircle % [
		circle rad .5   * $2;
		circle rad .475 * $2 at last circle.c;
		circle rad .45  * $2 at last circle.c;
		$1 at last circle.c;
	] %
\&.PE
edge from 1 to 2;
draw 1 as Triplecircle height 1 width 1;
draw 2 as Circle height 1 width 1;
\&.GE
.P2
.sp .4i
.ce
\fBFigure 5.\fR
.sp -3i
.PS 6
box invis at (-5,0);
arrowht = 0.111111;
arrowwid  = 0.055556;
define Circle	% [circle rad $2/2; $1 at last circle.c] %
define Triplecircle % [ circle rad .5   * $2; circle rad .475 * $2 at last circle.c; circle rad .45  * $2 at last circle.c; $1 at last circle.c; ] %
.ps 14
Node0: Triplecircle("1",1.000000,1.000000) at (0.500000,2.000000);
Node1: Circle("2",1.000000,1.000000) at (0.500000,0.500000);
spline -> solid from (0.500000,1.500000) to (0.500000,1.000000);
.ps
.PE
.KE
.......
.NH 2
Node Labels
.PP
The default label of a node is its name.
Another label can be set, which is particularly
useful to allow distinct nodes to share a common label.
.P1
draw "/usr/src/cmd" label "cmd";
draw "/usr/local/src/cmd" label "cmd";
.P2
.EQ
delim $$
.EN
.NH 2
Node Colors
.PP
Nodes may have color in \*(PO drawings.
A color value is a string that is passed to the
procedure \f5dagsetcolor\fP in the emitted \*(PO code.
In the standard \*(PO prologue this procedure
accepts a hue-saturation-brightness triple. 
Macros for common colors can be defined in a \f5.PS/.PE\fP block:
.P1
\&.PS
/red [.1 1 1] def
\&.PE
draw important_node color red;
.P2
Other interpretations of color values may be implemented
by redefining \f5dagsetcolor\fP and defining compatible node shapes.
Appendix B explains how to make gray-scale shaded nodes this way.
.NH 1
Edge Attributes
.PP
This section explains how to control the way edges are drawn.
The edge attributes are label,
pointsize of the label, ink style, color, and weight.
Attributes may be set when edges are created.
\f5draw edges\fP sets default attributes for edges
created afterward in the graph description.
Previously created edges cannot be changed because
unlike nodes, edges do not have identifiers.
.NH 2
Edge Labels
.PP
A label is a text string or fragment of executable graphics code 
drawn at the midpoint of an edge.  Text labels are preferred.
Graphics code is available for extreme cases, but it is discouraged
because it is dependent on the target graphics language.
Graphics code is enclosed in curly braces as shown below.
.P1
edge from a to b label "x" ;
edge from b to c label {circle rad .1};
.P2
.PP
Figure 6 is the description of a finite automaton with labeled edges
and its picture.
.EQ
delim off
.EN
.KF
.P1
\&.GR 6
draw nodes as Circle width .5 height .5;
draw LR_0 LR_3 LR_4 LR_8 as Doublecircle width .5 height .5;
draw edges pointsize 8;
LR_0 LR_2 label "SS(B)";
LR_0 LR_1 label "SS(S)";
LR_1 LR_3 label "S($end)";
LR_2 LR_6 label "SS(b)";
LR_2 LR_5 label "SS(a)";
LR_2 LR_4 label "S(A)";
LR_5 LR_7 label "S(b)";
LR_5 LR_5 label "S(a)";
LR_6 LR_6 label "S(b)";
LR_6 LR_5 label "S(a)";
LR_7 LR_8 label "S(b)";
LR_7 LR_5 label "S(a)";
LR_8 LR_6 label "S(b)";
LR_8 LR_5 label "S(a)";
\&.GE
.P2
.PS 6
arrowht = 0.069444;
arrowwid  = 0.027778;
define Box	% [box wid $2 ht $3 $1] %
define Square	% [box wid $2 ht $2 $1] %
define Circle	% [circle rad $2/2; $1 at last circle.c] %
define Ellipse	% [ellipse wid $2 ht $3 $1] %
define Diamond	% [move down $3/2; line up $3/2 right $2/2 then up $3/2 left $2/2 then down $3/2 left $2/2 then down $3/2 right $2/2; move up $3/2; $1;] %
define Doublecircle % [circle rad $2/2; circle rad .9*$2/2 at last circle.c; $1 at last circle.c] %
define Plaintext % [box invis wid $2 ht $3 $1] %
.ps 11
Node0: Doublecircle("LR_0",0.500000,0.500000) at (0.250000,1.972222);
Node1: Doublecircle("LR_3",0.500000,0.500000) at (2.250000,2.625000);
Node2: Doublecircle("LR_4",0.500000,0.500000) at (2.250000,0.875000);
Node3: Doublecircle("LR_8",0.500000,0.500000) at (4.250000,1.375000);
Node4: Circle("LR_2",0.500000,0.500000) at (1.250000,1.319444);
Node5: Circle("LR_1",0.500000,0.500000) at (1.250000,2.625000);
Node6: Circle("LR_6",0.500000,0.500000) at (5.250000,0.750000);
Node7: Circle("LR_5",0.500000,0.500000) at (2.250000,1.625000);
Node8: Circle("LR_7",0.500000,0.500000) at (3.250000,1.875000);
.ps
.ps 8
spline ->  from (0.430556,2.152778) to (1.027778,2.500000);
move to (0.750000,2.333333); "SS(S)";
spline ->  from (0.430556,1.791667) to (1.027778,1.444444);
move to (0.750000,1.597222); "SS(B)";
spline ->  from (4.000000,1.347222) to (3.500000,1.291667) to (3.250000,1.319444) to (3.000000,1.333333) to (2.472222,1.513889);
move to (3.250000,1.319444); "S(a)";
spline ->  from (4.430556,1.208333) to (5.027778,0.875000);
move to (4.750000,1.027778); "S(b)";
spline ->  from (1.458333,1.180556) to (2.013889,0.972222);
move to (1.750000,1.069444); "S(A)";
spline ->  from (1.486111,1.416667) to (2.013889,1.555556);
move to (1.750000,1.486111); "SS(a)";
spline ->  from (1.388889,1.111111) to (1.875000,0.625000) to (2.180556,0.319444) to (2.347222,0.291667) to (2.500000,0.250000) to (4.000000,0.250000) to (4.180556,0.291667) to (4.361111,0.319444) to (5.027778,0.638889);
move to (3.208333,0.250000); "SS(b)";
spline ->  from (1.500000,2.625000) to (2.000000,2.625000);
move to (1.750000,2.625000); "S($end)";
spline ->  from (5.000000,0.750000) to (3.500000,0.750000) to (3.333333,0.791667) to (3.166667,0.819444) to (2.402778,1.416667);
move to (3.722222,0.750000); "S(a)";
spline ->  from (5.194444,1.000000) to (5.138889,1.125000) to (5.250000,1.250000) to (5.361111,1.125000) to (5.305556,1.000000);
move to (5.250000,1.250000); "S(b)";
spline ->  from (2.194444,1.875000) to (2.138889,2.000000) to (2.250000,2.125000) to (2.361111,2.000000) to (2.305556,1.875000);
move to (2.250000,2.125000); "S(a)";
spline ->  from (2.486111,1.708333) to (2.750000,1.666667) to (3.000000,1.819444);
move to (2.750000,1.666667); "S(b)";
spline ->  from (3.000000,1.819444) to (2.750000,1.861111) to (2.486111,1.708333);
move to (2.750000,1.861111); "S(a)";
spline ->  from (3.458333,1.736111) to (4.027778,1.472222);
move to (3.750000,1.597222); "S(b)";
.ps
.PE
.sp -1.2i
.ce
\fBFigure 6.\fR
.KE
.EQ
delim $$
.EN
.NH 2
Edge Weights
.PP
Edge weights or costs are integers.
Increasing the weight of an edge makes \*(DG try to shorten it.
The default weight is 1;  \*(DG internally increases the
weight of long edges to improve the drawing.
In the Unix History graph of Figure 1, we might want to favor the edge
\f5"2 BSD" "2.8 BSD"\fP over \f5"4.1 BSD" "2.8 BSD"\fP.  This suggests:
.P1
edge from "2 BSD" to "2.8 BSD" weight 1000;
.P2
.PP
It's best to use large values to swamp the internal adjustment,
otherwise a little experimenting with weights will probably be needed.
.NH 2
Edge Styles
.PP
The graph description may name the ``ink'' or line style
for edges: \f5solid\fP, \f5dotted\fP, \f5dashed\fP, or \f5invis\fP. 
.P1
edge from "2 BSD" to "2.8 BSD" dashed;
.P2
.PP
Unfortunately \fItroff\fP does not have dotted or dashed splines
so they appear as solid splines in \*(PC output.
.NH 2
Edge Colors
.PP
Edge colors are much like node colors.
.P1
edge from "2 BSD" to "2.8 BSD" color "[.2 1 1 ]";
draw edges "[.3 .5 .5]";
.P2
.NH 1
Spacing Control
.PP
\fISeparate\fP statements set the minimum spacing in inches between
adjacent nodes or ranks.
.P1
separate nodes .75;
separate ranks 2;
separate ranks 2 exactly;
separate ranks 2 equally;
.P2
.PP
Because some drawings have edges that are almost horizontal
and thus hard to read, \*(DG may increase the separation
between certain ranks to improve the drawing.
When this happens, a side-effect is that ranks are not equally spaced.
The
.CW equally
qualifier
tells \*(DG to maintain even
spacing between ranks, which may be more aesthetically pleasing and readable;
.CW exactly
inhibits the rank spacing adjustment.
.PP
Another kind of spacing control is requested by putting the
keyword \f5fill\fP on the \f5GD\fP line.  This forces the drawing
to fill the bounding box by increasing the node and rank separations.
\&\f5fill\fP overrides ``\f5separate ranks exactly\fP''.  For example:
.P1
\&.GD 6 8 fill
.P2
.NH 1
Rank Assignment Control
.PP
The graph description may state that certain nodes should be
placed on the minimum or maximum rank,
or be kept as a group on the same rank.
.P1
minimum rank root1 root2 root3;
maximum rank leaf38 leaf39;
same rank add sub mul div shift;
.P2
.PP
Figure 7 is another drawing of Forrester's \fIWorld Dynamics\fP graph.
In this drawing, (1) source nodes
\fIS1, S8, S24, S30\fP and \fIS35\fP
are constrained to appear at the minimum rank, (2) target nodes 
\fIT1, T8, T24, T30\fP and \fIT35\fP are to appear at the maximum rank, and (3)
all other nodes stay at the same ranks as in the drawing of the abstract.
.KF
.PS 3.5
arrowht = 0.111111;
arrowwid  = 0.055556;
define Ellipse	% [ellipse wid $2 ht $3 $1] %
.ps 4
Node0: Ellipse("S8",0.750000,0.500000) at (4.444444,7.250000);
Node1: Ellipse("S24",0.750000,0.500000) at (2.097222,7.250000);
Node2: Ellipse("S1",0.750000,0.500000) at (8.319444,7.250000);
Node3: Ellipse("S35",0.750000,0.500000) at (10.819444,7.250000);
Node4: Ellipse("S30",0.750000,0.500000) at (17.000000,7.250000);
Node5: Ellipse("T8",0.750000,0.500000) at (0.375000,0.250000);
Node6: Ellipse("T24",0.750000,0.500000) at (2.041667,0.250000);
Node7: Ellipse("T1",0.750000,0.500000) at (7.569444,0.250000);
Node8: Ellipse("T35",0.750000,0.500000) at (3.041667,0.250000);
Node9: Ellipse("T30",0.750000,0.500000) at (16.458333,0.250000);
Node10: Ellipse("9",0.750000,0.500000) at (4.444444,6.250000);
Node11: Ellipse("25",0.750000,0.500000) at (2.569444,6.250000);
Node12: Ellipse("27",0.750000,0.500000) at (1.375000,1.250000);
Node13: Ellipse("2",0.750000,0.500000) at (8.263889,6.250000);
Node14: Ellipse("10",0.750000,0.500000) at (13.944444,6.250000);
Node15: Ellipse("43",0.750000,0.500000) at (11.319444,6.250000);
Node16: Ellipse("36",0.750000,0.500000) at (10.319444,6.250000);
Node17: Ellipse("31",0.750000,0.500000) at (18.194444,3.250000);
Node18: Ellipse("33",0.750000,0.500000) at (17.569444,4.250000);
Node19: Ellipse("42",0.750000,0.500000) at (4.819444,5.250000);
Node20: Ellipse("26",0.750000,0.500000) at (3.319444,5.250000);
Node21: Ellipse("3",0.750000,0.500000) at (5.819444,5.250000);
Node22: Ellipse("16",0.750000,0.500000) at (8.319444,4.250000);
Node23: Ellipse("17",0.750000,0.500000) at (9.513889,5.250000);
Node24: Ellipse("18",0.750000,0.500000) at (9.319444,4.250000);
Node25: Ellipse("11",0.750000,0.500000) at (6.819444,5.250000);
Node26: Ellipse("14",0.750000,0.500000) at (11.694444,4.250000);
Node27: Ellipse("13",0.750000,0.500000) at (13.569444,5.250000);
Node28: Ellipse("12",0.750000,0.500000) at (16.069444,4.250000);
Node29: Ellipse("32",0.750000,0.500000) at (14.458333,2.250000);
Node30: Ellipse("34",0.750000,0.500000) at (17.013889,3.250000);
Node31: Ellipse("4",0.750000,0.500000) at (5.319444,4.250000);
Node32: Ellipse("15",0.750000,0.500000) at (8.819444,2.250000);
Node33: Ellipse("19",0.750000,0.500000) at (10.319444,4.250000);
Node34: Ellipse("29",0.750000,0.500000) at (13.347222,2.250000);
Node35: Ellipse("37",0.750000,0.500000) at (12.444444,6.250000);
Node36: Ellipse("39",0.750000,0.500000) at (12.152778,3.250000);
Node37: Ellipse("41",0.750000,0.500000) at (14.569444,4.250000);
Node38: Ellipse("38",0.750000,0.500000) at (11.069444,5.250000);
Node39: Ellipse("40",0.750000,0.500000) at (12.069444,5.250000);
Node40: Ellipse("23",0.750000,0.500000) at (3.375000,1.250000);
Node41: Ellipse("5",0.750000,0.500000) at (2.375000,3.250000);
Node42: Ellipse("21",0.750000,0.500000) at (4.055556,3.250000);
Node43: Ellipse("20",0.750000,0.500000) at (8.819444,3.250000);
Node44: Ellipse("28",0.750000,0.500000) at (13.152778,3.250000);
Node45: Ellipse("6",0.750000,0.500000) at (0.375000,2.250000);
Node46: Ellipse("22",0.750000,0.500000) at (3.375000,2.250000);
Node47: Ellipse("7",0.750000,0.500000) at (0.375000,1.250000);
spline ->  from (4.444444,7.000000) to (4.444444,6.500000);
spline ->  from (2.236111,7.013889) to (2.458333,6.486111);
spline ->  from (1.972222,7.013889) to (1.819444,6.500000) to (1.777778,6.250000) to (1.722222,6.000000) to (1.388889,1.500000);
spline ->  from (8.305556,7.000000) to (8.277778,6.500000);
spline ->  from (8.583333,7.069444) to (13.194444,6.583333) to (13.944444,6.500000);
spline ->  from (10.972222,7.027778) to (11.208333,6.486111);
spline ->  from (10.666667,7.027778) to (10.430556,6.486111);
spline ->  from (16.944444,7.000000) to (16.861111,6.500000) to (16.847222,6.250000) to (16.819444,6.000000) to (16.819444,4.500000) to (16.847222,4.250000) to (16.861111,4.000000) to (17.194444,3.875000) to (17.916667,3.416667);
spline ->  from (17.138889,7.013889) to (17.347222,6.430556) to (17.402778,6.208333) to (17.444444,6.000000) to (17.555556,4.500000);
spline ->  from (4.569444,6.013889) to (4.736111,5.500000);
spline ->  from (4.319444,6.013889) to (4.152778,5.500000) to (4.152778,5.291667) to (4.138889,5.083333) to (4.750000,3.430556) to (4.791667,3.208333) to (4.819444,3.000000) to (4.819444,1.500000) to (4.861111,1.361111) to (4.888889,1.236111) to (7.236111,0.375000);
spline ->  from (2.569444,6.000000) to (2.569444,5.500000) to (2.611111,5.291667) to (2.638889,5.083333) to (3.194444,3.500000) to (3.277778,3.319444) to (3.347222,3.152778) to (3.680556,2.875000) to (4.055556,2.347222) to (4.111111,2.166667) to (4.152778,2.000000) to (4.194444,1.500000) to (4.375000,1.208333) to (4.555556,0.916667) to (4.819444,0.875000) to (7.208333,0.333333);
spline ->  from (2.791667,6.041667) to (3.166667,5.472222);
spline ->  from (1.569444,1.041667) to (1.902778,0.486111);
spline ->  from (7.916667,6.152778) to (6.000000,5.472222);
spline ->  from (8.236111,6.000000) to (8.208333,5.500000) to (8.222222,5.250000) to (8.236111,5.000000) to (8.291667,4.500000);
spline ->  from (8.555556,6.097222) to (9.291667,5.444444);
spline ->  from (8.069444,6.027778) to (7.694444,5.375000) to (7.652778,5.180556) to (7.611111,5.000000) to (7.583333,4.500000) to (7.583333,4.250000) to (7.569444,4.000000) to (7.569444,0.500000);
spline ->  from (8.402778,6.013889) to (8.597222,5.500000) to (8.680556,5.333333) to (8.763889,5.166667) to (9.180556,4.486111);
spline ->  from (13.944444,6.000000) to (13.194444,5.902778) to (6.986111,5.472222);
spline ->  from (14.069444,6.013889) to (14.236111,5.500000) to (14.402778,4.972222) to (14.097222,4.902778) to (12.041667,4.347222);
spline ->  from (14.250000,6.097222) to (15.250000,5.305556) to (15.291667,5.152778) to (15.319444,5.000000) to (15.319444,1.500000) to (15.291667,1.375000) to (15.250000,1.250000) to (7.930556,0.305556);
spline ->  from (13.944444,6.000000) to (13.694444,5.486111);
spline ->  from (14.277778,6.138889) to (16.000000,5.291667) to (16.041667,5.138889) to (16.069444,5.000000) to (16.069444,4.500000);
spline ->  from (11.236111,6.000000) to (11.125000,5.500000);
spline ->  from (11.541667,6.041667) to (11.916667,5.472222);
spline ->  from (10.319444,6.000000) to (10.319444,4.500000);
spline ->  from (18.194444,3.000000) to (17.083333,1.083333) to (16.611111,1.000000) to (16.138889,0.916667) to (7.944444,0.277778);
spline ->  from (18.194444,3.000000) to (17.611111,2.902778) to (14.819444,2.333333);
spline ->  from (17.861111,4.097222) to (18.569444,3.625000) to (18.847222,3.444444) to (18.902778,3.222222) to (18.944444,3.000000) to (18.944444,1.500000) to (18.916667,1.361111) to (18.875000,1.222222) to (16.777778,0.375000);
spline ->  from (17.402778,4.027778) to (17.138889,3.486111);
spline ->  from (4.972222,5.027778) to (5.208333,4.486111);
spline ->  from (3.652778,5.138889) to (5.027778,4.402778);
spline ->  from (5.666667,5.027778) to (5.402778,4.500000);
spline ->  from (8.236111,4.000000) to (8.125000,3.500000) to (8.138889,3.333333) to (8.138889,3.180556) to (8.444444,2.875000) to (8.680556,2.486111);
spline ->  from (9.736111,5.055556) to (10.152778,4.472222);
spline ->  from (9.555556,4.055556) to (10.527778,3.361111) to (10.750000,3.208333) to (10.972222,3.069444) to (13.013889,2.361111);
spline ->  from (6.583333,5.055556) to (5.541667,4.458333);
spline ->  from (11.375000,4.125000) to (10.347222,3.472222) to (10.138889,3.333333) to (9.916667,3.208333) to (9.055556,2.444444);
spline ->  from (13.347222,5.041667) to (10.513889,4.458333);
spline ->  from (16.055556,4.000000) to (16.041667,3.500000) to (16.013889,3.361111) to (15.972222,3.236111) to (13.347222,2.500000);
spline ->  from (14.458333,2.000000) to (13.902778,1.875000) to (3.750000,1.277778);
spline ->  from (16.958333,3.000000) to (13.902778,2.597222) to (13.347222,2.500000);
spline ->  from (4.972222,4.166667) to (2.708333,3.361111);
spline ->  from (8.625000,2.027778) to (7.708333,0.486111);
spline ->  from (10.319444,4.000000) to (9.819444,3.888889) to (4.430556,3.291667);
spline ->  from (10.319444,4.000000) to (9.125000,3.388889);
spline ->  from (10.513889,4.041667) to (13.138889,3.500000);
spline ->  from (13.569444,2.041667) to (14.013889,1.361111) to (14.083333,1.291667) to (14.152778,1.222222) to (16.138889,0.375000);
spline ->  from (12.555556,6.013889) to (12.736111,5.500000) to (12.777778,5.250000) to (12.819444,5.000000) to (12.819444,4.500000) to (12.791667,4.333333) to (12.750000,4.166667) to (12.305556,3.472222);
spline ->  from (12.763889,6.125000) to (14.319444,5.472222) to (14.750000,5.291667) to (14.750000,5.138889) to (14.736111,5.000000) to (14.625000,4.500000);
spline ->  from (12.138889,6.097222) to (11.319444,5.444444);
spline ->  from (12.319444,6.013889) to (12.152778,5.500000);
spline ->  from (11.791667,3.166667) to (9.166667,2.347222);
spline ->  from (14.375000,4.027778) to (14.027778,3.375000) to (13.958333,3.263889) to (13.888889,3.166667) to (13.347222,2.500000);
spline ->  from (11.069444,5.000000) to (10.513889,4.930556) to (5.652778,4.361111);
spline ->  from (11.888889,5.027778) to (10.458333,4.486111);
spline ->  from (3.069444,1.097222) to (2.277778,0.444444);
spline ->  from (3.375000,1.000000) to (3.986111,0.902778) to (7.208333,0.319444);
spline ->  from (2.041667,3.138889) to (0.666667,2.402778);
spline ->  from (2.291667,3.000000) to (2.180556,2.500000) to (2.152778,2.250000) to (2.125000,2.000000) to (2.125000,1.500000) to (2.166667,1.347222) to (2.194444,1.194444) to (2.847222,0.458333);
spline ->  from (2.458333,3.000000) to (2.569444,2.500000) to (2.638889,2.333333) to (2.694444,2.180556) to (3.000000,1.875000) to (3.236111,1.486111);
spline ->  from (3.861111,3.041667) to (3.513889,2.486111);
spline ->  from (8.819444,3.000000) to (8.819444,2.500000);
spline ->  from (13.222222,3.000000) to (13.305556,2.500000);
spline ->  from (0.375000,2.000000) to (0.375000,1.500000);
spline ->  from (3.375000,2.000000) to (3.375000,1.500000);
spline ->  from (3.152778,2.041667) to (2.694444,1.347222) to (2.694444,1.236111) to (2.694444,1.125000) to (2.944444,0.486111);
spline ->  from (0.375000,1.000000) to (0.375000,0.500000);
.ps
.PE
.ce
\fBFigure 7.\fR
.KE
.PP
Figure 8 illustrates how a time-line can be
created with \f5same rank\fP statements.
.KF
.PS 5.346320
arrowht = 0.111111;
arrowwid  = 0.055556;
define Box	% [box wid $2 ht $3 $1] %
define Plaintext % [box invis wid $2 ht $3 $1] %
.ps 9
Node0: Plaintext("1971",1.069444,0.597222) at (0.805556,18.958333);
Node1: Plaintext("1972",1.069444,0.597222) at (0.805556,17.861111);
Node2: Plaintext("1973",1.069444,0.597222) at (0.805556,16.763889);
Node3: Plaintext("1974",1.069444,0.597222) at (0.805556,15.666667);
Node4: Plaintext("1975",1.069444,0.597222) at (0.805556,14.569444);
Node5: Plaintext("1976",1.069444,0.597222) at (0.805556,13.472222);
Node6: Plaintext("1977",1.069444,0.597222) at (0.805556,12.375000);
Node7: Plaintext("1978",1.069444,0.597222) at (0.805556,11.277778);
Node8: Plaintext("1979",1.069444,0.597222) at (0.805556,10.180556);
Node9: Plaintext("1980",1.069444,0.597222) at (0.805556,9.083333);
Node10: Plaintext("1981",1.069444,0.597222) at (0.805556,7.986111);
Node11: Plaintext("1982",1.069444,0.597222) at (0.805556,6.888889);
Node12: Plaintext("1983",1.069444,0.597222) at (0.805556,5.791667);
Node13: Plaintext("1984",1.069444,0.597222) at (0.805556,4.694444);
Node14: Plaintext("1985",1.069444,0.597222) at (0.805556,3.597222);
Node15: Plaintext("1986",1.069444,0.597222) at (0.805556,2.500000);
Node16: Plaintext("1987",1.069444,0.597222) at (0.805556,1.402778);
Node17: Plaintext("future",1.611111,0.597222) at (0.805556,0.305556);
Node18: Box("Thompson sh",2.958333,0.597222) at (10.194444,18.958333);
Node19: Box("Ritchie cpp",2.958333,0.597222) at (15.666667,17.861111);
Node20: Box("SCCS",1.069444,0.597222) at (3.166667,15.666667);
Node21: Box("make",1.069444,0.597222) at (6.833333,13.472222);
Node22: Box("Bourne sh",2.416667,0.597222) at (11.708333,13.472222);
Node23: Box("Mashey sh",2.416667,0.597222) at (8.986111,13.472222);
Node24: Box("Reiser cpp",2.694444,0.597222) at (15.666667,11.277778);
Node25: Box("augmented make",3.777778,0.597222) at (5.236111,11.277778);
Node26: Box("Form sh",1.888889,0.597222) at (12.361111,11.277778);
Node27: Box("Csh",0.805556,0.597222) at (8.986111,11.277778);
Node28: Box("build",1.347222,0.597222) at (6.666667,10.180556);
Node29: Box("emacs",1.347222,0.597222) at (13.208333,9.083333);
Node30: Box("vi",0.750000,0.597222) at (10.097222,9.083333);
Node31: Box("RCS",0.805556,0.597222) at (1.986111,7.986111);
Node32: Box("esh",0.805556,0.597222) at (13.708333,7.986111);
Node33: Box("vsh",0.805556,0.597222) at (12.152778,7.986111);
Node34: Box("<curses>",2.152778,0.597222) at (10.430556,7.986111);
Node35: Box("etc.",1.069444,0.597222) at (10.430556,6.888889);
Node36: Box("ksh",0.805556,0.597222) at (11.083333,5.791667);
Node37: Box("8th ed. make",3.236111,0.597222) at (4.763889,4.694444);
Node38: Box("nmake",1.347222,0.597222) at (10.583333,3.597222);
Node39: Box("Versioned files",4.041667,0.597222) at (4.166667,3.597222);
Node40: Box("ncpp",1.069444,0.597222) at (15.013889,2.500000);
Node41: Box("nmake+viewpaths",4.041667,0.597222) at (9.388889,2.500000);
Node42: Box("mk",0.750000,0.597222) at (6.555556,2.500000);
Node43: Box("ksh-i",1.347222,0.597222) at (12.333333,2.500000);
Node44: Box("Ansi cpp",2.152778,0.597222) at (15.013889,1.402778);
Node45: Box("n2make",1.611111,0.597222) at (12.388889,1.402778);
Node46: Box("extended directories",5.388889,0.597222) at (8.638889,1.402778);
Node47: Box("fdelta",1.611111,0.597222) at (2.388889,1.402778);
Node48: Box("Advanced Software Development Environment",11.069444,0.597222) at (10.513889,0.305556);
.ps
.ps 14
spline ->  from (10.333333,18.666667) to (10.513889,18.083333) to (10.597222,17.819444) to (10.666667,17.569444) to (11.638889,13.763889);
spline ->  from (10.111111,18.666667) to (9.055556,13.763889);
spline ->  from (15.666667,17.569444) to (15.666667,11.569444);
spline ->  from (3.694444,15.486111) to (5.472222,14.597222) to (5.513889,14.430556) to (5.541667,14.277778) to (5.541667,12.666667) to (5.583333,12.500000) to (5.611111,12.347222) to (7.500000,11.319444) to (7.569444,11.152778) to (7.625000,10.986111) to (7.680556,10.472222) to (7.708333,10.180556) to (7.722222,9.888889) to (7.736111,9.375000) to (7.750000,9.083333) to (7.750000,8.791667) to (7.750000,6.083333) to (7.791667,5.791667) to (7.819444,5.500000) to (7.902778,4.986111) to (7.958333,4.694444) to (8.013889,4.402778) to (9.916667,3.805556);
spline ->  from (3.500000,15.375000) to (4.000000,14.666667) to (4.041667,14.472222) to (4.069444,14.277778) to (4.069444,13.763889) to (4.111111,13.555556) to (4.138889,13.361111) to (5.083333,11.569444);
spline ->  from (3.138889,15.375000) to (2.875000,11.569444) to (2.861111,11.277778) to (2.847222,10.986111) to (2.777778,8.277778) to (2.777778,7.986111) to (2.777778,7.694444) to (2.777778,4.986111) to (2.819444,4.819444) to (2.847222,4.652778) to (3.805556,3.888889);
spline ->  from (2.791667,15.375000) to (2.194444,14.652778) to (2.166667,14.458333) to (2.125000,14.277778) to (1.986111,8.277778);
spline ->  from (6.694444,13.180556) to (6.513889,12.597222) to (6.444444,12.458333) to (6.375000,12.333333) to (5.555556,11.569444);
spline ->  from (7.083333,13.180556) to (7.458333,12.500000) to (7.527778,12.388889) to (7.597222,12.291667) to (8.083333,11.402778) to (8.138889,11.194444) to (8.194444,10.986111) to (8.236111,10.472222) to (8.250000,10.180556) to (8.250000,9.888889) to (8.250000,6.083333) to (8.291667,5.861111) to (8.319444,5.652778) to (8.569444,4.902778) to (8.638889,4.777778) to (8.708333,4.666667) to (10.069444,3.888889);
spline ->  from (11.833333,13.180556) to (12.277778,11.569444);
spline ->  from (12.513889,13.180556) to (13.875000,12.416667) to (13.916667,12.250000) to (13.944444,12.083333) to (13.944444,11.569444) to (13.972222,11.277778) to (13.986111,10.986111) to (14.208333,9.375000) to (14.194444,9.180556) to (14.180556,8.986111) to (13.847222,8.277778);
spline ->  from (11.583333,13.180556) to (11.125000,11.569444) to (11.083333,11.277778) to (11.041667,10.986111) to (11.041667,9.375000) to (11.083333,9.194444) to (11.111111,9.027778) to (11.861111,8.277778);
spline ->  from (8.986111,13.180556) to (8.986111,11.569444);
spline ->  from (15.638889,10.986111) to (15.041667,2.791667);
spline ->  from (5.750000,10.986111) to (6.333333,10.472222);
spline ->  from (5.208333,10.986111) to (4.777778,4.986111);
spline ->  from (12.319444,10.986111) to (12.194444,9.375000) to (12.222222,9.194444) to (12.236111,9.013889) to (12.861111,8.083333) to (12.861111,7.944444) to (12.861111,7.819444) to (12.680556,7.180556) to (12.611111,7.013889) to (12.527778,6.847222) to (11.486111,6.083333);
spline ->  from (8.986111,10.986111) to (8.986111,8.277778) to (9.013889,7.986111) to (9.041667,7.694444) to (9.097222,7.180556) to (9.152778,6.930556) to (9.208333,6.694444) to (9.902778,6.472222) to (10.680556,6.027778);
spline ->  from (6.708333,9.888889) to (7.222222,4.986111) to (7.277778,4.805556) to (7.319444,4.638889) to (9.111111,2.791667);
spline ->  from (13.388889,8.791667) to (13.597222,8.277778);
spline ->  from (10.472222,8.861111) to (11.750000,8.194444);
spline ->  from (10.194444,8.791667) to (10.347222,8.277778);
spline ->  from (2.097222,7.694444) to (2.208333,7.180556) to (2.250000,6.888889) to (2.277778,6.597222) to (2.277778,4.986111) to (2.305556,4.694444) to (2.333333,4.402778) to (3.500000,3.888889);
spline ->  from (1.902778,7.694444) to (1.819444,7.180556) to (1.791667,6.888889) to (1.763889,6.597222) to (1.763889,3.888889) to (1.805556,3.638889) to (1.833333,3.388889) to (2.305556,1.694444);
spline ->  from (13.652778,7.694444) to (13.597222,7.180556) to (13.555556,6.944444) to (13.500000,6.708333) to (11.486111,5.944444);
spline ->  from (11.958333,7.694444) to (11.222222,6.083333);
spline ->  from (10.430556,7.694444) to (10.430556,7.180556);
spline ->  from (10.986111,5.500000) to (10.986111,4.986111) to (10.916667,4.694444) to (10.833333,4.402778) to (10.638889,3.888889);
spline ->  from (10.680556,5.722222) to (6.902778,4.402778) to (6.722222,4.347222) to (6.527778,4.305556) to (5.138889,3.888889);
spline ->  from (11.486111,5.666667) to (13.500000,4.722222) to (13.541667,4.555556) to (13.569444,4.402778) to (13.569444,1.694444) to (13.458333,1.347222) to (13.347222,1.013889) to (13.194444,0.986111) to (11.666667,0.597222);
spline ->  from (10.680556,5.555556) to (9.805556,4.763889) to (9.736111,4.583333) to (9.666667,4.402778) to (9.583333,3.888889) to (9.680556,3.541667) to (9.763889,3.208333) to (9.916667,3.180556) to (11.666667,2.694444);
spline ->  from (5.416667,4.402778) to (6.486111,3.652778) to (6.527778,3.472222) to (6.555556,3.305556) to (6.555556,2.791667);
spline ->  from (10.638889,3.888889) to (10.638889,4.402778) to (10.722222,4.694444) to (10.791667,4.986111) to (10.986111,5.500000);
spline ->  from (11.250000,3.472222) to (14.486111,2.638889);
spline ->  from (10.152778,3.305556) to (9.666667,2.791667);
spline ->  from (4.291667,3.305556) to (4.805556,1.347222) to (5.125000,1.180556) to (5.444444,1.027778) to (8.472222,0.597222);
spline ->  from (15.013889,2.208333) to (15.013889,1.694444);
spline ->  from (10.472222,2.208333) to (11.694444,1.694444);
spline ->  from (9.111111,2.208333) to (8.805556,1.694444);
spline ->  from (14.555556,1.111111) to (11.972222,0.597222);
spline ->  from (11.708333,1.111111) to (10.944444,0.597222);
spline ->  from (9.319444,1.111111) to (10.083333,0.597222);
spline ->  from (3.069444,1.111111) to (7.819444,0.597222);
.ps
.PE
.ce
\fBFigure 8.\fR
.SP
.KE
.PP
Forcing a group of nodes to be on the same rank can cause \fIflat\fP-edges, i.e.,
edges that point sideways instead of upward or downward.
If possible, \*(DG draws flat-edges from left to right.
(This means that, as a side-effect, left-to-right order in a rank can
be controlled by creating invisible flat-edges.)
Figure 9 shows a graph description
in which the relative placements of nodes are completely specified.
The edge crossing is unavoidable.
.KF
.sp .3i
.P1 10n
\&.GD
same rank 1 2;
same rank 3 4;
1 4;
2 3;
1 2 invis;
3 4 invis;
\&.GE
.P2
.sp .6i
.ce
\fBFigure 9.\fR
.sp -2i
.PS 6 2
.ps 10
box invis at (-2,0);
arrowwid = 0.050000;
arrowht  = 0.150000;
define Ellipse	% [ellipse wid $2 ht $3 $1] %
Box0: Ellipse("1",0.750000,0.500000) at (0.375000,1.250000);
Box1: Ellipse("2",0.750000,0.500000) at (1.375000,1.250000);
Box2: Ellipse("3",0.750000,0.500000) at (0.375000,0.250000);
Box3: Ellipse("4",0.750000,0.500000) at (1.375000,0.250000);
spline -> solid from (0.583333,1.041667) to (1.166667,0.458333);
spline -> solid from (1.166667,1.041667) to (0.583333,0.458333);
.ps
.PE
.KE
.PP
If edges are \f5ordered\fP, \*(DG automatically places head nodes on the
same rank and creates invisible flat-edges.  Figure 10
is a small part of a parse tree.
Ordered edges are needed to correctly represent the grammar.
.KF
.P1
\&.GD
ordered edge from "for-stmt" to "for" "var" "=" "expr1" "to" "expr2";
ordered edge from "expr1" to "left_op" "*" "right_op";
\&.GE
.P2
.PS 4.000000 1.702128
arrowht = 0.111111;
arrowwid  = 0.055556;
define Box	% [box wid $2 ht $3 $1] %
.ps 9
Node0: Box("for-stmt",1.000000,0.500000) at (3.000000,2.250000);
Node1: Box("for",0.750000,0.500000) at (0.500000,1.250000);
Node2: Box("var",0.750000,0.500000) at (1.500000,1.250000);
Node3: Box("=",0.750000,0.500000) at (2.500000,1.250000);
Node4: Box("expr1",0.750000,0.500000) at (3.500000,1.250000);
Node5: Box("to",0.750000,0.500000) at (4.500000,1.250000);
Node6: Box("expr2",0.750000,0.500000) at (5.500000,1.250000);
Node7: Box("left_op",0.875000,0.500000) at (2.444444,0.250000);
Node8: Box("*",0.750000,0.500000) at (3.500000,0.250000);
Node9: Box("right_op",1.000000,0.500000) at (4.625000,0.250000);
.ps
.ps 14
spline ->  from (2.500000,2.097222) to (0.763889,1.500000);
spline ->  from (2.500000,2.000000) to (1.833333,1.500000);
spline ->  from (2.833333,2.000000) to (2.611111,1.500000);
spline ->  from (3.166667,2.000000) to (3.388889,1.500000);
spline ->  from (3.500000,2.000000) to (4.166667,1.500000);
spline ->  from (3.500000,2.097222) to (5.236111,1.500000);
spline ->  from (3.152778,1.000000) to (2.680556,0.500000);
spline ->  from (3.500000,1.000000) to (3.500000,0.500000);
spline ->  from (3.875000,1.000000) to (4.375000,0.500000);
.ps
.PE
.ce
\fBFigure 10.\fR
.KE
.PP
There are many other properties of a graph drawing that one
might want to control,
but it is difficult to build a good tool that achieves
many, possibly conflicting, aesthetic goals.
Users can exercise some control over node placement by setting edge
weights and creating invisible edges.
For finer control over drawings we suggest
changing the drawing with a graphical editor or by modifying
the generated code.
.PP
There are no absolute restrictions on the graphs that \*(DG can draw.
Self-edges and multi-edges are allowed.
For picture clarity, it's best to avoid nodes with many incident edges.
For instance, in procedure call graphs, it is a good idea to eliminate common
library calls (for example, \fIstdio\fP) before making the drawing.
Removing these less interesting nodes both yields a more informative drawing and
reduces run time.
.NH 1
Drawing Algorithms
.PP
.I Dag
has three main components: the parser, \fIdraw_dag\fP,
and the code generator.  The parser is written in
\s-2YACC\s+2|reference(latest yacc).
It constructs a graph of attributed nodes and edges in memory.
The graph is passed to the drawing procedure \fIdraw_dag\fP,
which sets $(x,y)$ coordinates of nodes
and spline control points of edges.
The code generator traverses the attributed graph to emit target code
in an obvious fashion.  \fIDraw_dag\fP is thus the heart of \*(DG.
.PP
To design algorithms for drawing graphs,
we need to define what makes a good drawing.
We have chosen three desirable properties for drawings of directed graphs:
.PP
.B P1.
The drawing should reveal the partial order implied by edges of the graph.
Nodes are placed in ranks so that edges points downward.
If the graph is not acyclic, cycles are broken by reversing edges.
.PP
.B P2.
The drawing should not be cluttered with irrelevance.
For example, edge crossings are undesirable because they suggest
a connection that is not really present in the underlying abstract graph.
Edges also should not intersect nodes that are not their endpoints.
.PP
.B P3.
The drawing should reveal the relationships implied by edges by placing
adjacent nodes in the graph close together.
.PP
Unfortunately, these goals conflict.
For instance,
the placement of nodes in ranks may make it impossible to avoid edge crossings.
Also, the minimization of edge-crossings in a layout
is computationally intractable.
So we are satisfied to find heuristics that run quickly
and make good layouts in common cases.
We sketch these techniques briefly as an aid to understanding the
behavior of \*(DG.
.PP
.I Draw_dag
has four passes.  The first pass finds an optimal rank assignment for the
nodes, satisfying P1 and P3.  The optimal rank assignment problem
is to assign integer ranks to nodes such that the sum of the weighted edge
lengths is minimized.  The length of an edge here is taken as 
the difference in the ranks of its head and tail nodes.
The optimal rank assignment problem can be formulated as an integer program.
Because of its special characteristics the simplex method
for linear programming can be used to solve these integer programs.
However we have developed a
combinatorial algorithm that finds the solution 
in a factor of $O(|E|)$ less time than the standard simplex algorithm
and uses only $O(|E|)$ space.
Following rank assignment, \fIdraw_dag\fP
creates dummy nodes where long edges cross ranks.  The placement of dummy
nodes determines edge crossings and is also 
an aid to drawing splines for long edges.  Consequently,
optimal rank assignment is important not only for drawing quality, 
but also to minimize the number of dummy nodes.
This reduces edge crossing and improves the running time
of subsequent passes that have a factor of
$O(|V|)$ in their running time.
.PP
The second pass orders nodes from left to right within ranks.
This pass tries to reduce edge crossings (P2) subject to the
constraints on node order implied by flat edges (P1).
It is based on an iterative technique like that of
|reference(sugiyama)|reference(rowe browser).
The key improvements are a new weight function and local optimizations.
We call the weight function the ``generalized median'';
it produces less crossings by reducing the effects of widely spread nodes,
and is inexpensive to compute.
Informally, the generalized median 
of a node with respect to an adjacent rank is defined as follows.
When the number of neighbors of a node in the concerned adjacent rank
is odd, the generalized median is simply the median position of its
neighbors. When the number of neighbors is even, the generalized median
is in between the left and the right median positions.
The exact value is tilted toward the side where neighbor nodes are
more tightly packed.
.PP
The generalized median heuristic (and other similar heuristics)
tends to work better if the initial order of nodes is good.
To enhance this condition,
we apply transpositions of
adjacent nodes to precondition the inputs to the generalized median method and to
further optimize its solutions.
The transposition heuristic
can be viewed as a descent method to find local optima.
.PP
The third pass assigns absolute coordinates to nodes,
respecting the ordering determined by the previous pass (P3).
The optimal assignment can be formulated as a set of linear
constraints and an objective function involving absolute values.
This can be transformed into a linear program by a standard technique
that involves introducing extra variables.  \f5dag -O\fP works this way.
Since this pass is the bottleneck in \fIdraw_dag\fP,
we have developed a fast heuristic 
based on computation of the generalized median positions of adjacent nodes
with additional local optimizations.
.PP
The final pass finds the spline control points for edges.
There are several cases to consider, including self-edges, edges between
nodes on the same rank, adjacent ranks, or nonadjacent ranks, possibly
in the presence of multi-edges.
Short edges are easy to draw; we use a single B-spline 
|reference(mortenson) that is
shaped according to the number of parallel multi-edges being drawn.
Long edges, or edges that have endpoints on nonadjacent ranks,
are more difficult since they may
pass near other nodes and change direction.
The B-spline control points are chosen for
smoothness,
avoiding node intersections, and
avoiding edge crossings.
.PP
The dummy nodes are created with a
height a little larger than the highest non-dummy node in the same rank,
and width usually twice the default node separation.  This is big enough
to make a spline inside the boundaries of the dummy node, if necessary.
On the other hand, if there is space next to the dummy node, it is
often preferable to encroach on it to draw a smoother spline.
.KF top
.PS 6.5
scale=250
box ht 100 wid 75 at (0,0);
line dashed from last box.c + 200,120 to last box.c to last box.c + -200,-190;
line from last box.c + 200,120 to last box.c + -200,-190;
"(a)" at last box.c +0,-250;
box ht 100 wid 75 at (500,0);
line from last box.c + 200,156.7 to last box.c  to last box.c + -33.3,-200;
"\(bu" at last box.c + 37.5,27;
"\(bu" at last box.c+ -8.3333333,-51;
"\(bu" at last box.c + 0,-2;
"(b)" at last box.c +0,-250;
box ht 100 wid 75 at (1000,0);
line from last box.c + 200,60 to last box.c + -37.5,40;
line dashed from last box.c + -200,-40 to last box.c+ 37.5,7.5;
line from last box.c + -200,-40 to last box.c + 0,42.22222;
"\(bu" at last box.c + 0,40;
"\(bu" at last box.c + -37.5,26;
"\(bu" at last box.c + 37.5,42;
"(c)" at last box.c +0,-250;
.PE
.PS 6.5
scale=250
box ht 100 wid 75 at 0,0;
line from last box.c + 200,40 to last box.c + -37.5,-6.66666;
line from last box.c + -200,-40 to last box.c + 37.5,-20;
"\(bu" at last box.c + 0,-2;
"\(bu" at last box.c + 37.5,7;
"\(bu" at last box.c + -37.5,-30;
"(d)" at last box +0,-250;
box ht 100 wid 75 at 500,0;
line from last box.c + 116.7,-200 to last box.c + 37.5,-40;
line from last box.c + 200,-83.3 to last box.c + 0,-50;
"(e)" at last box +0,-250;
box ht 100 wid 75 at 1000,0;
line from last box.c + 116,-200 to last box.c + 0,-50;
line from last box.c + 200,-83.3 to last box.c + 0,-50;
"(f)" at last box +0,-250;
.PE
.ce
\fBFigure 11.\fR
.SP
.KE
.PP
The edge drawing procedure in \fIdraw_dag\fP visits each dummy node to
choose its spline control points.  If a straight line between the neighbors
of the dummy node does not intersect any other node, nor change the order of
edge crossings, then the dummy node is deleted (Figure 11a).
This removes small bumps in the edges.
Next, \fIdraw_dag\fP aims the incident edges as close to the center of the dummy node
box as possible, without intersecting adjacent nodes.
.I Draw_dag
uses the points where the edges intersect the dummy node
as spline control points.  It also finds a third point to determine
how the spline bends as it passes through the dummy node.
If the intersection of the edges lies inside the dummy node (Figure 11b),
\fIdraw_dag\fP chooses it as the third control point.
Otherwise, \fIdraw_dag\fP tries to move the edges toward each other
so they do intersect (Figure 11c) and
again takes the intersection as the third control point.
If the edges can't be made to intersect, but the angle between them is acute,
\fIdraw_dag\fP can still make a smooth spline by choosing the midpoint of the opposite side.
When the angle is obtuse, that is, the edges are close to being parallel,
then it chooses the midpoint of the dummy node box.
In the latter case the spline has two points of inflection
as it passes through the dummy node, which is not as smooth as the
other cases which have only one,
but the extra turn is necessary when an edge
makes a jog as it passes through a rank near other nodes (Figure 11d).
.PP
Moving an edge may create an edge intersection
near an incident real (non-dummy) node (Figure 11e).
Such intersections are eliminated by sorting the incident edges according
to the
.I x -coordinates
of the other endpoints, and moving their nearby
endpoints slightly (Figure 11f).
.PP
Tables 1-3 show the performance of \fIdraw_dag\fP using
various node placement methods.
The tables list the time (in seconds) spent in the three node placement
passes: time spent on level (or rank) assignment ($t sub level$),
time spent ordering nodes within ranks to reduce edge crossings ($t sub cross$),
and time spent finding final coordinates ($t sub  coord$).
The total time $t sub total$ is the sum of these plus time spent in initialization
and creating splines.
The times were measured on a VAX-8650 under the 4.3BSD UNIX System and
averaged over 5 runs to reduce statistical fluctuation.
Times were measured on three graphs,
the \fIWorld Dynamics\fP graph from the abstract of this paper and
two procedure call graphs from C programs.
\&\fIWorld Dynamics \fP has 48 nodes and 69 edges.
The initial placement has 126 crossings.
\&\fICallGraph-1\fP in Figure 12
has 38 nodes, 58 edges, and the initial placement has 128 crossings.
\&\fI CallGraph-2\fP in Figure 13 is \*(DG's call graph of
170 nodes and 258 edges.  Its initial placement has 1170 crossings.
.PP
We tried three algorithms for ordering of nodes within ranks:
(1) \&\fIgmedian\fP is \*(DG's generalized median algorithm,
(2) \&\fIbcenter\fP is the barycenter method described in |reference(sugiyama),
and (3) \fIrmedian\fP is the ``right median''
|reference(eades), which orders nodes using
the position of the median neighbor when the number of neighbors is odd,
and the right median neighbor otherwise.
We also tried all three methods in combination with transposition of
adjacent nodes.
To simplify the comparison of relative performance,
the iterative loop for minimizing edge crossing is set to terminate
after exactly 20 iterations.
In practice, the loop termination is determined by an adaptive parameter
that depends on the convergent rate of a solution.
Thus, for example, the total time for the \fIWorld Dynamics\fP
graph using \fIgmedian+\fP
is slightly higher than it would be in an actual run.
The experiments show that generalized median behaves well in a
variety of test cases, and in about the same run-time as the other methods.
All three methods improved by adding transposition, which seems well worth
the extra computational expense.   In Table 1, transposition
reduced edge crossing by 30%.
.KS
.SP 1
.TS
center, allbox;
c s c s s s
c c c c c c
l n n n n n.
World Dynamics	seconds
Method	$n sub  cross$	$t sub level$	$t sub crossing$	$t sub coord$	$t sub total$
gmedian	59	.06	.47	.56	1.18
bcenter	59	.06	.44	.47	1.08
rmedian	62	.05	.45	.51	1.11
gmedian+	41	.06	1.30	.49	1.96
bcenter+	43	.06	1.23	.50	1.88
rmedian+	47	.05	1.25	.49	1.88
.TE
.ce
Table 1
.KE
.KS
.PS 6.000000
arrowht = 0.111111;
arrowwid  = 0.055556;
define Box	% [box wid $2 ht $3 $1] %
define Square	% [box wid $2 ht $2 $1] %
define Circle	% [circle rad $2/2; $1 at last circle.c] %
define Ellipse	% [ellipse wid $2 ht $3 $1] %
define Diamond	% [move down $3/2; line up $3/2 right $2/2 then up $3/2 left $2/2 then down $3/2 left $2/2 then down $3/2 right $2/2; move up $3/2; $1;] %
define Doublecircle % [circle rad $2/2; circle rad .9*$2/2 at last circle.c; $1 at last circle.c] %
define Plaintext % [box invis wid $2 ht $3 $1] %
.ps 4
Node0: Ellipse("progen",0.750000,0.500000) at (12.527778,10.250000);
Node1: Ellipse("xmalloc",0.875000,0.500000) at (13.152778,2.250000);
Node2: Ellipse("setbuf",0.750000,0.500000) at (11.902778,9.250000);
Node3: Ellipse("initsymtbl",1.250000,0.500000) at (13.152778,9.250000);
Node4: Ellipse("scanner",0.875000,0.500000) at (6.319444,9.250000);
Node5: Ellipse("put_one",0.875000,0.500000) at (5.013889,8.250000);
Node6: Ellipse("setblkval",1.125000,0.500000) at (9.930556,5.250000);
Node7: Ellipse("Mapblk",0.750000,0.500000) at (11.222222,3.250000);
Node8: Ellipse("bhash",0.750000,0.500000) at (12.097222,2.250000);
Node9: Ellipse("token_fetch",1.375000,0.500000) at (8.875000,4.250000);
Node10: Ellipse("unget_one",1.125000,0.500000) at (3.944444,1.250000);
Node11: Ellipse("ungetc",0.750000,0.500000) at (3.944444,0.250000);
Node12: Ellipse("look_ahead",1.250000,0.500000) at (3.208333,8.250000);
Node13: Ellipse("comment_scan",1.500000,0.500000) at (3.208333,7.250000);
Node14: Ellipse("tree_walker",1.375000,0.500000) at (6.930556,4.250000);
Node15: Ellipse("dnfeval",0.875000,0.500000) at (7.416667,3.250000);
Node16: Ellipse("tilde_token",1.375000,0.500000) at (5.388889,2.250000);
Node17: Ellipse("q_token",0.875000,0.500000) at (9.680556,2.250000);
Node18: Ellipse("get_ch",0.750000,0.500000) at (11.416667,1.250000);
Node19: Ellipse("unget_ch",1.000000,0.500000) at (10.291667,1.250000);
Node20: Ellipse("atol",0.750000,0.500000) at (7.013889,2.250000);
Node21: Ellipse("next_or",0.875000,0.500000) at (8.069444,2.250000);
Node22: Ellipse("ret_token",1.125000,0.500000) at (10.916667,2.250000);
Node23: Ellipse("range_walker",1.500000,0.500000) at (8.444444,8.250000);
Node24: Ellipse("expreval",1.000000,0.500000) at (8.625000,7.250000);
Node25: Ellipse("next_val",1.000000,0.500000) at (10.083333,6.250000);
Node26: Ellipse("mcprse",0.750000,0.500000) at (11.388889,5.250000);
Node27: Ellipse("refblkval",1.125000,0.500000) at (11.041667,4.250000);
Node28: Ellipse("setsymval",1.125000,0.500000) at (15.013889,4.250000);
Node29: Ellipse("Mapsym",0.750000,0.500000) at (16.375000,3.250000);
Node30: Ellipse("hash",0.750000,0.500000) at (16.375000,2.250000);
Node31: Ellipse("randstr",0.875000,0.500000) at (17.847222,2.250000);
Node32: Ellipse("incrsymval",1.250000,0.500000) at (17.805556,4.250000);
Node33: Ellipse("refsymval",1.125000,0.500000) at (16.375000,4.250000);
Node34: Ellipse("echo_of",0.875000,0.500000) at (12.777778,4.250000);
Node35: Ellipse("mclex",0.750000,0.500000) at (13.833333,4.250000);
Node36: Ellipse("next_op",0.875000,0.500000) at (8.625000,6.250000);
Node37: Ellipse("quote_token",1.375000,0.500000) at (0.694444,7.250000);
.ps
.ps 14
spline ->  from (12.152778,10.208333) to (6.736111,9.319444);
spline ->  from (12.722222,10.027778) to (13.013889,9.500000);
spline ->  from (12.333333,10.027778) to (12.027778,9.486111);
spline ->  from (12.902778,10.208333) to (17.263889,9.500000) to (18.041667,9.250000) to (18.805556,9.000000) to (18.805556,3.500000) to (18.777778,3.291667) to (18.736111,3.083333) to (16.750000,2.875000) to (13.569444,2.319444);
spline ->  from (5.888889,9.194444) to (0.777778,8.500000) to (0.750000,8.250000) to (0.708333,8.000000) to (0.694444,7.500000);
spline ->  from (6.736111,9.180556) to (10.958333,8.277778) to (11.055556,8.138889) to (11.138889,8.000000) to (11.222222,6.500000) to (11.236111,6.250000) to (11.250000,6.000000) to (11.375000,5.500000);
spline ->  from (5.944444,9.125000) to (4.236111,8.500000) to (4.111111,8.416667) to (4.277778,8.166667) to (4.763889,7.361111) to (4.819444,7.180556) to (4.861111,7.000000) to (5.361111,2.500000);
spline ->  from (5.902778,9.166667) to (2.444444,8.597222) to (2.000000,8.388889) to (2.291667,8.055556) to (2.944444,7.486111);
spline ->  from (6.694444,9.125000) to (7.250000,8.791667) to (8.027778,8.458333);
spline ->  from (6.319444,9.000000) to (6.416667,6.500000) to (6.472222,6.250000) to (6.513889,6.000000) to (6.652778,5.500000) to (6.736111,5.250000) to (6.819444,5.000000) to (6.861111,4.500000);
spline ->  from (5.916667,9.166667) to (3.611111,8.444444);
spline ->  from (5.902778,9.180556) to (2.222222,8.625000) to (2.013889,8.597222) to (1.875000,8.291667) to (1.722222,8.000000) to (1.750000,6.500000) to (1.750000,6.250000) to (1.750000,6.000000) to (1.750000,2.500000) to (1.791667,2.361111) to (1.819444,2.222222) to (3.555556,1.430556);
spline ->  from (6.472222,9.013889) to (6.750000,8.402778) to (6.791667,8.194444) to (6.819444,8.000000) to (6.819444,6.500000) to (6.861111,6.347222) to (6.888889,6.194444) to (8.638889,4.486111);
spline ->  from (6.597222,9.055556) to (7.152778,8.430556) to (7.263889,8.208333) to (7.375000,8.000000) to (7.694444,6.500000) to (7.861111,6.208333) to (8.013889,5.916667) to (8.194444,5.875000) to (9.486111,5.402778);
spline ->  from (5.986111,9.083333) to (5.250000,8.458333);
spline ->  from (9.958333,5.000000) to (10.000000,4.500000) to (10.041667,4.291667) to (10.083333,4.083333) to (10.486111,3.875000) to (10.986111,3.444444);
spline ->  from (11.555556,3.125000) to (13.152778,2.500000);
spline ->  from (11.472222,3.055556) to (11.930556,2.472222);
spline ->  from (8.875000,4.000000) to (8.875000,2.500000) to (8.763889,2.194444) to (8.652778,1.888889) to (8.500000,1.875000) to (4.486111,1.319444);
spline ->  from (3.944444,1.000000) to (3.944444,0.500000);
spline ->  from (3.541667,8.041667) to (4.263889,7.319444) to (4.305556,7.152778) to (4.333333,7.000000) to (4.333333,3.500000) to (4.305556,3.250000) to (4.277778,3.000000) to (4.208333,2.500000) to (4.152778,2.250000) to (4.097222,2.000000) to (4.000000,1.500000);
spline ->  from (3.208333,8.000000) to (3.208333,7.500000);
spline ->  from (3.236111,7.000000) to (3.513889,2.500000) to (3.555556,2.305556) to (3.597222,2.125000) to (3.847222,1.500000);
spline ->  from (6.861111,4.500000) to (6.625000,5.000000) to (6.541667,5.250000) to (6.458333,5.500000) to (6.319444,6.000000) to (6.277778,6.250000) to (6.222222,6.500000) to (6.319444,9.000000);
spline ->  from (7.097222,4.013889) to (7.305556,3.486111);
spline ->  from (7.847222,3.305556) to (7.972222,3.361111) to (8.097222,3.250000) to (7.972222,3.138889) to (7.847222,3.194444);
spline ->  from (7.777778,3.111111) to (10.763889,2.486111);
spline ->  from (7.097222,3.430556) to (6.250000,3.875000) to (5.861111,4.000000) to (5.847222,4.250000) to (5.819444,4.500000) to (5.819444,8.000000) to (5.861111,8.194444) to (5.888889,8.402778) to (6.166667,9.013889);
spline ->  from (7.569444,3.013889) to (7.916667,2.486111);
spline ->  from (7.277778,3.013889) to (7.097222,2.500000);
spline ->  from (7.763889,3.097222) to (9.333333,2.402778);
spline ->  from (7.041667,3.125000) to (5.777778,2.458333);
spline ->  from (7.805556,3.125000) to (12.597222,2.611111) to (13.152778,2.500000);
spline ->  from (4.972222,2.055556) to (4.222222,1.472222);
spline ->  from (9.805556,2.013889) to (10.138889,1.486111);
spline ->  from (9.930556,2.041667) to (11.180556,1.444444);
spline ->  from (10.722222,2.013889) to (10.430556,1.486111);
spline ->  from (11.083333,2.013889) to (11.333333,1.500000);
spline ->  from (8.027778,8.458333) to (7.444444,8.791667) to (6.694444,9.125000);
spline ->  from (8.500000,8.000000) to (8.583333,7.500000);
spline ->  from (8.625000,7.000000) to (8.625000,6.500000);
spline ->  from (9.000000,7.083333) to (9.819444,6.458333);
spline ->  from (10.430556,6.069444) to (11.152778,5.444444);
spline ->  from (11.680556,5.097222) to (13.680556,4.472222);
spline ->  from (11.375000,5.500000) to (11.444444,6.000000) to (11.430556,6.250000) to (11.416667,6.500000) to (11.333333,8.000000) to (11.250000,8.138889) to (11.152778,8.277778) to (6.736111,9.180556);
spline ->  from (11.638889,5.069444) to (12.500000,4.444444);
spline ->  from (11.708333,5.125000) to (15.694444,4.611111) to (16.375000,4.500000);
spline ->  from (11.722222,5.125000) to (17.055556,4.611111) to (17.805556,4.500000);
spline ->  from (11.694444,5.111111) to (14.888889,4.500000);
spline ->  from (11.388889,5.000000) to (10.750000,4.902778) to (7.027778,4.500000);
spline ->  from (11.388889,5.000000) to (9.402778,4.416667);
spline ->  from (11.388889,5.000000) to (11.152778,4.500000);
spline ->  from (11.750000,5.305556) to (11.888889,5.361111) to (12.013889,5.250000) to (11.888889,5.138889) to (11.750000,5.194444);
spline ->  from (11.527778,5.013889) to (13.152778,2.500000);
spline ->  from (11.097222,4.000000) to (11.180556,3.500000);
spline ->  from (15.388889,4.069444) to (16.138889,3.444444);
spline ->  from (16.680556,3.111111) to (17.569444,2.444444);
spline ->  from (16.013889,3.166667) to (13.527778,2.375000);
spline ->  from (16.375000,3.000000) to (16.375000,2.500000);
spline ->  from (18.222222,4.069444) to (19.138889,3.375000) to (19.402778,3.138889) to (19.125000,3.000000) to (18.152778,2.430556);
spline ->  from (17.402778,4.055556) to (16.625000,3.444444);
spline ->  from (16.375000,4.000000) to (16.375000,3.500000);
spline ->  from (0.694444,7.000000) to (0.694444,2.500000) to (0.736111,2.263889) to (0.763889,2.041667) to (3.458333,1.375000);
.ps
.PE
.sp -2
.ce
\fBFigure 12.\fR  CallGraph-1.
.SP 2
.KE
.KS
.TS
center, allbox;
c s c s s s
c c c c c c
l n n n n n.
CallGraph-1	seconds
Method	$ n sub cross$	$t sub level$	$t sub crossing$	$t sub coord$	$t sub total$
gmedian	12	.04	.38	.57	1.08
bcenter	10	.04	.44	.90	1.48
rmedian	14	.04	.43	.70	1.26
gmedian+	11	.03	.73	.85	1.73
bcenter+	11	.04	.77	.64	1.56
rmedian+	11	.05	.89	.64	1.67
.TE
.ce
Table 2
.KE
.KS
.TS
center, allbox;
c s c s s s
c c c c c c
l n n n n n.
CallGraph-2	seconds
Method	$ n sub cross$	$t sub level$	$t sub crossing$	$t sub coord$	$t sub total$
gmedian	193	.50	2.87	2.91	6.52
bcenter	249	.53	2.73	2.82	6.36
rmedian	227	.53	2.96	3.07	6.84
gmedian+	154	.51	8.11	3.32	12.23
bcenter+	145	.52	8.21	3.05	12.04
rmedian+	200	.50	8.35	3.75	12.86
.TE
.ce
Table 3
.KE
.KS
.PS 6.000000 3.000000
arrowht = 0.111111;
arrowwid  = 0.055556;
define Box	% [box wid $2 ht $3] %
Node0: Box("start",0.750000,0.500000) at (16.500000,43.208333);
Node1: Box("main",0.750000,0.500000) at (16.500000,39.305556);
Node2: Box("yyparse",0.875000,0.500000) at (41.500000,35.402778);
Node3: Box("nextfile",1.000000,0.500000) at (32.055556,35.402778);
Node4: Box("fclose",0.750000,0.500000) at (12.611111,35.402778);
Node5: Box("global_init",1.375000,0.500000) at (30.375000,35.402778);
Node6: Box("gets",0.750000,0.500000) at (13.208333,27.597222);
Node7: Box("set_output_type",1.888889,0.500000) at (14.180556,35.402778);
Node8: Box("graph_init",1.250000,0.500000) at (16.000000,35.402778);
Node9: Box("sscanf",0.750000,0.500000) at (2.583333,35.402778);
Node10: Box("strcmp",0.750000,0.500000) at (14.291667,19.777778);
Node11: Box("reclaim_storage",1.888889,0.500000) at (6.347222,35.402778);
Node12: Box("make_drawing",1.500000,0.500000) at (47.916667,31.500000);
Node13: Box("yylex",0.750000,0.500000) at (41.875000,31.500000);
Node14: Box("node_lookup",1.375000,0.500000) at (43.180556,31.500000);
Node15: Box("ct_edge_t",1.125000,0.500000) at (54.416667,27.597222);
Node16: Box("apply_proto_node_t",2.263889,0.500000) at (39.625000,31.500000);
Node17: Box("setpointsize_node_tFi",2.638889,0.500000) at (28.194444,27.597222);
Node18: Box("setshape_node_t",1.888889,0.500000) at (25.180556,27.597222);
Node19: Box("enter_edgelist_edge_t",2.638889,0.500000) at (33.750000,31.500000);
Node20: Box("atof",0.750000,0.500000) at (9.611111,23.694444);
Node21: Box("init_proto",1.250000,0.500000) at (46.236111,31.500000);
Node22: Box("atoi",0.750000,0.500000) at (44.986111,31.500000);
Node23: Box("draw_dag",1.000000,0.500000) at (55.722222,27.597222);
Node24: Box("emit_ps",0.875000,0.500000) at (38.583333,27.597222);
Node25: Box("translate",1.125000,0.500000) at (49.791667,27.597222);
Node26: Box("find_drawing_size",2.138889,0.500000) at (47.916667,27.597222);
Node27: Box("dag_ranks",1.125000,0.500000) at (64.111111,23.694444);
Node28: Box("dag_positions",1.625000,0.500000) at (59.958333,23.694444);
Node29: Box("dag_levels",1.250000,0.500000) at (57.750000,23.694444);
Node30: Box("dag_spline",1.250000,0.500000) at (49.638889,23.694444);
Node31: Box("dag_start",1.125000,0.500000) at (53.597222,23.694444);
Node32: Box("dag_unitedges",1.625000,0.500000) at (45.444444,23.694444);
Node33: Box("dag_delstem",1.375000,0.500000) at (43.694444,23.694444);
Node34: Box("dag_insstem",1.375000,0.500000) at (56.194444,23.694444);
Node35: Box("dag_end",0.875000,0.500000) at (54.833333,23.694444);
Node36: Box("buildrank",1.125000,0.500000) at (82.055556,19.777778);
Node37: Box("nw",0.750000,0.500000) at (51.236111,11.972222);
Node38: Box("decompose",1.125000,0.500000) at (63.180556,19.777778);
Node39: Box("inversion",1.125000,0.500000) at (86.347222,11.972222);
Node40: Box("mincross",1.000000,0.500000) at (83.000000,15.875000);
Node41: Box("transpose",1.125000,0.500000) at (84.986111,11.972222);
Node42: Box("cross_stat",1.250000,0.500000) at (82.305556,11.972222);
Node43: Box("fixstem",0.875000,0.500000) at (80.513889,15.875000);
Node44: Box("initrank",1.000000,0.500000) at (60.972222,15.875000);
Node45: Box("median",0.750000,0.500000) at (81.055556,11.972222);
Node46: Box("copyrank",1.000000,0.500000) at (83.680556,11.972222);
Node47: Box("goodposition",1.500000,0.500000) at (70.902778,19.777778);
Node48: Box("smoothedges",1.375000,0.500000) at (61.694444,19.777778);
Node49: Box("levelposition",1.625000,0.500000) at (59.944444,19.777778);
Node50: Box("minnode",0.875000,0.500000) at (69.791667,15.875000);
Node51: Box("mincut",0.750000,0.500000) at (67.986111,11.972222);
Node52: Box("minpath",0.875000,0.500000) at (76.763889,15.875000);
Node53: Box("medianpos",1.125000,0.500000) at (78.027778,15.875000);
Node54: Box("define_median_t",1.888889,0.500000) at (72.527778,15.875000);
Node55: Box("rlength",0.875000,0.500000) at (70.902778,15.875000);
Node56: Box("minedge",0.875000,0.500000) at (74.152778,15.875000);
Node57: Box("verticalcut",1.375000,0.500000) at (71.041667,11.972222);
Node58: Box("startpos",1.000000,0.500000) at (79.333333,15.875000);
Node59: Box("edgefactor",1.250000,0.500000) at (75.458333,15.875000);
Node60: Box("define_order",1.500000,0.500000) at (69.361111,11.972222);
Node61: Box("mediansort",1.250000,0.500000) at (81.055556,8.069444);
Node62: Box("qsort",0.750000,0.500000) at (73.805556,8.069444);
Node63: Box("rcross",0.750000,0.500000) at (82.305556,8.069444);
Node64: Box("vcross",0.750000,0.500000) at (84.986111,8.069444);
Node65: Box("wmedian",0.875000,0.500000) at (73.763889,11.972222);
Node66: Box("vlength",0.875000,0.500000) at (72.416667,11.972222);
Node67: Box("qst",0.750000,0.500000) at (74.597222,4.166667);
Node68: Box("mediancmp",1.125000,0.500000) at (74.652778,0.250000);
Node69: Box("rightcmp",1.000000,0.500000) at (73.347222,0.250000);
Node70: Box("adjcmp",0.750000,0.500000) at (72.222222,0.250000);
Node71: Box("prioritycmp",1.375000,0.500000) at (78.250000,0.250000);
Node72: Box("leftcmp",0.875000,0.500000) at (76.888889,0.250000);
Node73: Box("intcmp",0.750000,0.500000) at (75.833333,0.250000);
Node74: Box("emit_ps_edge",1.500000,0.500000) at (41.013889,23.694444);
Node75: Box("emit_ps_node",1.500000,0.500000) at (38.250000,23.694444);
Node76: Box("emit_ps_header",1.763889,0.500000) at (33.736111,23.694444);
Node77: Box("emit_ps_edge_header",2.388889,0.500000) at (31.416667,23.694444);
Node78: Box("emit_ps_node_header",2.388889,0.500000) at (28.777778,23.694444);
Node79: Box("emit_ps_trailer",1.888889,0.500000) at (36.305556,23.694444);
Node80: Box("emit_ps_nodeport",2.013889,0.500000) at (33.625000,19.777778);
Node81: Box("printf",0.750000,0.500000) at (29.472222,15.875000);
Node82: Box("find_seg_midpoint",2.138889,0.500000) at (40.694444,19.777778);
Node83: Box("emit_ps_edgelabel_edge_t",3.013889,0.500000) at (37.875000,19.777778);
Node84: Box("doprnt",0.750000,0.500000) at (22.388889,11.972222);
Node85: Box("flsbuf",0.750000,0.500000) at (22.277778,8.069444);
Node86: Box("setlevels",1.125000,0.500000) at (56.833333,19.777778);
Node87: Box("balance",0.875000,0.500000) at (53.680556,15.875000);
Node88: Box("networksimplex",1.763889,0.500000) at (62.597222,15.875000);
Node89: Box("addedge",0.875000,0.500000) at (52.055556,15.875000);
Node90: Box("pathdegree",1.250000,0.500000) at (75.569444,11.972222);
Node91: Box("getminmax",1.125000,0.500000) at (77.000000,11.972222);
Node92: Box("treevalue",1.125000,0.500000) at (65.805556,11.972222);
Node93: Box("treeupdate",1.250000,0.500000) at (63.625000,11.972222);
Node94: Box("cutset",0.750000,0.500000) at (66.152778,8.069444);
Node95: Box("delete_edge",1.375000,0.500000) at (63.569444,8.069444);
Node96: Box("insert_edge",1.375000,0.500000) at (61.819444,8.069444);
Node97: Box("initlevel",1.125000,0.500000) at (60.444444,11.972222);
Node98: Box("spantree",1.000000,0.500000) at (62.250000,11.972222);
Node99: Box("assignpos",1.125000,0.500000) at (78.361111,11.972222);
Node100: Box("widthof",0.875000,0.500000) at (77.000000,8.069444);
Node101: Box("delimitname",1.375000,0.500000) at (33.847222,27.597222);
Node102: Box("newstring",1.125000,0.500000) at (32.333333,27.597222);
Node103: Box("eatwhitespace",1.625000,0.500000) at (26.027778,23.694444);
Node104: Box("escape",0.750000,0.500000) at (31.986111,19.777778);
Node105: Box("downspline",1.250000,0.500000) at (45.888889,19.777778);
Node106: Box("upspline",1.000000,0.500000) at (44.513889,19.777778);
Node107: Box("selfedge_t",1.250000,0.500000) at (50.388889,19.777778);
Node108: Box("setmargins",1.250000,0.500000) at (48.888889,19.777778);
Node109: Box("assignside",1.250000,0.500000) at (47.388889,19.777778);
Node110: Box("setname_node_t",1.763889,0.500000) at (30.638889,27.597222);
Node111: Box("ct_node_t",1.125000,0.500000) at (37.333333,27.597222);
Node112: Box("hash",0.750000,0.500000) at (35.652778,27.597222);
Node113: Box("ct",0.750000,0.500000) at (22.000000,27.597222);
Node114: Box("fopen",0.750000,0.500000) at (13.611111,23.694444);
Node115: Box("strlen",0.750000,0.500000) at (15.291667,19.777778);
Node116: Box("strcpy",0.750000,0.500000) at (24.097222,23.694444);
Node117: Box("setbounds",1.125000,0.500000) at (57.666667,15.875000);
Node118: Box("straight",1.000000,0.500000) at (56.361111,15.875000);
Node119: Box("addpoint",1.000000,0.500000) at (45.361111,15.875000);
Node120: Box("bzero",0.750000,0.500000) at (51.236111,8.069444);
Node121: Box("open",0.750000,0.500000) at (13.291667,19.777778);
Node122: Box("findiop",0.875000,0.500000) at (12.236111,19.777778);
Node123: Box("write",0.750000,0.500000) at (20.819444,4.166667);
Node124: Box("fstat",0.750000,0.500000) at (16.486111,4.166667);
Node125: Box("isatty",0.750000,0.500000) at (22.277778,4.166667);
Node126: Box("lineXbox",1.000000,0.500000) at (44.111111,15.875000);
Node127: Box("defineline",1.250000,0.500000) at (42.736111,15.875000);
Node128: Box("lineXselfedge",1.625000,0.500000) at (41.055556,15.875000);
Node129: Box("flatedge_t",1.250000,0.500000) at (51.888889,19.777778);
Node130: Box("copypoints",1.250000,0.500000) at (46.736111,15.875000);
Node131: Box("dfs",0.750000,0.500000) at (53.638889,19.777778);
Node132: Box("findroot",1.000000,0.500000) at (55.013889,19.777778);
Node133: Box("autosize_node_t",1.888889,0.500000) at (22.527778,23.694444);
Node134: Box("is_fixed_aspect_ratio_t",2.888889,0.500000) at (17.361111,19.777778);
Node135: Box("sethw_node_t",1.500000,0.500000) at (19.805556,19.777778);
Node136: Box("cat_libfile",1.375000,0.500000) at (21.500000,19.777778);
Node137: Box("unsquirrel",1.250000,0.500000) at (28.472222,19.777778);
Node138: Box("noedge_t",1.000000,0.500000) at (48.611111,15.875000);
Node139: Box("multiple",1.000000,0.500000) at (49.861111,15.875000);
Node140: Box("fflush",0.750000,0.500000) at (19.361111,15.875000);
Node141: Box("sprintf",0.875000,0.500000) at (22.027778,15.875000);
Node142: Box("system",0.750000,0.500000) at (20.611111,15.875000);
Node143: Box("close",0.750000,0.500000) at (12.847222,31.500000);
Node144: Box("scross",0.750000,0.500000) at (80.055556,11.972222);
Node145: Box("ct_t",0.750000,0.500000) at (20.958333,23.694444);
Node146: Box("emit_ps_setink",1.763889,0.500000) at (26.222222,19.777778);
Node147: Box("sti",0.750000,0.500000) at (55.166667,31.500000);
Node148: Box("reset_options",1.625000,0.500000) at (30.375000,31.500000);
Node149: Box("fgets",0.750000,0.500000) at (11.333333,23.694444);
Node150: Box("filbuf",0.750000,0.500000) at (10.680556,19.777778);
Node151: Box("append_edge",1.375000,0.500000) at (23.305556,27.597222);
Node152: Box("bfs",0.750000,0.500000) at (60.444444,8.069444);
Node153: Box("read",0.750000,0.500000) at (11.444444,15.875000);
Node154: Box("signal",0.750000,0.500000) at (21.111111,11.972222);
Node155: Box("sigvec",0.750000,0.500000) at (21.111111,8.069444);
Node156: Box("innum",0.750000,0.500000) at (6.555556,27.597222);
Node157: Box("ungetc",0.750000,0.500000) at (5.097222,23.694444);
Node158: Box("instr",0.750000,0.500000) at (7.597222,23.694444);
Node159: Box("doscan",0.750000,0.500000) at (0.375000,31.500000);
Node160: Box("execl",0.750000,0.500000) at (18.611111,11.972222);
Node161: Box("execv",0.750000,0.500000) at (18.611111,8.069444);
Node162: Box("execve",0.750000,0.500000) at (18.611111,4.166667);
Node163: Box("ioctl",0.750000,0.500000) at (22.277778,0.250000);
Node164: Box("reclaim_nodes",1.625000,0.500000) at (3.416667,31.500000);
Node165: Box("reclaim_edges",1.625000,0.500000) at (5.277778,31.500000);
Node166: Box("reclaim_hashtable",2.138889,0.500000) at (7.402778,31.500000);
Node167: Box("reclaim_samenodesets",2.513889,0.500000) at (9.972222,31.500000);
Node168: Box("freestrings",1.375000,0.500000) at (1.680556,31.500000);
Node169: Box("wait",0.750000,0.500000) at (20.111111,11.972222);
spline ->  from (16.500000,42.958333) to (16.500000,39.555556);
spline ->  from (16.875000,39.263889) to (46.569444,35.652778) to (47.694444,35.472222) to (48.805556,35.305556) to (54.791667,31.722222);
spline ->  from (16.875000,39.263889) to (46.763889,35.652778) to (47.888889,35.472222) to (49.000000,35.305556) to (54.791667,31.722222);
spline ->  from (16.875000,39.250000) to (41.069444,35.472222);
spline ->  from (16.875000,39.222222) to (31.569444,35.652778);
spline ->  from (16.236111,39.055556) to (12.861111,35.652778);
spline ->  from (16.875000,39.208333) to (29.694444,35.597222);
spline ->  from (16.166667,39.055556) to (11.680556,35.458333) to (11.652778,35.305556) to (11.611111,35.152778) to (11.597222,31.750000) to (11.638889,31.541667) to (11.666667,31.347222) to (13.111111,27.847222);
spline ->  from (16.347222,39.055556) to (14.333333,35.652778);
spline ->  from (16.472222,39.055556) to (16.027778,35.652778);
spline ->  from (16.125000,39.208333) to (2.958333,35.513889);
spline ->  from (16.527778,39.055556) to (16.972222,35.652778) to (16.986111,35.402778) to (17.000000,35.152778) to (17.000000,23.944444) to (16.972222,23.763889) to (16.930556,23.597222) to (14.458333,20.027778);
spline ->  from (16.125000,39.166667) to (7.000000,35.652778);
spline ->  from (41.944444,35.152778) to (47.513889,31.750000);
spline ->  from (41.527778,35.152778) to (41.847222,31.750000);
spline ->  from (41.611111,35.152778) to (43.069444,31.750000);
spline ->  from (41.930556,35.250000) to (51.652778,31.527778) to (51.722222,31.458333) to (51.791667,31.402778) to (54.250000,27.847222);
spline ->  from (41.375000,35.152778) to (39.750000,31.750000);
spline ->  from (41.472222,35.152778) to (41.152778,31.750000) to (41.027778,31.458333) to (40.902778,31.166667) to (40.750000,31.125000) to (29.083333,27.847222);
spline ->  from (41.180556,35.152778) to (37.083333,31.750000) to (36.902778,31.611111) to (36.708333,31.472222) to (25.847222,27.847222);
spline ->  from (41.069444,35.194444) to (34.250000,31.750000);
spline ->  from (41.069444,35.250000) to (31.708333,31.597222) to (31.527778,31.375000) to (31.333333,31.152778) to (12.680556,27.944444) to (12.541667,27.875000) to (12.388889,27.819444) to (9.777778,23.944444);
spline ->  from (41.819444,35.152778) to (45.930556,31.750000);
spline ->  from (41.736111,35.152778) to (44.763889,31.750000);
spline ->  from (32.055556,35.152778) to (32.055556,31.750000) to (32.027778,31.513889) to (31.986111,31.277778) to (22.277778,27.847222);
spline ->  from (12.569444,35.152778) to (12.125000,31.750000) to (12.152778,31.555556) to (12.166667,31.361111) to (13.902778,27.736111) to (13.902778,27.541667) to (13.888889,27.347222) to (11.513889,20.027778) to (11.513889,19.777778) to (11.513889,19.527778) to (11.805556,19.402778) to (18.986111,16.055556);
spline ->  from (12.625000,35.152778) to (12.833333,31.750000);
spline ->  from (31.069444,35.152778) to (44.013889,31.833333) to (44.375000,31.694444) to (44.166667,31.430556) to (40.583333,27.666667) to (40.513889,27.500000) to (40.430556,27.347222) to (39.458333,23.944444) to (39.388889,23.763889) to (39.319444,23.583333) to (39.000000,23.319444) to (35.069444,19.833333) to (35.041667,19.680556) to (35.000000,19.527778) to (35.000000,16.125000) to (35.041667,15.986111) to (35.069444,15.861111) to (50.861111,12.055556);
spline ->  from (30.375000,35.152778) to (30.375000,31.750000);
spline ->  from (13.083333,27.347222) to (11.458333,23.944444);
spline ->  from (14.194444,35.152778) to (14.458333,27.847222) to (14.472222,27.597222) to (14.472222,27.347222) to (14.472222,23.944444) to (14.472222,23.694444) to (14.458333,23.444444) to (14.305556,20.027778);
spline ->  from (2.430556,35.152778) to (0.513889,31.750000);
spline ->  from (6.152778,35.152778) to (3.597222,31.750000);
spline ->  from (6.277778,35.152778) to (5.347222,31.750000);
spline ->  from (6.416667,35.152778) to (7.333333,31.750000);
spline ->  from (6.597222,35.152778) to (9.736111,31.750000);
spline ->  from (6.027778,35.152778) to (1.972222,31.750000);
spline ->  from (48.444444,31.250000) to (55.222222,27.847222);
spline ->  from (47.277778,31.250000) to (39.013889,27.777778);
spline ->  from (48.041667,31.250000) to (49.666667,27.847222);
spline ->  from (47.916667,31.250000) to (47.916667,27.847222);
spline ->  from (41.847222,31.250000) to (34.388889,27.847222);
spline ->  from (41.833333,31.250000) to (32.833333,27.847222);
spline ->  from (41.847222,31.250000) to (35.000000,27.833333) to (34.930556,27.583333) to (34.847222,27.347222) to (26.625000,23.944444);
spline ->  from (41.805556,31.250000) to (13.583333,27.638889);
spline ->  from (43.125000,31.250000) to (31.347222,27.847222);
spline ->  from (43.180556,31.250000) to (42.208333,27.847222) to (42.180556,27.597222) to (42.138889,27.347222) to (42.138889,20.027778) to (42.111111,19.861111) to (42.069444,19.694444) to (41.763889,19.402778) to (38.444444,15.944444) to (38.250000,15.708333) to (38.402778,15.625000) to (50.861111,12.083333);
spline ->  from (43.152778,31.250000) to (37.402778,27.847222);
spline ->  from (43.152778,31.250000) to (36.027778,27.791667);
spline ->  from (43.125000,31.250000) to (21.625000,27.972222) to (19.444444,27.638889) to (19.375000,27.527778) to (19.305556,27.430556) to (17.930556,23.944444) to (17.847222,23.777778) to (17.763889,23.625000) to (14.500000,20.027778);
spline ->  from (53.861111,27.430556) to (43.013889,24.069444) to (42.861111,24.027778) to (42.750000,23.736111) to (42.638889,23.444444) to (42.638889,20.027778) to (42.611111,19.861111) to (42.569444,19.708333) to (38.944444,15.944444) to (38.750000,15.708333) to (38.916667,15.625000) to (50.861111,12.083333);
spline ->  from (38.847222,31.250000) to (28.916667,27.847222);
spline ->  from (38.638889,31.250000) to (26.041667,27.847222);
spline ->  from (38.722222,31.250000) to (26.736111,27.930556) to (26.583333,27.708333) to (26.430556,27.500000) to (22.777778,23.944444);
spline ->  from (27.805556,27.347222) to (22.888889,23.944444);
spline ->  from (25.000000,27.347222) to (22.694444,23.944444);
spline ->  from (24.888889,27.347222) to (20.958333,23.944444);
spline ->  from (33.041667,31.250000) to (23.902778,27.847222);
spline ->  from (56.222222,27.375000) to (63.569444,23.944444);
spline ->  from (56.013889,27.347222) to (59.694444,23.944444);
spline ->  from (55.861111,27.347222) to (57.625000,23.944444);
spline ->  from (55.555556,27.347222) to (50.041667,23.944444);
spline ->  from (55.666667,27.347222) to (53.736111,23.944444);
spline ->  from (55.430556,27.347222) to (46.125000,23.944444);
spline ->  from (55.402778,27.347222) to (44.222222,23.944444);
spline ->  from (55.750000,27.347222) to (56.166667,23.944444);
spline ->  from (55.694444,27.347222) to (54.888889,23.944444);
spline ->  from (38.750000,27.347222) to (40.861111,23.944444);
spline ->  from (38.569444,27.347222) to (38.277778,23.944444);
spline ->  from (38.416667,27.347222) to (34.055556,23.944444);
spline ->  from (38.333333,27.347222) to (31.888889,23.944444);
spline ->  from (38.236111,27.347222) to (29.430556,23.944444);
spline ->  from (38.500000,27.347222) to (36.458333,23.944444);
spline ->  from (64.666667,23.583333) to (81.500000,19.902778);
spline ->  from (64.111111,23.444444) to (64.111111,20.027778) to (64.083333,19.819444) to (64.041667,19.625000) to (63.736111,19.402778) to (59.250000,15.986111) to (59.138889,15.833333) to (59.027778,15.694444) to (51.611111,12.152778);
spline ->  from (64.041667,23.444444) to (63.236111,20.027778);
spline ->  from (64.666667,23.597222) to (84.652778,20.027778) to (85.361111,19.777778) to (86.069444,19.527778) to (86.333333,12.222222);
spline ->  from (60.708333,23.444444) to (70.208333,20.027778);
spline ->  from (60.083333,23.444444) to (61.583333,20.027778);
spline ->  from (59.875000,23.444444) to (58.833333,20.027778) to (58.763889,19.861111) to (58.694444,19.708333) to (55.138889,16.027778) to (55.027778,15.916667) to (54.916667,15.805556) to (51.472222,12.222222);
spline ->  from (59.958333,23.444444) to (59.944444,20.027778);
spline ->  from (57.694444,23.444444) to (56.888889,20.027778);
spline ->  from (57.791667,23.444444) to (58.236111,20.027778) to (58.222222,19.861111) to (58.194444,19.708333) to (54.638889,16.027778) to (54.527778,15.902778) to (54.416667,15.791667) to (51.444444,12.222222);
spline ->  from (57.750000,23.444444) to (57.763889,20.027778) to (57.736111,19.847222) to (57.694444,19.666667) to (57.388889,19.402778) to (53.944444,16.125000);
spline ->  from (50.263889,23.541667) to (64.111111,20.055556) to (64.444444,19.972222) to (64.569444,19.750000) to (64.694444,19.527778) to (67.152778,12.222222) to (67.236111,11.986111) to (67.305556,11.763889) to (67.611111,11.597222) to (73.430556,8.277778);
spline ->  from (49.388889,23.444444) to (46.125000,20.027778);
spline ->  from (49.291667,23.444444) to (44.833333,20.027778);
spline ->  from (49.694444,23.444444) to (50.347222,20.027778);
spline ->  from (49.861111,23.444444) to (52.819444,19.861111) to (52.819444,19.736111) to (52.819444,19.611111) to (52.513889,19.402778) to (48.111111,16.250000) to (47.805556,16.027778) to (47.805556,15.916667) to (47.805556,15.805556) to (51.013889,12.222222);
spline ->  from (49.583333,23.444444) to (48.930556,20.027778);
spline ->  from (49.486111,23.444444) to (47.527778,20.027778);
spline ->  from (53.597222,23.444444) to (53.638889,20.027778);
spline ->  from (53.750000,23.444444) to (55.819444,19.902778) to (55.819444,19.763889) to (55.819444,19.638889) to (55.513889,19.402778) to (51.625000,16.250000) to (51.305556,15.986111) to (51.277778,15.805556) to (51.236111,15.625000) to (51.236111,12.222222);
spline ->  from (53.694444,23.444444) to (54.930556,20.027778);
spline ->  from (45.319444,23.444444) to (43.750000,20.027778) to (43.666667,19.861111) to (43.569444,19.708333) to (39.944444,15.944444) to (39.736111,15.694444) to (40.097222,15.541667) to (50.861111,12.097222);
spline ->  from (43.652778,23.444444) to (43.180556,20.027778) to (43.125000,19.861111) to (43.069444,19.708333) to (39.444444,15.944444) to (39.250000,15.708333) to (39.444444,15.625000) to (50.861111,12.083333);
spline ->  from (82.125000,19.527778) to (82.944444,16.125000);
spline ->  from (82.208333,19.527778) to (84.250000,16.000000) to (84.305556,15.805556) to (84.361111,15.625000) to (84.944444,12.222222);
spline ->  from (82.055556,19.527778) to (82.097222,16.125000) to (82.111111,15.875000) to (82.111111,15.625000) to (82.291667,12.222222);
spline ->  from (81.944444,19.527778) to (80.611111,16.125000);
spline ->  from (81.500000,19.680556) to (61.000000,16.125000);
spline ->  from (82.277778,19.527778) to (85.055556,16.125000) to (85.194444,15.875000) to (85.333333,15.625000) to (86.277778,12.222222);
spline ->  from (51.236111,11.722222) to (51.236111,8.319444);
spline ->  from (62.861111,19.527778) to (58.750000,15.944444) to (58.638889,15.791667) to (58.527778,15.638889) to (51.611111,12.166667);
spline ->  from (82.861111,15.625000) to (81.180556,12.222222);
spline ->  from (83.138889,15.625000) to (84.861111,12.222222);
spline ->  from (82.958333,15.625000) to (82.347222,12.222222);
spline ->  from (83.041667,15.625000) to (83.638889,12.222222);
spline ->  from (83.222222,15.625000) to (86.138889,12.222222);
spline ->  from (84.986111,11.722222) to (84.986111,8.319444);
spline ->  from (84.805556,11.722222) to (82.472222,8.319444);
spline ->  from (82.305556,11.722222) to (82.305556,8.319444);
spline ->  from (80.430556,15.625000) to (79.388889,12.222222) to (79.319444,12.013889) to (79.236111,11.805556) to (78.916667,11.597222) to (74.166667,8.319444);
spline ->  from (80.486111,15.625000) to (80.083333,12.222222);
spline ->  from (60.472222,15.638889) to (51.611111,12.125000);
spline ->  from (81.055556,11.722222) to (81.055556,8.319444);
spline ->  from (80.680556,11.722222) to (74.180556,8.263889);
spline ->  from (81.138889,11.722222) to (82.222222,8.319444);
spline ->  from (70.833333,19.527778) to (69.861111,16.125000);
spline ->  from (70.708333,19.527778) to (68.055556,15.972222) to (68.027778,15.791667) to (67.986111,15.625000) to (67.986111,12.222222);
spline ->  from (71.305556,19.527778) to (76.388889,16.125000);
spline ->  from (71.388889,19.527778) to (77.569444,16.125000);
spline ->  from (71.013889,19.527778) to (72.430556,16.125000);
spline ->  from (70.902778,19.527778) to (70.902778,16.125000);
spline ->  from (71.125000,19.527778) to (73.944444,16.125000);
spline ->  from (70.777778,19.527778) to (69.055556,16.013889) to (69.055556,15.875000) to (69.055556,15.750000) to (70.916667,12.222222);
spline ->  from (71.486111,19.527778) to (78.847222,16.125000);
spline ->  from (71.208333,19.527778) to (75.166667,16.125000);
spline ->  from (70.736111,19.527778) to (68.555556,15.986111) to (68.555556,15.805556) to (68.541667,15.625000) to (69.305556,12.222222);
spline ->  from (70.166667,19.527778) to (60.097222,16.125000) to (60.069444,15.888889) to (60.027778,15.666667) to (51.611111,12.125000);
spline ->  from (61.416667,19.527778) to (57.916667,16.125000);
spline ->  from (61.263889,19.527778) to (55.583333,16.083333) to (55.500000,15.972222) to (55.416667,15.875000) to (51.500000,12.222222);
spline ->  from (61.333333,19.527778) to (56.708333,16.125000);
spline ->  from (70.069444,15.625000) to (73.513889,12.222222);
spline ->  from (69.972222,15.625000) to (72.250000,12.222222);
spline ->  from (68.361111,11.736111) to (73.430556,8.319444);
spline ->  from (76.680556,15.625000) to (75.638889,12.222222);
spline ->  from (76.777778,15.625000) to (76.986111,12.222222);
spline ->  from (78.055556,15.625000) to (78.333333,12.222222);
spline ->  from (72.611111,15.625000) to (73.680556,12.222222);
spline ->  from (74.125000,15.625000) to (73.791667,12.222222);
spline ->  from (74.180556,15.625000) to (74.541667,12.222222) to (74.597222,12.041667) to (74.638889,11.861111) to (76.847222,8.319444);
spline ->  from (71.236111,11.722222) to (73.625000,8.319444);
spline ->  from (79.263889,15.625000) to (78.416667,12.222222);
spline ->  from (75.472222,15.625000) to (75.569444,12.222222);
spline ->  from (69.666667,11.722222) to (73.527778,8.319444);
spline ->  from (73.861111,7.819444) to (74.541667,4.416667);
spline ->  from (73.805556,7.819444) to (73.847222,4.416667) to (73.875000,4.166667) to (73.902778,3.916667) to (74.597222,0.500000);
spline ->  from (73.777778,7.819444) to (73.375000,4.416667) to (73.361111,4.166667) to (73.347222,3.916667) to (73.347222,0.500000);
spline ->  from (73.722222,7.819444) to (72.625000,4.416667) to (72.569444,4.166667) to (72.513889,3.916667) to (72.236111,0.500000);
spline ->  from (74.069444,7.819444) to (77.402778,4.416667) to (77.555556,4.166667) to (77.694444,3.916667) to (78.208333,0.500000);
spline ->  from (74.000000,7.819444) to (76.486111,4.263889) to (76.541667,4.083333) to (76.583333,3.916667) to (76.875000,0.500000);
spline ->  from (73.930556,7.819444) to (75.625000,4.305556) to (75.666667,4.111111) to (75.708333,3.916667) to (75.819444,0.500000);
spline ->  from (73.763889,11.722222) to (73.805556,8.319444);
spline ->  from (74.513889,3.916667) to (73.430556,0.500000);
spline ->  from (74.597222,3.916667) to (74.652778,0.500000);
spline ->  from (74.750000,3.916667) to (76.750000,0.500000);
spline ->  from (74.847222,3.916667) to (78.013889,0.500000);
spline ->  from (74.430556,3.916667) to (72.375000,0.500000);
spline ->  from (74.680556,3.916667) to (75.750000,0.500000);
spline ->  from (74.972222,4.222222) to (75.097222,4.277778) to (75.222222,4.166667) to (75.097222,4.055556) to (74.972222,4.111111);
spline ->  from (40.513889,23.444444) to (34.097222,20.027778);
spline ->  from (40.638889,23.444444) to (35.652778,19.888889) to (35.541667,19.805556) to (35.430556,19.736111) to (29.861111,16.125000);
spline ->  from (40.986111,23.444444) to (40.708333,20.027778);
spline ->  from (40.805556,23.444444) to (38.069444,20.027778);
spline ->  from (37.736111,23.444444) to (30.888889,19.861111) to (30.777778,19.694444) to (30.652778,19.527778) to (29.555556,16.125000);
spline ->  from (37.819444,23.444444) to (32.361111,20.013889);
spline ->  from (33.500000,23.444444) to (29.680556,19.847222) to (29.638889,19.680556) to (29.597222,19.527778) to (29.486111,16.125000);
spline ->  from (33.027778,23.444444) to (22.180556,20.000000);
spline ->  from (33.430556,23.444444) to (28.805556,20.027778);
spline ->  from (31.069444,23.444444) to (26.555556,20.027778);
spline ->  from (31.152778,23.444444) to (27.847222,20.152778) to (27.541667,19.847222) to (27.541667,19.750000) to (27.541667,19.652778) to (29.347222,16.125000);
spline ->  from (28.513889,23.444444) to (25.041667,19.847222) to (25.041667,19.736111) to (25.041667,19.638889) to (25.347222,19.402778) to (29.180556,16.125000);
spline ->  from (35.888889,23.444444) to (30.388889,19.930556) to (30.291667,19.722222) to (30.180556,19.527778) to (29.513889,16.125000);
spline ->  from (33.347222,19.527778) to (29.736111,16.125000);
spline ->  from (29.097222,15.680556) to (22.763889,12.180556);
spline ->  from (22.375000,11.722222) to (22.277778,8.319444);
spline ->  from (22.180556,7.819444) to (20.916667,4.416667);
spline ->  from (21.902778,7.833333) to (16.861111,4.416667);
spline ->  from (22.277778,7.819444) to (22.277778,4.416667);
spline ->  from (57.222222,19.527778) to (62.236111,16.125000);
spline ->  from (56.500000,19.527778) to (52.361111,16.125000);
spline ->  from (56.555556,19.527778) to (52.958333,15.972222) to (52.875000,15.847222) to (52.791667,15.722222) to (51.333333,12.222222);
spline ->  from (53.513889,15.625000) to (51.388889,12.222222);
spline ->  from (62.819444,15.625000) to (65.597222,12.222222);
spline ->  from (62.666667,15.625000) to (63.555556,12.222222);
spline ->  from (62.875000,15.625000) to (66.361111,12.347222) to (66.666667,12.055556) to (66.680556,11.888889) to (66.694444,11.722222) to (66.194444,8.319444);
spline ->  from (61.861111,15.625000) to (51.611111,12.097222);
spline ->  from (62.750000,15.625000) to (64.680556,12.097222) to (64.680556,11.902778) to (64.666667,11.722222) to (63.638889,8.319444);
spline ->  from (62.513889,15.625000) to (61.458333,12.222222) to (61.430556,11.972222) to (61.402778,11.722222) to (61.791667,8.319444);
spline ->  from (62.458333,15.625000) to (60.583333,12.222222);
spline ->  from (62.569444,15.625000) to (62.277778,12.222222);
spline ->  from (52.000000,15.625000) to (51.291667,12.222222);
spline ->  from (77.000000,11.722222) to (77.000000,8.319444);
spline ->  from (65.833333,11.722222) to (66.125000,8.319444);
spline ->  from (64.250000,12.027778) to (64.375000,12.083333) to (64.500000,11.972222) to (64.375000,11.861111) to (64.250000,11.916667);
spline ->  from (66.527778,8.125000) to (66.652778,8.180556) to (66.777778,8.069444) to (66.652778,7.958333) to (66.527778,8.013889);
spline ->  from (60.444444,11.722222) to (60.444444,8.319444);
spline ->  from (62.347222,11.722222) to (63.486111,8.319444);
spline ->  from (62.222222,11.722222) to (61.847222,8.319444);
spline ->  from (78.263889,11.722222) to (77.083333,8.319444);
spline ->  from (33.305556,27.347222) to (26.527778,23.944444);
spline ->  from (31.819444,27.347222) to (24.944444,23.930556) to (24.888889,23.680556) to (24.819444,23.444444) to (15.583333,20.027778);
spline ->  from (31.791667,27.347222) to (24.472222,23.930556);
spline ->  from (45.847222,19.527778) to (45.388889,16.125000);
spline ->  from (45.763889,19.527778) to (44.222222,16.125000);
spline ->  from (45.666667,19.527778) to (42.930556,16.125000);
spline ->  from (45.555556,19.527778) to (41.361111,16.125000);
spline ->  from (44.569444,19.527778) to (45.305556,16.125000);
spline ->  from (44.388889,19.527778) to (42.847222,16.125000);
spline ->  from (44.277778,19.527778) to (41.277778,16.125000);
spline ->  from (44.486111,19.527778) to (44.138889,16.125000);
spline ->  from (50.041667,19.527778) to (45.680556,16.125000);
spline ->  from (30.083333,27.347222) to (23.041667,23.944444);
spline ->  from (37.513889,27.347222) to (39.819444,23.805556) to (39.819444,23.708333) to (39.819444,23.625000) to (36.069444,19.847222) to (36.055556,19.680556) to (36.041667,19.527778) to (36.638889,16.125000) to (36.694444,15.986111) to (36.750000,15.861111) to (50.861111,12.069444);
spline ->  from (36.777778,27.388889) to (21.013889,23.944444);
spline ->  from (36.861111,27.347222) to (27.236111,23.944444) to (27.194444,23.694444) to (27.138889,23.458333) to (20.305556,20.027778);
spline ->  from (21.625000,27.430556) to (13.986111,23.875000);
spline ->  from (21.777778,27.347222) to (18.861111,23.944444) to (18.722222,23.777778) to (18.583333,23.625000) to (15.375000,20.027778);
spline ->  from (22.152778,27.347222) to (34.763889,24.027778) to (35.111111,23.875000) to (34.916667,23.611111) to (31.305556,19.847222) to (31.319444,19.680556) to (31.319444,19.527778) to (32.291667,16.125000) to (32.361111,15.986111) to (32.430556,15.861111) to (50.861111,12.055556);
spline ->  from (22.027778,27.347222) to (23.958333,23.944444);
spline ->  from (13.583333,23.444444) to (13.305556,20.027778);
spline ->  from (13.513889,23.444444) to (12.319444,20.027778);
spline ->  from (45.763889,15.625000) to (50.861111,12.222222);
spline ->  from (22.277778,3.916667) to (22.277778,0.500000);
spline ->  from (51.444444,19.527778) to (45.777778,16.125000);
spline ->  from (51.541667,19.527778) to (47.069444,16.125000);
spline ->  from (47.041667,15.625000) to (50.944444,12.222222);
spline ->  from (53.444444,19.527778) to (50.805556,15.972222) to (50.791667,15.791667) to (50.763889,15.625000) to (51.208333,12.222222);
spline ->  from (53.291667,19.527778) to (48.930556,16.125000);
spline ->  from (53.375000,19.527778) to (50.097222,16.125000);
spline ->  from (54.013889,19.833333) to (54.138889,19.888889) to (54.263889,19.777778) to (54.138889,19.666667) to (54.013889,19.722222);
spline ->  from (22.027778,23.444444) to (15.472222,20.027778);
spline ->  from (22.180556,23.444444) to (17.694444,20.027778);
spline ->  from (22.347222,23.444444) to (19.972222,20.027778);
spline ->  from (21.347222,19.527778) to (19.500000,16.125000);
spline ->  from (21.541667,19.527778) to (22.000000,16.125000);
spline ->  from (21.444444,19.527778) to (20.666667,16.125000);
spline ->  from (48.791667,15.625000) to (51.069444,12.222222);
spline ->  from (19.361111,15.625000) to (19.361111,12.222222) to (19.375000,11.972222) to (19.388889,11.722222) to (19.833333,8.319444) to (19.888889,8.069444) to (19.930556,7.819444) to (20.763889,4.416667);
spline ->  from (22.055556,15.625000) to (22.361111,12.222222);
spline ->  from (20.638889,15.625000) to (21.083333,12.222222);
spline ->  from (20.472222,15.625000) to (18.736111,12.222222);
spline ->  from (20.583333,15.625000) to (20.138889,12.222222);
spline ->  from (20.583333,23.486111) to (14.666667,20.000000);
spline ->  from (26.444444,19.527778) to (29.263889,16.125000);
spline ->  from (55.541667,31.361111) to (64.472222,27.847222) to (64.791667,27.597222) to (65.111111,27.347222) to (65.111111,20.027778) to (65.083333,19.875000) to (65.041667,19.736111) to (59.750000,16.000000) to (59.638889,15.833333) to (59.527778,15.680556) to (51.611111,12.138889);
spline ->  from (54.791667,31.430556) to (36.638889,27.944444) to (36.416667,27.597222) to (36.180556,27.263889) to (24.472222,23.847222);
spline ->  from (54.791667,31.416667) to (37.555556,27.847222);
spline ->  from (55.111111,31.250000) to (54.458333,27.847222);
spline ->  from (11.291667,23.444444) to (10.722222,20.027778);
spline ->  from (10.736111,19.527778) to (11.402778,16.125000);
spline ->  from (10.680556,19.527778) to (10.708333,12.222222) to (10.750000,12.041667) to (10.777778,11.875000) to (16.305556,4.416667);
spline ->  from (21.111111,11.722222) to (21.111111,8.319444);
spline ->  from (6.458333,27.347222) to (5.194444,23.944444);
spline ->  from (6.763889,27.347222) to (9.416667,23.944444);
spline ->  from (6.625000,27.347222) to (7.527778,23.944444);
spline ->  from (7.805556,23.444444) to (10.486111,20.027778);
spline ->  from (0.750000,31.277778) to (6.166667,27.847222);
spline ->  from (0.430556,31.250000) to (1.750000,23.944444) to (1.805556,23.805556) to (1.861111,23.666667) to (10.305556,19.944444);
spline ->  from (0.541667,31.250000) to (4.944444,23.944444);
spline ->  from (18.611111,11.722222) to (18.611111,8.319444);
spline ->  from (18.611111,7.819444) to (18.611111,4.416667);
.PE
.ce
\fBFigure 13.\fR  CallGraph-2.
.KE
.NH 1
Conclusions
.PP
.I Dag
is a practical tool for drawing directed graphs.
We have favored ease of use over low-level drawing control,
by providing a simple graph description language and a few
well-tuned heuristics that produce
good drawings quickly in common cases.
.NH 1
References
.LP
|reference_placement
.BP
.Tm Syntax	s
.SH
Appendix A.  \fIDag\fP Syntax
.LP
.nf
\fI
program	\fR:\fP	statement-list
		\fR;\fI

statement-list	\fR:\fP	statement-list statement
		\fR|\fP	\f5/* empty */\fP
		\fR;\fI

statement	\fR:\fP	draw-statement
		\fR|\fP	edge-statement
		\fR|\fP	control-statement
		\fR;\fI

draw-statement	\fR:\fP	\f5draw \fPnode-list node-desc \f5;\fP
		\fR|\fP	\f5draw nodes \fPnode-desc \f5;\fP
		\fR|\fP	\f5draw edges \fPedge-desc \f5;\fP
		\fR;\fI

node-list		\fR:\fP	node-list \fR[\fP\f5,\fP\fR]\fP node-name
		\fR|\fP	node-name
		\fR;\fI

node-desc	\fR:\fP	node-desc-item
		\fR|\fP	node-desc node-desc-item
		\fR;\fI

node-desc-item	\fR:\fP	\f5width\fP float
		\fR|\fP	\f5height\fP float
		\fR|\fP	\f5pointsize\fP integer
		\fR|\fP	\f5label\fP string
		\fR|\fP	\f5label\fP \f5 { \fPdrawing-code\f5 } \fP
		\fR|\fP	\f5as { \fPdrawing-code\f5 } \fP
		\fR|\fP	\f5as\fP string
		\fR|\fP	\f5color\fP string
		\fR;\fI

edge-statement	\fR:\fP	\fR[\fP\f5ordered\fP\fR]\fP \fR[[\fP\f5back\fP\fR]\fPedge from\fP\fR]\fP node-name \fR[\fP\f5to\fP\fR]\fP head-list \f5;\fP
		\fR|\fP	\fR[\fP\f5back\fP\fR]\fP\f5path\fP \fR[\fP\f5from\fP\fR]\fP node-name \fR[\fP\f5to\fP\fR]\fP head-list \f5;\fP
		\fR;\fI

head-list		\fR|\fP	head-list head
		\fR|\fP	head-list \fR[\fP\f5,\fP\fR]\fP \fR[\fP\f5to\fP\fR]\fP head
		\fR|\fP	\f5/* empty */\fP
		\fR;\fI

head		\fR:\fP	node-name edge-desc
		\fR|\fP	node-name
		\fR;\fI

edge-desc	\fR:\fP	edge-desc-item
		\fR|\fP	edge-desc edge-desc-item
		\fR;\fI

edge-desc-item	\fR:\fP	\f5weight \fP integer
		\fR|\fP	\f5label \fPstring
		\fR|\fP	\f5label { \fPdrawing-code\f5 }\fP
		\fR|\fP	\f5pointsize\fP integer
		\fR|\fP	\f5color\fP string
		\fR|\fP	inkvalue
		\fR;\fI

inkvalue		\fR:\fP	\f5solid\fP
		\fR|\fP	\f5dashed\fP
		\fR|\fP	\f5dotted\fP
		\fR|\fP	\f5invis\fP
		\fR;\fI

control-statement	\fR:\fP	\f5separate\fP sep-list \f5;\fP
		\fR|\fP	\f5minimum rank\fP node-list \f5;\fP
		\fR|\fP	\f5maximum rank\fP node-list \f5;\fP
		\fR|\fP	\f5same rank\fP node-list \f5;\fP
		\fR;\fI

sep-list		\fR:\fP	sep-list sep-item
		\fR|\fP	\f5/* empty */\fP
		\fR;\fI

sep-item		\fR:\fP	\f5nodes\fP float
		\fR|\fP	\f5ranks\fP float
		\fR|\fP	\f5ranks\fP float \f5exactly\fP
		\fR|\fP	\f5ranks\fP float \f5equally\fP
		\fR;\fI

node-name	\fR:\fP	string
		\fR;\fI

.fi
\fR
.PP
A \fIstring\fP is any sequence of non-whitespace, non-punctuation characters,
or any quoted string that does not contain a newline.
A \fIdrawing-code\fP is any sequence of characters containing balanced 
left and right curly braces.
.NH 1
Appendix B.  \*(PO Interface
.PP
\f5dag -Tps\fP generates \*(PO\ code.
The drawing is in the default \*(PO\ coordinate system.
Two procedures are needed to define a new node shape.
The first draws the shape over the current point.
It takes three arguments off the stack:
the node width, height, and name.
The second procedure computes where edges intersect the shape.
Its arguments are the $x$ and $y$ dimensions of the shape, and a
ray.  The first point is the origin and the second point that
defines the ray is always on the bounding box of the node when
it is placed at $(0,0)$.  This procedure returns the point where
the ray intersects the shape (or if it misses the shape, then
the nearest point or at least the point on the bounding box
that was passed as an argument).  The argument points are always
in the same quadrant.
.PP
For some examples of shape-drawing procedures, examine the
output of \f5dag -Tps\fP for definitions of procedures such
as \f5Ellipse\fP and \f5Ellipse_clip\fP.
.PP
Shaded nodes may be useful when color output is not available.
Here is an example.
.P1
\&.GR
\&.PS
/setdagcolor {/daggrayscale exch def} def

/ShadedBox {
    /height exch def
    /width exch def
    /nodename exch def
    currentpoint 2 copy
.P3
    newpath
        moveto
        width -2 div
        height -2 div
        rmoveto
        width 0 rlineto
        0 height rlineto
        width neg 0 rlineto
    closepath
.P3
    gsave
        daggrayscale setgray
        fill
    grestore
    stroke
    moveto 
    nodename width .9 mul height .9 mul daglabel
} def
.P3
/ShadedBox_clip { Box_clip } def
\&.PE
draw nodes as ShadedBox;
draw nodes color "1";
draw a color ".7";
draw b color ".9";
a b c;
\&.GE
.P2
.PP
\&\f5setdagcolor\fP is called before drawing a node or edge.
The color value is emitted as \*(PO code
which usually pushes something on the stack
that \f5setdagcolor\fP consumes.
For drawing shaded boxes, colors will be real values
between 0 and 1, where 0 is black and 1 is white.
Thus \f5setdagcolor\fP is redefined to store its argument
it in the global variable \f5daggrayscale\fP where
\f5ShadedBox\fP can find it later.
.PP
In the definition of \f5ShadedBox\fP, the path of rectangle is created
and then the sequence \f5daggrayscale setgray fill\fP shades its interior.
The shading is done within \f5gsave/grestore\fP because
we want to shade neither the outline of the box itself nor its label.
.PP
Since \f5ShadedBox\fP has the same boundary as \f5Box\fP,
\f5ShadedBox_clip\fP is implemented by calling \f5Box_clip\fP.
