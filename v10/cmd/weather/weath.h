struct wline {
	char anam[3]; /* airport code */
	char nlat[5]; /* north latitude */
	char nlng[6]; /* west longitude */
	char wtime[4]; /* GMT */
	char wtemp[3]; /* temperature, farenheit */
	char wdewpt[3]; /* dewpoint */
	char wweath[6]; /* string, containing as shown ... */
		/*
		case 'A': strcat(temp, " hail"); break;
		case 'D': strcat(temp, " dust"); break;
		case 'F': strcat(temp, " fog"); break;
		case 'H': strcat(temp, " haze"); break;
		case 'I': if (s[1]=='P') strcat(temp, " sleet"); break;
		case 'K': strcat(temp, " smoke"); break;
		case 'L': strcat(temp, " drizzle"); break;
		case 'R': strcat(temp, " rain"); break;
		case 'W': strcat(temp, " showers"); break;
		case 'S': strcat(temp, " snow"); break;
		case 'T': strcat(temp, " thunderstorms"); break;
		case 'Z': strcat(temp, " freezing"); break;
		*/
	char wwind[7]; /* wind: xxyy[Gzz]: xx bearing (xx0 degrees), yy knots */
		/* Gzz: gusting to zz knots */
	char wcloud[1]; /* as shown */
		/*
		case 'C': s= "clear"; break;
		case 'P': s= "partly cloudy"; break;
		case 'O': s= "overcast"; break;
		default: s= "ordinary"; break;
		*/
	char wpress[3]; /* barometer, offset from 30 inches */
	char wday[3]; /* day of year */
	char newline[1];
	};
