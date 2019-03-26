#ifndef BEAM
#define BEAM

#include <vector>
#include <map>

#include "Geometry.h"


class Beam
{
 public:
  Beam( double elevationAngle );

  ~Beam();

  int scan( const Point& rLocation, 
	    const double azimuth,
	    AzEl& rBeamAzEl,
	    Point*& pIntersectPoint );

 private:
  Beam() { }

  double mElevationAngle;

};

#endif
