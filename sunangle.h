#include <math.h>

#define WEARTH 7.292115e-5 //earth rotation speed in rad/sec
#define EPHFILE "/usr/local/share/planetaryephemeris/lnxp1800p2200.423"
#define AU2METERS 149597871000.0
#define PI (4.0*atan((double)(1.0)))
#define DEG2RAD (PI/180.0)
#define AE 6378137.0
#define FLAT (1.0/298.257223563)
#define YR0  1972
#define MJD0 41317

int getsunangle(double lat,double lon,double ht,
            int year,int month,int day,int hour,int min,double sec,
            double *azim,double *elev);
int geod_cart(double *X,double lat,double lon,double height,
              double ae,double flat);
double get_gast(double et);
void ef_lh(double *ref,double *rlh,double lat,double lon);
void lh2azel(double *r,double *az,double *el);
void kal2jul(int yr,int month,int day,int hr,int min,double sec,
            double *juldate);
