#ifndef OCCLUSIONS
#define OCCLUSIONS

#include <vector>
#include <nlohmann/json.hpp>

#include "Geometry.h"


class Occlusions
{
 public:
  Occlusions( const nlohmann::json& rConfigJSON );

  static const std::vector<Polygon>&  getOcclusions();

 private:
  Occlusions() { }

  static std::vector<Polygon> mPolygons;
};

#endif
