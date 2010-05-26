#include "CrowdWorld.h"

CrowdWorld::CrowdWorld(){

}

CrowdWorld::~CrowdWorld(){

}

CrowdWorld * CrowdWorld::getInstance(){
  if(instance == NULL){
    instance = new CrowdWorld();
  }
  return instance;
}

CrowdWorld * CrowdWorld::destroyInstance(){
  if(instance != NULL)
    delete instance;
  return 0;
}


  //updates each agent with visibility and collision information
void CrowdWorld::updateAgents(){
  for( std::vector<Agent>::iterator it = agentList.begin();
       it != agentList.end();
       it++ ){
    

  }
}

  //calcs forces for each agent
void CrowdWorld::calcForces(){
  for( std::vector<Agent>::iterator it = agentList.begin();
       it != agentList.end();
       it++ ){
    it->calculateForces();
  }
}
  
  //applies forces for each agent
void CrowdWorld::stepWorld( float deltaT ){
  for( std::vector<Agent>::iterator it = agentList.begin();
       it != agentList.end();
       it++ ){
    it->applyForces(deltaT);
  }
}
