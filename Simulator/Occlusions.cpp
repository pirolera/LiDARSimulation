#include "Occlusions.h"
#include <iostream>
#include <stdexcept>

using namespace std;
using json = nlohmann::json;

std::vector<Polygon> Occlusions::mPolygons;

Occlusions::Occlusions( const json& rConfigJSON )
{
  //Don't do anything if construction is called more than once
  if ( mPolygons.size() > 0 )
  {
    return;
  }

  for ( auto& polEntry : rConfigJSON )
  {
    Polygon pol;
    for ( auto& pointEntry : polEntry )
    {
      Point point;
      point.x = pointEntry.at("x");
      point.y = pointEntry.at("y");
      point.z = pointEntry.at("z");
      pol.points.push_back( point );
    }

    //Compute normal vector of the plane where the polygon resides
    //This is the cross product of two vectors from the edges of the polygon
    pol.normal = Geometry::crossProduct( Geometry::vectorSubtract( pol.points[0], pol.points[1] ), 
					 Geometry::vectorSubtract( pol.points[1], pol.points[2] ) );

    mPolygons.push_back( pol );
  }
}

const vector<Polygon>&  Occlusions::getOcclusions()
{
  return mPolygons;
}

