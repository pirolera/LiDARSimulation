#include "LiDARSensor.h"

#include <iostream>
#include <cmath>
#include <math.h> //for atan2
#include <map>

#include "Occlusions.h"

using namespace std;
using json = nlohmann::json;


LiDARSensor::LiDARSensor( const json& rConfigJSON, const double startTime ) :
  mStartTime( startTime ),
  mFileWriter( "output.json" ) //ToDo: make the writer parameterized instead of hard-coded
{
  //get mSpinningSpeed, numBeams, elevationExtent
  double elevationExtent = rConfigJSON.at( "elevationExtent" ).get<double>();
  mSpinningSpeed = rConfigJSON.at( "spinningSpeed" ).get<double>();
  int numBeams = rConfigJSON.at( "numBeams" ).get<int>();

  //construct the vector of beams
  double beamExtent = elevationExtent / (double)numBeams;
  double elevation;
  for ( int beam = 0; beam < numBeams; beam++ )
  {
    elevation = -elevationExtent / 2 + beamExtent * beam + beamExtent / 2;
    Beam* newBeam = new Beam( elevation );
    mBeams.push_back( newBeam );
  }
}


LiDARSensor::~LiDARSensor()
{
  vector<Beam*>::iterator iter = mBeams.begin();
  vector<Beam*>::iterator end = mBeams.end();
  for( ; iter != end; iter++ )
  {
    delete *iter;
  }
}


int LiDARSensor::scan( const Point& rLocation, 
		       const double time )
{
  //first find azimuth
  double azimuth = fmod( mSpinningSpeed * ( time - mStartTime ), 2 * Geometry::PI );
  
  vector<Beam*>::iterator iter = mBeams.begin();
  vector<Beam*>::iterator end = mBeams.end();
  AzEl beamAzEl;
  Point* intersectPoint = 0x0;

  json output, position;
  output["t"] = time;
  position["x"] = rLocation.x;
  position["y"] = rLocation.y;
  position["z"] = rLocation.z;
  output["position"] = position;

  json beams = json::array();
  for( ; iter != end; iter++ )
  {
    json beam;
    if ( (*iter)->scan( rLocation, azimuth, beamAzEl, intersectPoint ) < 0 )
    {
      cout << "WARNING, beam did not scan" << endl;
    }

    beam["az"] = beamAzEl.az;
    beam["el"] = beamAzEl.el;

    if ( intersectPoint != 0x0 )
    {
      json point;
      point["x"] = intersectPoint->x;
      point["y"] = intersectPoint->y;
      point["z"] = intersectPoint->z;
      beam["point"] = point;

      //clean up memory
      delete intersectPoint;
      intersectPoint = 0x0;
    }

    beams.push_back( beam );
  }
  output["beams"] = beams;

  mFileWriter.write( output.dump() );
  return 0;
}
