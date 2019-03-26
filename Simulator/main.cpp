#include <iostream>
#include <fstream>
#include <cstdlib>
#include <nlohmann/json.hpp>

#include "Vehicle.h"
#include "Occlusions.h"


using namespace std;
using json = nlohmann::json;



int main (int argc, char** argv)
{
  if ( argc != 2 )
  {
    cout << "ERROR, need to pass JSON config file" << endl;
    return 0;
  }

  string configFile( argv[1] );
  json configJSON;
  ifstream i( configFile );
  i >> configJSON;


  Vehicle vehicle( configJSON );
  Occlusions occ( configJSON.at( "structures" ) );

  if ( vehicle.runSimulation() < 0 )
  {
    cout << "ERROR, Simulation Failed" << endl;
  }
  else
  {
    cout << "SUCCESS" << endl;
  }

  return 0;
}
