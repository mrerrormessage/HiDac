#ifndef _AGENT_H_
#define _AGENT_H_


#include "CrowdObject.h"
#include <math.h>
#include <vector>
#include <jsoncpp/value.h>
#include <iostream>
#include "constants.h"


class Agent : public CrowdObject { 
 private:
  //weights - avoidance weights
  float attractorWeight;
  float wallWeight;
  float obstacleWeight;
  float agentWeight;

  float fallenWeight;

  float acceleration;
  float maxVelocity;

  //the "size" of the agent
  float radius; 

  //states whether an agent is colliding with another
  bool isColliding;

  //whether agent is stopping or waiting. Not used at present. 
  bool stopping; 
  bool waiting; 

  //fallen-agent-avoidance parameter
  float Beta;

  //position - will assume continuous for now
  v2f pos;

  v2f vel;

  //norm and it's setter method are inherited from CrowdObject
  //need to copy something into norm
  v2f force;

  
  //this is needed for repulsion forces. Until those are implemented it will be set to <0.0, 0.0> in calculateForces
  v2f repelForce;

  //vision range - to calculate a vision rectangle, look out vislong units along velocity vector, then look by viswide / 2 units. 
  float vislong;
  float viswide;

  //lists of visible objects
  std::vector<CrowdObject> visobjects;

  //Attractor
  CrowdObject attractor;
  
  float computeVel( float deltaT);
  float computeAlpha( );
  void computeFallen( v2f ret);

 public: 
  Agent();
  Agent(Json::Value a);
  Json::Value getJson();
  void print();
  objtype getType();
  void getVelocity( v2f ret );
  void setVelocity( v2f set );
  float getSpeed( );
  void getDirection( v2f get); //should return a normalized velocity vector

  void getPos( v2f ret );
  void setPos( v2f set );
  
  //tells another agent whether it is visible
  //these are inherited from CrowdObject.h
  bool isVisible( v2f pos, v2f dir, float vislength, float viswidth);
  float getDistance( v2f pos );
  void getDirection( v2f pos, v2f res);


  void calculateForces( );

  void applyForces( float deltaT );

};

#endif
