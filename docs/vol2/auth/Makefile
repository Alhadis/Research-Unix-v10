PARTS=abs.ms s1.ms s2.ms s3.ms conc.ms
FIGS=fig1.x fig2.x fig6.x fig7.x fig8.x fig9.x fig10.x fig11.x

paper: $(PARTS) ${FIGS}
	refer -n -e -p refs.bib ${PARTS} | \
	troff -Tpost -ms > paper

manpage: authmgr.8
	troff -Tpost -man authmgr.8 > manpage

proof: paper
	proof paper

%.x: %.pic
	pic $%.pic > $%.x
