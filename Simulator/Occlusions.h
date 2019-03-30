#ifndef OCCLUSIONS
#define OCCLUSIONS

#include <vector>
#include <nlohmann/json.hpp>

#include "Geometry.h"

/*
 * Occlusions class to store all the structures defined in the config file
 */
class Occlusions
{
 public:

  /*
   * Construction for the Occlusions class; parses JSON object and converts to a list of Polygons
   *
   * @param rConfigJSON JSON configuration object
   */
  Occlusions( const nlohmann::json& rConfigJSON );

  /*
   * Getter method to get the list of Polygons representing structures in the config file
   *
   * @return Vector of Polygon objects representing structures in the config file
   */
  static const std::vector<Polygon>&  getOcclusions();

 private:

  //Vector of Polygon objects representing structures in the config file
  static std::vector<Polygon> mPolygons;
};

#endif
