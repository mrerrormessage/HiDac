#include "CrowdObject.h"
#include "Agent.h"
#include "Wall.h"
#include <vector>

class CrowdWorld {
 private:
  std::vector<Agent> agentList;
  std::vector<CrowdObject> objectList;
  
  //singleton
  CrowdWorld();
  ~CrowdWorld();
  CrowdWorld * instance;

 public:
  CrowdWorld* getInstance();
  CrowdWorld* destroyInstance();
  
  //updates each agent with visibility and collision information
  void updateAgents();

  //calcs forces for each agent
  void calcForces();
  
  //applies forces for each agent
  void stepWorld(float deltaT);
};
