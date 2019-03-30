#ifndef GEOMETRY
#define GEOMETRY

#include <vector>
#include <math.h>


//do not use Boost to determine if point is inside a polygon
#define USE_BOOST false


//Struct to represent a point or a vector in Cartesian space
struct Point
{
  double x;
  double y;
  double z;
};

//Struct with azimuth (clockwise angle in radians from north) and elevation (angle in radians from horizon)
struct AzEl
{
  double az;
  double el;
};

//Struct to represent a polygon; contains list of vertices and a normal vector
struct Polygon
{
  std::vector<Point> points;
  Point normal;
};


/*
 * Geometry class with static methods for point/vector/polygon calculations
 */
class Geometry
{
 public:

  /*
   * Compute cross product of two vectors
   *
   * @param rP1 First vector
   * @param rP2 Second vector
   * @return Vector representing cross product of the two input vectors
   */
  static const Point crossProduct( const Point& rP1, const Point& rP2 );
  

  /*
   * Compute dot product of two vectors
   *
   * @param rP1 First vector
   * @param rP2 Second vector
   * @return Double representing dot product value of the two input vectors
   */
  static double dotProduct(const Point& rP1, const Point& rP2) { return rP1.x * rP2.x + rP1.y * rP2.y + rP1.z * rP2.z; }


  /*
   * Compute subtraction of two vectors/points (rP2 - rP1)
   *
   * @param rP1 First vector/point
   * @param rP2 Second vector/point
   * @return Vector/point representing rP2 - rP1
   */
  static Point vectorSubtract( const Point& rP1, const Point& rP2 );


  /*
   * Compute addition of two vectors/points
   *
   * @param rP1 First vector/point
   * @param rP2 Second vector/point
   * @return Vector/point representing rP1 + rP2
   */
  static Point vectorAdd( const Point& rP1, const Point& rP2 );


  /*
   * Compute multiplication of vector/point with scalar
   *
   * @param scalar Scalar value to multiply the vector/point with
   * @param rP Vector/point
   * @return Vector/point representing scalar * rP
   */
  static Point vectorMultiply( const double scalar, const Point& rP );


  /*
   * Compute length of a vector
   *
   * @param rP Input vector for which to compute the length
   * @return Length of input vector
   */
  static double vectorLength( const Point& rP ) { return sqrt( rP.x * rP.x + rP.y * rP.y + rP.z * rP.z ); }


  /*
   * Find intersection of beam from a given location and a polygon
   *
   * @param rAzEl Definition of the beam
   * @param rPolygon Polygon to check for intersection
   * @param rLocation Location of the sensor
   * @rIntersectPoint Return intersection point, if found
   * @return Negative number if error occurs, 0 is code runs successful but there is no intersection,
   *         positive number if intersection found and rIntersectPoint is populated
   */
  static int findIntersection( const AzEl& rAzEl, 
			       const Polygon& rPolygon,
			       const Point& rLocation,
			       Point& rIntersectPoint );

#if USE_BOOST

  /*
   * Check if 3D point is inside a polygon (this function does not work in 3D)
   *
   * @param rPoint Point definition
   * @param rPolygon Polygon definition
   * @return True if point is inside polygon, false otherwise
   */
  static bool isInsideBoost( const Point& rPoint, const Polygon& rPolygon );
#endif

  /*
   * Check if 3D point is inside a polygon
   *
   * @param rPoint Point definition
   * @param rPolygon Polygon definition
   * @return True if point is inside polygon, false otherwise
   */
  static bool isInside( const Point& rPoint, const Polygon& rPolygon );


  // Definition of pi value
  static constexpr double PI = 3.14159265;
};

#endif
