#include "Beam.h"

#include <iostream>

#include "Occlusions.h"


using namespace std;


Beam::Beam( double elevationAngle ):
  mElevationAngle( elevationAngle )
{

}

Beam::~Beam()
{

}

int Beam::scan( const Point& rLocation, 
		const double azimuth,
		AzEl& rBeamAzEl,
		Point*& pIntersectPoint )
{
  cout << "\tScanning " << azimuth * 180 / Geometry::PI 
       << " " << mElevationAngle  * 180 / Geometry::PI << endl;

  rBeamAzEl.az = azimuth;
  rBeamAzEl.el = mElevationAngle;
  
  Point closestIntersectPoint;
  double distance = -1.0;
  const vector<Polygon>& polygons = Occlusions::getOcclusions();
  for ( const Polygon polygon : polygons )
  {
    Point intersectPoint;
    int ret = Geometry::findIntersection( rBeamAzEl, polygon, rLocation, intersectPoint );
    if ( ret < 0 )
    {
      cout << "WARNING, findIntersection did not work" << endl;
      continue;
    }

    if ( ret == 0 )
    {
      //Beam does not intersect
      continue;
    }

    cout << "\t\t\t------PolygonIntersection at " << intersectPoint.x << " " << intersectPoint.y << " " << intersectPoint.z << endl;

    //keep the closest intersection point if there are many
    double newDistance = Geometry::vectorLength( Geometry::vectorSubtract( rLocation, intersectPoint ) );
    if ( distance < 0 || distance > newDistance )
    {
      closestIntersectPoint = intersectPoint;
      distance = newDistance;
    }
  }

  //return intersection point, if any
  if ( distance >= 0 )
  {
    pIntersectPoint = new Point;
    pIntersectPoint->x = closestIntersectPoint.x;
    pIntersectPoint->y = closestIntersectPoint.y;
    pIntersectPoint->z = closestIntersectPoint.z;
  }

  return 0;
}
