/* Copyright (c) 1989, 1990 AT&T --- All Rights Reserved.              */
/* THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T.                */
/* The copyright notice does not imply actual or intended publication. */
/* AUTHORS:                                            */
/*     H. S. Baird - ATT-BL MH - first versions        */

#define UNITS "icpPsu"	/* units notation for:  inches, cm, points, picas,
			   scanner pixel (Scoor), and basic unit (also Scoor) */

#define INCHES_PER_POINT (0.0138)	/* cf. Chicago Manual of Style */

#define RADIANS_PER_DEGREE (PI/180.0)
#define DtoR (RADIANS_PER_DEGREE)	/* degrees * DtoR = radians */

#define Radians double		/* angle in radians */

/* machine-independent I/O has a resolution of 0.1 second of arc */
#define fwri_Radians(F,V) fwri_int3((F),(V)*2062648.1)
#define frdi_Radians(F) (frdi_int3(F)/(Radians)2062648.1)
