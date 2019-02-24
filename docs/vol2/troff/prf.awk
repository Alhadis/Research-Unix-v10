cat <<'EOF'
.LP
.2C
.de XY
.SP
\f1\\$1 (\f(CW\\$2\f1)
.br
.ft \\$2
..
.de XX
\N'\\$1'\d\s-4\f1\eN'\\$1'\fP\s0\u
..
.vs +1p
EOF

awk '	BEGIN { fontname = "'$1'"; }
NR == 1	{ name = substr($0, 3); printf ".XY %s \"%s\"\n", name, fontname; }
(NF == 4) && $4	{ printf ".XX %3.3d\n", $4; }' < /n/coma/usr/lib/font/devpost/$1

cat <<'EOF'
.br
.vs
EOF
