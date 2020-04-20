#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sunangle.h"
#include "jpleph.h"

int getsunangle(double lat,double lon,double ht,
            int year,int month,int day,int hour,int min,double sec,
            double *azim,double *elev)
{
  void *ephem;
  char ephnames[400][6];
  int errcode,i;
  double et,gast,gastradians,t,psia,omegaa,chia,eps0;
  double S1,C1,S2,C2,S3,C3,S4,C4;
  double P11,P12,P13,P21,P22,P23,P31,P32,P33;
  double sungha,hlength,sundecl,sunghadeg,sundecldeg;
  double robs_ef[3],rsun2000[3],rsunmean[3],rsun_ef[3],robs2sun_ef[3],robs2sun_lh[3];
  double ephvals[400];

  // Convert calendar date/time to full Julian date
  kal2jul(year,month,day,hour,min,sec,&et);

  // Compute earth-fixed cartesian coordinates of the observer in AU
  errcode = geod_cart(robs_ef,lat,lon,ht,AE,FLAT);
  if (errcode)
    exit(-1);
  for (i=0;i<3;i++)
    robs_ef[i] /= AU2METERS;

  // Compute the Greenwich Apparent Sidereal Time in hours
  gast = get_gast(et);
  //printf("GAST: %lf %d %lf\n",gast*15.0,int(gast*15.0),60.0*(gast*15.0-int(gast*15.0)));

  // Compute J2000 sun position
  ephem = jpl_init_ephemeris(EPHFILE,ephnames,ephvals);
  errcode = jpl_pleph(ephem,et,11,3,rsun2000,0);
  jpl_close_ephemeris(ephem);
  if (errcode)
    exit(-1);
  //printf("Sun J2000 xyz: %lf %lf %lf\n",rsun2000[0]*AU2METERS/1000.0,
  //       rsun2000[1]*AU2METERS/1000.0,rsun2000[2]*AU2METERS/1000.0);

  // Correct the sun vector for precession according to IAU2000A model
  eps0 = 84381.406; // arcsecs, as are psia, omegaa and chia below;
  t = (et-2451545.0)/36525.0; // time argument is centuries after J2000 epoch
  psia = 5038.481507*t-1.0790069*t*t-0.00114045*t*t*t+0.000132851*t*t*t*t
         -0.0000000951*t*t*t*t*t;
  omegaa = eps0-0.025754*t+0.0512623*t*t-0.00772503*t*t*t-0.000000467*t*t*t*t
           +0.0000003337*t*t*t*t*t;
  chia = 10.556403*t-2.3814292*t*t-0.00121197*t*t*t+0.000170663*t*t*t*t
         -0.0000000560*t*t*t*t*t;
  eps0 = eps0/60.0/60.0*PI/180.0;  // convert to rad
  psia = psia/60.0/60.0*PI/180.0;  // convert to rad
  omegaa = omegaa/60.0/60.0*PI/180.0;  // convert to rad
  chia = chia/60.0/60.0*PI/180.0; // convert to rad
  S1 = sin(eps0);
  C1 = cos(eps0);
  S2 = sin(-psia);
  C2 = cos(-psia);
  S3 = sin(-omegaa);
  C3 = cos(-omegaa);
  S4 = sin(chia);
  C4 = cos(chia);
  P11 = C4*C2-S2*S4*C3;
  P12 = C4*S2*C1+S4*C3*C2*C1-S1*S4*S3;
  P13 = C4*S2*S1+S4*C3*C2*S1+C1*S4*S3;
  P21 = -S4*C2-S2*C4*C3;
  P22 = -S4*S2*C1+C4*C3*C2*C1-S1*C4*S3;
  P23 = -S4*S2*S1+C4*C3*C2*S1+C1*C4*S3;
  P31 = S2*S3;
  P32 = -S3*C2*C1-S1*C3;
  P33 = -S3*C2*S1+C3*C1;
  rsunmean[0] = P11*rsun2000[0]+P12*rsun2000[1]+P13*rsun2000[2];
  rsunmean[1] = P21*rsun2000[0]+P22*rsun2000[1]+P23*rsun2000[2];
  rsunmean[2] = P31*rsun2000[0]+P32*rsun2000[1]+P33*rsun2000[2];

  // We do not correct for nutation, since this is complex and the effect is only ~10 arcsec

  // Convert mean sun coordinates to earth-fixed
  gastradians = gast*15.0*PI/180.0;
  rsun_ef[0] =  rsunmean[0]*cos(gastradians)+rsunmean[1]*sin(gastradians);
  rsun_ef[1] = -rsunmean[0]*sin(gastradians)+rsunmean[1]*cos(gastradians);
  rsun_ef[2] =  rsunmean[2];

  // Compute and print GHA and declination of sun
  sungha = -atan2(rsun_ef[1],rsun_ef[0]);
  while (sungha<0.0) sungha+=2.0*PI;
  hlength = sqrt(rsun_ef[0]*rsun_ef[0]+rsun_ef[1]*rsun_ef[1]);
  sundecl = atan2(rsun_ef[2],hlength);
  sunghadeg = sungha*180.0/PI;
  sundecldeg = sundecl*180.0/PI;
  //printf("Sun GHA: %03d %05.1lf\n",int(sunghadeg),60.0*(sunghadeg-int(sunghadeg)));
  //printf("Sun Decl: %02d %05.1lf\n",int(sundecldeg),60.0*(sundecldeg-int(sundecldeg)));

  //  Calculate vector from observer to sun in earth-fixed frame
  robs2sun_ef[0] = rsun_ef[0]-robs_ef[0];
  robs2sun_ef[1] = rsun_ef[1]-robs_ef[1];
  robs2sun_ef[2] = rsun_ef[2]-robs_ef[2];

  //  Transform observer to sun vector to local-horizontal coordinates
  ef_lh(robs2sun_ef,robs2sun_lh,lat,lon);
  lh2azel(robs2sun_lh,azim,elev);

  // Return
  return(0);

}


/*---------------------------------------------------------*
 NAME:     void geod_cart(X,lat,lon,height,ae,flat)

 PURPOSE:  Converts geodetic coordinates lat, lon, height 
           above the reference ellipsoid into cartesian 
           coordinates X[1..3], the position vector (xyz).  
           The reference ellipsoid is defined by Earth semi-
           major axis ae and flattening factor flat.  Lat 
           and lon must be in radians and X is returned in 
           the same units as height and ae (which must have 
           compatible units).  The formulation used here is 
           exact and should be accurate to better than 
           millimeter level at any altitude.
           WGS-84 (?) values for ae and flat:
           ae = 6378137.0 meters
           flat = 1.0 / 298.257223563

 AUTHOR:   John Gary Sonntag

 DATE:     July 1994
 *---------------------------------------------------------*/

int geod_cart(double *X,double lat,double lon,double height,double ae,double flat)
{
  double denom,slat,clat;

  /*  Compute X directly  */
  slat = sin(lat);
  clat = cos(lat);
  denom = sqrt(1.0 - flat*(2.0-flat)*slat*slat);
  X[0] = (height + ae/denom)*clat*cos(lon);
  X[1] = (height + ae/denom)*clat*sin(lon);
  X[2] = (height + ae*(1.0-flat)*(1.0-flat)/denom)*slat;
  return(0);

}


/*---------------------------------------------------------*
 NAME:     double get_gast(double et)

 PURPOSE:  Returns Greenwich Apparent Sidereal Time given
           Julian Date in decimal days.  Note this routine 
           expects Julian, not modified Julian, dates.
           Return value is expressed in hours.

 AUTHOR:   John Gary Sonntag

 DATE:     April 2014
 *---------------------------------------------------------*/
double get_gast(double et)
{
  double et0,h,d,d0,t,gmst,omega,l,deltapsi,epsilon,eqeq,gast;

  // Find JD of previous midnight
  et0 = et-int(et);
  if (et0<0.5) et0 = int(et)-0.5;
  else et0 = int(et)+0.5;

  // Compute hours elapsed since previous midnight
  h = (et-et0)*24.0;

  // Compute elapsed since since 2000 Jan 01 12:00:00 UT
  d = et-2451545.0;
  d0 = et0-2451545.0;

  // Compute Greenwich mean sidereal time
  t = d/36525.0;  // centuries elapsed since J2000 epoch
  gmst = 6.697374558+0.06570982441908*d0+1.00273790935*h+0.000026*t*t;

  // Compute equation of the equinoxes
  omega = (125.04-0.052954*d)*PI/180.0; // longitude of Moon ascending node
  l = (280.47+0.98565*d)*PI/180.0; // mean longitude of Sun
  deltapsi = -0.000319*sin(omega)-0.000024*sin(2.0*l); // nutation in longitude
  epsilon = (23.4393-0.0000004*d)*PI/180.0; // obliquity
  eqeq = deltapsi*cos(epsilon);

  // Compute Greenwich apparent sidereal time
  gast = gmst + eqeq;

  // Reduce to 0-24 hours by removing integer days
  gast = (gast/24.0-int(gast/24.0))*24.0;

  // Return
  return(gast);

}


/*---------------------------------------------------------*
 NAME:     void ef_lh(ref,rlh,lat,lon)

 PURPOSE:  Transforms a vector ref[0..2] expressed in the 
           Earth-fixed frame into the local-horizontal
           frame rlh[0..2].  Lat and lon are latitude and
           longitude of the local observer expressed in
           radians.

 AUTHOR:   John Gary Sonntag

 DATE:     August 1994
 *---------------------------------------------------------*/

void ef_lh(double *ref,double *rlh,double lat,double lon)
{
  double clat,slat,clon,slon;

  /*  Compute sine and cosine matrix elements  */
  clat = cos(lat);  slat = sin(lat);
  clon = cos(lon);  slon = sin(lon);

  /*  Transform the vector  */
  rlh[0] = -slat*clon*ref[0] - slat*slon*ref[1] + clat*ref[2];
  rlh[1] =  slon*ref[0] - clon*ref[1];
  rlh[2] =  clat*clon*ref[0] + clat*slon*ref[1] + slat*ref[2];

}


// local-horizontal frame: x-north, y=west, z=up ???
void lh2azel(double *r,double *az,double *el)
{
  double theta,hlength;

  // Compute vector component in local-horizontal plane
  theta = atan2(r[1],r[0]);
  //*az = PI/2.0-theta; // this would be x=east, y=north
  *az = -theta; // this would be x=north,y=west
  while (*az<0.0) *az+=2.0*PI;
  hlength = sqrt(r[0]*r[0]+r[1]*r[1]);

  // Compute elevation
  *el = atan2(r[2],hlength);

}


/*---------------------------------------------------------*
 NAME:     void kal2jul()

 PURPOSE:  Computes time in modified Julian form mjdtime
           given calendar time kaltime.  Not valid before
           1 Jan 1972 (mjd = 41317)

 AUTHOR:   John Gary Sonntag

 DATE:     July 1993

 MODIFIED: January 1994 - added structure arguments and
           prototyping.
           April 1994 - removed prototyping for compatibil-
           ity with stupid Sun compiler
 *---------------------------------------------------------*/

void kal2jul(int year,int month,int day,int hour,int min,double sec,
            double *juldate)
{
  int nyrs,yr,i,dmonth[13];
  long int ndays,mjd;
  double temp,mjs;

  /*  Define number of days per month  */
  dmonth[ 1] = 31;  dmonth[ 2] = 28;  dmonth[ 3] = 31;
  dmonth[ 4] = 30;  dmonth[ 5] = 31;  dmonth[ 6] = 30;
  dmonth[ 7] = 31;  dmonth[ 8] = 31;  dmonth[ 9] = 30;
  dmonth[10] = 31;  dmonth[11] = 30;  dmonth[12] = 31;

  /*  Count days due to past full years, including leap days  */
  nyrs = year - YR0;
  ndays = nyrs*365;
  for (yr=YR0; yr<year; yr++)
  {
    temp = (double)yr/4.0 - (double)(yr/4);
    if (temp == 0.0)
      ndays += 1;
  }

  /*  Add days due to past full months plus leap days  */
  for (i=1; i<month; i++)
    ndays += dmonth[i];
  temp = (double)(year)/4.0 - (double)((year)/4);
  if (temp == 0 && month > 2)
    ndays += 1;

  /*  Add days due to day of month  */
  ndays += day - 1;

  /*  Compute modified Julian day and secs-of-day */
  mjd = MJD0 + ndays;
  mjs = hour*3600.0 + min*60.0 + sec;

  // Compute full Julian date
  *juldate = mjd+mjs/86400.0+2400000.5;

}

