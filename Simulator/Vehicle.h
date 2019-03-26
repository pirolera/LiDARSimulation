#ifndef VEHICLE
#define VEHICLE

#include <string>
#include <vector>
#include <nlohmann/json.hpp>


#include "LiDARSensor.h"
#include "Geometry.h"


class Vehicle
{
 public:
  Vehicle( const nlohmann::json& rConfigJSON );

  ~Vehicle();

  int runSimulation();

 private:
  Vehicle() { }

  int computePosition( const double t, Point& rPosition );
  
  int parseVehiclePath( const nlohmann::json& rConfigJSON );

  //ToDo: change vector to map<t, Point>
  std::vector< std::vector< double > > mVehiclePath;

  double mTimeTick;

  LiDARSensor* mpLiDARSensor;

  int mLargerTimestampIndex;

};

#endif
