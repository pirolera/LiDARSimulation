#include "Vehicle.h"

#include <iostream>

using namespace std;
using json = nlohmann::json;


Vehicle::Vehicle( const json& rConfigJSON )
{
  //get mTimeTick and mVehiclePath
  const auto timeTick = rConfigJSON.at( "timeTick" );
  mTimeTick = timeTick.get<double>();

  string msg;
  if ( parseVehiclePath( rConfigJSON ) < 0 )
  {
    msg = "Vehicle constructor could not parse vehicle path";
    throw std::runtime_error( msg );
  }

  if ( mVehiclePath.size() < 2 )
  {
    msg = "Vehicle path has less than 2 waypoints";
    throw std::runtime_error( msg );
  }
  mPrevIterator = mVehiclePath.begin();
  mPostIterator = next(mPrevIterator);

  double startTime = mVehiclePath.begin()->first;
  mpLiDARSensor = new LiDARSensor( rConfigJSON.at( "sensor" ), startTime );
}


Vehicle::~Vehicle()
{
  delete mpLiDARSensor;
}


int Vehicle::runSimulation()
{
  double startTime = mVehiclePath.begin()->first;
  double endTime = mVehiclePath.rbegin()->first;

  Point position;
  for ( double t = startTime; t < endTime; t += mTimeTick )
  {
    if ( computePosition( t, position ) < 0 )
    {
      cout << "ERROR, could not compute position" << endl;
      return -1;
    }
    cout << "----cp---" << t << " " << position.x << " " << position.y << " " << position.z << endl;

    if ( mpLiDARSensor->scan( position, t ) < 0 )
    {
      cout << "ERROR, could not scan" << endl;
      return -1;
    }
  }

  //run for the last timestamp in the path
  position.x = mVehiclePath.rbegin()->second.x;
  position.y = mVehiclePath.rbegin()->second.y;
  position.z = mVehiclePath.rbegin()->second.z;
  cout << "----last---" << endTime << " " << position.x << " " << position.y << " " << position.z << endl;

  if ( mpLiDARSensor->scan( position, endTime ) < 0 )
  {
    cout << "ERROR, could not scan" << endl;
    return -1;
  }

  return 0;
}


int Vehicle::computePosition( const double t, Point& rPosition )
{
  if ( t >= mPostIterator->first )
  {
    if ( mPostIterator == mVehiclePath.end() )
    {
      cout << "ERROR, the computePosition function is called incorrectly" << endl;
      return -1;
    }
    mPrevIterator = mPostIterator;
    mPostIterator = next(mPostIterator);
    cout << "time window is now " << mPrevIterator->first << " " << mPostIterator->first << endl;
  }

  double timeRatio = ( t - mPrevIterator->first ) / ( mPostIterator->first - mPrevIterator->first );

  double deltaX = mPostIterator->second.x - mPrevIterator->second.x;
  double deltaY = mPostIterator->second.y - mPrevIterator->second.y;
  double deltaZ = mPostIterator->second.z - mPrevIterator->second.z;

  rPosition.x = mPrevIterator->second.x + deltaX * timeRatio;
  rPosition.y = mPrevIterator->second.y + deltaY * timeRatio;
  rPosition.z = mPrevIterator->second.z + deltaZ * timeRatio;

  return 0;
}


int Vehicle::parseVehiclePath( const json& rConfigJSON )
{
  const auto vehiclePath = rConfigJSON.at( "vehiclePath" );

  //store last time value to verify entries are in increasing time order
  double lastTime = -1.0, t;
  for ( auto& entry : vehiclePath )
  {
    t = entry.at( "t" );
    if ( t < lastTime )
    {
      cerr << "Entry " << entry << " is not in increasing time order" << endl;
      return -1;
    }

    lastTime = t;
    Point point;
    point.x = entry.at( "x" );
    point.y = entry.at( "y" );
    point.z = entry.at( "z" );
    mVehiclePath[t] = point;
  }

  return 0;
}

