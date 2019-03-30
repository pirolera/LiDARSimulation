#ifndef VEHICLE
#define VEHICLE

#include <string>
#include <vector>
#include <nlohmann/json.hpp>


#include "LiDARSensor.h"
#include "Geometry.h"

/*
 * Vehicle class to represent the vehicle where the LiDAR sensor is installed
 */
class Vehicle
{
 public:

  /*
   * Constructor for Vehicle class
   *
   * @param rConfigJSON JSON configuration object
   */
  Vehicle( const nlohmann::json& rConfigJSON );


  /*
   * Destructor for Vehicle class
   */
  ~Vehicle();


  /*
   * Runs the simulation
   *
   * @return Negative number if error occurs, 0 otherwise
   */
  int runSimulation();


 private:

  /*
   * Computes position of the vehicle at a given time by interpolating between waypoints of the path
   *
   * @param t The current simulation time
   * @param rPosition The return object with the position of the vehicle at time t
   * @return Negative number if error occurs, 0 otherwise
   */
  int computePosition( const double t, Point& rPosition );


  /*
   * Parse vehicle path from the JSON config object
   *
   * @param rConfigJSON JSON config object
   * @return Negative number if error occurs, 0 otherwise
   */
  int parseVehiclePath( const nlohmann::json& rConfigJSON );


  //Path of the vehicle as defined in the config file
  //ToDo: change vector to map<t, Point>
  std::vector< std::vector< double > > mVehiclePath;


  //Duration of time tick for the simulation
  double mTimeTick;


  //Definition of the LiDAR sensor installed in this vehicle
  LiDARSensor* mpLiDARSensor;


  //Index in mVehiclePath representing the waypoint with larger timestamp than current simulation time
  //Used by computePosition function to determine waypoint before and after the current time
  int mLargerTimestampIndex;

};

#endif
