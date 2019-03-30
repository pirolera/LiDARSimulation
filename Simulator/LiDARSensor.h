#ifndef LIDARSENSOR
#define LIDARSENSOR

#include "Beam.h"
#include <nlohmann/json.hpp>

#include "Geometry.h"
#include "JSONFileWriter.h"

/*
 * LiDARSensor class to represent the LiDAR sensor
 */
class LiDARSensor
{
 public:

  /*
   * Constructor for the LiDARSensor class
   *
   * @param rConfigJSON JSON object with the "sensor" part of the configuration
   * @param startTime Start time of the simulation as defined in vehicle path
   */
  LiDARSensor( const nlohmann::json& rConfigJSON, const double startTime );


  /*
   * Destructor for the LiDARSensor class
   */
  ~LiDARSensor();


  /*
   * Executes a full scan from the given position at the given time
   *
   * @param rLocation Current location
   * @param time Current time
   * @return Negative number if error occurs, 0 otherwise
   */
  int scan( const Point& rLocation, 
	    const double time );

 private:

  //Vector of sensor beams 
  std::vector< Beam* > mBeams;


  //Spinning speed of the sensor
  double mSpinningSpeed;


  //Simulation start time
  double mStartTime;


  //JSONFileWriter where to write output JSON for each scan
  JSONFileWriter mJSONFileWriter;
};

#endif
