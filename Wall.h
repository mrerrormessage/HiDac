#include "CrowdObject.h"
#include "constants.h"
#include <jsoncpp/value.h>




class Wall : public CrowdObject {
 private: 
  //wall start-point
  v2f start; 
  //wall end-point
  v2f end; 
 //norm is found in CrowdObject

 public:
  Wall();
  //Wall constructor taking the start and end vectors
  Wall( v2f s, v2f e );
  //each wall will be represented as two back-to-back walls. 

};

Wall::Wall * twoWalls( Json::Value w);
