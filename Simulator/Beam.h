#ifndef BEAM
#define BEAM

#include <vector>
#include <map>

#include "Geometry.h"


/*
 * Beam class to represent each beam in the LiDAR sensor
 */
class Beam
{
 public:

  /*
   * Constructor for the Beam class
   *
   * @param elevationAngle Beam elevation angle in radians from horizon
   */
  Beam( double elevationAngle );

  /*
   * Destructor for the Beam class
   */
  ~Beam();

  /*
   * Executes a full scan from the given position at the given azimuth
   *
   * @param rLocation Location of the vehicle/sensor
   * @param azimuth Azimuth angle of the beam represented in clockwise radians from north
   * @param rBeamAzEl Azimuth/Elevation of this beam for this scan; returned for logging purposes
   * @param pIntersectPoint Returned intersection point of the beam with any structures in the scene
   * @return Negative number if error occurs, 0 otherwise
   */
  int scan( const std::shared_ptr<Point> pLocation, 
	    const double azimuth,
	    std::shared_ptr<AzEl> pBeamAzEl,
	    Point*& pIntersectPoint );

 private:

  //Elevation angle for the beam in radians from horizon
  double mElevationAngle;

};

#endif
