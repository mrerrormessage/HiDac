#ifndef _CROWD_H_
#define _CROWD_H_

#include "constants.h"
#include <jsoncpp/value.h>

enum objtype {AGENT, 
	      WALL,
	      OBSTACLE,
	      FALLEN_AGENT,
	      ATTRACTOR};

class CrowdObject { 
 private: 
  v2f pos;

 protected:

  v2f norm;

  objtype myType;

 public: 
  CrowdObject( Json::Value c );
  CrowdObject( );
  //returns whether the object is visible within the vision rectangle presented
  virtual bool isVisible( v2f pos, v2f dir, float vislength, float viswidth);

  //returns the norm of the object in the return parameter
  virtual void getNorm( v2f ret ); 
  
  //gets the distance from the position argument to the object
  virtual float getDistance( v2f pos );

  //gets the direction vector from the calling position to the object
  virtual void getDirection( v2f pos, v2f res);

  //gets the type of object
  int getType();

  //returns a zero-vector for all non-agent CrowdObjects
  virtual void getVelocity( v2f ret );

  //functions needed by Agent - leaky abstractions
  virtual void getPos( v2f ret) {v2fMult( ret, 0.0, ret ); return;}
  virtual float getRadius() {return 0.0;}

};

#endif
