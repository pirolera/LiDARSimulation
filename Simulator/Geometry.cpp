#include "Geometry.h"

#include <iostream>
#include <string>
#include <cmath>        // std::abs

#if USE_BOOST
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_franklin.hpp>
#include <boost/geometry/strategies/cartesian/point_in_poly_crossings_multiply.hpp>

typedef boost::geometry::model::point<double, 3, boost::geometry::cs::cartesian> boost_point;
typedef boost::geometry::model::polygon<boost_point> boost_polygon;
#endif

using namespace std;


const shared_ptr<Point> Geometry::crossProduct( const shared_ptr<Point> pP1, 
                                                const shared_ptr<Point> pP2 )
{
  shared_ptr<Point> result( new Point);
  result->x = pP1->y * pP2->z - pP1->z * pP2->y;
  result->y = pP1->z * pP2->x - pP1->x * pP2->z;
  result->z = pP1->x * pP2->y - pP1->y * pP2->x;

  return result;
}


const shared_ptr<Point> Geometry::vectorSubtract( const shared_ptr<Point> pP1, 
                                                  const shared_ptr<Point> pP2 )
{
  shared_ptr<Point> result( new Point);
  result->x = pP2->x - pP1->x;
  result->y = pP2->y - pP1->y;
  result->z = pP2->z - pP1->z;

  return result;
}


const shared_ptr<Point> Geometry::vectorAdd( const shared_ptr<Point> pP1, 
                                             const shared_ptr<Point> pP2 )
{
  shared_ptr<Point> result( new Point);
  result->x = pP2->x + pP1->x;
  result->y = pP2->y + pP1->y;
  result->z = pP2->z + pP1->z;

  return result;
}


const shared_ptr<Point> Geometry::vectorMultiply( const double scalar, const shared_ptr<Point> pPoint )
{
  shared_ptr<Point> result( new Point);
  result->x = scalar * pPoint->x;
  result->y = scalar * pPoint->y;
  result->z = scalar * pPoint->z;

  return result;
}


int Geometry::findIntersection( const shared_ptr<AzEl> pAzEl, 
				const shared_ptr<Polygon> pPolygon, 
				const shared_ptr<Point> pLocation, 
				shared_ptr<Point> rIntersectPoint )
{
  //convert rAzEl into a 3D vector representing the unit vector in the rAzEl direction
  shared_ptr<Point> beam( new Point);
  beam->x = cos(pAzEl->el) * sin(pAzEl->az);
  beam->y = cos(pAzEl->el) * cos(pAzEl->az);
  beam->z = sin(pAzEl->el);


  /*
    1) Compute dot product of normal vector and beam vector
       If result is 0, beam and normal are perpendicular so no intersection between beam and polygon
  */
  double dotProd = dotProduct( beam, pPolygon->normal );
  if ( dotProd == 0.0 )
  {
    //return 0 because intersection point does not exist, beam is parallel to polygon plane
    return 0;
  }


  /*
    2) Compute intersection point between beam and plane
       - Compute beam scalar
       - intersection point is beamOrigin + beam * scalar
  */
  double scalar = dotProduct( pPolygon->normal, 
                              vectorSubtract( pLocation, pPolygon->points[0] ) ) / dotProd;
  if ( scalar < 0 )
  {
    //beam is pointing the other direction of the polygon, so no intersection
    return 0;
  }
  const shared_ptr<Point> intersect = vectorAdd( pLocation, vectorMultiply( scalar, beam ) );


  /*
    3) Check if intersection point is inside the polygon
  */
  if ( isInside( intersect, pPolygon ) )
  {
    rIntersectPoint->x = intersect->x;
    rIntersectPoint->y = intersect->y;
    rIntersectPoint->z = intersect->z;
    return 1;
  }

  //return 0 because point is not inside the polygon
  return 0;
}

#if USE_BOOST
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
#endif


bool Geometry::isInside( const shared_ptr<Point> pPoint, const shared_ptr<Polygon> pPolygon )
{
  double epsilon = 0.000001;
  int i, size = pPolygon->points.size();

  double totalAngle = 0;
  for ( i = 0; i < size; i++ )
  {
    shared_ptr<Point> v1 = pPolygon->points[i];
    shared_ptr<Point> v2 = pPolygon->points[(i+1) % size]; //use the first vertex if v1 is the last vertex
    shared_ptr<Point> vec1 = vectorSubtract( pPoint, v1 );
    shared_ptr<Point> vec2 = vectorSubtract( pPoint, v2 );
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

  if ( ::std::abs( totalAngle - 2 * PI ) < epsilon )
  {
    return true;
  }
  return false;
}
