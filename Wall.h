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

  //returns whether the object is visible within the vision rectangle presented
  bool isVisible( v2f pos, v2f dir, float vislength, float viswidth);

  //returns the norm of the object in the return parameter
  void getNorm( v2f ret ); 
  
  //gets the distance from the position argument to the object
  float getDistance( v2f pos );

  //gets the direction vector from the calling position to the object
  void getDirection( v2f pos, v2f res);

  //gets the type of object
  int getType();

  //returns a zero-vector for all non-agent CrowdObjects
  void getVelocity( v2f ret );


};

Wall::Wall * twoWalls( Json::Value w);
