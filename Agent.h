#ifndef _AGENT_H_
#define _AGENT_H_


#include "CrowdObject.h"
#include <math.h>
#include <cmath>
#include <vector>
#include <jsoncpp/value.h>
#include <iostream>
#include <cstdlib>
#include "constants.h"
#include "Wall.h"

class Agent : public CrowdObject { 
 private:
  //weights - avoidance weights
  float attractorWeight;
  float wallWeight;
  float obstacleWeight;
  float agentWeight;

  float fallenWeight;

  float personalSpace;

  float acceleration;
  float maxVelocity;

  //the "size" of the agent
  float radius; 

  //states whether an agent is colliding with another
  bool isColliding;
  std::vector<CrowdObject *> collideObjects;

  //lists of visible objects
  //should probably be pointers, will change when needed 
  std::vector<CrowdObject *> visObjects;

  //whether agent is stopping or waiting. 
  bool stopping; 
  int stoptime;
  bool waiting; 

  //whether agent is panicked - idea: make quantitative
  bool panic;

  //fallen-agent-avoidance parameter
  float Beta;

  //position - will assume continuous for now
  v2f pos;

  //velocity
  v2f vel;

  //norm and it's setter method are inherited from CrowdObject
  //need to copy something into norm
  v2f force;

  
  //this is needed for repulsion forces. Until those are implemented it will be set to <0.0, 0.0> in calculateForces
  v2f repelForce;
  void calculateRepelForce();

  //vision range - to calculate a vision rectangle, look out vislong units along velocity vector, then look by viswide / 2 units. 
  float vislong;
  float viswide;

  std::string mesh;

  //Attractor
  CrowdObject attractor;
  
  float computeVel( float deltaT);
  float computeAlpha( );
  void computeFallen( v2f ret);

 public: 
  Agent();
  Agent(Json::Value a);
  ~Agent();
  Json::Value getJson();
  void print();
  int getType();
  void getVelocity( v2f ret );
  void setVelocity( v2f set );
  float getSpeed( );
  void getDirection( v2f get); //should return a normalized velocity vector

  float getPersonalSpace();
  float getRadius();

  void getPos( v2f ret );
  void setPos( v2f set );

  std::string getMesh(){ return mesh; }

  void getNorm( v2f get );
  //tells another agent whether it is visible
  //these are inherited from CrowdObject.h
  bool isVisible( v2f pos, v2f dir, float vislength, float viswidth);
  
  float getDistance( v2f pos );
  void getDirection( v2f pos, v2f res);
 
  void calcAgentForce( CrowdObject::CrowdObject * a , v2f ret);
  void calculateForces( );

  void applyForces( float deltaT );

  //functions to update visibility and collision vectors
  void checkCollide( CrowdObject::CrowdObject * c );
  void checkVisible( CrowdObject::CrowdObject * c );

  //function to 'reset' at the end of a simulation step 
  void reset();
};

#endif
