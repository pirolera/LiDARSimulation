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

int Beam::scan( const shared_ptr<Point> pLocation, 
		const double azimuth,
		shared_ptr<AzEl> rBeamAzEl,
		Point*& pIntersectPoint )
{
  cout << "\tScanning " << azimuth * 180 / Geometry::PI 
       << " " << mElevationAngle  * 180 / Geometry::PI << endl;

  rBeamAzEl->az = azimuth;
  rBeamAzEl->el = mElevationAngle;
  
  shared_ptr<Point> closestIntersectPoint;
  double distance = -1.0;
  const vector< shared_ptr<Polygon> >& polygons = Occlusions::getOcclusions();
  for ( const shared_ptr<Polygon> polygon : polygons )
  {
    shared_ptr<Point> intersectPoint( new Point );
    int ret = Geometry::findIntersection( rBeamAzEl, polygon, pLocation, intersectPoint );
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

    cout << "\t\t\t------PolygonIntersection at " << intersectPoint->x << " " 
         << intersectPoint->y << " " << intersectPoint->z << endl;

    //keep the closest intersection point if there are many
    double newDistance = Geometry::vectorLength( Geometry::vectorSubtract( pLocation, intersectPoint ) );
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
    pIntersectPoint->x = closestIntersectPoint->x;
    pIntersectPoint->y = closestIntersectPoint->y;
    pIntersectPoint->z = closestIntersectPoint->z;
  }

  return 0;
}
