#include "CrowdObject.h"
#include "Agent.h"
#include "Wall.h"
#include "Render.h"
#include <vector>
#include <jsoncpp/value.h>

class CrowdWorld {
 private:
  std::vector<Agent * > agentList;
  std::vector<CrowdObject * > objectList;
  
  void createNewObject(Json::Value v);
  
 public:
  //build from JSON value
  CrowdWorld();
  CrowdWorld( Json::Value w );
  ~CrowdWorld();
  
  //updates each agent with visibility and collision information
  void updateAgents();

  //calcs forces for each agent
  void calcForces();
  
  //applies forces for each agent
  void stepWorld(float deltaT);

  //output functions
  void print();
  void render();
};
