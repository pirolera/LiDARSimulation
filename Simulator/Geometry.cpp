#include "Geometry.h"

#include <iostream>
#include <string>
#include <math.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_franklin.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_crossings_multiply.hpp>


using namespace std;
typedef boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian> boost_point;
typedef boost::geometry::model::polygon<boost_point> boost_polygon;


const Point Geometry::crossProduct( const Point& rP1, const Point& rP2 )
{
  Point result;
  result.x = rP1.y * rP2.z - rP1.z * rP2.y;
  result.y = rP1.z * rP2.x - rP1.x * rP2.z;
  result.z = rP1.x * rP2.y - rP1.y * rP2.x;

  return result;
}


double Geometry::dotProduct(const Point& rP1, const Point& rP2)
{
  return rP1.x * rP2.x + rP1.y * rP2.y + rP1.z * rP2.z;
}


Point Geometry::vectorSubtract( const Point& rP1, const Point& rP2 )
{
  Point result;
  result.x = rP2.x - rP1.x;
  result.y = rP2.y - rP1.y;
  result.z = rP2.z - rP1.z;

  return result;
}


Point Geometry::vectorAdd( const Point& rP1, const Point& rP2 )
{
  Point result;
  result.x = rP2.x + rP1.x;
  result.y = rP2.y + rP1.y;
  result.z = rP2.z + rP1.z;

  return result;
}


Point Geometry::vectorMultiply( const double scalar, const Point& rP )
{
  Point result;
  result.x = scalar * rP.x;
  result.y = scalar * rP.y;
  result.z = scalar * rP.z;

  return result;
}


double Geometry::vectorLength( const Point& rP )
{
  return sqrt( rP.x * rP.x + rP.y * rP.y + rP.z * rP.z );
}


/*
 * http://geomalgorithms.com/a05-_intersect-1.html
 */
int Geometry::findIntersection( const AzEl& rAzEl, 
				const Polygon& rPolygon, 
				const Point& rLocation, 
				Point& rIntersectPoint )
{
  //convert rAzEl into a 3D vector representing the unit vector in the rAzEl direction
  Point beam;
  beam.x = cos(rAzEl.el) * sin(rAzEl.az);
  beam.y = cos(rAzEl.el) * cos(rAzEl.az);
  beam.z = sin(rAzEl.el);
  //cout << "\t\tBeam = " << beam.x << " " << beam.y << " " << beam.z << endl;


  /*
    1) Compute dot product of normal vector and beam vector
       If result is 0, beam and normal are perpendicular so no intersection between beam and polygon
  */
  double dotProd = dotProduct( beam, rPolygon.normal );
  if ( dotProd == 0.0 )
  {
    //return 0 because intersection point does not exist
    return 0;
  }
  //cout << "\t\tdot product = " << dotProd << endl;


  /*
    2) Compute intersection point between beam and plane
       - Compute beam scalar
       - intersection point is beamOrigin + beam * scalar
  */
  double scalar = dotProduct( rPolygon.normal, vectorSubtract( rLocation, rPolygon.points[0] ) ) / dotProd;
  //cout << "\t\tscalar = " << scalar << endl;
  if ( scalar < 0 )
  {
    //beam is pointing the other direction of the polygon, so no intersection
    return 0;
  }
  Point intersect = vectorAdd( rLocation, vectorMultiply( scalar, beam ) );
  cout << "\t\tPlaneIntersect " << intersect.x << " " << intersect.y << " " << intersect.z << endl;


  /*
    3) Check if intersection point is inside the polygon
  */
  if ( isInside( intersect, rPolygon ) )
  {
    rIntersectPoint.x = intersect.x;
    rIntersectPoint.y = intersect.y;
    rIntersectPoint.z = intersect.z;
    return 1;
  }

  //return 0 because point is not inside the polygon
  return 0;
}

template <typename PointType>
void list_coordinates(PointType const& p)
{
  using boost::geometry::get;
  cout << get<0>(p) << " " << get<1>(p) << " " << get<2>(p) << endl;
}


bool Geometry::isInsideBoost( const Point& rPoint, const Polygon& rPolygon )
{
  boost_polygon poly;

  //create polygon wkt
  string poly_wkt( "POLYGON((" );
  for ( Point point : rPolygon.points )
    poly_wkt += ( to_string( point.x ) + " " + 
                  to_string( point.y ) + " " + 
                  to_string( point.z ) + "," );
  //add first point again to close the polygon
  poly_wkt += ( to_string( rPolygon.points[0].x ) + " " + 
                to_string( rPolygon.points[0].y ) + " " + 
                to_string( rPolygon.points[0].z ) );
  poly_wkt += "))";

  boost::geometry::read_wkt( poly_wkt, poly );

  boost_point p;
  boost::geometry::assign_values(p, rPoint.x, rPoint.y, rPoint.z );

  //print polygon
  //boost::geometry::for_each_point(poly, list_coordinates<boost_point>);

  typedef boost::geometry::strategy::within::franklin<boost_point, boost_point, void> fran;
  fran franStrategy;
  //return boost::geometry::within( p, poly, franStrategy );

  typedef boost::geometry::strategy::within::crossings_multiply<boost_point, boost_point, void> cm;
  cm cmStrategy;
  //return boost::geometry::within( p, poly, cmStrategy ); //2D works

  return boost::geometry::covered_by(p, poly); //2D works

  //return boost::geometry::intersects(p, poly); //2D works
}


bool Geometry::isInside( const Point& rPoint, const Polygon& rPolygon )
{
  double epsilon = 0.000001;
  int i, size = rPolygon.points.size();

  double totalAngle = 0;
  for ( i = 0; i < size; i++ )
  {
    Point v1 = rPolygon.points[i];
    Point v2 = rPolygon.points[(i+1) % size]; //use the first vertex if v1 is the last vertex
    Point vec1 = vectorSubtract( rPoint, v1 );
    Point vec2 = vectorSubtract( rPoint, v2 );
    double len1 = vectorLength( vec1 );
    double len2 = vectorLength( vec2 );

    if ( len1 * len2 < epsilon )
    {
      //rPoint is almost at one of the vertices, return true
      return true;
    }

    double cosAngle = dotProduct( vec1, vec2 ) / ( len1 * len2 );
    if ( cosAngle > 1 )
      cosAngle = 1;
    if ( cosAngle < -1 )
      cosAngle = -1;

    double angle = acos( cosAngle );
    totalAngle += angle;
  }

  if ( abs( totalAngle - 2 * PI ) < epsilon )
  {
    return true;
  }
  return false;
}
