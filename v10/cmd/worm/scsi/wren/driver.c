#include	<stdio.h>
#include	"../scsi.h"
#include	"../scsish.h"
#include	"../tcl.h"
#include	"fns.h"
#include	"wren.h"

static Wren *types[] =
{
	&wr_elite,
	&wr_wren5,
	0
};

static Wren *curtype = 0;

static int
setwren(char *err)
{
	int n;
	char product[17];
	int found;
	struct scsi_cmd cmd;
	struct scsi_return ret;

	/* find drive type */
	set6(cmd, 0x12, 0, 0, 0, 32, 0);
	if(s_io(0, &cmd, 0, &ret, 32, err))
		return(-1);
	fixedstr(&ret.data[16], 16, product);
	for(n = 0, found = 0; types[n]; n++)
		if(strcmp(product, types[n]->ident) == 0){
			found = 1;
			break;
		}
	if(found){
		if(types[n] != curtype){
			curtype = types[n];
			printf("drive is a %s (%s)\n",
				curtype->desc, curtype->ident);
		}
	} else {
		curtype = types[0];
		printf("drive is '%s'; pretending it's a %s\n",
			product, curtype->desc);
	}
	return(0);
}

int wr_modesense(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	if(setwren(cd->err))
		ERR_RETURN
	if(curtype->msense == 0){
		strcpy(cd->err, "modesense not implemented");
		ERR_RETURN
	}
	if((*curtype->msense)(cd, argc, argv))
		ERR_RETURN
	return(TCL_OK);
}

int wr_modeselect(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	if(setwren(cd->err))
		ERR_RETURN
	if(curtype->mselect == 0){
		strcpy(cd->err, "modeselect not implemented");
		ERR_RETURN
	}
	if((*curtype->mselect)(cd, argc, argv))
		ERR_RETURN
	return(TCL_OK);
}

int wr_diag(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	if(setwren(cd->err))
		ERR_RETURN
	if(curtype->diag == 0){
		strcpy(cd->err, "diagnostics not implemented");
		ERR_RETURN
	}
	if((*curtype->diag)(cd, argc, argv))
		ERR_RETURN
	return(TCL_OK);
}

int wr_logsense(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	if(setwren(cd->err))
		ERR_RETURN
	if(curtype->lsense == 0){
		strcpy(cd->err, "logging not implemented");
		ERR_RETURN
	}
	if((*curtype->lsense)(cd, argc, argv))
		ERR_RETURN
	return(TCL_OK);
}

int wr_logselect(ClientData cd, Tcl_Interp *it, int argc, char **argv)
{
	if(setwren(cd->err))
		ERR_RETURN
	if(curtype->lselect == 0){
		strcpy(cd->err, "logging not implemented");
		ERR_RETURN
	}
	if((*curtype->lselect)(cd, argc, argv))
		ERR_RETURN
	return(TCL_OK);
}
