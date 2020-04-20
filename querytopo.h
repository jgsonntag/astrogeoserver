#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define EGM96PATH "/usr/local/share/geoid/egm96/WW15MGH.DAC\0"
#define GTOPO30PATH "/usr/local/share/dem/gtopo30/\0"
#define BEDMAP2PATH "/usr/local/share/dem/bedmap2/bedmap2_surface.flt\0"
#define GIMP90PATH "/usr/local/share/dem/gimp90/gimp90m.dem\0"
#define C 299792458.0
#define WE 7.292115147e-5
#define MU 3.986005005e14
#define PI (4.0*atan((double)(1.0)))
#define AE 6378137.0
#define FLAT (1.0/298.257223563)
#define RAD2NM (180.0*60.0/PI)
#define RAD2KM (RAD2NM*6076.1*12.0*2.54/100.0/1000.0)


double querytopo(double,double,char *);
double querygeoid(double,double,char *);
int initquerytopo(),closequerytopo();
int initquerygeoid(),closequerygeoid();
void geod2ps(double,double,double,double,double,
             double,double,double *,double *);

