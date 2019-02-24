if echo 'a' |  -s 'a'; then :; else echo '1 fails  a ~ a' ; fi
if echo 'ba' |  -s 'a'; then :; else echo '2 fails  a ~ ba' ; fi
if echo 'bab' |  -s 'a'; then :; else echo '3 fails  a ~ bab' ; fi
if echo '' |  -s 'a'; then echo '4 fails  a !~ ' ;else :; fi
if echo 'x' |  -s 'a'; then echo '5 fails  a !~ x' ;else :; fi
if echo 'xxxxx' |  -s 'a'; then echo '6 fails  a !~ xxxxx' ;else :; fi
if echo 'x' |  -s '.'; then :; else echo '7 fails  . ~ x' ; fi
if echo 'xxx' |  -s '.'; then :; else echo '8 fails  . ~ xxx' ; fi
if echo '' |  -s '.'; then echo '9 fails  . !~ ' ;else :; fi
if echo 'xa' |  -s '.a'; then :; else echo '10 fails  .a ~ xa' ; fi
if echo 'xxa' |  -s '.a'; then :; else echo '11 fails  .a ~ xxa' ; fi
if echo 'xax' |  -s '.a'; then :; else echo '12 fails  .a ~ xax' ; fi
if echo 'a' |  -s '.a'; then echo '13 fails  .a !~ a' ;else :; fi
if echo 'ab' |  -s '.a'; then echo '14 fails  .a !~ ab' ;else :; fi
if echo '' |  -s '.a'; then echo '15 fails  .a !~ ' ;else :; fi
if echo 'x' |  -s '$'; then :; else echo '16 fails  $ ~ x' ; fi
if echo '' |  -s '$'; then :; else echo '17 fails  $ ~ ' ; fi
if echo 'x' |  -s '.$'; then :; else echo '18 fails  .$ ~ x' ; fi
if echo '' |  -s '.$'; then echo '19 fails  .$ !~ ' ;else :; fi
if echo 'a' |  -s 'a$'; then :; else echo '20 fails  a$ ~ a' ; fi
if echo 'ba' |  -s 'a$'; then :; else echo '21 fails  a$ ~ ba' ; fi
if echo 'bbba' |  -s 'a$'; then :; else echo '22 fails  a$ ~ bbba' ; fi
if echo 'ab' |  -s 'a$'; then echo '23 fails  a$ !~ ab' ;else :; fi
if echo 'x' |  -s 'a$'; then echo '24 fails  a$ !~ x' ;else :; fi
if echo '' |  -s 'a$'; then echo '25 fails  a$ !~ ' ;else :; fi
if echo 'x' |  -s '^'; then :; else echo '26 fails  ^ ~ x' ; fi
if echo '' |  -s '^'; then :; else echo '27 fails  ^ ~ ' ; fi
if echo '^' |  -s '^'; then :; else echo '28 fails  ^ ~ ^' ; fi
if echo 'a' |  -s '^a$'; then :; else echo '29 fails  ^a$ ~ a' ; fi
if echo 'xa' |  -s '^a$'; then echo '30 fails  ^a$ !~ xa' ;else :; fi
if echo 'ax' |  -s '^a$'; then echo '31 fails  ^a$ !~ ax' ;else :; fi
if echo 'xax' |  -s '^a$'; then echo '32 fails  ^a$ !~ xax' ;else :; fi
if echo '' |  -s '^a$'; then echo '33 fails  ^a$ !~ ' ;else :; fi
if echo 'ax' |  -s '^a.$'; then :; else echo '34 fails  ^a.$ ~ ax' ; fi
if echo 'aa' |  -s '^a.$'; then :; else echo '35 fails  ^a.$ ~ aa' ; fi
if echo 'xa' |  -s '^a.$'; then echo '36 fails  ^a.$ !~ xa' ;else :; fi
if echo 'aaa' |  -s '^a.$'; then echo '37 fails  ^a.$ !~ aaa' ;else :; fi
if echo 'axy' |  -s '^a.$'; then echo '38 fails  ^a.$ !~ axy' ;else :; fi
if echo '' |  -s '^a.$'; then echo '39 fails  ^a.$ !~ ' ;else :; fi
if echo '' |  -s '^$'; then :; else echo '40 fails  ^$ ~ ' ; fi
if echo 'x' |  -s '^$'; then echo '41 fails  ^$ !~ x' ;else :; fi
if echo '^' |  -s '^$'; then echo '42 fails  ^$ !~ ^' ;else :; fi
if echo 'xa' |  -s '^.a'; then :; else echo '43 fails  ^.a ~ xa' ; fi
if echo 'xaa' |  -s '^.a'; then :; else echo '44 fails  ^.a ~ xaa' ; fi
if echo 'a' |  -s '^.a'; then echo '45 fails  ^.a !~ a' ;else :; fi
if echo '' |  -s '^.a'; then echo '46 fails  ^.a !~ ' ;else :; fi
if echo 'a' |  -s '^.*a'; then :; else echo '47 fails  ^.*a ~ a' ; fi
if echo 'xa' |  -s '^.*a'; then :; else echo '48 fails  ^.*a ~ xa' ; fi
if echo 'xxxxxxa' |  -s '^.*a'; then :; else echo '49 fails  ^.*a ~ xxxxxxa' ; fi
if echo '' |  -s '^.*a'; then echo '50 fails  ^.*a !~ ' ;else :; fi
if echo 'xa' |  -s '^.+a'; then :; else echo '51 fails  ^.+a ~ xa' ; fi
if echo 'xxxxxxa' |  -s '^.+a'; then :; else echo '52 fails  ^.+a ~ xxxxxxa' ; fi
if echo '' |  -s '^.+a'; then echo '53 fails  ^.+a !~ ' ;else :; fi
if echo 'a' |  -s '^.+a'; then echo '54 fails  ^.+a !~ a' ;else :; fi
if echo 'ax' |  -s '^.+a'; then echo '55 fails  ^.+a !~ ax' ;else :; fi
if echo '' |  -s 'a*'; then :; else echo '56 fails  a* ~ ' ; fi
if echo 'a' |  -s 'a*'; then :; else echo '57 fails  a* ~ a' ; fi
if echo 'aaaa' |  -s 'a*'; then :; else echo '58 fails  a* ~ aaaa' ; fi
if echo 'xa' |  -s 'a*'; then :; else echo '59 fails  a* ~ xa' ; fi
if echo 'xxxx' |  -s 'a*'; then :; else echo '60 fails  a* ~ xxxx' ; fi
if echo 'a' |  -s 'aa*'; then :; else echo '61 fails  aa* ~ a' ; fi
if echo 'aaa' |  -s 'aa*'; then :; else echo '62 fails  aa* ~ aaa' ; fi
if echo 'xa' |  -s 'aa*'; then :; else echo '63 fails  aa* ~ xa' ; fi
if echo 'xxxx' |  -s 'aa*'; then echo '64 fails  aa* !~ xxxx' ;else :; fi
if echo '' |  -s 'aa*'; then echo '65 fails  aa* !~ ' ;else :; fi
if echo 'x$' |  -s '\$'; then :; else echo '66 fails  \$ ~ x$' ; fi
if echo '$' |  -s '\$'; then :; else echo '67 fails  \$ ~ $' ; fi
if echo '$x' |  -s '\$'; then :; else echo '68 fails  \$ ~ $x' ; fi
if echo '' |  -s '\$'; then echo '69 fails  \$ !~ ' ;else :; fi
if echo 'x' |  -s '\$'; then echo '70 fails  \$ !~ x' ;else :; fi
if echo '.' |  -s '\.'; then :; else echo '71 fails  \. ~ .' ; fi
if echo 'x' |  -s '\.'; then echo '72 fails  \. !~ x' ;else :; fi
if echo '' |  -s '\.'; then echo '73 fails  \. !~ ' ;else :; fi
if echo 'a^' |  -G -s '.^$'; then :; else echo '74 fails  -G .^$ ~ a^' ; fi
if echo '' |  -G -s '.^$'; then echo '75 fails  -G .^$ !~ ' ;else :; fi
if echo 'a^$' |  -G -s '.^$'; then echo '76 fails  -G .^$ !~ a^$' ;else :; fi
if echo 'x' |  -G -s '^x$'; then :; else echo '77 fails  -G ^x$ ~ x' ; fi
if echo 'yx' |  -G -s '^x$'; then echo '78 fails  -G ^x$ !~ yx' ;else :; fi
if echo 'xy' |  -G -s '^x$'; then echo '79 fails  -G ^x$ !~ xy' ;else :; fi
if echo 'a$' |  -G -s 'a\$'; then :; else echo '80 fails  -G a\$ ~ a$' ; fi
if echo 'a' |  -G -s 'a\$'; then echo '81 fails  -G a\$ !~ a' ;else :; fi
if echo 'cab' |  -G -s '\(ab\)$'; then :; else echo '82 fails  -G \(ab\)$ ~ cab' ; fi
if echo 'ab' |  -G -s '\(ab\)$'; then :; else echo '83 fails  -G \(ab\)$ ~ ab' ; fi
if echo 'ab$' |  -G -s '\(ab\)$'; then echo '84 fails  -G \(ab\)$ !~ ab$' ;else :; fi
if echo 'xry' |  -s 'xr+y'; then :; else echo '85 fails  xr+y ~ xry' ; fi
if echo 'xry' |  -s 'xr+y'; then :; else echo '85 fails  xr+y ~ xry' ; fi
if echo 'xrry' |  -s 'xr+y'; then :; else echo '86 fails  xr+y ~ xrry' ; fi
if echo 'xrry' |  -s 'xr+y'; then :; else echo '86 fails  xr+y ~ xrry' ; fi
if echo 'xrrrrrry' |  -s 'xr+y'; then :; else echo '87 fails  xr+y ~ xrrrrrry' ; fi
if echo 'xrrrrrry' |  -s 'xr+y'; then :; else echo '87 fails  xr+y ~ xrrrrrry' ; fi
if echo 'ry' |  -s 'xr+y'; then echo '88 fails  xr+y !~ ry' ;else :; fi
if echo 'ry' |  -s 'xr+y'; then echo '88 fails  xr+y !~ ry' ;else :; fi
if echo 'xy' |  -s 'xr+y'; then echo '89 fails  xr+y !~ xy' ;else :; fi
if echo 'xy' |  -s 'xr+y'; then echo '89 fails  xr+y !~ xy' ;else :; fi
if echo 'xy' |  -s 'xr?y'; then :; else echo '90 fails  xr?y ~ xy' ; fi
if echo 'xy' |  -s 'xr?y'; then :; else echo '90 fails  xr?y ~ xy' ; fi
if echo 'xry' |  -s 'xr?y'; then :; else echo '91 fails  xr?y ~ xry' ; fi
if echo 'xry' |  -s 'xr?y'; then :; else echo '91 fails  xr?y ~ xry' ; fi
if echo 'xrry' |  -s 'xr?y'; then echo '92 fails  xr?y !~ xrry' ;else :; fi
if echo 'xrry' |  -s 'xr?y'; then echo '92 fails  xr?y !~ xrry' ;else :; fi
if echo 'abcg' |  -s 'a(bc|def)g'; then :; else echo '93 fails  a(bc|def)g ~ abcg' ; fi
if echo 'abcg' |  -s 'a(bc|def)g'; then :; else echo '93 fails  a(bc|def)g ~ abcg' ; fi
if echo 'adefg' |  -s 'a(bc|def)g'; then :; else echo '94 fails  a(bc|def)g ~ adefg' ; fi
if echo 'adefg' |  -s 'a(bc|def)g'; then :; else echo '94 fails  a(bc|def)g ~ adefg' ; fi
if echo 'abc' |  -s 'a(bc|def)g'; then echo '95 fails  a(bc|def)g !~ abc' ;else :; fi
if echo 'abc' |  -s 'a(bc|def)g'; then echo '95 fails  a(bc|def)g !~ abc' ;else :; fi
if echo 'abg' |  -s 'a(bc|def)g'; then echo '96 fails  a(bc|def)g !~ abg' ;else :; fi
if echo 'abg' |  -s 'a(bc|def)g'; then echo '96 fails  a(bc|def)g !~ abg' ;else :; fi
if echo 'adef' |  -s 'a(bc|def)g'; then echo '97 fails  a(bc|def)g !~ adef' ;else :; fi
if echo 'adef' |  -s 'a(bc|def)g'; then echo '97 fails  a(bc|def)g !~ adef' ;else :; fi
if echo 'adeg' |  -s 'a(bc|def)g'; then echo '98 fails  a(bc|def)g !~ adeg' ;else :; fi
if echo 'adeg' |  -s 'a(bc|def)g'; then echo '98 fails  a(bc|def)g !~ adeg' ;else :; fi
if echo '1' |  -s '[0-9]'; then :; else echo '99 fails  [0-9] ~ 1' ; fi
if echo '567' |  -s '[0-9]'; then :; else echo '100 fails  [0-9] ~ 567' ; fi
if echo 'x0y' |  -s '[0-9]'; then :; else echo '101 fails  [0-9] ~ x0y' ; fi
if echo 'abc' |  -s '[0-9]'; then echo '102 fails  [0-9] !~ abc' ;else :; fi
if echo '' |  -s '[0-9]'; then echo '103 fails  [0-9] !~ ' ;else :; fi
if echo '1' |  -s '[^0-9]'; then echo '104 fails  [^0-9] !~ 1' ;else :; fi
if echo '567' |  -s '[^0-9]'; then echo '105 fails  [^0-9] !~ 567' ;else :; fi
if echo '' |  -s '[^0-9]'; then echo '106 fails  [^0-9] !~ ' ;else :; fi
if echo 'abc' |  -s '[^0-9]'; then :; else echo '107 fails  [^0-9] ~ abc' ; fi
if echo 'x0y' |  -s '[^0-9]'; then :; else echo '108 fails  [^0-9] ~ x0y' ; fi
if echo 'x0y' |  -s 'x[0-9]+y'; then :; else echo '109 fails  x[0-9]+y ~ x0y' ; fi
if echo 'x0y' |  -s 'x[0-9]+y'; then :; else echo '109 fails  x[0-9]+y ~ x0y' ; fi
if echo 'x23y' |  -s 'x[0-9]+y'; then :; else echo '110 fails  x[0-9]+y ~ x23y' ; fi
if echo 'x23y' |  -s 'x[0-9]+y'; then :; else echo '110 fails  x[0-9]+y ~ x23y' ; fi
if echo 'x12345y' |  -s 'x[0-9]+y'; then :; else echo '111 fails  x[0-9]+y ~ x12345y' ; fi
if echo 'x12345y' |  -s 'x[0-9]+y'; then :; else echo '111 fails  x[0-9]+y ~ x12345y' ; fi
if echo '0y' |  -s 'x[0-9]+y'; then echo '112 fails  x[0-9]+y !~ 0y' ;else :; fi
if echo '0y' |  -s 'x[0-9]+y'; then echo '112 fails  x[0-9]+y !~ 0y' ;else :; fi
if echo 'xy' |  -s 'x[0-9]+y'; then echo '113 fails  x[0-9]+y !~ xy' ;else :; fi
if echo 'xy' |  -s 'x[0-9]+y'; then echo '113 fails  x[0-9]+y !~ xy' ;else :; fi
if echo 'xy' |  -s 'x[0-9]?y'; then :; else echo '114 fails  x[0-9]?y ~ xy' ; fi
if echo 'xy' |  -s 'x[0-9]?y'; then :; else echo '114 fails  x[0-9]?y ~ xy' ; fi
if echo 'x1y' |  -s 'x[0-9]?y'; then :; else echo '115 fails  x[0-9]?y ~ x1y' ; fi
if echo 'x1y' |  -s 'x[0-9]?y'; then :; else echo '115 fails  x[0-9]?y ~ x1y' ; fi
if echo 'x23y' |  -s 'x[0-9]?y'; then echo '116 fails  x[0-9]?y !~ x23y' ;else :; fi
if echo 'x23y' |  -s 'x[0-9]?y'; then echo '116 fails  x[0-9]?y !~ x23y' ;else :; fi
if echo 'x' |  -i -s 'X'; then :; else echo '117 fails  -i X ~ x' ; fi
if echo 'read' |  -x -s 'read'; then :; else echo '118 fails  -x read ~ read' ; fi
if echo 'xy read' |  -x -s 'read'; then echo '119 fails  -x read !~ xy read' ;else :; fi
if echo 'x read y' |  -x -s 'read'; then echo '120 fails  -x read !~ x read y' ;else :; fi
if echo 'xread' |  -x -s 'read'; then echo '121 fails  -x read !~ xread' ;else :; fi
if echo 'readx' |  -x -s 'read'; then echo '122 fails  -x read !~ readx' ;else :; fi
if echo 'read' |  -xF -s 'read'; then :; else echo '123 fails  -xF read ~ read' ; fi
if echo 'xy read' |  -xF -s 'read'; then echo '124 fails  -xF read !~ xy read' ;else :; fi
if echo 'x read y' |  -xF -s 'read'; then echo '125 fails  -xF read !~ x read y' ;else :; fi
if echo 'xread' |  -xF -s 'read'; then echo '126 fails  -xF read !~ xread' ;else :; fi
if echo 'readx' |  -xF -s 'read'; then echo '127 fails  -xF read !~ readx' ;else :; fi
if echo 'read' |  -F -s 'read'; then :; else echo '128 fails  -F read ~ read' ; fi
if echo 'xy read' |  -F -s 'read'; then :; else echo '129 fails  -F read ~ xy read' ; fi
if echo 'x read y' |  -F -s 'read'; then :; else echo '130 fails  -F read ~ x read y' ; fi
if echo 'xread' |  -F -s 'read'; then :; else echo '131 fails  -F read ~ xread' ; fi
if echo 'readx' |  -F -s 'read'; then :; else echo '132 fails  -F read ~ readx' ; fi
if echo '.dexx' |  -s '[.]de..'; then :; else echo '133 fails  [.]de.. ~ .dexx' ; fi
if echo '.deyyy' |  -s '[.]de..'; then :; else echo '134 fails  [.]de.. ~ .deyyy' ; fi
if echo '.de' |  -s '[.]de..'; then echo '135 fails  [.]de.. !~ .de' ;else :; fi
if echo '.dex' |  -s '[.]de..'; then echo '136 fails  [.]de.. !~ .dex' ;else :; fi
if echo '|sec' |  -G -s '^|s'; then :; else echo '137 fails  -G ^|s ~ |sec' ; fi
if echo 'sec' |  -G -s '^|s'; then echo '138 fails  -G ^|s !~ sec' ;else :; fi
if echo 'CDAB' |  -G -s '..B'; then :; else echo '139 fails  -G ..B ~ CDAB' ; fi
if echo 'ABCD' |  -G -s '..B'; then echo '140 fails  -G ..B !~ ABCD' ;else :; fi
if echo '$tt$' |  -G -s '$.*tt.*\$'; then :; else echo '141 fails  -G $.*tt.*\$ ~ $tt$' ; fi
if echo 'vivi' |  -s '^([a-z]+)\1$'; then :; else echo '142 fails  ^([a-z]+)\1$ ~ vivi' ; fi
if echo 'vivify' |  -s '^([a-z]+)\1$'; then echo '143 fails  ^([a-z]+)\1$ !~ vivify' ;else :; fi
if echo 'vivi' |  -s '([a-z]+)\1'; then :; else echo '144 fails  ([a-z]+)\1 ~ vivi' ; fi
if echo 'vivify' |  -s '([a-z]+)\1'; then :; else echo '145 fails  ([a-z]+)\1 ~ vivify' ; fi
if echo 'revivi' |  -s '([a-z]+)\1'; then :; else echo '146 fails  ([a-z]+)\1 ~ revivi' ; fi
if echo 'vovify' |  -s '([a-z]+)\1'; then echo '147 fails  ([a-z]+)\1 !~ vovify' ;else :; fi
if echo 'viv' |  -s '([a-z]+)\1'; then echo '148 fails  ([a-z]+)\1 !~ viv' ;else :; fi
if echo 'beriberi' |  -G -s '\(....\).*\1'; then :; else echo '149 fails  -G \(....\).*\1 ~ beriberi' ; fi
if echo 'beriberi' |  -s '(....).*\1'; then :; else echo '150 fails  (....).*\1 ~ beriberi' ; fi
if echo '' |  -s '^$'; then :; else echo '151 fails  ^$ ~ ' ; fi
if echo '' |  -G -s '^$'; then :; else echo '152 fails  -G ^$ ~ ' ; fi
if echo 'abk' |  -s '[ab]\{2\}k'; then :; else echo '153 fails  [ab]\{2\}k ~ abk' ; fi
if echo 'xyaak' |  -s '[ab]\{2\}k'; then :; else echo '154 fails  [ab]\{2\}k ~ xyaak' ; fi
if echo 'zabak' |  -s '[ab]\{2\}k'; then :; else echo '155 fails  [ab]\{2\}k ~ zabak' ; fi
if echo 'zad' |  -s '[ab]\{2\}k'; then echo '156 fails  [ab]\{2\}k !~ zad' ;else :; fi
if echo 'bq' |  -s '[ab]\{2\}k'; then echo '157 fails  [ab]\{2\}k !~ bq' ;else :; fi
if echo 'abq' |  -s '[ab]\{2\}k'; then echo '158 fails  [ab]\{2\}k !~ abq' ;else :; fi
if echo 'abd' |  -s '[ab]\{2,\}d'; then :; else echo '159 fails  [ab]\{2,\}d ~ abd' ; fi
if echo 'abababad' |  -s '[ab]\{2,\}d'; then :; else echo '160 fails  [ab]\{2,\}d ~ abababad' ; fi
if echo 'ad' |  -s '[ab]\{2,\}d'; then echo '161 fails  [ab]\{2,\}d !~ ad' ;else :; fi
if echo 'ababaq' |  -s '[ab]\{2,\}d'; then echo '162 fails  [ab]\{2,\}d !~ ababaq' ;else :; fi
if echo 'qabd' |  -s 'q[ab]\{2,4\}d'; then :; else echo '163 fails  q[ab]\{2,4\}d ~ qabd' ; fi
if echo 'qababd' |  -s 'q[ab]\{2,4\}d'; then :; else echo '164 fails  q[ab]\{2,4\}d ~ qababd' ; fi
if echo 'qaaad' |  -s 'q[ab]\{2,4\}d'; then :; else echo '165 fails  q[ab]\{2,4\}d ~ qaaad' ; fi
if echo 'qad' |  -s 'q[ab]\{2,4\}d'; then echo '166 fails  q[ab]\{2,4\}d !~ qad' ;else :; fi
if echo 'qababad' |  -s 'q[ab]\{2,4\}d'; then echo '167 fails  q[ab]\{2,4\}d !~ qababad' ;else :; fi
if echo 'a]b' |  -E -s 'a[]]b'; then :; else echo '168 fails  -E a[]]b ~ a]b' ; fi
if echo 'a]b' |  -G -s 'a[]]b'; then :; else echo '169 fails  -G a[]]b ~ a]b' ; fi
if echo 'adc' |  -E -s 'a[^]b]c'; then :; else echo '170 fails  -E a[^]b]c ~ adc' ; fi
if echo 'adc' |  -G -s 'a[^]b]c'; then :; else echo '171 fails  -G a[^]b]c ~ adc' ; fi
if echo 'angelo' |  -i -s 'angel[^e]'; then :; else echo '172 fails  -i angel[^e] ~ angelo' ; fi
if echo 'ANGELH' |  -i -s 'angel[^e]'; then :; else echo '173 fails  -i angel[^e] ~ ANGELH' ; fi
if echo 'angel' |  -i -s 'angel[^e]'; then echo '174 fails  -i angel[^e] !~ angel' ;else :; fi
if echo 'ANGEL' |  -i -s 'angel[^e]'; then echo '175 fails  -i angel[^e] !~ ANGEL' ;else :; fi
if echo 'angele' |  -i -s 'angel[^e]'; then echo '176 fails  -i angel[^e] !~ angele' ;else :; fi
if echo 'ANGELE' |  -i -s 'angel[^e]'; then echo '177 fails  -i angel[^e] !~ ANGELE' ;else :; fi
if echo 'abc>' |  -G -s '^[^-].*>'; then :; else echo '178 fails  -G ^[^-].*> ~ abc>' ; fi
if echo '-a>' |  -G -s '^[^-].*>'; then echo '179 fails  -G ^[^-].*> !~ -a>' ;else :; fi
if echo 'abc' |  -i -s '^[A-Z]'; then :; else echo '180 fails  -i ^[A-Z] ~ abc' ; fi
if echo 'ABC' |  -i -s '^[A-Z]'; then :; else echo '181 fails  -i ^[A-Z] ~ ABC' ; fi
if echo 'abc' |  -i -s '^[^A-Z]'; then echo '182 fails  -i ^[^A-Z] !~ abc' ;else :; fi
if echo 'ABC' |  -i -s '^[^A-Z]'; then echo '183 fails  -i ^[^A-Z] !~ ABC' ;else :; fi
if echo '123' |  -i -s '^[^A-Z]'; then :; else echo '184 fails  -i ^[^A-Z] ~ 123' ; fi
if echo '|abc' |  -G -s '|abc'; then :; else echo '185 fails  -G |abc ~ |abc' ; fi
if echo 'abc' |  -G -s '|abc'; then echo '186 fails  -G |abc !~ abc' ;else :; fi
if echo 'acdacaaa' |  -G -s '\(ac*\)c*d[ac]*\1'; then :; else echo '187 fails  -G \(ac*\)c*d[ac]*\1 ~ acdacaaa' ; fi
if echo 'acdacaaa' |  -s '(ac*)c*d[ac]*\1'; then :; else echo '188 fails  (ac*)c*d[ac]*\1 ~ acdacaaa' ; fi
if echo 'am' |  -s 'ram|am'; then :; else echo '189 fails  ram|am ~ am' ; fi
if echo 'abc' |  -x -s '.|..'; then echo '190 fails  -x .|.. !~ abc' ;else :; fi
if echo 'for this line' |  -E -s '[a-za-za-za-za-za-za-za-za-za-z]'; then :; else echo '191 fails  -E [a-za-za-za-za-za-za-za-za-za-z] ~ for this line' ; fi
if echo 'for this line' |  -s '[a-za-za-za-za-za-za-za-za-za-z]'; then :; else echo '192 fails  [a-za-za-za-za-za-za-za-za-za-z] ~ for this line' ; fi
if echo 'but watch out' |  -E -s '[a-za-za-za-za-za-za-za-za-z]'; then :; else echo '193 fails  -E [a-za-za-za-za-za-za-za-za-z] ~ but watch out' ; fi
if echo 'but watch out' |  -s '[a-za-za-za-za-za-za-za-za-z]'; then :; else echo '194 fails  [a-za-za-za-za-za-za-za-za-z] ~ but watch out' ; fi
echo '216 simple tests'
