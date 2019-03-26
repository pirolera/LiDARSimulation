#ifndef LIDARSENSOR
#define LIDARSENSOR

#include "Beam.h"
#include <nlohmann/json.hpp>

#include "Geometry.h"
#include "FileWriter.h"

class LiDARSensor
{
 public:

  LiDARSensor( const nlohmann::json& rConfigJSON, const double startTime );

  ~LiDARSensor();

  int scan( const Point& rLocation, 
	    const double time );

 private:

  //ToDo: remove
  /*int computeAzElForOcclusionCorners( const std::vector<double>& rLocation,
    std::map<int, std::vector< std::vector< double > > >& rAzElPairs );*/

  std::vector< Beam* > mBeams;

  double mSpinningSpeed;

  double mStartTime;

  FileWriter mFileWriter;
};

#endif
