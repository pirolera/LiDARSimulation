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
  int computePosition( const double t, std::shared_ptr<Point> pPosition );


  /*
   * Parse vehicle path from the JSON config object
   *
   * @param rConfigJSON JSON config object
   * @return Negative number if error occurs, 0 otherwise
   */
  int parseVehiclePath( const nlohmann::json& rConfigJSON );


  //Path of the vehicle as defined in the config file. Key is timestamp, value is location
  std::map< double, Point > mVehiclePath;


  //Duration of time tick for the simulation
  double mTimeTick;


  //Definition of the LiDAR sensor installed in this vehicle
  LiDARSensor* mpLiDARSensor;


  //Iterators in mVehiclePath representing the waypoint with smaller and larger timestamp 
  //than current simulation time. Used by computePosition function to determine current location
  std::map< double, Point >::const_iterator mPrevIterator;
  std::map< double, Point >::const_iterator mPostIterator;

};

#endif
