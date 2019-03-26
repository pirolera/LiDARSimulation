#ifndef GEOMETRY
#define GEOMETRY

#include <vector>


struct Point
{
  double x;
  double y;
  double z;
};

struct AzEl
{
  double az;
  double el;
};

struct Polygon
{
  std::vector<Point> points;
  Point normal;
};


class Geometry
{
 public:
  static const Point crossProduct( const Point& rP1, const Point& rP2 );
  
  static double dotProduct(const Point& rP1, const Point& rP2);

  static Point vectorSubtract( const Point& rP1, const Point& rP2 );

  static Point vectorAdd( const Point& rP1, const Point& rP2 );

  static Point vectorMultiply( const double scalar, const Point& rP );

  static double vectorLength( const Point& rP1 );

  static int findIntersection( const AzEl& rAzEl, 
			       const Polygon& rPolygon,
			       const Point& rLocation,
			       Point& rIntersectPoint );

  static bool isInsideBoost( const Point& rPoint, const Polygon& rPolygon );

  static bool isInside( const Point& rPoint, const Polygon& rPolygon );


  static constexpr double PI = 3.14159265;
};

#endif
