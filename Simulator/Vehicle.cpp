#include "Vehicle.h"

#include <iostream>

using namespace std;
using json = nlohmann::json;


Vehicle::Vehicle( const json& rConfigJSON ) :
  mLargerTimestampIndex( 0 )
{
  //get mTimeTick and mVehiclePath
  const auto timeTick = rConfigJSON.at( "timeTick" );
  mTimeTick = timeTick.get<double>();

  if ( parseVehiclePath( rConfigJSON ) < 0 )
  {
    string msg( "Vehicle constructor could not parse vehicle path" );
    cerr << msg << endl;
    throw std::runtime_error( msg );
  }
  else
  {
    double startTime = mVehiclePath[0][0];
    mpLiDARSensor = new LiDARSensor( rConfigJSON.at( "sensor" ), startTime );
  }
}


Vehicle::~Vehicle()
{
  delete mpLiDARSensor;
}


int Vehicle::runSimulation()
{
  double startTime = mVehiclePath[0][0];
  double endTime = mVehiclePath.back()[0];

  Point position;
  for ( double t = startTime; t < endTime; t += mTimeTick )
  {
    if ( computePosition( t, position ) < 0 )
    {
      cout << "ERROR, could not compute position" << endl;
      return -1;
    }
    cout << "-------" << t << " " << position.x << " " << position.y << " " << position.z << endl;

    if ( mpLiDARSensor->scan( position, t ) < 0 )
    {
      cout << "ERROR, could not scan" << endl;
      return -1;
    }
  }

  //run for the last timestamp
  position.x = mVehiclePath.back()[1];
  position.y = mVehiclePath.back()[2];
  position.z = mVehiclePath.back()[3];
  cout << "-------" << endTime << " " << position.x << " " << position.y << " " << position.z << endl;

  if ( mpLiDARSensor->scan( position, endTime ) < 0 )
  {
    cout << "ERROR, could not scan" << endl;
    return -1;
  }

  return 0;
}


int Vehicle::computePosition( const double t, Point& rPosition )
{
  if ( t >= mVehiclePath[mLargerTimestampIndex][0] )
  {
    if ( mLargerTimestampIndex == mVehiclePath.size() - 1 )
    {
      cout << "ERROR, the computePosition function is called incorrectly" << endl;
      return -1;
    }
    mLargerTimestampIndex++;
    cout << "index is now " << mLargerTimestampIndex << endl;
  }

  double timeRatio = ( t - mVehiclePath[mLargerTimestampIndex - 1][0] ) / ( mVehiclePath[mLargerTimestampIndex][0] - mVehiclePath[mLargerTimestampIndex - 1][0] );

  double deltaX = mVehiclePath[mLargerTimestampIndex][1] - mVehiclePath[mLargerTimestampIndex - 1][1];
  double deltaY = mVehiclePath[mLargerTimestampIndex][2] - mVehiclePath[mLargerTimestampIndex - 1][2];
  double deltaZ = mVehiclePath[mLargerTimestampIndex][3] - mVehiclePath[mLargerTimestampIndex - 1][3];

  double newX = mVehiclePath[mLargerTimestampIndex - 1][1] + deltaX * timeRatio;
  double newY = mVehiclePath[mLargerTimestampIndex - 1][2] + deltaY * timeRatio;
  double newZ = mVehiclePath[mLargerTimestampIndex - 1][3] + deltaZ * timeRatio;

  rPosition.x = newX;
  rPosition.y = newY;
  rPosition.z = newZ;

  return 0;
}


int Vehicle::parseVehiclePath( const json& rConfigJSON )
{
  const auto vehiclePath = rConfigJSON.at( "vehiclePath" );

  //ToDo: verify entries are in increasing time order
  for ( auto& entry : vehiclePath )
  {
    vector<double> point{ entry.at( "t" ), entry.at( "x" ), entry.at( "y" ), entry.at( "z" ) };
    mVehiclePath.push_back( point );
  }

  return 0;
}

