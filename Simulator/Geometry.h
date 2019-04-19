#ifndef GEOMETRY
#define GEOMETRY

#include <vector>
#include <math.h>
#include <memory>

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
  std::vector< std::shared_ptr<Point> > points;
  std::shared_ptr<Point> normal;
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
   * @param pP1 First vector
   * @param pP2 Second vector
   * @return Vector representing cross product of the two input vectors
   */
  static const std::shared_ptr<Point> crossProduct( const std::shared_ptr<Point> pP1, 
                                                    const std::shared_ptr<Point> pP2 );
  

  /*
   * Compute dot product of two vectors
   *
   * @param pP1 First vector
   * @param pP2 Second vector
   * @return Double representing dot product value of the two input vectors
   */
  static double dotProduct(const std::shared_ptr<Point> pP1, const std::shared_ptr<Point> pP2) 
  { return pP1->x * pP2->x + pP1->y * pP2->y + pP1->z * pP2->z; }


  /*
   * Compute subtraction of two vectors/points (rP2 - rP1)
   *
   * @param pP1 First vector/point
   * @param pP2 Second vector/point
   * @return Vector/point representing pP2 - pP1
   */
  static const std::shared_ptr<Point> vectorSubtract( const std::shared_ptr<Point> pP1, 
                                                      const std::shared_ptr<Point> pP2 );


  /*
   * Compute addition of two vectors/points
   *
   * @param pP1 First vector/point
   * @param pP2 Second vector/point
   * @return Vector/point representing pP1 + pP2
   */
  static const std::shared_ptr<Point> vectorAdd( const std::shared_ptr<Point> pP1, 
                                                 const std::shared_ptr<Point> pP2 );


  /*
   * Compute multiplication of vector/point with scalar
   *
   * @param scalar Scalar value to multiply the vector/point with
   * @param pPoint Vector/point
   * @return Vector/point representing scalar * pPoint
   */
  static const std::shared_ptr<Point> vectorMultiply( const double scalar, 
                                                      const std::shared_ptr<Point> pPoint );


  /*
   * Compute length of a vector
   *
   * @param pPoint Input vector for which to compute the length
   * @return Length of input vector
   */
  static double vectorLength( const std::shared_ptr<Point> pPoint ) 
  { return sqrt( pPoint->x * pPoint->x + pPoint->y * pPoint->y + pPoint->z * pPoint->z ); }


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
  static int findIntersection( const std::shared_ptr<AzEl> pAzEl, 
			       const std::shared_ptr<Polygon> pPolygon,
			       const std::shared_ptr<Point> pLocation,
			       std::shared_ptr<Point> pIntersectPoint );

#if USE_BOOST

  /*
   * Check if 3D point is inside a polygon (this function does not work in 3D)
   *
   * @param pPoint Point definition
   * @param pPolygon Polygon definition
   * @return True if point is inside polygon, false otherwise
   */
  static bool isInsideBoost( const std::shared_ptr<Point> pPoint, 
                             const std::shared_ptr<Polygon> pPolygon );
#endif

  /*
   * Check if 3D point is inside a polygon
   *
   * @param pPoint Point definition
   * @param pPolygon Polygon definition
   * @return True if point is inside polygon, false otherwise
   */
  static bool isInside( const std::shared_ptr<Point> pPoint, const std::shared_ptr<Polygon> pPolygon );


  // Definition of pi value
  static constexpr double PI = 3.14159265;
};

#endif
