out:	ipcpaper.rf pfig1 pfig2 pfig3
	troff -ms ipcpaper.rf >out

ipcpaper.rf:	ipcpaper paper.refs
	refer -e -p paper.refs ipcpaper >ipcpaper.rf

pfig1:	fig1
	pic -D fig1 >pfig1

pfig2:	fig2
	pic -D fig2 >pfig2

pfig3:	fig3
	pic -D fig3 >pfig3

clean:
	rm -f out pfig* paper.refs.ig
